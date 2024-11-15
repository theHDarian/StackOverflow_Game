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

    // need to delete every single glyph here too!
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
    std::string font_name = font_path("ByteBounce.ttf").c_str();
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

    /*FT_Set_Pixel_Sizes(face, 0, 48);*/ // set to 0 for x, 48 for y. 0 means any x size is ok.
    // which makes sense because we have narrow & wide letters
    // but since we are using texture arrays, we need them to be the same size and have wasted space
    FT_Set_Pixel_Sizes(face, 256, 256);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    glGenTextures(1, &textureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    // recall how a texture array is set up; reference one down below in tutorial
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    // just load first 128 characters for now
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // first ascii char = first array
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, int(c),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // generate texture per character
        // let's try using an array instead!
        /*
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
        );*/
        // set texture options

        // now store character for later use
        // first = index to our letter
        Character character = {
            int(c),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // destroy freetype once finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    gl_has_errors();

    // let's use this as our triangle strip
    // to draw our textures on, as opposed to quad
    GLfloat vertex_data[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };

    // set up our lettermaps and transforms
    // 256 is just some size
    for (int i = 0; i < INSTANCED_ARRAY_SIZE; i++) {
        letterMap.push_back(0);
        transforms.push_back(mat4(1.0f));
    }

    // set up VAO for text rendering specifically
    // but shared VAO with render system might be easier
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // static b/c we're keeping this vertex
    // also note dynamic draw is more costly than static. try to keep things static (mentioned somewhere)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    // og: 6 * 4 = 6 vertices of 4 floats
    // not needed because: we only need x and y pos, not z, so we really need vec2
    // 6 vertices: 2 triangles to make quad. can change to 4 to make triangle strip instead.

    // load in projection matrix - only need to set once
    bool is_valid = loadEffectFromFile(shader_path("text.vs.glsl").c_str(), shader_path("text.fs.glsl").c_str(), program);
    assert(is_valid && (GLuint)program != 0);
    gl_has_errors();
    glUseProgram(program);
    unsigned int projectionLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    gl_has_errors();

    glEnableVertexAttribArray(0);
    // also change our stried
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    // 2 is our vertex coord size
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // our stride will be full length instead
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();
    glBindVertexArray(0); 
    gl_has_errors();

    // load texts
    loadText();

    return 1;
}

// what would API for text look like?
vec2 TextSystem::renderWord(std::string text, float x, float y, float scale, glm::vec3 color) {
    // temp put here to readjust sizes btween diff fonts
    scale *= 2.50; // for bytebounce
    scale *= 48.0f / 256.0f; // so letters still look as same as before

    float copyX = x;
    vec2 textEndPos = { x, y };

    // just bind once
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // which num are we on now?
    // remember we don't count newlines and spaces, since avoiding drawing them!
    int currentIndex = 0;
    
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        
        Character ch = Characters[*c];

        // newline addition referenced from https://www.youtube.com/watch?v=S0PyZKX4lyI
        if (*c == '\n') {
            y -= (Characters[65].Size.y) * 2.0 * scale;
            x = copyX;
            textEndPos = { x, y };
        }
        else {
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (256 - ch.Bearing.y) * scale;

            // interesting that scale is multiplied here and not in the shader?
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            if (*c == ' ') { // skip "blank space characters" by not actually drawing them
                x += (ch.Advance >> 6) * scale;
                textEndPos = { xpos + w, y };
                continue;
            }

            // set up all our stuff here, and pass it in at once at end
            // instead, set up matrix we'll use to transform our generic triangle strip
            // this will be where we want to draw our text (translate) and how big (Scale)
            // but since generic rect = 0 and 1, need to also put in actual char size data for scale
            // remember we need to take text bearings into account too
            transforms[currentIndex] = translate(mat4(1.0f), vec3(xpos, ypos, 0))
                * glm::scale(mat4(1.0f), vec3(256 * scale, 256 * scale, 0)); // 256 is size of each char
            // which letter are we drawing?
            letterMap[currentIndex] = ch.TextureID;
            

            // update VBO for each character
            // let's avoid constantly creating this constantly...
            //float vertices[6][4] = {
            //    { xpos,     ypos + h,   0.0f, 0.0f },
            //    { xpos,     ypos,       0.0f, 1.0f },
            //    { xpos + w, ypos,       1.0f, 1.0f },

            //    { xpos,     ypos + h,   0.0f, 0.0f },
            //    { xpos + w, ypos,       1.0f, 1.0f },
            //    { xpos + w, ypos + h,   1.0f, 0.0f }
            //};

            // render glyph texture over quad
            //glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            //gl_has_errors();
            // update content of VBO memory
            //glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //gl_has_errors();
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)
            //gl_has_errors();
            
            // render quad
            // we want to be able to do instanced call -- call shader multiple times at once
            // ie draw whole sentence at once
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            
            //gl_has_errors();
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

            // very inefficient way to get end position right now
            textEndPos = { xpos + w, y };
            // update index of drawn char
            currentIndex++;

            // put here for now
            // we don't want to draw more than we can fit at once, so just break for now
            if (currentIndex == INSTANCED_ARRAY_SIZE) {
                drawInstancedText(currentIndex);
                currentIndex = 0; // now can render more than 100 text at a time
            }
        }
    }

    drawInstancedText(currentIndex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();

    return textEndPos;
}

// length = how many rendering at once
void TextSystem::drawInstancedText(int length) {
    if (length > 0) {
        unsigned int transformLoc = glGetUniformLocation(program, "transforms");
        glUniformMatrix4fv(transformLoc, length, GL_FALSE, &transforms[0][0][0]); // b/c this is a vector of mat4s, need this many 0s??
        gl_has_errors();

        unsigned int letterMapLoc = glGetUniformLocation(program, "letterMap");
        glUniform1iv(letterMapLoc, length, &letterMap[0]);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
    }
}

std::vector<std::string> getTokenizedText(std::string text) {
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
    return tokenizedText;
}

void TextSystem::renderText(std::vector<std::string> tokenizedText, float x, float y, float scale, glm::vec3 color,
    vec2 topRightBound, vec2 bottomLeftBound) {
    // activate corresponding render state, hard code to just 1 text rendering program for now
// (can also pass shader itself as parameter and use that)
    glUseProgram(program);
    gl_has_errors();

    // can also consider adding a transform matrix here
    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    gl_has_errors();

    std::string newLine = "\n";
    vec2 textPos = { x, y };

    for (std::string word : tokenizedText) {
        // calculate the length of the word to determine if need to insert new line 
        float xpos = textPos.x;
        float ypos = textPos.y;

        // approximate word size as opposed to looping
        xpos += (Characters[65].Size.x + Characters[65].Bearing.x + 1.0f) * word.length() * scale * 2.50;

        // compare with text box size
        if (xpos > topRightBound.x /*|| xpos < bottomLeftBound.x*/) {
            textPos.y -= ((Characters[65].Size.y)) * 2.0 * 2.50 * scale;
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

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    gl_has_errors();
}

void TextSystem::renderText(std::string text, float x, float y, float scale, glm::vec3 color, 
    vec2 topRightBound, vec2 bottomLeftBound, std::string textName)
{
    // activate corresponding render state, hard code to just 1 text rendering program for now
    // (can also pass shader itself as parameter and use that)
    glUseProgram(program);
    gl_has_errors();

    // can also consider adding a transform matrix here
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    //glActiveTexture(GL_TEXTURE0);
    gl_has_errors();

    std::string newLine = "\n";
    std::vector<std::string> tokenizedText;
    vec2 textPos = { x, y };

    if (uiTexts.count(textName) > 0) {
        tokenizedText = uiTexts[textName];
        //std::cout << "found text for " << textName << std::endl;
    }
    else {
        tokenizedText = getTokenizedText(text);
    }

    for (std::string word : tokenizedText) {
        // calculate the length of the word to determine if need to insert new line 
        float xpos = textPos.x;
        float ypos = textPos.y;

        // approximate word size as opposed to looping
        xpos += (Characters[65].Size.x + Characters[65].Bearing.x + 1.0f) * word.length() * scale * 2.50;

        // compare with text box size
        if (xpos > topRightBound.x /*|| xpos < bottomLeftBound.x*/) {
            textPos.y -= ((Characters[65].Size.y)) * 2.0 * 2.50 * scale;
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
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
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
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound,
                textReq.textName);
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::renderMenuOverlayUIText() {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity& entity : registry.menuOverlayUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show)
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound,
                textReq.textName);
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::renderGameUIText() {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity entity : registry.gameUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound,
                textReq.textName);
        }
    }

    glBindVertexArray(0);
    gl_has_errors();

}

void TextSystem::renderDialogueUIText() {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity entity : registry.dialogueUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound,
                    textReq.textName);
            }
        }
            
    }

    // workaround for now instead of having text have its own show
    for (Entity entity : registry.dialogueChoices.entities) {
        auto& textReq = registry.textRenderRequests.get(entity);
        renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound,
            textReq.textName);
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::loadText() {
    std::string uiType = "uiText";
    std::string filename = dialogue_path(uiType + ".txt").c_str();
    std::ifstream entity_file(filename);
    std::string uiName;
    std::vector<std::string> tokenizedText;

    if (entity_file.is_open())
    {
        std::string line;

        while (!entity_file.eof())
        {
            std::getline(entity_file, line);

            if (line.length() > 0 && line[0] != '#')
            {
                // split line different based on what first word is
                std::string action = line.substr(0, line.find_first_of(" "));


                if (action.compare("UI") == 0) {
                    // new ui text, so place all prev lines into map, unless this is the first one
                    if (tokenizedText.size() > 0) {
                        uiTexts.insert({ uiName, tokenizedText });
                        tokenizedText.clear();
                    }

                    std::stringstream ss_line(line);
                    ss_line >> action >> uiName;
                }
                else { // this is just a body of text
                    // need to manually add \n back into strings... use this until can think of better way
                    //ref: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c?page=1&tab=scoredesc#tab-top
                    std::string delim = "\\n";
                    std::string uiTextBody = "";
                    auto start = 0U;
                    auto end = line.find(delim);
                    while (end != std::string::npos)
                    {
                        uiTextBody += line.substr(start, end - start) + '\n';
                        start = end + delim.length();
                        end = line.find(delim, start);
                    }
                    uiTextBody += line.substr(start, end);

                    // tokenize string by space (should maintain \n!)
                    // ref for tokenizing: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
                    std::string space = " ";
                    std::string newLine = "\n";
                    // consider adding other delimiters, like \tab, etc

                    std::string str = "";
                    // this is very expensive!!
                    // TODO: pre-tokenize all text before loading game
                    for (char c : uiTextBody) {
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
                    tokenizedText.push_back(newLine);
                }

            }
        }
        entity_file.close();
        uiTexts.insert({ uiName, tokenizedText });
    }
    else
    {
        std::cout << "ERROR: failed to open file: " << filename << std::endl;
    }
}