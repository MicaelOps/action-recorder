//
// Created by Micael Cossa on 01/02/2026.
//

#ifndef ACTION_RECORDER_HARDCODED_H
#define ACTION_RECORDER_HARDCODED_H

#include "script.h"

ActionsScript registerHard() {
    ActionsScript hardcoded("final");

    // 1. LEFT MOUSE CLICK Pos (695, 265) file explorer icon
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{956, 1054}});

    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1907, 923}});

    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1425, 321}});

    // 1. LEFT MOUSE CLICK Pos (695, 265) FILE NAME
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1507, 269}});

    // 2. WAIT_MILLISECONDS Number: 400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 400});

    // 3. LEFT MOUSE CLICK Pos (695, 265) FILE NAME
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1507, 269}});

    // 4. WAIT_MILLISECONDS Number: 400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 400});
    // 5. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});
    // 5. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 5. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 6. LEFT MOUSE CLICK Pos (1362, 1029) NOTEPAD TASKBAR ICON
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    // 7. LEFT MOUSE CLICK Pos (1200, 594) NOTEPAD TAB
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{230, 34}});

    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 200});

    // 8. SPECIAL_FUNCTION2
    hardcoded.addAction({ACTION_TYPE::SPECIAL_FUNCTION2, std::monostate()});

    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 200});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 9. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 10. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{35, 147}});

    // 11. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{35, 147}});

    // 12. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{35, 147}});

    // 13. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});
    // 13. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});
    // 13. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) tab mri
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{575, 14}});

    // close advanced search
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1707, 270}});

    // 15. LEFT MOUSE CLICK Pos (1723, 155) lamp
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1736, 131}});

    // 16. LEFT MOUSE CLICK Pos (1458, 155) text
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1458, 140}});

    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});
    // 17. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});


    // 18. LEFT MOUSE CLICK Pos (1403, 182)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1370, 186}});

    // 19. WAIT_MILLISECONDS Number: 2400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 4400});

    // fk hihg
    //hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1472, 690}});

    // 20. KEYBOARD CTRL + LETTER Text: a
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 21. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 200});

    // 22. SPECIAL FUNCTION
    hardcoded.addAction({ACTION_TYPE::SPECIAL_FUNCTION, std::monostate()});
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 200});

    // 23. LEFT MOUSE CLICK Pos (1485, 51) IDOX TAB
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{898, 15}});

    // fix unknown errors

    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1213, 271}}); // date issue
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1224, 245}}); // insert one or more

    hardcoded.addAction({ACTION_TYPE::DRAG_MOUSE,
                         std::pair<LOCATION, LOCATION>{LOCATION{1523, 150}, LOCATION{1523, 150}}});

    //file remove clicks
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 386}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 376}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 378}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 380}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 382}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{840, 384}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{835, 543}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{835, 523}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{835, 513}});
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{835, 503}});

    // 24. LEFT MOUSE CLICK Pos (1769, 575) URPN
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1165, 444}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 26. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 1. LEFT MOUSE CLICK Pos (695, 265) file explorer icon
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{956, 1054}});

    hardcoded.addAction({ACTION_TYPE::DRAG_MOUSE,
                         std::pair<LOCATION, LOCATION>{LOCATION{1562, 280}, LOCATION{606, 455}}});

    // 1. LEFT MOUSE CLICK Pos (695, 265) file explorer icon
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{956, 1054}});


    // 29. LEFT MOUSE CLICK Pos (1362, 1029) NOTEPAD TASKBAR WINDOW
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    // 30. LEFT MOUSE CLICK Pos (1200, 594)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{230, 34}});

    // 31. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{34, 173}});

    // 32. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{34, 173}});

    // 33. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{34, 173}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 150});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // 35. LEFT MOUSE CLICK Pos (1485, 51) IDOX TAB?
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{898, 15}});

    // 36. LEFT MOUSE CLICK Pos (1781, 811) idox date
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1235, 682}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 37. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});


    // 38. LEFT MOUSE CLICK Pos (1362, 1029) notepad taskbar icon
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{230, 34}});

    // 40. LEFT MOUSE CLICK Pos (872, 376) CODE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{36, 204}});

    // 41. LEFT MOUSE CLICK Pos (872, 376)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{36, 204}});

    // 42. LEFT MOUSE CLICK Pos (872, 376)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{37, 204}});

    // 43. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 100});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // idox tab
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{898, 15}});

    // 45. LEFT MOUSE CLICK Pos (1780, 737) idox document code
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1216, 619}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 46. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 45. LEFT MOUSE CLICK Pos (1780, 737) insert
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1100, 762}});

    return hardcoded;
}
#endif //ACTION_RECORDER_HARDCODED_H
