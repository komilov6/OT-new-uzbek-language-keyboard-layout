#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>

HHOOK hHook = NULL;

void SendUnicode(wchar_t ch) {
    INPUT input[2] = {0};

    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wScan = ch;
    input[0].ki.dwFlags = KEYEVENTF_UNICODE;

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wScan = ch;
    input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

        // O'ng Alt (AltGr) yoki Ctrl + Alt holatini aniqlash
        BOOL isRAlt = (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
        BOOL isCtrlAlt = ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0) &&
                         ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0);
        BOOL isAltGr = isRAlt || isCtrlAlt;

        BOOL isShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        BOOL isCaps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        BOOL isUpper = isShift ^ isCaps; // Shift va CapsLock mantiqiy XOR

        if (isAltGr) {
            switch (p->vkCode) {
                // Õ õ
                case 'O':
                    SendUnicode(isUpper ? 0x00D5 : 0x00F5);
                    return 1;

                // Ğ ğ
                case 'G':
                    SendUnicode(isUpper ? 0x011E : 0x011F);
                    return 1;

                // Ş ş
                case 'S':
                    SendUnicode(isUpper ? 0x015E : 0x015F);
                    return 1;

                // Ç ç
                case 'C':
                    SendUnicode(isUpper ? 0x00C7 : 0x00E7);
                    return 1;
            }
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    if (!hHook) {
        printf("Xatolik: Hook o'rnatilmadi!\n");
        return 1;
    }

    printf("Yangi o'zbek alifbosi klaviaturasi faol.\n");
    printf("Tugmalar birikmasi (O'ng Alt orqali):\n");
    printf("  AltGr + O -> õ / Õ\n");
    printf("  AltGr + G -> ğ / Ğ\n");
    printf("  AltGr + S -> ş / Ş\n");
    printf("  AltGr + C -> ç / Ç\n\n");
    printf("To'xtatish uchun konsol oynasini yoping.\n");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}