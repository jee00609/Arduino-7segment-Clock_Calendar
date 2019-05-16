#include <SoftwareSerial.h>

SoftwareSerial Serial7Segment(7, 8); //RX pin, TX pin

#define APOSTROPHE  5
#define COLON       4
#define DECIMAL4    3
#define DECIMAL3    2
#define DECIMAL2    1
#define DECIMAL1    0

const int _a = 2;
const int _b = 3;
const int _c = 4;
const int _d = 5;
const int _e = 6;
const int _f = 14;
const int _g = 15;
const int _dot = 9;

int dig1 = 10;
int dig2 = 11;
int dig3 = 12;

int days = 0;
int month = 0;
int day_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int Fnd_seg[10][8] = {
  {1, 0, 0, 0, 1, 0, 0, 0}, //0
  {1, 1, 1, 1, 1, 0, 0, 1}, //1
  {0, 1, 0, 0, 1, 1, 0, 0}, //2
  {0, 1, 1, 0, 1, 0, 0, 0}, //3
  {0, 0, 1, 1, 1, 0, 0, 1}, //4
  {0, 0, 1, 0, 1, 0, 1, 0}, //5
  {0, 0, 0, 0, 1, 0, 1, 0}, //6
  {1, 1, 1, 1, 1, 0, 0, 0}, //7
  {0, 0, 0, 0, 1, 0, 0, 0}, //8
  {0, 0, 1, 0, 1, 0, 0, 0}, //9
};

long millisTimer;

int seconds = 0;
int minutes = 0;
int hours = 0;
boolean amTime = true;
boolean colonOn = false;

char buffer[20];
char bufferIndex = 0;

char tempString[100]; //Used for sprintf

int d1 = 0;
int d2 = 0;
int d3 = 0;
int d4 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("OpenSegment Example Code");

  Serial7Segment.begin(9600); //Talk to the Serial7Segment at 9600 bps
  Serial7Segment.write('v'); //Reset the display - this forces the cursor to return to the beginning of the display
  Serial7Segment.print("2359"); //Send the hour and minutes to the display

  millisTimer = millis();
  //For testing, we initialize the variables to the current time
  seconds = 59;
  minutes = 58;
  hours = 23;

  // put your setup code here, to run once:
  pinMode(_a, OUTPUT);
  pinMode(_b, OUTPUT);
  pinMode(_c, OUTPUT);
  pinMode(_d, OUTPUT);
  pinMode(_e, OUTPUT);
  pinMode(_f, OUTPUT);
  pinMode(_g, OUTPUT);
  pinMode(_dot, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);

  month = 9;
  days = 30;

}

void dig1_Find_Write(int row) {
  int seg_Pin = 2;

  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  for (int segPos = 0; segPos < 8; segPos++) {
    digitalWrite(seg_Pin, Fnd_seg[row][segPos]);
    seg_Pin++;
    if (seg_Pin == 7)seg_Pin = 14;
    if (seg_Pin == 16)seg_Pin = 9;
  }
}

void dig2_Find_Write(int row) {
  int seg_Pin = 2;

  digitalWrite(dig1, LOW);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, LOW);
  for (int segPos = 0; segPos < 8; segPos++) {
    digitalWrite(seg_Pin, Fnd_seg[row][segPos]);
    seg_Pin++;
    if (seg_Pin == 7)seg_Pin = 14;
    if (seg_Pin == 16)seg_Pin = 9;
  }
}

void dig3_Find_Write(int row) {
  int seg_Pin = 2;

  digitalWrite(dig1, LOW);
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, HIGH);
  for (int segPos = 0; segPos < 8; segPos++) {
    if (segPos == 4) digitalWrite(seg_Pin, LOW);
    digitalWrite(seg_Pin, Fnd_seg[row][segPos]);
    seg_Pin++;
    if (seg_Pin == 7)seg_Pin = 14;
    if (seg_Pin == 16)seg_Pin = 9;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    buffer[bufferIndex]  = Serial.read();   //시리얼 통신으로 버퍼배열에 데이터 수신
    bufferIndex++;                          //데이터 수신 후 버퍼 인덱스 1 증가
  }

  double pis = atof(buffer);

  double pos = atof(buffer);                   //atoi()함수로 char값을 int값으로 변환

  int save = atoi(buffer);

  Serial7Segment.write(0x77);  // Decimal, colon, apostrophe control command
  Serial7Segment.write( (1 << APOSTROPHE) | (1 << COLON) | (1 << DECIMAL2) ); // Turns on colon, apostrophoe, and far-right decimal

  sprintf(tempString, "%02d%02d", hours, minutes);
  Serial7Segment.print(tempString); //Send serial string out the soft serial port to the S7S

  if ( (millis() - millisTimer) > 1000)
  {
    //Serial.println(millis() - millisTimer);
    millisTimer += 1000; //Adjust the timer forward 1 second

    seconds++;
    if (seconds > 59)
    {
      seconds -= 60; //Reset seconds and increment minutes
      minutes++;
      if (minutes > 59)
      {
        minutes -= 60; //Reset minutes and increment hours
        hours++;
        if (hours > 23)
        {
          hours -= 24; //Reset hours and flip AM/PM
        }
      }
    }
  }



  //Serial.print(pis);

  if (buffer[0] == '1') {
    if (pos != 0) {

      Serial.print("Input data : ");
      Serial.println(save);                    //int값으로 변환된 데이터 출력

      hours = save - 100;
      pis = ((pis - save) * 100);
      Serial.print("pis = ");
      Serial.println(pis);
      minutes = (int)pis;

      if (pis != minutes) {
        Serial.println("fuck");
        minutes = ((int)pis) + 1;
        if (minutes == 60) {
          minutes = minutes - 1;
        }
      }
      //    minutes = (int)pis;
      Serial.print("Input data2 : ");
      Serial.println(pis);
      Serial.println(minutes);
    }
  }

  if (buffer[0] == '2') {
    if (pos != 0) {

      Serial.print("Input data: ");
      Serial.println(save);

      month = save - 200;

      pis = (pis - save) * 100;

      dtostrf(pis, 4, 2, tempString);
      Serial.println(tempString);

      days = atoi(tempString);

      Serial.print("Input data2 : ");
      Serial.println(pis);
      Serial.println(days);
    }
  }

  if (hours == 0 && minutes == 0 && seconds == 0) {
    days += 1;
    if (days > day_of_month[month - 1]) {
      days = 1;
      month += 1;
    }
  }

  if (month > 12) {
    days = 1;
    month = 1;
  }

  d4 = month / 10;
  d3 = month % 10;
  d2 = days / 10;
  d1 = days % 10;

  for (int a = 0; a < 21; a++) {
    buffer[a] = NULL;
  }
  delay(10);
  //Debug print the time

  sprintf(tempString, "HH:MM:SS %02d:%02d:%02d", hours, minutes, seconds);
  // sprintf(tempString, "%02d%02d", hours, minutes);

  //  Serial7Segment.print(tempString); //Send serial string out the soft serial port to the S7S
  Serial.println(tempString);
  sprintf(tempString, "DD:YY %02d.%02d", month, days);
  Serial.println(tempString);


  if (month < 10) {
    for (int count = 0; count < 5; count++) {
      for (int i = 0; i < 10; i++) {
        dig3_Find_Write(d3);
        digitalWrite(6, LOW);
        delay(2);
        digitalWrite(6, HIGH);
        dig2_Find_Write(d2);
        delay(2);
        dig1_Find_Write(d1);
        delay(2);
      }
    }
  }

  Serial.print("Month ==");
  Serial.println(month);
  Serial.print("Days == ");
  Serial.println(days);

  if (month >= 10 && month <= 12) {
    for (int count = 0; count < 3; count++) {
      for (int i = 0; i < 20; i++) {
        dig3_Find_Write(d4);
        delay(2);
        dig2_Find_Write(d3);
        digitalWrite(6, LOW);
        delay(2);
        digitalWrite(6, HIGH);
        dig1_Find_Write(d2);
        delay(2);
      }

      for (int i = 0; i < 20; i++) {
        dig3_Find_Write(d3);
        digitalWrite(6, LOW);
        delay(2);
        digitalWrite(6, HIGH);
        dig2_Find_Write(d2);
        delay(2);
        dig1_Find_Write(d1);
        delay(2);
      }

      for (int i = 0; i < 20; i++) {
        dig3_Find_Write(d2);
        delay(2);
        dig2_Find_Write(d1);
        delay(2);
        dig1_Find_Write(d4);
        delay(2);
      }

      for (int i = 0; i < 20; i++) {
        dig3_Find_Write(d1);
        delay(2);
        dig2_Find_Write(d4);
        delay(2);
        dig1_Find_Write(d3);
        digitalWrite(6, LOW);
        delay(2);
        digitalWrite(6, HIGH);
      }

    }
  }

  sprintf(tempString, "%02d%02d", hours, minutes);
  Serial7Segment.print(tempString); //Send serial string out the soft serial port to the S7S

}
