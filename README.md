# Wasm3 + TinyGo on PSP

[![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen?style=flat-square)](/LICENSE)

![psp](https://github.com/user-attachments/assets/4d631e63-5412-47ba-a2e1-5a3262cabc3b)

## Required

- [PSPDEV](https://pspdev.github.io)
- [TinyGo](https://tinygo.org)
- [Wasm3](https://github.com/wasm3/wasm3)
  - You need compile it with PSP toolchain

## How to build

### Wasm3

1. `git clone https://github.com/wasm3/wasm3.git`
2. `cd wasm3`
3. Append the following code to `CMakeLists.txt`

```cmake
install(FILES
    ${PROJECT_SOURCE_DIR}/source/wasm3.h
    ${PROJECT_SOURCE_DIR}/source/m3_api_wasi.h
    # Add other headers as needed
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS m3
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
```

4. Create `psp-toolchain.cmake`

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)

# Example: /usr/local/pspdev or /opt/pspdev, etc. Modify according to your environment.
set(PSPDEV /path/to/pspdev)

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
