#include "core.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, const char **argv, char **env) {

    // --- PATCH 1: MAIN EXECUTION BOUNDARY BYPASS ---
    if (argc > 1 && argv != NULL && (strstr(argv, "t6sp.exe") || strstr(argv, "t6mp.exe") || strstr(argv, "t6zm.exe"))) {
        setenv("BOX64_DYNAREC_FASTROUND", "1", 1);
        setenv("BOX64_DYNAREC_WEAKBARRIER", "0", 1);
    }

    x64emu_t* emu = NULL;
    elfheader_t* elf_header = NULL;
    if (initialize(argc, argv, env, &emu, &elf_header, 1)) {
        return -1;
    }

    // --- PATCH 2: WOW64 STACK FRAMING SYSTEM ALIGNMENT ---
    if (emu && emu->regs && emu->regs[_SP].qword > 0xFFFFFFFF) {
        emu->regs[_BP].qword &= 0xFFFFFFFF; 
    }

    return emulate(emu, elf_header);
}
