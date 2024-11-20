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

    return 1;
}

/*
ref: https://learnopengl.com/In-Practice/Text-Rendering
optimizations made based on: https://www.youtube.com/watch?v=S0PyZKX4lyI
gist of changes referenced:
- newline character
- not actually drawing out spaces
- using a fixed texcoord with static draws instead (less costly than dynamic) and using a transform matrix to transform texcoods
- using instanced rendering to draw a bunch at once. limiting draw calls = limiting costly communications between cpu and gpu
- avoid binding every time too by using a texture array of characters

additional things added:
- adapted to consider text wrapping
- tokenized text beforehand to help with text wrapping
*/
void TextSystem::renderText(std::vector<std::string> tokenizedText, float x, float y, float scale, glm::vec3 color, 
    vec2 topRightBound, vec2 bottomLeftBound, TextAlignment alignment, bool isUI) {
    // temp put here to readjust sizes btween diff fonts
    scale *= FONT_ADJUST_FACTOR;
    scale *= 48.0f / 256.0f; // so letters still look as same as before after changing texture sizes

    float copyX = x;

    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);

    // which num char are we on now?
    // remember we don't count newlines and spaces, since avoiding drawing them!
    int currentIndex = 0;
    Motion motion = Motion(); // placeholder for text motion info

    for (std::string text : tokenizedText) {

        // approximate next word length and compare with text box size
        if ((x + (Characters[65].Size.x + Characters[65].Bearing.x + 0.5f) * text.length() * scale) > topRightBound.x/*|| xpos < bottomLeftBound.x*/) {
            y -= ((Characters[65].Size.y)) * 2.0 * scale;
            x = copyX;
        }
        if (y > topRightBound.y || y < bottomLeftBound.y) {
            // do nothing for now, unless want to write text that goes up and down
        }
        
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            if (*c == '\n') {
                y -= (Characters[65].Size.y) * 2.0 * scale;
                x = copyX;
            }
            else {
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (256 - ch.Bearing.y) * scale;

                if (*c == ' ') { // skip "blank space characters" by not actually drawing them
                    x += (ch.Advance >> 6) * scale;
                    continue;
                }

                // set up all our stuff here, and pass it in at once at end
                // set up matrix we'll use to transform our generic triangle strip
                // this will be where we want to draw our text (translate) and how big (Scale)
                // but since generic rect = 0 and 1, need to also put in actual char size data for scale
                // remember we need to take text bearings into account too
                motion.position = { xpos, ypos };
                motion.scale = { 256 * scale, 256 * scale };
                if (isUI) {
                    transforms[currentIndex] = createNormalModel(motion, vec2(0));
                }
                else {
                    // need to do more stuff to make position work, unfortunately...
                    WindowState& windowState = registry.windowStates.components[0];
                    transforms[currentIndex] = createFollowCameraModelText(motion, vec2(0));
                }

                //transforms[currentIndex] = translate(mat4(1.0f), vec3(xpos, ypos, 0))
                //    * glm::scale(mat4(1.0f), vec3(256 * scale, 256 * scale, 0)); // 256 is size of each char
                // which letter are we drawing?
                letterMap[currentIndex] = ch.TextureID;

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

                // update index of drawn char
                currentIndex++;

                // we don't want to draw more than we can fit at once, so issue a draw call when full
                if (currentIndex == INSTANCED_ARRAY_SIZE) {
                    drawInstancedText(currentIndex);
                    currentIndex = 0;
                }
            }
        }
    }

    drawInstancedText(currentIndex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    gl_has_errors();
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
    std::string space = " ";
    std::string newLine = "\n";
    std::vector<std::string> tokenizedText;
    // consider adding other delimiters, like \tab, etc

    std::string str = "";
    for (char c : text) {
        if (c == ' ' && str.length() > 0) {
            tokenizedText.push_back(str + space); // for some reason, need to add 2 spaces
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

void TextSystem::renderText(std::string text, float x, float y, float scale, glm::vec3 color, 
    vec2 topRightBound, vec2 bottomLeftBound, TextAlignment alignment, bool isUI)
{
    // consider saving this in the future w/ a dirty bit if it gets expensive
    // or consider a universal string to tokenized string map w/ hash, but would hashing that also get expensive?
    std::vector<std::string> tokenizedText = getTokenizedText(text);
    renderText(tokenizedText, x, y, scale, color, topRightBound, bottomLeftBound, alignment, isUI);
}

void TextSystem::renderMenuUIText() {
    // no longer need to turn these on again now that we render BEFORE post processing 
    // (which turns off blending)
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    for (Entity& entity : registry.menuUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        // for now, tie text visibility to entitie's render visibility
        // but assumption may not always hold
        if (registry.renderRequests.get(entity).show)
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
    }

    // workaround for now instead of having text have its own show
    for (Entity entity : registry.menuChoices.entities) {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (textReq.tokenizedText.size() > 0) {
            renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
        }
        else {
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
        }
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::renderMenuOverlayUIText() {
    glBindVertexArray(VAO);

    for (Entity& entity : registry.menuOverlayUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show)
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
    }

    glBindVertexArray(0);
    gl_has_errors();
}

void TextSystem::renderGameUIText() {
    glBindVertexArray(VAO);

    for (Entity entity : registry.gameOverlayUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment, false);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment, false);
            }
        }
    }

    for (Entity entity : registry.gameUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
        }
    }

    glBindVertexArray(0);
    gl_has_errors();

}

void TextSystem::renderDialogueUIText() {
    glBindVertexArray(VAO);

    for (Entity entity : registry.dialogueUITexts.entities)
    {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (registry.renderRequests.get(entity).show) {
            if (textReq.tokenizedText.size() > 0) {
                renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
            else {
                renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
            }
        }
            
    }

    // workaround for now instead of having text have its own show
    for (Entity entity : registry.dialogueChoices.entities) {
        auto& textReq = registry.textRenderRequests.get(entity);
        if (textReq.tokenizedText.size() > 0) {
            renderText(textReq.tokenizedText, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
        }
        else {
            renderText(textReq.text, textReq.x, textReq.y, textReq.scale, textReq.color, textReq.topRightBound, textReq.bottomLeftBound, textReq.alignment);
        }
    }

    glBindVertexArray(0);
    gl_has_errors();
}