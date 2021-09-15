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
TinyGPS gps;

bool state = false;
bool isBTFree = true;
char incomingByte; // Variável para armezenar o bytes recebidos
char delimiter = ';';

bool trackLocation = false;
float latitude = -25.418267;
float longitude = -49.264517;

void setup(){
    // Inicializar o serial bluetooth e GPS
    // gpsSerial.begin(9600);
    bluetoothSerial.begin(9600);
    Serial.begin(38400);
    // Inicializar o pino do LED
    pinMode(LED_PIN, OUTPUT);
}

void loop(){
    readBT();

    if(state){
        digitalWrite(LED_PIN, HIGH);
        sendLocation();

    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

void readCMD(String input){
    bluetoothSerial.read();
    
    if(input == "device=on"){
        state = true;
        writeBT("{\"result\":true,\"type\":\"success\",\"cmd\":\"device=on\",\"message\":\"O Arduino foi ligado!\"};");
    } else if(input == "device=off"){
        state = false;
        writeBT("{\"result\":false,\"type\":\"success\",\"cmd\":\"device=off\",\"message\":\"O Arduino foi desligado!\"};");
    } else if(input == "device=stopTrackLocation"){
        trackLocation = false;
        writeBT("{\"result\":false,\"type\":\"success\",\"cmd\":\"device=stopTrackLocation\",\"message\":\"O dispositivo está com o rastreamento desligado!\"};");
    } else if(input == "device=trackLocation"){
        trackLocation = true;
        writeBT("{\"result\":true,\"type\":\"success\",\"cmd\":\"device=trackLocation\",\"message\":\"O dispositivo está sendo rastreado!\"};");
    }  else {
        Serial.println(input);
        writeBT("{\"result\":false,\"type\":\"error\",\"cmd\":\""+input+"\",\"code\":\"unknow\",\"message\":\"Comando desconhecido!\"};");
    }
}

void sendLocation(){
    if(trackLocation){
        // Serial.println("Tracker ligado");
        String stringLat = "";

        latitude += 0.00100;
        longitude += 0.00100;
        stringLat = "{\"result\":{";
        stringLat += "\"latitude\":\"";
        stringLat += String(latitude, 6);
        stringLat += "\",\"longitude\":\"";
        stringLat += String(longitude, 6);
        stringLat += "\"";
        stringLat += "},\"type\":\"success\",\"cmd\":\"device=trackLocation\",\"message\":\"Novas coordenadas recebidas!\"};";
        Serial.println(stringLat);
        delay(600);
        writeBT(stringLat);
    } 
}

void readBT(){
    while(!isBTFree){}
    isBTFree = false;
    String CMD = "";
    
    while(!isBTFree){
        // Verifica se está chegando dados pelo serial
        bluetoothSerial.listen();
        if(bluetoothSerial.available() > 0){
            if(int(incomingByte) > -1) {
                // Armazena o byte na variárel
                incomingByte = bluetoothSerial.read();
                if(incomingByte != delimiter){
                    // Concatena o caractere na string CMD
                    CMD += incomingByte;
                } else {
                    Serial.println("\nComando recebido: "+CMD);

                    isBTFree = true;
                    readCMD(CMD);
                }
            } 
        } else if(CMD.length() < 1){
            isBTFree = true;
        }
    }
}
void writeBT(String data){
    while(!isBTFree){}
    isBTFree = false;
    while(!isBTFree){
        for(int i = 0; i < data.length(); i++){
            bluetoothSerial.write(data[i]);
        }
        Serial.println("Resposta enviada: "+data);
        isBTFree = true;
    }
}