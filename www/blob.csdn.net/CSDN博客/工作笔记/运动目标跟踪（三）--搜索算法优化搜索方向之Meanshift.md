# 运动目标跟踪（三）--搜索算法优化搜索方向之Meanshift - 工作笔记 - CSDN博客





2016年06月25日 16:35:38[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6825
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

[http://blog.csdn.net/jinshengtao/article/details/30258833](http://blog.csdn.net/jinshengtao/article/details/30258833)

这次将介绍基于MeanShift的目标跟踪算法，首先谈谈简介，然后给出算法实现流程，最后实现了一个单目标跟踪的MeanShift算法【matlab/c两个版本】

      csdn贴公式比较烦，原谅我直接截图了…



一、简介

     首先扯扯无参密度估计理论，无参密度估计也叫做非参数估计，属于数理统计的一个分支，和参数密度估计共同构成了概率密度估计方法。参数密度估计方法要求特征空间服从一个已知的概率密度函数，在实际的应用中这个条件很难达到。而无参数密度估计方法对先验知识要求最少，完全依靠训练数据进行估计，并且可以用于任意形状的密度估计。所以依靠无参密度估计方法，即不事先规定概率密度函数的结构形式，在某一连续点处的密度函数值可由该点邻域中的若干样本点估计得出。常用的无参密度估计方法有：直方图法、最近邻域法和核密度估计法。

     MeanShift算法正是属于核密度估计法，它不需要任何先验知识而完全依靠特征空间中样本点的计算其密度函数值。对于一组采样数据，直方图法通常把数据的值域分成若干相等的区间，数据按区间分成若干组，每组数据的个数与总参数个数的比率就是每个单元的概率值；核密度估计法的原理相似于直方图法，只是多了一个用于平滑数据的核函数。采用核函数估计法，在采样充分的情况下，能够渐进地收敛于任意的密度函数，即可以对服从任何分布的数据进行密度估计。

     然后谈谈MeanShift的基本思想及物理含义：

![](https://img-blog.csdn.net/20140612162112453)

![](https://img-blog.csdn.net/20140612162121671)

![](https://img-blog.csdn.net/20140612162126593)

    此外，从公式1中可以看到，只要是落入Sh的采样点，无论其离中心x的远近，对最终的Mh(x)计算的贡献是一样的。然而在现实跟踪过程中，当跟踪目标出现遮挡等影响时，由于外层的像素值容易受遮挡或背景的影响，所以目标模型中心附近的像素比靠外的像素更可靠。因此，对于所有采样点，每个样本点的重要性应该是不同的，离中心点越远，其权值应该越小。故引入核函数和权重系数来提高跟踪算法的鲁棒性并增加搜索跟踪能力。

      接下来，谈谈核函数：

![](https://img-blog.csdn.net/20140612162228578)

![](https://img-blog.csdn.net/20140612162236375)

核函数也叫窗口函数，在核估计中起到平滑的作用。常用的核函数有：Uniform，Epannechnikov，Gaussian等。本文算法只用到了Epannechnikov，它数序定义如下：

![](https://img-blog.csdn.net/20140612162317609)

二、基于MeanShift的目标跟踪算法

     基于均值漂移的目标跟踪算法通过分别计算目标区域和候选区域内像素的特征值概率得到关于目标模型和候选模型的描述，然后利用相似函数度量初始帧目标模型和当前帧的候选模版的相似性，选择使相似函数最大的候选模型并得到关于目标模型的Meanshift向量，这个向量正是目标由初始位置向正确位置移动的向量。由于均值漂移算法的快速收敛性，通过不断迭代计算Meanshift向量，算法最终将收敛到目标的真实位置，达到跟踪的目的。

     下面通过图示直观的说明MeanShift跟踪算法的基本原理。如下图所示：目标跟踪开始于数据点xi0（空心圆点xi0，xi1，…，xiN表示的是中心点，上标表示的是的迭代次数，周围的黑色圆点表示不断移动中的窗口样本点，虚线圆圈代表的是密度估计窗口的大小）。箭头表示样本点相对于核函数中心点的漂移向量，平均的漂移向量会指向样本点最密集的方向，也就是梯度方向。因为
 Meanshift 算法是收敛的，因此在当前帧中通过反复迭代搜索特征空间中样本点最密集的区域，搜索点沿着样本点密度增加的方向“漂移”到局部密度极大点点xiN，也就是被认为的目标位置，从而达到跟踪的目的，MeanShift 跟踪过程结束。

![](https://img-blog.csdn.net/20140612162405312)





![](https://img-blog.csdn.net/20140612162559703)

![](https://img-blog.csdn.net/20140612162604828)

![](https://img-blog.csdn.net/20140612162612562)

![](https://img-blog.csdn.net/20140612162619437)

![](https://img-blog.csdn.net/20140612162627406)

运动目标的实现过程【具体算法】：

![](https://img-blog.csdn.net/20140612162755734)



三、代码实现

说明：

1.RGB颜色空间刨分，采用16*16*16的直方图

2.目标模型和候选模型的概率密度计算公式参照上文

3.opencv版本运行：按P停止，截取目标，再按P，进行单目标跟踪

4.Matlab版本，将视频改为图片序列，第一帧停止，手工标定目标，双击目标区域，进行单目标跟踪。



matlab版本：





**[plain]**[view plain](http://blog.csdn.net/jinshengtao/article/details/30258833#)[copy](http://blog.csdn.net/jinshengtao/article/details/30258833#)[print](http://blog.csdn.net/jinshengtao/article/details/30258833#)[?](http://blog.csdn.net/jinshengtao/article/details/30258833#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- function [] = select()  
- close all;  
- clear all;  
- %%%%%%%%%%%%%%%%%%根据一幅目标全可见的图像圈定跟踪目标%%%%%%%%%%%%%%%%%%%%%%%  
- I=imread('result72.jpg');  
- figure(1);  
- imshow(I);  
- 
- 
- [temp,rect]=imcrop(I);  
- [a,b,c]=size(temp);         %a:row,b:col  
- 
- 
- %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%计算目标图像的权值矩阵%%%%%%%%%%%%%%%%%%%%%%%  
- y(1)=a/2;  
- y(2)=b/2;  
- tic_x=rect(1)+rect(3)/2;  
- tic_y=rect(2)+rect(4)/2;  
- m_wei=zeros(a,b);%权值矩阵  
- h=y(1)^2+y(2)^2 ;%带宽  
- 
- 
- for i=1:a  
-     for j=1:b  
-         dist=(i-y(1))^2+(j-y(2))^2;  
-         m_wei(i,j)=1-dist/h; %epanechnikov profile  
-     end  
- end  
- C=1/sum(sum(m_wei));%归一化系数  
- 
- 
- %计算目标权值直方图qu  
- %hist1=C*wei_hist(temp,m_wei,a,b);%target model  
- hist1=zeros(1,4096);  
- for i=1:a  
-     for j=1:b  
-         %rgb颜色空间量化为16*16*16 bins  
-         q_r=fix(double(temp(i,j,1))/16);  %fix为趋近0取整函数  
-         q_g=fix(double(temp(i,j,2))/16);  
-         q_b=fix(double(temp(i,j,3))/16);  
-         q_temp=q_r*256+q_g*16+q_b;            %设置每个像素点红色、绿色、蓝色分量所占比重  
-         hist1(q_temp+1)= hist1(q_temp+1)+m_wei(i,j);    %计算直方图统计中每个像素点占的权重  
-     end  
- end  
- hist1=hist1*C;  
- rect(3)=ceil(rect(3));  
- rect(4)=ceil(rect(4));  
- 
- 
- 
- 
- %%%%%%%%%%%%%%%%%%%%%%%%%读取序列图像  
- myfile=dir('D:\matlab7\work\mean shift\image\*.jpg');  
- lengthfile=length(myfile);  
- 
- 
- for l=1:lengthfile  
-     Im=imread(myfile(l).name);  
-     num=0;  
-     Y=[2,2];  
- 
- 
-     %%%%%%%mean shift迭代  
-     while((Y(1)^2+Y(2)^2>0.5)&num<20)   %迭代条件  
-         num=num+1;  
-         temp1=imcrop(Im,rect);  
-         %计算侯选区域直方图  
-         %hist2=C*wei_hist(temp1,m_wei,a,b);%target candidates pu  
-         hist2=zeros(1,4096);  
-         for i=1:a  
-             for j=1:b  
-                 q_r=fix(double(temp1(i,j,1))/16);  
-                 q_g=fix(double(temp1(i,j,2))/16);  
-                 q_b=fix(double(temp1(i,j,3))/16);  
-                 q_temp1(i,j)=q_r*256+q_g*16+q_b;  
-                 hist2(q_temp1(i,j)+1)= hist2(q_temp1(i,j)+1)+m_wei(i,j);  
-             end  
-         end  
-         hist2=hist2*C;  
-         figure(2);  
-         subplot(1,2,1);  
-         plot(hist2);  
-         hold on;  
- 
-         w=zeros(1,4096);  
-         for i=1:4096  
-             if(hist2(i)~=0) %不等于  
-                 w(i)=sqrt(hist1(i)/hist2(i));  
-             else  
-                 w(i)=0;  
-             end  
-         end  
- 
- 
- 
-         %变量初始化  
-         sum_w=0;  
-         xw=[0,0];  
-         for i=1:a;  
-             for j=1:b  
-                 sum_w=sum_w+w(uint32(q_temp1(i,j))+1);  
-                 xw=xw+w(uint32(q_temp1(i,j))+1)*[i-y(1)-0.5,j-y(2)-0.5];  
-             end  
-         end  
-         Y=xw/sum_w;  
-         %中心点位置更新  
-         rect(1)=rect(1)+Y(2);  
-         rect(2)=rect(2)+Y(1);  
-     end  
- 
- 
-     %%%跟踪轨迹矩阵%%%  
-     tic_x=[tic_x;rect(1)+rect(3)/2];  
-     tic_y=[tic_y;rect(2)+rect(4)/2];  
- 
-     v1=rect(1);  
-     v2=rect(2);  
-     v3=rect(3);  
-     v4=rect(4);  
-     %%%显示跟踪结果%%%  
-     subplot(1,2,2);  
-     imshow(uint8(Im));  
-     title('目标跟踪结果及其运动轨迹');  
-     hold on;  
-     plot([v1,v1+v3],[v2,v2],[v1,v1],[v2,v2+v4],[v1,v1+v3],[v2+v4,v2+v4],[v1+v3,v1+v3],[v2,v2+v4],'LineWidth',2,'Color','r');  
-     plot(tic_x,tic_y,'LineWidth',2,'Color','b');  
- 
- 
- end  


```
function [] = select()
close all;
clear all;
%%%%%%%%%%%%%%%%%%根据一幅目标全可见的图像圈定跟踪目标%%%%%%%%%%%%%%%%%%%%%%%
I=imread('result72.jpg');
figure(1);
imshow(I);


[temp,rect]=imcrop(I);
[a,b,c]=size(temp); 		%a:row,b:col


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%计算目标图像的权值矩阵%%%%%%%%%%%%%%%%%%%%%%%
y(1)=a/2;
y(2)=b/2;
tic_x=rect(1)+rect(3)/2;
tic_y=rect(2)+rect(4)/2;
m_wei=zeros(a,b);%权值矩阵
h=y(1)^2+y(2)^2 ;%带宽


for i=1:a
    for j=1:b
        dist=(i-y(1))^2+(j-y(2))^2;
        m_wei(i,j)=1-dist/h; %epanechnikov profile
    end
end
C=1/sum(sum(m_wei));%归一化系数


%计算目标权值直方图qu
%hist1=C*wei_hist(temp,m_wei,a,b);%target model
hist1=zeros(1,4096);
for i=1:a
    for j=1:b
        %rgb颜色空间量化为16*16*16 bins
        q_r=fix(double(temp(i,j,1))/16);  %fix为趋近0取整函数
        q_g=fix(double(temp(i,j,2))/16);
        q_b=fix(double(temp(i,j,3))/16);
        q_temp=q_r*256+q_g*16+q_b;            %设置每个像素点红色、绿色、蓝色分量所占比重
        hist1(q_temp+1)= hist1(q_temp+1)+m_wei(i,j);    %计算直方图统计中每个像素点占的权重
    end
end
hist1=hist1*C;
rect(3)=ceil(rect(3));
rect(4)=ceil(rect(4));




%%%%%%%%%%%%%%%%%%%%%%%%%读取序列图像
myfile=dir('D:\matlab7\work\mean shift\image\*.jpg');
lengthfile=length(myfile);


for l=1:lengthfile
    Im=imread(myfile(l).name);
    num=0;
    Y=[2,2];
    
    
    %%%%%%%mean shift迭代
    while((Y(1)^2+Y(2)^2>0.5)&num<20)   %迭代条件
        num=num+1;
        temp1=imcrop(Im,rect);
        %计算侯选区域直方图
        %hist2=C*wei_hist(temp1,m_wei,a,b);%target candidates pu
        hist2=zeros(1,4096);
        for i=1:a
            for j=1:b
                q_r=fix(double(temp1(i,j,1))/16);
                q_g=fix(double(temp1(i,j,2))/16);
                q_b=fix(double(temp1(i,j,3))/16);
                q_temp1(i,j)=q_r*256+q_g*16+q_b;
                hist2(q_temp1(i,j)+1)= hist2(q_temp1(i,j)+1)+m_wei(i,j);
            end
        end
        hist2=hist2*C;
        figure(2);
        subplot(1,2,1);
        plot(hist2);
        hold on;
        
        w=zeros(1,4096);
        for i=1:4096
            if(hist2(i)~=0) %不等于
                w(i)=sqrt(hist1(i)/hist2(i));
            else
                w(i)=0;
            end
        end
        
        
        
        %变量初始化
        sum_w=0;
        xw=[0,0];
        for i=1:a;
            for j=1:b
                sum_w=sum_w+w(uint32(q_temp1(i,j))+1);
                xw=xw+w(uint32(q_temp1(i,j))+1)*[i-y(1)-0.5,j-y(2)-0.5];
            end
        end
        Y=xw/sum_w;
        %中心点位置更新
        rect(1)=rect(1)+Y(2);
        rect(2)=rect(2)+Y(1);
    end
    
    
    %%%跟踪轨迹矩阵%%%
    tic_x=[tic_x;rect(1)+rect(3)/2];
    tic_y=[tic_y;rect(2)+rect(4)/2];
    
    v1=rect(1);
    v2=rect(2);
    v3=rect(3);
    v4=rect(4);
    %%%显示跟踪结果%%%
    subplot(1,2,2);
    imshow(uint8(Im));
    title('目标跟踪结果及其运动轨迹');
    hold on;
    plot([v1,v1+v3],[v2,v2],[v1,v1],[v2,v2+v4],[v1,v1+v3],[v2+v4,v2+v4],[v1+v3,v1+v3],[v2,v2+v4],'LineWidth',2,'Color','r');
    plot(tic_x,tic_y,'LineWidth',2,'Color','b');
    
    
end
```




 运行结果：

![](https://img-blog.csdn.net/20140612163122140)







opencv版本：



**[cpp]**[view plain](http://blog.csdn.net/jinshengtao/article/details/30258833#)[copy](http://blog.csdn.net/jinshengtao/article/details/30258833#)[print](http://blog.csdn.net/jinshengtao/article/details/30258833#)[?](http://blog.csdn.net/jinshengtao/article/details/30258833#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include "stdafx.h"
- #include "cv.h"
- #include "highgui.h"
- #define  u_char unsigned char
- #define  DIST 0.5
- #define  NUM 20
- 
- //全局变量
- bool pause = false;  
- bool is_tracking = false;  
- CvRect drawing_box;  
- IplImage *current;  
- double *hist1, *hist2;  
- double *m_wei;                                                                  //权值矩阵
- double C = 0.0;                                                                //归一化系数
- 
- void init_target(double *hist1, double *m_wei, IplImage *current)  
- {  
-     IplImage *pic_hist = 0;  
- int t_h, t_w, t_x, t_y;  
- double h, dist;  
- int i, j;  
- int q_r, q_g, q_b, q_temp;  
- 
-     t_h = drawing_box.height;  
-     t_w = drawing_box.width;  
-     t_x = drawing_box.x;  
-     t_y = drawing_box.y;  
- 
-     h = pow(((double)t_w)/2,2) + pow(((double)t_h)/2,2);            //带宽
-     pic_hist = cvCreateImage(cvSize(300,200),IPL_DEPTH_8U,3);     //生成直方图图像
- 
- //初始化权值矩阵和目标直方图
- for (i = 0;i < t_w*t_h;i++)  
-     {  
-         m_wei[i] = 0.0;  
-     }  
- 
- for (i=0;i<4096;i++)  
-     {  
-         hist1[i] = 0.0;  
-     }  
- 
- for (i = 0;i < t_h; i++)  
-     {  
- for (j = 0;j < t_w; j++)  
-         {  
-             dist = pow(i - (double)t_h/2,2) + pow(j - (double)t_w/2,2);  
-             m_wei[i * t_w + j] = 1 - dist / h;   
- //printf("%f\n",m_wei[i * t_w + j]);
-             C += m_wei[i * t_w + j] ;  
-         }  
-     }  
- 
- //计算目标权值直方
- for (i = t_y;i < t_y + t_h; i++)  
-     {  
- for (j = t_x;j < t_x + t_w; j++)  
-         {  
- //rgb颜色空间量化为16*16*16 bins
-             q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;  
-             q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;  
-             q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;  
-             q_temp = q_r * 256 + q_g * 16 + q_b;  
-             hist1[q_temp] =  hist1[q_temp] +  m_wei[(i - t_y) * t_w + (j - t_x)] ;  
-         }  
-     }  
- 
- //归一化直方图
- for (i=0;i<4096;i++)  
-     {  
-         hist1[i] = hist1[i] / C;  
- //printf("%f\n",hist1[i]);
-     }  
- 
- //生成目标直方图
- double temp_max=0.0;  
- 
- for (i = 0;i < 4096;i++)         //求直方图最大值，为了归一化
-     {  
- //printf("%f\n",val_hist[i]);
- if (temp_max < hist1[i])  
-         {  
-             temp_max = hist1[i];  
-         }  
-     }  
- //画直方图
-     CvPoint p1,p2;  
- double bin_width=(double)pic_hist->width/4096;  
- double bin_unith=(double)pic_hist->height/temp_max;  
- 
- for (i = 0;i < 4096; i++)  
-     {  
-         p1.x = i * bin_width;  
-         p1.y = pic_hist->height;  
-         p2.x = (i + 1)*bin_width;  
-         p2.y = pic_hist->height - hist1[i] * bin_unith;  
- //printf("%d,%d,%d,%d\n",p1.x,p1.y,p2.x,p2.y);
-         cvRectangle(pic_hist,p1,p2,cvScalar(0,255,0),-1,8,0);  
-     }  
-     cvSaveImage("hist1.jpg",pic_hist);  
-     cvReleaseImage(&pic_hist);  
- }  
- 
- void MeanShift_Tracking(IplImage *current)  
- {  
- int num = 0, i = 0, j = 0;  
- int t_w = 0, t_h = 0, t_x = 0, t_y = 0;  
- double *w = 0, *hist2 = 0;  
- double sum_w = 0, x1 = 0, x2 = 0,y1 = 2.0, y2 = 2.0;  
- int q_r, q_g, q_b;  
- int *q_temp;  
-     IplImage *pic_hist = 0;  
- 
-     t_w = drawing_box.width;  
-     t_h = drawing_box.height;  
- 
-     pic_hist = cvCreateImage(cvSize(300,200),IPL_DEPTH_8U,3);     //生成直方图图像
-     hist2 = (double *)malloc(sizeof(double)*4096);  
-     w = (double *)malloc(sizeof(double)*4096);  
-     q_temp = (int *)malloc(sizeof(int)*t_w*t_h);  
- 
- while ((pow(y2,2) + pow(y1,2) > 0.5)&& (num < NUM))  
-     {  
-         num++;  
-         t_x = drawing_box.x;  
-         t_y = drawing_box.y;  
-         memset(q_temp,0,sizeof(int)*t_w*t_h);  
- for (i = 0;i<4096;i++)  
-         {  
-             w[i] = 0.0;  
-             hist2[i] = 0.0;  
-         }  
- 
- for (i = t_y;i < t_h + t_y;i++)  
-         {  
- for (j = t_x;j < t_w + t_x;j++)  
-             {  
- //rgb颜色空间量化为16*16*16 bins
-                 q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;  
-                 q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;  
-                 q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;  
-                 q_temp[(i - t_y) *t_w + j - t_x] = q_r * 256 + q_g * 16 + q_b;  
-                 hist2[q_temp[(i - t_y) *t_w + j - t_x]] =  hist2[q_temp[(i - t_y) *t_w + j - t_x]] +  m_wei[(i - t_y) * t_w + j - t_x] ;  
-             }  
-         }  
- 
- //归一化直方图
- for (i=0;i<4096;i++)  
-         {  
-             hist2[i] = hist2[i] / C;  
- //printf("%f\n",hist2[i]);
-         }  
- //生成目标直方图
- double temp_max=0.0;  
- 
- for (i=0;i<4096;i++)         //求直方图最大值，为了归一化
-         {  
- if (temp_max < hist2[i])  
-             {  
-                 temp_max = hist2[i];  
-             }  
-         }  
- //画直方图
-         CvPoint p1,p2;  
- double bin_width=(double)pic_hist->width/(4368);  
- double bin_unith=(double)pic_hist->height/temp_max;  
- 
- for (i = 0;i < 4096; i++)  
-         {  
-             p1.x = i * bin_width;  
-             p1.y = pic_hist->height;  
-             p2.x = (i + 1)*bin_width;  
-             p2.y = pic_hist->height - hist2[i] * bin_unith;  
-             cvRectangle(pic_hist,p1,p2,cvScalar(0,255,0),-1,8,0);  
-         }  
-         cvSaveImage("hist2.jpg",pic_hist);  
- 
- for (i = 0;i < 4096;i++)  
-         {  
- if (hist2[i] != 0)  
-             {  
-                 w[i] = sqrt(hist1[i]/hist2[i]);  
-             }else
-             {  
-                 w[i] = 0;  
-             }  
-         }  
- 
-         sum_w = 0.0;  
-         x1 = 0.0;  
-         x2 = 0.0;  
- 
- for (i = 0;i < t_h; i++)  
-         {  
- for (j = 0;j < t_w; j++)  
-             {  
- //printf("%d\n",q_temp[i * t_w + j]);
-                 sum_w = sum_w + w[q_temp[i * t_w + j]];  
-                 x1 = x1 + w[q_temp[i * t_w + j]] * (i - t_h/2);  
-                 x2 = x2 + w[q_temp[i * t_w + j]] * (j - t_w/2);  
-             }  
-         }  
-         y1 = x1 / sum_w;  
-         y2 = x2 / sum_w;  
- 
- //中心点位置更新
-         drawing_box.x += y2;  
-         drawing_box.y += y1;  
- 
- //printf("%d,%d\n",drawing_box.x,drawing_box.y);
-     }  
-     free(hist2);  
-     free(w);  
-     free(q_temp);  
- //显示跟踪结果
-     cvRectangle(current,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0),2);  
-     cvShowImage("Meanshift",current);  
- //cvSaveImage("result.jpg",current);
-     cvReleaseImage(&pic_hist);  
- }  
- 
- void onMouse( int event, int x, int y, int flags, void *param )  
- {  
- if (pause)  
-     {  
- switch(event)  
-         {  
- case CV_EVENT_LBUTTONDOWN:   
- //the left up point of the rect
-             drawing_box.x=x;  
-             drawing_box.y=y;  
- break;  
- case CV_EVENT_LBUTTONUP:  
- //finish drawing the rect (use color green for finish)
-             drawing_box.width=x-drawing_box.x;  
-             drawing_box.height=y-drawing_box.y;  
-             cvRectangle(current,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0),2);  
-             cvShowImage("Meanshift",current);  
- 
- //目标初始化
-             hist1 = (double *)malloc(sizeof(double)*16*16*16);  
-             m_wei =  (double *)malloc(sizeof(double)*drawing_box.height*drawing_box.width);  
-             init_target(hist1, m_wei, current);  
-             is_tracking = true;  
- break;  
-         }  
- return;  
-     }  
- }  
- 
- 
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
-     CvCapture *capture=cvCreateFileCapture("test.avi");  
-     current = cvQueryFrame(capture);  
- char res[20];  
- int nframe = 0;  
- 
- while (1)  
-     {     
- /*  sprintf(res,"result%d.jpg",nframe);
-         cvSaveImage(res,current);
-         nframe++;*/
- if(is_tracking)  
-         {  
-             MeanShift_Tracking(current);  
-         }  
- 
- int c=cvWaitKey(1);  
- //暂停
- if(c == 'p')   
-         {  
-             pause = true;  
-             cvSetMouseCallback( "Meanshift", onMouse, 0 );  
-         }  
- while(pause){  
- if(cvWaitKey(0) == 'p')  
-                 pause = false;  
-         }  
-         cvShowImage("Meanshift",current);  
-         current = cvQueryFrame(capture); //抓取一帧
-     }  
- 
-     cvNamedWindow("Meanshift",1);  
-     cvReleaseCapture(&capture);  
-     cvDestroyWindow("Meanshift");  
- return 0;  
- }  


```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#define  u_char unsigned char
#define  DIST 0.5
#define  NUM 20

//全局变量
bool pause = false;
bool is_tracking = false;
CvRect drawing_box;
IplImage *current;
double *hist1, *hist2;
double *m_wei;																	//权值矩阵
double C = 0.0;																   //归一化系数

void init_target(double *hist1, double *m_wei, IplImage *current)
{
	IplImage *pic_hist = 0;
	int t_h, t_w, t_x, t_y;
	double h, dist;
	int i, j;
	int q_r, q_g, q_b, q_temp;
	
	t_h = drawing_box.height;
	t_w = drawing_box.width;
	t_x = drawing_box.x;
	t_y = drawing_box.y;

	h = pow(((double)t_w)/2,2) + pow(((double)t_h)/2,2);			//带宽
	pic_hist = cvCreateImage(cvSize(300,200),IPL_DEPTH_8U,3);     //生成直方图图像

	//初始化权值矩阵和目标直方图
	for (i = 0;i < t_w*t_h;i++)
	{
		m_wei[i] = 0.0;
	}

	for (i=0;i<4096;i++)
	{
		hist1[i] = 0.0;
	}

	for (i = 0;i < t_h; i++)
	{
		for (j = 0;j < t_w; j++)
		{
			dist = pow(i - (double)t_h/2,2) + pow(j - (double)t_w/2,2);
			m_wei[i * t_w + j] = 1 - dist / h; 
			//printf("%f\n",m_wei[i * t_w + j]);
			C += m_wei[i * t_w + j] ;
		}
	}

	//计算目标权值直方
	for (i = t_y;i < t_y + t_h; i++)
	{
		for (j = t_x;j < t_x + t_w; j++)
		{
			//rgb颜色空间量化为16*16*16 bins
			q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;
			q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;
			q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;
			q_temp = q_r * 256 + q_g * 16 + q_b;
			hist1[q_temp] =  hist1[q_temp] +  m_wei[(i - t_y) * t_w + (j - t_x)] ;
		}
	}

	//归一化直方图
	for (i=0;i<4096;i++)
	{
		hist1[i] = hist1[i] / C;
		//printf("%f\n",hist1[i]);
	}

	//生成目标直方图
	double temp_max=0.0;

	for (i = 0;i < 4096;i++)			//求直方图最大值，为了归一化
	{
		//printf("%f\n",val_hist[i]);
		if (temp_max < hist1[i])
		{
			temp_max = hist1[i];
		}
	}
	//画直方图
	CvPoint p1,p2;
	double bin_width=(double)pic_hist->width/4096;
	double bin_unith=(double)pic_hist->height/temp_max;

	for (i = 0;i < 4096; i++)
	{
		p1.x = i * bin_width;
		p1.y = pic_hist->height;
		p2.x = (i + 1)*bin_width;
		p2.y = pic_hist->height - hist1[i] * bin_unith;
		//printf("%d,%d,%d,%d\n",p1.x,p1.y,p2.x,p2.y);
		cvRectangle(pic_hist,p1,p2,cvScalar(0,255,0),-1,8,0);
	}
	cvSaveImage("hist1.jpg",pic_hist);
	cvReleaseImage(&pic_hist);
}

void MeanShift_Tracking(IplImage *current)
{
	int num = 0, i = 0, j = 0;
	int t_w = 0, t_h = 0, t_x = 0, t_y = 0;
	double *w = 0, *hist2 = 0;
	double sum_w = 0, x1 = 0, x2 = 0,y1 = 2.0, y2 = 2.0;
	int q_r, q_g, q_b;
	int *q_temp;
	IplImage *pic_hist = 0;

	t_w = drawing_box.width;
	t_h = drawing_box.height;
	
	pic_hist = cvCreateImage(cvSize(300,200),IPL_DEPTH_8U,3);     //生成直方图图像
	hist2 = (double *)malloc(sizeof(double)*4096);
	w = (double *)malloc(sizeof(double)*4096);
	q_temp = (int *)malloc(sizeof(int)*t_w*t_h);

	while ((pow(y2,2) + pow(y1,2) > 0.5)&& (num < NUM))
	{
		num++;
		t_x = drawing_box.x;
		t_y = drawing_box.y;
		memset(q_temp,0,sizeof(int)*t_w*t_h);
		for (i = 0;i<4096;i++)
		{
			w[i] = 0.0;
			hist2[i] = 0.0;
		}

		for (i = t_y;i < t_h + t_y;i++)
		{
			for (j = t_x;j < t_w + t_x;j++)
			{
				//rgb颜色空间量化为16*16*16 bins
				q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;
				q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;
				q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;
				q_temp[(i - t_y) *t_w + j - t_x] = q_r * 256 + q_g * 16 + q_b;
				hist2[q_temp[(i - t_y) *t_w + j - t_x]] =  hist2[q_temp[(i - t_y) *t_w + j - t_x]] +  m_wei[(i - t_y) * t_w + j - t_x] ;
			}
		}

		//归一化直方图
		for (i=0;i<4096;i++)
		{
			hist2[i] = hist2[i] / C;
			//printf("%f\n",hist2[i]);
		}
		//生成目标直方图
		double temp_max=0.0;

		for (i=0;i<4096;i++)			//求直方图最大值，为了归一化
		{
			if (temp_max < hist2[i])
			{
				temp_max = hist2[i];
			}
		}
		//画直方图
		CvPoint p1,p2;
		double bin_width=(double)pic_hist->width/(4368);
		double bin_unith=(double)pic_hist->height/temp_max;

		for (i = 0;i < 4096; i++)
		{
			p1.x = i * bin_width;
			p1.y = pic_hist->height;
			p2.x = (i + 1)*bin_width;
			p2.y = pic_hist->height - hist2[i] * bin_unith;
			cvRectangle(pic_hist,p1,p2,cvScalar(0,255,0),-1,8,0);
		}
		cvSaveImage("hist2.jpg",pic_hist);
	
		for (i = 0;i < 4096;i++)
		{
			if (hist2[i] != 0)
			{
				w[i] = sqrt(hist1[i]/hist2[i]);
			}else
			{
				w[i] = 0;
			}
		}
			
		sum_w = 0.0;
		x1 = 0.0;
		x2 = 0.0;

		for (i = 0;i < t_h; i++)
		{
			for (j = 0;j < t_w; j++)
			{
				//printf("%d\n",q_temp[i * t_w + j]);
				sum_w = sum_w + w[q_temp[i * t_w + j]];
				x1 = x1 + w[q_temp[i * t_w + j]] * (i - t_h/2);
				x2 = x2 + w[q_temp[i * t_w + j]] * (j - t_w/2);
			}
		}
		y1 = x1 / sum_w;
		y2 = x2 / sum_w;
		
		//中心点位置更新
		drawing_box.x += y2;
		drawing_box.y += y1;

		//printf("%d,%d\n",drawing_box.x,drawing_box.y);
	}
	free(hist2);
	free(w);
	free(q_temp);
	//显示跟踪结果
	cvRectangle(current,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0),2);
	cvShowImage("Meanshift",current);
	//cvSaveImage("result.jpg",current);
	cvReleaseImage(&pic_hist);
}

void onMouse( int event, int x, int y, int flags, void *param )
{
	if (pause)
	{
		switch(event)
		{
		case CV_EVENT_LBUTTONDOWN: 
			//the left up point of the rect
			drawing_box.x=x;
			drawing_box.y=y;
			break;
		case CV_EVENT_LBUTTONUP:
			//finish drawing the rect (use color green for finish)
			drawing_box.width=x-drawing_box.x;
			drawing_box.height=y-drawing_box.y;
			cvRectangle(current,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0),2);
			cvShowImage("Meanshift",current);
			
			//目标初始化
			hist1 = (double *)malloc(sizeof(double)*16*16*16);
			m_wei =  (double *)malloc(sizeof(double)*drawing_box.height*drawing_box.width);
			init_target(hist1, m_wei, current);
			is_tracking = true;
			break;
		}
		return;
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	CvCapture *capture=cvCreateFileCapture("test.avi");
	current = cvQueryFrame(capture);
	char res[20];
	int nframe = 0;

	while (1)
	{	
	/*	sprintf(res,"result%d.jpg",nframe);
		cvSaveImage(res,current);
		nframe++;*/
		if(is_tracking)
		{
			MeanShift_Tracking(current);
		}

		int c=cvWaitKey(1);
		//暂停
		if(c == 'p') 
		{
			pause = true;
			cvSetMouseCallback( "Meanshift", onMouse, 0 );
		}
		while(pause){
			if(cvWaitKey(0) == 'p')
				pause = false;
		}
		cvShowImage("Meanshift",current);
		current = cvQueryFrame(capture); //抓取一帧
	}

	cvNamedWindow("Meanshift",1);
	cvReleaseCapture(&capture);
	cvDestroyWindow("Meanshift");
	return 0;
}
```

运行结果：

![](https://img-blog.csdn.net/20140612163256578)![](https://img-blog.csdn.net/20140612163313859)

初始目标直方图：

![](https://img-blog.csdn.net/20140612163746265)


候选目标直方图：

![](https://img-blog.csdn.net/20140612163802015)

﻿﻿



