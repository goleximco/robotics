
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>       
#include <DMD.h>    
#include <TimerOne.h>  
#include "Arial_black_16.h"
#include "Font_6x14.h" //-> This font only contains numbers from 0-9. Used only to display time.
#define DISPLAYS_ACROSS 1 //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

RTC_DS1307 rtc; //-> RTC Declaration

int _day, _month, _year, _hour24, _hour12, _minute, _second, _dtw;
int hr24;
String st;
char nameoftheday[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char month_name[12][12] = {"January","February", "March", "April", "May", "June", "Jult", "August", "September", "October", "November", "December"};
const long interval = 1000; //-> Retrieve time and date data every 1 second
unsigned long previousMillis = 0;
char hr_24 [3];
String str_hr_24;
char mn [3];
String str_mn;
void ScanDMD() { 
  dmd.scanDisplayBySPI();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Arduino RTC DS1307");
  delay(1000);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> Set the time and date based on your computer time and date. If that doesn't work, use this line of code outside of "if (! rtc.isrunning())"
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> Set the time and date based on your computer time and date. Use this line of code if it doesn't work in "if (! rtc.isrunning())"
  //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  // If the time and date are successfully set, then deactivate the code line (make the code a comment), then re-upload the code.
  // if not done then the time and date will return to the beginning when it was set when arduino is reset or restarted.

  Timer1.initialize(1000);          
  Timer1.attachInterrupt(ScanDMD);   
  dmd.clearScreen(true);  
}

void loop() {
 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; //-> save the last time

    GetDateTime(); //-> Retrieve time and date data from DS1307
    
    dmd.selectFont(Font_6x14);

    str_hr_24=String(_hour24);
    str_hr_24.toCharArray(hr_24,3);

    if (_hour24<10) {
      dmd.drawString(0, 0, "0", 1, GRAPHICS_NORMAL);
      dmd.drawString(7, 0, hr_24, 1, GRAPHICS_NORMAL);
    }
    else {
      dmd.drawString(0, 0, hr_24, 2, GRAPHICS_NORMAL);
    }
    
    if (_second %2 == 0) {
      dmd.drawFilledBox(15,3,16,4, GRAPHICS_OR);
      dmd.drawFilledBox(15,11,16,12, GRAPHICS_OR);
    }
    else {
      dmd.drawFilledBox(15,3,16,4, GRAPHICS_NOR);
      dmd.drawFilledBox(15,11,16,12, GRAPHICS_NOR);
    }
   
    str_mn=String(_minute);
    str_mn.toCharArray(mn,3);

    if (_minute<10) {
      dmd.drawString(19, 0, "0", 1, GRAPHICS_NORMAL);
      dmd.drawString(26, 0, mn, 1, GRAPHICS_NORMAL);
    }
    else {
      dmd.drawString(19, 0, mn, 2, GRAPHICS_NORMAL);
    }
    

    if (_second==11) { //-> Display the date when seconds equal to 11
      scrolling_date();
    }
    
  }
  
}



void GetDateTime() {
  DateTime now = rtc.now();
  _day=now.day();
  _month=now.month();
  _year=now.year();
  _hour24=now.hour();
  _minute=now.minute();
  _second=now.second();
  _dtw=now.dayOfTheWeek();

  hr24=_hour24;
  if (hr24>12) {
    _hour12=hr24-12;
  }
  else if (hr24==0) {
    _hour12=12;
  }
  else {
    _hour12=hr24;
  }

  if (hr24<12) {
    st="AM";
  }
  else {
    st="PM"; 
  }  
}

void scrolling_date() {
  String Date = String(nameoftheday[_dtw]) + ", " + String(_day) + "-" + String(month_name[_month-1]) + "-" + String(_year);
  char dt[50];
  Date.toCharArray(dt,50);
  
  dmd.clearScreen(true); 
  dmd.selectFont(Arial_Black_16);
  dmd.drawMarquee(dt,strlen(dt),(32*DISPLAYS_ACROSS)-1,0);
  boolean ret=false;
  while(!ret){
    ret=dmd.stepMarquee(-1,0);
    delay(75); //-> //-> delay For scroll speed
  }
  dmd.clearScreen(true);
  return;
}
