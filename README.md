# Wasm3 + TinyGo on PSP

[![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen?style=flat-square)](/LICENSE)

![psp](https://github.com/user-attachments/assets/4d631e63-5412-47ba-a2e1-5a3262cabc3b)

## Required

- [PSPDEV](https://pspdev.github.io)
- [TinyGo](https://tinygo.org)
- [Wasm3](https://github.com/wasm3/wasm3)
  - You need compile it with PSP toolchain
  - or You can use my forked version [wasm3-for-psp](https://github.com/aethiopicuschan/wasm3-for-psp)

## How to build

### Wasm3

Basically, you can use the aforementioned forked version, but just in case it becomes outdated, I’m including this information as a precaution.

1. `git clone git@github.com:wasm3/wasm3.git`
2. `cd wasm3`
3. Append the following code to `CMakeLists.txt`

```cmake
install(DIRECTORY "${PROJECT_SOURCE_DIR}/source/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h")

install(TARGETS m3
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
```

4. Create `psp-toolchain.cmake`

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)

set(PSPDEV $ENV{PSPDEV})

set(CMAKE_C_COMPILER ${PSPDEV}/bin/psp-gcc)
set(CMAKE_CXX_COMPILER ${PSPDEV}/bin/psp-g++)
set(CMAKE_AR ${PSPDEV}/bin/psp-ar)
set(CMAKE_RANLIB ${PSPDEV}/bin/psp-ranlib)

set(CMAKE_FIND_ROOT_PATH ${PSPDEV})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

5. `mkdir build && cd build`
6. `cmake .. -DCMAKE_TOOLCHAIN_FILE=../psp-toolchain.cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_WASI=none -DCMAKE_INSTALL_PREFIX=~/wasm3`
7. `make`
8. `make install`

### Runtime(main.c)

```sh
mkdir build
cd build
psp-cmake ..
make
```

### Go

```sh
tinygo build -o hello.wasm -target=wasm -no-debug main.go
```
