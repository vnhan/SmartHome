/*=====================================================
   Project Centre Processing Unit of Nhannv SmartHome
   Design hardward and software by Nhannv
   Function :
 
   
=====================================================*/

//include library

#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"


//declare variable

// set pin numbers:
const int btn_hallway = 2;     // the number of the pushbutton pin
const int hallway_ledPin = 3;      // the number of the LED pin
const int btn_bedroom1 = 4; 
const int bedroom1_ledPin = 5;
const int btn_bedroom2 = 6; 
const int bedroom2_ledPin = 7;
const int btn_outdoor = 8; 
const int outdoor_ledPin = 9;


//flag of state
int flag_hallway = 0;
int flag_bedroom1 = 0;
int flag_bedroom2 = 0;
int flag_outdoor = 0;

// variables will change:
int buttonState = LOW;         // variable for reading the pushbutton status

//declare case
#define HALLWAY 1
#define BEDROOM1 2
#define BEDROOM2 3
#define OUTDOOR 4
#define SHORT_TIME_MODE 1
#define FULL_TIME_MODE 2
#define DEFAULT_BACKLIGHT_TIME 300

//declare RTC
const byte DS1307 = 0x68; //address of RTC module
//RTC variable
const byte NumberOfFields = 7;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
RTC_DS1307 RTC;

//declare LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Set the LCD I2C address

//Time variable
int second, minute, hour, day, wday, month, year;
String strSecond, strMinute, strHour, strDay, strWDay, strMonth, strYear;
int delayReadTime1 = 0;
int delayReadTime2 = 5;
int tempMinute = 0;
int tempSecond = 0;
int gMode = FULL_TIME_MODE;
int previousMode = FULL_TIME_MODE;
int delayBackLightTime = 0;

//declare function

void changeState(int ledChange) {
  lcd.clear();
  lcd.backlight();
  delayBackLightTime =0;
  displayDateTime(SHORT_TIME_MODE);
  
  if (ledChange == HALLWAY) {
    if (flag_hallway == 0) {
      digitalWrite(hallway_ledPin,LOW);
      //digitalWrite(outdoor_ledPin,LOW);
      digitalWrite(bedroom1_ledPin,HIGH);
      flag_hallway=1;  
      //Serial.println("Turn on light");
      displayLCD("Turn On Hallway",0,1);
    } 
    //temp function in case bedroom button is not finish
    else if (flag_hallway ==1) {
      digitalWrite(hallway_ledPin,HIGH);
      //digitalWrite(outdoor_ledPin,HIGH);
      digitalWrite(bedroom1_ledPin,LOW);
      flag_hallway=2;
      displayLCD("Turn On Bedroom1",0,1);
    }

    else if (flag_hallway ==2) {
      digitalWrite(hallway_ledPin,LOW);
      //digitalWrite(outdoor_ledPin,LOW);
      digitalWrite(bedroom1_ledPin,LOW);
      flag_hallway=3;
      displayLCD("Turn On 2 light",0,1);
    }
    
    else {
      digitalWrite(hallway_ledPin,HIGH);
      //digitalWrite(outdoor_ledPin,HIGH);
      digitalWrite(bedroom1_ledPin,HIGH); //in case temp function to turn off led
      flag_hallway=0;
      //Serial.println("Turn off light");
      displayLCD("Turn Off light",0,1);
    }


    
  }
  if (ledChange == BEDROOM1) {
    
  }
};


//Read time from RTC module
void readTime() {

  DateTime now = RTC.now(); 
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println(); 
//    delay(1000);
  second=now.second(), DEC;
  minute=now.minute(), DEC;
  hour=now.hour(), DEC;
  day = now.day(), DEC;
  month = now.month(), DEC;
  year = now.year(), DEC;

  if (second >= 10) {
    strSecond = String(second);
  } else strSecond = "0" + String(second);

  if (minute >=10) {
    strMinute = String(minute);
  } else strMinute = "0" + String(minute);

  if (hour >= 10) {
    strHour = String(hour);
  } else strHour = "0" + String(hour);

  if (day >= 10) {
    strDay = String(day);
  } else strDay = "0" + String(day);

  if (month >= 10) {
    strMonth = String(month);
  } else strMonth = "0" + String(month);

  strYear = String(year);
}

//Display LCD content
void displayLCD(String txtInput,int charNo, int lineNo) {
  //lcd.clear();
  //lcd.backlight();
  lcd.setCursor(charNo,lineNo);
  lcd.print(txtInput);
  delay(50);
}
//Flash screen
void flashScreenLCD() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting");
  delay(1000);
  lcd.setCursor(8,0);
  lcd.print(".");
  delay(1000);
  lcd.setCursor(9,0);
  lcd.print(".");
  delay(1000);
  lcd.setCursor(10,0);
  lcd.print(".");
  delay(1000);
  lcd.setCursor(11,0);
  lcd.print("OK");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("VNhanHome-Ver1.0");
  delay(3000);
  //lcd.clear();
  //lcd.noDisplay();
}

//display Time on LCD
void displayDateTime(int mode) {
  //readTime();
  //lcd.clear();
  previousMode = mode;
  if (mode == SHORT_TIME_MODE) {
    displayLCD(strHour,0,0);
    displayLCD(":",2,0);
    displayLCD(strMinute,3,0);
    displayLCD("-",5,0);
    displayLCD(strDay,6,0);
    displayLCD("/",8,0);
    displayLCD(strMonth,9,0);
    displayLCD("/",11,0);
    displayLCD(strYear,12,0);
    delayReadTime2=200;
  }
  else if (mode == FULL_TIME_MODE) {
    displayLCD("TIME:",0,0);//line 0
    displayLCD(strHour,6,0);
    displayLCD(":",8,0);
    displayLCD(strMinute,9,0);
    displayLCD(":",11,0);
    displayLCD(strSecond,12,0);
    
    displayLCD("DATE:",0,1);//line 1
    displayLCD(strDay,6,1);
    displayLCD("/",8,1);
    displayLCD(strMonth,9,1);
    displayLCD("/",11,1);
    displayLCD(strYear,12,1);
    delayReadTime2 = 5;
  }
    
  
}

//Off all light
void turnOffAllLight(){
  digitalWrite(hallway_ledPin,HIGH);
    digitalWrite(bedroom1_ledPin,HIGH);
    digitalWrite(bedroom2_ledPin,HIGH);
    digitalWrite(outdoor_ledPin,HIGH);
}

//schedule to turn off all light 
void scheduleTurnOffLight(int iHour, int iMinute) {
  if (hour == iHour && minute == iMinute && second <= 1) {
    turnOffAllLight();
    lcd.clear();
    lcd.backlight();
    delayBackLightTime =0;
    displayDateTime(SHORT_TIME_MODE);
    displayLCD("Schedule Off",0,1);
    delay(3000);
    //displayDateTime();
  }
}

void scheduleTurnOff1Light(int iHour, int iMinute, int light) {
  if (hour == iHour && minute == iMinute && second <= 1) {
    //turnOffAllLight();
    digitalWrite(light,HIGH);
    lcd.clear();
    lcd.backlight();
    delayBackLightTime =0;
    displayDateTime(SHORT_TIME_MODE);
    displayLCD("Schedule Off",0,1);
    delay(3000);
    //displayDateTime();
  }
}

//schedule to turn on the special light
void scheduleTurnOnLight (int iHour, int iMinute) {
  if (hour == iHour && minute == iMinute && second <= 1) {
    digitalWrite(hallway_ledPin,LOW);
    digitalWrite(outdoor_ledPin,LOW);
    lcd.clear();
    lcd.backlight();
    delayBackLightTime =0;
    displayDateTime(SHORT_TIME_MODE);
    displayLCD("Schedule On",0,1);
    delay(3000);
    //displayDateTime();
  }
}

void scheduleTurnOn1Light (int iHour, int iMinute, int light) {
  if (hour == iHour && minute == iMinute && second <= 1) {
    digitalWrite(light,LOW);
    //digitalWrite(outdoor_ledPin,LOW);
    lcd.clear();
    lcd.backlight();
    delayBackLightTime =0;
    displayDateTime(SHORT_TIME_MODE);
    displayLCD("Schedule On",0,1);
    delay(3000);
    //displayDateTime();
  }
}

//Start setup Program
void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.backlight();

  flashScreenLCD();
  lcd.clear();
  displayLCD("Setup device.",0,0);
  
  // initialize the LED pin as an output:
  pinMode(hallway_ledPin, OUTPUT);
  pinMode(bedroom1_ledPin, OUTPUT);
  pinMode(bedroom2_ledPin, OUTPUT);
  pinMode(outdoor_ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(btn_hallway, INPUT);
  //Serial.println("Begin");
  displayLCD("OK",14,0);
  delay(2000);
  turnOffAllLight();
  lcd.clear();
  
  displayLCD("Check RTC.",0,0);
  //Check RTC module
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    displayLCD("RTC is NOT running!",0,1);
    delay(2000);
    lcd.clear();
  } else {
    displayLCD("OK",14,0);
    delay(2000);
    lcd.clear();
    readTime();
  }
  displayDateTime(FULL_TIME_MODE);
  //RTC.adjust(DateTime(2018, 5, 6, 10, 10, 0)); //Set time for first time running or out of battery.
}

void loop() {
  //display Time
  if (delayReadTime1 < delayReadTime2) {
    delayReadTime1++;
  } else {
    readTime();
    delayReadTime1 =0;
  }
    
  if ((tempMinute != minute && gMode ==SHORT_TIME_MODE) || (tempSecond != second && gMode == FULL_TIME_MODE)) {
    
    if (previousMode != FULL_TIME_MODE) {
      gMode = FULL_TIME_MODE;
      lcd.clear();
      delayReadTime2 = 50;
    }
    displayDateTime(FULL_TIME_MODE);
    tempMinute = minute;
    tempSecond = second;
  }

  if(delayBackLightTime >= DEFAULT_BACKLIGHT_TIME) {
    lcd.noBacklight();
    delayBackLightTime=0;
  } else delayBackLightTime++;
  
//  if (delayReadTime1 < 6000) {
//    delayReadTime1++;
//    displayLCD(String(delayReadTime1),0,1);
//    displayLCD(String(delayReadTime2),4,1);
//      
//  } else {
//    if (delayReadTime2 < 10) {
//      delayReadTime2++;
//      delayReadTime1=0;
//      
//    } else {
//      displayDateTime();
//      delayReadTime1 = 0;
//      delayReadTime2 = 0;
//    }
//  }
  
  // read the state of the pushbutton value:
  //Serial.println("Read Button state");
  buttonState = digitalRead(btn_hallway);
  
  if (buttonState == HIGH) {
    //Serial.println("Button Click");
    while (buttonState == HIGH) {
      buttonState = digitalRead(btn_hallway);
      //Serial.println("Read Button state 2");
    }
    changeState(HALLWAY);
  }

  //scheduleTurnOnLight(18,30);
  scheduleTurnOn1Light(18,00, outdoor_ledPin);
  scheduleTurnOn1Light(18,30, hallway_ledPin);
  //scheduleTurnOffLight(23,00);
  scheduleTurnOff1Light(23,00, hallway_ledPin);
  scheduleTurnOff1Light(23,59, outdoor_ledPin);

  delay(50);
}



