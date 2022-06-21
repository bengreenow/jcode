// SET LIBRARIES
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <EEPROM.h>
//#include <MIDI.h>
// MIDI_CREATE_DEFAULT_INSTANCE();

// SET FONTS
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <FreeDefaultFonts.h>

#define FO9 &FreeSans9pt7b
#define BO9 &FreeSansBold9pt7b
#define FO12 &FreeSans12pt7b
#define BO12 &FreeSansBold12pt7b
#define FO18 &FreeSans18pt7b
#define BO18 &FreeSansBold18pt7b
#define DIG7 &FreeSevenSegNumFont

// SET COLOURS & PARAMS
#define BLK 0x0000
#define WHT 0xFFFF
#define RED 0xF800
#define GRN 0x07E0
#define BLU 0x001F
#define YEL 0xFFE0
#define CYN 0x07FF
#define MAG 0xF81F
#define SCR 0x10A2
#define BUT 0x2945
#define FRM 0x632C

#define RAD 8

// SET SCREEN
MCUFRIEND_kbv disp;

// SET TOUCH PANEL
#define MINPRESSURE 100
#define MAXPRESSURE 1000
#define BOOP 100
#define WAIT 150
#define MSG 2000
#define BNC 300

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 895
#define TS_MAXX 129
#define TS_MINY 71
#define TS_MAXY 907

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// press state
bool pressed(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t px, int16_t py)
{
  if ((px > x1 && px < x2) && (py > y1 && py < y2))
  {
    return true;
  }
  else
  {
    return false;
  }
}

// SET BUTTON ARRAYS

// button perams: |label|start X|start Y|width X|height Y|end X|end Y|font|text colour|cursor X|cursor Y|

typedef struct button
{
  char butLab[16];
  uint16_t topX;
  uint16_t topY;
  uint16_t sizX;
  uint16_t sizY;
  uint16_t botX;
  uint16_t botY;
  uint16_t butFont;
  uint16_t texCol;
  int16_t curX;
  int16_t curY;
} button;

// button list:

button butSet[30] =
    {
        // label,            start X,  start Y,  size X, size Y, end X,  end Y,  font, text colour,  offset X, offset Y, index

        "0", 84, 259, 72, 55, 155, 313, BO18, FRM, 0, 0,    // 0
        "1", 6, 76, 72, 55, 77, 130, BO18, FRM, -4, 0,      // 1
        "2", 84, 76, 72, 55, 155, 130, BO18, FRM, 0, 0,     // 2
        "3", 162, 76, 72, 55, 233, 130, BO18, FRM, 0, 0,    // 3
        "4", 6, 137, 72, 55, 77, 191, BO18, FRM, -2, 0,     // 4
        "5", 84, 137, 72, 55, 155, 191, BO18, FRM, 0, 0,    // 5
        "6", 162, 137, 72, 55, 233, 191, BO18, FRM, 0, 0,   // 6
        "7", 6, 198, 72, 55, 77, 252, BO18, FRM, 0, 0,      // 7
        "8", 84, 198, 72, 55, 155, 252, BO18, FRM, 0, 0,    // 8
        "9", 162, 198, 72, 55, 233, 252, BO18, FRM, 0, 0,   // 9
        ".", 162, 259, 72, 55, 233, 313, BO18, FRM, 0, +10, // 10
        "ESC", 6, 259, 72, 55, 77, 313, BO9, RED, 0, 0,     // 11
        "ENT", 162, 6, 72, 64, 233, 69, BO9, GRN, 0, 0,     // 12

        "Patch Recall", 10, 110, 220, 60, 229, 169, BO12, WHT, 0, 0,   // 13
        "MIDI Settings", 10, 180, 220, 60, 229, 229, BO12, WHT, 0, -2, // 14
        "LED Settings", 10, 250, 220, 60, 229, 309, BO12, WHT, 0, -2,  // 15

        "Switch A", 10, 110, 105, 60, 114, 169, BO9, WHT, 0, 0,  // 16
        "Switch B", 125, 110, 105, 60, 229, 169, BO9, WHT, 0, 0, // 17
        "Switch C", 10, 180, 105, 60, 114, 239, BO9, WHT, 0, 0,  // 18
        "Switch D", 125, 180, 105, 60, 229, 239, BO9, WHT, 0, 0, // 19
        "Tx Chan", 10, 250, 105, 60, 114, 309, BO9, WHT, 0, 0,   // 20
        "Exit", 125, 250, 105, 60, 229, 309, BO9, WHT, 0, 0,     // 21

        "LED A", 10, 110, 105, 60, 114, 169, BO9, WHT, 0, 0,  // 22
        "LED B", 125, 110, 105, 60, 229, 169, BO9, WHT, 0, 0, // 23
        "LED C", 10, 180, 105, 60, 114, 239, BO9, WHT, 0, 0,  // 24
        "LED D", 125, 180, 105, 60, 229, 239, BO9, WHT, 0, 0, // 25
        "On/Off", 10, 250, 105, 60, 114, 309, BO9, WHT, 0, 0, // 26
        "Exit", 125, 250, 105, 60, 229, 309, BO9, WHT, 0, 0,  // 27

        "ESC", 6, 6, 72, 64, 77, 69, BO9, RED, 0, 0,    // 28
        "SET", 162, 6, 72, 64, 233, 69, BO9, GRN, 0, 0, // 29
};

// SET GLOBALS
uint16_t i;
uint8_t n = 0;
char currentPage;
boolean ledChoice = false;

uint16_t lftX = 18, ctrX = 60, rgtX = 102, digY = 64;
uint16_t setL = 47, setC = 71, setR = 95, setY = 60;
uint8_t lftDig, ctrDig, rgtDig;

int16_t strX, strY;
uint16_t strW, strH;

byte midSet = 0;
byte ledSet = 0;
byte redFad, grnFad, bluFad;

// SET MEMORY INDEX
byte midChan, ccA, ccB, ccC, ccD;
byte redOffA, grnOffA, bluOffA;
byte redOffB, grnOffB, bluOffB;
byte redOffC, grnOffC, bluOffC;
byte redOffD, grnOffD, bluOffD;
byte redOnA, grnOnA, bluOnA;
byte redOnB, grnOnB, bluOnB;
byte redOnC, grnOnC, bluOnC;
byte redOnD, grnOnD, bluOnD;

byte *memSet[29]{
    &midChan, &ccA, &ccB, &ccC, &ccD,
    &redOffA, &grnOffA, &bluOffA,
    &redOffB, &grnOffB, &bluOffB,
    &redOffC, &grnOffC, &bluOffC,
    &redOffD, &grnOffD, &bluOffD,
    &redOnA, &grnOnA, &bluOnA,
    &redOnB, &grnOnB, &bluOnB,
    &redOnC, &grnOnC, &bluOnC,
    &redOnD, &grnOnD, &bluOnD};

void setup()
{

  // Setup the LCD
  Serial.begin(9600);
  disp.begin(0x9341);
  disp.setRotation(0);
  disp.fillScreen(BLK);

  // MIDI.begin(MIDI_CHANNEL_OMNI);

  disp.setFont(BO12);
  disp.getTextBounds("LOADING", 0, 0, &strX, &strY, &strW, &strH);
  wordUp("LOADING...", (240 - strW) / 2, 50, BO12, MAG);

  // load picture
  disp.drawRect(5, 305, 230, 10, WHT);

  // coming soon - pinmodes & hardware settings

  for (i = 0; i < 29; i++)
  {
    *memSet[i] = EEPROM.read(i);
  }

  // coming soon - report MIDI settings & hold for visual

  for (i = 0; i < 228; i++)
  {
    disp.fillRect(i + 6, 306, 1, 8, MAG);
    delay(15);
  }

  drawHomeScreen();
  currentPage = '0';
}

void loop()
{

  ////////---controller functions---/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //  if (digitalRead(??) == LOW) {
  //    MIDI.sendControlChange(127,ccA,midChan);
  //    delay(BNC);
  //    }
  //
  //  if (digitalRead(??) == LOW) {
  //    MIDI.sendControlChange(127,ccB,midChan);
  //    delay(BNC);
  //    }
  //
  //  if (digitalRead(??) == LOW) {
  //    MIDI.sendControlChange(127,ccC,midChan);
  //    delay(BNC);
  //    }
  //
  //  if (digitalRead(??) == LOW) {
  //    MIDI.sendControlChange(127,ccD,midChan);
  //    delay(BNC);
  //    }

  // pressing details
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 239);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 319);

    ////PAGE 0: home navigation select//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (currentPage == '0')
    {

      for (i = 13; i < 16; i++)
      {

        if (pressed(butSet[i].topX, butSet[i].topY, butSet[i].botX, butSet[i].botY, p.x, p.y))
        {

          boopFlash();

          switch (i)
          {

          case 13: // goto caller
            drawCallScreen();
            currentPage = '1';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 14: // goto settings
            drawSelectScreen();
            currentPage = '2';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 15: // goto LEDs
            drawLxScreen();
            currentPage = '4';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          } // switch end

        } // home buttons end

      } // pg0 for scan end

    } // page 0 end

    ////PAGE 1: MIDI caller program////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (currentPage == '1')
    {

      for (i = 0; i < 13; i++)
      {

        if (pressed(butSet[i].topX, butSet[i].topY, butSet[i].botX, butSet[i].botY, p.x, p.y))
        {

          boopFlash();

          //---NUM PAD KEYS---//
          if (i < 10)
          {

            if (n == 0)
            { // press first digit
              wordUp(butSet[i].butLab, ctrX, digY, DIG7, RED);
              ctrDig = i;
              n = 1;
            }

            else if (n == 1)
            { // press second digit
              disp.fillRect(ctrX, digY - 50, 32, 50, SCR);
              wordUp(butSet[ctrDig].butLab, lftX, digY, DIG7, RED);
              lftDig = ctrDig;
              wordUp(butSet[i].butLab, ctrX, digY, DIG7, RED);
              ctrDig = i;
              n = 2;
            }

            else if (n == 2)
            { // press a third bank digit shuffle left
              disp.fillRect(lftX, digY - 50, 74, 50, SCR);
              wordUp(butSet[ctrDig].butLab, lftX, digY, DIG7, RED);
              lftDig = ctrDig;
              wordUp(butSet[i].butLab, ctrX, digY, DIG7, RED);
              ctrDig = i;
            }

            else if (n == 3)
            { // dot is set & type patch digit
              rgtDig = constrain(i, 1, 8);
              wordUp(butSet[rgtDig].butLab, rgtX, digY, DIG7, RED);
              n = 4;
            }

            else if (n == 4)
            { // patch digit type over

              disp.fillRect(rgtX, digY - 50, 32, 50, SCR);
              rgtDig = constrain(i, 1, 8);
              wordUp(butSet[rgtDig].butLab, rgtX, digY, DIG7, RED);
            }
          }

          //---ESCAPE KEY---//
          else if (i == 11)
          {

            if (n < 1)
            { // return home
              drawHomeScreen();
              currentPage = '0';
              p.x = 0;
              p.y = 0;
              delay(WAIT);
            }

            else if (n == 1)
            { // delete a single bank digit
              disp.fillRect(ctrX, digY - 50, 32, 50, SCR);
              ctrDig = 0;
              n = 0;
            }

            else if (n == 2)
            { // delete last digit & reshuffle
              disp.fillRect(lftX, digY - 50, 74, 50, SCR);
              wordUp(butSet[lftDig].butLab, ctrX, digY, DIG7, RED);
              ctrDig = lftDig;
              lftDig = 0;
              n = 1;
            }

            else if (n == 3)
            { // delete all and restart
              disp.fillRect(lftX, digY - 50, 100, 50, SCR);
              lftDig = 0;
              ctrDig = 0;
              n = 0;
            }

            else if (n == 4)
            { // delete patch digit
              disp.fillRect(rgtX, digY - 50, 32, 50, SCR);
              rgtDig = 0;
              n = 3;
            }
          }

          //---DOT SET KEY---//
          else if (i == 10)
          { // no bank error
            if (n == 0)
            {
              wordUp("Enter bank first", 20, 45, FO9, YEL);
              clrMsg();
            }
            else if (n > 0 && n < 3)
            { // add dot and goto patch
              disp.fillCircle(96, 58, 3, RED);
              n = 3;
            }
          }

          //---ENTER KEY---//
          else if (i == 12)
          { // calculate, transmit and clear to reset
            if (n == 4)
            {
              disp.fillRect(lftX, digY - 50, 134, 50, SCR);
              wordUp("Sending...", 48, 44, FO9, GRN);

              uint8_t midBank = (7. / 112.) * ((lftDig * 10) + ctrDig);    // ES-8 bank to MIDI bank converter
              uint8_t bankLin = ((lftDig * 10) + ctrDig) - (midBank * 16); // ES-8 bank line selection restrictor
              uint8_t midPatch = bankLin * 8 + rgtDig - 1;                 // ES-8 patch to PC converter

              // MIDI.sendControlChange(0, midBank, midChan);
              // MIDI.sendProgramChange(midPatch, midChan);

              clrMsg();
              n = 0;
              lftDig = 0;
              ctrDig = 0;
              rgtDig = 0;
            }

          } // enter key end

        } // call num pad end

      } // pg1 for scan end

    } // page 1 end

    ////PAGE 2: MIDI selection ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (currentPage == '2')
    {

      for (i = 16; i < 22; i++)
      {

        if (pressed(butSet[i].topX, butSet[i].topY, butSet[i].botX, butSet[i].botY, p.x, p.y))
        {

          boopFlash();

          switch (i)
          {
          case 16:
            midSet = 1;
            drawCallScreen();
            currentPage = '3';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 17:
            midSet = 2;
            drawCallScreen();
            currentPage = '3';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 18:
            midSet = 3;
            drawCallScreen();
            currentPage = '3';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 19:
            midSet = 4;
            drawCallScreen();
            currentPage = '3';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 20:
            midSet = 5;
            drawCallScreen();
            currentPage = '3';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          case 21: // exit key
            midSet = 0;
            drawHomeScreen();
            currentPage = '0';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;
          }

        } // select panel end

      } // pg2 for scan end

    } // page 2 end

    ////PAGE 3: MIDI setting ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (currentPage == '3')
    {

      for (i = 0; i < 13; i++)
      {

        if (pressed(butSet[i].topX, butSet[i].topY, butSet[i].botX, butSet[i].botY, p.x, p.y))
        {

          boopFlash();

          //---NUM PAD KEYS---//
          if (i < 10)
          {

            if (n == 0)
            { // press first digit
              wordUp(butSet[i].butLab, setR, setY, BO18, WHT);
              rgtDig = i;
              n = 1;
            }

            else if (n == 1)
            { // press second digit
              disp.fillRect(setR, setY - 24, 24, 25, SCR);
              wordUp(butSet[rgtDig].butLab, setC, setY, BO18, WHT);
              ctrDig = rgtDig;
              wordUp(butSet[i].butLab, setR, setY, BO18, WHT);
              rgtDig = i;
              n = 2;
            }

            else if (n == 2)
            { // press a third digit
              disp.fillRect(setC, setY - 24, 48, 25, SCR);
              wordUp(butSet[ctrDig].butLab, setL, setY, BO18, WHT);
              lftDig = ctrDig;
              wordUp(butSet[rgtDig].butLab, setC, setY, BO18, WHT);
              ctrDig = rgtDig;
              wordUp(butSet[i].butLab, setR, setY, BO18, WHT);
              rgtDig = i;
              n = 3;
            }

            else if (n == 3)
            { // press further digit keeps previous three current
              disp.fillRect(setL, setY - 24, 72, 25, SCR);
              wordUp(butSet[ctrDig].butLab, setL, setY, BO18, WHT);
              lftDig = ctrDig;
              wordUp(butSet[rgtDig].butLab, setC, setY, BO18, WHT);
              ctrDig = rgtDig;
              wordUp(butSet[i].butLab, setR, setY, BO18, WHT);
              rgtDig = i;
            }
          }

          //---ESCAPE KEY---//
          else if (i == 11)
          {

            if (n < 1)
            { // return to select
              drawSelectScreen();
              currentPage = '2';
              lftDig = 0;
              ctrDig = 0;
              rgtDig = 0;
              n = 0;
              midSet = 0;
              p.x = 0;
              p.y = 0;
              delay(WAIT);
            }

            else if (n == 1)
            { // delete a single digit
              disp.fillRect(setR, setY - 24, 24, 25, SCR);
              rgtDig = 0;
              n = 0;
            }

            else if (n == 2)
            { // delete last digit & reshuffle to 1
              disp.fillRect(setC, setY - 24, 48, 25, SCR);
              wordUp(butSet[ctrDig].butLab, setR, setY, BO18, WHT);
              rgtDig = ctrDig;
              ctrDig = 0;
              n = 1;
            }

            else if (n == 3)
            { // delete last digit & reshuffle to 2
              disp.fillRect(setL, setY - 24, 72, 25, SCR);
              wordUp(butSet[ctrDig].butLab, setR, setY, BO18, WHT);
              rgtDig = ctrDig;
              wordUp(butSet[lftDig].butLab, setC, setY, BO18, WHT);
              ctrDig = lftDig;
              lftDig = 0;
              n = 2;
            }
          }

          //---ENTER KEY---//
          else if (i == 12)
          { // calculate and store to vari & EEPROM
            if (n > 0)
            {
              wordUp("Storing...", 48, 25, FO9, GRN);
              uint16_t midNum = (lftDig * 100) + (ctrDig * 10) + rgtDig;

              switch (midSet)
              {
              case 5:
                midChan = constrain(midNum, 1, 16);
                EEPROM.write(0, midChan);
                break;

              default:
                *memSet[midSet] = constrain(midNum, 0, 127);
                EEPROM.write(midSet, *memSet[midSet]);
                break;
              }

              // reportCard();
              n = 0;
              lftDig = 0;
              ctrDig = 0;
              rgtDig = 0;
              midSet = 0;
              clrMsg();
              currentPage = '2';
              drawSelectScreen();
              p.x = 0;
              p.y = 0;
              delay(WAIT);
            }

            else
            {
              wordUp("Enter setting first", 20, 45, FO9, YEL);
              clrMsg();
            }

          } // enter key end

        } // settings num pad end

      } // pg3 for scan end

    } // page 3 end

    ////PAGE 4: LED selection ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (currentPage == '4')
    {

      for (i = 22; i < 28; i++)
      {

        if (pressed(butSet[i].topX, butSet[i].topY, butSet[i].botX, butSet[i].botY, p.x, p.y))
        {

          boopFlash();

          byte l;
          if (ledChoice == true)
          {
            l = i - 17;
          }

          else
          {
            l = i - 21;
          }

          switch (i)
          {
          case 26: // on/off state toggle
            ledChoice = !ledChoice;
            disp.setFont(FO9);
            disp.getTextBounds("Selected state:", 0, 0, &strX, &strY, &strW, &strH);
            disp.fillRect(strW + 30, 79, 60, 13, SCR);
            if (ledChoice == true)
            {
              wordUp("ON", strW + 30, 91, FO9, GRN);
            }
            else
            {
              wordUp("OFF", strW + 30, 91, FO9, RED);
            }
            break;

          case 27: // exit to home
            ledSet = 0;
            drawHomeScreen();
            currentPage = '0';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;

          default: // go to fade with LED choice
            ledSet = l * 3 + 2;
            drawFadeScreen();
            currentPage = '5';
            p.x = 0;
            p.y = 0;
            delay(WAIT);
            break;
          }

        } // select panel end

      } // pg2 for scan end

    } // page 4 end

    ////PAGE 5: LED faders ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (currentPage == '5')
    {

      if (pressed(butSet[28].topX, butSet[28].topY, butSet[28].botX, butSet[28].botY, p.x, p.y))
      { // return to LED selection
        i = 28;
        boopFlash();
        ledSet = 0;
        drawLxScreen();
        currentPage = '4';
        p.x = 0;
        p.y = 0;
        delay(WAIT);
      }

      if (pressed(butSet[29].topX, butSet[29].topY, butSet[29].botX, butSet[29].botY, p.x, p.y))
      { // write current colour vals to mem
        i = 29;
        boopFlash();
        *memSet[ledSet] = redFad;
        EEPROM.write(ledSet, redFad);
        *memSet[ledSet + 1] = grnFad;
        EEPROM.write(ledSet + 1, grnFad);
        *memSet[ledSet + 2] = bluFad;
        EEPROM.write(ledSet + 2, bluFad);
      }

      if (p.y > 93 && p.y < 309)
      {
        if (p.x > 42 && p.x < 65)
        { // red fader
          redFad = map(p.y, 308, 94, 0, 255);
          disp.fillRect(43, 94, 22, 215, SCR);
          disp.fillRect(43, p.y, 22, 309 - p.y, RED);
        }

        if (p.x > 108 && p.x < 131)
        { // green fader
          grnFad = map(p.y, 308, 94, 0, 255);
          disp.fillRect(109, 94, 22, 215, SCR);
          disp.fillRect(109, p.y, 22, 309 - p.y, GRN);
        }

        if (p.x > 174 && p.x < 197)
        { // blue fader
          bluFad = map(p.y, 308, 94, 0, 255);
          disp.fillRect(175, 94, 22, 215, SCR);
          disp.fillRect(175, p.y, 22, 309 - p.y, BLU);
        }

        uint16_t ledChk = disp.color565(redFad, grnFad, bluFad); // colour mix dot
        disp.fillRoundRect(88, 6, 64, 64, 32, ledChk);
        disp.drawRoundRect(88, 6, 64, 64, 32, FRM);

      } // slide end

    } // page 5 end

  } // press end

} // loop end

// ====== Custom Functions ======

// draw home screen ----------------------------------------------------------------------------------------------------
void drawHomeScreen()
{

  disp.fillScreen(BLK);

  disp.fillRect(10, 10, 220, 90, SCR);
  disp.drawRect(10, 10, 220, 90, FRM);

  for (i = 13; i < 16; i++)
  {
    disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
    disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
    disp.setFont(butSet[i].butFont);
    disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
    wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);
  }
}

// draw MIDI caller screen ----------------------------------------------------------------------------------------------------
void drawCallScreen()
{

  disp.fillScreen(BLK);

  disp.fillRect(6, 6, 150, 64, SCR); // num screen
  disp.drawRect(6, 6, 150, 64, FRM); // frame screen

  lftDig = 0;
  ctrDig = 0;
  rgtDig = 0;
  n = 0;

  for (i = 0; i < 13; i++)
  {
    disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
    disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
    disp.setFont(butSet[i].butFont);
    disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
    wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);
  }
}

// draw MIDI settings menu ----------------------------------------------------------------------------------------------------
void drawSelectScreen()
{

  disp.fillScreen(BLK);

  disp.fillRect(10, 10, 220, 90, SCR);
  disp.drawRect(10, 10, 220, 90, FRM);

  disp.setFont(FO9);
  disp.getTextBounds("Choose setting to change:", 0, 0, &strX, &strY, &strW, &strH);
  wordUp("Choose setting to change:", (240 - strW) / 2, 29, FO9, GRN);
  wordUp("A CC#:", 19, 51, FO9, YEL);
  wordUp("B CC#:", 129, 51, FO9, YEL);
  wordUp("C CC#:", 19, 71, FO9, YEL);
  wordUp("D CC#:", 129, 71, FO9, YEL);
  wordUp("MIDI Tx Channel:", 19, 91, FO9, CYN);
  disp.getTextBounds("A CC#:", 0, 0, &strX, &strY, &strW, &strH);
  valUp(ccA, 19 + (strW + 7), 51, FO9, YEL);
  valUp(ccB, 129 + (strW + 7), 51, FO9, YEL);
  valUp(ccC, 19 + (strW + 7), 71, FO9, YEL);
  valUp(ccD, 129 + (strW + 7), 71, FO9, YEL);
  disp.getTextBounds("MIDI Tx Channel:", 0, 0, &strX, &strY, &strW, &strH);
  valUp(midChan, 19 + (strW + 7), 91, FO9, CYN);

  for (i = 16; i < 22; i++)
  {
    disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
    disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
    disp.setFont(butSet[i].butFont);
    disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
    wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);
  }
}

// draw LED selection menu ----------------------------------------------------------------------------------------------------
void drawLxScreen()
{

  disp.fillScreen(BLK);

  disp.fillRect(10, 10, 220, 90, SCR);
  disp.drawRect(10, 10, 220, 90, FRM);

  uint16_t colOffA = disp.color565(redOffA, grnOffA, bluOffA);
  uint16_t colOnA = disp.color565(redOnA, grnOnA, bluOnA);
  uint16_t colOffB = disp.color565(redOffB, grnOffB, bluOffB);
  uint16_t colOnB = disp.color565(redOnB, grnOnB, bluOnB);
  uint16_t colOffC = disp.color565(redOffC, grnOffC, bluOffC);
  uint16_t colOnC = disp.color565(redOnC, grnOnC, bluOnC);
  uint16_t colOffD = disp.color565(redOffD, grnOffD, bluOffD);
  uint16_t colOnD = disp.color565(redOnD, grnOnD, bluOnD);

  disp.setFont(FO9);
  disp.getTextBounds("Choose colour to change:", 0, 0, &strX, &strY, &strW, &strH);
  wordUp("Choose colour to change:", (240 - strW) / 2, 29, FO9, WHT);
  wordUp("LED A:", 19, 51, FO9, FRM);
  wordUp("LED B:", 129, 51, FO9, FRM);
  wordUp("LED C:", 19, 71, FO9, FRM);
  wordUp("LED D:", 129, 71, FO9, FRM);
  wordUp("Selected state:", 19, 91, FO9, WHT);
  disp.getTextBounds("Selected state:", 0, 0, &strX, &strY, &strW, &strH);
  if (ledChoice == true)
  {
    wordUp("ON", strW + 30, 91, FO9, GRN);
  }
  else
  {
    wordUp("OFF", strW + 30, 91, FO9, RED);
  }

  disp.getTextBounds("LED C:", 0, 0, &strX, &strY, &strW, &strH);
  disp.fillCircle(strW + 32, 45, 6, colOffA);
  disp.fillCircle(strW + 50, 45, 6, colOnA);
  disp.fillCircle(strW + 142, 45, 6, colOffB);
  disp.fillCircle(strW + 160, 45, 6, colOnB);
  disp.fillCircle(strW + 32, 65, 6, colOffC);
  disp.fillCircle(strW + 50, 65, 6, colOnC);
  disp.fillCircle(strW + 142, 65, 6, colOffD);
  disp.fillCircle(strW + 160, 65, 6, colOnD);
  disp.fillCircle(strW + 32, 45, 3, SCR);
  disp.fillCircle(strW + 142, 45, 3, SCR);
  disp.fillCircle(strW + 32, 65, 3, SCR);
  disp.fillCircle(strW + 142, 65, 3, SCR);

  for (i = 22; i < 28; i++)
  {
    disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
    disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
    disp.setFont(butSet[i].butFont);
    disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
    wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);
  }
}

// draw LED colour faders menu ----------------------------------------------------------------------------------------------------
void drawFadeScreen()
{
  disp.fillScreen(BLK);

  redFad = *memSet[ledSet];
  grnFad = *memSet[ledSet + 1];
  bluFad = *memSet[ledSet + 2];

  disp.fillRoundRect(88, 6, 64, 64, 32, disp.color565(redFad, grnFad, bluFad));
  disp.drawRoundRect(88, 6, 64, 64, 32, FRM);

  for (i = 28; i < 30; i++)
  {
    disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
    disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
    disp.setFont(butSet[i].butFont);
    disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
    wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);

    uint16_t mapFad;

    disp.drawRect(42, 93, 24, 217, WHT);
    disp.fillRect(43, 94, 22, 215, SCR);
    disp.drawRect(108, 93, 24, 217, WHT);
    disp.fillRect(109, 94, 22, 215, SCR);
    disp.drawRect(174, 93, 24, 217, WHT);
    disp.fillRect(175, 94, 22, 215, SCR);
    mapFad = map(redFad, 0, 255, 0, 215);
    disp.fillRect(43, 309 - mapFad, 22, mapFad, RED);
    mapFad = map(grnFad, 0, 255, 0, 215);
    disp.fillRect(109, 309 - mapFad, 22, mapFad, GRN);
    mapFad = map(bluFad, 0, 255, 0, 215);
    disp.fillRect(175, 309 - mapFad, 22, mapFad, BLU);
  }
}

// press button flash ----------------------------------------------------------------------------------------------------
void boopFlash()
{

  disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, SCR);
  disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, YEL);
  disp.setFont(butSet[i].butFont);
  disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
  wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, YEL);
  delay(BOOP);
  disp.fillRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, BUT);
  disp.drawRoundRect(butSet[i].topX, butSet[i].topY, butSet[i].sizX, butSet[i].sizY, RAD, FRM);
  disp.getTextBounds(butSet[i].butLab, 0, 0, &strX, &strY, &strW, &strH);
  wordUp(butSet[i].butLab, ((butSet[i].topX + (butSet[i].sizX / 2)) - (strW / 2)) + butSet[i].curX, (butSet[i].topY + (butSet[i].sizY / 2) + (strH / 2)) + butSet[i].curY, butSet[i].butFont, butSet[i].texCol);
}

// print label ----------------------------------------------------------------------------------------------------
void wordUp(char *dropWord, uint16_t typeX, uint16_t typeY, uint16_t dropFont, uint16_t fontCol)
{
  disp.setFont(dropFont);
  disp.setTextColor(fontCol);
  disp.setCursor(typeX, typeY);
  disp.print(dropWord);
}

// print value ----------------------------------------------------------------------------------------------------
void valUp(byte dropVal, uint16_t typeX, uint16_t typeY, uint16_t dropFont, uint16_t fontCol)
{
  disp.setFont(dropFont);
  disp.setTextColor(fontCol);
  disp.setCursor(typeX, typeY);
  disp.print(dropVal);
}

// clear message screen ----------------------------------------------------------------------------------------------------
void clrMsg()
{
  delay(MSG);
  disp.fillRect(7, 7, 148, 62, SCR);
}
