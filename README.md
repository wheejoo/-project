#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

char auth[] = "sQiF_9zJhEA1m209F2fkonJeiSVM_Yn3"; // nodeMCU3>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AndroidAPchy";
char pass[] = "chan0812";

SimpleTimer timer;

int echoPin1 = 14; //D5, 초음파 센서의 핀번호 설정
int trigPin1 = 12; //D6, 초음파 센서의 핀번호 설정

int echoPin2 = 13; //D7, 초음파 센서의 핀번호 설정
int trigPin2 = 15; //D8, 초음파 센서의 핀번호 설정

int buzzer = 0;    //D3, 부저
int LEDR = 5;      //D1
int LEDG = 4;       //D2

int num = 0;      //사람수 카운트
unsigned long rec_sen1 = 0; //센서 활성화 시간 기록
unsigned long rec_sen2 = 0;

int jud1 = 0; // 순서 판단 용
int jud2 = 0; 

int limitnum = 2; //방안에는 이 이상 넘을 수 없다. 

int who = 15; // Threshold

//######## reset 함수 ###########
void Reset(){
jud1 = 0;
jud2 = 0;
rec_sen1 =0;
rec_sen2 =0;
digitalWrite(LEDG, LOW);
Serial.println("Reset!");

}

//########## Blynk #######
  // send num
  BLYNK_READ(V4)
  {
    Blynk.virtualWrite(V4 , num);
  }

  // receive liminum
  BLYNK_CONNECTED(){
    Blynk.syncVirtual(V3);
  }
  BLYNK_WRITE(V3)
  {
    limitnum = param.asInt();
    Serial.print("Limitnum : ");
    Serial.println(limitnum);
  }
  // read limitnum
  BLYNK_READ(V3)
  {
    Blynk.virtualWrite(V3 , limitnum);
  }
  BLYNK_WRITE(V5)
  { 
    int buttonstate =  param.asInt();
    if(buttonstate == HIGH){
      digitalWrite(LEDR, LOW);
      noTone(buzzer);
      Serial.println("The situation is under control");
      num=0;
    }
  }

  
//###############################################
//################ setup ########################
void setup() {
  // put your setup code here, to run once:
pinMode(trigPin1, OUTPUT); // trig를 출력모드로 설정
pinMode(echoPin1, INPUT); //echo를 입력모드로 설정
pinMode(trigPin2, OUTPUT); // trig를 출력모드로 설정
pinMode(echoPin2, INPUT); //echo를 입력모드로 설정

pinMode(LEDR, OUTPUT);
pinMode(LEDG, OUTPUT);

//timer.setInterval(1000L, nowmyroom);

Serial.begin(9600);
Blynk.begin(auth, ssid, pass);


}
//###################################
//########### loop 문 ###############
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentmillis = millis();
  timer.run();
  Blynk.run();
  float duration1, distance1; //초음파센서의 값
  float duration2, distance2; //초음파센서의 값

   //문과 사람의 거리


  
  //########## 초음파 센서 1 ################
  digitalWrite(trigPin1, HIGH);// 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  delay(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH); // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance1 = ((float)(340 * duration1) / 10000) / 2;  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  //Serial.print((int)distance1);
  //Serial.println("cm");// 수정한 값을 출력

  //########### 초음파 센서 2 ###############
  digitalWrite(trigPin2, HIGH);// 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  delay(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH); // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance2 = ((float)(340 * duration2) / 10000) / 2;  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  //Serial.print((int)distance2);
  //Serial.println("cm");// 수정한 값을 출력


  //########### 센서 활성화 ##########  
  if((int)distance1 < who){
     rec_sen1 = currentmillis;
     Serial.println("sen1 activated!");
     jud1 = 1; // sen1 이 마지막
  }
  
  //Serial.println(currentmillis);
  //Serial.println(rec_sen1);

  // millis를 통해 일정 시간이 지나면 꺼짐. 
  if(currentmillis - rec_sen1 >= 3000 && currentmillis - rec_sen1 < 3600){
    Reset();
  }
 
  if((int)distance2 < who){  
      rec_sen2 = currentmillis;
      Serial.println("sen2 activated!");
      jud2=1;  //sen2 이 마지막  
  }
  if(currentmillis - rec_sen2 >= 3000 && currentmillis - rec_sen2 < 3600){
    Reset();
  }

  //unsigned long gap = rec_sen2 - rec_sen1;      //gap>0 이면, sen1이 먼저 활성화 되었다.
  //temp1>temp2 이면 sensor1이 먼저 활성화 되었음.
  
Serial.print("jud1 : ");
Serial.print(jud1);
Serial.print("     jud2 : ");
Serial.println(jud2);

//Serial.println(rec_sen1);
//Serial.println(rec_sen2);

long gap = rec_sen2-rec_sen1; 
  
  //################ 사람 수 판단 ##################
  if((jud1 == 1 && jud2 == 1) && (gap)< -300){
    num--;   
    Reset();                        // 지나가는 도중 중복 카운트를 피하기 위해
    digitalWrite(LEDG, HIGH);
    tone(buzzer, 800, 500);
    timer.setTimeout(300, Reset);  // 사람이 지나 간 후 jud 변수 완벽한 초기화를 위해
    Serial.println("the main comming out");
  }

  if((jud1 == 1 && jud2 == 1) && (gap)> 300){
    num++;
    Reset();
    digitalWrite(LEDG, HIGH);
    tone(buzzer, 800, 500);
    timer.setTimeout(300, Reset);
    Serial.println("the main comming in");
  }
  
  Serial.print("num : ");
  Serial.println(num);


  //############## 판단조건 ###############
  if(num>limitnum){
      
      Blynk.notify("Something is happend in my rooms!");
      Serial.println("Something is Happen!");
      digitalWrite(LEDR, HIGH);
      tone(buzzer, 800, 5000);
      
      
  }


  

  delay(10);
}

////

#include <SoftwareSerial.h>

SoftwareSerial btSerial(2,3); // Rx, Tx for arduino, Tx, Rx for BT
int LED1=13;
int temp=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
btSerial.begin(9600);
pinMode(LED1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

if(btSerial.available()){
  temp = btSerial.read();
  btSerial.write();
  if(temp=='1'){
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
  }
  if(temp=='2'){
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
  }
}
}
