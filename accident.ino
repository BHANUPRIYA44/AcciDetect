#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "https://mitaccident.firebaseio.com/" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "CLZG9KKIbo1pfgfr8uT55WltnXSHkPeLKyPnNzPa"
#define WIFI_SSID "raspberrypi"                  //wifi name
#define WIFI_PASSWORD "12345678"              // wifi password
FirebaseData firebaseData;   //Define FirebaseESP32 data object


#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
int data;
int mq3_analogPin = A0;
const int buzzer =  D1;


String acc="0";

void setup() {
  
s.begin(115200);
Serial.begin(115200);
 Connect_Wifi();  //..connecting wifi
  pinMode(buzzer,OUTPUT);

      

}
 
void loop() {
  if (s.available()>0)
  {
    data=s.read();
   Serial.print("BPM:");

    Serial.println(data);
   Firebase.setInt(firebaseData,"user1/heartbeat",data);
  // get value   
  
    Serial.print("accident value: ");  
    acc= Firebase.getString(firebaseData,"user1/acc");                 
    acc=firebaseData.stringData();
    Serial.println(acc);  

  if (acc=="1")
  {
    digitalWrite(buzzer,HIGH);
       Serial.println("[info]    Accident Occured");  


  }
    if (acc=="0")
  {
    digitalWrite(buzzer,LOW);
    Serial.println("[info]    No Accident");  
  }
  delay(1000);
  int mq3_value = analogRead(mq3_analogPin);
  Serial.print("Alcohol:");  
  Serial.println(mq3_value);
  if (mq3_value<100)
  {
      Serial.println("HIGH Alcohol"); 
      digitalWrite(buzzer,HIGH);
      delay(1000);
       digitalWrite(buzzer,LOW);
      delay(1000);
      
    }
  else{
      Serial.println("NO Alcohol Detected"); 

     digitalWrite(buzzer,LOW);
    
  }
  
Firebase.setFloat(firebaseData, "user1/alcohol", mq3_value);
delay(2000);
  }
  else{
    Serial.println("Please Wear Heart Beat sensor");
    
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Connect wifi

void Connect_Wifi()                                              
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
