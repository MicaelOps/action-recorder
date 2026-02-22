//
// Created by Micael Cossa on 01/02/2026.
//

#ifndef ACTION_RECORDER_SCRIPT_H
#define ACTION_RECORDER_SCRIPT_H

#include <memory>
#include <variant>
#include <string>

class ActionsScript;

enum class ACTION_TYPE : unsigned short {

    RIGHT_MOUSE_CLICK = 0,
    LEFT_MOUSE_CLICK = 1,
    KEYBOARD_CONTROL_COMBO = 2, // CTRL C OR CTRL V
    KEYBOARD_TEXT = 3,
    PASTE_CLIPBOARD = 4, // PASTING CLIPBOARD WITHOUT CTRL V
    SPECIAL_FUNCTION = 5, // RELATED TO MY JOB <3
    INSERT_VKKEY = 6,
    WAIT_MILLISECONDS = 7,
    DRAG_MOUSE = 8,
    SPECIAL_FUNCTION2 = 9,
    SPECIAL_FUNCTION3 = 10,
    SPECIAL_FUNCTION4 = 12,
    SPECIAL_FUNCTION5 = 13,
    REPEAT_SCRIPT = 11,

};

struct LOCATION {
    long X =0, Y = 0;
};

struct WINDOWS_ACTION {
    ACTION_TYPE action;
    std::variant<std::monostate, std::string, LOCATION, unsigned short, int, std::pair<LOCATION, LOCATION>, std::pair<std::string, int>> data; // Location for mouse clicks, text for keyboard

    [[nodiscard]] std::string_view getActionName() const noexcept;
    [[nodiscard]] bool playAction(bool repeaterCall) const noexcept;


    void printAction() const noexcept;

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

    ActionsScript(ActionsScript&&) noexcept = default;
    ActionsScript& operator=(ActionsScript&&) noexcept = default;
    explicit ActionsScript(std::string name) : name(std::move(name)), header(nullptr) {}

    void insertActionAt(WINDOWS_ACTION action, int pos);
    void moveAction(int oldpos, int newpos);
    void swapAction(int pos1, int pos2);

    void addAction(WINDOWS_ACTION action);

    void playAllActions(bool repeaterCall) const;
    void printAllActions() const;

    [[nodiscard]] std::string_view getName() const noexcept;

    [[nodiscard]] int getNumberOfActions() const noexcept;

    void removeAction(int pos);
};


#endif //ACTION_RECORDER_SCRIPT_H
