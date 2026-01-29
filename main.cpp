//
// Created by Micael Cossa on 27/01/2026.
//
#include <iostream>
#include <windows.h>
#include <thread>
#include <queue>
#include <utility>
#include <mutex>

enum class ACTION_TYPE : unsigned short {

    RIGHT_MOUSE_CLICK = 0,
    LEFT_MOUSE_CLICK = 1,
    KEYBOARD_CONTROL_COMBO = 2, // CTRL C OR CTRL V
    KEYBOARD_TEXT = 3,
    PASTE_CLIPBOARD = 4 // PASTING CLIPBOARD WITHOUT CTRL V
};
struct WINDOWS_ACTION {
    ACTION_TYPE action;
    void* data; // Location for mouse clicks, text for keyboard
};

struct LOCATION {
    LONG X, Y;
};

struct ScriptNode {
    WINDOWS_ACTION val;
    std::unique_ptr<ScriptNode> next;
};


class ActionsScript {

private:
    std::string name;
    int size = 0;
    std::unique_ptr<ScriptNode> header;
    ScriptNode* footer = nullptr;

public:
    explicit ActionsScript(std::string name) : name(std::move(name)), header(nullptr) {}

    void addAction(WINDOWS_ACTION action) {

        auto node = std::make_unique<ScriptNode>(ScriptNode{action, nullptr});
        ScriptNode* raw = node.get();

        size++;

        if (!header) {
            header = std::move(node);
            footer = raw;
        } else {
            footer->next = std::move(node);
            footer = raw;
        }
    }

    [[nodiscard]] int getNumberOfActions() const {
        return size;
    }
};

void playAction(WINDOWS_ACTION action);

void registerHookThread();

size_t currScript = -1;

std::vector<ActionsScript> scripts;
HHOOK hook;
DWORD hookWinThreadID = 0;

int main() {

    std::cout << "Welcome to the recorder. To play a script type play, to start recording a script type record, to edit the record type analyse. \n";
    std::string command;


    do {

        std::cin >> command;

        if(command == "analyse") {

        } else if(command == "play") {

        } else if(command == "create") {
            std::cout << "Input name: \n";


        } else if(command == "finish") {
            PostThreadMessage(hookWinThreadID, WM_QUIT, 0, 0);

        } else if(command == "record") {
            std::cout << "To finish recording type finish. \n";
            currScript = scripts.size();
            scripts.emplace_back("Unnamed");
            std::jthread hookworker(registerHookThread);
        }
    } while(command != "stop");

    // Stop the Thread
    if(hookWinThreadID != 0)
        PostThreadMessage(hookWinThreadID, WM_QUIT, 0, 0);

    return 0;
}


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        auto* info = (MSLLHOOKSTRUCT*)lParam;

        if (wParam == WM_LBUTTONDOWN) {
            LOCATION location = {info->pt.x, info->pt.y};
            WINDOWS_ACTION windowsAction = {ACTION_TYPE::LEFT_MOUSE_CLICK, &location};
            scripts[currScript].addAction(windowsAction);
            std::cout << "Action: " << scripts[currScript].getNumberOfActions() << " Left click detected at (" << info->pt.x << ", " << info->pt.y << ")\n";

        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
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

void playAction(WINDOWS_ACTION action) {
    std::vector<INPUT> inputs;
    switch(action.action) {
        case ACTION_TYPE::LEFT_MOUSE_CLICK: {
            auto* location = static_cast<LOCATION*>(action.data);
            SetCursorPos(location->X, location->Y);

            inputs.resize(2);

            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        }

        case ACTION_TYPE::RIGHT_MOUSE_CLICK: {

            auto* location = static_cast<LOCATION*>(action.data);
            SetCursorPos(location->X, location->Y);

            inputs.resize(2);

            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        }

        case ACTION_TYPE::KEYBOARD_CONTROL_COMBO: {

            if (!action.data)
                break;

            auto* text = static_cast<const char*>(action.data);

            char c = text[0]; // 'c' or 'v'
            WORD vk = (c == 'c') ? 'C' : 'V';

            sendKey(VK_CONTROL, false);
            sendKey(vk, false);
            sendKey(vk, true);
            sendKey(VK_CONTROL, true);
            break;
        }

        case ACTION_TYPE::PASTE_CLIPBOARD: {
            sendKey(VK_CONTROL, false);
            sendKey('V', false);
            sendKey('V', true);
            sendKey(VK_CONTROL, true);
            break;
        }

        case ACTION_TYPE::KEYBOARD_TEXT: {
            if (!action.data)
                break;

            auto* text = static_cast<const char*>(action.data);
            // Convert std::string → UTF-16
            int len = MultiByteToWideChar(
                    CP_UTF8, 0,
                    text, -1,
                    nullptr, 0
            );

            std::wstring wtext(len, L'\0');
            MultiByteToWideChar(
                    CP_UTF8, 0,
                    text, -1,
                    wtext.data(), len
            );

            for (wchar_t ch : wtext) {
                if (ch == L'\0') break;
                sendChar(ch);
            }
            break;
        }
    }

    if (!inputs.empty()) {
        SendInput(
                static_cast<UINT>(inputs.size()),
                inputs.data(),
                sizeof(INPUT)
        );
    }
}