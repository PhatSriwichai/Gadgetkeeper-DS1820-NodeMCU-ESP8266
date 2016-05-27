#include <ESP8266WiFi.h>
#include <string.h>
#define SERVER_PORT 80 //กำหนดใช้ Port 80
const char* ssid = "My ASUS0079"; //กำหนด SSID
const char* password = "ao0897224117"; //กำหนด Password
const char* server_char = "api.gadgetkeeper.com";

String first_string = "GET /v1/things/"; //กำหนดชื่อ Server ที่ต้องการเชื่อมต่อ
String thing_key = "cd007761233411e69b064b58a7906d4e";/* กำหนดค่าคำสั่ง HTTP GET */
String prop = "/properties/";
String prop_key = "38b20be6233511e69b064b58a7906d4e";
String api = "/value.json?ApiKey=";
String api_key = "409e2a611c4f440d9c4b7627375c5226";
String final_string = " HTTP/1.1\r\nHost: api.gadgetkeeper.org\r\n\r\n";

WiFiServer server(SERVER_PORT); // เปิดใช้งาน TCP Port 80
WiFiClient client; // ประกาศใช้ client
unsigned long previousMillis = 0; // กำหนดตัวแปรเก็บค่า เวลาสุดท้ายที่ทำงาน
const long interval = 10000; // กำหนดค่าตัวแปร ให้ทำงานทุกๆ 10 วินาที
void Client_Request();
int i = 0;

void setup()
{ Serial.begin(115200);
  WiFi.begin(ssid, password); // เชื่อมต่อกับ AP
  while (WiFi.status() != WL_CONNECTED) // ตรวจเช็ค และ รอจนเชื่อมต่อ AP สำเร็จ
  { delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // แสดง IP Address ที่ได้
}
void loop()
{ while (client.available()) // ตรวจเช็คว่ามีการส่งค่ากลับมาจาก Server หรือไม่
  { String line = client.readStringUntil('\n'); //อ่านค่าที่ Server ตอบหลับมาทีละบรรทัด
    String get_data;
    //
   if(line.indexOf('.') == 2){
        get_data = line;
        Serial.println("Temp now = "+get_data+" Celsius.");
   }
  }
  unsigned long currentMillis = millis(); //อ่านค่าเวลาที่ ESP เริ่มทำงานจนถึงเวลาปัจจุบัน
  if (currentMillis - previousMillis >= interval) /*ถ้าหากเวลาปัจจุบันลบกับเวลาก่อหน้านี้ มีค่ามากกว่าค่า interval ให้คำสั่งภายใน if ทำงาน*/
  { previousMillis = currentMillis; /*ให้เวลาปัจจุบัน เท่ากับ เวลาก่อนหน้าเพื่อใช้คำนวณเงื่อนไขในรอบถัดไป*/
    Client_Request(); //เรียกใช้งานฟังก์ชั่น Client_Request
  }
}
//==================================================
void Client_Request()
{ Serial.println("Connect TCP Server");
  int cnt = 0;
  while (!client.connect(server_char, SERVER_PORT)) //เชื่อมต่อกับ Server และรอจนกว่าเชื่อมต่อสำเร็จ
  { Serial.print(".");
    delay(100);
    cnt++;
    if (cnt > 50) //ถ้าหากใช้เวลาเชื่อมต่อเกิน 5 วินาที ให้ออกจากฟังก์ชั่น
      return;
  }
  Serial.println("Success");
  delay(500);
  client.print(first_string + thing_key + prop + prop_key + api + api_key + final_string); //ส่งคำสั่ง HTTP GET ไปยัง Server
  delay(100);
}
//==================================================
String cut_string(String input, String header, String get_string) {
  if (input.indexOf(header) != -1) //ตรวจสอบว่าใน input มีข้อความเหมือนใน header หรือไม่
  { int num_get = input.indexOf(header); //หาตำแหน่งของข้อความ header ใน input
    if (num_get != -1) //ตรวจสอบว่าตำแหน่งที่ได้ไม่ใช่ -1 (ไม่มีข้อความ get_string ใน input)
    { num_get = input.indexOf(get_string, num_get); //หาตำแหน่งของข้อความ get_string ใน input เริ่มจากตำแหน่ง header
      int start_val = input.indexOf("\"", num_get) + 1; // หาตำแหน่งแรกของ “ เริ่มจากตำแหน่ง header
      int stop_val = input.indexOf("\"", start_val); // หาตำแหน่งสุดท้ายของ “ เริ่มจากตำแหน่ง start
      return (input.substring(start_val, stop_val)); // ตัดเอาข้อความระหว่า “แรก และ ”สุดท้าย
    }
    else
    { return ("NULL"); //Return ข้อความ NULL เมื่อไม่ตรงเงื่อนไข
    }
  }
  return ("NULL"); //Return ข้อความ NULL เมื่อไม่ตรงเงื่อนไข
}

