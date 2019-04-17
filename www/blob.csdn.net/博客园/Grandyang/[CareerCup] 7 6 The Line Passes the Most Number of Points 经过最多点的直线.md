# [CareerCup] 7.6 The Line Passes the Most Number of Points 经过最多点的直线 - Grandyang - 博客园







# [[CareerCup] 7.6 The Line Passes the Most Number of Points 经过最多点的直线](https://www.cnblogs.com/grandyang/p/4779741.html)







7.6 Given a two-dimensional graph with points on it, find a line which passes the most number of points.



这道题给了我们许多点，让我们求经过最多点的一条直线。给之前那道[7.5 A Line Cut Two Squares in Half 平均分割两个正方形的直线](http://www.cnblogs.com/grandyang/p/4774754.html)一样，都需要自己写出点类和直线类。在直线类中，我用我们用斜率和截距来表示直线，为了应对斜率不存在情况，我们还需用一个flag来标记是否为垂直的线。在直线类中，我们要有判断两条直线是否相等的函数。判断相等的方法和之前那道[7.3 Line Intersection 直线相交](http://www.cnblogs.com/grandyang/p/4768309.html)相同，都需要使用epsilon，只要两个数的差值的绝对值小于epsilon，我们就认定是相等的。对于给定的所有点，每两个点能组成一条直线，我们的方法是遍历所有的直线，把所有相同的直线都存入哈希表中，key是直线的斜率，映射关系是斜率和直线集合的映射，那么我们只需找到包含直线最多的那个集合即可，参见代码如下：



```
class Point {
public:
    double _x, _y;
    Point(double x, double y): _x(x), _y(y) {};
};

class Line {
public:
    static constexpr double _epsilon = 0.0001;
    double _slope, _intercept;
    bool _infi_slope = false;
    Line(Point p, Point q) {
        if (fabs(p._x - q._x) > _epsilon) {
            _slope = (p._y - q._y) / (p._x - q._x);
            _intercept = p._y - _slope * p._x;
        } else {
            _infi_slope = true;
            _intercept = p._x;
        }
    }
    static double floorToNearestEpsilon(double d) {
        int r = (int)(d / _epsilon);
        return ((double)r) * _epsilon;
    }
    bool isEquivalent(double a, double b) {
        return (fabs(a - b) < _epsilon);
    }
    bool isEquivalent(Line other) {
        if (isEquivalent(_slope, other._slope) && isEquivalent(_intercept, other._intercept) && (_infi_slope == other._infi_slope)) {
            return true;
        }
        return false;
    }
};

class Solution {
public:
    Line findBestLine(vector<Point> &points) {
        Line res(points[0], points[1]);
        int bestCnt = 0;
        unordered_map<double, vector<Line> > m;
        for (int i = 0; i < (int)points.size(); ++i) {
            for (int j = i + 1; j < (int)points.size(); ++j) {
                Line line(points[i], points[j]);
                insertLine(m, line);
                int cnt = countEquivalentLines(m, line);
                if (cnt > bestCnt) {
                    res = line;
                    bestCnt = cnt;
                }
            }
        }
        return res;
    }
    void insertLine(unordered_map<double, vector<Line> > &m, Line &line) {
        vector<Line> lines;
        double key = Line::floorToNearestEpsilon(line._slope);
        if (m.find(key) != m.end()) {
            lines = m[key];
        } else {
            m[key] = lines;
        }
        lines.push_back(line);
    }
    int countEquivalentLines(unordered_map<double, vector<Line> > &m, Line &line) {
        double key = Line::floorToNearestEpsilon(line._slope);
        double eps = Line::_epsilon;
        return countEquivalentLines(m[key], line) + countEquivalentLines(m[key - eps], line) + countEquivalentLines(m[key + eps], line);
    }
    int countEquivalentLines(vector<Line> &lines, Line &line) {
        if (lines.empty()) return 0;
        int res = 0;
        for (auto &a : lines) {
            if (a.isEquivalent(line)) ++res;
        }
        return res;
    }
};
```














