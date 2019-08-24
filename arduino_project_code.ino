#include<SoftwareSerial.h>
int blueTx=2;  
int blueRx=3;  
SoftwareSerial mySerial(blueTx, blueRx);

//--<변수 선언>--//
int a;          // 블루투스 리드 값을 받기 위한 선언
int b;          // 리드 값 받은걸 b에다 저장 하기 위함
int tmp = 18;   // 초기에 희망 온도를 설정
int Hum = 50;   // 초기에 희망 습도를 설정
//--------------//

//--------------<LCD>---------------//       
#include <Wire.h>                   //
#include <LiquidCrystal_I2C.h>      // LCD 라이브러리 사용 선언 
LiquidCrystal_I2C LCD(0x27,16,2);   // LCD(주소,16칸,2줄) 핀 설정
//------------------------------ ---//

//-------<온습도 센서>-------//
#include "DHT.h"            // 온,습도 라이브러리 사용 선언
int DHTPIN = 10;            // DHTPIN을 10번으로 지정
int DHTTYPE = DHT11;        // DHTTYPE을 DHT11으로 설정
DHT dht(DHTPIN, DHTTYPE);   // DHT를 설정 dht라는 이름으로 DHT타입과 핀을 설정해줌 
//--------------------------//

//------<모터>------//
#include <Servo.h> // Servo 라이브러리 사용 선언
Servo MoterR;      // Servo 타입의 변수로 (MoterR) 지정
Servo MoterL;      // Servo 타입의 변수로 (MoterL) 지정
//-----------------//

//-----<수위 센서>-----//
int RainSensor = A0;  // 정수형 변수 RainSeneor 에 아날로그 핀 A0을 설정 
//--------------------//


void setup()// 아두이노 실행 시 가장 먼저 한번만 실행 되는 부분(주로 초기 설정 하는 부분)
{
   Serial.begin(9600);
   
//<불루투스 시리얼 통신을 위한 설정>//
   mySerial.begin(9600);        //
//------------------------------//
  
   
//---------<모터>---------// 이름.attach(핀 번호) : 이름을 정해준 서브모터에 핀번호 설정
   MoterR.attach(9);     // MoterR 에 9번 핀 선언
   MoterL.attach(13);    // MoterL 에 13번 핀 선언
                         // 기본 값을 창문이 닫혀 있는 걸로 설정
   MoterR.write(160);    // MoterR 를 160도 로 회전
   MoterL.write(20);     // MoterL 를  20도 로 회전
//-----------------------//   
}

void loop()// setup함수가 실행된 후 계속 반복 되는 함수    
{


  int Light = analogRead(A2); // 조도 센서에서 읽은 값을 정수형 변수 Light에다가 저장

    
//---------------<온,습도 측정>---------------//
 int Temperature = dht.readTemperature();   // dht라는 이름으로 정해준 센서로 온도 값을 읽어서 정수형 변수 Temperature 에 저장 
 int Humidity = dht.readHumidity();         // dht라는 이름으로 정해준 센서로 습도 값을 읽어서 정수형 변수 Humidity 에 저장
//------------------------------------------//

//---------------<수위 센서 값 저장>---------------//
 int RainSensorData = analogRead(RainSensor);    // 아날로그 값인 RainSeneor 을 읽어서 정수형 변수 RainSeneorData 에 저장 
//-----------------------------------------------//

//--<수위 센서에 따른 모터(창문) 회전>--// 센서의 물이 묻이면 수치값이 100이상으로 나오는 걸 이용 ++(블루투스로 값 받아서 실행)

//----<블루투스에서 받아온 값 저장>----//
  if(mySerial.available())          // 블루투스에서 받아온 값이 있으면 if문 실행 
  {                                 // 
    int a = mySerial.read();        // 블루투스로 받아온 값을 a로 받아주고 
    b=a;                            // 그 값을 다시 b로 다시 저장 (if문 밖에서 사용하기 위함)
  }                                 //
                                    //
 if(b=='o')                         // 블루투스에서 받아온 값이 'o'면 if문 실행하여 창문 여닫는 전체 시스템을 ON시킴
 {                                  //
   if(RainSensorData < 50)          // RainSeneorData의 값이 50이면 if문에 속해 있는 문장 실행(비가 오지 않는 상황)++(센서에 물이 닿지 않으면 값이 엄청 낮은 값을 받음)
   {                                // 창문이 열림
     MoterR.write(90);              // MoterR 를 90도로 회전     
     MoterL.write(90);              // MoterL 를 90도로 회전     
   delay(15);                       //
   }                                //
    else if(RainSensorData > 50)    // RainSeneorData의 값이 100이상 이면 if문에 속해 있는 문장 실행(비가 오는 상황)++(센서에 물이 닿으면 값이 50이상을 유지함)
    {                               // 창문이 닫힘
     MoterR.write(160);             // MoterR 를 180도 로 회전
     MoterL.write(20);              // MoterL 를   0도 로 회전   
     delay(15);                     //
    }                               //
    b='o';                          // if문을 벗어나도 시스템상 ON이 유지될 수 있게 해줌 
                                    // 
 }                                  //
 else if(b=='x')                    // 블루투스로 읽어 온 값이 'x'면 전체 시스템을 OFF시키고 창문을 닫음
 {                                  // 창문이 닫힌 상태
    MoterR.write(160);              // MoterR 를 180도 로 회전
    MoterL.write(20);               // MoterL 를   0도 로 회전  
    delay(15);                      //
    b='x';                          // 마찬가지로 if문을 벗어나도 시스템이 계속 작동이 안되는 걸 유지하기 위해 설정 
 }                                  //
//----------------------------------// 
 
//----------<희망 온,습도 설정>----------//

//-----<희망 온도 올리기>-----// 
  if(b=='u')                // 블루투스로 입력하는 단어가 u이면 if문 실행
{                           //
  tmp++;  
    LCD.clear();// 기존에 설정 해둔 온도값 1도 올리기
  LCD.print("D.T : ");      // LCD화면에 "D.T : "화면 띄우기
  LCD.print(tmp);           // 키를 눌러 1도 올린 온도 표현
  LCD.print("'C");          // 단위 띄우기
  delay(400);               // 변경한 희망온도를 0.4초 동안 띄워주기
  LCD.clear();              // LCD화면 지우기
  b="";                     // b에 입힌 변수 지우기
}                           //
//--------------------------//

//-----<희망 온도 내리기>-----//
  if(b=='d')                // 블루투스로 입력하는 단어가 d이면 if문 실행
{                           //
  tmp--;                    //
    LCD.clear();            // 희망 온도 내리기 
  LCD.print("D.T : ");      //
  LCD.print(tmp);           //
    LCD.print("'C");        //
  delay(400);               //
  LCD.clear();              // 
  b="";                     //
}                           //
//--------------------------//

//-----<희망 습도 올리기>-----//
  if(b=='r')                //
{                           //
  Hum++;                    //
    LCD.clear();            //
  LCD.print("D.H : ");      //
  LCD.print(Hum);           //
  LCD.print("%");           //
  delay(400);               //
  LCD.clear();              //
  b="";                     //
}                           //
//--------------------------//

//-----<희망 습도 내리기>-----//
  if(b=='l')                //
{                           //
  Hum--;  
    LCD.clear();//
  LCD.print("D.H : ");      //
  LCD.print(Hum);           // 
  LCD.print("%");           //
  delay(400);               //
  LCD.clear();              // 
  b="";                     //
}                           //
//--------------------------//
 
//<희망 온도와 현재 온도에 따른 에어컨 ON/OFF(3색 LED표현)>//
   if((Temperature > tmp)&&(Humidity < Hum))          // 현재 온도가 희망 온도 보다 높으면 에어컨 ON
   {                                                  // 
      analogWrite(8,0);                              // 조건에 맞으면 파란 불빛
      analogWrite(7,255);                            //
      analogWrite(6,0);                              //
   }                                                 //
                                          //
//---------------------------------------------------//

//<희망 습도와 현재 습도에 따른 제습기 ON/OFF(3색 LED표현)>//
   if((Humidity > Hum)&&(Temperature < tmp))                               //
   {                                                 //
      analogWrite(8,0);                              //
      analogWrite(7,0);                              //
      analogWrite(6,255);                            //
   }                                                 //
//---------------------------------------------------//

 if((Temperature > tmp)&&(Humidity > Hum))
 {
      analogWrite(8,255);                              //
      analogWrite(7,0);                              //
      analogWrite(6,0);                            //
 }

   if((Humidity < Hum)&&(Temperature < tmp))                           //
   {                                                 //
      analogWrite(8,0);                              //
      analogWrite(7,0);                              //
      analogWrite(6,0);                              //
   }                                                 //

//-----<조도센서를 이용해서 실내등 키기>-----// 
  if(Light<800)                          //
  {                                      //
    analogWrite(12,155);                 //
    analogWrite(11,155);                 //
    analogWrite(5,155);                  //
  }                                      // 
  else if(Light>800)                     //
  {                                      //
    analogWrite(12,0);                   //
    analogWrite(11,0);                   //
    analogWrite(5,0);                    //
  }                                      //
//---------------------------------------//

  Serial.print(Temperature);
  Serial.print("도");            //온도 값
  Serial.print(Humidity);
  Serial.println("%");           //습도 값
  Serial.println(RainSensorData);
  Serial.println(Light);



//----------<LCD>-----------// 
  LCD.init();               // LCD 시작
  LCD.backlight();          // LCD 백그라운드 키기
  LCD.print("Temperature:");// LCD판에 Temperature: 글자를 찍어냄
  LCD.print(Temperature);   // 위에 글자 이어서 변수 Temperature 값을 이어서 찍어냄
  LCD.print("`C");          // 뒤에 마찬가지로 `C 를 찍어냄
  LCD.setCursor(0,1);       // LCD.setCursor(글자 위치,줄 위치) (A-1,B-1)로 생각하면 편함 ex)두 번째 줄 첫 번째 칸 A=1,B=1 (0,1)
  LCD.print("Humidity:");   // LCD판에 Humidity: 글자를 찍어냄
  LCD.print(Humidity);      // 위에 글자 이어서 변수 Humidity 값을 이어서 찍어냄
  LCD.print("%");           // 뒤에 마찬가지로 % 를 찍어냄
//---------------------------


}
