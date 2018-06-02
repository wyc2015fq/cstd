

#ifndef SHAREDMSTTING_H
#define SHAREDMSTTING_H

#include "cstd.h"
#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <vector>

using namespace std;

struct labelPoint
{
    int x;
    int y;
    int label;
};
struct Point {
  int x, y;
  Point(int x_=0, int y_=0) {
    x = x_;
    y = y_;
  }
};
struct Scalar {
  double val[4];
  Scalar() {}
  static Scalar all(double x) {return Scalar(x,x,x,x);}
  Scalar(double r, double g, double b, double a=0) {
    val[0] = r;
    val[1] = g;
    val[2] = b;
    val[3] = a;
  }
};

struct Tuple
{
    Scalar f;
    Scalar b;
    double   sigmaf;
    double   sigmab;
    
    int flag;
    
};

struct Ftuple
{
    Scalar f;
    Scalar b;
    double   alphar;
    double   confidence;
};

/*程序中认定Point中 x为行，y为列，可能错误，但对程序结果没有影响*/
class SharedMatting
{
public:
    SharedMatting();
    virtual ~SharedMatting();
    
    void loadImage(const char * filename);
    void loadTrimap(const char * filename);
    
    void solveAlpha();
    void save(const char * filename);
    
protected:
    void expandKnown();
    void gathering();
    void refineSample();
    void localSmooth();
    void sample(vector<vector<Point> > &F, vector<vector<Point> > &B);
    void getMatte();
    
    
private:
    double chromaticDistortion(int i, int j, Scalar f, Scalar b);
    double neighborhoodAffinity(int i, int j, Scalar f, Scalar b);
    
    
    double aP(int i, int j, double pf, Scalar f, Scalar b);
    double gP(Point p, Point fp, Point bp, double distance, double probability);
    
    double energyOfPath(int i1, int j1, int i2, int j2);
    double probabilityOfForeground(Point p, vector<Point>& f, vector<Point>& b);
    double pixelDistance(Point s, Point d);
    double colorDistance2(Scalar cs1, Scalar cs2);
    double sigma2(Point p);
    double comalpha(Scalar c, Scalar f, Scalar b);
    
private:
    img_t pImg[1];
    img_t matte[1];
    
    vector<Point> unknownSet;
    vector<struct Tuple> tuples;
    vector<struct Ftuple> ftuples;
    
    int height;
    int width;
    int ** unknownIndex;//Unknown的索引信息；
    int ** m_ppTriData;
    int ** alpha;
    
    size_t step;
    int channels;
    uchar* data;
    
};

#endif

