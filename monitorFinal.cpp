#include <Arduino.h>
#include <WiFi.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

PulseOximeter pox;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
WiFiClient espClient;

const char* ssid ="RamirezRodriguez_2.4Gnormal";
const char* password = "Juggerwicho1";

//SDA => 21
//SCL => 22

WiFiServer server(80);

String header;

int heartRate;
int spo2;
float temp;
float gas;
int c;

int heartRandom;
int spo2Random;
int distanceRandom;
float tempRandom;

int trigger = 2;
int echo = 4;

int duration;
int distance;

int gasValue;
int gasSensor =  34;

#define I2C_SDA 21
#define I2C_SCL 22
#define REPORTING_PERIOD_MS 1000

uint32_t tsLastReport = 0;
uint8_t max30100_address = 0x57;
uint8_t irmlx90614_address = 0x5A;

//Functions 

void wifiInit() {
    Serial.println("Connencting to");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);  
    }
    Serial.println("");
    Serial.println("WiFi connected!!!!");
    digitalWrite(2,HIGH);
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void startMAX30100() {
    pox.begin(); 
    Serial.println("Initializing pulse oximeter..");
    if (!pox.begin()) {
            Serial.println("MAX FAILED");
            // for(;;);
        } else {
            Serial.println("MAX SUCCESS");
    }
}

void webApp(){

    WiFiClient client = server.available();

    if (client) {                             
        Serial.println("New Client.");          
        String currentLine = "";                //
        while (client.connected()) {            // loop mientras el cliente está conectado
            if (client.available()) {             // si hay bytes para leer desde el cliente
                char c = client.read();             // lee un byte
                Serial.write(c);                    // imprime ese byte en el monitor serial
                header += c;
                    if (c == '\n') {                    // si el byte es un caracter de salto de linea
                    // si la nueva linea está en blanco significa que es el fin del 
                    // HTTP request del cliente, entonces respondemos:
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        
                        client.println("<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Control Monitor</title> <script src='https://cdn.tailwindcss.com'></script> <script src='https://kit.fontawesome.com/f1f52b6796.js' crossorigin='anonymous'></script></head><body class='h-screen bg-zinc-900'> <section class='grid-rows-2 text-center'> <div class='grid pb-5'> <h1 class='text-4xl bg-gray-400 py-4 uppercase font-bold text-white tracking-widest font-roboto'>Monitor Control App</h1> </div> <div class='grid grid-cols-1 lg:grid-cols-2 p-5'> <section class='grid place-content-center gap-y-10 text-center'> <a href = 'getECG' class='bg-lime-500 font-bold text-xl px-6 py-4 rounded text-white hover:bg-lime-600 hover:scale-110 transition duration-300 shadow-xl shadow-lime-600/50'> Get ECG Data </a> <a href = 'getSP02' class='bg-sky-400 font-bold text-xl px-6 py-4 rounded text-white hover:bg-sky-600 hover:scale-110 transition duration-300 shadow-xl shadow-sky-600/50'> Get SPO2 Data </a> <a href = 'getTemp' class='bg-red-500 font-bold text-xl px-6 py-4 rounded text-white hover:bg-red-700 hover:scale-110 transition duration-300 shadow-xl shadow-red-600/50'> Get Temp Data </a> <a href = 'getGas' class='bg-violet-400 font-bold text-xl px-6 py-4 rounded text-white hover:bg-violet-600 hover:scale-110 transition duration-300 shadow-xl shadow-violet-600/50'> Get Gas Data </a> <a href = 'getDistance' class='bg-yellow-400 font-bold text-xl px-6 py-4 rounded text-white hover:bg-yellow-600 hover:scale-110 transition duration-300 shadow-xl shadow-yellow-600/50'> Get Distance Data </a></section> <section class='grid md:grid-cols-2 p-5 mt-5'> <section class='grid place-content-center gap-y-10 text-center font-sans'> <section class='gird grid-cols-3 tracking-widest'> <h2 class='font-bold text-lg text-white uppercase'>ECG Data</h2>");

                        if(header.indexOf("GET /getECG") >= 0){
                            client.print(" <p class='text-3xl text-lime-500 font-bold' ><span class=' text-8xl ' id='ecg'>"+String(heartRandom)+"</span>pbm</p>");
                        }else{
                            client.print(" <p class='text-3xl text-lime-500 font-bold' ><span class=' text-8xl ' id='ecg'>0</span>pbm</p>");
                        }
                        client.print("<section class='grid gird-rows-2 blink hidden ' id='alarmECG'> <i class='grid fa-solid fa-triangle-exclamation text-yellow-300 text-4xl'></i> <p class='text-yellow-300 text-xs font-bold uppercase' id='descECG'>cucu</p> </section> </section> <section class='gird grid-cols-3 tracking-widest'> <h2 class='font-bold text-lg text-white uppercase'>SPO2 Data</h2>");
                        
                        if(header.indexOf("GET /getSP02") >= 0){
                            client.print("<p class='text-3xl text-sky-400 font-bold' ><span class=' text-8xl ' id='spo2'>"+String(spo2Random)+"</span>%</p>");
                        }else{
                            client.print("<p class='text-3xl text-sky-400 font-bold' ><span class=' text-8xl ' id='spo2'>0</span>%</p>");
                        }
                        client.print("<section class='grid gird-rows-2 blink hidden' id='alarmSPO2'> <i class='grid fa-solid fa-triangle-exclamation text-yellow-300 text-4xl'></i> <p class='text-yellow-300 text-xs font-bold uppercase' id='descSPO2'>cucu</p> </section> </section> <section class='gird grid-cols-3 tracking-widest'> <h2 class='font-bold text-lg text-white uppercase'>Temp Data</h2>");

                        if(header.indexOf("GET /getTemp") >= 0){
                            client.print("<p class='text-3xl text-red-500 font-bold' ><span class=' text-8xl ' id='temp'>"+String(tempRandom)+"</span>°C</p>");

                        }else{
                            client.print("<p class='text-3xl text-red-500 font-bold' ><span class=' text-8xl ' id='temp'>0</span>°C</p>");
                        }
                        client.print(" <section class='grid gird-rows-2 blink hidden' id='alarmTemp'> <i class='grid fa-solid fa-triangle-exclamation text-yellow-300 text-4xl'></i> <p class='text-yellow-300 text-xs font-bold uppercase' id='descTemp'>cucu</p> </section> </section> </section> <section class='grid place-content-center gap-y-10 text-center font-sans'> <section class='gird grid-cols-3 tracking-widest'> <h2 class='font-bold text-lg text-white uppercase'>Gas Data</h2>");

                        if(header.indexOf("GET /getGas") >= 0){
                            client.print("<p class='text-3xl text-violet-500 font-bold' ><span class=' text-8xl ' id='gas'>"+String(gasValue)+"</span>units</p>");

                        }else{
                            client.print("<p class='text-3xl text-violet-500 font-bold' ><span class=' text-8xl ' id='gas'>0</span>units</p>");
                        }
                        client.print("<section class='grid gird-rows-2 blink hidden' id='alarmGas'> <i class='grid fa-solid fa-triangle-exclamation text-yellow-300 text-4xl'></i> <p class='text-yellow-300 text-xs font-bold uppercase' id='descGas'>cucu</p> </section> </section> <section class='gird grid-cols-3 tracking-widest'> <h2 class='font-bold text-lg text-white uppercase'>Distance Data</h2>");

                        if(header.indexOf("GET /getDistance") >= 0){
                            client.print("<p class='text-3xl text-yellow-500 font-bold' ><span class=' text-8xl ' id='distance'>"+String(distance)+"</span>cm</p>");
                        }else {
                            client.println("<p class='text-3xl text-yellow-500 font-bold' ><span class=' text-8xl ' id='distance'>0</span>cm</p>");
                        }
                        client.println("<section class='grid gird-rows-2 blink hidden' id='alarmDistance'> <i class='grid fa-solid fa-triangle-exclamation text-yellow-300 text-4xl'></i> <p class='text-yellow-300 text-xs font-bold uppercase' id='descDistance'></p> </section> </section> </section> </section> </div> </section> <style> .blink { animation: blink 1s infinite; } @keyframes blink { 0% { opacity: 0; } 50% { opacity: 1; } 100% { opacity: 0; } } </style> <audio id='audio'> <source src='https://docs.google.com/uc?export=download&id=1aOJ5_D0znz-lOnAhUh9nRjNHs_YT3Vp5' type='audio/ogg' ></audio>");

                        client.print("<script>let ecgHTML = document.querySelector('#ecg').textContent;let spo2HTML = document.querySelector('#spo2').textContent;let tempHTML = document.querySelector('#temp').textContent;let distanceHTML = document.querySelector('#distance').textContent;let gasHTML = document.querySelector('#gas').textContent;const ecgAlarm = document.querySelector('#alarmECG');const spo2Alarm = document.querySelector('#alarmSPO2');const tempAlarm = document.querySelector('#alarmTemp');const distanceAlarm = document.querySelector('#alarmDistance');const gasAlarm = document.querySelector('#alarmGas');const ecgDescription = document.querySelector('#descECG');const spo2Description = document.querySelector('#descSPO2');const tempDescription = document.querySelector('#descTemp');const distanceDescription = document.querySelector('#descDistance');const gasDescription = document.querySelector('#descGas');const audio = document.querySelector('#audio');ecgHTML = Number(ecgHTML);spo2HTML = Number(spo2HTML);tempHTML = Number(tempHTML);distanceHTML = Number(distanceHTML);gasHTML = Number(gasHTML);if(ecgHTML > 120 || ecgHTML < 50 || spo2HTML < 90 || tempHTML > 37 || tempHTML < 35 || distanceHTML > 10 || gasHTML > 300){ audio.loop = true;audio.play(); if(ecgHTML > 120 && ecgHTML !== 0){ ecgAlarm.classList.remove('hidden'); ecgDescription.textContent = 'Tachycardia'; } else if (ecgHTML < 50 && ecgHTML !== 0){ ecgAlarm.classList.remove('hidden'); ecgDescription.textContent = 'Bradycardia'; } if(spo2HTML < 90 && spo2HTML !== 0){ spo2Alarm.classList.remove('hidden'); spo2Description.textContent = 'Hypoxia'; } if (tempHTML > 37 && tempHTML !== 0){ tempAlarm.classList.remove('hidden'); tempDescription.textContent = 'Fever'; } else if(tempHTML < 35 && tempHTML !== 0){ tempAlarm.classList.remove('hidden'); tempDescription.textContent = 'Hypothermia'; } if(distanceHTML > 15 && distanceHTML !== 0){ distanceAlarm.classList.remove('hidden'); distanceDescription.textContent = 'Too far'; } if(gasHTML > 300 && gasHTML !== 0){ gasAlarm.classList.remove('hidden'); gasDescription.textContent = 'Too much gas'; }}else{ ecgAlarm.classList.add('hidden'); spo2Alarm.classList.add('hidden'); tempAlarm.classList.add('hidden'); distanceAlarm.classList.add('hidden'); gasAlarm.classList.add('hidden'); audio.pause();}</script></body></html>");

                        client.println();
                        break;
                    } else { 
                        currentLine = "";
                    }
                } else if (c != '\r') { 
                    currentLine += c;      
                }
            }
        }
        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
} 

void readData(){
    pox.update();
    temp = mlx.readObjectTempC();
    heartRate = pox.getHeartRate();
    spo2 = pox.getSpO2();
    // gasValue = analogRead(gasSensor);
    // digitalWrite(trigger, HIGH);
    // delay(1);  
    // digitalWrite(trigger, LOW);  
    // duration = pulseIn(echo, HIGH);
    // distance = duration / 58.2;
    heartRandom = random(60, 100);
    spo2Random = random(95, 100);
    tempRandom = random(35, 37);
    // distanceRandom = random(0, 10);

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        // Serial.println("////////////////DATA//////////////////////");
        Serial.println("Heart rate:" + String(heartRate));
        Serial.println("SpO2:" + String(spo2) + "%");
        Serial.println("Temperature:" + String(temp));
        Serial.println("Distance:" + String(distance) + "cm");
        Serial.println("Gas:" + String(gasValue));
        tsLastReport = millis();
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(2,OUTPUT);
    wifiInit();
    mlx.begin();
    Wire.begin();
    server.begin();
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
    delay(2000);
    startMAX30100();
}

void loop() {
    readData();
    webApp();
}
