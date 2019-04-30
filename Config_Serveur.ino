/*
  LoRa Simple Arduino Server :
  Support Devices: 
  * LoRa Mini
  * LoRa Shield + Arduino;
  * LoRa GPS Shield + Arduino. 
  
  Example sketch showing how to create a simple messageing server, 
  with the RH_RF95 class. RH_RF95 class does not provide for addressing or
  reliability, so you should only use RH_RF95 if you do not need the higher
  level messaging abilities.

  It is designed to work with the other example LoRa Simple Client

  modified 16 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/

#include <SPI.h>
#include <RH_RF95.h>
#define  BUF_SIZE     20
// Singleton instance of the radio driver
RH_RF95 rf95;

int led = A2;
float frequency = 868.0;
uint8_t id;

union U{
  float f;
  uint8_t i[4];
};

U Utemp, Uhumid, Upress, Ugas, Uflame1, Uflame2, Uflame3;

void setup() 
{
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  Serial.println("Start Sketch");
  if (!rf95.init())
    Serial.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults BW Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  Serial.print("Listening on frequency: ");
  Serial.println(frequency);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len) && buf[0]==2)
    {
      digitalWrite(led, HIGH);
      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      //Parser les données reçues

      for(int j = 0 ; j <= 3 ; j++){
      Utemp.i[j] = buf[j+1];
      Upress.i[j] = buf[j+5];
      Uhumid.i[j] = buf[j+9];
      Ugas.i[j] = buf[j+13];
      Uflame1.i[j] = buf[j+17];
      Uflame2.i[j] = buf[j+21];
      Uflame3.i[j] = buf[j+25];
      }
      
      Serial.println(Utemp.f);
      Serial.println(Upress.f);
      Serial.println(Uhumid.f);
      Serial.println(Ugas.f);
      Serial.println(Uflame1.f);
      Serial.println(Uflame2.f);
      Serial.println(Uflame3.f);
     

      // Send a reply
      uint8_t data[] = "Serveur quentin (test 2)";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("recv failed (Filtrage ID)");
    }
  }

}
