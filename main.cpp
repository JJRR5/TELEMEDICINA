#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include "BluetoothSerial.h"

BluetoothSerial BT;

int led = 2;

char respuesta;

void setup() {
    Serial.begin(115200);

    pinMode(led,OUTPUT);
    digitalWrite(led,LOW);

    if(BT.begin("PEPE PRO")){
      BT.println("Conectado");
    };
    
}

void loop() {
  while(BT.available()){
    respuesta = BT.read();
    if (respuesta == 'A'){
      digitalWrite(led,HIGH);    
    } 
    else if (respuesta == 'B'){
      digitalWrite(led,LOW);
    }
  }
}

// String leerDatos(){
// if(BT.available()){
//   respuesta = SerialBT.readString();
// }
//   return respuesta;
//   delay(50);
// }