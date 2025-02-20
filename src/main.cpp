#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Cấu hình cảm biến DHT11
#define DHTPIN 4 // Chọn chân GPIO của ESP32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Cấu hình WiFi
const char* ssid = "LLL";       
const char* password = "09090909"; 

// Cấu hình Thinkspeak
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "HWM38MBKE651PBZN"; 

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Kết nối WiFi
    WiFi.begin(ssid, password);
    Serial.print("Đang kết nối WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nĐã kết nối WiFi!");
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Lỗi đọc cảm biến!");
        return;
    }

    Serial.print("Nhiệt độ: "); Serial.print(temperature);
    Serial.print("°C - Độ ẩm: "); Serial.print(humidity);
    Serial.println("%");

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(server) + "?api_key=" + apiKey +
                     "&field1=" + String(temperature) +
                     "&field2=" + String(humidity);

        http.begin(url);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.println("Gửi dữ liệu thành công!");
        } else {
            Serial.println("Gửi thất bại!");
        }
        http.end();
    }

    delay(1000); 
}
