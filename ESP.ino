#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "esp_task_wdt.h"


TaskHandle_t Task1; 
TaskHandle_t Task3;

#define DHTPIN 19     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 22
#define ledPin 12
#define pump 2
#define SensorPin 34
#define Btn1 25
#define Btn2 26

const int AirValue = 4095;   //Giá trị trung bình của cảm biến đo được khi ở không khí bình thường
const int WaterValue = 2700;  //Giá trị trung bình của cảm biến đo được khi ở dưới nước
int soilMoistureValue = 0;
int soilmoisturepercent=0;

DHT dht(DHTPIN, DHTTYPE);
// Khởi tạo LCD với địa chỉ I2C 0x27, 16 cột và 2 hàng
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define WIFI_SSID "Tien Phuoc"
#define WIFI_PASSWORD "1haiba456789"

#define DATABASE_URL "https://dht22-b345f-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyAQQell80mDAsZvhIiyGu6EWoA-PTBgvB4"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void updateFirebaseLEDStatus(int status) {
  if (Firebase.RTDB.setInt(&fbdo, "/ESP32_count/ledStatus", status)) {
    Serial.println("LED status updated on Firebase: " + String(status));
  } else {
    Serial.println("Failed to update LED status on Firebase");
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(Btn1, INPUT_PULLUP);
  pinMode(Btn2, INPUT_PULLUP);
  Serial.begin(115200);
  lcd.init(); // Khởi động LCD
  lcd.backlight(); // Bật đèn nền
  dht.begin();

  esp_task_wdt_init(portMAX_DELAY, true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

 if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign up successful");
  } else {
    Serial.println("Firebase sign up failed");
    Serial.println(config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,1,&Task1,0);   
  xTaskCreatePinnedToCore(Task3code,"Task3",10000,NULL,1,&Task3,1);  
  esp_task_wdt_add(Task1);
  esp_task_wdt_add(Task3);
}

void Task1code( void * parameter ){
  for(;;){
     // Đọc nhiệt độ và độ ẩm từ DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
    lcd.clear(); // Xóa màn hình trước khi hiển thị dữ liệu mới
    lcd.setCursor(0, 0); // Di chuyển con trỏ đến hàng đầu tiên, cột đầu tiên
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1); // Di chuyển con trỏ đến hàng thứ hai, cột đầu tiên
    lcd.print("Humi: ");
    lcd.print(humidity);
    lcd.print("%");
    // Cập nhật dữ liệu lên Firebase
    Firebase.RTDB.setFloat(&fbdo, "/Temperature", temperature);
    Firebase.RTDB.setFloat(&fbdo, "/Air humidity", humidity);

    soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    if(soilmoisturepercent > 100)
    {
      Firebase.RTDB.setFloat(&fbdo, "/Soil humidity", 100);
      delay(5000);
    }
    else if(soilmoisturepercent <0)
    {
     Firebase.RTDB.setFloat(&fbdo, "/Soil humidity", 0);
      delay(5000);
    }
    else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
    {
      Firebase.RTDB.setFloat(&fbdo, "/Soil humidity", soilmoisturepercent);
      delay(5000);
    }
  }
}

void Task3code(void * parameter){
  // Biến lưu trữ trạng thái của LED
  bool ledState = LOW; 
  // Ban đầu, tắt LED
  digitalWrite(ledPin, ledState);
  digitalWrite(pump, ledState);
  while(true) { 

    // Đọc trạng thái của nút 1 và nút 2
    int buttonState1 = digitalRead(Btn1);

    // Kiểm tra nút 2
    if (buttonState1 == LOW) { 
      // Bật LED
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      digitalWrite(pump, HIGH);
    }
  
    if (buttonState1 == HIGH) { 
      // Tắt LED
      ledState = LOW;
      digitalWrite(ledPin, ledState);
      digitalWrite(pump, LOW);

    }
  }
}

void loop() {

}
