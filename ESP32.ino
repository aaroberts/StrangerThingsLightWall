include <WiFi.h> //include the Wifi Library
include <ESPTelnet.h> //include the ESPTelnet Library
include <WebServer.h> //include the WebServer library
define RX1 3 //RX1 //Receive PIN on the ESP32 - The TX PIN from the Arduino goes to this PIN.
define TX1 1 //TX1 //Receive PIN on the ESP32 - The RX PIN from the Arduino goes to this PIN.
// Replace with your network credentials
const char* ssid = "XXXXXX"; //Enter your SSID here
const char* password = "XXXXXX"; //Enter your password here

ESPTelnet telnet;
WebServer server(80); // HTTP server runs on port 80
IPAddress ip;

// HTML page stored in PROGMEM to send serial messsages to the Arduino and control the lights.
const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Serial Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
body {
background-color: #121212;
color: #FFFFFF;
font-family: Arial, sans-serif;
text-align: center;
margin: 0;
padding: 10px; /* Reduced padding /
}
h1 {
color: #FF0000;
margin-bottom: 20px; / Adjusted spacing /
}
.button-grid {
display: flex;
flex-wrap: wrap;
justify-content: center;
margin: 0 auto;
}
.button-grid button {
background-color: #1F1B24;
color: #FFFFFF;
border: none;
padding: 15px;
margin: 5px;
font-size: 16px;
cursor: pointer;
border-radius: 5px;
flex: 1 0 30%;
box-sizing: border-box;
}
.button-grid button:hover {
background-color: #3700B3;
}
form {
margin-top: 20px;
}
input[type="text"] {
padding: 10px;
font-size: 16px;
width: calc(100% - 20px); / Adjusted width /
max-width: 380px; / Slightly less than iPhone 14 width /
background-color: #48474a;
color: #FFFFFF;
border: 1px solid #606061;
border-radius: 10px;
box-sizing: border-box;
}
input[type="submit"] {
background-color: #8c9191;
color: #000000;
border: none;
padding: 10px 20px;
margin-top: 10px;
font-size: 16px;
cursor: pointer;
border-radius: 5px;
}
input[type="submit"]:hover {
background-color: #018786;
}
/ Media Query for iPhone 14 /
@media only screen and (max-width: 390px) {
.button-grid button {
flex: 1 0 31%; / Adjust to fit three buttons per row */
margin: 3px;
padding: 10px;
font-size: 14px;
}
input[type="text"] {
max-width: 100%;
}
}
</style>
</head>
<body>
<h1>Stranger Messages</h1>
<form onsubmit="sendCustomMessage(); return false;">
<input type="text" id="customMessage" placeholder="Enter custom message">
<input type="submit" value="Send">
</form>
<div class="button-grid">
<button onclick="sendPredefinedMessage('2')">flash</button>
<button onclick="sendPredefinedMessage('3')">big flash</button>
<button onclick="sendPredefinedMessage('232, , 2 3 , .')">flash show</button>
<button onclick="sendPredefinedMessage(',')">all on</button>
<button onclick="sendPredefinedMessage('.')">all off</button>
<button onclick="sendPredefinedMessage('happy halloween')">happy halloween</button>
<button onclick="sendPredefinedMessage('candy here')">candy here</button>
<button onclick="sendPredefinedMessage('trick or treat')">trick or treat</button>
<button onclick="sendPredefinedMessage('nice costume')">nice costume</button>
<button onclick="sendPredefinedMessage('boo')">boo</button>
<button onclick="sendPredefinedMessage('have you seen eleven')">have you seen eleven</button>
<button onclick="sendPredefinedMessage('hello')">hello</button>
<button onclick="sendPredefinedMessage('im here')">im here</button>
<!--
<button onclick="sendPredefinedMessage('im in a strange place')">im in a strange place</button>
<button onclick="sendPredefinedMessage('im cold and damp')">im cold and damp</button>
<button onclick="sendPredefinedMessage('something evil here')">something evil here</button>
-->
<button onclick="sendPredefinedMessage('run')">run</button>
<button onclick="sendPredefinedMessage('hide')">hide</button>
<button onclick="sendPredefinedMessage('its coming')">its coming</button>
<button onclick="sendPredefinedMessage('yes')">yes</button>
<button onclick="sendPredefinedMessage('no')">no</button>
<button onclick="sendPredefinedMessage('1')">test</button>
<!--
<button onclick="sendPredefinedMessage('hgfedcba')">hgfedcba</button>
<button onclick="sendPredefinedMessage('ijklmnopq')">ijklmnopq</button>
<button onclick="sendPredefinedMessage('zyxwvutsr')">zyxwvutsr</button>
-->
</div>
<br><br>

<script>
function sendPredefinedMessage(message) {
fetch('/send?message=' + encodeURIComponent(message));
}
function sendCustomMessage() {
var message = document.getElementById('customMessage').value;
fetch('/send?message=' + encodeURIComponent(message));
}
</script>
</body>
</html>
)====";

// Function prototypes
void handleRoot();
void handleSend();
void onTelnetConnect(String ip);
void onTelnetDisconnect(String ip);
void onTelnetReconnect(String ip);
void onTelnetConnectionAttempt(String ip);
void onTelnetInput(String input);

void setup() {
// Initialize Serial for debugging (optional)
Serial.begin(115200);
delay(1000);
Serial.println("\nStarting ESP32 Telnet to Serial Bridge...");

// Initialize Serial1 for communication with the other device
// You can choose Serial1 or Serial2 and assign custom TX/RX pins if needed
Serial1.begin(2400, SERIAL_8N1, RX1, TX1); // Adjust baud rate and pins as necessary

// Connect to Wi-Fi network
WiFi.begin(ssid, password);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nConnected to Wi-Fi");

//init and get the time
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
printLocalTime();

// Start Telnet server
telnet.begin();
Serial.print("Telnet server started at ");
Serial.println(WiFi.localIP());

// Set Telnet callback functions
telnet.onConnect(onTelnetConnect);
telnet.onDisconnect(onTelnetDisconnect);
telnet.onReconnect(onTelnetReconnect);
telnet.onConnectionAttempt(onTelnetConnectionAttempt);

// Set the callback function for incoming Telnet data
telnet.onInputReceived(onTelnetInput);

// Start HTTP server
server.on("/", handleRoot);
server.on("/send", handleSend);
server.begin();
Serial.println("HTTP server started");
}

void loop() {
// Handle Telnet client connections
telnet.loop();

// Handle HTTP client requests
server.handleClient();

// Check for data from serial device and send to Telnet client
while (Serial1.available()) {
char serialData = Serial1.read();
telnet.write(serialData); // Send data to Telnet
telnet.println(serialData);
Serial.print("Serial1 to Telnet: ");
Serial.println(serialData);
}
}

// Handle root URL
void handleRoot() {
server.send_P(200, "text/html", MAIN_page);
}

// Handle send endpoint
void handleSend() {
if (server.hasArg("message")) {
String message = server.arg("message");
Serial1.print(message); // Send to serial device
Serial1.flush();
Serial.println("HTTP to Serial1: " + message);
server.send(200, "text/plain", "Message sent: " + message);
} else {
server.send(400, "text/plain", "Bad Request");
}
}

// Callback functions
void onTelnetConnect(String ip) {
Serial.println("Telnet Client connected: " + ip);
telnet.println("Welcome! Type your commands.");
}

void onTelnetDisconnect(String ip) {
Serial.println("Telnet Client disconnected: " + ip);
}

void onTelnetReconnect(String ip) {
Serial.println("Telnet Client reconnected: " + ip);
}

void onTelnetConnectionAttempt(String ip) {
Serial.println("Telnet Connection attempt from: " + ip);
}

void onTelnetInput(String input) {
// This function is called whenever input is received from the Telnet client
// Send the input to the serial device
telnet.println("Sending " + input);
Serial1.print(input);
Serial1.flush(); // Ensure all data is sent
Serial.println(input);
}
