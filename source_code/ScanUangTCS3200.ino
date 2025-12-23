#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#define S0 4 
#define S1 5 
#define S2 6 
#define S3 7 
#define sensorOut 8 
#define buzzerPin 3 
#define buttonPin 10   
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  
 
int r = 0, g = 0, b = 0; 
long totalSaldo = 0; 
 
int toleransi = 5;  
 
void setup() { 
  // Setup Pin 
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT); 
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT); 
  pinMode(sensorOut, INPUT); 
   
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT); 
 
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); 
 
  Serial.begin(9600); 
  lcd.init(); lcd.backlight(); 
 
 
   
  // Tampilan Awal LCD 
  lcd.setCursor(0, 0); lcd.print("Celengan Pintar"); 
  lcd.setCursor(0, 1); lcd.print("System Ready.."); 
  tone(buzzerPin, 2000, 100); delay(150); tone(buzzerPin, 3000, 100); 
  delay(2000); 
   
  tampilSaldo(); 
} 
 
void loop() { 
  // --- CEK TOMBOL RESET --- 
  if (digitalRead(buttonPin) == LOW) { 
    tone(buzzerPin, 1000, 200); 
    totalSaldo = 0; 
    lcd.clear();  
    lcd.setCursor(0,0); lcd.print("SALDO DI-RESET!"); 
    lcd.setCursor(0,1); lcd.print("Rp 0"); 
    delay(1000);  
    tampilSaldo(); 
  } 
 
  // ---  PROSES SCAN WARNA --- 
  bacaWarnaRataRata(); 
   
  Serial.print("R:"); Serial.print(r); 
  Serial.print(" G:"); Serial.print(g); 
  Serial.print(" B:"); Serial.println(b); 
 
  long uangTerdeteksi = 0; 
 
  
  // RP 100.000  
  if (cekWarna(41, 65, 54)) { 
    uangTerdeteksi = 100000; 
  } 
 
  // RP 50.000  
  else if (cekWarna(57, 51, 41)) { 
    uangTerdeteksi = 50000; 
  } 
   
  // RP 20.000  
  else if (cekWarna(58, 56, 53)) { 
    uangTerdeteksi = 20000; 
  } 
 
  // RP 10.000  
  else if (cekWarna(72, 86, 62)) { 
    uangTerdeteksi = 10000; 
  } 
   
  // RP 5.000  
  else if (cekWarna(45, 59, 48)) { 
    uangTerdeteksi = 5000; 
  } 
   
  // RP 2.000  
  else if (cekWarna(47, 44, 38)) { 
    uangTerdeteksi = 2000; 
 
 
  } 
   
  // RP 1.000  
  else if (cekWarna(36, 39, 40)) { 
    uangTerdeteksi = 1000; 
  } 
 
  
  if (uangTerdeteksi > 0) { 
    lcd.clear(); 
    lcd.setCursor(0,0); lcd.print("Masuk: Rp"); lcd.print(uangTerdeteksi); 
    lcd.setCursor(0,1); lcd.print("Total: Rp"); lcd.print(totalSaldo + 
uangTerdeteksi); 
     
  
    tone(buzzerPin, 1500, 100); delay(100); tone(buzzerPin, 2500, 200); 
     
    totalSaldo += uangTerdeteksi; 
     
    delay(4000);  
     
    tampilSaldo(); 
  } 
   
  delay(250);  
} 
 
 
void bacaWarna() { 
  digitalWrite(S2, LOW); digitalWrite(S3, LOW); 
  r = pulseIn(sensorOut, LOW); delay(10); 
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); 
  g = pulseIn(sensorOut, LOW); delay(10); 
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH); 
  b = pulseIn(sensorOut, LOW); delay(10); 
} 
 
void bacaWarnaRataRata() { 
  long totalR = 0, totalG = 0, totalB = 0; 
  for(int i=0; i<5; i++) {  
    bacaWarna(); 
    totalR += r; totalG += g; totalB += b; 
  } 
  r = totalR / 5; g = totalG / 5; b = totalB / 5; 
} 
 
bool cekWarna(int targetR, int targetG, int targetB) { 
  if (r >= targetR - toleransi && r <= targetR + toleransi && 
      g >= targetG - toleransi && g <= targetG + toleransi && 
      b >= targetB - toleransi && b <= targetB + toleransi) { 
    return true; 
  } 
  return false; 
} 
 
void tampilSaldo() { 
  lcd.clear(); 
  lcd.setCursor(0, 0); lcd.print("Saldo Tabungan:"); 
  lcd.setCursor(0, 1); lcd.print("Rp "); lcd.print(totalSaldo); 
} 
