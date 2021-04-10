#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Definição dos pinos utilizados
#define LED_PIN 7
#define BT_TX 10
#define BT_RX 11
#define GPS_TX 8
#define GPS_RX 9

// Criar um software serial bluetooth e GPS
SoftwareSerial bluetoothSerial(BT_TX, BT_RX);
// SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
// TinyGPS gps;

bool state = false;
char incomingByte; // Variável para armezenar o bytes recebidos
char delimiter = ';';
String CMD = "";

void setup(){
    // Inicializar o serial bluetooth e GPS
    // gpsSerial.begin(9600);
    bluetoothSerial.begin(9600);
    Serial.begin(9600);
    // Inicializar o pino do LED
    pinMode(LED_PIN, OUTPUT);
}

void loop(){
    bool gpsReceived = false;

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

    // while(gpsSerial.available()){
    //     char cIn = gpsSerial.read();
    //     gpsReceived = gps.encode(cIn);
    // }
    // if(gpsReceived){
    //     Serial.println("----------------------------------------------------------");

    //     long latitude, longitude;
    //     unsigned long delayInfo;

    //     gps.get_position(&latitude, &longitude, &delayInfo);
    //     Serial.print(float(latitude) / 100000, 6);

    //     if(latitude != TinyGPS::GPS_INVALID_F_ANGLE){
    //         Serial.print("Latitude: ");
    //         Serial.println(float(latitude) / 100000, 6);
    //     }
    //     if(longitude != TinyGPS::GPS_INVALID_F_ANGLE){
    //         Serial.print("Longitude: ");
    //         Serial.println(float(longitude) / 100000, 6);
    //     }
    //     if(delayInfo != TinyGPS::GPS_INVALID_AGE){
    //         Serial.print("Delay da informação: ");
    //         Serial.println(delayInfo);
    //     }
        
    // }

    
}

void readCMD(String input){
    Serial.println(input);
    if(input == "device=on"){
        state = true;
        
        bluetoothSerial.write("{\"result\":true,\"type\":\"success\",\"cmd\":\"device=on\",\"message\":\"O Arduino foi ligado!\"};");

    } else if(input == "device=off"){
        state = false;
        
        bluetoothSerial.write("{\"result\":false,\"type\":\"success\",\"cmd\":\"device=off\",\"message\":\"O Arduino foi desligado!\"};");
    } else {
        Serial.println(input);

        bluetoothSerial.write("{\"result\":false,\"type\":\"error\",\"code\":\"unknow\",\"message\":\"Comando desconhecido!\"};");
    }
}