# Sobel算子取代：基于特定点方向的canny边缘检测 - wishchinYang的专栏 - CSDN博客
2016年06月22日 19:22:05[wishchin](https://me.csdn.net/wishchin)阅读数：997
**前言：**
       Canny边缘检测使用了Sobel算子，计算dx和dy两个方向，对于特定方向的边缘检测，可以作少量修改。
**代码：**
计算特定方向上的边缘
```cpp
void CannyOrient( 
			cv::Mat &_src, cv::Mat &_dst,
			cv::Point2f  &seed,
			double low_thresh, double high_thresh,
			int aperture_size, bool L2gradient )
		{
			low_thresh  = 0; 
			high_thresh = 1;
			//使用特定种子方向上的点寻找方向梯度
			FindBestGradient( _src, _dst,seed,aperture_size,  L2gradient );
			//二值化，生成边缘图像
			//计算直方图，寻找极大极小边缘点，根据波峰和波谷
			int hh[256];
			memset(hh,0,256*sizeof(int ) );
			int minV = 255;
			int maxV =   0;
			cvWish::CalHist(_dst,1,minV,maxV,hh);
			cv::Mat canvas( 600 ,256*4, CV_8U, 1);
			for(int i=0;i< 256; ++i){
				int Pos = ( (int)(hh[i]) ) %( canvas.rows);
				canvas.at<uchar>( canvas.rows - Pos-1,i*4 ) =255;
			}
			cv::imshow("canvas",canvas);
			cv::waitKey(1);
			return;
		}
```
寻找特定边缘：
```cpp
//在种子点方向上寻找合适的梯度，用于寻找边缘
		void FindBestGradient( 
			cv::Mat &_src, cv::Mat &_dst,
			cv::Point2f  &seed,
			int aperture_size, bool L2gradient )
		{
			//角度矩阵
			cv::Mat df = cv::Mat::zeros( _src.rows,_src.cols, CV_32FC1 );
			//梯度矩阵
			cv::Mat dg = cv::Mat::zeros( _src.rows,_src.cols, CV_32FC1 );
			//原始图像
			cv::Mat ds = _src.clone();
			//目标图像 uchar型
			cv::Mat dd = _src.clone();
			//1.根据角度计算梯度//得到梯度矩阵
			//使用N*1的算子
			int n = aperture_size;//必须为奇数
			//对每个柱进行初始化
			//搜索柱:在射线方向上搜索l_Search 个像素；宽度为 
			int l_Search = n;
			int w_Search = 1;
			std::vector<std::vector<std::pair<cv::Point ,float>  >  >  beam;
			beam.resize( l_Search );
			for (int i=0;i< beam.size();++i)
			{
				beam[i].resize(w_Search);
			}//初始化柱
			//设定系数//生成模板
			double gap = 2.0/ (n-1);
			std::vector< double > mask(l_Search);
			for (int i=0;i< mask.size();++i)
			{
				mask[i] = -1 + i*gap ;
				//std::cout<< "  mask[i]:" << mask[i] ;
			}
			//2.生成角度图像
			//在射线方向上寻找//方法不是太好，但是没有寻找到简单有效的方法
			for ( int y=0 ;y< ds.rows;++y )
			{
				float* ptr = (float*)( df.data + y * df.step);
				for ( int x=0; x< ds.cols; ++x )
				{
					//计算角度
					//float ag = (float)(cvWish::cosCv(seed,cv::Point2f( x,y ) ) );
					//df.at<float>(y ,x) = (float)(cvWish::cosCv(seed,cv::Point2f( x,y ) ) );
					*ptr = (float)(cvWish::cosCv(seed,cv::Point2f( x,y ) ) );
					++ptr;
				}
			}
			//计算差值-导数
			for (int y=0 ;y< ds.rows;++y)
			{
				float* pf = (float*)( df.data + y * df.step);
				float* pg = (float*)( dg.data + y * dg.step);
				unsigned char* pd = (unsigned char*)( dd.data + y * dd.step);
				for (int x=0;x< ds.cols;++x )
				{
					//计算角度
					//cvWish::BeamInit(l_Search,w_Search,cv::Point2f( x,y ),df.at<float >(y,x),beam,0);//0表示从中部开始搜索
					cvWish::BeamInit(l_Search,w_Search,cv::Point2f( x,y ), *pf ,beam,0);//0表示从中部开始搜索
					cvWish::BeamNormal(dg.cols, dg.rows , beam);
#ifdef SHOW_TEMP
					int ii =0;
					for (;ii<beam.size()  ;++ii)
					{
						int j=0;
						for (;j<beam[ii].size()  ;++j)
						{
							//canvasSrc.at<cv::Vec3b>(beam[ii][j].first.y ,beam[ii][j].first.x )[0] =255 ;
							//canvasSrc.at<cv::Vec3b>(beam[ii][j].first.y ,beam[ii][j].first.x )[1] =0 ;
							//canvasSrc.at<cv::Vec3b>(beam[ii][j].first.y ,beam[ii][j].first.x )[2] =0 ;
						}	
					}
					//cv::imshow("edgeEvolution",canvasSrc);
					//cv::waitKey(1);
#endif
					//dg.at<float >(y,x)= 0;
					//for ( int k =0; k< l_Search; ++k ){
					//	dg.at<float >(y,x) += (float)( mask[k]* ds.at<unsigned char>(beam[k][0].first.y,beam[k][0].first.x)  );
					//}
					//int s = abs ( ( (int)(dg.at<float >(y,x) ) )%255 ) ;
					//dd.at<unsigned char >( y, x ) =(unsigned char) (s);
					*pg = 0;
					for ( int k =0; k< l_Search; ++k ){
						*pg += (float)( mask[k]* ds.at<unsigned char>(beam[k][0].first.y,beam[k][0].first.x)  );
					}
					int s = abs ( ( (int)(*pg ) )%255 ) ;
					*pd = (unsigned char) (s);
					++pf;
					++pg;
					++pd;
				}
			}
                       ds.copyTo(_dst);
			return;
		}
```
辅助代码：
```cpp
//功能： 初始化任意角度的一个方柱，大小已经确定：l_Search*w_Search
	//沿射线方向 寻找 一个柱//默认 参数 0 从中部开始
	//参数 1代表 从底部开始；参数 2代表从top开始
	void BeamInit( 
		const int l_Search, const int w_Search,
		const cv::Point2f &pc, const float angle,
		std::vector<std::vector<std::pair<cv::Point ,float> >  >  &beam,
		const int bottomOrTop )
	{
		assert (l_Search%2 >0);//确定是奇数
		assert (w_Search%2 >0);
		assert ( beam.size() == l_Search);//不改变大小
		assert ( beam[0].size() == w_Search);
		//往角度方向延长
		cv::Point2f  ps(0,0);
		const float angleVert = angle+ PI_1_2 < PI_4_2?  ( angle+ PI_1_2): ( angle+ PI_1_2)- PI_4_2;
		cv::Point2f  pIdx(0,0);
		switch (bottomOrTop)
		{
		case 0:
			//往底部移动
			ps.y = pc.y-(0- sin(angle)*l_Search/2);
			ps.x = pc.x-(0- cos(angle)*l_Search/2);
			//往左边移动
			ps.y -= 0- sin(angleVert)*w_Search/2;
			ps.x -= 0- cos(angleVert)*w_Search/2;
			//对每个点计算
			for (int i=0;i< beam.size();++i )
			{
				pIdx.y = ps.y +(0- sin(angle)*i);
				pIdx.x = ps.x +(0- cos(angle)*i);
				for (int j=0;j< beam[i].size();++j )
				{
					beam[i][j].first.y = pIdx.y +(0- sin(angleVert)*j);
					beam[i][j].first.x = pIdx.x +(0- cos(angleVert)*j);
				}
			}
			//int iwCenter = w_Search>>1;
			//int ilCenter = l_Search>>1;
			////先算十字中间
			//for ( int i=0; i< beam.size(); ++i )
			//{
			//	beam[i][iwCenter].first.x = 0- cos(angle)*i;;
			//}
			break;
		case 1:
			//往底部移动//默认底部，因此不需要移动
			ps.y = pc.y;//-(0- sin(angle)*l_Search/2);
			ps.x = pc.x;//-(0- cos(angle)*l_Search/2);
			//往左边移动
			ps.y -= 0- sin(angleVert)*w_Search/2;
			ps.x -= 0- cos(angleVert)*w_Search/2;
			//对每个点计算
			for (int i=0;i< beam.size();++i )
			{
				pIdx.y = ps.y +(0+ sin(angle)*i);
				pIdx.x = ps.x +(0+ cos(angle)*i);
				for (int j=0;j< beam[i].size();++j )
				{
					beam[i][j].first.y = pIdx.y +(0- sin(angleVert)*j);
					beam[i][j].first.x = pIdx.x +(0- cos(angleVert)*j);
				}
			}
			break;
		case 2:
			//往底部移动//默认顶部，因此需要移动到底
			ps.y = pc.y -(0- sin(angle)*l_Search);
			ps.x = pc.x -(0- cos(angle)*l_Search);
			//往左边移动
			ps.y -= 0- sin(angleVert)*w_Search/2;
			ps.x -= 0- cos(angleVert)*w_Search/2;
			//对每个点计算
			for (int i=0;i< beam.size();++i )
			{
				pIdx.y = ps.y +(0- sin(angle)*i);
				pIdx.x = ps.x +(0- cos(angle)*i);
				for (int j=0;j< beam[i].size();++j )
				{
					beam[i][j].first.y = pIdx.y +(0- sin(angleVert)*j);
					beam[i][j].first.x = pIdx.x +(0- cos(angleVert)*j);
				}
			}
			break;
		default:
			//和case 0 相同，默认设定为中间位置
			//往底部移动
			ps.y = pc.y-(0- sin(angle)*l_Search/2);
			ps.x = pc.x-(0- cos(angle)*l_Search/2);
			//往左边移动
			ps.y -= 0- sin(angleVert)*w_Search/2;
			ps.x -= 0- cos(angleVert)*w_Search/2;
			//对每个点计算
			for (int i=0;i< beam.size();++i )
			{
				pIdx.y = ps.y +(0- sin(angle)*i);
				pIdx.x = ps.x +(0- cos(angle)*i);
				for (int j=0;j< beam[i].size();++j )
				{
					beam[i][j].first.y = pIdx.y +(0- sin(angleVert)*j);
					beam[i][j].first.x = pIdx.x +(0- cos(angleVert)*j);
				}
			}
			break;
		}
		return;
	}
```
```cpp
//对柱规整，使其不超出边界
	void BeamNormal( 
		const int width ,
		const int height ,
		std::vector<std::vector<std::pair<cv::Point ,float> >  >  &beam
		)
	{
		int w = width  -1;
		int h = height -1;
		//对每个点 s
		for ( int i=0;i< beam.size();++i )
		{
			for (int j=0;j< beam[i].size();++j )
			{
				if (beam[i][j].first.x < 0 )
				{
					beam[i][j].first.x = 0;
				}
				if (beam[i][j].first.y < 0 )
				{
					beam[i][j].first.y = 0;
				}
				if (beam[i][j].first.x >w )
				{
					beam[i][j].first.x = w;
				}
				if (beam[i][j].first.y > h )
				{
					beam[i][j].first.y = h;
				}
			}
		}
		return;
	}
```
**代码效果：**
![](https://img-blog.csdn.net/20160622192351702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160622192336160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160622192409187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
