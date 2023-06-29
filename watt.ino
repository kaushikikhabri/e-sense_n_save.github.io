
#include <ESP8266WiFi.h>
#define ACTectionRange 20;    //set Non-invasive AC Current Sensor tection range (5A,10A,20A)

String apiKey = "WH08FLOCNKQ2OD6A";     //  Enter here your Write API key from ThingSpeak.com
const char *ssid =  "Anisha";     // enter your wifi name
const char *pass =  "anishajain";      // enter your wifi password
const char* server = "api.thingspeak.com";
int inp=0;
const int sensorIn = A0;
int mVperAmp = 100; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;

WiFiClient client;


// float resolution  = 3.3 / 1024;                   // Input Voltage Range is 1V to 3.3V
float t;
int flag=-1;  
 

void setup() {
  // put your setup code here, to run once:
       Serial.begin(9600);
       delay(10);
        pinMode(A0, INPUT);
       pinMode(inp,OUTPUT);
       digitalWrite(inp,LOW);
     
     
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

}

void loop() {
  // put your main code here, to run repeatedly:
  Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; // sq root
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 float Wattage = (220*AmpsRMS)-18; //Observed 18-20 Watt when no load was connected, so substracting offset value to get real consumption.
 if(Wattage<0)
 {
   Wattage=0;
  }
 Serial.print(AmpsRMS);
 Serial.println(" Amps RMS ");
 Serial.print(Wattage); 
 Serial.println(" Watt ");

 
  flag=flag+1;
  Serial.println(flag);
         
 
//  delay(1000);
//  if(flag==5)
//  {
//     digitalWrite(inp,HIGH);
//      delay(5000);
//    }

  if (isnan(Wattage))
                 {
                     Serial.println("Failed to read from Current sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(Wattage);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             Serial.println("%. Send to Thingspeak.");
 
                           
                        }
          client.stop();
 
          Serial.println("Waiting...");
 
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);

}
float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();

   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }

    }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5)/1024.0;
      
   return result;
 }
