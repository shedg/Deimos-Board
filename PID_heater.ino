#include <AutoPID.h>             //nice quick PID with variable parameters, has an aggressive curve when far away and then tries to smooth to a nice lower value to prevent overshoot
#include <Thermistor.h>         //these 3 are all for making a simple and quick thermistor reading
#include <NTC_Thermistor.h>
#include <AverageThermistor.h>
#include <LiquidCrystal.h>      //just to use a simple LCD screen to display temperatures
#include <AceButton.h>
using namespace ace_button;
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
/*LCD SETUP----------------------------------------------------------------------------------------
initialize the library by associating any needed LCD interface pin
 with the arduino pin number it is connected to*/
 
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int Contrast_Pin = A4;

//this one is to print a custom arrow on the LCD
byte customChar[] = {
  B00100,
  B00110,
  B00011,
  B11111,
  B11111,
  B00011,
  B00110,
  B00100
};

//BUTTONS SETUP--------------------------------------------------------------------------------------

//3 buttons, SET,UP and DOWN to manage the setpoint
const int  SETPin = A1;
const int  UPPin = A2;
const int  DOWNPin = A3;

AceButton SET(SETPin);
AceButton UP(UPPin);
AceButton DOWN(DOWNPin);

//even handler for the buttons
void handleEvent(AceButton*, uint8_t, uint8_t);

//index for the setting mode
bool setting =0;

//THERMISTOR SETUP-------------------------------------------------------------------------------------

//Values for the thermistor, it's set to an average glass bead thermistor for 3D printers
#define SENSOR_PIN             A0 //A0
#define REFERENCE_RESISTANCE   100000
#define NOMINAL_RESISTANCE     100000
#define NOMINAL_TEMPERATURE    25
#define B_VALUE                3950

/**
  How many readings are taken to determine a mean temperature.
  The more values, the longer a calibration is performed,
  but the readings will be more accurate.
*/
#define READINGS_NUMBER 10

/**
  Delay time between a temperature readings
  from the temperature sensor (ms).
*/
#define DELAY_TIME 10

//creating the thermistor
Thermistor* thermistor = NULL;

//PID SETUP---------------------------------------------------------------------------------

//pid settings and gains
#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define KP 2 //.12
#define KI 5 //.0003
#define KD 1 //0
#define OUTPUT_PIN 5 //D5 this is the pin that drives the mosfet for the heater

//Define Variables for the PID we'll be connecting to
double Setpoint, Input, Output;

//input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&Input, &Setpoint, &Output, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void setup() {
  
  //setting up the button's pins and index number
  // Button uses the built-in pull up register.
  pinMode(SETPin, INPUT_PULLUP);
  pinMode(UPPin, INPUT_PULLUP);
  pinMode(DOWNPin, INPUT_PULLUP);

  SET.init(SETPin);
  UP.init(UPPin);
  DOWN.init(DOWNPin);

  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  int clickDelay=100;
  void setClickDelay(uint16_t clickDelay);
  
  
  //set the contrast pin on the display 
  pinMode(Contrast_Pin, OUTPUT);
  analogWrite(Contrast_Pin, 5);
  
  // set up the LCD's number of columns and rows and prepares the display so that it only has to update the numbers
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("Current:   Set:");
  lcd.createChar(0, customChar);
  
  //THERMISTOR SETUP
  Serial.begin(115200);

  Thermistor* originThermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE
  );
  thermistor = new AverageThermistor(
    originThermistor,
    READINGS_NUMBER,
    DELAY_TIME
  );

  //PID SETUP
  
  //initialize the variables we're linked to
  Input = thermistor->readCelsius(); //uses the thermistor library to read the temperature in °C
  
  Setpoint = 15; //so that when turning it on it will always be a safe value and won't turn on the heater unless we set a temperature

  pinMode(OUTPUT_PIN, OUTPUT);
  //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  
  //set PID update interval (ms)
  myPID.setTimeStep(40);
}


void loop() {
  
  
  //writes on the display the current temperatures 
  lcd.setCursor(0, 1);
  lcd.print(Input, 1);
  lcd.write(B11011111);
  lcd.print("C");
  lcd.setCursor(11, 1);
  lcd.print(Setpoint);
    
  //check buttons status
   SET.check();
   
  if(setting==1)
  {
    lcd.setCursor(10, 1);
    lcd.write(byte(0)); 
    UP.check();
    DOWN.check();
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print(" ");
  }
  
  
  Input = thermistor->readCelsius();
  Serial.print("Input: ");
  Serial.println(Input);
  myPID.run();
  Serial.print("Output: ");
  Serial.println(Output);
  analogWrite(OUTPUT_PIN, Output);

  
}

//handler for the buttons to check input
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  
   switch (eventType){
    case AceButton::kEventPressed:{
      switch (button->getPin()){
        case SETPin :         
          setting=!setting;
          break;
        case UPPin :
          Setpoint++;
          break;
        case DOWNPin :
          Setpoint--;
          break;
        }
    }
 }
}
