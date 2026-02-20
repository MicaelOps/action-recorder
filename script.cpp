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
#include <cwctype>

bool stop_playing = false;
double pricedealing = 0;
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
void ActionsScript::playAllActions(bool repeater) const {
    auto* temp = header.get();
    DWORD local_threadID;


    if (!isRepeating()){
        std::jthread hookworker([&](HOOKPROC func) {
            registerHookThread(hooka, local_threadID, func);
        }, StopPlayingProc);

        hookworker.detach();
    }

    while(temp && !stop_playing) {
        std::cout << "aaeee";
        temp->val.playAction(repeater);
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

    if(!OpenClipboard(nullptr)) {
        std::cout << "Unable to open clipboard  \n";
        return;
    }
    size_t size = (text.size()+1)*sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);

    if (!hMem) {
        std::cout << "Unable to GlobalAlloc \n";
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
        case ACTION_TYPE::SPECIAL_FUNCTION4:
            return "SPECIAL_FUNCTION4";
        case ACTION_TYPE::SPECIAL_FUNCTION5:
            return "SPECIAL_FUNCTION5";
    }
    return "unknown action";
}

void WINDOWS_ACTION::playAction(bool repeaterCall) const noexcept{
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


                auto number = text.substr(start, pos - start);

                setClipboardText(number);
                std::wcout << L"Number detected " << number << "\n";
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
            // Move to start position
            SetCursorPos(locations.first.X, locations.first.Y);
            Sleep(100);

            // Press mouse button
            INPUT input{};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));

            // Wait for the drag to register
            Sleep(300);

            // Smooth drag with intermediate steps
            int steps = 20;
            for (int i = 1; i <= steps; ++i) {
                int x = locations.first.X + (locations.second.X - locations.first.X) * i / steps;
                int y = locations.first.Y + (locations.second.Y - locations.first.Y) * i / steps;
                SetCursorPos(x, y);
                Sleep(10);  // Small delay between each step
            }

            // Hold at final position
            Sleep(500);

            // Release mouse button
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION4: {
            // Open the clipboard
            if (!OpenClipboard(nullptr)) {
                std::cout << "UNABLE TO TO GET CLIPBOARD" << std::endl;

                return;
            }

            // Get the clipboard content (text format)
            HANDLE hData = GetClipboardData(CF_TEXT);
            if (hData == nullptr) {
                std::cout << "Failed to get clipboard data!" << std::endl;
                CloseClipboard();
                return;
            }

            // Lock the handle to get the text pointer
            char *pszText = static_cast<char *>(GlobalLock(hData));
            if (pszText == nullptr) {
                std::cout << "Failed to lock clipboard data!" << std::endl;
                CloseClipboard();
                return;
            }

            // Convert to std::string
            std::string clipboardText(pszText);

            // Close the clipboard
            GlobalUnlock(hData);

            // Regex pattern to match the task and number
            std::regex pattern(R"(SM-Task-(\d{5})\s+(\d+(\.\d{1,2})?))");

            std::smatch matches;
            if (std::regex_search(clipboardText, matches, pattern)) {

                std::string task = "SM-Task-" + matches[1].str();  // Task ID
                pricedealing = std::stod(matches[2].str());  // Number
                std::cout << "proccessing "<< task << " "  << pricedealing << std::endl;
                EmptyClipboard();

                // Allocate memory for the task string and copy it to the clipboard
                HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, task.size() + 1);
                if (hGlobal) {
                    char *pData = static_cast<char *>(GlobalLock(hGlobal));
                    memcpy(pData, task.c_str(), task.size() + 1);
                    GlobalUnlock(hGlobal);
                    SetClipboardData(CF_TEXT, hGlobal);
                }
            } else {
                std::cerr << "No matching format found!" << std::endl;
            }
            CloseClipboard();
            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION5: {
            if (!OpenClipboard(nullptr)) {
                std::cout << "Failed t55o open clipboard!" << std::endl;
                break;
            }

            // Get the clipboard content (text format)
            HANDLE hData = GetClipboardData(CF_TEXT);
            if (hData == nullptr) {
                std::cout << "Failed 55to get clipboard data!" << std::endl;
                CloseClipboard();
                break;
            }

            // Lock the handle to get the text pointer
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText == nullptr) {
                std::cout << "Failed to55 lock clipboard data!" << std::endl;
                CloseClipboard();
                break;
            }

            // Convert to std::string
            std::string clipboardText(pszText);

            // Close the clipboard after locking
            GlobalUnlock(hData);
            CloseClipboard();

            // Regex pattern to find the first £ followed by a number, with optional commas and decimals
            std::regex pattern(R"(\£\s?(\d{1,3}(?:,\d{3})*(?:\.\d{1,2})?))");

            std::smatch matches;
            if (std::regex_search(clipboardText, matches, pattern)) {
                std::string amountStr = matches[1].str();

                // Remove commas (if present)
                amountStr.erase(std::remove(amountStr.begin(), amountStr.end(), ','), amountStr.end());

                // Convert to double
                try {
                    std::stringstream ss(amountStr);
                    double amount = 0.0;
                    ss >> amount;

                    if(pricedealing == amount) {
                        std::cout << "price match!";
                    } else {
                        std::cout << "price did not match";
                    }
                    break;
                }
                catch (const std::exception& e) {
                    std::cout << "Failed to convert amount: " << e.what() << std::endl;
                    break;
                }
            } else {
                std::cout << "No valid pound amount found!" << std::endl;
                break;
            }
            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION2: { // file name
            std::wstring input = getClipboardText();

            // Retry if clipboard is empty (not ready yet)
            int retries = 0;
            while (input.empty() && retries < 3) {
                Sleep(100);
                input = getClipboardText();
                retries++;
            }

            std::wstring output;

            std::wregex range_pattern(LR"(^\d+-(\d+\s+))", std::regex_constants::icase);
            input = std::regex_replace(input, range_pattern, L"$1");

            std::wregex hyphen_code_pattern(LR"(-([a-zA-Z]))", std::regex_constants::icase);
            input = std::regex_replace(input, hyphen_code_pattern, L" $1");

            std::vector<std::wstring> code_keywords = {
                    L"HD", L"SD", L"MW", L"FIREDOOR", L"- ", L"-", L"cr", L"ELE", L"EIR", L"cr", L"eicr", L"bcn", L"ct", L"emer", L"trada", L"bmtrada", L"BM", L"FIRE", L"ct", L"Full", L"Full H", L"Htg", L"Hse", L"AMENDED", L"RADS", L"Rads", L"FAN", L"DEICR", L"SWI", L"KIT", L"KIT Survey", L"LAS", L"BATTERY", L"BAT", L"HO", L"KIT Asbes", L"Asbest", L"WAIVER", L"Waiver Mains", L"DA LAS", L"CP12", L"Gas Safe",
                    L"Handover", L"Boiler", L"Form", L"IMS", L"CB", L"CB5", L"CB4", L"CB3", L"HWT", L"EIC", L"Asbestos", L"DA KIT", L"LAS HO", L"TEST", L"TEST ONLY"
                    // Add more keywords as needed
            };


            std::wregex date_pattern(LR"((\d{2})\s*([a-zA-Z]{3,4})\s*(\d{2}))", std::regex_constants::icase);
            std::wsmatch date_match;

            std::wstring remaining = input;
            std::wstring formatted_date;

            // Extract and format date
            if (std::regex_search(input, date_match, date_pattern)) {
                std::wstring day = date_match[1].str();
                std::wstring month = date_match[2].str();
                std::wstring year = date_match[3].str();
                formatted_date = day + L" " + month + L" " + year;

                // Remove date from input
                remaining = input.substr(0, date_match.position());
            } else {
                // Default date if no date found
                formatted_date = L"01 jun 2019";
            }

            // Trim trailing whitespace
            while (!remaining.empty() && std::iswspace(remaining.back())) {
                remaining.pop_back();
            }

            // Split remaining into words
            std::vector<std::wstring> words;
            std::wstringstream ss(remaining);
            std::wstring word;

            while (ss >> word) {
                words.push_back(word);
            }

            // Find where the code section starts (first keyword match)
            size_t code_start_index = words.size();

            for (size_t i = 0; i < words.size(); ++i) {

                for (const auto& keyword : code_keywords) {

                    std::wstring word_lower = words[i];
                    std::wstring keyword_lower = keyword;

                    std::transform(word_lower.begin(), word_lower.end(), word_lower.begin(), ::towlower);
                    std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::towlower);

                    if (word_lower == keyword_lower) {
                        code_start_index = i;
                        break;
                    }
                }

                if (code_start_index < words.size()) {
                    break; // Found the start of code section
                }
            }

            // LIMIT: Address can be maximum 3 words
            // If no keyword found or keyword is after word 3, split at word 3
            if (code_start_index > 3) {
                code_start_index = 3;
            }

            // Build street address (words before code section)
            std::wstring street_address;
            for (size_t i = 0; i < code_start_index; ++i) {
                if (i > 0) street_address += L" ";
                street_address += words[i];
            }

            // Build code section (words from code start onward)
            std::wstring code_section;
            for (size_t i = code_start_index; i < words.size(); ++i) {
                if (i > code_start_index) code_section += L" ";
                code_section += words[i];
            }

            // Safety: If no address found (empty input), use placeholder
            if (street_address.empty()) {
                street_address = L"NO ADDRESS";
            }

            // If no code section found, use default
            if (code_section.empty()) {
                code_section = L"cert";
            }

            // Build final output - ALWAYS output something
            output = street_address;
            output += L"\n" + formatted_date;
            output += L"\n" + code_section;

            // Set to clipboard
            setClipboardText(output);
            std::wcout << L"Processed:\n" << output << L"\n";
            break;
        }
        case ACTION_TYPE::SPECIAL_FUNCTION3: // paste the remaining
            break;
        case ACTION_TYPE::REPEAT_SCRIPT:

            auto repeatinfo = std::get<std::pair<ActionsScript*, int>>(data);

            if(!repeaterCall) {
                std::cout << "first time repeater ";
                repeatinfo.first->toggleRepeater();
                for(int i = 0; i < repeatinfo.second; ++i) {
                    repeatinfo.first->playAllActions(true);
                }
                repeatinfo.first->toggleRepeater();
            }
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