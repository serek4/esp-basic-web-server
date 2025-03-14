#include "esp-basic-web-server.hpp"

AsyncWebServer asyncWebServer(HTTP_PORT);

BasicWebServer::BasicWebServer(const char* user, const char* pass)
    : _user(user)
    , _pass(pass) {
}
BasicWebServer::BasicWebServer()
    : BasicWebServer::BasicWebServer(HTTP_USER, HTTP_PASS) {
}

void BasicWebServer::setup() {
	if (!(filesystem._fsStarted)) {
		BASIC_FS_PRINTLN("file system not mounted yet, mounting");
		filesystem.setup();
	}
	// add default headers
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	if (filesystem._fsStarted) {
		asyncWebServer.addHandler(new SPIFFSEditor(SPIFFS_EDITOR_ARGS));
		// redirect to file editor
		asyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
			request->redirect("/edit");
		});
	}
	// 404 not found handler
	asyncWebServer.onNotFound([](AsyncWebServerRequest* request) {
		String message = "File Not Found\n\n";
		message += "\nURI: ";
		message += request->url();
		message += "\nMethod: ";
		message += request->methodToString();
		message += "\nArguments: ";
		message += request->args();
		message += "\n";
		for (uint8_t i = 0; i < request->args(); i++) {
			message += " " + request->argName(i);
			message += ": " + request->arg(i) + "\n";
		}
		message += "\n";
		request->send(404, "text/plain", message);
	});
	for (auto& handler : _httpHandlers) {
		asyncWebServer.on(handler.uri, handler.method, handler.handler);
	}
}
void BasicWebServer::begin() {
	asyncWebServer.begin();
	BASIC_WEB_SERVER_PRINTLN("web server started");
}
void BasicWebServer::end() {
	asyncWebServer.end();
	BASIC_WEB_SERVER_PRINTLN("web server stopped");
}
void BasicWebServer::addHttpHandler(const char* uri, const ArRequestHandlerFunction& handler) {
	HttpHandler newHandler;
	newHandler.uri = uri;
	newHandler.method = HTTP_GET;
	newHandler.handler = handler;
	_httpHandlers.push_back(newHandler);
}
void BasicWebServer::addHttpHandler(const char* uri, WebRequestMethodComposite method, const ArRequestHandlerFunction& handler) {
	HttpHandler newHandler;
	newHandler.uri = uri;
	newHandler.method = method;
	newHandler.handler = handler;
	_httpHandlers.push_back(newHandler);
}
void BasicWebServer::setConfig(const Config& config) {
	_user = config.user;
	_pass = config.pass;
}
void BasicWebServer::getConfig(Config& config) {
	config.user = _user;
	config.pass = _pass;
}
BasicWebServer::Config BasicWebServer::getConfig() {
	Config config;
	config.user = _user;
	config.pass = _pass;
	return config;
}
