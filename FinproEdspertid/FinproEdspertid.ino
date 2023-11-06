#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <RTClib.h>

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

RTC_DS3231 rtc;

String days[] = {"Senin", "selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"};

const char *ssid = "";
const char *pass = "";

const int IN1 = D6;
const int IN2 = D7;
const int IN3 = D8;


struct TimeRtc {
  String row1;
  String row2;
  int jam, min, sec;
};


TimeRtc getTimeRtc(DateTime now) {
  TimeRtc time;
  int jam = now.hour();
  int min = now.minute();
  int sec = now.second();
  time.jam = jam;
  time.min = min;
  time.sec = sec;
  return time;
};


TimeRtc getTwoRowStrings(DateTime now) {
  TimeRtc time;
  String hari = days[now.dayOfTheWeek()];
  String tgl = String(now.day());
  String bln = String(now.month());
  String thn = String(now.year() % 100);
  String jam = String(now.hour());
  String min = String(now.minute());
  String sec = String(now.second());
  time.row1 = hari + " " + tgl + "/" + bln + "/" + thn;
  time.row2 = jam + ":" + min + ":" + sec;
  return time;
};


void connectToWifi();
void getTime();
void lampu_teras_otomatis();
void lampu_kamar_otomatis();

void setup() {
  Serial.begin(115200);
  rtc.begin();
  // rtc.adjust(DateTime(2023, 10, 28, 20, 55, 0));
  connectToWifi(); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // set pin relay
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
}

void loop() {
  TimeRtc lcd = getTwoRowStrings(rtc.now());

  Blynk.run();

  lampu_teras_otomatis();
  lampu_kamar_otomatis();

  Blynk.virtualWrite(V3, lcd.row1);
  Blynk.virtualWrite(V4, lcd.row2);
}


void lampu_teras_otomatis() {
  // lampu teras otomatis
  TimeRtc time = getTimeRtc(rtc.now());
  if (time.jam == 5 && time.min == 30 && time.sec == 0) {
    digitalWrite(IN1, HIGH);
    Blynk.virtualWrite(V0, LOW);
    Serial.println("V0: Off");
  }

  if (time.jam == 17 && time.min == 45 && time.sec == 0) {
    digitalWrite(IN1, LOW);
    Blynk.virtualWrite(V0, HIGH);
    Serial.println("V0: On");
  } 
}


void lampu_kamar_otomatis() {
  // lampu kamar otomatis
  TimeRtc time = getTimeRtc(rtc.now());
  if (time.jam == 4 && time.min == 45 && time.sec == 0) {
    digitalWrite(IN3, LOW);
    Blynk.virtualWrite(V2, HIGH);
    Serial.println("V2: On");
  } 
}


BLYNK_WRITE(V0) {
  int val = param.asInt();
  if (val) {
    digitalWrite(IN1, LOW);
    Serial.println("V0: On");
  }
  else {
    digitalWrite(IN1, HIGH);
    Serial.println("V0: Off");
  }
}


BLYNK_WRITE(V1) {
  int val = param.asInt();
  if (val) {
    digitalWrite(IN2, LOW);
    Serial.println("V1: On");
  }
  else {
    digitalWrite(IN2, HIGH);
    Serial.println("V1: Off");
  }
}


BLYNK_WRITE(V2) {
  int val = param.asInt();
  if (val) {
    digitalWrite(IN3, LOW);
    Serial.println("V2: On");
  }
  else {
    digitalWrite(IN3, HIGH);
    Serial.println("V2: Off");
  }
}


void connectToWifi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to " + String(ssid));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
}

void getTime() {
  DateTime now = rtc.now();
  int tahun = now.year();
  int bulan = now.month();
  int hari = now.day();
  int jam = now.hour();
  int menit = now.minute();
  int detik = now.second();
  
  Serial.println("Tahun: " + String(tahun) + ", Bulan: " + String(bulan) + ", Hari: " + String(hari) + ", Jam: " + String(jam) + ", Menit: " + String(menit) + ", Detik: " + String(detik));
  delay(1000);
}
