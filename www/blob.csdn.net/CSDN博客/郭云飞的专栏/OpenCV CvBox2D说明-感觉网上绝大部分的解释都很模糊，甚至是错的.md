
# OpenCV:CvBox2D说明 - 感觉网上绝大部分的解释都很模糊，甚至是错的 - 郭云飞的专栏 - CSDN博客


2017年01月19日 16:17:53[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1539



```python
typedef struct CvBox2D
{
    CvPoint2D32f center;  /* Center of the box.                          */
    CvSize2D32f  size;    /* Box width and length.                       */
    float angle;          /* Angle between the horizontal axis           */
                          /* and the first side (i.e. length) in degrees */
#ifdef __cplusplus
    CvBox2D(CvPoint2D32f c = CvPoint2D32f(), CvSize2D32f s = CvSize2D32f(), float a = 0) : center(c), size(s), angle(a) {}
    CvBox2D(const cv::RotatedRect& rr) : center(rr.center), size(rr.size), angle(rr.angle) {}
    operator cv::RotatedRect() const { return cv::RotatedRect(center, size, angle); }
#endif
}
CvBox2D;
```

![](https://img-blog.csdn.net/20170119163914803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

(一)  OpenCV坐标系原点说明：左上角为原点
(二)  CvBox2D中角度angle（θ）的选取规则为：X轴逆时针旋转和碰到矩形第一条边（延长线）的夹角。
(三)  角度angle （θ）的值永远为负，取值范围为（-90， 0]。
(四)  角度angle （θ）为角度，非弧度。
(五)  X轴逆时针碰到的第一条边为width（width和height的大小无关系，就是说width可能比height大，也可能小）。


