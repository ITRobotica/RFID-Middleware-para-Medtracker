//Baseado no exemplo: SerialToSerialBTM

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


String name = "HQ_UHF_READER";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

//Funções auxiliares

int pfx_TimedRead()
{
    int c;
    unsigned long _startMillis = millis();
    unsigned long _timeout = 3000;
    do {
        c = SerialBT.read();
        if(c >= 0)
            return c;
        if(_timeout == 0)
            return -1;
        yield();
    } while(millis() - _startMillis < _timeout);
    return -1;     // -1 indicates timeout
}

size_t pfx_ReadFromBluetooth(uint8_t *buffer, size_t length)
{
    size_t count = 0;
    while(count < length)
    {
      int c = pfx_TimedRead();
      if(c < 0)
      {
        break;
      }
      *buffer++ = (uint8_t) c;
      count++;
    }
    return count;
}

void pfx_SendBluetoothComandInBytes(std::vector<uint8_t> command)
{
  //Serial.println("Enviando comando");
  for(uint8_t i = 0 ; i < command.size(); i++)
  {
    //Serial.println(command[i]);
    SerialBT.write(command[i]);
  }
}

void pfx_SendBluetoothComandInString(String command)
{
  //Serial.println("Enviando comando");
  for(uint8_t i = 0 ; i < command.length(); i++)
  {
    //Serial.println(command[i]);
    SerialBT.write(command[i]);
  }
  SerialBT.write(0x0d);
  SerialBT.write(0x0a);
}
void pfx_ReceiveTag(uint8_t length, uint32_t timeout)
{
  std::string received_data = "";
  uint32_t rx_timeout = millis();
  while(millis() - rx_timeout < timeout)
  {
    if(SerialBT.available())
    {
      if(SerialBT.available() > 52)
      {
        Serial.println("Falha ao receber TAG");
        while(SerialBT.available())
        {
          SerialBT.read();
        }
        return;
      }
      while(SerialBT.available())
      {
        received_data += (char) SerialBT.read();
      }
      size_t position = received_data.find("e=");
      if(position == std::string::npos)
      {
        Serial.println("Falha ao receber TAG");
        return;
      }
      std::string filtered_received_data = received_data.substr(position + 2, 12);
      Serial.printf("Tag recebida: %s\n", filtered_received_data.c_str());
      return;
    }
  }
  Serial.println("Tag não recebida");
}


void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32test", true); 
  Serial.println("Tentando conexão com DOTR-900. Certifique-se que o aparelho está ligado...");
  
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect(name);
  
  if(connected)
  {
    Serial.println("Conectado com sucesso!");
  }
  else
  {
    while(!SerialBT.connected(10000))
    {
      Serial.println("Falha ao conectar. Verifique se o DOTR-900 está ligado. Reiniciando para tentar novamente...");
      sleep(1);
      ESP.restart();
    }
  }
  
  sleep(2);
  
  while(true)
  {
    Serial.println("Tentando abrir conexão...");
    std::vector<uint8_t> open_interface_command = {0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a};
    pfx_SendBluetoothComandInBytes(open_interface_command);

    uint8_t buffer_read[4];
    size_t count = pfx_ReadFromBluetooth(buffer_read, 4);
    uint8_t connection_response_1[4] = {13, 10, 36, 62}; //<0d><0a>$>
    uint8_t connection_response_2[3] = {10, 36, 62}; //<0d><0a>$>
    uint8_t connection_response_3[2] = {36, 62}; //$>
      
    if(memcmp(buffer_read, connection_response_1, 4) == 0 || memcmp(buffer_read, connection_response_2, 3) == 0 || memcmp(buffer_read, connection_response_3, 2) == 0)
    {
      Serial.println("Abertura de conexão realizada!");
      break;
    }
    sleep(1);
  }

  sleep(2);
}

void loop()
{
  String read_tag_command = "r,1,1,0,0,1,0,0";
  pfx_SendBluetoothComandInString(read_tag_command);
  pfx_ReceiveTag(45, 3000);
  delay(1500);
}
