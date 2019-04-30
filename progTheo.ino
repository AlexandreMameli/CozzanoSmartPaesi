#include "seeed_bme680.h"

#include <stdio.h>
#include <stdlib.h>

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

union U {
    float f;
    uint8_t i[4];
  };

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial start!!!");
  delay(100);
  while (!bme680.init()) 
  {
    Serial.println("bme680 init failed ! can't find device!");
  delay(10000);
  }

  U u;
}

void loop() {
  if (bme680.read_sensor_data()) 
  {
    Serial.println("Failed to perform reading :(");
    return;
  }

  U uTemp, uPress, uHumi, uGas, uF1, uF2, uF3;

  uTemp.f = bme680.sensor_result_value.temperature;
  uPress.f = bme680.sensor_result_value.pressure;
  uHumi.f = bme680.sensor_result_value.humidity;
  uGas.f = bme680.sensor_result_value.gas;
  uF1.f = analogRead(A0);
  uF2.f = analogRead(A1);
  uF3.f = analogRead(A2);
  
  Serial.print(bme680.sensor_result_value.temperature);
  Serial.print(",");
  Serial.print(bme680.sensor_result_value.pressure);
  Serial.print(",");
  Serial.print(bme680.sensor_result_value.humidity);
  Serial.print(",");
  Serial.print(bme680.sensor_result_value.gas);
  Serial.print(",");
  Serial.print(analogRead(A0));
  Serial.print(",");
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.print(analogRead(A2));
  Serial.println();
  
  delay(2000);
}
