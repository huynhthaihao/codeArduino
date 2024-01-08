#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4

#include <SoftwareSerial.h> // Thư viện cho cổng nối tiếp mềm
#define RX_PIN 10// Chân RX của cổng nối tiếp mềm
#define TX_PIN 11 // Chân TX của cổng nối tiếp mềm
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Tạo một đối tượng cổng nối tiếp mềm

const int trig = 5;     // chân trig của HC-SR04
const int echo = 6;     // chân echo của HC-SR04
int CAMBIEN = 13;  
int Inrelay = 2; 
int anhsang = A0;
int giatri;
int den = 4; 
int coi = 12;
const int obstaclePin = 3;  // Chân cảm biến hồng ngoại 1
float nhietdo;
    
int kcach;  
String Data;
void truyen_uart()
{
  delay(2000);
  String Data = String(nhietdo)+";"+String(kcach-12)+";"+String(giatri);
    mySerial.println(Data);
    Serial.print(Data);

}
void CBndo() 
{
nhietdo = mlx.readObjectTempC();
nhietdo=nhietdo+1.1;// chỉnh sai số
truyen_uart();
if(nhietdo>=36){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(nhietdo);
  lcd.print("*C");
  lcd.print(" SOT CAO");
 lcd.setCursor(2,1);
  lcd.print("NGUY HIEM...!");
  digitalWrite(coi, HIGH);
   delay(3000);
     digitalWrite(coi, LOW); 
} 
else {
  lcd.clear();
 lcd.setCursor(0,0);
  lcd.print("Nhietdo: ");
  lcd.print(nhietdo);
  lcd.print("*C");
 lcd.setCursor(2,1);
  lcd.print("SUC KHOE TOT");
   delay(3000);
}
}
void CBhngoai(){
   if(digitalRead(CAMBIEN) == 0)  
  {
    digitalWrite(Inrelay, LOW);  
    delay(1500);               
    digitalWrite(Inrelay, HIGH); 
    delay(300);      
     CBdtich();
     delay(2000);            
}
  else                           
  {   

    digitalWrite(Inrelay, HIGH); 
   
  }
}

void CBdtich(){
 unsigned long duration; // biến đo thời gian
            
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    kcach = int(duration/2/29.412);
    truyen_uart();
    /* In kết quả ra Serial Monitor */
    //Serial.print(kcach);  
    //Serial.println("ml");
   if(kcach>10){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Dung tich:");
    lcd.print(kcach-12);
    lcd.print("ml");
    lcd.setCursor(3, 1);
    lcd.print("LUU Y...!");
    digitalWrite(coi, HIGH); //Đèn sáng
    delay(2000);
    digitalWrite(coi, LOW);  
   }
   else {
    lcd.clear();
    lcd.setCursor(0, 0);
   lcd.print("Dung tich:");
  lcd.print(kcach+12);
    lcd.print("ml");
      delay(2000);

   }

}

void CBasang() 
{
  giatri = digitalRead(anhsang); //Đọc giá trị cảm biến và gán vào biến giatri
  truyen_uart();
  if (giatri == 1) //Nếu giá trị quang trở lớn hơn 1000
  {
    digitalWrite(den, HIGH); //Đèn sáng

  }
  else //Ngược lại
  {
    digitalWrite(den, LOW); //Đèn tắt
  }
  //Serial.print("Giá trị cảm biến: ");
  //Serial.println(giatri);
  delay(200);
}

void setup() {
Serial.begin(9600); // Khởi tạo cổng nối tiếp mềm với tốc độ baud là 9600
mySerial.begin(9600);
pinMode(Inrelay, OUTPUT);
pinMode(CAMBIEN, INPUT_PULLUP);
pinMode(obstaclePin, INPUT);
pinMode(coi, OUTPUT);
pinMode(den, OUTPUT);
digitalWrite(den, LOW);
pinMode(anhsang, INPUT); //Cảm biến nhận tín hiệu
mlx.begin();
lcd.init();
lcd.backlight();
truyen_uart();
pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
lcd.setCursor(0,0);
lcd.print("RUA TAY-DO NHIET");
lcd.setCursor(5,1);
lcd.print("TU DONG");
delay(2000);
lcd.clear();
}
void loop() {
  // Kiểm tra trạng thái cảm biến hồng ngoại (ví dụ: chân 2)
  int obstacleStatus = digitalRead(obstaclePin);
// Nếu có vật cản, đọc và hiển thị nhiệt độ từ cảm biến MLX90614
  if (obstacleStatus == LOW) {
CBndo();
delay(5000);
}
 else {
    // Nếu không có vật cản, hiển thị thông báo trên màn hình LCD
    lcd.clear();
  lcd.setCursor(5,0);
lcd.print("RUA TAY");
lcd.setCursor(0,1);
lcd.print("BAO VE SUC KHOE");

  }
  CBasang();
  CBhngoai();
   delay(1000);  
}