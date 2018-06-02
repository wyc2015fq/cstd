www.pudn.com _ tin6.rar _ Unit1.cpp_ change_2004-04-18_size_10911b

    

  
//---------------------------------------------------------------------------   
   
#include <vcl.h>   
#pragma hdrstop   
#include <fstream.h>   
#include<iostream.h>   
#include<math.h>   
#include "Unit1.h"   
#include <stdlib.h>   
//---------------------------------------------------------------------------   
#pragma package(smart_init)   
#pragma resource "*.dfm"   
   
TForm1 *Form1;   
//---------------------------------------------------------------------------   
__fastcall TForm1::TForm1(TComponent* Owner)   
        : TForm(Owner)   
{   
}   
//---------------------------------------------------------------------------   
   
void __fastcall TForm1::N2Click(TObject *Sender)   
{   
if(OpenDialog1->Execute())   
{   
        fstream file(OpenDialog1->FileName.c_str());   
        int i=0,j,n_Tin;   
        double max_x=0.0,max_y=0.0,min_x=10000.0,min_y=10000.0;   
        while(!file.eof())   
        {   
        file>>Points[i].index;   
        file>>Points[i].x;   
        if(max_x<Points[i].x)   
        max_x=Points[i].x;   
        if(min_x>Points[i].x)   
        min_x=Points[i].x;   
        file>>Points[i].y;   
        if(max_y<Points[i].y)   
        max_y=Points[i].y;   
        if(min_y>Points[i].y)   
        min_y=Points[i].y;   
        i=i+1;   
        }   
        center_x=(max_x+min_x)/2;   
        center_y=(max_y+min_y)/2;   
        for(i=0;i<n_Points;i++)   
        for(j=0;j<n_Points;j++)   
        {   
                Edges[i][j]=0;   
        }   
        n_Tin=Tin();   
        DrawTin(n_Tin);   
}   
}   
   
int TForm1::Tin()                             //生成三角网数据   
{   
        double tem=1.E+30,tem1;   
        int L=0,K=0,i,j;   
        TriangleNet[L].index=0;   
        for(i=0;i<n_Points;i++)                //找到第一个点   
        {   
                tem1=Dis(Points[i].x,Points[i].y,center_x,center_y);   
                if(tem1<tem)   
                {   
                        TriangleNet[L].NodeA=i;   
                        tem=tem1;   
                }   
        }   
                tem=1.E+30;   
        for(i=0;i<n_Points;i++)               //找到第二个点   
        {   
                if(i!=TriangleNet[L].NodeA)   
                {   
                        tem1=Dis(Points[TriangleNet[L].NodeA].x,Points[TriangleNet[L].NodeA].y,Points[i].x,Points[i].y);   
                        if(tem1<tem)   
                                {   
                                        TriangleNet[L].NodeB=i;   
                                        tem=tem1;   
                        }   
                }   
        }   
                tem=1.0E+30;   
        for(i=0;i<n_Points;i++)                //找第三个点   
        {   
                if(i!=TriangleNet[L].NodeA&&i!=TriangleNet[L].NodeB)   
                {   
                        tem1=cosc(TriangleNet[L].NodeA,TriangleNet[L].NodeB,i);   
                        if(tem1>0&&tem1<tem)   
                        {   
                        tem=tem1;   
                        TriangleNet[L].NodeC=i;   
                        }   
                }   
        }   
        K++;   
        Edges[TriangleNet[L].NodeA][TriangleNet[L].NodeB]++;   
        Edges[TriangleNet[L].NodeB][TriangleNet[L].NodeC]++;   
        Edges[TriangleNet[L].NodeC][TriangleNet[L].NodeA]++;   
        int NagB_Point;   
        while(K!=L||L==0)   
        {   
                if(Edges[TriangleNet[L].NodeA][TriangleNet[L].NodeB]+Edges[TriangleNet[L].NodeB][TriangleNet[L].NodeA]!=2)   
                {   
                NagB_Point=NagBPoint(TriangleNet[L].NodeA,TriangleNet[L].NodeB,TriangleNet[L].NodeC);   
                if(NagB_Point!=-1)   
                {   
                        TriangleNet[K].NodeA=TriangleNet[L].NodeA;   
                        TriangleNet[K].NodeB=TriangleNet[L].NodeB;   
                        TriangleNet[K].NodeC=NagB_Point;   
                        TriangleNet[K].index=K;   
                        TriangleNet[K].AdjTriangleC=L;   
                        TriangleNet[L].AdjTriangleC=K;   
                        Edges[TriangleNet[K].NodeA][TriangleNet[K].NodeB]++;   
                        Edges[TriangleNet[K].NodeB][TriangleNet[K].NodeC]++;   
                        Edges[TriangleNet[K].NodeC][TriangleNet[K].NodeA]++;   
                        K++;   
                        }   
                else   
                TriangleNet[L].AdjTriangleC=-1;   
                }   
                if(Edges[TriangleNet[L].NodeB][TriangleNet[L].NodeC]+Edges[TriangleNet[L].NodeC][TriangleNet[L].NodeB]!=2)   
                {   
                NagB_Point=NagBPoint(TriangleNet[L].NodeB,TriangleNet[L].NodeC,TriangleNet[L].NodeA);   
                if(NagB_Point!=-1)   
                {   
                        TriangleNet[K].NodeA=TriangleNet[L].NodeB;   
                        TriangleNet[K].NodeB=TriangleNet[L].NodeC;   
                        TriangleNet[K].NodeC=NagB_Point;   
                        TriangleNet[K].index=K;   
                        TriangleNet[K].AdjTriangleC=L;   
                        TriangleNet[L].AdjTriangleA=K;   
                        Edges[TriangleNet[K].NodeA][TriangleNet[K].NodeB]++;   
                        Edges[TriangleNet[K].NodeB][TriangleNet[K].NodeC]++;   
                        Edges[TriangleNet[K].NodeC][TriangleNet[K].NodeA]++;   
                        K++;   
                        }   
                else   
                TriangleNet[L].AdjTriangleA=-1;   
                }   
                if(Edges[TriangleNet[L].NodeC][TriangleNet[L].NodeA]+Edges[TriangleNet[L].NodeA][TriangleNet[L].NodeC]!=2)   
                {   
                NagB_Point=NagBPoint(TriangleNet[L].NodeC,TriangleNet[L].NodeA,TriangleNet[L].NodeB);   
                if(NagB_Point!=-1)   
                {   
                        TriangleNet[K].NodeA=TriangleNet[L].NodeC;   
                        TriangleNet[K].NodeB=TriangleNet[L].NodeA;   
                        TriangleNet[K].NodeC=NagB_Point;   
                        TriangleNet[K].index=K;   
                        TriangleNet[K].AdjTriangleC=L;   
                        TriangleNet[L].AdjTriangleB=K;   
                        Edges[TriangleNet[K].NodeA][TriangleNet[K].NodeB]++;   
                        Edges[TriangleNet[K].NodeB][TriangleNet[K].NodeC]++;   
                        Edges[TriangleNet[K].NodeC][TriangleNet[K].NodeA]++;   
                        K++;   
                        }   
                else   
                TriangleNet[L].AdjTriangleB=-1;   
                }   
                L++;   
        }   
        return L;   
}   
   
   
void TForm1::DrawTin(int n)                               //画出三角网   
{   
        int i,j;   
        for(i=0;i<n_Points;i++)   
        for(j=0;j<n_Points;j++)   
        {   
                Edges[i][j]=0;   
        }   
        for(i=0;i<n;i++)   
        {   
                Draw(TriangleNet[i]);   
                //Edges[TriangleNet[i].NodeA][TriangleNet[i].NodeB]++;   
                //Edges[TriangleNet[i].NodeB][TriangleNet[i].NodeC]++;   
                //Edges[TriangleNet[i].NodeC][TriangleNet[i].NodeA]++;   
        }   
}   
//--------------------------------------------------------------------------   
int TForm1::NagBPoint(int Point_1,int Point_2,int Point_3)     //NagetiveBiggestPoint   
{   
//求出与点Point_3在线段Point_1Point_2一侧相反的点中与线段形成的夹角最大的点号   
        bool flag=IsPositive(Point_1,Point_2,Point_3);   
        bool flag1;   
        double tem=10E+30,tem1;   
        int n=-1;   
        for(int i=0;i<n_Points;i++)   
        {   
                if(i!=Point_1&&i!=Point_2&&Point_3)   
                {   
                        flag1=IsPositive(Point_1,Point_2,i);   
                        if(flag1!=flag)   
                        {   
                                tem1=cosc(Point_1,Point_2,i);   
                                if(tem1<=tem)   
                                {   
                                        tem=tem1;   
                                        n=i;   
                                        }   
                                }   
                        }   
                }   
        return n;   
}   
//-------------------------------------------------------------------------   
double TForm1::Dis(double x1,double y1,double x2,double y2)   
{   
        return abs(x1-x2)+abs(y1-y2);   
}   
//-------------------------------------------------------------------------   
double TForm1::cosc(int NodeA,int NodeB,int NodeC)   
{   
        double aa,bb,cc;   
        aa=(Points[NodeB].x-Points[NodeC].x)*(Points[NodeB].x-Points[NodeC].x)+(Points[NodeB].y-Points[NodeC].y)*(Points[NodeB].y-Points[NodeC].y);   
        bb=(Points[NodeA].x-Points[NodeC].x)*(Points[NodeA].x-Points[NodeC].x)+(Points[NodeA].y-Points[NodeC].y)*(Points[NodeA].y-Points[NodeC].y);   
        cc=(Points[NodeB].x-Points[NodeA].x)*(Points[NodeB].x-Points[NodeA].x)+(Points[NodeB].y-Points[NodeA].y)*(Points[NodeB].y-Points[NodeA].y);   
        return (aa+bb-cc)/(2*sqrt(aa)*sqrt(bb));   
   
}   
//-------------------------------------------------------------------------   
void TForm1::Draw(struct TriangleNet TNet)              //画三角形   
{   
        //注释部分是优化代码   
        //判断要画的边是否已被画过,是则跳过,否则画   
        int height=300;   
        Form1->Canvas->TextOutA(Points[TNet.NodeA].x,height-Points[TNet.NodeA].y,TNet.NodeA+1);   
        Form1->Canvas->TextOutA(Points[TNet.NodeB].x,height-Points[TNet.NodeB].y,TNet.NodeB+1);   
        Form1->Canvas->TextOutA(Points[TNet.NodeC].x,height-Points[TNet.NodeC].y,TNet.NodeC+1);   
        //if(Edges[TNet.NodeA][TNet.NodeB]+Edges[TNet.NodeB][TNet.NodeA]==0)   
        //{   
        Form1->Canvas->MoveTo(Points[TNet.NodeA].x,height-Points[TNet.NodeA].y);   
        Form1->Canvas->LineTo(Points[TNet.NodeB].x,height-Points[TNet.NodeB].y);   
        //}   
        //if(Edges[TNet.NodeB][TNet.NodeC]+Edges[TNet.NodeC][TNet.NodeB]==0)   
        //{   
        //Form1->Canvas->MoveTo(Points[TNet.NodeB].x,height-Points[TNet.NodeB].y);   
        Form1->Canvas->LineTo(Points[TNet.NodeC].x,height-Points[TNet.NodeC].y);   
        //}   
        //if(Edges[TNet.NodeC][TNet.NodeA]+Edges[TNet.NodeA][TNet.NodeC]==0)   
        //{   
        //Form1->Canvas->MoveTo(Points[TNet.NodeC].x,height-Points[TNet.NodeC].y);   
        Form1->Canvas->LineTo(Points[TNet.NodeA].x,height-Points[TNet.NodeA].y);   
        //}   
}   
//-------------------------------------------------------------------------   
bool TForm1::IsPositive(int Point_1,int Point_2,int Point)      //判断点是否在线段的正方向   
{   
        double a,b,f;   
        a=(Points[Point_2].y-Points[Point_1].y)/(Points[Point_2].x-Points[Point_1].x);   
        b=(Points[Point_1].y*Points[Point_2].x-Points[Point_2].y*Points[Point_1].x)/(Points[Point_2].x-Points[Point_1].x);   
        f=Points[Point].y-a*Points[Point].x-b;   
        if(f>=0)   
        return true;   
        else   
        return false;   
}   
//---------------------------------------------------------------------------   
   
void __fastcall TForm1::N3Click(TObject *Sender)   
{   
        Application->Terminate();   
}   
//---------------------------------------------------------------------------   
   

 
