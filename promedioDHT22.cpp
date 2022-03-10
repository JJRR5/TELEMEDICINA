#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BluetoothSerial.h>

BluetoothSerial bt;

int sensor = 26;
float temp;
float humedad;
int finloop = 0;

float promedioTemp;
float promedioHumedad;

DHT dht(sensor,DHT22);

void promedio();

char mensaje;

void setup() {
  Serial.begin(115000);
  dht.begin();
  bt.begin("PEPE CRACK");
}

void loop() {
  temp = dht.readTemperature();

  humedad = dht.readHumidity();

  mensaje = bt.read();

  String prueba = String(bt.read());

  if (prueba != "-1"){
    if(mensaje == 'A'){
      bt.println(String(temp)+"C");
    } else if (mensaje == 'B'){
      bt.println(String(humedad)+"%");
    }
    else if(mensaje == 'C'){
      bt.println("");
    }
    else{
      bt.println("Comando no valido");
    }
  }
  promedio();
}

void promedio(){
  if (!(isnan(temp) && isnan(humedad))) {
    if(finloop <= 60){
      promedioTemp += temp;
      promedioHumedad += humedad;
    }
    finloop+=1;
    if(finloop >= 60 && mensaje == 'C'){
      promedioTemp = promedioTemp / 60;
      promedioHumedad = promedioHumedad / 60;
      bt.println("Temperatura promedio: "+String(promedioTemp)+"ºC"+"\nHumedad promedio: "+String(promedioHumedad)+"%");
      finloop = 0;
    }else if (finloop < 60 && mensaje == 'C') {
      bt.println("El promedio aun no esta listo");
    }
  }
  delay(1000);
}