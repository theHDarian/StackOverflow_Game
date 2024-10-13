// ref: https://learnopengl.com/In-Practice/Text-Rendering

#include "text_system.hpp" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "render_system_init.hpp"
#include "tiny_ecs_registry.hpp"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint text_VAO, VBO;
GLuint program;
// projection matrix; may consider using same one as render system instead
// note: original render system has bottom right be (window width, window height)
// but this tutorial's projection matrix has top right be (Window width, window height)
glm::mat4 projection;

int initFreetypeLib() {
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
    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();
    glBindVertexArray(0); 
    gl_has_errors();

    bool is_valid = loadEffectFromFile(shader_path("text.vs.glsl").c_str(), shader_path("text.fs.glsl").c_str(), program);
    assert(is_valid && (GLuint)program != 0);
    gl_has_errors();

    return 1;
}

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // activate corresponding render state, hard code to just 1 text rendering program for now
    // (can also pass shader itself as parameter and use that)
    glUseProgram(program);
    gl_has_errors();

    unsigned int projectionLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    gl_has_errors();

    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    gl_has_errors();
    glActiveTexture(GL_TEXTURE0);
    gl_has_errors();
    glBindVertexArray(text_VAO);
    gl_has_errors();

    float copyX = x;

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        // newline addition referenced from https://www.youtube.com/watch?v=S0PyZKX4lyI
        if (*c == '\n') {
            y -= ((ch.Size.y)) * 1.3 * scale;
            x = copyX;
        }
        else {
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

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
        }
       
    }
    glBindVertexArray(0);
    gl_has_errors();
    glBindTexture(GL_TEXTURE_2D, 0);
    gl_has_errors();

    glDisable(GL_BLEND);
}