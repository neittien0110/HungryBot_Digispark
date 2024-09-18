//-------------------------------------------------------------------------------
// For Arduino NodeMCU ESP8266 12 / ESP8266-12E / ESP-01(S) / ATTiny85
//
// Pre-compile: Select the corect Arduino Type
//    1. Open Arduino IDE and go to Files menutiem. Click on the Preference in the Arduino IDE.
//    2. In Additional Manager URLS section, type this URL and click OK.
//     http://arduino.esp8266.com/stable/package_esp8266com_index.json
//    3. Back to the main IDE
//    4. Click Tools / select Board Manager... Search and install ESP8266 with version 2.7.4 (must not 3.x.x)
//    5. Click Tools / select Board:. Choice board "ESP8266" or NodeMCU 0.9 (ESP-12 Module) or NodeMCU 1.0 (ESP-12E Module)
//-------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// CÀI ĐẶT TRÌNH BIÊN DỊCH CHO DEV KIT
//----------------------------------------------------------------------------------
//  Nếu device là DIGISPARK_KICKSTARTER_PRO hoặc ATTiny85, thì:
//  1. Trong menu File, chọn Preferences. Ở mục Additional Boards Magagers Urls, dán thêm nội dung:  http://digistump.com/package_digistump_index.json
//  2. Trong menu Tools, chọn Boards.., chọn Board Managers. Tìm kiếm và cài đặt board Digistump AVR Boards
//  3. Trong menu Tools, ở phần Boards.., chọn board có tên là Digispark Pro (Default 16.5Mhz)
//  4. Trước khi nạp code thì phải rút device khỏi máy tính.
//     Trên IDE, bấm nút Upload code
//     Khi nào trong Console của IDE hiện ra dòng chữ Please..... thì mới cắm device vào. Đợi một lúc là xong.
//----------------------------------------------------------------------------------
#define NAME "Hungybot"  

//----------------------------------------------------------------------------------
// Định nghia các loại DEVKIT hỗ trọ
//----------------------------------------------------------------------------------
// Sử dụng: uncomment chính xác loại thiết bị sử dụng

#define ATTiny85
//#define DIGISPARK_KICKSTARTER_PRO        //Layout: https://user-images.githubusercontent.com/34286173/85187891-1062d480-b258-11ea-8e80-517ec6a48a9d.png
//#define ARDUINO_NODEMCU_12  //Layout: https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266-NodeMCU-kit-12-E-pinout-gpio-pin.png?quality=100&strip=all&ssl=1
//#define ARDUINO_WEMOS_8266_D1_MINI
//#define ARDUINO_ESP_01S      //In ArduinoIDE: Board=Generic ESP8266 Module


//Định nghĩa lại góc quay của servo, cho thư viện Servo.h
#define MIN_PULSE_WIDTH       544     // Đổ rộng xung tương ứng với góc quay tổi thiểu của SG90s
#define MAX_PULSE_WIDTH      2400     // Đổ rộng xung tương ứng với góc quay tổi đa của SG90s

//------------------------------------------------------------------------------------------
// PORTING FIRWARE: khai báo các chân tín hiệu điều khiển của mainboard cho phù hợp với
// phần mềm, OS
//------------------------------------------------------------------------------------------
#ifdef DIGISPARK_KICKSTARTER_PRO
  #define ECHOPIN 0
  #define TRIGPIN 1
  #define PIN_SERVOHAND 2
  #define PIN_LED 1             // Đo khoảng cách và đèn hiệu sẽ chắc chắn không sử dụng cùng lúc. Hiện tượng đèn nháy báo hiệu quá trình đo khoảng cách đang thực hiện
  #define KEYBOARD_WEDGE        // Giao tiếp thiết bị-máy tính dạng console: Bàn phím ảo chỉ có tác dụng với các board có USB trực tiếp (không qua CH340..) như là DigiSpark Pro, Digispark Kickstarter Pro, Arduino Due, Ardruino Zero
  #include <SoftRcPulseOut.h>
  #include "DigiKeyboard.h"
  SoftRcPulseOut ServoHand;
#endif

#ifdef ATTiny85
  #define ECHOPIN 0
  #define TRIGPIN 1
  #define PIN_SERVOHAND 3
  #define PIN_LED 1             // Đo khoảng cách và đèn hiệu sẽ chắc chắn không sử dụng cùng lúc. Hiện tượng đèn nháy báo hiệu quá trình đo khoảng cách đang thực hiện
  #define KEYBOARD_WEDGE        // Giao tiếp thiết bị-máy tính dạng console: Bàn phím ảo chỉ có tác dụng với các board có USB trực tiếp (không qua CH340..) như là DigiSpark Pro, Digispark Kickstarter Pro, Arduino Due, Ardruino Zero
  #include <SoftRcPulseOut.h>
  #include "DigiKeyboard.h"
  SoftRcPulseOut ServoHand;
  #define LED_BUILDIN 1
  #define PIN_CHARGER_KEY 2   // Đánh thức Phím Key của Bộ sạc  
#endif

#ifdef ARDUINO_NODEMCU_12
  #define ECHOPIN D6
  #define TRIGPIN D5
  #define PIN_SERVOHAND D4
  #define PIN_LED D7
  #define PIN_LIGHT_SENSOR A0
  #define SERIAL_COMMUNICATION   // Giao tiếp thiết bị-máy tính dạng console: Serial 9600
  #define SIOT_DATA              // Có gửi dữ liệu về server
  #define POST_THREADHOLD 100    // Chu kì truyền dữ liệu distance, lumen về server. Tầm 30 giây = 100
  #include <Servo.h>
  Servo ServoHand;
#endif


#ifdef ARDUINO_WEMOS_8266_D1_MINI
  #define ECHOPIN D3
  #define TRIGPIN D2
  #define PIN_SERVOHAND D4
  #define PIN_LED D7
  //#define PIN_LIGHT_SENSOR A0
  //#define SERIAL_COMMUNICATION   // Giao tiếp thiết bị-máy tính dạng console: Serial 9600
  //#define SIOT_DATA              // Có gửi dữ liệu về server
  #define POST_THREADHOLD 100    // Chu kì truyền dữ liệu distance, lumen về server. Tầm 30 giây = 100
  #include <Servo.h>
  Servo ServoHand;
#endif

#ifdef ARDUINO_ESP_01S
  //----------------------------------------------------------------------------
  //             <== 3.3V    RX       ==> Echo
  //                 RST     GPIO0    ==> button Key of Charger 
  //                 CHPD    GPIO2  ==> Trig
  //    Servo <== TX/GPIO1   GND    ==>
  //----------------------------------------------------------------------------
  #define ECHOPIN 3           //Tránh chấn số 0, vì dễ bị coi là ở mode nạp code do SR-04 sẽ luôn kéo ECHO về 0
  #define TRIGPIN 2
  #define PIN_SERVOHAND 1    // GPIO1 = TX
  #define PIN_LED 2          // LED_BUILDIN
  #define PIN_CHARGER_KEY 0   // Đánh thức Phím Key của Bộ sạc
  #include <Servo.h>
  Servo ServoHand;
  #define LED_BUILDIN 2  
  #define MIN_PULSE_WIDTH       544     // Đổ rộng xung tương ứng với góc quay tổi thiểu của SG90s
  #define MAX_PULSE_WIDTH      2400     // Đổ rộng xung tương ứng với góc quay tổi đa của SG90s
#endif

#define NOW 1
#define FREE_ANGLE 100
#define THROUGH_ANGLE 0
#define DISTANCE_LOW_THREADHOLD 8
#define REFRESH_PERIOD_MS 20  // Khoảng trễ để đợi servo hoàn thành góc quay
#define IDLE_THREADHOLD 50000  // Đếm số lần lặp tốc độ cao để bắt đầu chuyển sang chế độ lặp tốc độ chậm nhằm giảm tiêu thụ điện.
//#define DEPTH_SLEEP  // Giới hạn thời gian kiểm tra để tiết kiệm pin. Cần sử dụng các mode sleep của hardware.

#ifdef SIOT_DATA
  #include "src/SiotCore.h"
  SiotCore core;
  unsigned int post_ignore;  // đếm số lần giãn cách, giảm số lượng lần đẩy dữ liệu lên server
  //Biến chứa kết quả ack từ SIOT server sau khi upload data
  String response;
#endif


unsigned int idle_count;  // đếm số lần tĩnh lặng để tiết kiệm năng lượng

//Biến chứa khoảng cách. Xem hàm GetDistance
unsigned int distance;
unsigned int pre_distance;

#ifdef PIN_LIGHT_SENSOR
  //Biến chứa mức sáng. Xem hàm GetLumen
  unsigned int lumen;
  unsigned int pre_lumen;
#endif



/*
   Server di chuyen 1 goc nhat dinh
   @param pos goc can di chuyen = 0 --> 180
   @remark Bởi vì việc di chuyển cần thời gian, còn lâu hơn cả 1 chu kì điều khiển, nên phai lặp lại chu kì điều khiển trong nhiều lần mới bảo đảm.
*/
void ServoMove(int pos) {
  int stall = 14;
  for (; stall > 0; stall--) {
    ServoHand.write(pos);      // tell servo to go to position in variable 'pos'
    delay(REFRESH_PERIOD_MS);  // waits 20ms for refresh period
#if (defined(DIGISPARK_KICKSTARTER_PRO) || defined(ATTiny85))
    SoftRcPulseOut::refresh(NOW);  // Bắt buộc phải có hàm này để bắt đầu thực thi góc quay
#endif
  }
}

//-----------------------------------------------------------------------------------------------------------------------
// the setup routine runs once when you press reset:
//-----------------------------------------------------------------------------------------------------------------------
void setup() {
  // initialize the digital pin as an output
  pinMode(PIN_LED, OUTPUT);  //LED on Model A  or Pro
  pinMode(PIN_CHARGER_KEY, OUTPUT);  // Button K on charge module, LOW Active to wakeup him

  // Yêu cầu mạch sạc hoạt động bằng 1 xung LOW
  digitalWrite(PIN_CHARGER_KEY, LOW);
  delay(200);
  digitalWrite(PIN_CHARGER_KEY, HIGH);
  
  // Báo hiệu bắt đầu
  digitalWrite(PIN_LED, HIGH);
  delay(100);
  digitalWrite(PIN_LED, LOW);
  delay(500);
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_LED, LOW);

  // Thiết lập ngay mode vào ra của pin đo khoảng cách
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  // Khởi tạo servo cánh tay đổ rác
 #if defined(ATTiny85) 
    ServoHand.attach(PIN_SERVOHAND);
 #else
    ServoHand.attach(PIN_SERVOHAND, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
#endif
  ServoMove(FREE_ANGLE);

  //ServoHand.detach();   //Không dược detach quá sớm vì có thể servo chưa chuyển động xong


#ifdef SIOT_DATA
  // Khởi tạo kết nối với máy chủ SIOT Sphere
  core.init();
  core.updateData(URL_TURNONTIME, String(distance), response, POST);  // Đẩy thời điểm được bật lên server
#endif

  //Update fireware từ xa. Chưa áp dụng.
  //core.updateFireware("1.0")

  ServoHand.detach();

  // Báo hiệu đã chỉnh servo
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_LED, LOW);

  // Thiết lập chân ADC A0 duy nhất cho các cảm biến analog: ánh sáng
  //pinMode(PIN_LIGHT_SENSOR, INPUT);

  idle_count = 0;
#ifdef SIOT_DATA
  post_ignore = 0;
#endif  


}

// Lấy khoảng cách từ cảm biến SR-04
int GetDistance() {
  digitalWrite(TRIGPIN, LOW);  // Set chan Trig xuong muc thap 2uS
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);  //Gui mot xung 10uS muc cao vao chan Trig
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);                      // Gui lai muc thap vao chan Trig
  unsigned int distance = pulseIn(ECHOPIN, HIGH);  // Doc thoi gian tra ve. Hay dung kieu unsign. Da ghi nhan co so am tra ve roi
  return distance / 58;                            // Tinh toan khoang cach
}

#ifdef PIN_LIGHT_SENSOR
// Lấy mức độ ánh sáng
int GetLumen() {
  return analogRead(PIN_LIGHT_SENSOR);
}
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
// the loop routine runs over and over again forever:
//-----------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  pre_distance = distance;
  distance = GetDistance();

#ifdef PIN_LIGHT_SENSOR
  pre_lumen = lumen;
  lumen = GetLumen();
#endif

  delay(100);

 // Nếu đợi lâu không đổ rác thì sẽ giảm thời gian đợi, giúp tiết kiệm năng lượng
#ifdef DEPTH_SLEEP  // Giới hạn thời gian kiểm tra để tiết kiệm pin. Cần sử dụng các mode sleep của hardware.
  if (idle_count > IDLE_THREADHOLD) {
    delay(2000);
  }
#endif
  
  if (distance < DISTANCE_LOW_THREADHOLD) {
    // Bật đèn báo và  Yêu cầu mạch sạc hoạt động bằng 1 xung LOW
   
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_CHARGER_KEY, LOW);    
    delay(1000);
    digitalWrite(PIN_CHARGER_KEY, HIGH);

    // Tắt đèn để dành năng lượng cho servo
    digitalWrite(PIN_LED, LOW);

    // Attach và Detach ngay sau khi sử dụng để tiết kiệm năng lượng
 #if defined(ATTiny85) 
    ServoHand.attach(PIN_SERVOHAND);
 #else
    ServoHand.attach(PIN_SERVOHAND, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
#endif    

    ServoMove(THROUGH_ANGLE);
    delay(500);
    ServoMove(FREE_ANGLE);

#ifdef SIOT_DATA
    core.updateData(URL_EATING, String(idle_count), response, POST);  // Đẩy thời điểm đổ rác lên hệ thống
    core.updateData(URL_DISTANCE, String(distance), response, POST);  // Đẩy dữ liệu khoảng cách lên hệ thống
#endif

    //Fix-bug: Neu su dung nguon Adpater (khong su dung dung data usb cable) thi khoang cach do duoc tu lan thu 2 tro luon < nguong, lam cho BOT lien tuc do rac.
    //Nghi ngo nguyen nhan: do khong gui thong tin qua DigiKeyboard, viec lay do khoang cach dien ra nhanh qua, khi ma dong co servo chua ve dung vi tri,
    //Giai phap: them do tre
    delay(200);

    // Attach và Detach ngay sau khi sử dụng để tiết kiệm năng lượng
    ServoHand.detach();
    digitalWrite(LED_BUILDIN, LOW);

    idle_count = 0;
  } else {
    // Đêm tăng số lần không có rác
    idle_count++;
  }

  //Gửi dữ liệu về server SIOT nếu có thay đổi
#ifdef SIOT_DATA
  post_ignore++;
  if (post_ignore > POST_THREADHOLD) {
    if (pre_distance != distance) {
      core.updateData(URL_DISTANCE, String(distance), response, POST);  // Đẩy dữ liệu khoảng cách lên hệ thống
    }
    if (pre_lumen != lumen) {
      core.updateData(URL_LUMINATION, String(lumen), response, POST);  // Đẩy dữ liệu đô sáng lên hệ thống
    }
    post_ignore = 0;
  }
#endif
}
