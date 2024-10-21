#include <string>

// mostly put these here so they can be shared with text render system
bool gl_compile_shader(GLuint shader);
bool loadEffectFromFile(const std::string& vs_path, const std::string& fs_path, GLuint& out_program);