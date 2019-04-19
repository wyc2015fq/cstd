# 非最大抑制（NMS） - 数据之美的博客 - CSDN博客
2017年03月22日 10:43:58[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：274
非极大值抑制（NMS）
      非极大值抑制顾名思义就是抑制不是极大值的元素，搜索局部的极大值。这个局部代表的是一个邻域，邻域有两个参数可变，一是邻域的维数，二是邻域的大小。这里不讨论通用的NMS[算法](http://lib.csdn.net/base/datastructure)，而是用于在目标检测中用于提取分数最高的窗口的。例如在行人检测中，滑动窗口经提取特征，经分类器分类识别后，每个窗口都会得到一个分数。但是滑动窗口会导致很多窗口与其他窗口存在包含或者大部分交叉的情况。这时就需要用到NMS来选取那些邻域里分数最高（是行人的概率最大），并且抑制那些分数低的窗口。
代码如下
**[plain]**[view
 plain](http://blog.csdn.net/H2008066215019910120/article/details/25917609#)[copy](http://blog.csdn.net/H2008066215019910120/article/details/25917609#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/347894/fork)
- function pick = nms(boxes, overlap)  
- 
- % pick = nms(boxes,overlap)  
- % Non-maximumsuppression.  
- % Greedily selecthigh-scoring detections and skip detections  
- % that are significantlycovered by a previously selected detection.  
- 
- % boxes = boxes';  
- 
- if isempty(boxes)  
-     pick = [];  
- else  
-     x1 = boxes(:,1);  
-     y1 = boxes(:,2);  
-     x2 = boxes(:,3);  
-     y2 = boxes(:,4);  
- %     x1 = boxes(:,1);  
- %     y1 = boxes(:,2);  
- %     x2 = boxes(:,2);  
- %     y2 = boxes(:,4);  
-     s = boxes(:,end);  
-     area = (x2-x1+1) .* (y2-y1+1);%Çó³öËùÓÐÃæ»ý  
- 
-     [vals, I] = sort(s);  
-     pick = [];  
-     while ~isempty(I)  
-         last = length(I);  
-         i = I(last);  
-         pick = [pick; i];  
-         suppress = [last];  
-         for pos = 1:last-1  
-             j = I(pos);  
-             xx1 = max(x1(i), x1(j));  
-             yy1 = max(y1(i), y1(j));  
-             xx2 = min(x2(i), x2(j));  
-             yy2 = min(y2(i), y2(j));  
-             w = xx2-xx1+1;  
-             h = yy2-yy1+1;  
-             if w > 0 && h > 0  
-                 % compute overlap  
-                 o = w * h / min(area(i),area(j));  
- %                 o = w/area(j);  
-                 if o > overlap  
-                     suppress = [suppress; pos];  
-                 end  
-             end  
-         end  
-         I(suppress) = [];  
-     end  
- end  
      输入的窗口的位置和分数，以及窗口面积交叉是多大比例进行抑制。boxes应当是N*5的矩阵，一行代表着一个窗口，包括[x,y,width,height ,score ]，overlap是介于0~1的实数。输出的是局部分数最大的窗口序号序列。
     程序解读：首先计算出所有窗口的面积，对所有窗口的分数进行从小到大排序取出最高分数的序号。循环计算1到次高分数窗口与最高分数窗口的交叉面积与两者间最小面积的比例，若超过overlap那么把这一窗口抑制了。交叉面积怎么计算呢？如下图对应于程序
**[plain]**[view
 plain](http://blog.csdn.net/H2008066215019910120/article/details/25917609#)[copy](http://blog.csdn.net/H2008066215019910120/article/details/25917609#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/347894/fork)
- xx1 = max(x1(i), x1(j));  
- yy1 = max(y1(i), y1(j));  
- xx2 = min(x2(i), x2(j));  
- yy2 = min(y2(i), y2(j));  
- w = xx2-xx1+1;  
- h = yy2-yy1+1;  
- if w > 0 && h > 0  
-     % compute overlap  
-     o = w * h / min(area(i),area(j));    
-      if o > overlap  
-         suppress = [suppress; pos];  
-     end  
- end  
![](https://img-blog.csdn.net/20140515214611031)
