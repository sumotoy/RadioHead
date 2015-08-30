#define enableOLED

#include <SPI.h>
#if defined(enableOLED)
#include <OLED_SSD1306.h>
#endif
#include <RH_NRF24.h>

#define OLED_DC     21
#define OLED_CS     10
#define OLED_RESET  6
#define RF_CS         9 //out
#define RF_CE         20 //out<---------- change to 20???
#define RF_INT        2 //in



uint8_t initError = 0b00000000;
boolean readyToGo = false;
volatile int chan[128];


RH_NRF24 nrf24(RF_CE, RF_CS);
#if defined(enableOLED)
OLED_SSD1306 oled(OLED_CS, OLED_DC, OLED_RESET);
#endif



void setup() {

  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("start");

  if (!nrf24.init()) bitSet(initError, 0);
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm)) bitSet(initError, 1);
#if defined(enableOLED)
  oled.begin();
  if (oled.getError() != 0) {
    Serial.println(oled.getError());
    bitSet(initError, 2);
  }
  oled.setCursor(0, 0);
  oled.setTextColor(WHITE);
#endif
  if (initError == 0) {
#if defined(enableOLED)
    oled.print("Radio Spectrum");
#endif
    Serial.println("ready");
    readyToGo = true;
  } else {
#if defined(enableOLED)
    oled.print("error");
#endif
    Serial.println("error");
  }
#if defined(enableOLED)
  oled.display();
#endif
}

void loop() {
  if (readyToGo) {
    for (int j = 0 ; j < 10 ; j++) { // On intègre sur 100 itérations
      for (int i = 0 ; i < 128 ; i++) { // On test les 128 canaux (chaque canal correspond à une fréquence)
        nrf24.setChannel(i);
        nrf24.setModeRx();
        delay(1);
        uint8_t rpd = nrf24.spiReadRegister(RH_NRF24_REG_09_RPD); // 1bit malheureusement
        if (rpd > 0) chan[i]++;
      }
    }

#if defined(enableOLED)
    oled.clearDisplay();
    oled.display();
#endif
    for (int i = 0 ; i < 128 ; i++) { // On affiche le résultat
#if defined(enableOLED)
      int tempV = oled.height() - map(chan[i], 0, 100, 1, oled.height());
      oled.drawFastVLine(i, tempV, oled.height() , WHITE);
#endif
      Serial.print(" ");
      Serial.print(chan[i], DEC);
      chan[i] = 0;
    }
#if defined(enableOLED)
    oled.display();
#endif
    Serial.println();
    delay(100);
  }
}
