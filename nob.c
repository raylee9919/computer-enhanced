// Comment this to disable ANSI color output.
#define NOB_COLORS

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "deps/nob.h"

#define BUILD_DIR "./build/"


int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    nob_log(NOB_INFO, "Running in '%s'", nob_get_current_dir_temp());

    nob_mkdir_if_not_exists(BUILD_DIR);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cl", "-nologo", "-std:c++17", "-Od", "-W4", "-Z7", "./src/page_fault.cpp", "-Fe:./build/page_fault.exe");
    nob_cmd_append(&cmd, "-DNOMINMAX");
    nob_cmd_append(&cmd, "-link", "kernel32.lib");
    nob_log(NOB_INFO, "Building project");
    if (!nob_cmd_run_sync(cmd)) {
        return 1;
    }

    nob_log(NOB_INFO, "Build successful!");
}
