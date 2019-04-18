# OpenCV：OpenCV图像旋转的代码 - wishchinYang的专栏 - CSDN博客
2017年02月27日 13:45:29[wishchin](https://me.csdn.net/wishchin)阅读数：906
OpenCV图像旋转的代码
```cpp
cv::transpose( bfM, bfM )
```
前提：使用两个矩阵Mat型进行下标操作是不行的，耗费的时间太长了。直接使用两个指针对拷贝才是王道。不知道和OpenCV比较效果如何。
贴出下面的代码：  C++     
```cpp
//图像旋转
    cv::Mat Transpose(cv::Mat &inMat)  
    {
        cv::Mat outMat( inMat.cols, inMat.rows, inMat.type() );
        const int L = inMat.cols;
        std::vector<unsigned char*>  headers(L);
        for ( int x=0; x< inMat.cols; ++x )
        {
            //
            unsigned char* ptrB = ( unsigned char* )(outMat.data + x * outMat.step) ;
            headers[x] = ptrB;
        }
        for ( int y=0 ;y < inMat.rows; ++y )
        {
            unsigned char* ptrA = ( unsigned char* )( inMat.data + y * inMat.step );
            for ( int x=0; x< inMat.cols; ++x )
            {
                *headers[x]= *ptrA;
                ++(headers[x]);
                ++ptrA;
            }
        }
        return outMat;
    }//cv::Mat  Transpose(cv::Mat &inMat)
```
效果图：
       原始图像：爱德华.蒙克的《星夜》（Edvard Munch  《*Starry Night* 》. 1893. 135.6 × 140 cm.[J. Paul Getty Museum](https://en.wikipedia.org/wiki/J._Paul_Getty_Museum).）
![](https://img-blog.csdn.net/20170227113006838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
测试结果1：
       很遗憾，图像336*400，速度是为使用OpenCV库的1/20，3.167s。怎么改进？0.166s。
改进一下：
```cpp
void cvWish::transpose( unsigned char* data, size_t step, int col )
	{
		int i, j;
		for( i = 0; i < col; i++ )
		{
			unsigned char* row   = (unsigned char*)(data + step*i);
			unsigned char* dataR = data + i*sizeof(unsigned char);
			for( j = i+1; j < col; j++ )
				std::swap( row[j], *( unsigned char* )( dataR + step*j ) );
		}
	}
    int Transpose( cv::Mat &inMat, cv::Mat &outMat )  
    {
        outMat = inMat.clone();
        size_t step = inMat.step;
        int col  = inMat.cols;
        cvWish::transpose( outMat.data,step, col );
        return 1;
    }
```
测试结果2：
       很遗憾，图像336*400，速度依然是为使用OpenCV库的1/6, 0.947s。怎么改进？

