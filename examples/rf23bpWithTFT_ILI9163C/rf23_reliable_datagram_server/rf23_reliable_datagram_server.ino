// rf23bp_reliable_datagram_server
// with TFT_ILI9163C and sumotoy modified radioHead library
// that allow use correctly SPI transaction compatible devices
// in the same SPI lines.
// Tested (and worked) with Teensy 3.2, Teensy 3.0 and Teensy LC

#include <RHReliableDatagram.h>
#include <RH_RF22.h>
#include <SPI.h>
#include <TFT_ILI9163C.h>

#define __CS_TFT        10
#define __DC_TFT        9
#define __BCK_TFT       3

#define __CS_RFN        20
#define __INT_RFN       2

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

volatile bool inited = false;

TFT_ILI9163C tft = TFT_ILI9163C(__CS_TFT, __DC_TFT);
// Singleton instance of the radio driver
RH_RF22 driver = RH_RF22(__CS_RFN, __INT_RFN);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup()
{
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000));
  Serial.println("started");
  if (manager.init()) inited = true;
  tft.begin(true);
  tft.useBacklight(__BCK_TFT);
  tft.backlight(1);
  if (!inited) {
    tft.println("init failed");
  } else {
    if (driver.setFrequency(868.00)) {
      driver.setTxPower(RH_RF22_RF23BP_TXPOW_29DBM);
      tft.print("receive at 868Mhz");
    } else {
      tft.setTextColor(RED);
      tft.print("Failed!");
      inited = false;
    }
    delay(2000);
    tft.clearScreen();
  }
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
}

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];

void loop()
{
  if (inited && manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    tft.clearScreen();
    if (manager.recvfromAck(buf, &len, &from))
    {
      tft.setTextColor(WHITE);
      tft.setCursor(0, 0);
      tft.print("req from:");
      tft.setTextColor(GREEN);
      tft.print("0x");
      tft.println(from, HEX);
      tft.setTextColor(YELLOW);
      tft.println((char*)buf);
      tft.setTextColor(CYAN);
      tft.print("RSSI:");
      tft.print(driver.lastRssi(),DEC);
      tft.println(" dbn");
      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from)) {
        tft.setTextColor(RED);
        tft.println("sendtoWait failed");
      }
    }
  }
}
