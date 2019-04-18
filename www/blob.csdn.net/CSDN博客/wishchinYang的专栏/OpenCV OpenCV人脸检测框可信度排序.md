# OpenCV: OpenCV人脸检测框可信度排序 - wishchinYang的专栏 - CSDN博客
2017年04月18日 16:48:04[wishchin](https://me.csdn.net/wishchin)阅读数：1381
        参考文章：[http://blog.csdn.net/hua_007/article/details/45368607](http://blog.csdn.net/hua_007/article/details/45368607)
       使用OpenCV进行人脸识别时，使用
   casecade.detectMultiScale
       函数，可输出每个检测框的置信度
代码段：
```cpp
const int numReserve = 4;
std::vector<  int > rejLevel;
std::vector<double>   levelW;
cascade.detectMultiScale( smallImg, eyes, rejLevel, levelW, 1.1, 3, 0, cv::Size(), cv::Size(), true);
cout << "eyes size=:" << eyes.size() << endl;
            
// 重新排序
std::vector<std::pair<float, int> > eyeConfi;
for (int i = 0; i < (int)(eyes.size()); i++){
     eyeConfi.push_back(std::make_pair(levelW[i], i));
}
std::qsort(&eyeConfi[0], eyeConfi.size(), sizeof(eyeConfi[0]), cmpMapWishchin);
//重新获取，只取前四个
for (int i = 0; i < (int)(eyes.size()) && i<numReserve ; i++)
{
       cv::Rect eyeSin = eyes[eyeConfi[i].second];
       eyesFinal.push_back(eyeSin) ;
}
```
函数原型：
```cpp
CV_WRAP virtual void detectMultiScale( const Mat& image,
                                   CV_OUT vector<Rect>& objects,
                                   vector<int>& rejectLevels,
                                   vector<double>& levelWeights,
                                   double scaleFactor=1.1,
                                   int minNeighbors=3, int flags=0,
                                   Size minSize=Size(),
                                   Size maxSize=Size(),
                                   bool outputRejectLevels=false );
```
cmp函数：
```cpp
int cmpMapWishchin(const void *p1, const void *p2)
         {
            int v = 1;
            std::pair<float, int >  *pp1, *pp2;
            pp1 = (std::pair<float, int > *) p1;
            pp2 = (std::pair<float, int > *) p2;
            //最大值排序
            if (pp1->first - pp2->first < 0){
                v = 1;
            }
            else{
                v = -1;
            }
            return (v);
         }
```
函数的原理是这样的（个人理解，有错误请指教）：
首先一个level一个level地测试样本，然后每一个level给一个对应的得分，也就是levelWeight，如果这个weight低于或者高于对应level的threshold，则被抛弃。
坚持到最后一个level并且在最后一个level仍然满足threshold的框就是正确的脸（正样本）。
所以，人脸的分应该是这样：level越大，分数越高，在相同的level，levelWeight越大分数越高。
但是实际上真正的人脸都是能坚持到level20（最后一个level）的，所以只比对最后一个level的所有大于1的框的levelWeight进行比对就可以知道脸的得分啦~
试验结果：
![](https://img-blog.csdn.net/20170418170453370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170418170457804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

