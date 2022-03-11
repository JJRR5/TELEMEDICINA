#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BluetoothSerial.h>

BluetoothSerial bt;

int sensor = 26;
float temp;
float humedad;
int finloop = 0;

#define LED 2

float promedioTemp;
float promedioHumedad;

DHT dht(sensor,DHT22);

void promedio();

void mensajeDisplay(String msj,int x , int y, int size);

char mensaje;


#define width 128
#define height 64

#define oled_reset -1
#define oled_adress 0x3C

Adafruit_SSD1306 display(width, height, &Wire, oled_reset); 

void limpiar();

void setup() {

  Serial.begin(115200);

  pinMode(LED,OUTPUT);


  dht.begin();

  bt.begin("PEPE CRACK");

  if(!display.begin(SSD1306_SWITCHCAPVCC,oled_adress)){
    Serial.println(F("Fallo en la asignación de SSD1306"));
  }
  limpiar();

  if(bt.begin("PEPE CRACK")){
    digitalWrite(LED,HIGH);
  }
  delay(1000);
  mensajeDisplay("Welcome :)",0,0,2);
  

}
void loop() {
  temp = dht.readTemperature();

  humedad = dht.readHumidity();

  mensaje = bt.read();

  String prueba = String(bt.read());

  if (prueba != "-1"){
    if(mensaje == 'A'){
      bt.println(String(temp)+"C");
      mensajeDisplay(String(temp)+"C",0,20,3);
    } else if (mensaje == 'B'){
      bt.println(String(humedad)+"%");
      mensajeDisplay(String(humedad)+"%",0,20,3);
    }
    else if(mensaje == 'C'){
      bt.println("");
    }
    else if(mensaje == 'L'){
      limpiar();
    }
    else{
      bt.println("Comando no valido");
      mensajeDisplay("Comando no valido",0,0,2);
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
      
      bt.println("Temperatura promedio:"+String(promedioTemp)+"C"+"\nHumedad promedio:"+String(promedioHumedad)+"%");
      mensajeDisplay(String("Promedio\n")+"T:"+String(promedioTemp)+"C"+"\nH:"+String(promedioHumedad)+"%",0,0,2);

      finloop = 0;
    }else if (finloop < 60 && mensaje == 'C') {
      mensajeDisplay("Calculando promedio",0,0,2);
      bt.println("Calculando promedio"); 
    }
  }
  delay(1000);
}

void limpiar(){
  display.clearDisplay();
  display.display();
}

void mensajeDisplay(String msj, int x = 0, int y = 0,int size = 1){
  limpiar();
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(msj);
  display.display();
}