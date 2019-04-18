# Skywind Inside » MINI-FOOLFLY 游戏代码
## MINI-FOOLFLY 游戏代码
April 12th, 2005[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
此程序仅向人说明编写游戏程序的若干功能实现和常用处理方法，编译版本下载：
[http://www.skywind.me/resource/oldworks/foolfly.zip](http://www.skywind.me/resource/oldworks/foolfly.zip)
/* Fool fly game v1.0 by skywind
 * 空战摸拟源程序，林伟 1999年7月30日
 *
 * 这是一个空战程序的模式例子，并没有做任何优化与扩充
 * 仅仅向人说明编写游戏程序的若干功能实现或常用处理方法
 * 程序是DOS版的，现在写游戏的平台早已变化了许多，但是
 * 我尽量容入了一些不变理论，希望才入门的游戏设计者能受
 * 到一些启发，通过它也可以看出当日DOS下设计的困难重重。
 *
 * 整个程序不足300行，请用BC31或者TC20的Large模式编译
 */#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <mem.h>
#include <time.h>
#include <math.h>
#include <alloc.h>
#define  MAX_STAR   80                    /* 最多星星数目         */#define  MAX_OBJ    30                    /* 最多物体(子弹和敌人) */#define  MAXY      169                  /* y坐标的最大值        */typedefunsignedlong ulong;
char far *VideoBuf=(char far*)0xa0000000L, *MemBuf;     /* MemBuf 图形二级缓存 */char  keys[128];                                        /* keys[0..100] 键盘上各键的状态   */const KEY_UP=72,KEY_DOWN=80,KEY_LEFT=75,KEY_RIGHT=77,KEY_ESC=1,KEY_CTRL=29;
                                                /* 键盘扫描码 */void interrupt far (*OldInt9)(void);                            /* 老的键盘中断程序地址 */void interrupt NewInt9(void);                                   /* 新的键盘中断服务程序 */char GameStart(void);
struct TStars { int x,y,v; } Stars[MAX_STAR];   /* 星星的描述 */struct TObj { int mode,type,index,next,x,y; } Objs[MAX_OBJ]; /* 子弹和敌人的描述 */long GameScore=0;                               /* 游戏分数 */unsignedchar fly_pic[260]={      /* 飞机的图形，我是用程序将PCX图形转换过来的 */
16,0,16,0,0,0,0,29,25,0,0,0,0,25,29,196,0,0,0,0,0,0,0,25,25,0,0,0,0,25,0,196,0,
0,0,0,112,112,0,25,196,40,40,29,112,112,40,112,0,112,112,112,40,40,112,112,40,40,
25,25,112,40,40,112,112,112,112,112,112,40,112,25,40,40,112,112,112,40,40,25,
40,112,112,0,0,112,112,25,40,40,40,112,40,40,40,25,40,112,0,0,0,0,112,25,40,
112,40,112,112,112,40,25,112,0,0,0,0,0,0,29,112,0,40,0,112,0,112,29,0,0,0,0,
0,0,0,29,112,0,40,54,112,0,112,29,0,0,0,0,0,0,0,29,0,0,40,54,112,0,0,29,0,0,
0,0,0,0,0,0,0,40,112,25,112,112,0,0,0,0,0,0,0,0,0,0,0,40,112,25,112,112,0,0,
0,0,0,0,0,0,0,0,0,40,112,112,112,112,0,0,0,0,0,0,0,0,0,0,0,0,40,112,112,0,0,
0,0,0,0,0,0,0,0,0,0,0,40,25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,0,0,0,0,0,0,0,
0};    /* 数据头四个字节分别由两个16位数据表示图片的长和宽，后面是图片8bit的颜色数据 */void copyright(void)
{ printf("Game Over. your score %ld\n",GameScore);
  printf("Thank you for play it, if you have any question please call me\n");
  printf("(0871)7167710 or lwwind@yeah.net, Lin Wei\n");
}
int game_init(void);
int game_restore(void);
void main(void)
{
  if (!game_init()) return;                    /* 游戏初始化 */
  printf("           - STAR WAR -\r");         /*  打印标题  */
  GameStart();                                 /* 游戏主循环 */
  game_restore();                              /* 还原初始化 */
  copyright();                                 /* 显示说明文 */
}
int game_init(void)
{
  long i;
  union REGS regs;
  MemBuf=(char far*)farmalloc(320L*(MAXY+1));
  if (!MemBuf) {
     printf("not enough memory\n"); return 0;
  }
  memset(MemBuf,0,320L*(MAXY+1));
  randomize();                                          /* 初始化随机函数 */for (i=0;i<128;i++) keys[i]=0;                        /* 键盘码初始化 */for (i=0;i<MAX_STAR;i++) {                            /* 星空初始化   */
      Stars[i].x=random(320);
      Stars[i].y=-20+random(250);
      Stars[i].v=1;
      if (i<MAX_STAR/3) Stars[i].v++;
  }
  for (i=0;i<MAX_OBJ;i++) Objs[i].mode=0;    /* 物体初始化 *//* 键盘中断初始化：设置新的中断 */
  OldInt9=getvect(9);
  disable();
  setvect(9,NewInt9);
  enable();
  /* 设置 320x200x256c图形模式 */
  regs.x.ax=0x13;
  int86(0x10,®s,®s);
  return 1;
}
int game_restore(void)
{
  union REGS regs;
  /* 还原设置：还原老的中断 */
  disable();
  setvect(9,OldInt9);
  enable();
  regs.x.ax=3;
  int86(0x10,®s,®s);
  farfree(MemBuf);
  return 0;
}
/* 键盘服务程序 */void interrupt NewInt9(void)
{
  unsignedchar key;
  key=inportb(0x60);                                    /* 读键盘扫描码 */if (key<0x80) keys[key]=1;                            /* 如果最高位是0,则为按下 */else keys[key&0x7f]=0;                              /* 如果最高位是1,则为放开 */
  key=inportb(0x61); key|=0x80; outportb(0x61,key);     /* 告诉键盘已接收 */
  outportb(0x61,key&0x7f);
  outportb(0x20,0x20);                                  /* 发送中断结束信号 */
}
/*---------------------------- 游戏图形引擎 ---------------------------------*/void pixel(unsigned x,unsigned y,char c)                /* 画点 */
{
  if (x>=320||y>MAXY) return;                           /* 判断范围 */
  MemBuf[(y<<8)+(y<<6)+x]=c;                            /* MemBuf[y*320+x]=c */
}
/* 将二级缓存的内容显示出来 */void show(void)
{
  int offset=(199-MAXY)*160;
  memcpy(VideoBuf+offset,MemBuf,320L*(MAXY+1));
}
/* 清屏 */void clear(void)
{
  memset(MemBuf,0,320L*(MAXY+1));
}
/* 就是绘制图块，(x,y)是坐标b是内存地址mode表示是否上下颠倒
 * 敌人和主人是同样的图片做颠倒
 */void putimage(int x,int y,char *b,int mode)
{
  int len, wid, i, j;
  len = b[0] + (int)b[1] * 256;
  wid = b[2] + (int)b[3] * 256;
  x-=len/2; y-=wid/2;                                   /* 中心对称 */for (j=0,b=b+4;j<wid;j++) for (i=0;i<len;i++,b++)
    { if (*b&&!mode) pixel(x+i,y+j,*b);                 /* 是否镜面翻转 */if (*b&&mode)  pixel(x+i,y+wid-j-1,*b);
    }
}
void drawfire(int x,int y)                              /* 画出激光 */
{ int i,j;
  for (i=-6;i<6;i++) { pixel(x-5,y+i,9); pixel(x+3,y+i,9); }
}
/*---------------------------- 游戏控制引擎 ---------------------------------*/unsignedlong timepass;                    /* 游戏进行的时间 */int  GameOver, Sound=0;
int  fly_x=160,fly_y=MAXY*2/3,fly_flag=1,fire_flag=1;
void drivers(void);                     /* 控制产生敌人的函数 */void control(void);                     /* 对象控制(事件处理) */int  AllocObj(void)                     /* 分配空余对象 */
{ int i=0;
  while (i<MAX_OBJ&&Objs[i].mode) i++;
  if (i>=MAX_OBJ) i=MAX_OBJ-1;
  return i;
}
int  CheckHit(int x1,int y1,int x2,int y2,int r)    /* 检查碰撞 */
{
  if (abs(x1-x2)<=r&&abs(y1-y2)<=r) return 1;
  return 0;
}
ulong fclock(void)                    /* 读取 1.19MHz的32位系统时钟 */
{
  ulong t;
  disable(); outportb(0x43,0); t=inportb(0x40);
  t+=(inportb(0x40)<<8); t=0xffff^t; enable();        /* (not t) + (clock()<<16) */return (clock()<<16)+t;
}
char GameStart(void)
{
  ulong start=0;
  timepass=0;
  while (!GameOver)
   {
     while (fclock()-start<=45000L); start=fclock();     /* 时间控制 */
     clear();                        /* 1.清屏. 以下5点为游戏的主循环 */
     timepass++;                     /* 2.时间基数++ */
     control();                      /* 3.事件处理   */
     drivers();                      /* 4.事件引擎   */
     show();                         /* 5.显示       */if (--Sound<=0) nosound();             /* 声音处理 */if ((timepass&3)==0) printf("%d\r",GameScore);    /* 到一定的时间更新显示分数 */if (keys[KEY_ESC]) GameOver=1;
   }
  sound(105); delay(700); nosound();                    /* Game Over发音 */return 1;
}
/* 游戏的事件处理主程序分别处理星空和物体还有主角控制
 * 通过扫描对象数组：Objs[MAX_OBJ]来完成，根据扫描到的对象属性Objs[i].mode来判断
 * 到底是什么对象：敌机或者子弹。并进入相应的处理程序：分析对象的状态，然后作出
 * 调整，并重新更改状态，这就是游戏编程同时处理众多物体的核心思想。
 */void control(void)
{
  int i,j,x,y, ok;
  /* 星星处理 */for (i=0;i<MAX_STAR;i++) {
    Stars[i].y+=Stars[i].v;       /* 向下移动 */if (Stars[i].y>MAXY) {        /* 如果移出屏幕就让它重新出现在屏幕上方某位置 */
    Stars[i].x=random(320);
    Stars[i].y=-random(60);
    }
    if (Stars[i].v==1) pixel(Stars[i].x,Stars[i].y,23);    /* 速度不一样颜色也不一样 */else pixel(Stars[i].x,Stars[i].y,28);
  }
  /* 对象处理 */for (i=0;i<MAX_OBJ;i++)                /* 扫描对象数组 */
   { x=Objs[i].x; y=Objs[i].y; ok=1;
     switch (Objs[i].mode)                /* mode表示敌人，子弹等 */
      {
    /* 敌人处理 */case 1: if (Objs[i].index==0) {
         switch (Objs[i].type)                  /* 检测敌人的三种状态 */
         { case 0: if (!random(30)) Objs[i].type=1; break; /* 3种动作状态 */case 1: if (x<fly_x) x++; if (x>fly_x) x--;
               if (!random(40)) Objs[i].type=2; break;
           case 2:
           default:if (!random(3)) y+=3;
               break;
         }
         if (CheckHit(x,y,fly_x,fly_y,15)) fly_flag=0;    /* 与主角相撞 */
         putimage(x,y,fly_pic,0);
        } else { /* 如果被击中就闪烁地绘制飞机，然后消失 */if ((Objs[i].index&3)==0) putimage(x,y,fly_pic,0);
         if (++Objs[i].index>50) Objs[i].mode=0;
        }
        y++; if (y>MAXY) Objs[i].mode=0;    /* 移出屏幕就清除 */break;
    /* 激光处理 */case 2: y-=4; if (y<-20) Objs[i].mode=0; drawfire(x,y);
        for (j=0;j<MAX_OBJ;j++)                 /* 检查与敌机相碰 */if (Objs[j].mode==1&&CheckHit(x,y,Objs[j].x,Objs[j].y,15)&&
            !Objs[j].index&&ok)            /* 击中敌机 */
         {
           Objs[j].index=1; ok=0; Objs[i].mode=0;
           sound(220); Sound=10; GameScore+=10;
         }
        break;
      }
     Objs[i].x=x; Objs[i].y=y;                /* 更新坐标 */
   }
  /* 主角控制 */if (keys[KEY_UP])  if (--fly_y<0) fly_y=0;         /* 如果上键被按下 */if (keys[KEY_DOWN]) if (++fly_y>MAXY) fly_y=MAXY;    /* 如果下键被按下 */if (keys[KEY_LEFT]) if (--fly_x<0) fly_x=0;        /* 如果左键被按下 */if (keys[KEY_RIGHT]) if (++fly_x>319) fly_x=319;    /* 如果右键被按下 */if (keys[KEY_CTRL]&&fire_flag) {             /* 如果按下CTRL */
     i=AllocObj(); Objs[i].x=fly_x,
     Objs[i].y=fly_y-10, Objs[i].mode=2; fire_flag=0;
  }
  if (!keys[KEY_CTRL]) fire_flag=1;            /* 如果放开CTRL */
  putimage(fly_x,fly_y,fly_pic,1);            /* 画出主角     */if (!fly_flag) GameOver=1;                /* 判断主角状态 */
}
/* 控制产生敌人的部分 */void drivers(void)
{
  if (random(30)==0)
  { int i=AllocObj(); Objs[i].x=random(320); Objs[i].y=-random(20);
    Objs[i].mode=1; Objs[i].type=Objs[i].index=0;
    if (!random(20)) Objs[i].type=2;
  }
}
