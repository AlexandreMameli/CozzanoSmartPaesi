/*
  LoRa Simple Client for Arduino :
  Support Devices: LoRa Shield + Arduino 
  
  Example sketch showing how to create a simple messageing client, 
  with the RH_RF95 class. RH_RF95 class does not provide for addressing or
  reliability, so you should only use RH_RF95 if you do not need the higher
  level messaging abilities.
  It is designed to work with the other example LoRa Simple Server
  User need to use the modified RadioHead library from:
  https://github.com/dragino/RadioHead
  modified 16 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/

#include <SPI.h>
#include <RH_RF95.h>

#include "seeed_bme680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10


#define IIC_ADDR  uint8_t(0x76)


/**  NOTE!!!!!!!!!!!!  Select the communication protocol correctly **/

Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */
//Seeed_BME680 bme680;             /* SPI PROTOCOL */
//Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);/*SPI PROTOCOL*/

char* dataBuff;
String dataString = "";

uint8_t id = 2;

union U {
    float f;
    uint8_t i[4];
  };

// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 868.0;

U uTemp, uPress, uHumi, uGas, uF1, uF2, uF3;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);

  // Setup Spreading Factor (6 ~ 12)
  rf95.setSpreadingFactor(7);
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);

  while (!bme680.init()) 
  {
    Serial.println("bme680 init failed ! can't find device!");
  delay(10000);
  }
}

void loop()
{
 if (bme680.read_sensor_data()) 
  {
    Serial.println("Failed to perform reading :(");
  }
  
  uTemp.f = bme680.sensor_result_value.temperature;
  uPress.f = bme680.sensor_result_value.pressure;
  uHumi.f = bme680.sensor_result_value.humidity;
  uGas.f = bme680.sensor_result_value.gas;
  uF1.f = analogRead(A0);
  uF2.f = analogRead(A1);
  uF3.f = analogRead(A2);

  Serial.println(uTemp.f);
  Serial.println(uPress.f);
  Serial.println(uHumi.f);
  Serial.println(uGas.f);
  Serial.println(uF1.f);
  Serial.println(uF2.f);
  Serial.println(uF3.f);
  
  Serial.println("Sending to LoRa Server");
  // Send a message to LoRa Server
  uint8_t dataOut[] = {id, uTemp.i[0], uTemp.i[1], uTemp.i[2], uTemp.i[3], uPress.i[0], uPress.i[1], uPress.i[2], uPress.i[3], uHumi.i[0], uHumi.i[1], uHumi.i[2], uHumi.i[3], uGas.i[0], uGas.i[1], uGas.i[2], uGas.i[3], uF1.i[0], uF1.i[1], uF1.i[2], uF1.i[3], uF2.i[0], uF2.i[1], uF2.i[2], uF2.i[3], uF3.i[0], uF3.i[1], uF3.i[2], uF3.i[3]};
  rf95.send(dataOut, sizeof(dataOut));
  
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);
}
