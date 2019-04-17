# [CareerCup] 7.3 Line Intersection 直线相交 - Grandyang - 博客园







# [[CareerCup] 7.3 Line Intersection 直线相交](https://www.cnblogs.com/grandyang/p/4768309.html)







7.3 Given two lines on a Cartesian plane, determine whether the two lines would intersect.



这道题说是在[笛卡尔坐标系](https://zh.wikipedia.org/wiki/%E7%AC%9B%E5%8D%A1%E5%84%BF%E5%9D%90%E6%A0%87%E7%B3%BB)中，让我们确定两条直线是否相交。

那么我们首先要写个直线的类来表示直线，最常见的表示方法为y=kx+b，k为斜率，b为与y轴的交点，然后我们来考虑什么情况下两条直线会相交，首先，如果两条直线完全重合的话，应该也算是相交的，其次如果两条直线不平行的话，那么也是相交的，那么我们判断相交条件主要基于这两点，首先看它们的斜率是否相等，如果斜率不相等，肯定是相交的，其次看它们的于y轴的交点是否相同，如果相同肯是相交的，这样就把两条直线重合的情况也包括了。还有需要注意的是，我们不能用int型来表示k和b，而且判断相等的时候最好也不要用'=='，而是引入epsilon，赋一个超小值，只要两个数之差小于epsilon，我们就可认定它们相等，反之大于epsilon，则不等。



```
class Line {
public:
    const static double _epsilon = 0.00001;
    double _slope;
    double _yintercept;
    Line(double s, double y): _slope(s), _yintercept(y) {};
    bool intersect(Line line2) {
        return abs(_slope - line2._slope) > _epsilon || abs(_yintercept - line2._yintercept) < _epsilon;
    }  
};
```














