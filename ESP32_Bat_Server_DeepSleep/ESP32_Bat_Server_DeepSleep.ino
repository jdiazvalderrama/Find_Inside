#include <WiFi.h>
#include <HTTPClient.h>

#define TIME_TO_SLEEP 60e6

const char* ssid = "RedWifi";
const char* password = "pass";
int contador = 0;


void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  WiFi.begin(ssid, password); // Conecta a la red Wi-Fi
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    contador += 1;
    Serial.println("Conectando a la red WiFi...");
    encenderLed(1);
    if(contador>=10){
      contador = 0;
      esp_deep_sleep_start();
    }
  }
  Serial.println("Conectado a la red WiFi");
  encenderLed(3);
  Serial.println("PRUEBA DEEP SLEEP");

  

}

void encenderLed(int cantidad){
  for(int i=0; i<=cantidad; i++){
    digitalWrite(5, 1);
    delay(100);
    digitalWrite(5, 0);
    delay(100);
  }
}

void realizarGET() {
  String miMac = WiFi.macAddress();
  int valorADC = analogRead(35);
  float voltajeADC = (valorADC / 4095.0) * 3.3;
  float voltajeBateria = voltajeADC * 2;

  if(WiFi.status() == WL_CONNECTED){
    // Escanear redes disponibles
    int n = WiFi.scanNetworks();
    String wifiData = String(voltajeBateria)+"_" + miMac + "_";

    for (int i = 0; i < n; ++i) {
      // Separa los datos de cada red con una coma y cada red con un pipe
      wifiData += WiFi.SSID(i) + "@" + WiFi.BSSIDstr(i) + "@" + String(WiFi.RSSI(i));
      if (i < n - 1) {
        wifiData += "_"; // No agrega '|' después del último elemento
      }
    }

    // Codifica la cadena wifiData para que pueda ser enviada en una URL
    wifiData = urlencode(wifiData);
    
    // Construye la URL completa
    String serverPath = "https://servidos.com/get.php?data=" + wifiData;

    // Crea un objeto HTTPClient
    HTTPClient http;
    
    // Inicia la conexión
    http.begin(serverPath);
    
    // Realiza la solicitud GET
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.println(httpResponseCode);
      String response = http.getString();
      encenderLed(2);
      Serial.println(response);
    } else {
      Serial.print("ERROR");
      encenderLed(10);
      Serial.println(httpResponseCode);
    }
    
    // Cierra la conexión
    http.end();
  } else {
    Serial.println("Error en la conexión WiFi");
    encenderLed(5);
  }
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (unsigned int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield(); // Para evitar el watchdog timer en bucles largos
  }
  return encodedString;
}

void loop() {
  realizarGET(); // Realiza la solicitud GET
  Serial.println("A Dormir Zzzzz...");
  //delay(10000);
  esp_deep_sleep_start();
}
