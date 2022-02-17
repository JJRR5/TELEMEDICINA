#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BluetoothSerial.h>

BluetoothSerial serialbt;

int sensor = 26;
float temp;
float humedad;
int finloop = 1;

float promedioTemp;
float promedioHumedad;

DHT dht(sensor,DHT22);

void promedio();

void setup() {
  Serial.begin(115000);
  dht.begin();
  serialbt.begin("PEPE CRACK");
  if(!serialbt.begin("PEPE CRACK")){
    serialbt.println("Ocurrio un error al iniciar el  Bluetooth");
  }
}

void loop() {
  while(serialbt.available()){
    temp = dht.readTemperature();
    humedad = dht.readHumidity();
    promedio();
  }
}

void promedio(){
  if (!(isnan(temp) && isnan(humedad))) {
    promedioTemp += temp;
    promedioHumedad += humedad;
    finloop+=1;
    if(finloop >= 60){
      promedioTemp = promedioTemp / 60;
      promedioHumedad = promedioHumedad / 60;
      serialbt.println("Temperatura promedio: "+String(promedioTemp)+"ºC"+"\nHumedad promedio: "+String(promedioHumedad)+"%");
      finloop = 1;
    }
  }else{
    Serial.println("Error de medicion");
  }
  delay(1000);
}
