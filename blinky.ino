
// ==================================================
// Roshan With Embedded
// Blinky
// ==================================================

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "time.h"
#include <math.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// ==================================================
// 1. ASSETS & CONFIG
// ==================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 21
#define SCL_PIN 22
#define TOUCH_PIN 15   // you can change to any free GPIO

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- WEATHER ICONS ---
const unsigned char bmp_clear[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0xc0, 0x80, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x06, 0xff, 0xff, 0x60, 0x06, 0xff, 0xff, 0x60, 0x06, 0xff, 0xff, 0x60, 0x00, 0xff, 0xff, 0x00, 0x3e, 0xff, 0xff, 0x7c, 0x3e, 0xff, 0xff, 0x7c, 0x3e, 0xff, 0xff, 0x7c, 0x00, 0xff, 0xff, 0x00, 0x06, 0xff, 0xff, 0x60, 0x06, 0xff, 0xff, 0x60, 0x06, 0xff, 0xff, 0x60, 0x00, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x01, 0x0f, 0xf0, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char bmp_clouds[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xfe, 0x0f, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char bmp_rain[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0xe0, 0x1c, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x03, 0x80, 0x70, 0x00, 0x03, 0x80, 0x70, 0x00, 0x03, 0x00, 0x60, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char mini_sun[] PROGMEM = { 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x10, 0x08, 0x04, 0x20, 0x03, 0xc0, 0x27, 0xe4, 0x07, 0xe0, 0x07, 0xe0, 0x27, 0xe4, 0x03, 0xc0, 0x04, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00 };
const unsigned char mini_cloud[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x07, 0xe0, 0x0f, 0xf0, 0x1f, 0xf8, 0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfc, 0x7f, 0xfe, 0x3f, 0xfe, 0x1f, 0xfc, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char mini_rain[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x07, 0xe0, 0x0f, 0xf0, 0x1f, 0xf8, 0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfc, 0x7f, 0xfe, 0x3f, 0xfe, 0x1f, 0xfc, 0x00, 0x00, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11 };
const unsigned char bmp_tiny_drop[] PROGMEM = { 0x10, 0x38, 0x7c, 0xfe, 0xfe, 0x7c, 0x38, 0x00 };

// --- EMOTION PARTICLES (16x16) ---
const unsigned char bmp_heart[] PROGMEM = { 0x00, 0x00, 0x0c, 0x60, 0x1e, 0xf0, 0x3f, 0xf8, 0x7f, 0xfc, 0x7f, 0xfc, 0x7f, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char bmp_zzz[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x30, 0x00, 0x7e, 0x00, 0x00, 0x3c, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x30, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00 };
const unsigned char bmp_anger[] PROGMEM = { 0x00, 0x00, 0x11, 0x10, 0x2a, 0x90, 0x44, 0x40, 0x80, 0x20, 0x80, 0x20, 0x44, 0x40, 0x2a, 0x90, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// --- GLOBALS ---
int currentPage = 0;
const int TOTAL_PAGES = 5; 
bool highBrightness = true;
int tapCounter = 0;
unsigned long lastTapTime = 0;
bool lastPinState = false;
unsigned long pressStartTime = 0;
bool isLongPressHandled = false;
const unsigned long LONG_PRESS_TIME = 800;
const unsigned long DOUBLE_TAP_DELAY = 300;
unsigned long lastPageSwitch = 0;
const unsigned long PAGE_INTERVAL = 8000;

// MOODS
#define MOOD_NORMAL 0
#define MOOD_HAPPY 1
#define MOOD_SURPRISED 2
#define MOOD_SLEEPY 3
#define MOOD_ANGRY 4
#define MOOD_SAD 5
#define MOOD_EXCITED 6
#define MOOD_LOVE 7
#define MOOD_SUSPICIOUS 8
int currentMood = MOOD_NORMAL;

String city;       // Loaded from Preferences
String countryCode; // Loaded from Preferences
String apiKey;   // Loaded from Preferences or defaults
String wifiSsid; // Loaded from Preferences
String wifiPass; // Loaded from Preferences
unsigned long lastWeatherUpdate = 0;
float temperature = 0.0;
float feelsLike = 0.0;
int humidity = 0;
String weatherMain = "Loading";
String weatherDesc = "Wait...";

struct ForecastDay {
  String dayName;
  int temp;
  String iconType;
};
ForecastDay fcast[3];

const char* ntpServer = "pool.ntp.org";
String tzString = "IST-5:30"; // Loaded from Preferences (e.g. "IST-5:30")
//const char* tzString = "IST-5:30";   // Loaded from Preferences (e.g. "IST-5:30")

// ==================================================
// 2. ULTRA PRO PHYSICS ENGINE
// ==================================================

struct Eye {
  float x, y;  // Current position (Top-Left)
  float w, h;  // Current size
  float targetX, targetY, targetW, targetH;

  // Pupil Physics (Secondary Motion)
  float pupilX, pupilY;
  float targetPupilX, targetPupilY;

  // Physics constants
  float velX, velY, velW, velH;
  float pVelX, pVelY;
  float k = 0.12;   // Eye Spring
  float d = 0.60;   // Eye Damping (Heavier feel)
  float pk = 0.08;  // Pupil Spring (Softer/Laggier)
  float pd = 0.50;  // Pupil Damping

  bool blinking;
  unsigned long lastBlink;
  unsigned long nextBlinkTime;

  void init(float _x, float _y, float _w, float _h) {
    x = targetX = _x;
    y = targetY = _y;
    w = targetW = _w;
    h = targetH = _h;
    pupilX = targetPupilX = 0;
    pupilY = targetPupilY = 0;
    nextBlinkTime = millis() + random(1000, 4000);
  }

  void update() {
    // 1. Main Eye Physics
    float ax = (targetX - x) * k;
    float ay = (targetY - y) * k;
    float aw = (targetW - w) * k;
    float ah = (targetH - h) * k;

    velX = (velX + ax) * d;
    velY = (velY + ay) * d;
    velW = (velW + aw) * d;
    velH = (velH + ah) * d;

    x += velX;
    y += velY;
    w += velW;
    h += velH;

    // 2. Pupil Physics (Dragging behind)
    float pax = (targetPupilX - pupilX) * pk;
    float pay = (targetPupilY - pupilY) * pk;
    pVelX = (pVelX + pax) * pd;
    pVelY = (pVelY + pay) * pd;
    pupilX += pVelX;
    pupilY += pVelY;
  }
};

Eye leftEye, rightEye;
unsigned long lastSaccade = 0;
unsigned long saccadeInterval = 3000;
float breathVal = 0;

// ==================================================
// 2b. CONFIG PORTAL (WiFi + API Key via local web)
// ==================================================
#define CONFIG_AP_SSID   "Blinky-Setup"
#define CONFIG_AP_PASS   "12345678"
#define CONFIG_HOLD_MS   5000

Preferences prefs;
WebServer configServer(80);
bool inConfigMode = false;

void loadConfig() {
  prefs.begin("deskbuddy", true);
  wifiSsid    = prefs.getString("ssid", "");
  wifiPass    = prefs.getString("pass", "");
  apiKey      = prefs.getString("apikey", "af18fb12705e7d8c7aabbb6fc1a461d7");
  city        = prefs.getString("city", "Bengaluru");
  countryCode = prefs.getString("country", "IN");
  tzString    = prefs.getString("tz", "IST-5:30");
  prefs.end();
  
  Serial.println("=== Config Loaded ===");
  Serial.print("SSID: "); Serial.println(wifiSsid);
  Serial.print("City: "); Serial.println(city);
  Serial.print("API Key: "); Serial.println(apiKey.isEmpty() ? "EMPTY" : "SET");
}

void clearAllPreferences() {
  prefs.begin("deskbuddy", false);
  prefs.clear();  // Erase everything
  prefs.end();
  Serial.println("=== ALL PREFERENCES CLEARED ===");
}
void saveConfig(const String& s, const String& p, const String& ak,
                const String& cty, const String& ctry, const String& tz) {
  prefs.begin("deskbuddy", false);
  prefs.putString("ssid", s);
  prefs.putString("pass", p);
  prefs.putString("apikey", ak);
  prefs.putString("city", cty);
  prefs.putString("country", ctry);
  prefs.putString("tz", tz);
  prefs.end();
}

void handleConfigRoot() {
  prefs.begin("deskbuddy", true);
  String sSsid = prefs.getString("ssid", "");
  String sApik = prefs.getString("apikey", "af18fb12705e7d8c7aabbb6fc1a461d7");
  String sCity = prefs.getString("city", "Bengaluru");
  String sCtry = prefs.getString("country", "IN");
  String sTz   = prefs.getString("tz", "IST-5:30");
  prefs.end();

  String html = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Blinky Config</title>
<style>
body{font-family:sans-serif;max-width:420px;margin:30px auto;padding:24px;background:#0c1929;color:#e8f4fc;}
h1{color:#5ba3f5;margin-bottom:8px;}
input{width:100%;padding:10px;margin:6px 0;border:1px solid #2d4a6f;border-radius:6px;box-sizing:border-box;background:#1a2d47;color:#e8f4fc;}
input:focus{outline:none;border-color:#5ba3f5;}
button{width:100%;padding:12px;background:#3498db;color:#fff;border:none;border-radius:6px;font-size:16px;cursor:pointer;margin-top:16px;}
button:hover{background:#2980b9;}
label{display:block;margin-top:14px;color:#8ab4e8;font-size:14px;}
.section{margin-top:20px;padding-top:16px;border-top:1px solid #1e3a5f;}
.section-title{color:#5ba3f5;font-size:13px;margin-bottom:8px;}
</style></head><body>
<h1>Blinky Setup</h1>
<form action="/save" method="POST">
<label>WiFi SSID</label><input name="ssid" placeholder="Your WiFi name" value=")rawliteral";
  html += sSsid;
  html += R"rawliteral(">
<label>WiFi Password</label><input name="pass" type="password" placeholder="WiFi password">
<div class="section"><div class="section-title">Weather (OpenWeatherMap)</div>
<label>API Key</label><input name="apikey" placeholder="API key" value=")rawliteral";
  html += sApik;
  html += R"rawliteral(">
<label>City</label><input name="city" placeholder="e.g. London" value=")rawliteral";
  html += sCity;
  html += R"rawliteral(">
<label>Country Code</label><input name="country" placeholder="e.g. IN, US, GB" value=")rawliteral";
  html += sCtry;
  html += R"rawliteral(">
</div>
<div class="section"><div class="section-title">Time</div>
<label>Timezone</label><input name="tz" placeholder="e.g. IST-5:30, EST5EDT" value=")rawliteral";
  html += sTz;
  html += R"rawliteral(">
</div>
<button type="submit">Save &amp; Reboot</button>
</form></body></html>)rawliteral";
  configServer.send(200, "text/html", html);
}

void handleConfigSave() {
  if (!configServer.hasArg("ssid") || configServer.arg("ssid").length() == 0) {
    configServer.send(400, "text/html",
      "<html><body style='font-family:sans-serif;background:#0c1929;color:#e8f4fc;padding:40px;'>"
      "<h2 style='color:#f55b5b'>Error!</h2><p>WiFi SSID is required</p>"
      "<a href='/' style='color:#5ba3f5'>Go Back</a></body></html>");
    return;
  }
  
  String s   = configServer.arg("ssid");
  String p   = configServer.arg("pass");
  String ak  = configServer.arg("apikey");
  String cty = configServer.arg("city");
  String ctr = configServer.arg("country");
  String tz  = configServer.arg("tz");
  
  // Set defaults if empty
  if (ak.isEmpty())  ak  = "af18fb12705e7d8c7aabbb6fc1a461d7";
  if (cty.isEmpty()) cty = "Bengaluru";
  if (ctr.isEmpty()) ctr = "IN";
  if (tz.isEmpty())  tz  = "IST-5:30";
  
  Serial.println("=== Saving Config ===");
  Serial.print("SSID: "); Serial.println(s);
   Serial.print("Pass: "); Serial.println(p.isEmpty() ? "EMPTY" : "****");
  Serial.print("City: "); Serial.println(cty);
    Serial.print("Country: "); Serial.println(ctr);
  Serial.print("TZ: "); Serial.println(tz);
  
  saveConfig(s, p, ak, cty, ctr, tz);
  
  configServer.send(200, "text/html",
    "<html><body style='font-family:sans-serif;background:#0c1929;color:#e8f4fc;padding:40px;'>"
    "<h2 style='color:#5ba3f5'>Saved!</h2><p>Rebooting in 3 seconds...</p></body></html>");
  
  delay(3000);
  ESP.restart();
}

void startConfigPortal() {
  inConfigMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(CONFIG_AP_SSID, CONFIG_AP_PASS);
  configServer.on("/", handleConfigRoot);
  configServer.on("/save", HTTP_POST, handleConfigSave);
  configServer.begin();
  display.clearDisplay();
  display.setFont(NULL);
  display.setCursor(0, 0);
  display.print("Config mode\n\nConnect to:\n");
  display.print(CONFIG_AP_SSID);
  display.print("\n\nThen open:\n192.168.4.1");
  display.display();
}

// ==================================================
// 3. LOGIC & NETWORK
// ==================================================
const unsigned char* getBigIcon(String w) {
  if (w == "Clear") return bmp_clear;
  if (w == "Clouds") return bmp_clouds;
  if (w == "Rain" || w == "Drizzle") return bmp_rain;
  return bmp_clouds;
}
const unsigned char* getMiniIcon(String w) {
  if (w == "Clear") return mini_sun;
  if (w == "Rain" || w == "Drizzle" || w == "Thunderstorm") return mini_rain;
  return mini_cloud;
}

void updateMoodBasedOnWeather() {
  int m = MOOD_NORMAL;
  if (weatherMain == "Clear") m = MOOD_HAPPY;
  else if (weatherMain == "Rain" || weatherMain == "Drizzle") m = MOOD_SAD;
  else if (weatherMain == "Thunderstorm") m = MOOD_SURPRISED;
  else if (weatherMain == "Clouds") m = MOOD_NORMAL;
  else if (temperature > 25) m = MOOD_EXCITED;
  else if (temperature < 5) m = MOOD_SLEEPY;
  currentMood = m;
}

void handleTouch() {
  bool currentPinState = digitalRead(TOUCH_PIN);
  unsigned long now = millis();
  
  // Touch started (transition from LOW to HIGH)
  if (currentPinState && !lastPinState) {
    pressStartTime = now;
    isLongPressHandled = false;
  } 
  // Touch being held (LONG PRESS for mood change only)
  else if (currentPinState && lastPinState) {
    if ((now - pressStartTime > LONG_PRESS_TIME) && !isLongPressHandled) {
      lastPageSwitch = now;
      // Long press only changes mood on emotion page
      if (currentPage == 0) {
        currentMood++;
        if (currentMood > MOOD_SUSPICIOUS) currentMood = 0;
        lastSaccade = 0;
      }
      isLongPressHandled = true;
    }
  } 
  // Touch released (SHORT TAP)
  else if (!currentPinState && lastPinState) {
    if ((now - pressStartTime < LONG_PRESS_TIME) && !isLongPressHandled) {
      tapCounter++;
      lastTapTime = now;
    }
  }
  
  lastPinState = currentPinState;
  
  // Check for double tap or single tap
  if (tapCounter > 0) {
    if (now - lastTapTime > DOUBLE_TAP_DELAY) {
      lastPageSwitch = now;
      
      // Double tap = brightness toggle
      if (tapCounter == 2) {
        highBrightness = !highBrightness;
        display.setContrast(highBrightness ? 255 : 1);
        display.display();
      } 
      // Single tap = next page
      else if (tapCounter == 1) {
        currentPage++;
        if (currentPage >= TOTAL_PAGES) currentPage = 0;
      }
      
      tapCounter = 0;
    }
  }
}
void getWeatherAndForecast() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return;
  }

  HTTPClient http;

  // ================= CURRENT WEATHER =================
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + apiKey + "&units=metric";

  Serial.println("Requesting Weather:");
  Serial.println(url);

  http.begin(url);
  int httpCode = http.GET();

  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    String payload = http.getString();

    Serial.println("Payload:");
    Serial.println(payload);

    JSONVar myObject = JSON.parse(payload);

    if (JSON.typeof(myObject) != "undefined" && myObject.hasOwnProperty("main")) {

      temperature = double(myObject["main"]["temp"]);
      feelsLike  = double(myObject["main"]["feels_like"]);
      humidity   = int(myObject["main"]["humidity"]);

      weatherMain = (const char*)myObject["weather"][0]["main"];
      weatherDesc = (const char*)myObject["weather"][0]["description"];

      // 🔥 SAFE uppercase
      if (weatherDesc.length() > 0) {
        weatherDesc[0] = toupper(weatherDesc[0]);
      }

      Serial.println("Parsed Weather OK");
      updateMoodBasedOnWeather();

    } else {
      Serial.println("ERROR: JSON missing 'main'");
    }

  } else {
    Serial.println("ERROR: HTTP request failed");
  }

  http.end();

  // ================= FORECAST =================
  url = "http://api.openweathermap.org/data/2.5/forecast?q=" + city + "," + countryCode + "&appid=" + apiKey + "&units=metric";

  Serial.println("Requesting Forecast:");
  Serial.println(url);

  http.begin(url);
  httpCode = http.GET();

  Serial.print("Forecast HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    String payload = http.getString();

    Serial.println("Forecast Payload:");
    Serial.println(payload);

    JSONVar fo = JSON.parse(payload);

    if (JSON.typeof(fo) != "undefined" && fo.hasOwnProperty("list")) {

      struct tm t;
      getLocalTime(&t);

      int today = t.tm_wday;
      const char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

      int indices[3] = { 7, 15, 23 };

      for (int i = 0; i < 3; i++) {
        int idx = indices[i];

        fcast[i].temp = (int)double(fo["list"][idx]["main"]["temp"]);
        fcast[i].iconType = (const char*)fo["list"][idx]["weather"][0]["main"];

        int nextDayIndex = (today + i + 1) % 7;
        fcast[i].dayName = days[nextDayIndex];
      }

      Serial.println("Forecast Parsed OK");

    } else {
      Serial.println("ERROR: Forecast JSON invalid");
    }

  } else {
    Serial.println("ERROR: Forecast HTTP failed");
  }

  http.end();
}

// ==================================================
// 4. DRAWING & ANIMATION
// ==================================================

void drawEyelidMask(float x, float y, float w, float h, int mood, bool isLeft) {
  int ix = (int)x;
  int iy = (int)y;
  int iw = (int)w;
  int ih = (int)h;
  display.setTextColor(SH110X_BLACK);

  // ANGRY: Sharp slanted cut
  if (mood == MOOD_ANGRY) {
    if (isLeft)
      for (int i = 0; i < 16; i++) display.drawLine(ix, iy + i, ix + iw, iy - 6 + i, SH110X_BLACK);
    else
      for (int i = 0; i < 16; i++) display.drawLine(ix, iy - 6 + i, ix + iw, iy + i, SH110X_BLACK);
  }
  // SAD: Inverse slanted cut
  else if (mood == MOOD_SAD) {
    if (isLeft)
      for (int i = 0; i < 16; i++) display.drawLine(ix, iy - 6 + i, ix + iw, iy + i, SH110X_BLACK);
    else
      for (int i = 0; i < 16; i++) display.drawLine(ix, iy + i, ix + iw, iy - 6 + i, SH110X_BLACK);
  }
  // HAPPY/LOVE: Cheek push up
  else if (mood == MOOD_HAPPY || mood == MOOD_LOVE || mood == MOOD_EXCITED) {
    display.fillRect(ix, iy + ih - 12, iw, 14, SH110X_BLACK);
    display.fillCircle(ix + iw / 2, iy + ih + 6, iw / 1.3, SH110X_BLACK);  // Round cut
  }
  // SLEEPY: Heavy lids
  else if (mood == MOOD_SLEEPY) {
    display.fillRect(ix, iy, iw, ih / 2 + 2, SH110X_BLACK);
  }
  // SUSPICIOUS: One eye squint, one open
  else if (mood == MOOD_SUSPICIOUS) {
    if (isLeft) display.fillRect(ix, iy, iw, ih / 2 - 2, SH110X_BLACK);
    else display.fillRect(ix, iy + ih - 8, iw, 8, SH110X_BLACK);
  }
}

void drawUltraProEye(Eye& e, bool isLeft) {
  int ix = (int)e.x;
  int iy = (int)e.y;
  int iw = (int)e.w;
  int ih = (int)e.h;

  // 1. Draw Sclera (White base)
  int r = 8;
  if (iw < 20) r = 3;
  display.fillRoundRect(ix, iy, iw, ih, r, SH110X_WHITE);

  // 2. Draw Pupil (Black inner eye) - Constrained to be inside
  // Calculate center of eye
  int cx = ix + iw / 2;
  int cy = iy + ih / 2;

  // Pupil size is proportional to eye size
  int pw = iw / 2.2;
  int ph = ih / 2.2;

  // Apply pupil offset (Gaze)
  int px = cx + (int)e.pupilX - (pw / 2);
  int py = cy + (int)e.pupilY - (ph / 2);

  // Clamp pupil to ensure it doesn't leave the eye completely
  if (px < ix) px = ix;
  if (px + pw > ix + iw) px = ix + iw - pw;
  if (py < iy) py = iy;
  if (py + ph > iy + ih) py = iy + ih - ph;

  display.fillRoundRect(px, py, pw, ph, r / 2, SH110X_BLACK);

  // 3. Draw Specular Highlight (The "Glint" of life)
  // A tiny white dot in the top-right of the pupil
  if (iw > 15 && ih > 15) {
    display.fillCircle(px + pw - 4, py + 4, 2, SH110X_WHITE);
  }

  // 4. Apply Eyelid Masks (Expressions)
  drawEyelidMask(e.x, e.y, e.w, e.h, currentMood, isLeft);
}

void updatePhysicsAndMood() {
  unsigned long now = millis();
  breathVal = sin(now / 800.0) * 1.5;  // Breathing effect

  // --- BLINK LOGIC ---
  if (now > leftEye.nextBlinkTime) {
    leftEye.blinking = true;
    leftEye.lastBlink = now;
    rightEye.blinking = true;
    leftEye.nextBlinkTime = now + random(2000, 6000);
  }
  if (leftEye.blinking) {
    leftEye.targetH = 2;
    rightEye.targetH = 2;  // Close
    if (now - leftEye.lastBlink > 120) {
      leftEye.blinking = false;
      rightEye.blinking = false;
    }
  }

  // --- SACCADE (Gaze) LOGIC ---
  if (!leftEye.blinking && now - lastSaccade > saccadeInterval) {
    lastSaccade = now;
    saccadeInterval = random(500, 3000);

    // Pick direction
    int dir = random(0, 10);
    float lx = 0, ly = 0;

    if (dir < 4) {
      lx = 0;
      ly = 0;
    }  // Center
    else if (dir == 4) {
      lx = -6;
      ly = -4;
    }  // TL
    else if (dir == 5) {
      lx = 6;
      ly = -4;
    }  // TR
    else if (dir == 6) {
      lx = -6;
      ly = 4;
    }  // BL
    else if (dir == 7) {
      lx = 6;
      ly = 4;
    }  // BR
    else if (dir == 8) {
      lx = 8;
      ly = 0;
    }  // R
    else if (dir == 9) {
      lx = -8;
      ly = 0;
    }  // L

    // Move pupil target relative to center
    leftEye.targetPupilX = lx;
    leftEye.targetPupilY = ly;
    rightEye.targetPupilX = lx;
    rightEye.targetPupilY = ly;

    // Move eye container slightly (Head follow)
    leftEye.targetX = 18 + (lx * 0.3);
    leftEye.targetY = 14 + (ly * 0.3);
    rightEye.targetX = 74 + (lx * 0.3);
    rightEye.targetY = 14 + (ly * 0.3);
  }

  // --- MOOD SHAPES (Overrides targets) ---
  if (!leftEye.blinking) {
    float baseW = 36;
    float baseH = 36;

    // Breathing effect applied to height
    baseH += breathVal;

    switch (currentMood) {
      case MOOD_NORMAL:
        leftEye.targetW = baseW;
        leftEye.targetH = baseH;
        rightEye.targetW = baseW;
        rightEye.targetH = baseH;
        break;
      case MOOD_HAPPY:
      case MOOD_LOVE:
        leftEye.targetW = 40;
        leftEye.targetH = 32;
        rightEye.targetW = 40;
        rightEye.targetH = 32;
        break;
      case MOOD_SURPRISED:
        leftEye.targetW = 30;
        leftEye.targetH = 45;
        rightEye.targetW = 30;
        rightEye.targetH = 45;
        // Jitter pupil
        leftEye.targetPupilX += random(-1, 2);
        break;
      case MOOD_SLEEPY:
        leftEye.targetW = 38;
        leftEye.targetH = 30;
        rightEye.targetW = 38;
        rightEye.targetH = 30;
        break;
      case MOOD_ANGRY:
        leftEye.targetW = 34;
        leftEye.targetH = 32;
        rightEye.targetW = 34;
        rightEye.targetH = 32;
        break;
      case MOOD_SAD:
        leftEye.targetW = 34;
        leftEye.targetH = 40;
        rightEye.targetW = 34;
        rightEye.targetH = 40;
        break;
      case MOOD_SUSPICIOUS:
        leftEye.targetW = 36;
        leftEye.targetH = 20;  // Left Squint
        rightEye.targetW = 36;
        rightEye.targetH = 42;  // Right Wide
        break;
    }
  }

  leftEye.update();
  rightEye.update();
}

void drawEmoPage() {
  updatePhysicsAndMood();

  // Draw Floating Particles based on Mood
  if (currentMood == MOOD_LOVE) {
    display.drawBitmap(56, 0, bmp_heart, 16, 16, SH110X_WHITE);
  } else if (currentMood == MOOD_SLEEPY) {
    display.drawBitmap(110, 0, bmp_zzz, 16, 16, SH110X_WHITE);
  } else if (currentMood == MOOD_ANGRY) {
    display.drawBitmap(56, 0, bmp_anger, 16, 16, SH110X_WHITE);
  }

  drawUltraProEye(leftEye, true);
  drawUltraProEye(rightEye, false);
}

// --- STANDARD PAGES ---
void drawForecastPage() {
  display.fillRect(0, 0, 128, 16, SH110X_WHITE);
  display.setFont(NULL);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(20, 4);
  display.print("3-DAY FORECAST");
  display.setTextColor(SH110X_WHITE);
  display.drawLine(42, 16, 42, 64, SH110X_WHITE);
  display.drawLine(85, 16, 85, 64, SH110X_WHITE);
  for (int i = 0; i < 3; i++) {
    int xStart = i * 43;
    int centerX = xStart + 21;
    display.setFont(NULL);
    String d = fcast[i].dayName;
    if (d == "") d = "Wait";
    display.setCursor(centerX - (d.length() * 3), 20);
    display.print(d);
    display.drawBitmap(centerX - 8, 28, getMiniIcon(fcast[i].iconType), 16, 16, SH110X_WHITE);
    display.setFont(&FreeSansBold9pt7b);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(fcast[i].temp).c_str(), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(centerX - (w / 2) - 2, 60);
    display.print(fcast[i].temp);
    display.fillCircle(centerX + (w / 2) + 1, 52, 2, SH110X_WHITE);
  }
}
void drawClock() {
  struct tm t;
  if (!getLocalTime(&t)) {
    display.setFont(NULL);
    display.setTextColor(SH110X_WHITE);  // ADD THIS
    display.setCursor(30, 30);
    display.print("Syncing...");
    return;
  }
  display.setTextColor(SH110X_WHITE);  // ADD THIS
  String ampm = (t.tm_hour >= 12) ? "PM" : "AM";
  int h12 = t.tm_hour % 12;
  if (h12 == 0) h12 = 12;
  
  display.setFont(NULL);
  display.setTextSize(1);
  display.setCursor(114, 0);
  display.print(ampm);
  display.setFont(&FreeSansBold18pt7b);
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", h12, t.tm_min);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 42);
  display.print(timeStr);
  display.setFont(&FreeSans9pt7b);
  char dateStr[20];
  strftime(dateStr, 20, "%a, %b %d", &t);
  display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 62);
  display.print(dateStr);
}
void drawWeatherCard() {
  if (WiFi.status() != WL_CONNECTED) {
    display.setFont(NULL);
    display.setTextColor(SH110X_WHITE);  // ADD THIS
    display.setCursor(0, 0);
    display.print("No WiFi");
    return;
  }
   display.setTextColor(SH110X_WHITE);  // RESET COLOR TO WHITE
  display.drawBitmap(96, 0, getBigIcon(weatherMain), 32, 32, SH110X_WHITE);
  display.setFont(&FreeSansBold9pt7b);
  String c = city;
  c.toUpperCase();
  display.setCursor(0, 14);
  if (c.length() > 9) c = c.substring(0, 8) + ".";
  display.print(c);
  display.setFont(&FreeSansBold18pt7b);
  int tempInt = (int)temperature;
  display.setCursor(0, 48);
  display.print(tempInt);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(String(tempInt).c_str(), 0, 48, &x1, &y1, &w, &h);
  display.fillCircle(x1 + w + 5, 26, 4, SH110X_WHITE);
  display.setFont(NULL);
  display.drawBitmap(88, 32, bmp_tiny_drop, 8, 8, SH110X_WHITE);
  display.setCursor(100, 32);
  display.print(humidity);
  display.print("%");
  display.setCursor(88, 45);
  display.print("~");
  display.print((int)feelsLike);
  display.drawLine(0, 52, 128, 52, SH110X_WHITE);
  display.setCursor(0, 55);
  display.print(weatherDesc);
}
void drawWorldClock() {
display.setTextColor(SH110X_WHITE);  // ADD THIS AT THE START
  struct tm t;
  if (!getLocalTime(&t)) {
    display.setFont(NULL);
    display.setCursor(20, 28);
    display.print("Time not synced");
    return;
  }
  time_t now;
  time(&now);
  time_t indiaEpoch = now + (5 * 3600) + (30 * 60);
  time_t sydneyEpoch = now + (11 * 3600);
  struct tm* indiatm = gmtime(&indiaEpoch);
  int i_h = indiatm->tm_hour;
  int i_m = indiatm->tm_min;
  struct tm* sydneytm = gmtime(&sydneyEpoch);
  int s_h = sydneytm->tm_hour;
  int s_m = sydneytm->tm_min;
  display.fillRect(0, 0, 128, 16, SH110X_WHITE);
  display.setFont(NULL);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(32, 4);
  display.print("WORLD CLOCK");
  display.setTextColor(SH110X_WHITE);
  display.drawLine(64, 18, 64, 54, SH110X_WHITE);
  display.setFont(NULL);
  display.setCursor(16, 22);
  display.print("INDIA");
  display.setFont(&FreeSansBold9pt7b);
  char iStr[10];
  sprintf(iStr, "%02d:%02d", i_h, i_m);
  display.setCursor(5, 46);
  display.print(iStr);
  display.setFont(NULL);
  display.setCursor(78, 22);
  display.print("SYDNEY");
  display.setFont(&FreeSansBold9pt7b);
  char sStr[10];
  sprintf(sStr, "%02d:%02d", s_h, s_m);
  display.setCursor(69, 46);
  display.print(sStr);
  display.setFont(NULL);
  display.setCursor(35, 56);
  display.print("Tap to Exit");
}

// ==================================================
// 5. BOOT & MAIN
// ==================================================

void playBootAnimation() {
  display.setTextColor(SH110X_WHITE);
  int cx = 64;
  int cy = 32;
  // Expanding Circle Wipe
  for (int r = 0; r < 80; r += 4) {
    display.clearDisplay();
    display.fillCircle(cx, cy, r, SH110X_WHITE);
    display.display();
    delay(10);
  }
  // Inverse Wipe
  for (int r = 0; r < 80; r += 4) {
    display.clearDisplay();
    display.fillCircle(cx, cy, 80, SH110X_WHITE);
    display.fillCircle(cx, cy, r, SH110X_BLACK);
    display.display();
    delay(10);
  }

  // Text Display - "PR-TECH"
  display.setFont(&FreeSansBold9pt7b);
  String bootText = "Blinky";

  // Calculate width to center it perfectly
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(bootText, 0, 0, &x1, &y1, &w, &h);

  display.clearDisplay();
  display.setCursor((SCREEN_WIDTH - w) / 2, 36);
  display.print(bootText);
  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n\n=== DESK BUDDY STARTING ===");

  // 🔥 TOUCH PIN FIRST
  pinMode(TOUCH_PIN, INPUT);
  
  // 🔥 I2C INIT - CRITICAL TIMING FOR NO-RST DISPLAYS
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);  // Conservative 100kHz
  delay(1000);  // LONGER delay for capacitor discharge on power-up

  Serial.println("Attempting I2C scan...");
  
  // 🔥 I2C SCANNER - Verify OLED is detected
  Wire.beginTransmission(0x3C);
  byte error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("OLED detected at 0x3C");
  } else {
    Serial.println("ERROR: OLED NOT detected!");
    Serial.println("Check wiring: SDA->21, SCL->22");
    delay(5000);
    ESP.restart();
  }

  // 🔥 SOFTWARE RESET SEQUENCE (Since no hardware RST pin)
  // This mimics what a hardware reset would do
  
  Serial.println("Sending software reset commands...");
  
  // Turn display OFF
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);  // Command mode
  Wire.write(0xAE);  // Display OFF
  if (Wire.endTransmission() != 0) {
    Serial.println("Command failed!");
  }
  delay(200);
  
  // Set display clock divide ratio
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xD5);  // Set display clock
  Wire.write(0x80);  // Default ratio
  Wire.endTransmission();
  delay(50);
  
  // Set multiplex ratio
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xA8);  // Set multiplex
  Wire.write(0x3F);  // 1/64 duty
  Wire.endTransmission();
  delay(50);
  
  // Set display offset
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xD3);  // Set display offset
  Wire.write(0x00);  // No offset
  Wire.endTransmission();
  delay(50);
  
  // Set start line
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0x40);  // Set start line to 0
  Wire.endTransmission();
  delay(50);
  
  // Charge pump setting (CRITICAL for OLED without external VBAT)
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0x8D);  // Charge pump command
  Wire.write(0x14);  // Enable charge pump
  Wire.endTransmission();
  delay(100);
  
  // Segment re-map
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xA1);  // Segment remap (column 127 mapped to SEG0)
  Wire.endTransmission();
  delay(50);
  
  // COM output scan direction
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xC8);  // Scan from COM[N-1] to COM0
  Wire.endTransmission();
  delay(50);
  
  // Set COM pins hardware configuration
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xDA);  // Set COM pins
  Wire.write(0x12);  // Alternative COM pin config
  Wire.endTransmission();
  delay(50);
  
  // Set contrast
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0x81);  // Set contrast command
  Wire.write(0xFF);  // Maximum contrast
  Wire.endTransmission();
  delay(50);
  
  // Set pre-charge period
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xD9);  // Set pre-charge
  Wire.write(0xF1);  // Phase 1: 15 clocks, Phase 2: 1 clock
  Wire.endTransmission();
  delay(50);
  
  // Set VCOMH deselect level
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xDB);  // Set VCOMH
  Wire.write(0x40);  // ~0.77 x VCC
  Wire.endTransmission();
  delay(50);
  
  // Entire display ON (resume from RAM)
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xA4);  // Resume from RAM
  Wire.endTransmission();
  delay(50);
  
  // Set normal display (not inverted)
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xA6);  // Normal display
  Wire.endTransmission();
  delay(50);
  
  // Deactivate scroll
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0x2E);  // Deactivate scroll
  Wire.endTransmission();
  delay(50);
  
  // TURN DISPLAY ON
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(0xAF);  // Display ON
  Wire.endTransmission();
  delay(300);  // Wait for display to stabilize

  Serial.println("Software reset complete - Display should be ON");

  // 🔥 NOW INIT ADAFRUIT LIBRARY
  Serial.println("Initializing Adafruit library...");
  
  bool displayReady = false;
  
  for (int attempt = 0; attempt < 3; attempt++) {
    Serial.print("Init attempt ");
    Serial.println(attempt + 1);
    
    // Pass TRUE for reset parameter even though we don't have RST pin
    // The library will skip the hardware reset if no pin is defined
    if (display.begin(0x3C, true)) {
      displayReady = true;
      Serial.println("✓ Adafruit library init SUCCESS");
      break;
    } else {
      Serial.println("✗ Adafruit library init FAILED");
      delay(500);
    }
  }

  if (!displayReady) {
    Serial.println("CRITICAL: Display init failed - RESTARTING in 3s");
    delay(3000);
    ESP.restart();
  }

  // 🔥 VERIFY DISPLAY IS WORKING
  display.clearDisplay();
  display.setContrast(255);
  display.setRotation(0);
  
  // Draw test pattern
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setFont(NULL);
  
  display.fillRect(0, 0, 128, 64, SH110X_BLACK);
  display.drawRect(0, 0, 128, 64, SH110X_WHITE);
  display.setCursor(20, 10);
  display.print("BOOT OK");
  display.drawLine(0, 20, 128, 20, SH110X_WHITE);
  display.setCursor(15, 30);
  display.print("OLED READY");
  
  display.display();
  delay(1500);

  Serial.println("Display verified working!");

  // Clear for next stage
  display.clearDisplay();
  display.display();
  
  // ================= FACTORY RESET CHECK =================
  bool factoryReset = false;
  unsigned long factoryResetStart = millis();
  unsigned long pressStart = 0;
  bool pressed = false;

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.print("Hold 5s for");
  display.setCursor(0, 20);
  display.print("factory reset");
  display.display();

  while (millis() - factoryResetStart < 5000) {
    if (digitalRead(TOUCH_PIN) == HIGH) {
      if (!pressed) {
        pressStart = millis();
        pressed = true;
      }
      if (millis() - pressStart >= 5000) {
        factoryReset = true;
        break;
      }
    } else {
      pressed = false;
    }
    delay(50);
  }

  if (factoryReset) {
    display.clearDisplay();
    display.setCursor(10, 20);
    display.print("Resetting...");
    display.display();
    
    prefs.begin("deskbuddy", false);
    prefs.clear();
    prefs.end();
    
    delay(2000);
    ESP.restart();
  }

  // ================= LOAD CONFIG =================
  loadConfig();

  // ================= CONFIG MODE CHECK =================
  bool forceConfig = false;
  unsigned long configCheckStart = millis();
  pressed = false;
  pressStart = 0;

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 5);
  display.print("Hold 5s for");
  display.setCursor(0, 15);
  display.print("config mode");
  display.display();

  while (millis() - configCheckStart < 5000) {
    if (digitalRead(TOUCH_PIN) == HIGH) {
      if (!pressed) {
        pressStart = millis();
        pressed = true;
      }
      if (millis() - pressStart >= 5000) {
        forceConfig = true;
        break;
      }
    } else {
      pressed = false;
    }
    delay(50);
  }

  if (wifiSsid.isEmpty()) {
    forceConfig = true;
  }

  if (forceConfig) {
    startConfigPortal();
    return;
  }

  // ================= NORMAL BOOT =================
  Serial.println("Starting normal boot...");

  leftEye.init(18, 14, 36, 36);
  rightEye.init(74, 14, 36, 36);

  playBootAnimation();

  playBootAnimation();

  // ================= WIFI =================
  display.clearDisplay();
  display.setCursor(20, 20);
  display.print("Connecting WiFi...");
  display.display();

  Serial.print("Connecting to: ");
  Serial.println(wifiSsid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid.c_str(), wifiPass.c_str());

  unsigned long wifiStart = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - wifiStart < 15000)) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());

    display.clearDisplay();
    display.setCursor(10, 20);
    display.print("Connected!");
    display.setCursor(10, 35);
    display.print(WiFi.localIP());
    display.display();
    delay(2000);

  } else {
    Serial.println("\nWiFi FAILED");

    display.clearDisplay();
    display.setCursor(10, 20);
    display.print("WiFi Failed");
    display.display();
    delay(2000);

    startConfigPortal();
    return;
  }

  // ================= TIME =================
  display.clearDisplay();
  display.setCursor(15, 28);
  display.print("Syncing time...");
  display.display();

  configTime(0, 0, ntpServer);
  setenv("TZ", tzString.c_str(), 1);
  tzset();

  struct tm timeinfo;
  int attempts = 0;
  while (!getLocalTime(&timeinfo) && attempts < 20) {
    delay(500);
    attempts++;
  }

  // ================= WEATHER =================
  display.clearDisplay();
  display.setCursor(10, 28);
  display.print("Getting weather...");
  display.display();

  getWeatherAndForecast();

  lastWeatherUpdate = millis();
  lastPageSwitch = millis();

  Serial.println("=== BOOT COMPLETE ===");

  display.clearDisplay();
  display.setCursor(30, 28);
  display.print("Ready!");
  display.display();
  delay(1000);
}

void loop() {
  if (inConfigMode) {
    configServer.handleClient();
    return;
  }
  
  unsigned long now = millis();
  
  handleTouch();
  
  // Update weather every 10 minutes
  if (now - lastWeatherUpdate > 600000) {
    getWeatherAndForecast();
    lastWeatherUpdate = now;
  }

  // Auto page rotation - ALL 5 pages
  if (now - lastPageSwitch > PAGE_INTERVAL) {
    currentPage++;
    if (currentPage >= TOTAL_PAGES) currentPage = 0;
    lastPageSwitch = now;
    lastSaccade = 0;
  }

  // Draw current page
  display.clearDisplay();
  
  switch (currentPage) {
    case 0: 
      drawEmoPage(); 
      break;
      
    case 1: 
      drawClock(); 
      break;
      
    case 2: 
      drawWeatherCard(); 
      break;
      
    case 3: 
      drawWorldClock(); 
      break;
      
    case 4: 
      drawForecastPage(); 
      break;
  }
  
  display.display();
}