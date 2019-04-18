# OpenCV：简单计算曲线弧度-弓形弧度 - wishchinYang的专栏 - CSDN博客
2018年10月27日 17:00:22[wishchin](https://me.csdn.net/wishchin)阅读数：1092
在判断曲线拟合度时候，需要进行曲线曲率计算。本文中使用根据弦的方法计算曲线弧度半径，得到曲率。
首先判定是否为弧：
简单判定：不要选取较多的点，若线段不是偏向一个方向，则不为弧
```cpp
bool  isArcCurve( std::vector<cv::Point> &curve )
		{
			bool isArc = false;
			cv::Point ps, pe,vec;
			ps = cv::Point(curve[0]);
			pe = cv::Point(curve[curve.size() - 1]);
			double angle0 = colorWish::arccos(ps, pe);
			std::vector<double > rls;//左侧还是右侧
			for (int i = 1; i < curve.size() - 1; ++i) {
				double angle1 = colorWish::arccos( ps, curve[i] );
				rls.push_back( angle1 );
			}
			//若出现一次反转，则判定为否，以此保证曲线的单方向
			std::vector<bool >  rlbs;
			for ( int i = 0; i < rls.size() ; ++i ) {
				bool isRight = false;
				if ( rls[i]>angle0 ){
					isRight = true;
				} else{
					isRight = false;
				}
				rlbs.push_back( isRight );
			}
			//若出现一次反转，则判定为否，以此保证曲线的单方向
			bool isSinVec = false;
			bool isRight = false;
			if ( rlbs.size()>0 ){
				isRight = rlbs[0];
				for (int i = 1; i < rlbs.size(); ++i){
					if ( isRight!= rlbs[i]){
						isSinVec = true;
					}
				}
			}else{
			}
		
			isArc = isSinVec;
			return isArc;
		}
```
```
double colorWish::dis2(double x1, double y1, double x2, double y2) {
		return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	}
	// 计算两点的坐标夹角，360度余弦值
	double colorWish::arccos(double x0, double y0, double x1, double y1)
	{
		double angle = 0;
		double l = dis2(x0, y0, x1, y1);
		l = sqrt(l);
		if (y1 - y0 > 0) { angle = acos((x1 - x0) / l); }
		else { angle = 3.141592653 + 3.141592653 - acos((x1 - x0) / l); }
		//std::cout << (angle) << " ";
		return angle;
	}//arccos
	double colorWish::arccos(cv::Point cneter, cv::Point p) {
		return arccos(double(cneter.x), double(cneter.y), double(p.x), double(p.y));
	}
```
计算弧度值：
计算弓形曲率，使用遍历方法找弓形高（曲线上任一点），根据海伦公式计算出高
```cpp
//计算弓形的曲率-此方法仍然只保证对连续点集确定有效
		//计算方法：两端长度为弦长，高度为弓形高，以此计算曲率
		double  getArcCurvity( std::vector<cv::Point> &curve )
		{
			double curvity = 0;
			if ( curve.size()<4 ){
				return 0;
			}
			cv::Point ps, pe;
			ps = cv::Point( curve[0] );
			pe = cv::Point( curve[curve.size()-1] );
			double lArc = disCv( ps, pe );//弦长
			double hArc = 0;
			for ( int i=1;i<curve.size()-1;++i ){
				double h = p2Line( curve[i],ps,pe );//点到直线的距离
				if (hArc<h){
					hArc = h;
				}
			}
		
			//曲率公式-使用R^2 = L^2 +(R-H)^2即 R= ( L^2/H +H)/2
			double R = 0.5*( lArc*lArc/hArc +hArc );
			curvity = 1 / R;
			return curvity;
		}
```
```cpp
//点 p 到线段（p1,p2）的距离-只计算三角形情况，且只计算垂直距离
		//使用海伦公式
		double p2Line(const cv::Point &p0,const cv::Point &p1, const  cv::Point &p2)
		{
			double dis = 0;
			double a, b, c,  p, S,h;
			a = disCv(p0, p1);
			b = disCv(p0, p2);
			c = disCv(p1, p2);
			p = (a + b + c)/2;
			S = sqrt(p*(p-a)*(p - b)*(p - c) );
			h = S * 2 / a;
			dis = h ;
			return dis;
		}
```
```
double disCv(const cv::Point2f &p1, const  cv::Point2f &p2)
		{
			double dis = 0;
			dis = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
			return dis;
		}
```
弓形弧度半径公式：
double R = 0.5*( lArc*lArc/hArc +hArc );
double  curvity = 1 / R;
此种方法，比使用houghCircle方法粗糙，但简单直观。
寻找结果图像：
![](https://img-blog.csdnimg.cn/20181027170236210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_27,color_FFFFFF,t_70)
