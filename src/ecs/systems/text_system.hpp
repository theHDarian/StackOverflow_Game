#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include <map>
#include <string>
#include "common.hpp"
#include "render_system_init.hpp"
#include "tiny_ecs_registry.hpp"
#include "render_system.hpp"

void parseBodyDecorations(std::string dialogueBody, std::string& parsedBody, std::vector<TextDecorationSpan>& decorationSpans);
std::vector<std::string> getTokenizedText(std::string text);
std::vector<std::string> getFormattedText(std::vector<std::string> tokenizedText, float scale, TextAlignment alignment, vec2 textPosition, vec2 topRightBound, vec2 bottomLeftBound);
bool textOverflowed(std::string text, float scale, TextAlignment alignment, vec2 textPosition, vec2 topRightBound, vec2 bottomLeftBound);
int getIndexLine(std::vector<std::string> lines, int charIndex);

const float FONT_ADJUST_FACTOR = 1.50;
const std::string FONT_FILE = "DepartureMono-Regular.otf";
const float DEFAULT_FONT_SIZE = 48.f;

// manually change this for different fonts for now

// adjust factor 2.5 - for bytebounce, monogram
// adjust factor 1.5 - for chicago, home video, monocraft, departureMono-Regular

class TextSystem {
public:
    TextSystem();
    ~TextSystem();

    int initFreetypeLib();
    void renderMenuUIText();
    void renderGameOverlayUIText();

    void renderGameUIText();

    void renderDialogueUIText();
    void renderMenuOverlayUIText();

private:
    struct Character {
        int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    GLuint program;
    std::vector<mat4> transforms;
    std::vector<vec4> colors;
    std::vector<int> letterMap;
    GLuint textureArray;
    int INSTANCED_ARRAY_SIZE = 100; // note: tutorial used 400, but my pc can only handle 100 lol
    // if it's too big: will cause a linking error when trying to compile shader files
    
    // projection matrix; may consider using same one as render system instead
    // note: original render system has bottom right be (window width, window height)
    // but this tutorial's projection matrix has top right be (Window width, window height)
    glm::mat4 projection;

    void renderText(TextRenderRequest& request, Entity entity, bool isUI = true);
    void drawInstancedText(int length);
    float getTextLength(std::string text, float scale);
    void drawAStatusText(Entity& entity, HPBarUI& hpBar, TextRenderRequest& textReq, vec2 startingPos, vec2 offset, vec2 iconSize, std::string count);
};