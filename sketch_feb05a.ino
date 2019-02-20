
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <Harman.h>

//here we are declaring our variables.
char minvalue[3] = {};//declared a minval array
char maxvalue[3] = {};//declared a maxval array
String val1;//declaring variable
String val2;//declaring variable
int minv;//declaring variable
int maxv;//declaring variable

int k;//declaring variable
//----------------------------

/* Soil Moisture Basic Example
  Basic sketch to print out soil moisture values to the Serial Monitor and the keypad */
int val = 0;//variable for storing moisture value.
int soilPin = A0;//Declare a variable for the soil moisture sensor
int soilPower = 7;//Variable for Soil moisture Power
short int pump_pin = 12;//declaring a variable
short int led_pin = 13;//declaring a variable
//Rather than powering the sensor through the 3.3V or 5V pins,
//we'll use a digital pin to power the sensor. This will
//prevent corrosion of the sensor as it sits in the soil
//-------------------------

// Set the LCD address to 0x27   for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);
const byte ROWS = 4; //four rows for keypad
const byte COLS = 3; //four columns for keypad
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);//initialize an instance of class NewKeypad
void setup() {
  Serial.begin(9600);//initializing the serial monitor and set baud rates to 9600
  lcd.begin();//initializing the lcd.
  lcd.backlight();// Turn on the blacklight and print a message.
  lcd.print("SYSTEM INIT.....");
  pinMode(pump_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  //Set D7 as an OUTPUT
  pinMode(soilPower, OUTPUT);
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  delay(1000);//delay for 1 second
  lcd.clear();
  lcd.print("INITIALIZED");
  delay(1000);
}

void loop() {

  char customKey = customKeypad.getKey();//keypad is to accept the keys pressed

  if (customKey) {
    Serial.println(customKey);
    if (customKey == '#') {//if you have presed the #value
      lcd.clear();
      lcd.print("Enter min hmdity");
      k = 0;
      while (k < 3) {
        char customKey = customKeypad.getKey();
        if (customKey) {
          lcd.setCursor(5 + k, 1);//
          lcd.print(customKey);
          minvalue[k] = customKey;
          k++;
        }
      }
      val1 = minvalue;
      minv = val1.toInt();//converting minval to an integer value 
      delay(1000);
      lcd.clear();
      lcd.print("Enter max hmdity");
      k = 0;
      while (k < 3) {
        char customKey = customKeypad.getKey();
        if (customKey) {
          lcd.setCursor(5 + k, 1);
          lcd.print(customKey);
          maxvalue[k] = customKey;
          k++;
        }

      }
      val2 = maxvalue;
      maxv = val2.toInt();
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Min Value =");
      lcd.setCursor(12, 0);
      lcd.print(minv);
      lcd.setCursor(0, 1);
      lcd.print("Max Value =");
      lcd.setCursor(12, 1);
      lcd.print(maxv);
      delay(2000);
    }
  }
  //-------------------------------------------------------------------
  //get soil moisture value from the function below and print it
  Serial.print("Soil Moisture = ");
  Serial.println(readSoil());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture =");
  lcd.setCursor(12, 0);
  lcd.print(readSoil());

  if (readSoil() > minv && readSoil() < maxv) {
    digitalWrite(pump_pin, HIGH);
    digitalWrite(led_pin, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("PUMP ON");
  }
  else {
    digitalWrite(pump_pin, LOW);
    digitalWrite(led_pin, LOW);

  }

  //This 1 second time frame is used so you can test the sensor and see it change in real-time.
  //For in-plant applications, you will want to take readings much less frequently.
  delay(1000); //take a reading every second
  //--------------------------------------------------------------------


}

//--------------------------------------------------------------------
int readSoil()
{
  digitalWrite(soilPower, HIGH); //turn D7 "On"
  delay(100); //wait 100 milliseconds
  val = analogRead(soilPin); //Read the SIG value from sensor
  digitalWrite(soilPower, LOW); //turn D7 "Off"
  return val; //send current moisture value
}












//---------------------------------------------------------------------
