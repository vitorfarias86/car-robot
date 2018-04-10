#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <aREST.h>

#define LISTEN_PORT 80
#define OLED_RESET 4


const char* ssid = "Vitor Farias";
const char* password = "naointeressa";

Adafruit_SSD1306 display(OLED_RESET);
aREST rest = aREST();
WiFiServer server(LISTEN_PORT);

int leftMotorForward = 14; /* GPIO14(D5)  */
int leftMotorBackward = 12;  /* GPIO12(D6)  */
int rightMotorForward = 13; /* GPIO12(D7)  */
int rightMotorBackward = 15; /* GPIO12(D8)  */


// Declare functions to be exposed to the API
int MoveCar(String data);

void setup() {

  
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);
  
  
  rest.function("move-car",MoveCar);

  
  Serial.begin(115200);
  connectWiFi();
  configureDisplay();

  
}

void loop() {


    // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  
}

int MoveCar(String data){

    Serial.println("Move Car " + data);

    if (data == "forward") MotorForward();

    else if (data == "backward") MotorBackward();

    else if (data == "left") TurnLeft();

    else if (data == "right") TurnRight();

    else if (data == "stop") MotorStop();

   return 1;
}

void MotorForward(void)   {
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

void MotorBackward(void)   {
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,LOW);
}


void TurnLeft(void)   {

  analogWrite(leftMotorForward, 767);
  analogWrite(rightMotorForward, 1023);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,LOW);  
}


void TurnRight(void)   {

  
  analogWrite(leftMotorForward, 1023);
  analogWrite(rightMotorForward, 767);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,LOW);
}


void MotorStop(void)   {
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

void connectWiFi(){
  char myIpString[24];
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  IPAddress myIp = WiFi.localIP();
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.println((myIp));
  sprintf(myIpString, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  showMessage(ssid, myIpString);

    // Start the server
  server.begin();
  Serial.println("Server started");

  
}
void configureDisplay(){
  Serial.println("Setting up display");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.setTextColor(WHITE);
  display.clearDisplay();
  Serial.println("Success..");

}

void showMessage(const char* ssid, const char* ip){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Connected to ");
  display.println(ssid);
  display.setCursor(0,16);
  display.print("IP address: ");
  display.println(ip);
  display.display();
  delay(2000); 
}

