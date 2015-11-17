/* Sketch for 3 RGB LED proof of concept of an LED clock mirror
 * Created by Andy Waplinger, 2015
 * http://andywaplinger.com/
 * https://github.com/andywaplinger/ledclock01
 */

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

const int hourB =  2;      // hour blue
const int hourG =  3;      // hour green
const int hourR =  4;      // hour red
const int quartB =  5;      // 15 min blue
const int quartG =  6;      // 15 min green
const int quartR =  7;      // 15 min red
const int minB =  8;      // min blue
const int minG =  9;      // min green
const int minR =  10;      // min red

const int motionPin = 0;     // the cell and 10K pulldown are connected to a0
int motionReading;     // the analog reading from the sensor divider



void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(35,59,23,7,17,10,15);
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(
  byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

void displayHour()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  pinMode(hourB, OUTPUT);
  pinMode(hourG, OUTPUT);
  pinMode(hourR, OUTPUT);
  
  if (hour >= 0 && hour < 10){
    digitalWrite(hourB, HIGH);
    digitalWrite(hourG, LOW);
    digitalWrite(hourR, LOW);
  }
  else if (hour >= 10 && hour < 18){
    digitalWrite(hourB, HIGH);
    digitalWrite(hourG, HIGH);
    digitalWrite(hourR, LOW);
  }
  else if (hour >= 18){
    digitalWrite(hourB, HIGH);
    digitalWrite(hourG, HIGH);
    digitalWrite(hourR, HIGH);
  }
}

void displayQuart(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  pinMode(quartB, OUTPUT);
  pinMode(quartG, OUTPUT);
  pinMode(quartR, OUTPUT);

  if (minute >= 45){
    digitalWrite(quartB, HIGH);
    digitalWrite(quartG, HIGH);
    digitalWrite(quartR, HIGH);
  }
  else if (minute >= 30){
    digitalWrite(quartB, HIGH);
    digitalWrite(quartG, HIGH);
    digitalWrite(quartR, LOW);
  }
  else if (minute >= 15){
    digitalWrite(quartB, HIGH);
    digitalWrite(quartG, LOW);
    digitalWrite(quartR, LOW);
  }
  else {
    digitalWrite(quartB, LOW);
    digitalWrite(quartG, LOW);
    digitalWrite(quartR, LOW);
  }
}

void displayMin()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  pinMode(minB, OUTPUT);
  pinMode(minG, OUTPUT);
  pinMode(minR, OUTPUT);

  if (minute >= 0 && minute < 10){
    digitalWrite(minB, HIGH);
    digitalWrite(minG, LOW);
    digitalWrite(minR, LOW);
  }
  if (minute >= 10 && minute < 30){
    digitalWrite(minB, HIGH);
    digitalWrite(minG, HIGH);
    digitalWrite(minR, LOW);
  }
  if (minute >= 30){
    digitalWrite(minB, HIGH);
    digitalWrite(minG, HIGH);
    digitalWrite(minR, HIGH);
  }
}

void displayOff()
{
  digitalWrite(hourB, LOW);
  digitalWrite(hourG, LOW);
  digitalWrite(hourR, LOW);

  digitalWrite(quartB, LOW);
  digitalWrite(quartG, LOW);
  digitalWrite(quartR, LOW);

  digitalWrite(minB, LOW);
  digitalWrite(minG, LOW);
  digitalWrite(minR, LOW);
}

void loop()
{
  motionReading = analogRead(motionPin);
  
  if (motionReading > 75){
    displayOff();
    Serial.print("Too bright");
    Serial.print('\n');
  }
  else {
    displayHour();
    displayQuart();
    displayMin();
  }
  displayTime(); // display the real-time clock data on the Serial Monitor
  Serial.print(motionReading);
  Serial.print('\n');
  delay(200); // every second
}
