//LCD............................................................................................................
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

//BUZZER.........................................................................................................
const int buzzer = 8;

int lamp = 13; // LED terhubung pada pin digital 13
int lamp2 = 12; // LED terhubung pada pin digital 12
//TGS............................................................................................................
float tgs;
float Volt;
float Ro;
float Rt;//Rs
float ppm;
// Your threshold value
int sensorThres = 400;

//TCS...........................................................................................................
#define S0  2
#define S1  3
#define S2  4
#define S3  5
#define OUT 6

int A,D,C = 0;
int R;
int G;
int B;


unsigned long datamilis = 0;
int count = 0;

void setup() {
 // initialize the lcd
 lcd.init(); 
 // initialize the tgs sensor
 pinMode(tgs, INPUT); 
 pinMode(buzzer, OUTPUT);
 // initialize the tcs sensor
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(OUT, INPUT);  
 // Setting frequency-scaling to 20%
 digitalWrite(S0,HIGH);
 digitalWrite(S1,LOW);
 pinMode(lamp, OUTPUT); // Mengatur pin sebagai output
 pinMode(lamp2, OUTPUT); // Mengatur pin sebagai output
 Serial.begin(9600);
                    
}

void loop() {
  if(millis() - datamilis > 100){
  datamilis = millis();
  tampilan();
  sensortgs();
  // Setting red filtered photodiodes to be read Red frequency
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  A = pulseIn(OUT, LOW);  // Reading the output Red frequency
  R = A - 20; 
  // Setting Green filtered photodiodes to be read Green frequency
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  D = pulseIn(OUT, LOW);  // Reading the output Green frequency
  G = B - 20;
  // Setting Blue filtered photodiodes to be read Blue frequency
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  C = pulseIn(OUT, LOW);  // Reading the output Blue frequency
  B = C - 20;
  // Print RGB Sensor Values
  Serial.print("R= ");
  Serial.print(R);
  Serial.print(" | ");
  Serial.print("G= ");
  Serial.print(G);
  Serial.print(" | ");
  Serial.print("B= ");
  Serial.print(B);
  Serial.println();

  //buzzer urgent........................................................................................
  if(ppm >= 12){
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(500);
    
    }
  else if(ppm <= 11){
    digitalWrite(buzzer,LOW);
    }
  //LED urgent........................................................................................
  if (R < 70 && G < 70 && B < 70) {
    Serial.println("Status: Aman");
    digitalWrite(lamp,LOW);
    digitalWrite(lamp2,LOW);
  }
  else if (R >= 80 && R <= 100 && G >= 80 && G <= 100 && B >= 80 && B <= 100) {
    Serial.println("Status: Sedang");
    digitalWrite(lamp,HIGH);
    digitalWrite(lamp2,LOW);
  }
  else if (R > 110 && G > 110 || B > 110) {
    Serial.println("Status: Bahaya");
    digitalWrite(lamp,HIGH);
    digitalWrite(lamp2,HIGH);
  }
 }
}
void sensortgs() {
  tgs = analogRead(D3);
  Volt = (tgs/1023)*5;
  Rt = (5/Volt-1)*10;
  Ro = (5-Volt)*(5-Volt)/Rt;
  ppm = 35*(Ro-0.32/0.32)+20;
  
  if(ppm <0) ppm = 0;

  Serial.print(tgs);
  Serial.print(" || ");
  Serial.print(Volt);
  Serial.print(" V || ");
  Serial.print(Rt);
  Serial.print(" Rs || ");
  Serial.print(Ro);
  Serial.print(" Ro || ");
  Serial.print(ppm);
  Serial.println(" ppm ");
  delay (1000);
}
void tampilan(){
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CEK FORMALIN + WARNA");
  lcd.setCursor(0, 1);
  lcd.print("R = ");
  lcd.setCursor(3, 1);
  lcd.print(R);
  lcd.setCursor(6, 1);
  lcd.print(" G = ");
  lcd.setCursor(10, 1);
  lcd.print(G);
  lcd.setCursor(13, 1);
  lcd.print(" B = ");
  lcd.setCursor(17, 1);
  lcd.print(B);
  lcd.setCursor(0, 2);
  lcd.print("FORMALIN :");
  lcd.setCursor(10, 2);
  lcd.print(ppm);
  lcd.setCursor(15, 2);
  lcd.print("ppm");
  delay(500);
}
