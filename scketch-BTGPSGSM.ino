#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Definição dos pinos utilizados
#define LED_PIN 7
#define BT_TX 10
#define BT_RX 11

// Criar um software serial bluetooth e GPS
SoftwareSerial bluetoothSerial(BT_TX, BT_RX);

bool state = false;
char incomingByte; // Variável para armezenar o bytes recebidos
char delimiter = ';';
String CMD = "";

void setup(){
    // Inicializar o serial bluetooth e GPS
    bluetoothSerial.begin(9600);
    Serial.begin(38400);

    // Inicializar o pino do LED
    pinMode(LED_PIN, OUTPUT);
}

void loop(){
    // Verifica se está chegando dados pelo serial
    if(bluetoothSerial.available() > 0){
        // Armazena o byte na variárel
        incomingByte = bluetoothSerial.read();

        if(incomingByte != delimiter){
            // Concatena o caractere na string CMD
            CMD += incomingByte;
        } else {
            // Avalia o comando
            readCMD(CMD);

            // Zera a string do comando
            CMD = "";
        }
    }

    if(state){
        digitalWrite(LED_PIN, HIGH);
        
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

void readCMD(String input){
    Serial.println(input);
    if(input == "device=on"){
        state = true;
        btWrite("{\"result\":true,\"type\":\"success\",\"cmd\":\"device=on\",\"message\":\"O Arduino foi ligado!\"}");
    } else if(input == "device=off"){
        state = false;
        btWrite("{\"result\":false,\"type\":\"success\",\"cmd\":\"device=off\",\"message\":\"O Arduino foi desligado!\"}");
    } else {
        Serial.println(input);
        btWrite("{\"result\":false,\"type\":\"error\",\"code\":\"unknow\",\"message\":\"Comando desconhecido!\"}");
    }
}

void btWrite(String data){
    for(int i = 0; i < data.length(); i++){
        bluetoothSerial.write(data[i]);
    }
}
