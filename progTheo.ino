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

typedef struct realArray{
    char elName;
    int value;
  }realArray;

void printArr(realArray* arr){
    Serial.print("{");
    for(int i = 0; i < (sizeof(arr) / sizeof(int)); i++){
        Serial.print("{");
        Serial.print(arr[i].elName);
        Serial.print(" , ");
        Serial.print(arr[i].value);
        Serial.print("}");
          if(i < (sizeof(arr) / sizeof(int)) - 1){
            Serial.print(" , ");
          }
    }
    Serial.print("}");
    Serial.println("");
}

realArray data[7];

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

  for(int i = 0; i < 7; i++){ //set array to zero
    data[i].value = 0;  
  }

  data[0].elName = "temperature";  //adding the names
  data[1].elName = "pressure";
  data[2].elName = "humidity";
  data[3].elName = "gas";
  data[4].elName = "flameGreen";
  data[5].elName = "flameBrown";
  data[6].elName = "flameBlue";
}

void loop() {
  if (bme680.read_sensor_data()) 
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("temperature ===>> ");
  Serial.print(bme680.sensor_result_value.temperature);
  Serial.println(" C");

  if(abs(data[0].value - bme680.sensor_result_value.temperature) >= 0.3){
      data[0].value = bme680.sensor_result_value.temperature;
    }

  Serial.print("pressure ===>> ");
  Serial.print(bme680.sensor_result_value.pressure/ 1000.0);
  Serial.println(" KPa");

  if(abs(data[1].value - bme680.sensor_result_value.pressure) >= 0.10){
      data[1].value = bme680.sensor_result_value.pressure;
    }

  Serial.print("humidity ===>> ");
  Serial.print(bme680.sensor_result_value.humidity);
  Serial.println(" %");

  if(abs(data[2].value - bme680.sensor_result_value.humidity) >= 0.10){
      data[2].value = bme680.sensor_result_value.humidity;
    }

  Serial.print("gas ===>> ");
  Serial.print(bme680.sensor_result_value.gas/ 1000.0);
  Serial.println(" Kohms");

  if(abs(data[3].value - bme680.sensor_result_value.gas) >= 1){
      data[3].value = bme680.sensor_result_value.gas;
    }

  Serial.print("flameVert ===>> ");
 Serial.print(analogRead(A1)); Serial.print(" "); Serial.print(digitalRead(1));
  Serial.println();

  if(abs(data[4].value - analogRead(A1)) >= 10){
      data[4].value = analogRead(A1);
    }
  
  Serial.print("flameBrown ===>> ");
  Serial.print(analogRead(A0)); Serial.print(" "); Serial.print(digitalRead(0));
  Serial.println();
  
  if(abs(data[5].value - analogRead(A0)) >= 10){
      data[5].value = analogRead(A0);
    }

  Serial.print("flameBlue ===>> ");
Serial.print(analogRead(A2)); Serial.print(" "); Serial.print(digitalRead(2));
  Serial.println();
  
  if(abs(data[6].value - analogRead(A2)) >= 10){
      data[6].value = analogRead(A2);
    }
  
  Serial.println();
  Serial.println();

  printArr(data);
  
  delay(2000);
}
