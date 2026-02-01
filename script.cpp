//
// Created by Micael Cossa on 01/02/2026.
//

#define WIN32_LEAN_AND_MEAN

#include "script.h"
#include "utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <regex>
#include <sstream>
#include <iomanip>
#include <cwctype>

bool stop_playing = false;
HHOOK hooka;

// Stop mechanism
LRESULT CALLBACK StopPlayingProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if(wParam == WM_RBUTTONDOWN) {
            stop_playing = true;
            std::cout << "Play interrupted! \n";
        }
    }
    return CallNextHookEx(hooka, nCode, wParam, lParam);
}

void ActionsScript::addAction(WINDOWS_ACTION action) {

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
void ActionsScript::playAllActions() const {
    auto* temp = header.get();
    DWORD local_threadID;


    std::jthread hookworker([&](HOOKPROC func) {
        registerHookThread(hooka, local_threadID, func);
    }, StopPlayingProc);

    hookworker.detach();

    while(temp && !stop_playing) {
        temp->val.playAction();
        temp = temp->next.get();
        Sleep(300);
    }

    // Was the script interrupted or finished?
    if(stop_playing)
        stop_playing = false;
}
void ActionsScript::printAllActions() const {
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

void ActionsScript::toggleRepeater()  noexcept{
    repeating = !repeating;
}

[[nodiscard]] bool ActionsScript::isRepeating() const noexcept {
    return repeating;
}

[[nodiscard]] std::string ActionsScript::getName() const noexcept{
    return name;
}

[[nodiscard]] int ActionsScript::getNumberOfActions() const noexcept{
    return size;
}


void setClipboardText(const std::wstring& text) {

    size_t size = (text.size()+1)*sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);

    if (!hMem) {
        CloseClipboard();
        return;
    }

    void* ptr = GlobalLock(hMem);
    memcpy(ptr, text.c_str(), size);
    GlobalUnlock(hMem);

    HANDLE handle = SetClipboardData(CF_UNICODETEXT, hMem);

    if(!handle)
        std::cout << "Unable to set clipboard " << GetLastError() << " \n";

    CloseClipboard();
}
void ActionsScript::removeAction(int pos) {

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
void ActionsScript::swapAction(const int pos1, const int pos2) {

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

void ActionsScript::moveAction(int oldpos, int newpos) {
    if (oldpos == newpos || oldpos < 1 || oldpos > size || newpos < 1 || newpos > size) {
        std::cout << "Move action denied, invalid positions.\n";
        return;
    }

    // Get the action to move
    auto* temp = header.get();
    for (int i = 1; i < oldpos; ++i) {
        temp = temp->next.get();
    }
    WINDOWS_ACTION action_to_move = temp->val;

    // Remove it from old position
    removeAction(oldpos - 1);

    // Insert at new position
    insertActionAt(action_to_move, newpos - 1);
}

void ActionsScript::insertActionAt(WINDOWS_ACTION action, int pos) {
    auto new_node = std::make_unique<ScriptNode>(ScriptNode{std::move(action), nullptr});

    if (pos == 0) {
        // Insert at beginning
        new_node->next = std::move(header);
        header = std::move(new_node);

        if (!header->next)
            footer = header.get();
    } else {
        // Find node before insertion point
        auto* temp = header.get();
        for (int i = 1; i < pos; ++i) {
            if (!temp) return;
            temp = temp->next.get();
        }

        // Insert after temp
        new_node->next = std::move(temp->next);
        temp->next = std::move(new_node);

        if (!temp->next->next)
            footer = temp->next.get();
    }

    size++;
}


void WINDOWS_ACTION::printAction() const noexcept {

    std::cout << getActionName();

    if (auto* location = std::get_if<LOCATION>(&data)) {
        std::cout << " Pos ("
                  << location->X << " , "
                  << location->Y << ")\n";

    } else if (auto* text = std::get_if<std::string>(&data)) {
        std::cout << " Text: " << *text << "\n";

    } else if (auto* number = std::get_if<WORD>(&data)) {
        std::cout << " Number: " << *number << "\n";

    } else if (auto* intn = std::get_if<int>(&data)) {
        std::cout << " Number: " << *intn << "\n";

    } else if (auto* locPair = std::get_if<std::pair<LOCATION, LOCATION>>(&data)) {
        std::cout << " From ("
                  << locPair->first.X << " , " << locPair->first.Y << ")"
                  << " To ("
                  << locPair->second.X << " , " << locPair->second.Y << ")\n";

    } else {
        // std::monostate or unknown
        std::cout << "\n";
    }
}


std::wstring getClipboardText() {

    std::wstring result;

    if (!OpenClipboard(nullptr)) {
        std::cout << "Unable to access clipboard data \n";
        return result;
    }

    HANDLE textData = GetClipboardData(CF_UNICODETEXT);

    if (textData) {
        auto *text = static_cast<wchar_t *>(GlobalLock(textData));
        if (text) {
            result = text;
        }
        GlobalUnlock(textData);
    }
    CloseClipboard();
    return result;
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
        case ACTION_TYPE::SPECIAL_FUNCTION:
            return "SPECIAL FUNCTION";
        case ACTION_TYPE::INSERT_VKKEY:
            return "INSERT_VKKEY";
        case ACTION_TYPE::WAIT_MILLISECONDS:
            return "WAIT_MILLISECONDS";
        case ACTION_TYPE::DRAG_MOUSE:
            return "DRAG_MOUSE";
        case ACTION_TYPE::SPECIAL_FUNCTION2:
            return "SPECIAL_FUNCTION2";
        case ACTION_TYPE::SPECIAL_FUNCTION3:
            return "SPECIAL_FUNCTION3";
        case ACTION_TYPE::REPEAT_SCRIPT:
            return "REPEAT_SCRIPT";
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

            WORD vk = VkKeyScan(std::get<std::string>(data)[0]) & 0xFF;

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
        case ACTION_TYPE::SPECIAL_FUNCTION: {

            std::wstring text = getClipboardText();
            auto pos = text.find(L"NLPG Ref.No:");

            if (pos == std::wstring::npos) {
                std::cout << "Unable to find NLPG Number \n";
            } else {
                pos += wcslen(L"NLPG Ref.No:");

                while (pos < text.size() && iswspace(text[pos])) {
                    ++pos;
                }

                size_t start = pos;
                while (pos < text.size() && iswdigit(text[pos])) ++pos;


                long long number = std::stoll(text.substr(start, pos - start));
                std::cout << "Number detected " << number << "\n";
                break;
            }
        }
        case ACTION_TYPE::INSERT_VKKEY: {
            auto word_key = std::get<WORD>(data);
            sendKey(word_key, false);
            sendKey(word_key, true);
            break;
        }
        case ACTION_TYPE::WAIT_MILLISECONDS: {
            auto milliseconds = std::get<int>(data);
            Sleep(milliseconds);
            break;
        }
        case ACTION_TYPE::DRAG_MOUSE: {
            std::pair<LOCATION, LOCATION> locations = std::get<std::pair<LOCATION, LOCATION>>(data);
            SetCursorPos(locations.first.X, locations.first.Y);
            Sleep(50);

            inputs.resize(2);
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, inputs.data(), sizeof(INPUT));

            // Drag to end position
            Sleep(50);
            SetCursorPos(locations.second.X, locations.second.Y);
            Sleep(400);

            // Release
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION2: { // file name
            std::wstring input = getClipboardText();
            std::wstring output;

            std::wregex date_pattern(LR"(\d{2}[a-zA-Z]{3}\d{2})", std::regex_constants::icase);
            std::wsmatch date_match;

            std::wstring remaining = input;
            std::wstring formatted_date;

            if (std::regex_search(input, date_match, date_pattern)) {
                std::wstring found_date = date_match.str();

                formatted_date = found_date.substr(0, 2) + L" " +
                                 found_date.substr(2, 3) + L" " +
                                 found_date.substr(5, 2);

                remaining = input.substr(0, date_match.position());
            }

            while (!remaining.empty() && std::iswspace(remaining.back())) {
                remaining.pop_back();
            }

            std::wstring street_address;
            std::wstring code_section;

            size_t first_space = remaining.find(L' ');
            if (first_space != std::wstring::npos) {
                size_t second_space = remaining.find(L' ', first_space + 1);

                if (second_space != std::wstring::npos) {
                    size_t third_space = remaining.find(L' ', second_space + 1);

                    if (third_space != std::wstring::npos) {
                        // Found all three spaces - split after third word
                        street_address = remaining.substr(0, third_space);
                        code_section = remaining.substr(third_space + 1);

                        // Trim whitespace from code
                        while (!code_section.empty() && std::iswspace(code_section.front())) {
                            code_section.erase(0, 1);
                        }
                    } else {
                        street_address = remaining;
                    }
                } else {
                    street_address = remaining;
                }
            } else {
                street_address = remaining;
            }

            if (!street_address.empty()) {
                output = street_address;

                if (!formatted_date.empty()) {
                    output += L"\n" + formatted_date;
                }

                if (!code_section.empty()) {
                    output += L"\n" + code_section;
                }

                // Set to clipboard
                setClipboardText(output);
                std::wcout << L"Processed:\n" << output << L"\n";
            } else {
                std::cout << "Unable to parse address\n";
            }

            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION3: // paste the remaining
            break;
        case ACTION_TYPE::REPEAT_SCRIPT:

            auto repeatinfo = std::get<std::pair<ActionsScript*, int>>(data);

            if(repeatinfo.first->isRepeating())
                return;

            repeatinfo.first->toggleRepeater(); // turn it on

            for(int i = 0; i < repeatinfo.second; ++i) {
                repeatinfo.first->playAllActions();
            }
            repeatinfo.first->toggleRepeater(); // turn it off
            break;
    }

    if (!inputs.empty()) {
        SendInput(
                static_cast<UINT>(inputs.size()),
                inputs.data(),
                sizeof(INPUT)
        );
    }
}