// ref: https://learnopengl.com/In-Practice/Text-Rendering

#include "text_system.hpp" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

TextSystem::TextSystem() {

}

TextSystem::~TextSystem() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (program) {
        glDeleteProgram(program);
    }
}

int TextSystem::initFreetypeLib() {
    // set up projection matrix
    WindowState& windowState = registry.windowStates.components[0];
    projection = glm::ortho(0.0f, static_cast<float>(windowState.width), 0.0f, static_cast<float>(windowState.height));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = font_path("Pixeled.ttf").c_str();
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // just load first 128 characters for now
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy freetype once finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    gl_has_errors();

    // set up VAO for text rendering specifically
    // but shared VAO with render system might be easier
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // load in projection matrix - only need to set once
    bool is_valid = loadEffectFromFile(shader_path("text.vs.glsl").c_str(), shader_path("text.fs.glsl").c_str(), program);
    assert(is_valid && (GLuint)program != 0);
    gl_has_errors();
    glUseProgram(program);
    unsigned int projectionLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    gl_has_errors();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();
    glBindVertexArray(0); 
    gl_has_errors();

    return 1;
}

// what would API for text look like?
vec2 TextSystem::renderWord(std::string text, float x, float y, float scale, glm::vec3 color) {
    float copyX = x;
    vec2 textEndPos = { x, y };
    
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        // newline addition referenced from https://www.youtube.com/watch?v=S0PyZKX4lyI
        if (*c == '\n') {
            y -= ((ch.Size.y)) * 1.3 * scale;
            x = copyX;
            textEndPos = { x, y };
        }
        else {
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            // interesting that scale is multiplied here and not in the shader?
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            gl_has_errors();
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            gl_has_errors();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            gl_has_errors();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            gl_has_errors();
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            gl_has_errors();
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

            // very inefficient way to get end position right now
            textEndPos = { xpos + w, y };
        }

    }
    return textEndPos;
}

void TextSystem::renderText(std::string text, float x, float y, float scale, glm::vec3 color, vec2 topRightBound, vec2 bottomLeftBound)
{
    // activate corresponding render state, hard code to just 1 text rendering program for now
    // (can also pass shader itself as parameter and use that)
    glUseProgram(program);
    gl_has_errors();

    // can also consider adding a transform matrix here
    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    gl_has_errors();

    // iterate through all characters
    // need to keep track of words then...

    // tokenize string by space (should maintain \n!)
    // ref for tokenizing: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
    std::vector<std::string> tokenizedText;
    std::string space = " ";
    std::string newLine = "\n";
    // consider adding other delimiters, like \tab, etc

    std::string str = "";
    for (char c : text) {
        if (c == ' ' && str.length() > 0) {
            tokenizedText.push_back(str + space + space); // for some reason, need to add 2 spaces
            str = "";
        }
        else if (c == '\n') {
            tokenizedText.push_back(str);
            tokenizedText.push_back(newLine);
            str = "";
        }
        else {
            str += c;
        }
    }
    tokenizedText.push_back(str);

    vec2 textPos = { x, y };
    for (std::string word : tokenizedText) {
        // calculate the length of the word to determine if need to insert new line 
        // copy pasted from word render for now
        std::string::const_iterator c;
        float xpos = textPos.x;
        float ypos = textPos.y;

        for (c = word.begin(); c != word.end(); c++)
        {
            Character ch = Characters[*c];
            xpos += x + ch.Bearing.x * scale;
            ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        }

        // compare screen size
        // may use "text box" borders instead, but use screen for now
        // consider also adding padding
        WindowState& ws = registry.windowStates.components[0];
        if (xpos > topRightBound.x || xpos < bottomLeftBound.x) {
            textPos.y -= ((Characters[0].Size.y)) * 1.3 * scale;
            textPos.x = x;
        }
        if (ypos > topRightBound.y || ypos < bottomLeftBound.y) {
            // do nothing for now, unless want to write text that goes up and down
        }

        textPos = renderWord(word, textPos.x, textPos.y, scale, color);
        // check if new line should be applied
        if (newLine.compare(word) == 0) {
            textPos.x = x;
        }
    }

    std::cout << std::endl;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    gl_has_errors();
}

void TextSystem::renderMenuUIText() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity& entity : registry.menuUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        // for now, tie text visibility to entitie's render visibility
        // but assumption may not always hold
        if (registry.renderRequests.get(entity).show)
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound);
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::renderGameUIText() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity entity : registry.gameUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            // rendering every frame is slower than render when change happens?
            std::string textString = textReq.text;
            if (registry.stackUI.has(entity)) {
                StackCompile& stack = registry.stackCompile.get(registry.players.entities[0]);
                textString = "Stack: " + std::to_string(stack.currStack.size()) + " / " + std::to_string(stack.baseStackSize);
            }
            renderText(textString, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound);
        }
    }

    glBindVertexArray(0);
    gl_has_errors();

}

void TextSystem::renderDialogueUIText() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity entity : registry.dialogueUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show)
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound);
    }

    glBindVertexArray(0);
    gl_has_errors();
}