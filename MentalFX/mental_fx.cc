#include "mental_glfw_window.h"

int main(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

    MFW_C_Window window_{};
    window_.run();

    return 0;
}
