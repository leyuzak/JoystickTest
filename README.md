# JoystickTest

# Joystick Library (C++)

Linux ortamında `/dev/input/js0` üzerinden joystick verisi okumak için yazılmış basit ve thread-safe bir C++ kütüphanesi.

---

## 🚀 Özellikler

* Non-blocking joystick okuma
* Thread-safe veri erişimi
* Callback desteği
* Enum tabanlı buton kontrolü
* Axis normalize + deadzone

---

## 📁 Dosyalar

* `joystick.h` → kütüphane arayüzü
* `joystick.cpp` → implementasyon
* `main.cpp` → örnek kullanım
* `test.cpp` → basit test

---

## ⚙️ Derleme

```bash
g++ -fPIC -c joystick.cpp -o joystick.o
g++ -shared -o libjoystick.so joystick.o
```

---

## ▶️ Kullanım (Örnek main)

```cpp
#include "joystick.h"
#include <iostream>
#include <unistd.h>

int main() {
    Joystick js;

    js.start();

    while (true) {
        auto data = js.getLatestData();

        std::cout << "X: " << data.x
                  << " Y: " << data.y
                  << " Twist: " << data.twist << std::endl;

        if (data.isPressed(Button::Trigger)) {
            std::cout << "Trigger basildi!" << std::endl;
        }

        usleep(100000); // 100 ms
    }

    js.stop();
}
```

---

## 🧠 Nasıl Çalışır?

* `start()` → ayrı thread başlatır
* joystick sürekli okunur
* veri `ControlData` içine yazılır
* `getLatestData()` ile çekilir

---

## 🎯 Buton Kullanımı

```cpp
if (data.isPressed(Button::Thumb)) {
    // işlem yap
}
```

---

## 📌 Axis Değerleri

| Axis  | Açıklama   |
| ----- | ---------- |
| x     | sağ-sol    |
| y     | ileri-geri |
| twist | dönme      |

Değer aralığı:

```
-1.0  →  1.0
```

---

## ⚠️ Gereksinimler

* Linux
* `/dev/input/js0` erişimi

Gerekirse:

```bash
sudo chmod 666 /dev/input/js0
```

---

## 🔥 Notlar

* Deadzone: 0.05
* Maksimum 13 buton destekli
* 8 axis destekli

---

## 👨‍💻 Geliştirici

leyuzak
