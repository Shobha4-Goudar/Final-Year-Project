#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>

#define DHTPIN D5
#define DHTTYPE DHT11
#define GAS_SENSOR_PIN A0
#define LED_PIN D0
#define BUZZER_PIN D6

DHT dht(2, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address if necessar

const char* ssid = "Galaxy A21s";
const char* password = "Shobha@4";
unsigned long myChannelNumber = 2899746; // Replace with your ThingSpeak channel number
const char* myWriteAPIKey = "UUTKDUYA3S34M2MM"; // Replace with your ThingSpeak write API key 
WiFiClient client;
void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
 lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
   lcd.print("PROJECT SUBMITED");
   lcd.setCursor(0, 1);
   lcd.print("       BY       ");
   delay(3000);
   lcd.clear();

   lcd.setCursor(0, 0);
   lcd.print("PREETI HOMBALMTH");
   lcd.setCursor(0, 1);
   lcd.print(" SHOBHA  GOUDAR ");
   delay(3000);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(" TULASI VALLURU ");
     delay(3000);
   lcd.clear();
lcd.setCursor(0, 0);
   lcd.print(" UNDER GUIDANCE ");
   lcd.setCursor(0, 1);
   lcd.print(" Prof: MAHANT.K ");
   delay(3000);
   lcd.clear();
  lcd.setCursor(0, 0);
    lcd.print(" FOOD  SPOILAGE ");
    lcd.setCursor(0, 1);
    lcd.print("    DETECTING    ");
  pinMode(16, OUTPUT);
  pinMode(12, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int gasValue = analogRead(GAS_SENSOR_PIN);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(7, 3);
  lcd.print(" Gas:");
  lcd.print(gasValue);
  lcd.print("%");
  

  // Send data to ThingSpeak
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, gasValue);
  
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (responseCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(responseCode);
  }

  // Detect spoilage
  int threshold = 230; // Set your threshold value based on your gas sensor calibration
  if (gasValue > threshold) {
    digitalWrite(16, HIGH);
    digitalWrite(12, HIGH);
    Serial.println("Food spoilage detected!");
    lcd.setCursor(0, 0);
    lcd.print(" FOOD  SPOILAGE ");
    lcd.setCursor(0, 1);
    lcd.print("    DETECTED    ");
  } else {
    digitalWrite(16, LOW);
    digitalWrite(12, LOW);
  }

  delay(2000); // Delay between readings
}
