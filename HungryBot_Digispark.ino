#include <SoftRcPulseOut.h>
#include "DigiKeyboard.h"

#define PIN_LED         1
#define PIN_SERVOHAND   2
#define NOW             1
#define REFRESH_PERIOD_MS 20
#define FREE_ANGLE      100
#define THROUGH_ANGLE      0
SoftRcPulseOut ServoHand;


#define ECHOPIN 0
#define TRIGPIN 1
#define DISTANCE 60 // Distance value
#define COVER_THREADHOLD 8

unsigned int idle_count; // đếm số lần tĩnh lặng để tiết kiêm năng lượng

bool KeyboardMode;    // Chế độ bàn phím hay không
/*
 * Server di chuyen 1 goc nhat dinh
 * @param pos goc can di chuyen = 0 --> 180
 * @remark Bởi vì việc di chuyển cần thời gian, còn lâu hơn cả 1 chu kì điều khiển, nên phai lặp lại chu kì điều khiển trong nhiều lần mới bảo đảm.
 */
void ServoMove(int pos )
{
  int stall=14;
  for(; stall > 0; stall--)  {
    ServoHand.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(REFRESH_PERIOD_MS);        // waits 20ms for refresh period 
    SoftRcPulseOut::refresh(NOW);    // generates the servo pulse Now
  }
}

// the setup routine runs once when you press reset:
void setup() {                 
  // initialize the digital pin as an output 
  pinMode(PIN_LED, OUTPUT); //LED on Model A  or Pro

  KeyboardMode = false;

  // Báo hiệu bắt đầu
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_LED, LOW);
  delay(1000);
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_LED, LOW);


  if (GetDistance()< COVER_THREADHOLD){
    // Kích hoạt chế độ bàn phím. Áp dụng khi nối với máy tính
    // Nếu sử dụng dây nguồn từ adapter mà không có máy tính, thiết bị sẽ bị treo đấy.
    KeyboardMode = true;
  }
  
  ServoHand.attach(PIN_SERVOHAND);
  ServoMove(FREE_ANGLE);

  // Báo hiệu đã chỉnh servo
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_LED, LOW);

  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);    

  idle_count = 0;
}

int GetDistance()
{
  digitalWrite(TRIGPIN, LOW);                    // Set chan Trig xuong muc thap 2uS
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);                   //Gui mot xung 10uS muc cao vao chan Trig
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);                    // Gui lai muc thap vao chan Trig
  unsigned int  distance = pulseIn(ECHOPIN, HIGH); // Doc thoi gian tra ve. Hay dung kieu unsign. Da ghi nhan co so am tra ve roi
  return distance/58;                           // Tinh toan khoang cach
}

unsigned int dis;

// the loop routine runs over and over again forever:
void loop() {
  dis = GetDistance(); 
  // Gửi về bàn phím nếu ơ chế độ phù hợp
  if (KeyboardMode) {
    DigiKeyboard.sendKeyStroke(0);  
    DigiKeyboard.println(dis);
    DigiKeyboard.delay(200);
  }
  if (idle_count < 2000) {
    delay(100);
  } else {
    delay(1000);
  }
  if (dis < COVER_THREADHOLD) {
      idle_count = 0;
      digitalWrite(PIN_LED, HIGH);
      delay(2000);
      ServoMove(THROUGH_ANGLE);  
      delay(500);
      ServoMove(FREE_ANGLE);
      digitalWrite(PIN_LED, LOW);
      //Fix-bug: Neu su dung nguon Adpater (khong su dung dung data usb cable) thi khoang cach do duoc tu lan thu 2 tro luon < nguong, lam cho BOT lien tuc do rac.
      //Nghi ngo nguyen nhan: do khong gui thong tin qua DigiKeyboard, viec lay do khoang cach dien ra nhanh qua, khi ma dong co servo chua ve dung vi tri, 
      //Giai phap: them do tre 
      delay(1000);            
  } else {
      idle_count++;
  }
}
