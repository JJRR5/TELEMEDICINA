#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

#define sensor 26


DHT dht(sensor,DHT22);

float temp;
float humedad;

void promedio();

int finloop = 0;

float promedioTemp;
float promedioHumedad;

void setup() {
    Serial.begin(115200);

    dht.begin();
}

void loop() {
    temp = dht.readTemperature();

    humedad = dht.readHumidity();

    Serial.println(String(temp)+"C");
    Serial.println(String(humedad)+"%");
    delay(1000);
    promedio();

}


void promedio(){
    if (!(isnan(temp) && isnan(humedad))) {
        if(finloop <= 60){
            promedioTemp += temp;
            promedioHumedad += humedad;
        }
        finloop+=1;
        if(finloop >= 60){
            promedioTemp = promedioTemp / 60;
            promedioHumedad = promedioHumedad / 60;
            
            Serial.println("Temperatura promedio:"+String(promedioTemp)+"C"+"\nHumedad promedio:"+String(promedioHumedad)+"%");

            finloop = 0;
        }else if (finloop < 60) {
            Serial.println("Calculando promedio"); 
        }
    }
    delay(1000);
}