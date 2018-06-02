
#include "draw/imdraw.inl"
#include "draw/imdrawaa.inl"

struct tm* getlocaltime() {
  time_t t;
  t = time(NULL);

  return localtime(&t);
}

const char MDay[][12] = {"January  ", "February ", "March    ", "April    ", "May      ", "June     ",
    "July     ", "August   ", "September", "October  ", "November ", "December "
                        };
const char Day[][12] = {"Sunday   ", "Monday   ", "Tuesday  ", "Wednesday", "Thursday ", "Friday   ", "Saturday "};
const char Hour[][12] = {"0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "11", "12",
    "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"
                        };
const char Min[][12] = {"0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "11", "12",
    "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34",
    "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45",
    "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
                       };
const char Sec[][12] = {"0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "11", "12",
    "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34",
    "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45",
    "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
                       };
const char Mday[][12] = {"1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "11", "12",
    "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
    "25", "26", "27", "28", "29", "30", "31"
                        };

#define TEXTOUT(im, x, y, txt, len)    imdraw_text(im, iRECT(x, y, x+10000, y+100), txt, -1, zimo_songti_12, _clr, 0, 0)
void TimeDateBG(img_t* im, int Px, int Py)
{
  COLOR _clr;
  char*  Label = "  CarvingPattern.softeng_softeng@yahoo.co.id  ";

  imdraw_fill(im, 34540);
  _clr = _RGB(200, 255, 100);
  TEXTOUT(im, Px - 340, 30, "                                                                             ", 77);
  TEXTOUT(im, Px - 330, 30, Hour[getlocaltime()->tm_hour], 2);
  TEXTOUT(im, Px - 310, 30, ":", 1);
  TEXTOUT(im, Px - 300, 30, Min[getlocaltime()->tm_min], 2);
  TEXTOUT(im, Px - 280, 30, ":", 1);
  TEXTOUT(im, Px - 260, 30, Sec[getlocaltime()->tm_sec], 2);
  TEXTOUT(im, Px - 230, 30, "/", 1);
  TEXTOUT(im, Px - 220, 30, Day[getlocaltime()->tm_wday], 9);
  TEXTOUT(im, Px - 135, 30, ",", 1);
  TEXTOUT(im, Px - 130, 30, MDay[getlocaltime()->tm_mon], 9);
  TEXTOUT(im, Px - 55 , 30, Mday[getlocaltime()->tm_mday - 1], 2);

  //Tittle of program
  _clr = (900000);
  TEXTOUT(im, Px - 340, 10, Label, 46);
}

void HelpToFunctionButton(img_t* im)
{
  int Py = im->h, Px = im->w;
  COLOR _clr;
  _clr = (_RGB(35, 75, 0));
  imdrawaa_rect(im, 0, (Py - 180), 190, (Py - 25), _clr, 0, 0);

  _clr = (_RGB(200, 255, 100));
  TEXTOUT(im, 0, (Py - 160), "  [F10]      =  The CarvingB  ", 29);
  TEXTOUT(im, 0, (Py - 140), "  [F11]      =  The CarvingA  ", 29);
  TEXTOUT(im, 0, (Py - 120), "  [F12]      =  The Graffiti  ", 30);
  TEXTOUT(im, 0, (Py - 100), "  [SPACE] =  Pause  ", 19);
  TEXTOUT(im, 0, (Py - 80) , "  [PgUp]   =  Next carving  ", 28);
  TEXTOUT(im, 0, (Py - 60) , "  [ESC]     =  Exit program  ", 29);
}

void Pause()
{
  if (GetAsyncKeyState(VK_SPACE))
    do {}
    while (GetAsyncKeyState(VK_SPACE));
}

double Speed()
{
  static double speed = 1000;

  if (GetAsyncKeyState(VK_UP)) {
    speed -= 10;
  }

  if (GetAsyncKeyState(VK_DOWN)) {
    speed += 10;
  }

  if (speed < 10) {
    speed = 10;
  }

  if (speed > 15000) {
    speed = 15000;
  }

  return speed;
}

double StepAngle()
{
  static double StepAGL = 0.25;

  if (GetAsyncKeyState(VK_RIGHT)) {
    StepAGL = 0.5;
  }

  if (GetAsyncKeyState(VK_LEFT)) {
    StepAGL = 0.25;
  }

  return StepAGL;
}

int MathArtAnimation(img_t* im)
{
  //Random based by system time in computer
  int i, n, m  = 0;
  int Py = im->h, Px = im->w;
  COLOR _clr;

  for (; m <= (getlocaltime()->tm_hour * 5) + getlocaltime()->tm_min; m ++) {
    rand();
  }

  do {
    static int inited = 0, _COEF,
               _COEF2,
               _COEF3,
               RandPnt1 = 2,
               RandPnt2 = 2,
               RandPnt3 = 2,
               RandPnt4 = 2;
    static double _AGL = 0;
    double CX, CY, Cx, Cy;

    if (!inited) {
      inited = 1;
      _COEF = -7 + rand() % 6,
      _COEF2 = -7 + rand() % 14,
      _COEF3 = -10 + rand() % 16;
    }

    _clr = (_RGB(200, 255, 100));

    if (_AGL == 360) {
      _COEF  = -7 + rand() % 6;
      _COEF2 = -7 + rand() % 14;
      _COEF3 = -10 + rand() % 16;
      _AGL   = 0;
      RandPnt1 = rand() % 25;
      RandPnt2 = rand() % 25;
      RandPnt3 = rand() % 25;
      RandPnt4 = rand() % 25;
    }

    _AGL += StepAngle();

    CX = (Px / 2) + (Py / 4) * sin(_AGL);
    TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Py/4)*sin(AGL)", 26);
    CY = (Py / 2) + (Py / 4) * cos(_AGL);
    TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL)", 26);

    if (RandPnt1 % 2 == 0) {
      CX = (Px / 2) + (Py / 4) * sin(_AGL * _COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Py/4)*sin(AGL*COEF)", 31);
    }

    if (RandPnt1 % 3 == 0) {
      CY = (Py / 2) + (Py / 4) * cos(_AGL * _COEF);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL*COEF)", 31);
    }

    ///////////////////////////////////////////////////////////////CURVE I

    Cx = CX + (Py / 7) * sin(_AGL * _COEF2);
    TEXTOUT(im, (Px / 2 - 220), (Py - 50), "X = x+(Py/7)*sin(AGL*COEF2)", 27);
    Cy = CY + (Py / 7) * cos(_AGL * _COEF2);
    TEXTOUT(im, (Px / 2 - 220), (Py - 30), "Y = y+(Py/7)*cos(AGL*COEF2)", 27);

    if (RandPnt2 % 2 == 0) {
      Cx = CX + (Py / 7) * sin(_AGL * _COEF) * cos(_AGL * _COEF2);
      TEXTOUT(im, (Px / 2 - 220), (Py - 50), "X = x+(Py/7)*sin(AGL*COEF)*cos(AGL*COEF2)", 41);
    }

    if (RandPnt2 % 3 == 0) {
      Cy = CY + (Py / 7) * cos(_AGL * _COEF) * sin(_AGL * _COEF2);
      TEXTOUT(im, (Px / 2 - 220), (Py - 30), "Y = y+(Py/7)*cos(AGL*COEF)*sin(AGL*COEF2)", 41);
    }

    if (RandPnt2 % 4 == 0) {
      Cx = CX + (Py / 7) * cos(_AGL * _COEF2);
      TEXTOUT(im, (Px / 2 - 220), (Py - 50), "X = x+(Py/7)*cos(AGL*COEF2)", 27);
    }

    if (RandPnt2 % 5 == 0) {
      Cy = CY + (Py / 7) * sin(_AGL * _COEF2);
      TEXTOUT(im, (Px / 2 - 220), (Py - 30), "Y = y+(Py/7)*sin(AGL*COEF2)", 27);
    }

    ///////////////////////////////////////////////////////////////CURVE II

    /*Sb.X*/
    if (RandPnt3 % 4 == 0) {
      Cx = Cx + (Py / 15) * sin(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 50), "X = X+(Py/15)*sin(AGL*COEF3)", 28);
    }
    else if (RandPnt3 % 3 == 0) {
      Cx = Cx + (Py / 20) * cos(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 50), "X = X+(Py/20)*cos(AGL*COEF3)", 28);
    }
    else if (RandPnt3 % 2 == 0) {
      Cx = Cx + (Py / 15) * sin(_AGL * _COEF2) * cos(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 50), "X = X+(Py/15)*sin(AGL*COEF2)*cos(AGL*COEF3)", 43);
    }

    /*Sb.Y*/
    if (RandPnt4 % 4 == 0) {
      Cy = Cy + (Py / 15) * cos(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 30), "Y = Y+(Py/15)*cos(AGL*COEF3)", 28);
    }
    else if (RandPnt4 % 3 == 0) {
      Cy = Cy + (Py / 20) * sin(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 30), "Y = Y+(Py/20)*sin(AGL*COEF3)", 28);
    }
    else if (RandPnt4 % 2 == 0) {
      Cy = Cy + (Py / 15) * cos(_AGL * _COEF2) * sin(_AGL * _COEF3);
      TEXTOUT(im, (Px - 360), (Py - 30), "Y = Y+(Py/15)*cos(AGL*COEF2)*sin(AGL*COEF3)", 43);
    }

    ///////////////////////////////////////////////////////////////CURVE III

    //PRINT
    _clr = (_RGB(245, 255, 200));

    for (n = 0; n <= Speed() - 10; n++) {
      (im, Cx, Cy);
    }

    //DELAY
    if (_AGL == 360) {
      for (i = 500; i <= 3000; i += 500) {
        Sleep(500);
        TimeDateBG(Px, Py);
      }
    }

    //NEXT picture
    if (GetAsyncKeyState(VK_PRIOR)) {
      _AGL = 360;
    }

    //FOR background per drawing
    if (_AGL == 0.25) {
      int m2;

      if (rand() % 2 == 0) {
        m2 = ((4 + (rand() % 5)) * 1000);
      }
      else {
        m2 = ((13 + (rand() % 6)) * 1000);
      }

      SetBkColor(im, m2 + 990);

      for (int m1 = 0; m1 <= Py; m1 += 10)
        for (int n = 0; n <= Px; n += 30) {
          TEXTOUT(im, n, m1, "        ", 8);
        }

      if (rand() % 2 == 0) {
        hbrush = CreateHatchBrush(1, _RGB(155, 155, 50));
        SelectObject(im, hbrush);
        Rectangle(im, 50, 50, Px - 50, Py - 50);
      }
    }

    Pause();
    TimeDateBG(Px, Py);

    //TO Change The Another MathArtAnimation
    //if(GetAsyncKeyState(VK_F10))
    //  return 0;

    //if(GetAsyncKeyState(VK_F11))
    //  return 0;
    ////

    //EXIT Program
    if (GetAsyncKeyState(VK_ESCAPE)) {
      system("explorer");
      exit(1);
    }

  }
  while (1);

  return 0;
}
//#############################################
//#############################################

int MathArtAnimation_2(void)
{
  //Random based by system time in computer
  for (int n, m  = 0;
      m <= (getlocaltime()->tm_hour * 5) + getlocaltime()->tm_min;
      m ++) {
    rand();
  }

  int  Px, Py;
  GetPixelValue(&Px, &Py);

  do {
    static int COEF  = -13 + rand() % 10,
               RAND  = rand() % 12,
               COEF2 = -13 + rand() % 10,
               RAND2 = rand() % 12;
    static double AGL = 0;
    double CX, CY, Cx, Cy;

    SetTextColor(im, _RGB(200, 255, 100));

    if (AGL == 720) {
      COEF  = -17 + rand() % 15;
      RAND  = rand() % 12;
      COEF2 = -17 + rand() % 15;
      RAND2 = rand() % 12;
      AGL   = 0;
    }

    AGL += 0.5;

    if (RAND % 6 == 0) {
      CX = (Px / 2) + (Py / 4) * sin(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * cos(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Py/4)*sin(AGL*COEF)", 31);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL*COEF)", 31);
    }
    else if (RAND % 5 == 0) {
      CX = (Px / 2) + (Px / 4) * sin(AGL * COEF) * cos(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * cos(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Px/4)*sin(AGL*COEF)*cos(AGL*COEF)", 45);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL*COEF)", 31);
    }
    else if (RAND % 4 == 0) {
      CX = (Px / 2) + (Px / 4) * cos(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * sin(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Px/4)*cos(AGL*COEF)", 31);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*sin(AGL*COEF)", 31);
    }
    else if (RAND % 3 == 0) {
      CX = (Px / 2) + (Px / 4) * sin(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * cos(AGL * COEF) * sin(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Px/4)*sin(AGL*COEF)", 31);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL*COEF)*sin(AGL*COEF)", 45);
    }
    else if (RAND % 2 == 0) {
      CX = (Px / 2) + (Py / 4) * cos(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * sin(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Py/4)*cos(AGL*COEF)", 31);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*sin(AGL*COEF)", 31);
    }
    else {
      CX = (Px / 2) + (Px / 4) * sin(AGL * COEF);
      CY = (Py / 2) + (Py / 4) * cos(AGL * COEF);
      TEXTOUT(im, (20), (Py - 50), "x = (Px/2)+(Px/4)*sin(AGL*COEF)", 31);
      TEXTOUT(im, (20), (Py - 30), "y = (Py/2)+(Py/4)*cos(AGL*COEF)", 31);
    }

    ///////////////////////////////////////////////////////////CURVE I
    if (RAND2 % 6 == 0) {
      Cx = CX + (Py / 5) * sin(AGL * COEF2);
      Cy = CY + (Py / 5) * cos(AGL * COEF2);
      TEXTOUT(im, (Px - 230), (Py - 50), "X = x+(Py/5)*sin(AGL*COEF2)", 27);
      TEXTOUT(im, (Px - 230), (Py - 30), "Y = y+(Py/5)*cos(AGL*COEF2)", 27);
    }
    else if (RAND2 % 5 == 0) {
      Cx = CX + (Px / 5) * sin(AGL * COEF2) * cos(AGL * COEF2);
      Cy = CY + (Py / 5) * cos(AGL * COEF2);
      TEXTOUT(im, (Px - 330), (Py - 50), "X = x+(Px/5)*sin(AGL*COEF2)*cos(AGL*COEF2)", 42);
      TEXTOUT(im, (Px - 330), (Py - 30), "Y = y+(Py/5)*cos(AGL*COEF2)", 27);
    }
    else if (RAND2 % 4 == 0) {
      Cx = CX + (Px / 5) * cos(AGL * COEF2);
      Cy = CY + (Py / 5) * sin(AGL * COEF2);
      TEXTOUT(im, (Px - 230), (Py - 50), "X = x+(Px/5)*cos(AGL*COEF2)", 27);
      TEXTOUT(im, (Px - 230), (Py - 30), "Y = y+(Py/5)*sin(AGL*COEF2)", 27);
    }
    else if (RAND2 % 3 == 0) {
      Cx = CX + (Px / 5) * sin(AGL * COEF2);
      Cy = CY + (Py / 5) * cos(AGL * COEF2) * sin(AGL * COEF2);
      TEXTOUT(im, (Px - 330), (Py - 50), "X = x+(Px/5)*sin(AGL*COEF2)", 27);
      TEXTOUT(im, (Px - 330), (Py - 30), "Y = y+(Py/5)*cos(AGL*COEF2)*sin(AGL*COEF2)", 42);
    }
    else if (RAND2 % 2 == 0) {
      Cx = CX + (Py / 5) * cos(AGL * COEF2);
      Cy = CY + (Py / 5) * sin(AGL * COEF2);
      TEXTOUT(im, (Px - 230), (Py - 50), "X = x+(Py/5)*cos(AGL*COEF2)", 27);
      TEXTOUT(im, (Px - 230), (Py - 30), "Y = y+(Py/5)*sin(AGL*COEF2)", 27);
    }
    else {
      Cx = CX + (Px / 5) * sin(AGL * COEF2);
      Cy = CY + (Py / 5) * cos(AGL * COEF2);
      TEXTOUT(im, (Px - 230), (Py - 50), "X = x+(Px/5)*sin(AGL*COEF2)", 27);
      TEXTOUT(im, (Px - 230), (Py - 30), "Y = y+(Py/5)*cos(AGL*COEF2)", 27);
    }

    ///////////////////////////////////////////////////////////CURVE II


    //PRINT
    pen = CreatePen(PS_SOLID, 1, _RGB(245, 255, 200));
    SelectObject(im, pen);

    for (n = 0; n <= Speed() - 10; n++) {
      LineTo(im, Cx, Cy);
    }

    RoundRect(im, Cx, Cy, Cx + 5, Cy + 4, Cx + 5, Cy + 4);

    //DELAY
    if (AGL == 720)
      for (int i = 500; i <= 3000; i += 500) {
        Sleep(500);
        TimeDateBG(Px, Py);
      }

    //FOR background per drawing
    if (AGL <= 0.5) {
      int m2;

      if (rand() % 2 == 0) {
        m2 = ((4 + (rand() % 5)) * 1000);
      }
      else {
        m2 = ((13 + (rand() % 6)) * 1000);
      }

      if (rand() % 2 == 0) {
        SetBkColor(im, m2 + 990);

        for (int m1 = 0; m1 <= Py; m1 += 10)
          for (int n = 0; n <= Px; n += 30) {
            TEXTOUT(im, n, m1, "        ", 8);
          }
      }
      else if (rand() % 1 == 0) {
        hbrush = CreateHatchBrush(1, _RGB(55, 55, 55));
        SelectObject(im, hbrush);
        SetBkColor(im, _RGB(20, 20, 20));
        Rectangle(im, 0, 0, Px, Py);
      }
    }

    //NEXT picture
    if (GetAsyncKeyState(VK_PRIOR)) {
      AGL = 720;
    }

    Pause();
    TimeDateBG(Px, Py);

    //TO Change The Another MathArtAnimation
    //if(GetAsyncKeyState(VK_F10))
    //  return 0;

    //if(GetAsyncKeyState(VK_F12))
    //  return 0;
    ////

    //EXIT Program
    if (GetAsyncKeyState(VK_ESCAPE)) {
      system("explorer");
      exit(1);
    }

  }
  while (1);

  return 0;
}
//#############################################
//#############################################

int MathArtAnimation_3(void)
{
  int Px, Py;
  GetPixelValue(&Px, &Py);

  //Random based by system time in computer
  for (int m0  = 0;
      m0 <= (getlocaltime()->tm_hour * 100) + getlocaltime()->tm_min + getlocaltime()->tm_sec;
      m0 ++) {
    rand();
  }

  //Global Declaration of variabel
  double  a = 18.9, x, y;
  int   k = -8 + rand() % 17,
        l = -16 + rand() % 32,
        m = -16 + rand() % 40,
        n,
        rnd = 1,
        Rnd = 1,
        RND = 1,
        k1  = k;
  ////

  do {
    double range = 0.01;

    if (k <= -7  ||  k > 6) {
      range = 0.005;
    }

    a += range;

    //Make color
    SetBkColor(im, _RGB(20, 20, 20));
    pen = CreatePen(PS_SOLID, 1, _RGB(200, 255, 200));
    SelectObject(im, pen);
    hbrush = CreateHatchBrush(1, _RGB(55, 55, 55));
    SelectObject(im, hbrush);

    //Drawing Part 1
    y = (Py / 2) + 24 * cos(a);
    x = (Px / 2) + 24 * sin(a);

    if (rnd > 5) {
      LineTo(im, x, y);
    }

    if (Rnd % 5 == 0) {
      rnd = 10;
    }

    //Drawing Part 2
    x = (Px / 2) + 200 * sin(a);
    y = (Py / 2) + 200 * cos(a);

    //When (Rnd%5==0)'s condition
    if (Rnd % 5 == 0) {
      k = k1;
    }

    int o = k;

    if (RND % 3 == 0  &&  Rnd % 5 != 0) {
      o = (k - 4) / 2;
    }

    if (Rnd % 5 == 0  && (k < -1 || k > 4)) {
      if (k > 4) {
        Rnd = 1;
        RND = 6;
      }
      else {
        k = 2;
      }
    }

    ////

    /////Addon effect for Drawing Part 4
    double Rad = 100,
           _Rad = 100;
    int _rnd,
        _rnd_cos;

    if (a >= 18.9) {
      _rnd   = rand() % 100;
      _rnd_cos = -9 + rand() % 15;
    }

    if (rnd <= 5  &&  a < 12.6) {
      _Rad = 50;
      Rad  = 50;
    }

    if (_rnd % 1 == 0) {
      Rad = 100;
    }

    if (_rnd % 3 == 0) {
      Rad = _Rad * cos(a * _rnd_cos);
    }

    if (_rnd % 4 == 0) {
      Rad = _Rad * cos(a * k);
    }

    /////

    //Drawing Part 4
    y = y + Rad * cos(a * o);

    if (Rnd % 5 == 0) {
      x = x + Rad * sin(a * (k - 2) / 2) * cos(a * (k - 2) / 2);
    }
    else {
      x = x + Rad * sin(a * k);
    }

    if (Rnd % 1 == 0  &&  a < 12.6) {
      LineTo(im, x, y);
    }

    //else
    if (a < 6.3  || (a < 12.6 && rnd > 5)) {
      LineTo(im, x, y);
      RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
    }

    ////

    //////Addon effect when rnd<=5
    if (a <= 12.61  &&  rnd <= 5) {
      LineTo(im, x + 50 * sin(a), y + 100 * cos(a * m));
      RoundRect(im, x + 50 * sin(a)    ,  y + 100 * cos(a * m),
          (x + 50 * sin(a)) + 5 , (y + 100 * cos(a * m)) + 4, x + 5, y + 4);
    }

    /////

    //Drawing Part 5
    int B[51];
    B[50] = l;
    B[30] = m;

    for (int A = 50; A >= 30; A -= 20) {
      x = x + A * sin(a * B[A]);
      y = y + A * cos(a * B[A]);

      if (a >= 12.6) {
        if (RND % 5 != 0) {
          if (Rnd % 5 == 0  && ((k > -2 && k < 2)  ||  k > 2)) {
            RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
          }
          else {
            if (rnd <= 5  &&  Rnd % 5 != 0  &&  Rnd % 4 != 0) {
              LineTo(im, x, y);
            }

            RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
          }
        }
        else {
          if ((k < -2 || k > 4)  &&  a < 12.7) {
            k = -2 + rand() % 5;
          }

          if (Rnd % 4 != 0) {
            RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
          }
          else {
            if (rnd <= 5  &&  Rnd % 5 != 0  &&  Rnd % 4 != 0) {
              LineTo(im, x, y);
            }

            RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
          }
        }
      }
    }

    if (a >= 12.6  &&  rnd <= 5  &&  k > -5  &&  k < 5  &&  l > -9) {
      x = x + 30 * sin(a * k);
      y = y + 30 * cos(a * k);
      RoundRect(im, x, y, x + 5, y + 4, x + 5, y + 4);
    }

    ////


    //Speed of drawing
    static int speedK = 10;

    if (GetAsyncKeyState(VK_UP)) {
      speedK --;
    }

    if (GetAsyncKeyState(VK_DOWN)) {
      speedK ++;
    }

    if (speedK < 1) {
      speedK = 1;
    }

    Sleep(speedK);
    ////

    //Delay per drawing
    if ((a >= 6.29  &&  a <= 6.29)  ||  a >= 18.9) {
      Sleep(3500);
    }

    //For background per drawing
    int BG = 0;

    if ((a >= 12.60  &&  a <= 12.61)  && (rnd <= 5  ||  Rnd % 6 == 0)) {
      BG = 1;

      if (a <= 12.605  &&  range == 0.005) {
        Sleep(3500);
      }

      if (a <= 12.610  &&  range == 0.01) {
        Sleep(3500);
      }
    }

    if (a <= 0.02 || (a >= 6.29 && a <= 6.3) || BG == 1) {
      if (rand() % 2 == 0) { //Type 1
        int m2;

        if (rand() % 2 == 0) {
          m2 = ((4 + (rand() % 5)) * 1000);
        }
        else {
          m2 = ((13 + (rand() % 6)) * 1000);
        }

        SetBkColor(im, m2 + 990);

        for (int m1 = 0; m1 <= Py; m1 += 10)
          for (int n = 0; n <= Px; n += 30) {
            TEXTOUT(im, n, m1, "        ", 8);
          }
      }
      else      //Type 2 (with color degradation)
        if (rand() % 30000 == 0) {
          int m2 = 34060;

          for (int m1 = 0; m1 <= hypot((Px / 2), (Py / 2)); m1++) {
            SetBkColor(im, m2 + (m1 / 3));

            for (double Agl = 0; Agl <= 720; Agl += 0.5) {
              TEXTOUT(im, (Px / 2) + m1 * sin(Agl), (Py / 2) + m1 * cos(Agl), " ", 1);
            }
          }
        }
        else if (rand() % 1 == 0) {
          Rectangle(im, 0, 0, Px, Py);
        }
    }

    ////

    //Next the carving
    if (GetAsyncKeyState(VK_PRIOR)) {
      a = 18.9;
    }


    //Random of values to draw various carvings
    if (a >= 18.9) {
      if (rnd == 5 || rnd == 3) {
        a = 0;
      }
      else {
        a = 6.29;
      }

      k = -12 + rand() % 21;
      l = -15 + rand() % 10;
      m = -15 + rand() % 10;
      rnd = -5 + rand() % 30;
      Rnd = rand() % 25;
      RND = rand() % 1000;
      k1  = k;
    }

    ////

    //Pause the carving process
    Pause();

    SetCursorPos(Px + 1000, Py + 1000);
    //ShowCursor(FALSE);

    //Clock.Date
    //Rectangle(im,Px-350,5,Px-20,55);
    TimeDateBG(Px, Py);
    //

    //To Change The Another MathArtAnimation
    //if(GetAsyncKeyState(VK_F12))
    //  return 0;

    //if(GetAsyncKeyState(VK_F11))
    //  return 0;
    ////

    //Exit Program
    if (GetAsyncKeyState(VK_ESCAPE)) {
      system("explorer");
      exit(1);
    }
  }
  while (1);

  return 0;
}

int test_Carving_Pattern()
{
  do {
    HelpToFunctionButton();

    if (GetAsyncKeyState(VK_F12)) {
      MathArtAnimation();
    }

    if (GetAsyncKeyState(VK_F11)) {
      MathArtAnimation_2();
    }

    if (GetAsyncKeyState(VK_F10)) {
      MathArtAnimation_3();
    }

    Sleep(100);

    //EXIT Program
    if (GetAsyncKeyState(VK_ESCAPE)) {
      system("explorer");
      exit(1);
    }
  }
  while (1);

  return 0;
}

//  x1=((x+50*sin(a*l))+400*cos(a)*sin(a))+100*sin(a*(k-2)/2)*cos(a*(k-2)/2);
//  x2=(((x+30*sin(a*m))+50*sin(a*l))+400*cos(a)*sin(a))+100*sin(a*(k-2)/2)*cos(a*(k-2)/2);