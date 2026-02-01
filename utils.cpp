//
// Created by Micael Cossa on 01/02/2026.
//
#include "utils.h"
#include <iostream>

void registerHookThread(HHOOK& hook, DWORD& hookWinThreadID, HOOKPROC function) {

    hookWinThreadID = GetCurrentThreadId();
    hook = SetWindowsHookEx(WH_MOUSE_LL, function, GetModuleHandle(nullptr), 0);

    if(hook == nullptr) {
        std::cout << "Something went wrong , Error: " << GetLastError();
        return;
    }

    MSG msg;
    BOOL msgRet;
    while((msgRet = GetMessage(&msg, nullptr, 0 ,0)) != 0) {

        if(msgRet == -1) {
            std::cout << "Something bad happened to the capture \n";
            break;
        }

        if(msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    hookWinThreadID = 0;
    UnhookWindowsHookEx(hook);
}


void sendKey(WORD vk, bool keyUp = false) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    if (keyUp)
        input.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}

void sendChar(wchar_t ch) {
    INPUT inputs[2]{};

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wScan = ch;
    inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;

    inputs[1] = inputs[0];
    inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));
}