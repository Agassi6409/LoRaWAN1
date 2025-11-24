#include <Arduino.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

static const int  LED_PIN = 2; //LED (GPIO2, aktív-HIGH) 
static const bool LED_ACTIVE_LOW = false;  //aktív-HIGH

static inline void led_on() {
    digitalWrite(LED_PIN, LED_ACTIVE_LOW ? LOW : HIGH);
}
static inline void led_off() {
    digitalWrite(LED_PIN, LED_ACTIVE_LOW ? HIGH : LOW);
}

static const u4_t DEVADDR = 0xFC003F0F;
static uint8_t NWKSKEY[16] = {
  0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
  0xAA,0xAA,0xAA,0x9A,0x9A,0x9A,0x9A,0x9A
};
static uint8_t APPSKEY[16] = {
  0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
  0xAA,0xAA,0xAA,0x9A,0x9A,0x9A,0x9A,0x9A
};

// LMIC pinmap: ss=17,  dio0=23, dio1=33, rst=22
const lmic_pinmap lmic_pins = {
  .nss  = 17,
  .rxtx = LMIC_UNUSED_PIN,
  .rst  = 22,
  .dio  = { 23, 33, LMIC_UNUSED_PIN }
};

// ---- Payload és periódus ----
static const uint8_t HELLO[] = "Hello";
const uint32_t UPLINK_PERIOD = 10000;   // 10 s

void os_getDevKey (u1_t*) {} //Linker miatt üres OTAA hook-
void os_getArtEui (u1_t*) {}
void os_getDevEui (u1_t*) {}

void onEvent(ev_t ev) { // ---- LMIC eseménykezelő ----
  switch (ev) {
    case EV_TXSTART:     
      led_on(); //TX alatt LED ON, az első RX slot elején OFF       
      break;
    case EV_RXSTART:
      led_off(); //RX1 kezdődik -> ne égjen tovább
      break;
    case EV_TXCOMPLETE:
      led_off();        // biztos, ami biztos
      Serial.println(F("EV_TXCOMPLETE"));
      break;
    default:
      break;
  }
}

static void do_send() {
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, skip"));
    return;
  }
 
LMIC_setTxData2(
    1,                   // FPort=1 (1..223 tartomány)
    (xref2u1_t)HELLO,    // Payload pointer
    sizeof(HELLO) - 1, // Payload length (-1: a '\0' nélkül)
    0                    // Confirmed uplink? (0 = nem az)
);
  Serial.println(F("Uplink queued: \"Hello\" (FPort=1)"));
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  pinMode(LED_PIN, OUTPUT);
  led_off();               // induláskor legyen kikapcsolva
  // ESP32 SPI pinek (SCK, MISO, MOSI, SS)
  SPI.begin(18, 16, 19, 17);
  os_init();
  LMIC_reset();
  // ABP session netid=0x000000
  LMIC_setSession(0x000000, DEVADDR, NWKSKEY, APPSKEY);
  // EU868
  LMIC_setAdrMode(1);              // opcionális
  LMIC_setDrTxpow(DR_SF7, 14);    // ha nem bízzuk ADR-re
  do_send();
}

void loop() {
  os_runloop_once();
  static uint32_t last = 0;
  const uint32_t now = millis();
  if (now - last >= UPLINK_PERIOD) {
    last = now;
    do_send();
  }
}
