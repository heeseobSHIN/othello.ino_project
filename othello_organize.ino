#include <Adafruit_NeoPixel.h>
#include <LedControl.h>

int UD = 0, LR = 0, BT = 0, nLED, xdot, ydot, retval;
int UD_1 = 0, LR_1 = 0, BT_1 = 0;
int OTH[8][8], BKP[8][8], i, j, k, Turn = 0, rScore, gScore, count;
boolean flag = false;
bool dataState = false;


#define PIN 3      // Output pin for led strip
#define NUMPIXELS 64 // Number of LED Diodes
#define SPEED 250

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define CLK A2
#define DIO 11

//TM1637Display display(CLK, DIO);

void BlowLED() {
  rScore = 0;
  gScore = 0;
  flag = true;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      nLED = 8 * i + j;
      if (OTH[i][j] == 0) { //RED
        strip.setPixelColor(nLED, strip.Color(30, 0, 30));
        rScore = rScore + 1;
      }
      if (OTH[i][j] == 1) { //GREEN
        strip.setPixelColor(nLED, strip.Color(0, 30, 30));
        gScore = gScore + 1;
      }
      if (OTH[i][j] == 2) { //NO COLOR
        strip.setPixelColor(nLED, strip.Color(0, 0, 0));
        flag = false;
      }
    }
  }
  strip.show();
  //led0이면 빨강, 1이면 초록, 2면 없음
  //Display SCORES on TM1637
  //  display.showNumberDecEx(rScore * 100 + gScore, 64, true);

  //Full Board Case, 판이 다찼을때
  if (flag == true) {

    for (int n = 0; n < 150; n++) {
      for (int v = 0; v < 64; v++) {
        strip.setPixelColor(v, 20, 20, 20);
      }

      strip.setBrightness(n);
      strip.show();
    }

    for (int n = 150; n >= 0; n--) {
      for (int v = 0; v < 64; v++) {
        strip.setPixelColor(v, 20, 20, 20);
      }
      strip.setBrightness(n);
      strip.show();

    }
  }
}

int valid()
{
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
      BKP[i][j] = OTH[i][j];

  retval = 0;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (OTH[i][j] == Turn ) {

        Serial.println("////");
        Serial.println(Turn);
        Serial.println("====");
        Serial.println(i);
        Serial.println(j);
        Serial.println("====");
        Serial.println(xdot);
        Serial.println(ydot);

        //Serial.println();
        //delay(10 * SPEED);

        if (abs(j - ydot) == abs(i - xdot)) { //대각선 abs는 절댓값 계산
          if (i < xdot && j < ydot) {
            count = 0;
            for (k = i + 1; k < xdot; k++) {
              if (OTH[k][j + k - i] == 1 - Turn) count += 1;
              Serial.println("11시 방향");
            }
            if (count + 1 == xdot - i && count > 0) {
              for (k = i + 1; k < xdot; k++) {
                BKP[k][j + k - i] =  Turn;
              }
              retval = 1;
            }
          }

          // 찍는 점의 1시방향
          if (i > xdot && j < ydot) {
            count = 0;
            for (k = xdot + 1; k < i; k++) {
              if (OTH[k][j + k - xdot] == 1 - Turn) count += 1;
              Serial.println("1시 방향");
            }
            if (count + 1 == i - xdot && count > 0) {
              for (k = xdot + 1; k < i; k++) {
                BKP[k][j + k - xdot] = Turn;
              }
              retval = 1;
            }
          }

          // 찍은 곳 7시
          if (i < xdot && j > ydot) {
            count = 0;
            for (k = i + 1; k < xdot; k++) {
              if (OTH[k][ydot + k - i] == 1 - Turn) count += 1;
              Serial.println("7시 방향");
            }
            if (count + 1 == xdot - i  && count > 0) {
              for (k = i + 1; k < xdot; k++) {
                BKP[k][ydot + k - i] =  Turn;
              }
              retval = 1;
            }
          }

          // 찍은 곳 5시
          if (i > xdot && j > ydot) {
            count = 0;
            for (k = xdot + 1; k < i; k++) {
              if (OTH[k][ydot + k - xdot] == 1 - Turn) count += 1;
              Serial.println("5시 방향");
            }
            if (count + 1 == i - xdot && count > 0) {
              for (k = xdot + 1; k < i; k++) {
                BKP[k][ydot + k - xdot] = Turn;
              }
              retval = 1;
            }
          }
        }
        /////////////////////////////////////////////////////////////////////////////////////
        if (j == ydot) { //같은 x축 9시 방향
          if (i < xdot) {
            count = 0;
            for (k = i + 1; k < xdot; k++) {
              if (OTH[k][ydot] == 1 - Turn) count += 1;
              Serial.println("9시 방향");
            }
            if (count + 1 == xdot - i  && count > 0) {
              for (k = i + 1; k < xdot; k++) {
                BKP[k][ydot] =  Turn;
              }
              retval = 1;
            }
          }

          if (i > xdot) {// x축 3시 방향
            count = 0;
            for (k = xdot + 1; k < i; k++) {
              if (OTH[k][ydot] == 1 - Turn) count += 1;
              Serial.println("3시 방향");
            }
            if (count + 1 == i - xdot && count > 0) {
              for (k = xdot + 1; k < i; k++) {
                BKP[k][ydot] = Turn;
              }
              retval = 1;
            }
          }
        }
        /////////////////////////////////////////////////////////////////////////////////////
        if (i == xdot) { //같은 y축 12시 방향
          if (j < ydot) {
            count = 0;
            for (k = j + 1; k < ydot; k++) {
              if (OTH[xdot][k] == 1 - Turn) count += 1;
              Serial.println("12시 방향");
            }
            if (count + 1 == ydot - j && count > 0) {
              for (k = j + 1; k < ydot; k++) {
                BKP[xdot][k] =  Turn;
              }
              retval = 1;
            }
          }

          if (j > ydot) { // y축 6시 방향
            count = 0;
            for (k = ydot + 1; k < j; k++) {
              if (OTH[xdot][k] == 1 - Turn) count += 1;
              Serial.println("6시 방향");
            }
            if (count + 1 == j - ydot && count > 0) {
              for (k = ydot + 1; k < j; k++) {
                BKP[xdot][k] =  Turn;
              }
              retval = 1;
            }
          }
        }

      }
    }
  }


  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
      OTH[i][j] = BKP[i][j];

  return retval;
}

void setup()
{
  Serial.begin(9600); //Sets the baud for serial data transmission
  strip.begin();
  strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'
  //  display.setBrightness(2);

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
      OTH[i][j] = 2; //No Color

  OTH[3][3] = 1;
  OTH[3][4] = 0;
  OTH[4][3] = 0;
  OTH[4][4] = 1;

  BlowLED();
}

void loop() {
  unsigned long interval = 500; // the time we need to wait
  unsigned long previousMillis = 0;


  long start_time = millis() ;
  long wait = millis() ;


  if (1 - Turn == 0 ) {
    UD = analogRead(A9);
    LR = analogRead(A10);
    BT = analogRead(A11);
    runothello();
  }
  else {

    UD = analogRead(A0);
    LR = analogRead(A1);
    BT = analogRead(A2);

    runothello();
  }

  //  Serial.println(rScore);
  //  Serial.println(gScore);
}


void runothello() {
  if ((unsigned long)(millis() + previousMillis) >= 500) {
    previousMillis = millis();

    xdot = map(LR, 0, 1021, 0, 7); //This maps the values//
    ydot = map(UD, 0, 1021, 0, 7);

    nLED = 8 * xdot + ydot;
    Serial.print("LR ");
    Serial.println(LR);
    Serial.println(" ");
    Serial.print("UD ");
    Serial.println(UD);

    strip.setPixelColor(nLED, strip.Color(128, 128, 255)); //Yellow
    if (Turn == 0) {
      strip.setPixelColor(nLED, strip.Color(50  , 20 , 5 ));
    } else {
      strip.setPixelColor(nLED, strip.Color(0  , 50 , 50 ));
    }
    strip.show();

    if (BT == 0 && OTH[xdot][ydot] == 2) {
      if (valid() == 1) {
        OTH[xdot][ydot] = Turn;
        Turn = 1 - Turn;
      }
    }

    //delay(SPEED);
    BlowLED();
  }

}
