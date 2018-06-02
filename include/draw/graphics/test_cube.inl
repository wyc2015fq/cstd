#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

int a[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1};            /*INITIALIZE SIX PLANS*/
int b[3][3] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
int c[3][3] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
int d[3][3] = {3, 3, 3, 3, 3, 3, 3, 3, 3};
int e[3][3] = {5, 5, 5, 5, 5, 5, 5, 5, 5};
int f[3][3] = {7, 7, 7, 7, 7, 7, 7, 7, 7};
int top[3][3], front[3][3], side[3][3];
int position = 0, temp[3] = {0, 0, 0}, new[3][3];     /*POSITION ASSIGN THE ARROW POSITION*/
int sign;


void drawing(int inital_x, int inital_y, int lenth)        /*DRAWING THE STRUCTURE OF THE PICTURE*/
{
  setlinestyle2(0, 3, 0, 0);
  line(inital_x + 1.5 * lenth, inital_y + 0, inital_x + 4.5 * lenth, inital_y + 0);
  line(inital_x + 1.0 * lenth, inital_y + 0.5 * lenth, inital_x + 4.0 * lenth, inital_y + 0.5 * lenth);
  line(inital_x + 0.5 * lenth, inital_y + 1 * lenth, inital_x + 3.5 * lenth, inital_y + 1 * lenth);
  line(inital_x + 0, inital_y + 1.5 * lenth, inital_x + 3 * lenth, inital_y + 1.5 * lenth);
  line(inital_x + 0, inital_y + 2.5 * lenth, inital_x + 3 * lenth, inital_y + 2.5 * lenth);
  line(inital_x + 0, inital_y + 3.5 * lenth, inital_x + 3 * lenth, inital_y + 3.5 * lenth);
  line(inital_x + 0, inital_y + 4.5 * lenth, inital_x + 3 * lenth, inital_y + 4.5 * lenth);
  line(inital_x + 0, inital_y + 1.5 * lenth, inital_x + 0, inital_y + 4.5 * lenth);
  line(inital_x + 1.0 * lenth, inital_y + 1.5 * lenth, inital_x + 1.0 * lenth, inital_y + 4.5 * lenth);
  line(inital_x + 2.0 * lenth, inital_y + 1.5 * lenth, inital_x + 2.0 * lenth, inital_y + 4.5 * lenth);
  line(inital_x + 3.0 * lenth, inital_y + 1.5 * lenth, inital_x + 3.0 * lenth, inital_y + 4.5 * lenth);
  line(inital_x + 3.5 * lenth, inital_y + 1.0 * lenth, inital_x + 3.5 * lenth, inital_y + 4.0 * lenth);
  line(inital_x + 4.0 * lenth, inital_y + 0.5 * lenth, inital_x + 4.0 * lenth, inital_y + 3.5 * lenth);
  line(inital_x + 4.5 * lenth, inital_y + 0, inital_x + 4.5 * lenth, inital_y + 3.0 * lenth);
  line(inital_x + 1.5 * lenth, inital_y + 0, inital_x + 0, inital_y + 1.5 * lenth);
  line(inital_x + 2.5 * lenth, inital_y + 0, inital_x + 1.0 * lenth, inital_y + 1.5 * lenth);
  line(inital_x + 3.5 * lenth, inital_y + 0, inital_x + 2.0 * lenth, inital_y + 1.5 * lenth);
  line(inital_x + 4.5 * lenth, inital_y + 0, inital_x + 3.0 * lenth, inital_y + 1.5 * lenth);
  line(inital_x + 4.5 * lenth, inital_y + 1.0 * lenth, inital_x + 3.0 * lenth, inital_y + 2.5 * lenth);
  line(inital_x + 4.5 * lenth, inital_y + 2.0 * lenth, inital_x + 3.0 * lenth, inital_y + 3.5 * lenth);
  line(inital_x + 4.5 * lenth, inital_y + 3.0 * lenth, inital_x + 3.0 * lenth, inital_y + 4.5 * lenth);
}
void color(int inital_x, int inital_y, int lenth)          /*FILL COLOR BY THE DATA*/
{
  int i, j;
  int color_num;
  int x1 = inital_x + 1.5 * lenth;
  int y1 = inital_y + 0.25 * lenth;
  int x2, y2;

  for (i = 0; i < 3; i++) {
    x2 = x1;
    y2 = y1;

    for (j = 0; j < 3; j++) {
      color_num = top[i][j];
      setfillstyle(1, color_num);
      floodfill(x2, y2, WHITE);
      x2 = x2 + lenth;
    }

    x1 = x1 - 0.5 * lenth;
    y1 = y1 + 0.5 * lenth;
  }

  x1 = inital_x + 0.5 * lenth;
  y1 = inital_y + 2 * lenth;

  for (i = 0; i < 3; i++) {
    x2 = x1;
    y2 = y1;

    for (j = 0; j < 3; j++) {
      color_num = front[i][j];
      setfillstyle(1, color_num);
      floodfill(x2, y2, WHITE);
      x2 = x2 + lenth;
    }

    y1 = y1 + lenth;
  }

  x1 = inital_x + 3.25 * lenth;
  y1 = inital_y + 2 * lenth;

  for (i = 0; i < 3; i++) {
    x2 = x1;
    y2 = y1;

    for (j = 0; j < 3; j++) {
      color_num = side[i][j];
      setfillstyle(1, color_num);
      floodfill(x2, y2, WHITE);
      x2 = x2 + 0.5 * lenth;
      y2 = y2 - 0.5 * lenth;
    }

    y1 = y1 + lenth;
  }
}
void arrow(int inital_x, int inital_y, int lenth)          /*DRAWING THE ARROW BY THE POSITION*/
{
  if (position <= 2) {
    line(inital_x - 30, inital_y + 2 * lenth - 3 + position * lenth, inital_x - 20, inital_y + 2 * lenth - 3 + position * lenth);
    line(inital_x - 30, inital_y + 2 * lenth + 3 + position * lenth, inital_x - 20, inital_y + 2 * lenth + 3 + position * lenth);
    line(inital_x - 21, inital_y + 2 * lenth - 6 + position * lenth, inital_x - 15, inital_y + 2 * lenth + position * lenth);
    line(inital_x - 21, inital_y + 2 * lenth + 6 + position * lenth, inital_x - 15, inital_y + 2 * lenth + position * lenth);
  }

  if (position <= 5 && position > 2) {
    line(inital_x + 0.5 * lenth + (position - 3)*lenth - 3, inital_y + 4.5 * lenth + 30, inital_x + 0.5 * lenth + (position - 3)*lenth - 3, inital_y + 4.5 * lenth + 20);
    line(inital_x + 0.5 * lenth + (position - 3)*lenth + 3, inital_y + 4.5 * lenth + 30, inital_x + 0.5 * lenth + (position - 3)*lenth + 3, inital_y + 4.5 * lenth + 20);
    line(inital_x + 0.5 * lenth + (position - 3)*lenth - 6, inital_y + 4.5 * lenth + 21, inital_x + 0.5 * lenth + (position - 3)*lenth, inital_y + 4.5 * lenth + 15);
    line(inital_x + 0.5 * lenth + (position - 3)*lenth + 6, inital_y + 4.5 * lenth + 21, inital_x + 0.5 * lenth + (position - 3)*lenth, inital_y + 4.5 * lenth + 15);
  }

  if (position > 5) {
    line(inital_x + 3.25 * lenth + (position - 6)*lenth * 0.5 - 3 + 5, inital_y + 4.5 * lenth + 30 - (position - 5) * 0.5 * lenth + 20, inital_x + 3.25 * lenth + (position - 6) * 0.5 * lenth - 3 + 5, inital_y + 4.5 * lenth + 20 - (position - 5) * 0.5 * lenth + 20);
    line(inital_x + 3.25 * lenth + (position - 6)*lenth * 0.5 + 3 + 5, inital_y + 4.5 * lenth + 30 - (position - 5) * 0.5 * lenth + 20, inital_x + 3.25 * lenth + (position - 6) * 0.5 * lenth + 3 + 5, inital_y + 4.5 * lenth + 20 - (position - 5) * 0.5 * lenth + 20);
    line(inital_x + 3.25 * lenth + (position - 6)*lenth * 0.5 - 6 + 5, inital_y + 4.5 * lenth + 21 - (position - 5) * 0.5 * lenth + 20, inital_x + 3.25 * lenth + (position - 6) * 0.5 * lenth + 5, inital_y + 4.5 * lenth + 15 - (position - 5) * 0.5 * lenth + 20);
    line(inital_x + 3.25 * lenth + (position - 6)*lenth * 0.5 + 6 + 5, inital_y + 4.5 * lenth + 21 - (position - 5) * 0.5 * lenth + 20, inital_x + 3.25 * lenth + (position - 6) * 0.5 * lenth + 5, inital_y + 4.5 * lenth + 15 - (position - 5) * 0.5 * lenth + 20);
  }
}

void copy(int* p, int* q)
{
  int i, j;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      *q = *p;
      p++;
      q++;
    }
  }
}

void change1(int* p, int* q)                               /*CONVERT TWO ARRAY*/
{
  int i, ling;
  p = p + sign;
  q = q + sign;

  if (temp[0] == 0) {
    for (i = 0; i < 3; i++) {
      temp[i] = *p;
      p = p + 3;
    }

    p = p - 9;
  }

  for (i = 0; i < 3; i++) {
    ling = *q;
    *q = temp[i];
    temp[i] = ling;
    p = p + 3;
    q = q + 3;
  }
}
void change2(int* p, int x)                                /*CHANGE THE ARRAY BY REVERSE-CLOCKWISE*/
{
  int i, j, k;

  for (k = 0; k < x; k++) {
    for (i = 0; i < 3; i++) {
      for (j = 2; j >= 0; j--) {
        new[j][i] = *p;
        p = p + 1;
      }
    }

    p = p - 9;

    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
        *p = new[i][j];
        p++;
      }
    }

    p = p - 9;
  }
}


void test_cube()
{
  int l = 60, x = 50, y = 50, key, count, input = 0, errorcode;   /*L MEANS THE LENTH OF THE SQUARE,X MEANS THE INITAIL POSITION OF X,Y MEANS THE INITIAL POSITION OF Y*/
  int a1[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1};           /*INITIALIZE SIX PLANS FOR BACKUP*/
  int b1[3][3] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
  int c1[3][3] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
  int d1[3][3] = {3, 3, 3, 3, 3, 3, 3, 3, 3};
  int e1[3][3] = {5, 5, 5, 5, 5, 5, 5, 5, 5};
  int f1[3][3] = {7, 7, 7, 7, 7, 7, 7, 7, 7};
  //int graphdriver=VGA,graphmode=VGAHI;
  initgraph(640, 480, 0);

  drawing(x, y, l);                                     /*DRAWING THE BIG SQUARE*/
  drawing(x + 400, y, l / 2);                           /*DRAWING THE SMALL SQUARE*/

  do {
    sign = position % 3;                                  /*COPY DATA TO SHOW*/
    copy(&a, &top);
    copy(&b, &front);
    copy(&c, &side);
    color(x, y, l);                                       /*SHOWING THE BIG SQUARE*/
    copy(&e, &top);
    change2(&top, 1);
    copy(&f, &front);
    change2(&front, 2);
    copy(&d, &side);
    color(x + 400, y, l / 2);                             /*SHOWING THE SMALL SQUARE*/
    setcolor(14);
    arrow(x, y, l);
    key = bioskey(0);                                     /*READ KEYBORD*/

    switch (key) {
    case 283:
      printf("Esc");                               /*IF INPUT_KEY="ESC" TO EXIT THE PROGRAM*/
      goto end;

    case 20480:
      setcolor(0);                               /*IF INPUT_KEY="DOWN" TO CHANGE THE ARROW*/
      arrow(x, y, l);

      if (position == 8) {
        position = 0;
      }
      else {
        position = position + 1;
      }

      break;                                     /*IF INPUT_KEY="UP" TO CHANGE THE ARROW*/

    case 18432:
      setcolor(0);
      arrow(x, y, l);

      if (position == 0) {
        position = 8;
      }
      else {
        position = position - 1;
      }

      break;

    case 19712:
      input = 1;                                 /*IF INPUT_KEY="RIGHT" TO MOVE FORWARD*/
      break;

    case 19200:
      input = 3;                                 /*IF INPUT_KEY="LEFT" TO MOVE REWARD*/
      break;

    case 7181:
      setcolor(0);                               /*IF INPUT_KEY="ENTER" TO RELOAD */
      arrow(x, y, l);
      copy(&a1, &a);
      copy(&b1, &b);
      copy(&c1, &c);
      copy(&d1, &d);
      copy(&e1, &e);
      copy(&f1, &f);
      position = 0;
      break;

    case 14624:
      setcolor(0);                               /*IF INPUT_KEY="SPACE" TO RANDOM MOVE*/
      arrow(x, y, l);
      randomize();
      position = random(9);
      input = 1;
      break;
    }

    if (input != 0) {                                     /*USE POSITION TO JUDGE AND CHANGE DATA*/
      /*IF INPUT=0,CHANGE CLOCKWISE,IF INPUT=3,CHANGE REVERSE-CLOCKWISE*/
      for (count = 0; count < input; count++) {
        if (position / 3 == 0) {                   /*IF NEED TO CHANGE TOP*/
          change2(&b, 1);
          change2(&c, 1);
          change2(&d, 3);
          change2(&f, 1);
          change1(&b, &c);
          change1(&c, &d);
          change1(&d, &f);
          change1(&f, &b);
          change2(&b, 3);
          change2(&c, 3);
          change2(&d, 1);
          change2(&f, 3);
          temp[0] = 0;
          temp[1] = 0;
          temp[2] = 0;

          if (position % 3 == 0) {
            change2(&a, 1);
          }

          if (position % 3 == 2) {
            change2(&e, 1);
          }
        }

        if (position / 3 == 1) {                   /*IF NEED TO CHANGE FRONT*/
          change1(&a, &b);
          change1(&b, &e);
          change1(&e, &d);
          change1(&d, &a);
          temp[0] = 0;
          temp[1] = 0;
          temp[2] = 0;

          if (position % 3 == 0) {
            change2(&f, 1);
          }

          if (position % 3 == 2) {
            change2(&c, 1);
          }
        }

        if (position / 3 == 2) {                   /*IF NEED TO CHANGE SIDE*/
          change2(&a, 3);
          change2(&e, 1);
          change2(&f, 2);
          change1(&a, &c);
          change1(&c, &e);
          change1(&e, &f);
          change1(&f, &a);
          change2(&a, 1);
          change2(&e, 3);
          change2(&f, 2);
          temp[0] = 0;
          temp[1] = 0;
          temp[2] = 0;

          if (position % 3 == 0) {
            change2(&b, 1);
          }

          if (position % 3 == 2) {
            change2(&d, 1);
          }
        }
      }

      input = 0;
    }
  }
  while (1);

end:
  clearviewport();                                      /*CLEAR THE SCREEN*/
}


