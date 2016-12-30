
void setupWeb() {
  server.on("/", handleRoot);
  server.on("/settemp", handleSettemp);
  server.on("/currenttemp", handleGetCurrent);
  server.onNotFound(handleNotFound);
}


void handleRoot() {
  String content = "<html><head>"
  "<script type=\"text/javascript\">"
  "function refresh() {"
	"var req = new XMLHttpRequest();"
	"console.log(\"getting current temp\");"
	"req.onreadystatechange=function() {"
			"if (req.readyState==4 && req.status==200) {"
				"document.getElementById('currentTemp').innerText = req.responseText;"
			"}"
	"}"
	"req.open(\"GET\", '/currenttemp', true);"
	"req.send(null);"
  "}"
 "function init() { "
	   "var int=self.setInterval(function(){refresh()},30000); "
 "}"
 "</script></head><body onload=\"init()\"><H2>Welcome to your ESP8266 Sous-Vide Cooker!</H2><br>";

  content += "<div style=\"display: inline-block\">Current Temperature: </div>"
  "&nbsp;<div style=\"display: inline-block\" id=\"currentTemp\">";
  content += String(currentTemp) + "</div><br>";
  content += "Target Temperature: " + String(targetTemp) + "<br>";
  content += "Relay status: ";
  content += (digitalRead(RELAY_OUT_PIN) == LOW) ? "ON" : "OFF";
  content += "<br>";
  content += "<form action=\"/settemp\">";
  content += "New target temperature: <br>";
  content += "<input type=\"text\" name=\"targetTemp\" value=\"\"><br>";
  content += " <input type=\"submit\" value=\"Submit\"></form>";


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
      String content = "new target temperature = " + String(t);
      server.send(200, "text/plain", content);
    } else {
      server.send(200, "text/plain", "invalid post value " + targettemp);
    }
  }
}

void handleGetCurrent() {
  server.send(200, "text/plain", String(currentTemp));
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
