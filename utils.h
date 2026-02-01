//
// Created by Micael Cossa on 01/02/2026.
//

#ifndef ACTION_RECORDER_UTILS_H
#define ACTION_RECORDER_UTILS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void registerHookThread(HHOOK& hook, DWORD& hookWinThreadID, HOOKPROC function);
void sendKey(WORD vk, bool keyUp);
void sendChar(wchar_t ch);

#endif //ACTION_RECORDER_UTILS_H
