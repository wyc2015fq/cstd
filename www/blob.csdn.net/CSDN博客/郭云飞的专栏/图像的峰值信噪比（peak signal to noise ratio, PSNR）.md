
# 图像的峰值信噪比（peak signal to noise ratio, PSNR） - 郭云飞的专栏 - CSDN博客


2017年10月20日 18:51:30[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：5801


峰值信噪比（*PSNR*）是一个表示信号最大可能功率和影响它的表示精度的破坏性噪声功率的比值的工程术语。由于许多信号都有非常宽的动态范围，峰值信噪比常用对数分贝单位来表示。
在图像处理中，要对图像进行客观的评价，常常需要计算*PSNR。PSNR*是衡量图像失真或是噪声水平的客观标准。2个图像之间PSNR值越大，则越相似。普遍基准为30dB，30dB以下的图像劣化较为明显。
**PSNR定义：**
![](https://img-blog.csdn.net/20171020185522890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中，**MAX**表示图像颜色的最大数值，8bit图像最大取值为255。**MSE**为均方差，定义为：
![](https://img-blog.csdn.net/20171020185659073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中，**I**和*K*分辨是原始图像和处理后的图像，*m*n*为量图像的大小。

```python
/********************************************
* 输入格式是cv::Mat类型，I1，I2代表是输入的两幅图像
*/
double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       	// |I1 - I2|AbsDiff函数是 OpenCV 中计算两个数组差的绝对值的函数
    s1.convertTo(s1, CV_32F);  	// 这里我们使用的CV_32F来计算，因为8位无符号char是不能进行平方计算
    s1 = s1.mul(s1);           	// |I1 - I2|^2
    Scalar s = sum(s1);        	// 对每一个通道进行加和
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
    if(sse <= 1e-10) 			// 对于非常小的值我们将约等于0
        return 0;
    else {
        double  mse =sse /(double)(I1.channels() * I1.total());	// MSE
        double psnr = 10.0*log10((255*255)/mse);
        return psnr;
    }
}
```



