#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <EMailSender.h>

int Dblock = 26;
int UPSPin = 2;

int flag = 0;

const char* host = "192.168.79.126";
const int port = 5000;

char ssid[100]       = "Horrizon's";
char password[100]   = "1234567890";
char myMailAdr[100]  = "20bec131@nirmauni.ac.in";
char myMailPass[100] = "etjejmynawfmamuc";
char toMailAdr[100]  = "20bec130@nirmauni.ac.in";

EMailSender emailSend(myMailAdr, myMailPass);

void send_mail(String msg) {
  EMailSender::EMailMessage message;
  message.subject = "Special Assign";
  message.message = msg;
  EMailSender::Response resp = emailSend.send(toMailAdr, message);
}

WiFiClient client;
IPAddress serverIP;

void setup() {
  Serial.begin(9600);
  
  M5.begin();
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.printf("WIFI START:%s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    M5.Lcd.print(".");
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  M5.Lcd.println(" - READY");
  Serial.println("Connected to WiFi");
  
  pinMode(UPSPin, INPUT);
  pinMode(Dblock, INPUT);

//  digitalWrite(UPSPin, LOW);
//  digitalWrite(Dblock, LOW);

  // Resolve IP address of Python GUI server
  if (!WiFi.hostByName(host, serverIP)) {
    Serial.println("Failed to resolve server IP address");
    while (1);
  }
  Serial.print("Server IP address: ");
  Serial.println(serverIP);
}

void loop() {

  M5.update();
  if (digitalRead(UPSPin) == 1 && flag == 0 && digitalRead(Dblock)==0)
  { 
    // Connect to Python GUI server
    if (!client.connected())
    {
      Serial.println("Connecting to server...");
      if (client.connect(serverIP, port)) {
        Serial.println("Connected to server");
        client.println("UPS On");
        client.flush();
      }
      else
      {
        Serial.println("Connection failed");
      }
    }
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.setTextColor(GREEN); // color
    M5.Lcd.drawString("UPS ON", 160, 80, 2);
    M5.Lcd.setTextColor(RED); // color
    M5.Lcd.drawString("DBlock OFF", 160, 120, 2);
    M5.Lcd.setTextColor(WHITE); // color
    M5.Lcd.drawString("Sending Mail", 160, 160, 2);
    
    String msg = "UPS ON ";
    send_mail(msg);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.setTextColor(GREEN); // color
    M5.Lcd.drawString("UPS ON", 160, 80, 2);
    M5.Lcd.setTextColor(RED); // color
    M5.Lcd.drawString("DBlock OFF", 160, 120, 2);
    M5.Lcd.setTextColor(WHITE); // color
    M5.Lcd.drawString("Email Sent", 160, 160, 2);
    
    flag = 1;
  }
  
  else if (digitalRead(UPSPin) == 0 && digitalRead(Dblock) == 1)
  {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.setTextColor(RED); // color
    M5.Lcd.drawString("UPS OFF", 160, 80, 2);
    M5.Lcd.setTextColor(GREEN); // color
    M5.Lcd.drawString("DBlock ON", 160, 120, 2);
    M5.Lcd.setTextColor(WHITE); // color
    M5.Lcd.drawString("Power ON", 160, 160, 2);
    
    if (!client.connected())
    {
      Serial.println("Connecting to server...");
      if (client.connect(serverIP, port)) {
        Serial.println("Connected to server");
        client.println("Power On");
        client.flush();
      }
      else
      {
        Serial.println("Connection failed");
      }
    }
    
    flag = 0;
  }

  else if (digitalRead(UPSPin) == 0 && digitalRead(Dblock) == 0)
  {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.setTextColor(RED); // color
    M5.Lcd.drawString("UPS OFF", 160, 80, 2);
    M5.Lcd.setTextColor(RED); // color
    M5.Lcd.drawString("DBlock OFF", 160, 120, 2);
    M5.Lcd.setTextColor(WHITE); // color
    M5.Lcd.drawString("Total Failure", 160, 160, 2);
    
    if (!client.connected())
    {
      Serial.println("Connecting to server...");
      if (client.connect(serverIP, port)) {
        Serial.println("Connected to server");
        client.println("Total Failure");
        client.flush();
      }
      else
      {
        Serial.println("Connection failed");
      }
    }
    
    flag = 0;
  }
  else if (digitalRead(UPSPin) == 1 && digitalRead(Dblock)==0)
  { 
    // Connect to Python GUI server
    if (!client.connected())
    {
      Serial.println("Connecting to server...");
      if (client.connect(serverIP, port)) {
        Serial.println("Connected to server");
        client.println("UPS On");
        client.flush();
      }
      else
      {
        Serial.println("Connection failed");
      }
    }
  }
  delay(50);
}
