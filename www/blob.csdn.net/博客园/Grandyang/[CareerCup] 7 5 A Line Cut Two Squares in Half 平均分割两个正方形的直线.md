# [CareerCup] 7.5 A Line Cut Two Squares in Half 平均分割两个正方形的直线 - Grandyang - 博客园







# [[CareerCup] 7.5 A Line Cut Two Squares in Half 平均分割两个正方形的直线](https://www.cnblogs.com/grandyang/p/4774754.html)







7.5 Given two squares on a two-dimensional plane, find a line that would cut these two squares in half. Assume that the top and the bottom sides of the square run parallel to the x-axis



这道题给了我们两个正方形，让我们求一条可以讲这两个正方形平均分为两个部分的直线，前提是两个正方形都和x轴平行。那么我们首先要知道啥样的直线才能将一个正方形平均分为两部分呢，答案是任意一条过正方形中心的直线，那么将两个正方形平均分为两部分就是连接两个正方形中心的直线。这道题需要我们建立多个类，点，直线，和正方形，是一道考察面向对象程序设计的好题目。其中点类由两个double型的变量表示，直线类由两个点类表示，正方形类由左上顶点和右下顶点还有边长表示。比较重要的两个子函数extend和cut，其中extend是求从一个中心连向另一个中心，且交另一个中心的外边缘的点，至于是内边缘还是外边缘由第三个参数的正负所决定。cut函数是求连接两个中心的直线分别和两个正方形的外边缘的交点，两点确定一条直线即可。可能作为读者的你会问，为啥这么麻烦，直接将两个中点一连不就是直线了，为啥要这么麻烦，这么装B。没错，楼主就是要带你装B带你飞，这解法是为了防止人家要求满足题意的线段，那么线段两个端点正好在正方形的边上，没错，就是这么拉风，参见代码如下：



```
class Point {
public:
    double _x, _y;
    Point(double x, double y): _x(x), _y(y) {};
};

class Line {
public:
    Point _start, _end;
    Line(Point start, Point end): _start(start), _end(end) {};
};

/*
  (left, top)_________
            |         |
            |         | size
            |_________|
                      (right, down)  
*/
class Square {
public:
    double _left, _top, _right, _down, _size;
    Square(double left, double top, double right, double down, double size): _left(left), _top(top), _right(right), _down(down), _size(size) {};
    Point middle() {
        return Point((_left + _right) * 0.5, (_top + _down) * 0.5);
    }
    // Return the point whrere the line connecting mid1 and mid2 intercepts the edge of square 1.
    Point extend(Point mid1, Point mid2, double size) {
        double xdir = mid1._x < mid2._x ? -1 : 1;
        double ydir = mid1._y < mid2._y ? -1 : 1;
        if (mid1._x == mid2._x) return Point(mid1._x, mid1._y + ydir * size * 0.5);
        double slope = (mid1._y - mid2._y) / (mid1._x - mid2._x);
        double x1 = 0, y1 = 0;
        if (fabs(slope) == 1) {
            x1 = mid1._x + xdir * size * 0.5;
            y1 = mid1._y + ydir * size * 0.5;
        } else if (fabs(slope) < 1) {
            x1 = mid1._x + xdir * size * 0.5;
            y1 = slope * (x1 - mid1._x) + mid1._y;
        } else {
            y1 = mid1._y + ydir * size * 0.5;
            x1 = (y1 - mid1._y) / slope + mid1._x;
        }
        return Point(x1, y1);
    }
    // Calculate the line that connecting two mids
    Line cut(Square other) {
        Point p1 = extend(middle(), other.middle(), _size);
        Point p2 = extend(middle(), other.middle(), -1 * _size);
        Point p3 = extend(other.middle(), middle(), other._size);
        Point p4 = extend(other.middle(), middle(), -1 * other._size);
        Point start = p1, end = p1;
        vector<Point> points = {p2, p3, p4};
        for (int i = 0;i < points.size(); ++i) {
            if (points[i]._x < start._x || (points[i]._x == start._x && points[i]._y < start._y)) {
                start = points[i];
            } else if (points[i]._x > end._x || (points[i]._x == end._x && points[i]._y > end._y)) {
                end = points[i];
            }
        }
        return Line(start, end);
    }
};
```














