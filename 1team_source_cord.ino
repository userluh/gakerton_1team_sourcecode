#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "KT_GiGA_2G_Wave2_7652"
#define WIFI_PASSWORD "bk2fef5307"
#define API_KEY "AIzaSyDiN7P3_IpQUH4-HzdV8G3zKVohq2Kx0B0"
#define DATABASE_URL "leeunho-app-default-rtdb.firebaseio.com" 
#define USER_EMAIL "djsgh1233357@naver.com"
#define USER_PASSWORD "qlqjs119@"

Servo servo1;

float sn[101];//출력할 sin함수의 값을 저장할 배열
float t=0.01;//0~2*pi를 나타내야 하므로 t를 0.01로 고정
unsigned long t_prev=0; //이전시간 
int i=0;//sin함수에 대입할 x값, 0~100; 
int gajungchi;
int k;
int flag1=1;
int flag2=0;
int flag3=0;
int flag4=0;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

void setup() {
  Serial.begin(2400);
  randomSeed(analogRead(0));
  servo1.attach(15);
  servo1.write(0);

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

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; 

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 , 1024 );
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
}

void loop(){
  unsigned long t_now=millis();

  if(flag1){ // 임의의 뇌파파형
    if(t_prev<= t_now && t_now <=t_prev+1300){
      gajungchi=random(0.5,15);
      k=random(30,51);
    }
    else{
      flag1=0;
      flag2=1;
      flag3=0;
      flag4=0;
      t_prev=t_now;
    }
  }

  if(flag2){ // 뇌파분석패턴1 에대한 행동1 동작 (팔을 올리기)
    if(t_prev<= t_now && t_now <=t_prev+300){
      gajungchi=45;
      k=random(30,51);
      servo1.write(60);
    }
    else{
      flag1=0;
      flag2=0;
      flag3=1;
      flag4=0;
      t_prev=t_now;
    }
  }

  if(flag3){ // 임의의 뇌파파형
    if(t_prev<= t_now && t_now <=t_prev+1300){
      gajungchi=random(0.5,15);
      k=random(30,51);
    }
    else{
      flag1=0;
      flag2=0;
      flag3=0;
      flag4=1;
      t_prev=t_now;
    }
  }
  
  if(flag4){ // 뇌파분석패턴2 에대한 행동2 동작 (팔을 내리기)
    if(t_prev<= t_now && t_now <=t_prev+300){
      gajungchi=57;
      k=random(30,51);
      servo1.write(0);
    }
    else{
      flag1=1;
      flag2=0;
      flag3=0;
      flag4=0;
      t_prev=t_now;
    }
  }

  sn[i]=gajungchi*sin(k*PI*i*t);//사인 함수값
  Serial.println(sn[i]);
  Firebase.setFloat(fbdo,F("/test/Float"),sn[i]);


  i++; //x 값을 증가시킴 
  if(i==101){
     i=0;
  }
}

