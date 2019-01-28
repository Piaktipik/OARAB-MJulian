
// Adpatado de: https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/
// librerias
#include <ESP8266WiFi.h>

const char* ssid = "OARAB";
const char* password = "515tema*.";

// datos Conexion RED
//const char* ssid = "OARAB-HOME";
//const char* password = "Obrancoenovo18";

//const char* ssid = "WIFI-ITM";
//const char* password = "";

// puerto escucha por defecto (8080)
WiFiServer server(80);

void setup() {
  // puerto serial
  Serial.begin(115200);
  delay(10);

  // se inician los puertos apagados
  //pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  digitalWrite(2, HIGH); // se inicia apagado
  //digitalWrite(0, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  if (request.indexOf("/light1on") > 0)  {
    digitalWrite(2, LOW);
  }
  if (request.indexOf("/light1off") > 0)  {
    digitalWrite(2, HIGH);
  }

  if (request.indexOf("/light2on") > 0)  {
    //digitalWrite(2, LOW);
  }
  if (request.indexOf("/light2off") > 0)  {
    //digitalWrite(2, HIGH);
  }

  
  /*if (request.indexOf("/light3on") > 0)  {
    digitalWrite(0, HIGH);
  }
  if (request.indexOf("/light3off") > 0)  {
    digitalWrite(0, LOW);
  }
  
  if (request.indexOf("/light4on") > 0)  {
    digitalWrite(13, HIGH);
  }
  if (request.indexOf("/light4off") > 0)  {
    digitalWrite(13, LOW);
  }
  */
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("</head>");
  client.println("<body bgcolor = \"#aaaaaa\">");
  client.println("<hr/><hr>");
  client.println("<h4><center> OARAB UDEA (Control ON/OFF Camera & Telescope) </center></h4>");
  client.println("<hr/><hr>");
  client.println("<br><br>");
  
  client.println("<center>");
  client.println(" Telescope ");
  client.println("<a href=\"/light1on\"\"><button> On </button></a>");
  client.println("<a href=\"/light1off\"\"><button> Off </button></a><br />");
  
  client.println("</center>");
  client.println("<br><br>");
  
  client.println("<center>");
  client.println(" Camera ");
  client.println("<a href=\"/light2on\"\"><button> On </button></a>");
  client.println("<a href=\"/light2off\"\"><button> Off </button></a><br />");
 
  client.println("</center>");
  client.println("<br><br>");
  
  /*client.println("<center>");
  client.println("Device 3");
  client.println("<a href=\"/light3on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/light3off\"\"><button>Turn Off </button></a><br />");
  
  client.println("</center>");
  client.println("<br><br>");
  
  client.println("<center>");
  client.println("Device 4");
  client.println("<a href=\"/light4on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/light4off\"\"><button>Turn Off </button></a><br />");
  
  client.println("</center>");
  client.println("<br><br>");
  */
  
  client.println("<center>");
  client.println("<table border=\"5\">");
  client.println("<tr>");
  
  if (digitalRead(2)){
    client.print("<td> Telescope is OFF </td>");
  }else{
    client.print("<td> Telescope is ON </td>");
  }
  client.println("<br />");
  
  if (digitalRead(2)){
    client.print("<td> Camera is OFF </td>");
  }else{
    client.print("<td> Camera is ON </td>");
  }
  client.println("</tr>");
  
  client.println("<tr>");

  /*if (digitalRead(0)){
    client.print("<td>Light 3 is ON</td>");
  }else{
    client.print("<td>Light 3 is OFF</td>");
  }

  if (digitalRead(13)){
    client.print("<td>Light 4 is ON</td>");
  }else{
    client.print("<td>Light 4 is OFF</td>");
  }
  client.println("</tr>");
  */

  client.println("</table>");
  client.println("</center>");
  client.println("</html>");
  delay(1);

  Serial.println("Client disonnected");
  Serial.println("");

}
