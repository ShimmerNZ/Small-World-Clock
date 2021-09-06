#include <Arduino.h>
#include "ServoEasing.h"
#include "SoftwareSerial.h"
#include <DFMiniMp3.h>
#include <Wire.h>
#include "RTClib.h"

#define SERVO1_PIN 9
#define INFO // to see serial output of loop

class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

ServoEasing Servo1;
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);
long randNumber;
const int SOUND_PIN = 4;
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void printDetail(uint8_t type, int value);

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}

void setup() {
    mySoftwareSerial.begin(9600);
    randomSeed(analogRead(0));
    Serial.begin(115200);
    mp3.begin();
    pinMode(SOUND_PIN, INPUT_PULLUP);
    Serial.println();
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
    uint16_t volume = mp3.getVolume();
    Serial.print("volume ");
    Serial.println(volume);
    mp3.setVolume(15);
    uint16_t count = mp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    Serial.print("files ");
    Serial.println(count);
    Serial.println("starting...");
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_SERVO_EASING));
    Serial.print(F("Attach servo at pin "));
    Serial.println(SERVO1_PIN);
    if (Servo1.attach(SERVO1_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching servo"));
    }
     Servo1.write(90);
    // Wait for servo to reach start position.
    delay(500);
    //******************* RTC part
    #ifndef ESP8266
      while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      abort();
    }
    if (! rtc.initialized() || rtc.lostPower()) {
      Serial.println("RTC is NOT initialized, let's set the time!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    rtc.start();
    float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
    float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
    float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
    float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
    int offset = round(deviation_ppm / drift_unit);
    Serial.print("Offset is "); Serial.println(offset); // Print to control offset
}

void loop() 
{
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    int soundswitch = digitalRead(SOUND_PIN);
    if (now.hour() > 8 && now.hour() < 22 && now.minute() != 0 && now.second() == 0 && soundswitch == HIGH){
       randomSeed(analogRead(0));
       randNumber = random(5);
       mp3.playMp3FolderTrack(randNumber);
       Serial.print("track "); 
       Serial.println(randNumber); 
       #ifdef INFO
       Serial.println(F("Move to 180 degree and back nonlinear in one second each using interrupts"));
       #endif
       Servo1.setEasingType(EASE_CUBIC_IN_OUT);
       for (int i = 0; i < 8; ++i) {
       #ifdef ENABLE_MICROS_AS_DEGREE_PARAMETER
         Servo1.startEaseToD((544 + (((2400 - 544) / 4) * 3)), 1000);
       #else
         Servo1.startEaseToD(180, 1000); 
       #endif
       while (ServoEasing::areInterruptsActive()) {
       }
       #ifdef ENABLE_MICROS_AS_DEGREE_PARAMETER
           Servo1.startEaseToD((544 + (((2400 - 544) / 4) * 3)), 1000);
       #else
           Servo1.startEaseToD(0, 1000);
       #endif
          while (ServoEasing::areInterruptsActive()) {
       }
       }
       Servo1.setEasingType(EASE_LINEAR);
       Servo1.write(90);
    }
    delay(300);
}
