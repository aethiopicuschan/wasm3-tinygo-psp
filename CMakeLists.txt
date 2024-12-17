cmake_minimum_required(VERSION 3.10)
project(Wasm3PSPExample C)

set(CMAKE_C_STANDARD 99)

# For Wasm3
include_directories(~/wasm3/include)
link_directories(~/wasm3/lib)

# Modules
set(PSP_LIBS
    pspdebug
    pspge
    pspdisplay
    pspctrl
    pspgu
    psprtc
)

# For PSP
add_executable(hello_psp main.c)

# Add suffix .elf
set_target_properties(hello_psp PROPERTIES SUFFIX ".elf")

# Link Wasm3
target_link_libraries(hello_psp m3 ${PSP_LIBS})

# Create EBOOT.PBP
add_custom_command(TARGET hello_psp POST_BUILD
    # Dummy files(Empty files)
    COMMAND touch ICON0.PNG
    COMMAND touch ICON1.PMF
    COMMAND touch PIC0.PNG
    COMMAND touch PIC1.PNG
    COMMAND touch SND0.AT3
    COMMAND touch DATA.PSAR

    COMMAND psp-fixup-imports hello_psp.elf
    COMMAND mksfo hello_psp hello_psp.sfo

    # Output: EBOOT.PBP
    COMMAND pack-pbp EBOOT.PBP hello_psp.sfo ICON0.PNG ICON1.PMF PIC0.PNG PIC1.PNG SND0.AT3 hello_psp.elf DATA.PSAR

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
