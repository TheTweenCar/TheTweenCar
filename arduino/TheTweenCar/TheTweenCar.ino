#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Html.hpp"

/*
 * private.h is not commited to the repository.
 * This contains information private to the indrvidual building the code.
 * PRIVATE_STASSID
 * PRIVATE_STAPSK
 */
 
#include "private.h"
const char* ssid = PRIVATE_STASSID;     /// The SSID for the WiFi network. (Add PRIVATE_STASSID to private.h)
const char* password = PRIVATE_STAPSK;  /// The password for the WIFI network. (Add PRIVATE_STAPSK to private.h)

/*
 * Hostname used by dynamic DNS. e.g. ttc.local
 */
#ifndef HOST_NAME
#define HOST_NAME "ttc"
#endif

/*
 * LED control.
 */
#define LED_PIN LED_BUILTIN /// IO Pin used for LED
#define LED_OFF 1           /// Off state for LED
#define LED_ON 0            /// On state for LED

/*
 * Setup a server and a http engine.
 */
ESP8266WebServer server(80);  /// Web server class.Server starts on port provided.
Html html(&server);           /// HTML service started on server.

/**
 * handleRoot()
 */
void handleRoot() {
  digitalWrite(LED_PIN, LED_ON);  
  HtmlPage page = HtmlPage(
    HtmlTitle("The Tween Car"),
    HtmlHeader("The Tween Car","1")
  );
  html.send(page);
  digitalWrite(LED_PIN, LED_OFF);
}

/*
 * handle not found
 */
 void handleNotFound() {
  digitalWrite(LED_PIN, LED_ON);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_PIN, LED_OFF);
}

void setup() {
  int led_state = LED_ON;
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN,led_state);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, led_state); // Flash LED while connecting.
    led_state = !led_state;
  }

  digitalWrite(LED_PIN, LED_OFF);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(HOST_NAME)) {
    Serial.println("MDNS responder started");
  }

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });
    
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
    
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
}
