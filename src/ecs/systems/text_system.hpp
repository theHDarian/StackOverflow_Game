#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include <map>
#include <string>
#include "common.hpp"
#include "render_system_init.hpp"
#include "tiny_ecs_registry.hpp"

std::vector<std::string> getTokenizedText(std::string text);

class TextSystem {
public:
    TextSystem();
    ~TextSystem();

    int initFreetypeLib();
    void renderMenuUIText();
    void renderGameUIText();
    void renderDialogueUIText();

private:
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    GLuint program;
    // projection matrix; may consider using same one as render system instead
    // note: original render system has bottom right be (window width, window height)
    // but this tutorial's projection matrix has top right be (Window width, window height)
    glm::mat4 projection;
    std::unordered_map<std::string, std::vector<std::string>> uiTexts;

    void renderText(std::string text, float x, float y, float scale, glm::vec3 color, vec2 topRightBound, vec2 bottomLeftBound, std::string textName = "");
    // lazy overloading
    void renderText(std::vector<std::string> tokenizedText, float x, float y, float scale, glm::vec3 color, vec2 topRightBound, vec2 bottomLeftBound);
    vec2 renderWord(std::string text, float x, float y, float scale, glm::vec3 color);
    void loadText();
};