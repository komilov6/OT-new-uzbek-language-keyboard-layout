# Yangi O'zbek Alifbosi Klaviaturasi (C tilida)

Bu loyiha **"Operatsion tizimlar"** fani doirasida amaliy topshiriq sifatida tayyorlangan. Dastur Windows va macOS operatsion tizimlarida C tilidagi tizim chaqiriqlari (System Calls va API) yordamida klaviatura tugmalarini tutib olib, yangi O'zbek lotin alifbosidagi maxsus harflarni (ö, ğ, ş, ç) kiritish imkonini beradi.

**Muallif:** [@komilov6](https://github.com/komilov6)

## Ishlash printsipi

Dastur hech qanday qo'shimcha tizim fayllarini o'zgartirmaydi va ro'yxatdan o'tgan rasmiy klaviaturalarni buzmaydi. U faqatgina **fon rejimida (background process)** ishlaydi. 
* **Windows'da:** `WH_KEYBOARD_LL` past darajadagi ilgagi (Low-level hook) orqali.
* **macOS'da:** `CGEventTap` mexanizmi orqali ishlaydi.

## Fayllar
* `uz-win.c` — Windows OS uchun manba kodi.
* `uz-mac.c` — macOS uchun manba kodi.

---

## 💻 Windows tizimida ishlatish

### Kompilyatsiya qilish
Terminal yoki CMD'da MinGW (gcc) yordamida kodni kompilyatsiya qiling:
```bash
gcc uz-win.c -o uz-win.exe
```

### Ishga tushirish
Kompilyatsiya qilingan faylni ishga tushiring:
```bash
./uz-win.exe
```
Dastur oynasi ochiq turgan vaqtda klaviatura xizmati faol bo'ladi.

### Qanday foydalaniladi?
Tugmalar birikmasi (O'ng `Alt` - AltGr orqali):
* `AltGr + O` -> õ / Õ
* `AltGr + G` -> ğ / Ğ
* `AltGr + S` -> ş / Ş
* `AltGr + C` -> ç / Ç

### Asosiy (standart) klaviaturaga qaytish
Dastur vaqtincha ishlaydi. Odatiy klaviaturaga qaytish uchun shunchaki **ochilib turgan qora konsol oynasini yoping**. Shu zahotiyoq tizim o'zining standart holatiga qaytadi.

---

## 🍏 macOS tizimida ishlatish

### Kompilyatsiya qilish
Terminalda Clang yordamida quyidagi buyruqni bering:
```bash
clang -O2 uz-mac.c -framework ApplicationServices -o uz-mac
```

### Ishga tushirish va Ruxsat berish
Mac tizimida tugmalarni o'zgartirish uchun dasturga **Maxsus imkoniyatlar (Accessibility)** ruxsati kerak bo'ladi. 
1. Terminalda ishga tushiring: `./uz-mac`
2. Agar xatolik bersa, `System Settings -> Privacy & Security -> Accessibility` bo'limiga kirib, Terminal dasturiga ruxsat bering.

### Qanday foydalaniladi?
Tugmalar birikmasi (`Option` - Alt orqali):
* `Option + O` -> ö / Ö
* `Option + G` -> ğ / Ğ
* `Option + S` -> ş / Ş
* `Option + C` -> ç / Ç

### Asosiy (standart) klaviaturaga qaytish
Dasturni to'xtatish va Mac'ning standart klaviaturasiga qaytish uchun Terminal oynasida **`Ctrl + C`** tugmalarini bosing yoki Terminal oynasini yoping.