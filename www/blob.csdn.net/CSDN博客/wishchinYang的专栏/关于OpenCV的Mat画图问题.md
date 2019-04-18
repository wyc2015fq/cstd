# 关于OpenCV的Mat画图问题 - wishchinYang的专栏 - CSDN博客
2015年10月14日 16:27:15[wishchin](https://me.csdn.net/wishchin)阅读数：612
       由于OpenCV的java版本画图有太多错误，只能自己编写画图的代码，在一个函数中，编写出画圆和深度距离的代码，
代码如下：
```java
public int CircleMyMat(Mat Show, Point3 Center, double Radius, Scalar COLOR ,int Width)
		{
			//请空画布为白色
			Show = Mat.zeros(Show.size(), CvType.CV_8UC3);
			//Core.bitwise_not(Show, Show);//反色！
			double[] C= Show.get(0, 0);
			C[0]= 0;C[1]= 0;C[2]= 255;//C[0]= 0;C[1]= 0;C[2]= 0;
			//1.获取圈的值//画出圆！
			int CenterX = (int) Center.x;
			int CenterY = (int) Center.y;
			int    R = (int)Radius;
			List<Point3 >  CirclePoints = new LinkedList<Point3 >();
			//得到圆上点的值！
			int MaxX= CenterX + (int)R;
			int MinX= CenterX - (int)R;
			int i =0; int X =0; int Y =0;
			for(i = 0-R; i<R ;++i){
				Point3 PointIn= new Point3();
				PointIn.x  = i + CenterX ;
				PointIn.y  = Math.sqrt( Math.abs(R*R - i*i) );//这个公式ZTMD关键！
				PointIn.y  = CenterY + PointIn.y ;
				CirclePoints.add(PointIn);
			}
			//取下半部分！
			for(i = 0-R; i<R ;++i){
				Point3 PointIn= new Point3();
				PointIn.x  = i + CenterX ;
				PointIn.y  = Math.sqrt( Math.abs(R*R - i*i) );//这个公式ZTMD关键！
				PointIn.y  = CenterY - PointIn.y ;
				PointIn.z  = 1;
				CirclePoints.add(PointIn);
			}
			//2.获取Z值//画在右方如何？//固定缩放系数//保证不要跑出边界！
			int CenterZ = (int) Center.z;
			int Zl      = CenterZ;//Zl直接作为长度被使用！
			//2.2 。画出一条线来！
			Point ttt0 = new Point(); 
			Point ttt1 = new Point();
			ttt0.x = CenterX;
			ttt0.y = CenterY;
			ttt1.x = CenterX +  Zl;//ttt1.x = CenterX + 20;//
			ttt1.y = CenterY;
			lineMyMat(Show, ttt0, ttt1, COLOR ,Width);
			//3.画出圈来！//在此调整粗细！
			double[] P;
			int Idx =0;
			for( Idx=0; Idx< CirclePoints.size(); ++Idx)
			{
				P= C;
				X= (int )CirclePoints.get(Idx).x;
				Y= (int )CirclePoints.get(Idx).y;
				Show.put(Y,X, P);//frameRf.put(Y, X, P);
				++X;++Y;
				Show.put( Y, X, C);
				++X;++Y;
				Show.put( Y, X, C);
			}
			return 1;
		}
```
出现了无法画图的问题，总是找不到原因。
最后，把
```java
Show = Mat.zeros(Show.size(), CvType.CV_8UC3);
```
注销掉，程序可以运行了。
原因到底是什么，一时难以理解......................

