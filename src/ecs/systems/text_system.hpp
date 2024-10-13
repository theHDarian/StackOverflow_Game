#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include "common.hpp"
#include <map>
#include <string>

int initFreetypeLib();
void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);