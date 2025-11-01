//#define CFG_sx1276_radio 1; //#define CFG_sx1261_radio 1
//#define CFG_sx1262_radio 1

#pragma once
// MCCI LMIC – projektkonfiguráció (globális)
// EU868 + SX1276/SX127x

// --- Régió és rádió ---
#define CFG_eu868         1          // EU 863–870 MHz
#define CFG_sx1276_radio  1          // SX1276/77/78/79

// --- Memória spórolás / Class-B kikapcs ---
#define DISABLE_PING      1          // Class-B ping slotok kikapcs
#define DISABLE_BEACONS   1          // Class-B beacons kikapcs

// --- Debug (opcionális) ---
// 0 = nincs, 1 = alap, 2 = bőbeszédű
#define LMIC_DEBUG_LEVEL  2
// az LMIC belső printf-je a Serial-ra átirányyítva:
#define LMIC_PRINTF_TO    Serial

// --- RX időzítés robusztusabbá tétele ESP32-n ---
// kicsit nagyvonalúbb (±0.5–1% körüli) hibakeret az órajelre,
// instabil időzítés / WiFi zaj esetén segít megtalálni az RX ablakot.
#define LMIC_CLOCK_ERROR_PPM  10000   // 1.0%  (5–10k tipikus érték)

// --- SPI órajel (opcionális) hosszú vezeték esetén érdemes lassítani: ---
#define LMIC_SPI_FREQ  100000    

// --- EU868 RX2 (Az LMIC default: 869.525 MHz @ SF12/125 kHz ) Módosítás 
//// #define LMIC_DNW2DR   DR_SF12
//// #define LMIC_RX2_FREQ 869525000