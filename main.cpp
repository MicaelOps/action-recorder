//
// Created by Micael Cossa on 27/01/2026.
//
#include <iostream>
#include <windows.h>
#include <thread>

void testSomething();
void registerHookThread();

HHOOK hook;
DWORD hookWinThreadID = 0;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        auto* info = (MSLLHOOKSTRUCT*)lParam;

        if (wParam == WM_LBUTTONDOWN) {
            std::cout << "Left click at ("
                      << info->pt.x << ", "
                      << info->pt.y << ")\n";
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}
int main() {



    std::cout << "Recorder started, to run the actions type start.";
    std::string command;

    std::jthread hookworker(registerHookThread);
    do {

        std::cin >> command;



        if(command == "start") {
            std::cout << "Starting..";
        } else if(command == "record") {

        } else if(command == "test") {
            testSomething();
        }

    } while(command != "stop");
    PostThreadMessage(hookWinThreadID, WM_QUIT, 0, 0);
    return 0;
}

void registerHookThread() {

    hookWinThreadID = GetCurrentThreadId();
    hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(nullptr), 0);

    if(hook == nullptr) {
        std::cout << "Something went wrong , Error: " << GetLastError();
        return;
    }

    MSG msg;
    BOOL msgRet;
    while((msgRet = GetMessage(&msg, nullptr, 0 ,0)) != 0) {

        if(msgRet == -1) {
            std::cout << "Something bad happened to the capture";
            break;
        }

        if(msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hook);
}
void testSomething() {
    Sleep(3000);
    INPUT inputs[4] = {};

    // Executing the Control + V (Pasting) keyboard command

    // Ctrl down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    // V down
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';

    // V up
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // Ctrl up
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
}