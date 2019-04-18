# OpenCV：判定曲线为弧线的简单方法 - wishchinYang的专栏 - CSDN博客
2018年10月27日 17:14:39[wishchin](https://me.csdn.net/wishchin)阅读数：1031
个人分类：[OpenCV																[C++编程																[STL/算法](https://blog.csdn.net/wishchin/article/category/2626239)](https://blog.csdn.net/wishchin/article/category/1508333)](https://blog.csdn.net/wishchin/article/category/5778933)
借鉴了一个前同事的一个代码段（良心代码），功能为判定曲线为弧线，并且给出拟合度；
代码段：
```cpp
//寻找所有canny 弧
		int findArcsOfAllCanny( cv::Mat &inMatCanny,
			std::vector<std::vector<std::pair< cv::Point, float> > > &circlesPupil, cv::RotatedRect  &ecf, int method)
		{
			//简单方法，链接各个连通域的质心，拼凑成一个连通域
			std::vector<std::vector<cv::Point> >  all_contours;
			std::vector<cv::Vec4i>              hierarchy;
			cv::findContours(inMatCanny, all_contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);//连续点
			int minSize = 20;
			int maxSize =1000;
#ifdef SHOW_TEMP
			cv::Mat canvas = cv::Mat::zeros(inMatCanny.rows, inMatCanny.cols, CV_8UC3);
			cv::bitwise_not(canvas, canvas);
			for (int i = 0; i < all_contours.size(); i++) {
				if (all_contours[i].size() > minSize) {
					cv::Mat canvas2 = canvas.clone();
					cv::drawContours(canvas, all_contours, i, cv::Scalar(0, 0, 255));
					cv::imshow("canvasEdgeSrc", canvas); cv::waitKey(1);
				}
			}
#endif
			//在找出所有的弧，并计算出弧度
			int kSegLen = 6;//设定固定寻找弦长度
			if ( 2== method){
				kSegLen = 10;
			}else{
				kSegLen = 8;
			}
			std::vector<std::vector<cv::Point> > edgesValid(0);
			for (int i = 0; i < all_contours.size(); i++) {
				if (all_contours[i].size() > minSize && all_contours[i].size() < maxSize ) {
					//添加
					std::vector<std::vector<cv::Point> > curves = divideCurveByCurvity( all_contours[i], kSegLen );
					for ( int k=0;k< curves.size();++k ){
						if ( curves[k].size()>=5 ) {
							edgesValid.push_back(curves[k]); //
						}
					}
				}
			}
			//在找出所有的弧，并计算出弧度
			circlesPupil.resize(0);
			{
				int counter = edgesValid.size();
				std::vector<std::vector<cv::Point> > convex_contour(counter);//(all_contours.size() );
				//std::vector<cv::Point>          approx_poly;//影响了后面的程序
				std::vector<int>                apdNum(counter);//(all_contours.size() );
				std::vector<cv::RotatedRect >   rects;
				std::vector<float>              sigmas(counter);//(all_contours.size() );
				//float mv = 99999; int uind = 0;
				//1.发现凸边缘
				int idx = 0;
				for (int i = 0; i < edgesValid.size(); i++)
				{
					float mv = 99999; int uind = 0;
					std::vector<cv::Point>          approx_poly;
					bool showTemp = true;
					if (showTemp) {
						cv::Mat canvas( 200, 200, CV_8UC3 );
						for (int k = 0; k < edgesValid[i].size(); ++k)
							canvas.at<cv::Vec3b>(edgesValid[i][k].y, edgesValid[i][k].x) = cv::Vec3b(255, 0, 0);
						cv::imshow("edgesValid", canvas); cv::waitKey(10);
					}
					cv::convexHull(edgesValid[i], convex_contour[i], false, true);//放大时插值图片可否保持凸包情形
					if (showTemp) {
						cv::Mat canvas(200, 200, CV_8UC3);
						for (int k = 0; k < convex_contour[i].size(); ++k)
							canvas.at<cv::Vec3b>(convex_contour[i][k].y, convex_contour[i][k].x) = cv::Vec3b(255, 0, 0);
						cv::imshow("convex_contour", canvas); cv::waitKey(10);
					}
					float ts = isCircle(convex_contour[i]);
					sigmas[i] = ts;
					cv::RotatedRect rr = cv::minAreaRect(convex_contour[i]);
					rects.push_back(rr);
					cv::approxPolyDP(convex_contour[i], approx_poly, 1, true);
					apdNum[i] = approx_poly.size();
					if (mv > ts && apdNum[i] >= 10) {
						mv = ts;
						uind = i;
					}
				}
				//2.判定所有弧
				int minPs = 5;
				std::vector<std::vector<cv::Point2f> >  arc_pts(0);
				std::vector<int>        ulist, uInds;
				for (int i = 0; i < convex_contour.size(); ++i) {
					std::vector<cv::Point2f>   arc_pt(0);
					bool isFind = findArc2(convex_contour[i], arc_pt, ulist);//函数有问题，对于一个点/两个相同的点 也判断为弧！
					if (isFind && arc_pt.size() >= minPs) {//if (isFind)
						arc_pts.push_back(arc_pt);
						uInds.push_back(i);
					}
				}
				/////////////////////////////////////////////////////
				//2.1 判断弧的拟合度
				for (int i = 0; i < arc_pts.size(); ++i) {
					sortPoints(arc_pts[i]);
					sampleArc(arc_pts[i]);
				}
				circlesPupil.resize(uInds.size());
				if (circlesPupil.size() < 1) { return 0; }
				for (int i = 0; i < uInds.size(); ++i) {
					circlesPupil[i].resize(convex_contour[uInds[i]].size());
					for (int j = 0; j < convex_contour[uInds[i]].size(); ++j) {
						circlesPupil[i][j].first = convex_contour[uInds[i]][j];
					}
				}
#ifdef SHOW_TEMP
				cv::Mat canvasV = cv::Mat::zeros(inMatCanny.rows, inMatCanny.cols, CV_8UC3);
				cv::bitwise_not(canvasV, canvasV);
				for (int i = 0; i < circlesPupil.size(); i++) {
					for (int j = 0; j < circlesPupil[i].size(); j++) {
						cv::circle(canvasV, circlesPupil[i][j].first, 1, (0, 0, 255), 1, 8, 0);
					}
					cv::imshow("canvasValid", canvasV); cv::waitKey(1);
				}
#endif
			}
			return 1;
		}//findArcsOfAllCanny
```
```cpp
bool findArc2(const std::vector<cv::Point>& hpts, std::vector<cv::Point2f>& arc_pts, std::vector<int>& inds)
		{
			arc_pts.clear();
			if (hpts.size() <= 2) { return false; }
			std::vector<edgePoint>epts(hpts.size());
			for (int i = 0; i < hpts.size(); i++) {
				cv::Vec3d v1, v2, v3;
				if (i == 0) {
					v1 = cv::Vec3d(hpts[0].x - hpts[hpts.size() - 1].x,
						hpts[0].y - hpts[hpts.size() - 1].y, 0);
					v2 = cv::Vec3d(hpts[1].x - hpts[0].x,
						hpts[1].y - hpts[0].y, 0);
				}
				else if (i == hpts.size() - 1) {
					v1 = cv::Vec3d(hpts[i].x - hpts[i - 1].x,
						hpts[i].y - hpts[i - 1].y, 0);
					v2 = cv::Vec3d(hpts[0].x - hpts[i].x,
						hpts[0].y - hpts[i].y, 0);
				}
				else {
					v1 = cv::Vec3d(hpts[i].x - hpts[i - 1].x,
						hpts[i].y - hpts[i - 1].y, 0);
					v2 = cv::Vec3d(hpts[i + 1].x - hpts[i].x,
						hpts[i + 1].y - hpts[i].y, 0);
				}
				float len1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
				float len2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
				v1[0] /= len1; v1[1] /= len1; v1[2] /= len1;
				v2[0] /= len2; v2[1] /= len2; v2[2] /= len2;
				v3 = VecCross(v1, v2);
				epts[i].angle = sqrt(v3[0] * v3[0] + v3[1] * v3[1] + v3[2] * v3[2]);
				epts[i].ind = i;
				epts[i].pos = hpts[i];
			}
			cv::sort(epts, cmp);
			inds.push_back(epts[0].ind);
			for (int i = 1; i < epts.size(); i++) {
				float len = epts[i] * epts[0];//运算符重载失效，为何？
				//float len =0;
				//edgePoint a = epts[i]; const edgePoint b= epts[0];
				//len = sqrt((double)(a.pos.x - b.pos.x) * (a.pos.x - b.pos.x) +(a.pos.y - b.pos.y) * (a.pos.y - b.pos.y));
				if (len <= 10)
					continue;
				else {
					inds.push_back(epts[i].ind);
					break;
				}
			}
			if (inds.size() <= 1) { return false; }
			//inds.push_back(epts[1].ind);
			int leftlen = std::max(inds[0], inds[1]) - std::min(inds[0], inds[1]);
			int rightlen = hpts.size() - leftlen;
			if (inds[0] < hpts.size() && inds[1] < hpts.size())//附件！wishchin!!!排除大下标
			{
				if (leftlen > rightlen) {
					for (int i = 0; i < hpts.size(); i++) {
						if (i >= std::min(inds[0], inds[1]) && i <= std::max(inds[0], inds[1])) {
							if (!cvWish::line::isLinear(hpts[inds[0]], hpts[inds[1]], hpts[i])) {
								arc_pts.push_back(hpts[i]);
							}
						}
					}
				}
				else {
					for (int i = 0; i < hpts.size(); i++) {
						if (i > std::min(inds[0], inds[1]) && i < std::max(inds[0], inds[1])) {
							//arc_pts.push_back(hpts[i]);
						}
						else {
							//std::cout<<"id = "<<i<<endl;
							if (!cvWish::line::isLinear(hpts[inds[0]], hpts[inds[1]], hpts[i])) {
								arc_pts.push_back(hpts[i]);
							}
						}
					}
				}
				arc_pts.push_back(hpts[inds[0]]);
				arc_pts.push_back(hpts[inds[1]]);
				return true;
			}
			else		return false;
		}//findArc2
```
```cpp
inline cv::Vec3d VecCross(const cv::Vec3d& p1, const cv::Vec3d& p2)
{
	return cv::Vec3d(
		p1[1]*p2[2]-p1[2]*p2[1],p1[2]*p2[0]-p1[0]*p2[2],
		p1[0]*p2[1]-p1[1]*p2[0]
	);
}
```
```cpp
//判断是否在一条线上
			bool isLinear(const cv::Point2f& a, const cv::Point2f& b, const cv::Point2f& c)
			{
				cv::Point2f a1 = b - a;
				float alen = b * a;
				if (alen <= 0.5)
					return true;
				a1.x /= alen; a1.y /= alen;
				cv::Point2f a2 = c - a;
				float alen2 = c * a;
				if (alen2 <= 0.5)
					return true;
				a2.x /= alen2; a2.y /= alen2;
				cv::Vec3d cc = VecCross(cv::Vec3d(a1.x, a1.y, 0), cv::Vec3d(a2.x, a2.y, 0));
				float len = abs(cc[2]);
				//std::cout<<"len = "<<len<<endl;
				if (len <= 0.02)
					return true;
				return false;
			}
		};
```
```cpp
//判断是否是圆//方法不怎么地！
		float isCircle(const std::vector<cv::Point>& hull)
		{
			cv::RotatedRect r = cv::minAreaRect(hull);
			std::vector<cv::Point2f>pts;
			getRectBottomLine(r, pts);
			cv::Point2f center = cv::Point2f((pts[0].x + pts[1].x) / 2.0, (pts[0].y + pts[1].y) / 2.0);
			float mean_v = 0;
			std::vector<float>lens(hull.size());
			for (int i = 0; i < hull.size(); i++) {
				float tlen = sqrt((hull[i].x - center.x) * (hull[i].x - center.x) +
					(hull[i].y - center.y) * (hull[i].y - center.y));
				lens[i] = tlen;
				mean_v += tlen;
			}
			mean_v /= hull.size();
			float sigma = 0.0;
			for (int i = 0; i < hull.size(); i++) {
				sigma += (lens[i] - mean_v) * (lens[i] - mean_v);
			}
			sigma = sqrt(sigma);
			sigma /= hull.size();
			return sigma;
		}//isCircle
```
```cpp
void getRectBottomLine(const cv::RotatedRect& r, std::vector<cv::Point2f>& pts)
		{
			pts.resize(2);
			//std::vector<cv::Point2f>pts;
			//cv::Point2f * pp = new cv::Point2f(4);
			//r.points(pp);
			//std::vector<cv::Point2f>  pp(4);// = new cv::Point2f(4);
			cv::Point2f pp[4];
			r.points(pp);
			float mid_x = 0;
			for (int i = 0; i < 4; i++) {
				mid_x += pp[i].x;
			}
			mid_x /= 4;
			float lmax = -99999, rmax = -99999;
			int lu = 0, ru = 0;
			for (int i = 0; i < 4; i++) {
				if (pp[i].x < mid_x) {
					if (lmax < pp[i].y) {
						lmax = pp[i].y;
						lu = i;
					}
				}
				else if (pp[i].x >= mid_x) {
					if (rmax < pp[i].y) {
						rmax = pp[i].y;
						ru = i;
					}
				}
			}
			//cv::Point2f pSpl;
			pts[0].x = pp[lu].x; pts[0].y = pp[lu].y;
			//cv::Point2f pSpr =pp[ru];
			pts[1].x = pp[ru].x; pts[1].y = pp[ru].y;
			//pts.push_back(pp[lu]);
			//pts.push_back(pp[ru]);
			//pts.push_back(pSpl );
			//pts.push_back(pSpr );
			//delete [] pp;
			return;
		}//getRectBottomLine
```
```cpp
//排序点集；不使用qsort//仅用于寻找 合适的 弧
void sortPoints( std::vector<cv::Point2f>& pts )
{
	int pSize = pts.size();
	cv::Point2f LeftPoint = pts[pSize-1];
	cv::Point2f RightPoint = pts[pSize-2];
	if(LeftPoint.x > RightPoint.x)
	{
		swapPoint(LeftPoint , RightPoint);
	}
	int mSize = pSize / 2;
	cv::Point2f mPoint = pts[mSize];
	int caseFlag = 1;
	if( mPoint.y < std::min( LeftPoint.y , RightPoint.y ) )
	{
		caseFlag = 2;
	}
	std::vector<cv::Point2f> newpts;
	std::vector<int>visited(pSize-2, 0);
	if(caseFlag == 1)
	{
		newpts.push_back(RightPoint);
	}
	else
	{
		newpts.push_back(LeftPoint);
	}
	for(int i=0;i<pSize-2;i++)
	{
		float dis = 9999.0;
		int uind = 0;
		cv::Point2f pTop = newpts[newpts.size()-1];
		for(int j=0;j<pSize-2;j++)
		{
			if(!visited[j])
			{
				float dis_j = pts[j] * pTop;
				if(dis > dis_j)
				{
					dis = dis_j;
					uind = j;
				}
			}
		}
		newpts.push_back(pts[uind]);
		visited[uind] = 1;
	}
	if(caseFlag == 1)
	{
		newpts.push_back(LeftPoint);
	}
	else
	{
		newpts.push_back(RightPoint);
	}
	pts.clear();
	pts = newpts;
	return;
}
```
```cpp
inline void swapPoint(cv::Point2f& a, cv::Point2f& b)
{
	cv::Point2f c;
	c = b; b= a; a = c;
}
```
```cpp
//确定弧的弧的相似性质
		void sampleArc(std::vector<cv::Point2f>& arc_points)
		{
			std::vector<cv::Point2f> new_arc;
			std::vector<float> arc_dis;
			float max_dis = 99999; int uind = 0;
			for (int i = 1; i < arc_points.size(); i++) {
				float tdis = arc_points[i] * arc_points[i - 1];
				arc_dis.push_back(tdis);
				if (max_dis > tdis) {
					max_dis = tdis;
					uind = i;
				}
			}
			for (int i = 0; i < arc_dis.size(); i++) {
				int sampleNum = floor(arc_dis[i] / max_dis);
				cv::Point2f n = arc_points[i + 1] - arc_points[i];
				float nLen = arc_points[i + 1] * arc_points[i];
				n.x /= nLen; n.y /= nLen;
				for (int j = 0; j < sampleNum; j++) {
					cv::Point2f tp = arc_points[i];
					tp.x += j * max_dis * n.x;
					tp.y += j * max_dis * n.y;
					new_arc.push_back(tp);
				}
				cv::Point2f ltp = arc_points[i];
				ltp.x += sampleNum * max_dis * n.x;
				ltp.y += sampleNum * max_dis * n.y;
				float tLen = ltp * arc_points[i + 1];
				if (tLen < 0.5 * max_dis) {
				}
				else {
					new_arc.push_back(ltp);
				}
			}
			float lastDis = arc_points[arc_points.size() - 1] * new_arc[new_arc.size() - 1];
			if (lastDis < 0.5 * max_dis) {
				new_arc[new_arc.size() - 1] = arc_points[arc_points.size() - 1];
			}
			else {
				new_arc.push_back(arc_points[arc_points.size() - 1]);
			}
			arc_points.clear();
			arc_points = new_arc;
		}
```
分割弧线的方法：
```cpp
//根据曲率断开曲线-在曲线首尾处也需要判断是否连接！
		std::vector<std::vector<cv::Point> >  divideCurveByCurvity( std::vector<cv::Point> &curve, const int k)
		{
			std::vector<std::vector<cv::Point> > curves;
			//拟合椭圆的效果不好，不能贴合边，而是每个点最小二乘
			//cv::RotatedRect ec;
			//if ( curve.size()>5 ){
			//	ec = cv::fitEllipse(curve);
			//	cv::Mat canvas(80, 80, CV_8UC3);
			//	cv::ellipse(canvas, ec, cv::Scalar(0, 0, 255), 1, 8);
			//	cv::imshow("ellipse", canvas); cv::waitKey(100);
			//}
			bool showTemp = true;
			if ( showTemp && curve.size() >= 5 ) {
				cv::Mat canvas( 200, 200, CV_8UC3 );
				for ( int i = 0; i < curve.size(); ++i ) {
					canvas.at<cv::Vec3b>(curve[i].y, curve[i].x) = cv::Vec3b(255, 0, 0);
					cv::imshow("curveDiscre", canvas); cv::waitKey(10);
				}
				cv::imshow("curveDiscre", canvas); cv::waitKey(10);
			}
			
			int numCurve = 0;
			int L = curve.size();
			std::vector<double>  curvitys;
			std::vector<bool >  isArcs;
			std::vector<double>  dis2ps;
			bool useCurvity = true;
			//const int k = 9;//选取计算曲率的曲线长度,原始图选择6可以
			//使用曲率半径或许更好
			if ( curve.size()>5 )
			{
				curve[0]; curve[curve.size()-1];
				int i = 0;
				//for (auto ptr = curve.begin(); ptr != curve.end(),i< curve.size(); ++ptr,++i)
				for ( ; i< curve.size();  ++i ){
					std::vector<cv::Point> curveSeg;
					if ( useCurvity )
					{
						//curveSeg.push_back(cv::Point(curve[(i) % L]));
						//curveSeg.push_back(cv::Point(curve[(1 + i) % L]));
						//curveSeg.push_back(cv::Point(curve[(2 + i) % L]));
						//curveSeg.push_back(cv::Point(curve[(3 + i) % L]));
						//curveSeg.push_back(cv::Point(curve[(4 + i) % L]));
						for (int j = 0; j < k; ++j){
							curveSeg.push_back(cv::Point(curve[(i+j) % L]));
						}
						double curvity = 0;// 
						bool isArc = false;
						curvity = getCurvity( curveSeg, isArc);
						//curvity =  getCurvityR(curveSeg);//使用曲率半径//效果也不好，必须有负值！
						curvitys.push_back(curvity);
						isArcs.push_back( isArc );
					} 
					else{//使用距离截断
						double  dis2p = cvWish::disCv(cv::Point(curve[(i) % L]), cv::Point(curve[(1+i) % L]));
						dis2ps.push_back(dis2p);
					}
				}
				
				if ( useCurvity )
				{//根据曲率判断是否断开，这与曲线的曲率均值密切相关
					double disCrThres = 0.12;//与R半径相关，在大于0.1的时候断开，即表示为弯道//判断是否为弧在下一步
					//bool isChange = false;
					//bool isExcess = false;
					bool isDown = false;//可能有冗余，或者多不过比少一个安全
					for ( i = 0; i < curvitys.size(); ++i ) {
						if (curvitys[i] < disCrThres ) {
							if ( !isDown){
								isDown = true;
								numCurve += 1;//只数分开的段数，同时表示有效的段数
							}else{//isChange = false;
							}
						}else {
							isDown = false;
						}
					}//for
					int pos = -1;//分段
					if (numCurve > 0) {
						curves.resize(numCurve);
					}else {
						curves.resize(1);
					}
					isDown = false;//需要重新初始化？
					for ( i = 0; i < curvitys.size(); ++i ) {
						if ( curvitys[i] < disCrThres ) {
							if (!isDown) {
								isDown = true;
								pos += 1;//注意0和1
							}else {//isDown = false;
							}
							curves[pos].push_back(cv::Point(curve[(i) % L]));
						}else {
							isDown = false;
						}
					}//for
				} 
				else
				{//根据距离判断是否断开
					double disThres = 8;//与R半径相关，为了避免45度时候断开
					for (i = 0; i < dis2ps.size(); ++i) {
						if (dis2ps[i] > disThres) {
							numCurve += 1;
						}
					}
					int pos = 0;
					if (numCurve > 0) {
						curves.resize(numCurve);
					}else {
						curves.resize(1);
					}
					for (i = 0; i < dis2ps.size(); ++i) {
						if (dis2ps[i] > disThres) {
							pos += 1;
							pos %= numCurve;
						}
						curves[pos].push_back(cv::Point(curve[(i) % L]));
					}
				}
			}
			showTemp = true;
			if ( showTemp ) {
				for (int n = 0; n < curves.size(); ++n) {
					cv::Mat canvas(200, 200, CV_8UC3);
					for (int i = 0; i < curves[n].size(); ++i)
						canvas.at<cv::Vec3b>(curves[n][i].y, curves[n][i].x) = cv::Vec3b(255, 0, 0);
					cv::imshow("curveDiscreSeg", canvas); cv::waitKey(10);
				}
			}
			return curves;
		}//divideCurveByCurvity
```
curvity = getCurvity( curveSeg, isArc);函数在上一篇文章中：[https://blog.csdn.net/wishchin/article/details/83447712](https://blog.csdn.net/wishchin/article/details/83447712)
图片检测结果：
![](https://img-blog.csdnimg.cn/20181027171410708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_27,color_FFFFFF,t_70)
