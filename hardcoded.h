//
// Created by Micael Cossa on 01/02/2026.
//

#ifndef ACTION_RECORDER_HARDCODED_H
#define ACTION_RECORDER_HARDCODED_H

#include "script.h"

ActionsScript registerHard() {
    ActionsScript hardcoded("final");

    // 1. LEFT MOUSE CLICK Pos (695, 265) FILE NAME
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{695, 265}});

    // 2. WAIT_MILLISECONDS Number: 400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 400});

    // 3. LEFT MOUSE CLICK Pos (695, 265) FILE NAME
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{695, 265}});

    // 4. WAIT_MILLISECONDS Number: 400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 400});

    // 5. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 6. LEFT MOUSE CLICK Pos (1362, 1029) NOTEPAD TASKBAR ICON
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    // 7. LEFT MOUSE CLICK Pos (1200, 594) NOTEPAD TAB
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1200, 594}});

    // 8. SPECIAL_FUNCTION2
    hardcoded.addAction({ACTION_TYPE::SPECIAL_FUNCTION2, std::monostate()});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 9. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 10. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1016, 148}});

    // 11. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1016, 148}});

    // 12. LEFT MOUSE CLICK Pos (973, 333) ADDRESS LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1016, 148}});

    // 13. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // 14. LEFT MOUSE CLICK Pos (1297, 51) S
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1297, 51}});

    // 15. LEFT MOUSE CLICK Pos (1723, 155)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1723, 155}});

    // 16. LEFT MOUSE CLICK Pos (1458, 155)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1458, 155}});

    // 17. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 18. LEFT MOUSE CLICK Pos (1403, 182)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1403, 182}});

    // 19. WAIT_MILLISECONDS Number: 2400
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 4400});

    // 20. KEYBOARD CTRL + LETTER Text: a
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 21. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});

    // 22. SPECIAL FUNCTION
    hardcoded.addAction({ACTION_TYPE::SPECIAL_FUNCTION, std::monostate()});

    // 23. LEFT MOUSE CLICK Pos (1485, 51) IDOX TAB
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1485, 51}});

    // 24. LEFT MOUSE CLICK Pos (1769, 575)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1769, 575}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 26. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 27. DRAG_MOUSE From (695, 265) To (1455, 173)
    hardcoded.addAction({ACTION_TYPE::DRAG_MOUSE,
                         std::pair<LOCATION, LOCATION>{LOCATION{695, 265}, LOCATION{1455, 173}}});

    // 28. WAIT_MILLISECONDS Number: 2000
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 2000});

    // 29. LEFT MOUSE CLICK Pos (1362, 1029) NOTEPAD TASKBAR WINDOW
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    // 30. LEFT MOUSE CLICK Pos (1200, 594)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1200, 594}});

    // 31. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{990, 177}});

    // 32. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{990, 177}});

    // 33. LEFT MOUSE CLICK Pos (871, 352) DATE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{990, 177}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});


    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // 35. LEFT MOUSE CLICK Pos (1485, 51) IDOX TAB?
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1485, 51}});

    // 36. LEFT MOUSE CLICK Pos (1781, 811)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1781, 811}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 37. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 38. LEFT MOUSE CLICK Pos (1362, 1029)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1362, 1029}});

    // 39. LEFT MOUSE CLICK Pos (1200, 594)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1200, 594}});

    // 40. LEFT MOUSE CLICK Pos (872, 376) CODE LINE
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{996, 195}});

    // 41. LEFT MOUSE CLICK Pos (872, 376)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{996, 195}});

    // 42. LEFT MOUSE CLICK Pos (872, 376)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{996, 195}});

    // 43. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("c")});


    // 14. LEFT MOUSE CLICK Pos (1297, 51) BROWSER TASKBAR
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1023, 1043}});

    // 44. LEFT MOUSE CLICK Pos (1485, 51)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1485, 51}});

    // 45. LEFT MOUSE CLICK Pos (1780, 737)
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1780, 737}});

    // 34. KEYBOARD CTRL + LETTER Text: c
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("a")});

    // 46. KEYBOARD CTRL + LETTER Text: v
    hardcoded.addAction({ACTION_TYPE::KEYBOARD_CONTROL_COMBO, std::string("v")});

    // 45. LEFT MOUSE CLICK Pos (1780, 737) insert
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{1694, 895}});

    // 28. WAIT_MILLISECONDS Number: 2000
    hardcoded.addAction({ACTION_TYPE::WAIT_MILLISECONDS, 2000});

    // 27. DRAG_MOUSE From (695, 265) To (1455, 173)
    hardcoded.addAction({ACTION_TYPE::DRAG_MOUSE,
                         std::pair<LOCATION, LOCATION>{LOCATION{1907, 185}, LOCATION{1903, 918}}});

    // 45. LEFT MOUSE CLICK Pos (1780, 737) insert
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{600, 40}});

    // 45. LEFT MOUSE CLICK Pos (1780, 737) insert
    hardcoded.addAction({ACTION_TYPE::LEFT_MOUSE_CLICK, LOCATION{937, 920}});

    return hardcoded;
}
#endif //ACTION_RECORDER_HARDCODED_H
