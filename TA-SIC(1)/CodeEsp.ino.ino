#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "Dapur Hani";
const char* password = "nas10nal120199";

const char* serverName = "http://192.168.18.32:5000/post-data";

#define DHTTYPE DHT11
#define DHTPIN 4

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menyambungkan...");
  }
  Serial.println("Tersambung");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca sensor");
  } else {
    Serial.println("Temperature: " + String(temperature));
    Serial.println("Humidity: " + String(humidity));

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      http.begin(serverName);
      
      http.addHeader("Content-Type", "application/json");
      
      String httpRequestData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      Serial.print("Mengirim POST request: ");
      Serial.println(httpRequestData);
    
      int httpResponseCode = http.POST(httpRequestData);
            if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Eror mengirim POST: ");
        Serial.println(httpResponseCode);
      }
      
      http.end();
    } else {
      Serial.println("Error menyambung WiFi");   
    }
  }
  
  delay(10000);
}
