
void setupWeb() {
  server.on("/", handleRoot);
  server.on("/settemp", handleSettemp);
  server.onNotFound(handleNotFound);
}


void handleRoot() {
  String content = "<html><body><H2>Welcome to your ESP8266 Sous-Vide Cooker!</H2><br>";

  content += "Current Temperature: " + String(currentTemp) + "<br>";
  content += "Target Temperature: " + String(targetTemp) + "<br>";
  content += "Relay status: ";
  content += (digitalRead(RELAY_OUT_PIN) == LOW) ? "ON" : "OFF";
  content += "<br>";

  content += "</body></html>";

  server.send(200, "text/html", content);
}

void handleSettemp() {
  String arg = "targetTemp";
  if (server.hasArg(arg)) {
    String targettemp = server.arg(arg);
    float t = targettemp.toFloat();
    if (t >= MIN_TARGET_TEMP && t <= MAX_TARGET_TEMP) {
      targetTemp = t;
    }
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
