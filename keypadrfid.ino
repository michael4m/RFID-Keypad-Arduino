#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SPI.h>
// Create instances
MFRC522 mfrc522(10, 0); // MFRC522 mfrc522(SS_PIN, RST_PIN)
//LiquidCrystal_I2C lcd(0x27, 16, 2);


constexpr uint8_t relay = 9;
char initial_password[6] = {'1','2','3','7','5','9'};  // pass
String tagUID = "AA B6 45 09";  // card code
String tagUID2 = "C7 12 D5 34";
char password[6];   // Variable to password
boolean RFIDMode = true; // boolean 
boolean NormalMode = true; // boolean 
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter
// defining how many rows and columns our keypad have
const byte rows = 4;
const byte columns = 3;
// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Initializing pins for keypad
byte row_pins[rows] = {2, 3, 4, 5};
byte column_pins[columns] = {6, 7, 8};
// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  //lcd.init();   // LCD screen
  //lcd.backlight();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  delay(100);
 // lcd.clear(); // Clear LCD screen
}
void loop() {
  if (NormalMode == false) {
    // Function to receive message
  }
  else if (NormalMode == true) {
    // System will first look for mode
    if (RFIDMode == true) {
      // Function to receive message
      //lcd.setCursor(0, 0);
      //lcd.print("   Door Lock");
      //lcd.setCursor(0, 1);
      //lcd.print(" Scan Your Tag ");
      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }
      //Reading from the card
      String tag = "";
      for (byte j = 0; j < mfrc522.uid.size; j++)
      {
        tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
        tag.concat(String(mfrc522.uid.uidByte[j], HEX));
      }
      tag.toUpperCase();
      //Checking the card
      if ((tag.substring(1) == tagUID) || (tag.substring(1) == tagUID2))
      {
        // If UID of tag is matched.
       // lcd.clear();
       // lcd.print("Tag Matched");
        Serial.print("Tag Matched");
        delay(500);
       // lcd.clear();
       //lcd.print("Enter Password:");
        Serial.print("Enter Password:");
       // lcd.setCursor(0, 1);
        RFIDMode = false; // Make RFID mode false
      }
      else
      {
        // If UID of tag is not matched.
       // lcd.clear();
       // lcd.setCursor(0, 0);
       // lcd.print("Wrong Tag Shown");
       // lcd.setCursor(0, 1);
        //lcd.print("Access Denied");
        delay(3000);
       // lcd.clear();
      }
    }
    // If RFID mode is false, it will look for keys from keypad
    if (RFIDMode == false) {
      key_pressed = keypad_key.getKey(); // Storing keys
      if (key_pressed)
      {
        password[i++] = key_pressed; // Storing in password variable
        //lcd.print("*");
      }
      if (i == 6) // If 4 keys are completed
      {
        delay(200);
        if (!(strncmp(password, initial_password, 6))) // If password is matched
        {
          //lcd.clear();
          //lcd.print("Pass Accepted");
          Serial.print("Pass Accepted");
          digitalWrite(relay, LOW);
          delay(6000);
          digitalWrite(relay, HIGH);
         // lcd.clear();
          i = 0;
          RFIDMode = true; // Make RFID mode true
        }
        else    // If password is not matched
        {
          //lcd.clear();
          //lcd.print("Wrong Password");
          Serial.print("Wrong Password");
          delay(1000);
          //lcd.clear();
          i = 0;
          RFIDMode = true;  // Make RFID mode true
        }
      }
    }
  }
}

