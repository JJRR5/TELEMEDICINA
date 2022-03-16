#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BluetoothSerial.h>

// Buttons
#define button1 13
#define button2 12
#define button3 14
//END BUTTONS

// LED
#define buildInLed 2

// BT
BluetoothSerial bt;

// DHT 
int sensor = 26;
int sensor2 = 18;

float temp;

float temp2 ;

float promedioTemp;

DHT dht(sensor,DHT22);
DHT dht2(sensor2,DHT22);
// END DHT

// OLED
#define width 128
#define height 64

//leds
#define R  25
#define G  33
#define B  32

#define oled_reset -1
#define oled_adress 0x3C

Adafruit_SSD1306 display(width, height, &Wire, oled_reset); 
// END OLED

// FUNCTION DECLARATION
void limpiar();
float promedio(float temp1, float temp2);
void mensajeDisplay(String msj,int x , int y, int size);

char mensaje;


void setup() {
  Serial.begin(115200);

  pinMode(buildInLed,OUTPUT); //LEbuildInLed BT

  pinMode(button1,INPUT_PULLUP); 
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);

  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);

  digitalWrite(R,LOW);
  digitalWrite(G,LOW);
  digitalWrite(B,LOW);

  dht.begin(); //SENSOR

  bt.begin("PEPE CRACK"); //BT NAME

  if(!display.begin(SSD1306_SWITCHCAPVCC,oled_adress)){
    Serial.println(F("Fallo en la asignación de SSD1306"));
  }

  limpiar(); 

  if(bt.begin("PEPE CRACK")){
    digitalWrite(buildInLed,HIGH);
  }
  delay(1000);
  mensajeDisplay("Welcome :)",0,0,2);
}




void loop() {
  temp = dht.readTemperature();

  temp2 = dht2.readTemperature();

  mensaje = bt.read();

  String prueba = String(bt.read());

  if (prueba != "-1"){
      if(temp < 20){
        digitalWrite(R,LOW);
        digitalWrite(G,LOW);
        digitalWrite(B,LOW);
        mensajeDisplay("La temperatura no es adecuada",0,0,1);
      }
      else if((digitalRead(button1) || mensaje == 'A') && temp >= 20 && temp < 27 ){
        float final = promedio(temp,temp2);
        mensajeDisplay("Promededio: "+String(final)+"C",0,0,2);
        digitalWrite(R,HIGH);
        digitalWrite(G,LOW);
        digitalWrite(B,LOW);


      } else if ((digitalRead(button2) || mensaje == 'B') && temp >= 27 && temp <34){
        float final = promedio(temp,temp2);
        mensajeDisplay("Promededio: "+String(final)+"C",0,0,2);
        digitalWrite(G,HIGH);
        digitalWrite(R,LOW);
        digitalWrite(B,LOW);

      }
      else if((digitalRead(button3) || mensaje == 'C') && temp >= 34 && temp < 40.1 ){
        float final = promedio(temp,temp2);
        mensajeDisplay("Promededio: "+String(final)+"C",0,0,2);
        digitalWrite(G,LOW);
        digitalWrite(B,HIGH);
        digitalWrite(R,LOW);

      }
      else if (temp > 40.1){
        digitalWrite(G,HIGH);
        digitalWrite(B,HIGH);
        digitalWrite(R,HIGH);
        mensajeDisplay("¡Error!,Demasiado caliente",0,0,1);
      }
      else{
        bt.println("Comando no valido");
        mensajeDisplay("Comando no valido",0,0,2);
      }
  }

}

// Functions

float  promedio(float temp, float temp2){
    promedioTemp = (temp + temp2) / 2;
    return promedioTemp;
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