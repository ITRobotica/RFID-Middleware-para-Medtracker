# RFID-Ubidots-Python
Código desenvolvido na Arduino IDE, o qual faz envio de dados para Ubidots (Broket MQQT de testes).Neste os dados referentes às tags RFID podem ser puxados por uma API através de um código em Python, disponibilizado neste repositório. Vale destacar que o token é muito importante para acesso ao UBIDOTS.

![image](image/itrobotica.jpeg)

## Middleware (Esp32 - nodemcu )

Utilizou-se como middleware placas com o microcontrolador ESP32, o qual possui conexão bluetooth (BLE) e Wi-Fi.
Deve-se conectar a placa (middleware) na rede via WI-FI. 
Após isso basta abrir o arquivo .ino dentro da pasta ubidots-rfid.

Para utilizar o código é necessária instalação das seguintes bibliotecas:

-UbidotsConnectMQTT (esp32-mqtt-main)
-MFC522-1.4.10
-SPI
-WIRE

-Trocar os dados de rede Wi-Fi
``const char *WIFI_SSID = "XXXXXXXX";      // Put here your Wi-Fi SSID``
``const char *WIFI_PASS = "XXXXXXXX";      // Put here your Wi-Fi password``


-Após a inicialização do código, é necessário trocar o **TOKEN** na ln16 do código 
``const char *UBIDOTS_TOKEN = "BBFF-ze3jXoYE4FNtWXJUdfbnSlQbrnldMJ";``

Após isso pode iniciar o código, se o **TOKEN** e a questões WIFI estiverem corretos, o código irá mandar diretamente para os servidores da UBIDOTS. 

## Backend (Python)

Para poder puxar os dados da Ubidots para outros projetos é necessário realizar uma conexão com a API gerada pelo usuário da conta do serviço, no caso a API do UBIDOTS pode ser encontrado no site ao lado do TOKEN.

Antes de iniciar a captura de dados, é necessário realizar a instalação do módulo do Ubidots no interpretador python. Para a instalação deste deve-se realizar o seguinte comando:

``pip install ubidots``

Após a instalação, basta "puxar" os dados no código com importação: ``from ubidots import ApiClient`` e, a seguir, realizar leitura dos dados conforme a [API](https://github.com/ubidots/ubidots-python). O código na pasta puxa o ultimo valor de um sensor criado pelo microntrolador do middleware.
