//
// Created by Micael Cossa on 27/01/2026.
//
#include <iostream>
#include <windows.h>
#include <thread>
#include <queue>
#include <utility>
#include <mutex>
#include <algorithm>
#include <variant>
#include <string>

enum class ACTION_TYPE : unsigned short {

    RIGHT_MOUSE_CLICK = 0,
    LEFT_MOUSE_CLICK = 1,
    KEYBOARD_CONTROL_COMBO = 2, // CTRL C OR CTRL V
    KEYBOARD_TEXT = 3,
    PASTE_CLIPBOARD = 4 // PASTING CLIPBOARD WITHOUT CTRL V
};

struct LOCATION {
    LONG X, Y;
};

struct WINDOWS_ACTION {
    ACTION_TYPE action;
    std::variant<std::monostate, std::string, LOCATION> data; // Location for mouse clicks, text for keyboard

    [[nodiscard]] std::string getActionName() const noexcept;
    void printAction() const noexcept;
    void playAction() const noexcept;
};



struct ScriptNode {
    WINDOWS_ACTION val;
    std::unique_ptr<ScriptNode> next;
};


void createMenu();

void playScript();

void analyseScript();


void registerHookThread();

class ActionsScript {

private:
    std::string name;
    int size = 0;
    std::unique_ptr<ScriptNode> header;
    ScriptNode* footer = nullptr;

public:
    explicit ActionsScript(std::string name) : name(std::move(name)), header(nullptr) {}

    void addAction(WINDOWS_ACTION action) {

        auto node = std::make_unique<ScriptNode>(ScriptNode{std::move(action), nullptr});
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
    void playAllActions() const {
        auto* temp = header.get();
        while(temp) {
            temp->val.playAction();
            temp = temp->next.get();
            Sleep(1000);
        }
    }
    void printAllActions() const {
        auto* temp = header.get();
        int pos = 1;
        while(temp) {

            WINDOWS_ACTION action = temp->val;
            std::cout << pos << ". -> ";
            action.printAction();
            temp = temp->next.get();
            pos++;
        }
    }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    [[nodiscard]] int getNumberOfActions() const {
        return size;
    }

    void removeAction(int pos) {

        auto* temp = header.get();

        if(pos == 0) {

            if(!header)
                return;

            header = std::move(header->next);
        } else {

            for(int i = 1; i < pos; ++i) {

                if(!temp) {
                    std::cout << "Remove action denied, out of bounds? \n";
                    return;
                }

                temp = temp->next.get();
            }

            temp->next = std::move(temp->next->next);

            if(!temp->next)
                footer = temp;
        }

        size--;
    }

    // Completely unnecessary to do it like this, but I wanted to act smart and overcomplicate things because it looks cool, and I am stupid!
    void swapAction(const int pos1, const int pos2) {

        if(pos1 > size || pos2 > size) {
            std::cout << "Swap action denied, out of bounds. \n";
            return;
        }
        auto* min_pos_temp = header.get();

        const int max_pos = max(pos1, pos2), min_pos = min(pos1, pos2);

        for(int i = 1; i < min_pos; ++i) {
            min_pos_temp = min_pos_temp->next.get();
        }

        auto* max_pos_temp = min_pos_temp;

        for(int i = min(pos1, pos2); i < max_pos; ++i) {
            max_pos_temp = max_pos_temp->next.get();
        }

        std::swap(min_pos_temp->val, max_pos_temp->val);
    }
};

size_t currScript = -1;

std::vector<ActionsScript> scripts;
HHOOK hook;
DWORD hookWinThreadID = 0;

int main() {

    std::cout << "Welcome to the recorder. ";
    std::string command;


    do {

        std::cout << "To play a script type play, to start recording a script type record, to edit the record type analyse. \n";
        std::cin >> command;
        std::cin.ignore(1000, '\n');

        if(command == "analyse") {
            analyseScript();

        } else if(command == "play") {
            playScript();
        } else if(command == "create") {
            createMenu();

        } else if(command == "finish") {
            if(hookWinThreadID == 0)
                std::cout << "There is no recording occurring. \n";
            else {
                PostThreadMessage(hookWinThreadID, WM_QUIT, 0, 0);
                std::cout << "Recording has been finished.";
            }

        } else if(command == "record") {

            std::string scriptname;
            std::cout << "Input the script name \n";
            std::cin >> scriptname;

            std::cout << "To finish recording type finish. \n";
            currScript = scripts.size();
            scripts.emplace_back(scriptname);
            std::jthread hookworker(registerHookThread);
            hookworker.detach();
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
        LOCATION location = {info->pt.x, info->pt.y};

        if (wParam == WM_LBUTTONDOWN) {

            WINDOWS_ACTION windowsAction = {ACTION_TYPE::LEFT_MOUSE_CLICK, location};

            scripts[currScript].addAction(windowsAction);
            std::cout << "Action: " << scripts[currScript].getNumberOfActions() << " Left click detected at (" << info->pt.x << ", " << info->pt.y << ")\n";

        } else if(wParam == WM_RBUTTONDOWN) {
            WINDOWS_ACTION windowsAction = {ACTION_TYPE::RIGHT_MOUSE_CLICK, location};

            scripts[currScript].addAction(windowsAction);
            std::cout << "Action: " << scripts[currScript].getNumberOfActions() << " Right click detected at (" << info->pt.x << ", " << info->pt.y << ")\n";

        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}


constexpr int findIntLength(std::string_view text) {
    int count = 0;
    for (const auto &item: text) {
        if(!isdigit(item))
            return count;
        count++;
    }
    return count;
}

void analyseScript(ActionsScript& script) {
    std::string command;

    do {
        std::cout << " \n \n \nOrder of Actions: \n";
        script.printAllActions();

        std::cout << "\n\n\nOptions: \n"
                  << "To stop analysis, write stop_analysis' \n"
                  << "Add Left click at pos by writing 'add_leftclick X Y' \n"
                  << "Add Right click at pos by writing 'add_rightclick X Y' \n"
                  << "Add Ctrl letter by writing 'add_ctrl letter' \n"
                  << "Add Keyboard output by writing  'add_kbtext text' \n"
                  << "Remove an action by writing 'remove pos' \n"
                  << "Swap Actions pos by writing 'swap pos1 pos2' \n";


        std::getline(std::cin, command);


        if(command.starts_with("add_leftclick")) {

            std::string posString = command.substr(14);
            int xpos_length = findIntLength(posString);
            int Xpos = std::stoi(posString.substr(0, xpos_length));
            int Ypos = std::stoi(posString.substr(xpos_length + 1));

            LOCATION loc {Xpos, Ypos};
            WINDOWS_ACTION action = {ACTION_TYPE::LEFT_MOUSE_CLICK, loc};
            script.addAction(action);

            std::cout << "Action added successfully. \n";
        } else if(command.starts_with("add_rightclick")) {
            std::string posString = command.substr(15);
            int xpos_length = findIntLength(posString);

            int Xpos = std::stoi(posString.substr(0, xpos_length));
            int Ypos = std::stoi(posString.substr(xpos_length + 1));

            LOCATION loc {Xpos, Ypos};
            WINDOWS_ACTION action = {ACTION_TYPE::RIGHT_MOUSE_CLICK, loc};
            script.addAction(action);

            std::cout << "Action added successfully. \n";
        } else if(command.starts_with("add_ctrl")) {
            WINDOWS_ACTION action = {ACTION_TYPE::KEYBOARD_CONTROL_COMBO, command.substr(9,1)};
            script.addAction(action);
            std::cout << "Action added successfully. \n";
        } else if(command.starts_with("add_kbtext")) {
            WINDOWS_ACTION action = {ACTION_TYPE::KEYBOARD_TEXT, command.substr(11)};
            script.addAction(action);
            std::cout << "Action added successfully. \n";

        } else if(command.starts_with("remove")) {
            int pos = std::stoi(command.substr(7));
            script.removeAction(pos-1);
            std::cout << "Action removed successfully. \n";
        } else if(command.starts_with("swap")) {

            std::string posString = command.substr(5);
            int pos1_length = findIntLength(posString);
            int pos1 = std::stoi(posString.substr(0, pos1_length));
            int pos2 = std::stoi(posString.substr(pos1_length + 1));

            if(pos1 == pos2)
                continue;

            script.swapAction(pos1, pos2);
        }
    } while (command != "stop_analysis");
}

void createMenu() {
    std::string scriptname;
    std::cout << "Input the script name \n";
    std::cin >> scriptname;

    bool exists = std::ranges::any_of(scripts.begin(), scripts.end(), [&](const auto& script) {return script.getName() == scriptname;});

    if(exists) {
        std::cout << "There is a script with that name! \n";
        return;
    }

    std::cout << "Script created now you can analyse to add actions. \n";
    analyseScript(scripts.emplace_back(scriptname));
}

void playScript() {
    std::string scriptname;
    std::cout << "Input the script name \n";
    std::cin >> scriptname;

    auto scriptIt = std::ranges::find_if(scripts.begin(), scripts.end(), [&](const auto& script) {return script.getName() == scriptname;});
    if(scriptIt == scripts.end()) {
        std::cout << "Script not found \n";
        return;
    }

    std::cout << "Script found.. Starting in 3 seconds.. \n";

    Sleep(3000);

    (*scriptIt).playAllActions();

}

void analyseScript() {
    std::string scriptname;
    std::cout << "Input the script name \n";
    std::cin >> scriptname;

    auto scriptIt = std::ranges::find_if(scripts.begin(), scripts.end(), [&](const auto& script) {return script.getName() == scriptname;});
    if(scriptIt == scripts.end()) {
        std::cout << "Script not found \n";
        return;
    }
    analyseScript(*scriptIt);
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

void WINDOWS_ACTION::printAction() const noexcept{

    std::cout << getActionName();

    if(auto* location = std::get_if<LOCATION>(&data))
        std::cout << " Pos (" << location->X << " , " << location->Y <<") \n";
    else
        std::cout << " Text: " << get<std::string>(data) << " \n";
}

std::string WINDOWS_ACTION::getActionName() const noexcept {
    switch(action) {
        case ACTION_TYPE::LEFT_MOUSE_CLICK:
            return "LEFT MOUSE CLICK";
        case ACTION_TYPE::RIGHT_MOUSE_CLICK:
            return "RIGHT MOUSE CLICK";
        case ACTION_TYPE::KEYBOARD_CONTROL_COMBO:
            return "KEYBOARD CTRL + LETTER";
        case ACTION_TYPE::KEYBOARD_TEXT:
            return "KEYBOARD TEXT";
        case ACTION_TYPE::PASTE_CLIPBOARD:
            return "PASTE CLIPBOARD";
    }
    return "unknown action";
}

void WINDOWS_ACTION::playAction() const noexcept{
    std::vector<INPUT> inputs;
    switch(action) {
        case ACTION_TYPE::LEFT_MOUSE_CLICK: {
            auto location = std::get<LOCATION>(data);
            SetCursorPos(location.X, location.Y);

            inputs.resize(2);

            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        }

        case ACTION_TYPE::RIGHT_MOUSE_CLICK: {

            auto location = std::get<LOCATION>(data);
            SetCursorPos(location.X, location.Y);

            inputs.resize(2);

            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        }

        case ACTION_TYPE::KEYBOARD_CONTROL_COMBO: {

            auto text = std::get<std::string>(data);

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


            auto text = std::get<std::string>(data);
            // Convert std::string → UTF-16
            int len = MultiByteToWideChar(
                    CP_UTF8, 0,
                    text.c_str(), -1,
                    nullptr, 0
            );

            std::wstring wtext(len, L'\0');
            MultiByteToWideChar(
                    CP_UTF8, 0,
                    text.c_str(), -1,
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
