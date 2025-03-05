#define PHONE_NUMBER "+84328813393"
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <GY6050.h>

GY6050 gyro(0x68);                            // Create Gyro object with "0x68" as I2C address (most likely the address of your MPU6050)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 26, TXPin = 27;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
// Thông tin mạng Wi-Fi
const char* ssid = "Nu Nu";
const char* password = "nunu1007";

// Tạo server trên cổng 80
WebServer server(80);

// Khởi tạo đối tượng GPS
 
// Tọa độ mặc định của marker
//10.897855, 106.881086
float latitude = 10.897855;   // Hà Nội
float longitude = 106.881086; // Hà Nội
int AX, AY, AZ, GX, GY, GZ;
long times = 0;
#include <HardwareSerial.h>

#define simSerial               Serial2
#define MCU_SIM_BAUDRATE        115200
#define MCU_SIM_TX_PIN              17
#define MCU_SIM_RX_PIN              16

int sobuoc = 0;
bool tt = true, tt1= true;
//"setInterval(autoRefresh, 1000);"
// Hàm lấy HTML với tọa độ và số bước
String getHTML() {
    String html = "<!DOCTYPE html><html lang='vi'><head><meta charset='UTF-8'>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "<title>Vị trí Người Thân</title>"
                  "<link rel='stylesheet' href='https://unpkg.com/leaflet@1.7.1/dist/leaflet.css' />"
                  "<style>"
                  "body { font-family: Arial, sans-serif; margin: 0; padding: 0; background-color: #f4f4f9; color: #333; }"
                  "h2 { text-align: center; padding: 20px; color: #0077cc; }"
                  "#map { height: 500px; width: 100%; margin: 20px auto; border-radius: 10px; box-shadow: 0px 4px 8px rgba(0,0,0,0.2); }"
                  ".info-container { display: flex; justify-content: center; margin-top: 20px; }"
                  ".info-table { width: 50%; border-collapse: collapse; border: 1px solid #ddd; box-shadow: 0px 4px 8px rgba(0,0,0,0.1); }"
                  ".info-table th, .info-table td { padding: 15px; text-align: center; border: 1px solid #ddd; }"
                  ".info-table th { background-color: #0077cc; color: white; }"
                  ".info-table td { background-color: #fafafa; }"
                  "</style>"
                  // Thêm script auto-refresh
                  "<script>"
                  "function autoRefresh() {"
                  "    window.location = window.location.href;"
                  "}"
                  
                  "</script></head><body>"
                  "<h2>Vị trí Người Thân</h2>"
                  "<div id='map'></div>"
                  "<div class='info-container'>"
                  "<table class='info-table'>"
                  "<tr><th>Thông tin</th><th>Giá trị</th></tr>"
                  "<tr><td>Số bước</td><td>" + String(sobuoc) + "</td></tr>"
                  "</table>"
                  "</div>"
                  "<script src='https://unpkg.com/leaflet@1.7.1/dist/leaflet.js'></script>"
                  "<script>"
                  "var map = L.map('map').setView([" + String(latitude, 6) + ", " + String(longitude, 6) + "], 13);"
                  "L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 19, attribution: '© OpenStreetMap contributors' }).addTo(map);"
                  "L.marker([" + String(latitude, 6) + ", " + String(longitude, 6) + "]).addTo(map).bindPopup('<b>Vị trí hiện tại</b>').openPopup();"
                  "</script></body></html>";
    return html;
}



// Hàm đọc tọa độ từ GPS
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  { latitude = gps.location.lat(), 6;
  longitude = gps.location.lng(), 6;
    Serial.print(latitude);
    Serial.print(F(","));
    Serial.print(longitude);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();  }



void setup() {
    Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Đang kết nối WiFi...");
    }
    Serial.println("Đã kết nối WiFi");

    // Cấu hình server web
    server.on("/", []() {
        
        server.send(200, "text/html", getHTML());
    });

    // Bắt đầu server
    server.begin();
    Serial.println("Server đang chạy!");
    Serial.println(WiFi.localIP());
    gyro.initialisation();
    simSerial.begin(MCU_SIM_BAUDRATE, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);
    delay(1000);
    // Check AT Command
    sim_at_cmd("AT");

    // Product infor
    sim_at_cmd("ATI");

    // Check SIM Slot
    sim_at_cmd("AT+CPIN?");

    // Check Signal Quality
    sim_at_cmd("AT+CSQ");

    sim_at_cmd("AT+CIMI");


}

void loop() {
    // Duy trì server
    server.handleClient();
    
    MPU();
    tenga();
    dem();
    Serial.print("SO BUOC: ");
    Serial.println(sobuoc);
    while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
}

void MPU() {
    AX = gyro.refresh('A', 'X');
    AY = gyro.refresh('A', 'Y');
    AZ = gyro.refresh('A', 'Z');
    GX = gyro.refresh('G', 'X');
    GY = gyro.refresh('G', 'Y');
    GZ = gyro.refresh('G', 'Z');
    Serial.print("AcX = ");
    Serial.print(AX);           // Ask for the X axis of the Accelerometer and print it
    Serial.print(" | AcY = ");
    Serial.print(AY);           // Ask for the Y axis of the Accelerometer and print it
    Serial.print(" | AcZ = ");
    Serial.print(AZ);           // Ask for the Z axis of the Accelerometer and print it
    Serial.print(" | Tmp = ");
    Serial.print(gyro.refresh('T', 'C'));           // Ask for the Temperature in Celsius and print it
    Serial.print(" C | ");
    Serial.print(gyro.refresh('T', 'F'));           // Ask for the Temperature in Farenheit and print it
    Serial.print(" F");
    Serial.print(" | GyX = ");
    Serial.print(GX);           // Ask for the X axis of the Gyroscope and print it
    Serial.print(" | GyY = ");
    Serial.print(GY);           // Ask for the Y axis of the Gyroscope and print it
    Serial.print(" | GyZ = ");
    Serial.println(GZ);         // Ask for the Z axis of the Gyroscope and print it, then carriage return
    delay(25);
}

void tenga() {
    if (AX >= 60 || AX <= -60 || AY >= 60 || AY <= -60 || AZ > 60) {
      if(millis() - times >= 2000){
      Serial.println("CANH BAO TE NGA");
      if(tt1){
        nhantin();
      }
      tt1 = false;
    }
    }
    else {
      tt1 = true;
      times = millis();
    }
    }

void dem() {
    if (GX >= 60 || GX <= -60 || GY >= 60 || GY <= -60 || GZ > 60) {
        
        if (tt) {
            sobuoc = sobuoc + 1;
            delay(200);
        }
        tt = false;
    } else tt = true;
}

void nhantin() {
  Serial.println("DANG NHAN TIN");
   sim_at_cmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"";
    temp += (String)PHONE_NUMBER;
    temp += "\"";
    sim_at_cmd(temp);
    sim_at_cmd("CANH BAO! NGUOI THAN DANG TE NGA");

    // End charactor for SMS
    sim_at_send(0x1A);
}
void sim_at_wait()
{
    delay(100);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}
bool sim_at_send(char c){
    simSerial.write(c);
    return true;
}
bool sim_at_cmd(String cmd){
    simSerial.println(cmd);
    sim_at_wait();
    return true;
}
