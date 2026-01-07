// C++
// File: `src/platform/winmain_shim.cpp`
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Declare your real main implemented elsewhere
extern "C" int main(void);

// Forward WinMain to main so the linker is satisfied when using WIN32 subsystem
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}
#endif
