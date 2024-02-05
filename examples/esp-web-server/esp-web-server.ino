#include <esp-basic-web-server.h>
#include <esp-basic-wifi.h>

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASS "your-wifi-password"

BasicWiFi wifi(WIFI_SSID, WIFI_PASS);
BasicWebServer webServer;

void setup() {
	Serial.begin(115200);
	Serial.println("");
	filesystem.setup(true);
	webServer.addHttpHandler("/reconnectWiFi", reconnectWiFiHandler);
	webServer.setup();
	wifi.onGotIP(handleWiFiGotIP);
	wifi.setup();
	if (wifi.waitForConnection() == BasicWiFi::wifi_got_ip) {
	}
	Serial.println("setup done!");
}

void loop() {
	delay(10);
}

void handleWiFiGotIP(GOT_IP_HANDLER_ARGS) {
	webServer.begin();
}
void reconnectWiFiHandler(AsyncWebServerRequest* request) {
	AsyncWebServerResponse* response = request->beginResponse(200, "text/plain", "reconnect WiFi command sent");
	request->send(response);
	wifi.reconnect();
}
