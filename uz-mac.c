#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>

// macOS Virtual Keycode'lari (ANSI klaviatura uchun):
// O = 31, G = 5, S = 1, C = 8
#define KEY_O 31
#define KEY_G 5
#define KEY_S 1
#define KEY_C 8

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown) {
        return event;
    }

    CGEventFlags flags = CGEventGetFlags(event);
    int64_t keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    // Option (Alt) bosilganligini tekshirish
    bool isOption = (flags & kCGEventFlagMaskAlternate) != 0;
    bool isShift  = (flags & kCGEventFlagMaskShift) != 0;
    bool isCaps   = (flags & kCGEventFlagMaskAlphaShift) != 0;
    bool isUpper  = isShift ^ isCaps; // Katta harf mantiqi

    if (isOption) {
        UniChar targetChar = 0;

        switch (keycode) {
            case KEY_O: targetChar = isUpper ? 0x00D6 : 0x00F6; break; // Õ / õ
            case KEY_G: targetChar = isUpper ? 0x011E : 0x011F; break; // Ğ / ğ
            case KEY_S: targetChar = isUpper ? 0x015E : 0x015F; break; // Ş / ş
            case KEY_C: targetChar = isUpper ? 0x00C7 : 0x00E7; break; // Ç / ç
        }

        if (targetChar != 0) {
            // Hodisadagi belgini yangi Unicode belgiga almashtiramiz
            CGEventKeyboardSetUnicodeString(event, 1, &targetChar);

            // Tizim Option tugmasining standart belgilarini (masalan, ø, ©) aralashtirib yubormasligi uchun bayroqni tozalaymiz
            CGEventSetFlags(event, flags & ~kCGEventFlagMaskAlternate);

            return event;
        }
    }

    return event;
}

int main() {
    // Klaviaturadagi barcha tugma bosilishlarini (KeyDown) ushlaydigan maska
    CGEventMask eventMask = (1 << kCGEventKeyDown);

    // Event Tap yaratish
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        eventCallback,
        NULL
    );

    if (!eventTap) {
        fprintf(stderr, "Xatolik: Event Tap yaratib bo'lmadi!\n");
        fprintf(stderr, "Dasturga 'Accessibility' (Maxsus imkoniyatlar) ruxsati berilganiga ishonch hosil qiling.\n");
        return 1;
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    printf("macOS uchun Yangi O'zbek klaviaturasi faol.\n");
    printf("  Option + O -> ö / Ö\n");
    printf("  Option + G -> ğ / Ğ\n");
    printf("  Option + S -> ş / Ş\n");
    printf("  Option + C -> ç / Ç\n\n");
    printf("To'xtatish uchun: Ctrl + C\n");

    // Asosiy sikl (RunLoop)
    CFRunLoopRun();

    CFRelease(runLoopSource);
    CFRelease(eventTap);
    return 0;
}