//Programa : RFID - Ubidots
//Autor : Nikolas Anjos 
 
#include <SPI.h>
#include <MFRC522.h>
#include "UbidotsEsp32Mqtt.h"
 
// #define SS_PIN 10
#define RST_PIN 23
#define SCK 22
#define SDA 21
#define MISO 14
#define MOSI 13


const char *UBIDOTS_TOKEN = "BBFF-ze3jXoYE4FNtWXJUdfbnSlQbrnldMJ";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "XXXXXXX";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "XXXXXXX";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "RFID_Ubidots";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "RFID-Sensor"; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
uint8_t analogPin = 34; // Pin used to read data from GPIO34 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);


MFRC522 mfrc522(SDA, RST_PIN);   // Create MFRC522 instance.

char st[20];

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}



void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin(SCK,MISO,MOSI,SDA);      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL);

  timer = millis();
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println(); 
  mensageminicial();
}
 
void loop() 
{
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL);
  }
  ubidots.loop();

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "29 75 1F B4") //UID 1 - Chaveiro
  {
    Serial.println("CARTÃO BRANCO !");
    Serial.println();
    ubidots.add(VARIABLE_LABEL, 1);
    ubidots.publish(DEVICE_LABEL);
    delay(3000);
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "4C BF B8 21") //UID 2 - Cartao
  {
    Serial.println("CHAVEIRO !");
    Serial.println();
    ubidots.add(VARIABLE_LABEL, 2);
    ubidots.publish(DEVICE_LABEL);
    delay(3000);
    mensageminicial();
  }

  if (conteudo.substring(1) == "62 BA 51 13") //UID 3 - bUS
  {
    Serial.println("ONIBUS !");
    Serial.println();
    ubidots.add(VARIABLE_LABEL, 3);
    ubidots.publish(DEVICE_LABEL);
    delay(3000);
    mensageminicial();
  }
  
} 
 
void mensageminicial()
{
  
  Serial.print(" Aproxime o seu");  
  Serial.println("cartao do leitor");  
}
