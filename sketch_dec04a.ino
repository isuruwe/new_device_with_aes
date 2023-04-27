#include <SoftwareSerial.h>
#include <Wire.h>

#include <AES.h>
#include <Base64.h>
#define DEBUG true
byte key[] = 
{12,87,14,25,75,16,95,65,71,56,36,14,29,63,66,35
                                        ,88,92,91,72,89,87,36,19,58,63,67,41,23,37,47,55} ;

byte my_iv[] = 
{
  0x5, 0x6, 0x1, 0x4, 0x7, 0x3, 0x4, 0x9, 0x7, 0x5, 0x1, 0x0, 0x5, 0x2, 0x9, 0x1 ,
} ;
byte check [4*N_BLOCK] ;
SoftwareSerial mySerial(10,11);
String apn = "dialogbb";                       //APN
String apn_u = "";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "http://203.115.24.227:8888/detevts/postvts.ashx";  //URL for HTTP-POST-REQUEST
int Powerkey = 9;
String name1;   //String for the first Paramter (e.g. Sensor1)
String description;   //String for the second Paramter (e.g. Sensor2)

String altt;
String spd;
String dirr;
AES aes ;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(Powerkey, OUTPUT);  
  power();  
call();
  delay(1000);
}
void loop() {

    name1 = "123";
    description = "ABC";

   String altt="1";
String spd="2";
String dirr="3";
      gsm_sendhttp();
    delay(1000);
}
void gsm_sendhttp() {
  mySerial.println("AT");
  runsl();
  delay(5000);
 
 mySerial.println("AT+CMEE=2");
    runsl();
  mySerial.println("AT+CBAND=EGSM_PCS_BAND");
    runsl();
    delay(5000);
mySerial.println("AT+CPIN?");
  runsl();
  delay(100);
 
  mySerial.println("AT+CREG=1");
  runsl();
  delay(100);
  mySerial.println(" AT+CREG?");
  runsl();
  delay(100);
  
  mySerial.println("AT+CGATT?");
  runsl();
  delay(100);
  
   mySerial.println("AT+CSQ");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR =1,1");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=2,1");
  runsl();
  delay(2000);
  mySerial.println("AT+HTTPINIT");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
  delay(100);
  getgps();
sendData( "AT+CGNSINF",1000,DEBUG);   
String lat="";
String lon="";
String temp="";
String hum="";

//lat=enco(name1);
//lon=enco(description);
if(!name1.startsWith(",")){
  mySerial.println("AT+HTTPPARA=URL,"+url);
  //?p1=B1&tempi="+DHT.temperature+"&humi="+DHT.humidity+"&distan="+enco(name1)+"*"+enco(description)+"&alti="+altt+"&spd="+spd+"&dirr="+dirr);

  runsl();
   Serial.println("10");
  delay(100);
  
mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
 runsl();
   Serial.println("10");
    delay(100);
  mySerial.println("AT+HTTPDATA=300,10000");  
 runsl();
   delay(1000);
//mySerial.println("tempi="+temp);   

mySerial.println("distan="+enco(name1)+"*"+enco(description));
mySerial.println("&p1=A1");
mySerial.println("&alti="+altt);
mySerial.println("&spd="+spd);
mySerial.println("&dirr="+dirr);
mySerial.println("&humi="+String(hum));
mySerial.println("&tempi="+String(temp));
 runsl();
   Serial.println("10");
 delay(10000);
  mySerial.println("AT+HTTPACTION=1");
  runsl();
  delay(10000);
}
  mySerial.println("AT+HTTPREAD");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPTERM");
  runsl(); 
    Serial.println("16");
}
void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}
void call(void)
   {
         sendData( "AT+CSQ",1000,DEBUG);
         sendData("ATD10086;",1000,DEBUG);
         delay(10000);
         sendData( "ATH",1000,DEBUG);
   }
void getgps(void)
{
   sendData( "AT+CGNSPWR=1",1000,DEBUG); 
   sendData( "AT+CGNSSEQ=RMC",1000,DEBUG); 
}


String sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    mySerial.println(command); 
    long int time = millis();   
    while( (time+timeout) > millis())
    {
      while(mySerial.available())
      {       
        char c = mySerial.read(); 
        response+=c;
      }  
    }    
    if(debug)
    {

     response= response.substring(46); 
      
     name1=response.substring(0,8);
     description=response.substring(9,18);
      altt=response.substring(19,24);
      spd=response.substring(25,29);
      dirr=response.substring(30,35);
    
     if(altt.startsWith(",")){
 
  altt.remove(0, 1);
   Serial.println("a");
}
if(dirr.startsWith(",")){
  
  dirr.remove(0, 1);
  Serial.println("b");
}
if(spd.startsWith(",")){
  
  spd.remove(0, 1);
  Serial.println("c");
}

 
 
    
     Serial.println("lat");
      Serial.println(name1);
      Serial.println("lon");
      Serial.println(description);
      Serial.println("altt");
      Serial.println(altt);
      Serial.println("spd");
      Serial.println(spd);
      Serial.println("dirr");
      Serial.println(dirr);
    }    
    return response;
}
void power(void)
{
  digitalWrite(Powerkey, LOW); 
  delay(1000);               // wait for 1 second
  digitalWrite(Powerkey, HIGH);
}

String enco(String vb){
  byte succ = aes.set_key (key, 256) ;

  byte iv [N_BLOCK] ;
  for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;

  int decodedLen = 32;
  String s=vb;
  char der[32];
  s.toCharArray(der,32);
  byte* decoded =der;

  succ = aes.cbc_encrypt (decoded, check, (decodedLen/N_BLOCK), iv) ;

  aes.clean();
 
  int inputLen = sizeof(check);
  
  int encodedLen = base64_enc_len(inputLen);
  char encoded[encodedLen];
 
  base64_encode(encoded, check, inputLen); 
 Serial.println(encoded);
String weer="";
weer=encoded;
 weer.remove(44);
  return weer;
  }

