#pragma once

#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <esp-basic-fs.hpp>

// #define BASIC_WEB_SERVER_DEBUG
// debug printing macros
// clang-format off
#ifdef BASIC_WEB_SERVER_DEBUG
#define DEBUG_PRINTER Serial
#define BASIC_WEB_SERVER_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define BASIC_WEB_SERVER_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#define BASIC_WEB_SERVER_PRINTF(...) { DEBUG_PRINTER.printf(__VA_ARGS__); }
#else
#define BASIC_WEB_SERVER_PRINT(...) {}
#define BASIC_WEB_SERVER_PRINTLN(...) {}
#define BASIC_WEB_SERVER_PRINTF(...) {}
#endif
// clang-format on

#ifdef ARDUINO_ARCH_ESP32
#define SPIFFS_EDITOR_ARGS FILE_SYSTEM, _user, _pass
#elif defined(ARDUINO_ARCH_ESP8266)
#define SPIFFS_EDITOR_ARGS _user, _pass, FILE_SYSTEM
#endif

#define HTTP_PORT 80
#define HTTP_USER "admin"
#define HTTP_PASS "admin"


class BasicWebServer {
  public:
	struct HttpHandler {
		const char* uri;
		WebRequestMethodComposite method;
		ArRequestHandlerFunction handler;
	};
	struct Config {
		String user;
		String pass;
	};

	BasicWebServer();
	BasicWebServer(const char* user, const char* pass);

	void addHttpHandler(const char* uri, const ArRequestHandlerFunction& handler);
	void addHttpHandler(const char* uri, WebRequestMethodComposite method, const ArRequestHandlerFunction& handler);
	void setup();
	void begin();
	void setConfig(const Config& config);
	void getConfig(Config& config);
	Config getConfig();

  private:
	String _user;
	String _pass;
	std::vector<HttpHandler> _httpHandlers;
};

extern AsyncWebServer asyncWebServer;
