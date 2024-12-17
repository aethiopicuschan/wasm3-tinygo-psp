#include <stdio.h>
#include <string.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "wasm3.h"
#include "m3_api_wasi.h"

#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("Wasm3Example", 0, 1, 0);
PSP_HEAP_SIZE_KB(1024);

static const void* host_debug_println(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *stack, void *mem)
{
    uint32_t ptr = (uint32_t) stack[0];
    uint32_t length = (uint32_t) stack[1];

    uint8_t* bytes = (uint8_t*)mem + ptr;

    char buffer[256];
    if (length >= sizeof(buffer)) {
        length = sizeof(buffer)-1;
    }
    memcpy(buffer, bytes, length);
    buffer[length] = '\0';

    printf("%s\n", buffer);

    return NULL;
}

static void waitForUser()
{
    SceCtrlData pad;
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    printf("\nPress START to exit.\n");

    while (1) {
        sceCtrlPeekBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_START) {
            break;
        }
        sceDisplayWaitVblankStart();
    }

    sceKernelExitGame();
}

int main(int argc, char *argv[])
{
    pspDebugScreenInit();

    SceUID fd = sceIoOpen("hello.wasm", PSP_O_RDONLY, 0777);
    if (fd < 0) {
        printf("Failed to open hello.wasm\n");
        waitForUser();
        return -1;
    }

    SceOff size = sceIoLseek(fd, 0, PSP_SEEK_END);
    sceIoLseek(fd, 0, PSP_SEEK_SET);

    uint8_t* wasmBytes = (uint8_t*)malloc(size);
    if (!wasmBytes) {
        printf("Failed to allocate memory for WASM\n");
        sceIoClose(fd);
        waitForUser();
        return -1;
    }

    if (sceIoRead(fd, wasmBytes, size) != size) {
        printf("Failed to read WASM file\n");
        free(wasmBytes);
        sceIoClose(fd);
        waitForUser();
        return -1;
    }

    sceIoClose(fd);

    IM3Environment env = m3_NewEnvironment();
    if (!env) {
        printf("m3_NewEnvironment failed\n");
        free(wasmBytes);
        waitForUser();
        return -1;
    }

    IM3Runtime runtime = m3_NewRuntime(env, 64*1024, NULL);
    if (!runtime) {
        printf("m3_NewRuntime failed\n");
        m3_FreeEnvironment(env);
        free(wasmBytes);
        waitForUser();
        return -1;
    }

    IM3Module module;
    M3Result result = m3_ParseModule(env, &module, wasmBytes, (uint32_t)size);
    free(wasmBytes);

    if (result != m3Err_none) {
        printf("m3_ParseModule: %s\n", result);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        waitForUser();
        return -1;
    }

    result = m3_LoadModule(runtime, module);
    if (result != m3Err_none) {
        printf("m3_LoadModule: %s\n", result);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        waitForUser();
        return -1;
    }

    // "v(ii)" = void(i32,i32)
    result = m3_LinkRawFunction(module, "debug", "println", "v(ii)", &host_debug_println);
    if (result != m3Err_none) {
        printf("m3_LinkRawFunction: %s\n", result);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        waitForUser();
        return -1;
    }

    IM3Function f;
    result = m3_FindFunction(&f, runtime, "start");
    if (result != m3Err_none) {
        printf("m3_FindFunction(start): %s\n", result);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        waitForUser();
        return -1;
    }

    result = m3_CallV(f);
    if (result != m3Err_none) {
        printf("m3_Call(start): %s\n", result);
    }

    m3_FreeRuntime(runtime);
    m3_FreeEnvironment(env);

    waitForUser();

    return 0;
}
