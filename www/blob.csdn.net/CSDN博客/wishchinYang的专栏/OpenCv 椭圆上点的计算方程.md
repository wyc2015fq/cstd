# OpenCv:椭圆上点的计算方程 - wishchinYang的专栏 - CSDN博客
2016年06月22日 19:36:09[wishchin](https://me.csdn.net/wishchin)阅读数：3034
椭圆
        椭圆（Ellipse）是平面内到定点F1、F2的距离之和等于常数（大于|F1F2|）的动点P的轨迹，F1、F2称为椭圆的两个焦点。其数学表达式为：                                                                                                                 |PF1|+|PF2|=2a（2a>|F1F2|）。[1]
        椭圆是[圆锥曲线](http://baike.baidu.com/view/368458.htm)的一种，即圆锥与平面的截线。[2]椭圆在[开普勒](http://baike.baidu.com/view/4416.htm)[行星](http://baike.baidu.com/view/22572.htm)运行三定律中扮演了[重要](http://baike.baidu.com/subview/505621/18968499.htm)角色，即[恒星](http://baike.baidu.com/subview/1538/5032550.htm)是椭圆两焦点中的一个，是数学科重点研究的一个[项目](http://baike.baidu.com/view/25183.htm)。[3]
**标准方程：**
        椭圆的标准方程共分两种情况：
               当[焦点](http://baike.baidu.com/subview/213284/8380485.htm)在x轴时，椭圆的标准方程是：x^2/a^2+y^2/b^2=1，(a>b>0)；
当焦点在y轴时，椭圆的标准方程是：y^2/a^2+x^2/b^2=1，(a>b>0)；
                                                  其中a^2-c^2=b^2
**参数方程：**
       椭圆上点的参数方程为：
             y = a *sin(  alp ) 
             x=  a *cos( alp )  (a>b>0)；
 此时的角度alp不是中心点到椭圆上点的角度，而是椭圆的仿射圆上的点到圆心的角度，计算角度应考虑到压缩。
**压缩方向：**
         Height方向拉伸；
         计算变化后的beta；
         计算坐标：
             y = a *sin(  beta ) 
             x=  a *cos( beta )  (a>b>0)；
         Height方向压缩；
y = a *sin(  beta ) *（b/a）
x=  a *cos( beta )                   (a>b>0)；
       计算距离。
~~**椭圆上点的计算方程：**~~
~~        对于 (a>b>0)；~~
~~               对应的圆的方程： R = a;~~
~~               圆上的点的坐标： x2 = R * sin(Beta)    y2 = R * cos(beta);~~
~~               不变性：  alp = beta                  ~~
~~       对应椭圆点的坐标：~~
~~               角度：  alp = beta~~
~~               角度：  alp = beta~~
**计算椭圆上点的代码：**
*     代码是错误的，不能把点压缩到椭圆上*
```cpp
//调整椭圆边缘到标准椭圆；在角度方向上进行拉伸
		//angleOfDip 为椭圆的偏斜角，弧度值！
		//增加边界检查
		template <class T1,class T2>
		float AdjustEllipseEdge(
			std::vector<std::pair< T1, T2 > >  &closeEdgeIn,
			std::vector<std::pair< T1, T2 > >  &closeEdgeOut,
			const cv::RotatedRect &ecf,
			const cv::Point2f &rfCentroidS,
			const double angleOfDipSrc,
			const int ww,
			const int hh)
		{
			assert(closeEdgeIn.size() == closeEdgeOut.size() );
			int w = ww -1;
			int h = hh -1;
			const cv::Point2f rfCentroid = ecf.center;
			//cv::Point2f rfCentroid(0,0);
			std::vector< double > angleListS;//为点椭圆角度，用于求取 椭圆点到中心的距离
			angleListS.resize( closeEdgeIn.size() );
			int vOrH = 0;//水平或者竖直？
			vOrH = ecf.size.width > ecf.size.height? 0:1;//若0，则为V;或者为1，水平
			double angleOfDip = 0;
			if (0 == vOrH )
			{//若为水平//width 的倾角
				angleOfDip  = angleOfDipSrc; 
			} 
			else
			{
				angleOfDip  = angleOfDipSrc - PI_1_2; 
			}
			double a = max(ecf.size.height/2.0,ecf.size.width /2.0);//长轴//固定后使用方程
			double b = min(ecf.size.height/2.0,ecf.size.width /2.0);
#ifdef SHOW_TEMP
			cv::Mat canvasSrc = cv::Mat::zeros(200,200,CV_8UC3);
			cv::bitwise_not(canvasSrc,canvasSrc);
			cv::ellipse(canvasSrc,ecf,cv::Scalar(0,0,255),1,8);
#endif
			//在此测试，cos计算的代码
#ifdef SHOW_TEMP
			cv::RotatedRect ecT = RotatedRect(Point2f(100,100), Size2f(50,100), 30);
			std::vector<std::pair< cv::Point2f, double > >  PointCosTest(0);
			cvWish::polygon::GetElipseEdge(ecT, PointCosTest, (ecT.size.height + ecT.size.height)/5.0 );
			cv::ellipse(canvasSrc, ecT, cv::Scalar(0,0,255), 1, 8);
			
			for ( int i=0; i< PointCosTest.size(); ++i)
			{
				cv::circle( canvasSrc, PointCosTest[i].first, 1, cv::Scalar(255,0,0), 1, 8, 0 );
				double af = cvWish::cosCv(ecT.center,PointCosTest[i].first);//cosCv出现计算问题
				std::cout<< "Cos:" << af<< std::endl;
				std::cout<< "Angle:" << PointCosTest[i].second << std::endl;
				cv::imshow("PointCosTest",canvasSrc);
				cv::waitKey(1);
			}
#endif
			for ( int i=0; i<closeEdgeIn.size(); ++i )
			{
				closeEdgeIn[i].second  = cvWish::cosCv( rfCentroid, closeEdgeIn[i].first );
				angleListS[i]  = closeEdgeIn[i].second;
				angleListS[i] -= angleOfDip;//旋转
				angleListS[i]  = angleListS[i]> PI_4_2 ? angleListS[i] - PI_4_2:angleListS[i];
				//探测距离
				double disPC    = cvWish::disCv(rfCentroid,closeEdgeIn[i].first);
				double alp =  angleListS[i];
				//alp =  alp *180/M_PI;
				double disShould = 
					sqrt( b*sin(alp ) *b*sin(alp ) + a*cos(alp) *a*cos(alp) );//公式无误，角度出现问题？
				     //sqrt( b*cos(alp ) *b*cos(alp ) + a*sin(alp) *a*sin(alp) );//公式无误，角度出现问题？
				//可能问题，方向角度出现往长轴极点的方向进行压缩，导致生成距离变大。
				
				//double disShould = sqrt( 
				//	ecf.size.width*cos(angleListS[i]) *ecf.size.width*cos(angleListS[i]) /4
				//	+ ecf.size.height*sin(angleListS[i]) *ecf.size.height*sin(angleListS[i])/4 );
				std::cout<< alp << std::endl;
				std::cout<< cos(alp)  << std::endl;
				std::cout<<"disPc:" <<disPC << std::endl;
				std::cout<< "disShould:" << disShould << std::endl;
#ifdef SHOW_TEMP
				//cv::Mat canvasSrc(100,100,CV_8UC3);
				cv::circle(canvasSrc,closeEdgeIn[i].first,1,cv::Scalar(255,0,0),1,8,0);
				cv::imshow("edgeEvolution",canvasSrc);
				cv::waitKey(1);
#endif
				//调整点到椭圆上
				//adjustPoint2Elipse();
				//根据距离 往角度方向上拉伸点//角度其实产生了偏离//偏角使用图片偏角
				cvWish::PullPoint2Out( closeEdgeIn[i].first, closeEdgeIn[i].second, ( disPC - disShould ) );	
				closeEdgeOut[i].first  = closeEdgeIn[i].first;
				////已确认大于0，此时确认不超边界
				closeEdgeOut[i].first.x = min(closeEdgeOut[i].first.x,w);
				closeEdgeOut[i].first.y = min(closeEdgeOut[i].first.y,h);
				closeEdgeOut[i].second = closeEdgeIn[i].second;
#ifdef SHOW_TEMP
				cv::circle(canvasSrc,closeEdgeOut[i].first,1,cv::Scalar(0,255,0),1,8,0);
				cv::imshow("edgeEvolution",canvasSrc);
				cv::waitKey(1);
#endif
			}
			return 1.0;
		}
```
**代码修改：**
*     使用一个仿射变换*
```cpp
//调整椭圆边缘到标准椭圆；在角度方向上进行拉伸
		//angleOfDip 为椭圆的偏斜角，弧度值！
		//增加边界检查
		template <class T1,class T2>
		float AdjustEllipseEdge(
			std::vector<std::pair< T1, T2 > >  &closeEdgeIn,
			std::vector<std::pair< T1, T2 > >  &closeEdgeOut,
			const cv::RotatedRect &ecf,
			const cv::Point2f &rfCentroidS,
			const double angleOfDipSrc,
			const int ww,
			const int hh)
		{
			assert(closeEdgeIn.size() == closeEdgeOut.size() );
            int w = ww -1;
            int h = hh -1;
            const cv::Point2f rfCentroid = ecf.center;
            //cv::Point2f rfCentroid(0,0);
            std::vector< double > angleListS;//为点椭圆角度，用于求取 椭圆点到中心的距离
            angleListS.resize( closeEdgeIn.size() );
            int vOrH = 0;//水平或者竖直？
            vOrH = ecf.size.width > ecf.size.height? 0:1;//若0，则为V;或者为1，水平
            double angleOfDip = 0;
            if (0 == vOrH )
            {//若为水平//width 的倾角
                angleOfDip  = angleOfDipSrc; 
            } 
            else
            {
                angleOfDip  = angleOfDipSrc - PI_1_2; 
            }
            //double a = max(ecf.size.height/2.0,ecf.size.width /2.0);//长轴//固定后使用方程
            //double b = min(ecf.size.height/2.0,ecf.size.width /2.0);
            double b = ecf.size.height/2.0//长轴//固定后使用方程
            double a = ecf.size.width /2.0;
            double compressFactor = b /a ;//压缩或者缩放因子
#ifdef SHOW_TEMP
            cv::Mat canvasSrc = cv::Mat::zeros(200,200,CV_8UC3);
            cv::bitwise_not(canvasSrc,canvasSrc);
            cv::ellipse(canvasSrc,ecf,cv::Scalar(0,0,255),1,8);
#endif
            //在此测试，cos计算的代码
#ifdef SHOW_TEMP
            cv::RotatedRect ecT = RotatedRect(Point2f(100,100), Size2f(50,100), 30);
            std::vector<std::pair< cv::Point2f, double > >  PointCosTest(0);
            cvWish::polygon::GetElipseEdge(ecT, PointCosTest, (ecT.size.height + ecT.size.height)/5.0 );
            cv::ellipse(canvasSrc, ecT, cv::Scalar(0,0,255), 1, 8);
            
            for ( int i=0; i< PointCosTest.size(); ++i)
            {
                cv::circle( canvasSrc, PointCosTest[i].first, 1, cv::Scalar(255,0,0), 1, 8, 0 );
                double af = cvWish::cosCv(ecT.center,PointCosTest[i].first);//cosCv出现计算问题
                std::cout<< "Cos:" << af<< std::endl;
                std::cout<< "Angle:" << PointCosTest[i].second << std::endl;
                cv::imshow("PointCosTest",canvasSrc);
                cv::waitKey(1);
            }
#endif
            for ( int i=0; i<closeEdgeIn.size(); ++i )
            {
                closeEdgeIn[i].second  = cvWish::cosCv( rfCentroid, closeEdgeIn[i].first );
                //压缩方向
                angleListS[i]  = closeEdgeIn[i].second;
                angleListS[i] -= angleOfDip;//旋转
                angleListS[i]  = angleListS[i]> PI_4_2 ? angleListS[i] - PI_4_2:angleListS[i];
                //探测距离
                double disPC    = cvWish::disCv(rfCentroid,closeEdgeIn[i].first);
                //double alp =  angleListS[i];
                //alp =  alp *180/M_PI;
                //double disShould = sqrt( b*sin(alp ) *b*sin(alp ) + a*cos(alp) *a*cos(alp) );//公式无误，角度出现问题？
                //可能问题，方向角度出现往长轴极点的方向进行压缩，导致生成距离变大。
                //计算对应仿射圆的角度
                double xDeta =  closeEdgeIn[i].first.x - rfCentroid.x;
                double yDeta =  closeEdgeIn[i].first.y - rfCentroid.y;
                yDeta /= compressFactor;
                //计算角度
                double beta = cvWish::cosCv( rfCentroid, cv::Point2f( rfCentroid.x + xDeta, rfCentroid.y+ yDeta ) );
                double r = a;                                                                
                xDeta  =  r* cos(beta);
                yDeta  =  r* sin(beta);
                yDeta *= compressFactor;
                //直接计算距离
                double disShould = sqrt( xDeta*xDeta + yDeta*yDeta );//公式无误，角度出现问题？
                std::cout<<"disPc:" <<disPC << std::endl;
                std::cout<< "disShould:" << disShould << std::endl;
#ifdef SHOW_TEMP
                //cv::Mat canvasSrc(100,100,CV_8UC3);
                cv::circle(canvasSrc,closeEdgeIn[i].first,1,cv::Scalar(255,0,0),1,8,0);
                cv::imshow("edgeEvolution",canvasSrc);
                cv::waitKey(1);
#endif
                //调整点到椭圆上
                //adjustPoint2Elipse();
                //根据距离 往角度方向上拉伸点//角度其实产生了偏离//偏角使用图片偏角
                cvWish::PullPoint2Out( closeEdgeIn[i].first, closeEdgeIn[i].second, ( disPC - disShould ) );    
                closeEdgeOut[i].first  = closeEdgeIn[i].first;
                ////已确认大于0，此时确认不超边界
                closeEdgeOut[i].first.x = min(closeEdgeOut[i].first.x,w);
                closeEdgeOut[i].first.y = min(closeEdgeOut[i].first.y,h);
                closeEdgeOut[i].second = closeEdgeIn[i].second;
#ifdef SHOW_TEMP
                cv::circle(canvasSrc,closeEdgeOut[i].first,1,cv::Scalar(0,255,0),1,8,0);
                cv::imshow("edgeEvolution",canvasSrc);
                cv::waitKey(1);
#endif
            }
            return 1.0;
        }
```
从一个椭圆上面获取特定个数的点的函数：
```cpp
//参数描述：椭圆；输出的点集；欲获取的点的个数
		int polygon::GetElipseEdge(
			const cv::RotatedRect &ecf,  
			std::vector<std::pair< cv::Point2f, double > >  &ellipseEdge,
			const int numPs,
			cv::Rect &roiRestrict,
			bool openEdgeRestrict )
		{
			if ( numPs == 0 )
			{
				return numPs;
			}
			else
			{
				ellipseEdge.resize( numPs );
			}
			//对椭圆进行划分
			const double angleGap = PI_4_2/numPs;
			const double cx = ecf.center.x;
			const double cy = ecf.center.y;
			const float angleOfDip =  PI_1_2 + ecf.angle*3.1415926 /180.0;//为何偏移了 半个pi
			//const double angleOfDip =0- ecf.angle*3.1415926 /180.0;//
			double w = ecf.size.width /2.0;
			double h = ecf.size.height/2.0;
			for (int i=0 ;i< numPs;++i )
			{
				double as = i*angleGap ;
				double a = as ;
				a += angleOfDip;
				a = a>PI_4_2? a-PI_4_2:a;
				double y = (w) *sin( a );
				double x = (h) *cos( a );
				//旋转
				float xDeta = x*cos( angleOfDip ) - y*sin( angleOfDip );
				float yDeta = x*sin( angleOfDip ) + y*cos( angleOfDip );
				cv::Point2f p( cx+xDeta, cy+yDeta);
				//ellipseEdge[i] = (std::pair< T1, T2 >)(std::make_pair( p,as ) );
				//ellipseEdge[i] = (std::pair< cv::Point2f, double >)(std::make_pair( p,as ) );//此处代码只为运行于GCC修改，有问题，模板库不能使用！！！wishchin!!!
				ellipseEdge[i].first.x = p.x;
				ellipseEdge[i].first.y = p.y;
				ellipseEdge[i].second  =  as;
			}
			
			if (openEdgeRestrict)
			{
				float x,y;
				float xS(roiRestrict.x), yS(roiRestrict.y), xE(roiRestrict.x+roiRestrict.width), yE(roiRestrict.y+roiRestrict.height );
				
				for (int i=0 ;i< numPs;++i )
				{
					x = ellipseEdge[i].first.x;
					y = ellipseEdge[i].first.y;
					x = (std::min)( (std::max)(x,xS),xE );
					y = (std::min)( (std::max)(y,yS),yE );
					//ellipseEdge[i].first = cv::Point2f(x,y);
					ellipseEdge[i].first.x = x;
					ellipseEdge[i].first.y = y;
				}
			} 
			else
			{
			}
			return 1;
		}
```
 结果显示：
       原始结果：                                                                                    修改后结果：
![](https://img-blog.csdn.net/20160623114933415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160623114958713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160623115042063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
