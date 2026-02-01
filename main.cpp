//
// Created by Micael Cossa on 27/01/2026.
//

#define WIN32_LEAN_AND_MEAN
#include "script.h"
#include "utils.h"

#include <iostream>
#include <thread>
#include <queue>
#include <utility>
#include <algorithm>

void createMenu();

void playScript();

void analyseScript();





size_t currScript = -1;

HHOOK hook;
DWORD hookWinThreadID = 0;


std::vector<ActionsScript> scripts;


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {

        auto* info = (MSLLHOOKSTRUCT*)lParam;
        LOCATION location = {info->pt.x, info->pt.y};

        if (wParam == WM_LBUTTONDOWN) {

            WINDOWS_ACTION windowsAction = {ACTION_TYPE::LEFT_MOUSE_CLICK, location};

            scripts[currScript].addAction(windowsAction);

            std::cout << "MSLLHOOKSTRUCT Location: " << scripts[currScript].getNumberOfActions() << " Left click detected at (" << info->pt.x << ", " << info->pt.y << ")\n";

        } else if(wParam == WM_RBUTTONDOWN) {
            WINDOWS_ACTION windowsAction = {ACTION_TYPE::RIGHT_MOUSE_CLICK, location};

            scripts[currScript].addAction(windowsAction);
            std::cout << "Action: " << scripts[currScript].getNumberOfActions() << " Right click detected at (" << info->pt.x << ", " << info->pt.y << ")\n";
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}



int main() {

    std::cout << "Welcome to the recorder. \n";
    std::string command;
    SetProcessDPIAware();

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


            std::jthread hookworker([&](HOOKPROC func) {
                registerHookThread(hook, hookWinThreadID, func);
            }, MouseProc);

            hookworker.detach();
        }

    } while(command != "stop");

    // Stop the Thread
    if(hookWinThreadID != 0)
        PostThreadMessage(hookWinThreadID, WM_QUIT, 0, 0);

    return 0;
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

    std::cout << "\n\nOptions: \n"
              << "To stop analysis, write stop_analysis' \n"
              << "Add Left click at pos by writing 'add_leftclick X Y' \n"
              << "Add Right click at pos by writing 'add_rightclick X Y' \n"
              << "Add Ctrl letter by writing 'add_ctrl letter' \n"
              << "Add Keyboard output by writing  'add_kbtext text' \n"
              << "Remove an action by writing 'remove pos' \n"
              << "Swap Actions pos by writing 'swap pos1 pos2' \n"
              << "Wait seconds by writing 'wait milliseconds' \n"
              << "Insert specific VK_KEY by writing 'add_vkkey key_number' \n"
              << "Move action to pos by writing 'move pos newpos \n"
              << "Add Drag Option by writing 'add_drag x1 y1 x2 y2' \n";

    do {

        std::cout << "Order of Actions: \n";
        script.printAllActions();

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
        } else if(command.starts_with("add_specialfunction")) {
            int id = std::stoi(command.substr(20));
            WINDOWS_ACTION action = {static_cast<ACTION_TYPE>(id), std::monostate()};
            script.addAction(action);
        } else if(command.starts_with("swap")) {

            std::string posString = command.substr(5);
            int pos1_length = findIntLength(posString);
            int pos1 = std::stoi(posString.substr(0, pos1_length));
            int pos2 = std::stoi(posString.substr(pos1_length + 1));

            if(pos1 == pos2)
                continue;

            script.swapAction(pos1, pos2);
            std::cout << "Action swapped successfully. \n";
        } else if(command.starts_with("move")) {

            std::string posString = command.substr(5);
            int pos1_length = findIntLength(posString);
            int pos1 = std::stoi(posString.substr(0, pos1_length));
            int pos2 = std::stoi(posString.substr(pos1_length + 1));

            if(pos1 == pos2)
                continue;

            script.moveAction(pos1, pos2);
            std::cout << "Action moved successfully. \n";
        } else if(command.starts_with("wait")) {

            std::string posString = command.substr(5);
            int pos1_length = findIntLength(posString);
            int pos1 = std::stoi(posString.substr(0, pos1_length));
            WINDOWS_ACTION action = {ACTION_TYPE::WAIT_MILLISECONDS, pos1};
            script.addAction(action);
            std::cout << "Action added successfully. \n";
        } else if(command.starts_with("add_drag")) {
            std::string posString = command.substr(9);
            int x1_length = findIntLength(posString);
            int X1 = std::stoi(posString.substr(0, x1_length));

            posString = posString.substr(x1_length + 1);
            int y1_length = findIntLength(posString);
            int Y1 = std::stoi(posString.substr(0, y1_length));

            posString = posString.substr(y1_length + 1);
            int x2_length = findIntLength(posString);
            int X2 = std::stoi(posString.substr(0, x2_length));

            int Y2 = std::stoi(posString.substr(x2_length + 1));

            std::pair<LOCATION, LOCATION> locs = {LOCATION{X1, Y1}, LOCATION{X2, Y2}};
            WINDOWS_ACTION action = {ACTION_TYPE::DRAG_MOUSE, locs};
            script.addAction(action);
            std::cout << "Drag action added successfully. \n";
        } else if(command.starts_with("add_vkkey")) {

            std::string posString = command.substr(10);
            int pos1_length = findIntLength(posString);
            WORD pos1 = std::stoi(posString.substr(0, pos1_length));
            WINDOWS_ACTION action = {ACTION_TYPE::INSERT_VKKEY, pos1};
            script.addAction(action);
            std::cout << "Action added successfully. \n";
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
