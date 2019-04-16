# MeanShift算法C++解析（四） - lyx的专栏 - CSDN博客





2015年11月03日 21:30:55[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1500







之前的MeanShift算法只是一个大致的算法，没有任何附加的功能哦。现在还是从程序本身理解算法吧，等真的理解透了理论在来扯一下整个MeanShift 的无参概率算法的思想吧。

MeanShift算法在视频追踪的运用中有一点很重要，就是核函数，整个目标窗口叫做核函数窗口。所谓核函数吗，在视频追踪中就是之前提到的每一个像素点投票的票数，也就是重要程度啦！测试了三种核函数，分别是Epanechnikov核函数，Gaussian核函数和Uniform核函数，各自在程序中的计算公式如下所示：


m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

                m_wei[i * t_w + j] = exp(- dist / h/2);//Gaussian核函数

                m_wei[i * t_w + j] = 1- sqrt(dist / bandNorm);//uniform核函数

    在我的测试过程中，原本以为Epanechnikov核函数或者高斯核函数效果会比较好，因为追踪目标的中间点的权重比较大，而边缘的影响比较小。但是实际过程中竟然发现Uniform核函数的效果最好，比较吃惊。后期研究理论了再好好琢磨琢磨吧。



这两天修改过后的跟踪算法有自适应窗口的功能。所有代码在最后。重要代码简单说一下吧~


所谓窗口自适应就是根据匹配程度，自己会改变核函数窗口的大小，当然啦，在实际处理过程中我对窗口变化的最上下限做了限制。并且大小变化尺度以10%进行，这也是大部分论文上提到的比例。希望后期自己能相处一个好的算法吧。








    int num = 0, i = 0, j = 0;

    int num_adj_smlr = 0,num_adj_bg = 0;

    double normCofKeep= 0.0,normCofSml = 0.0,normCofBg = 0.0;

    double bandNorm = 0.0,bandSml = 0.0,bandBg = 0.0;

    double dist = 0.0;

    double Bhattacharyya1 = 0,Bhattacharyya2 = 0,Bhattacharyya3 = 0;

    int t_w = 0, t_h = 0, t_x = 0, t_y = 0;

    int t_w_adj_smlr = 0,t_h_adj_smlr = 0,t_x_adj_smlr = 0,t_y_adj_smlr = 0;

    int t_w_adj_bg = 0,t_h_adj_bg = 0,t_x_adj_bg = 0,t_y_adj_bg = 0;




    double sum_w = 0, x1 = 0, x2 = 0,y1 = 2.0, y2 = 2.0;

    double sum_w_adj_smlr = 0, x1_adj_smlr = 0, x2_adj_smlr = 0,y1_adj_smlr = 2.0, y2_adj_smlr = 2.0;

    double sum_w_adj_bg = 0, x1_adj_bg = 0, x2_adj_bg = 0,y1_adj_bg = 2.0, y2_adj_bg = 2.0;

    int q_r, q_g, q_b;

    int *q_temp;

    double *m_weiNorm;

    double *m_weiSml;

    double *m_weiBg;

在最初的函数内部的变量定义的时候，基本定义了三份，一份用于变小，一份用于保持，一份用于放大。代码还没有优化，后期进行一下封装吧。







    t_w = drawing_box.width;

    t_h = drawing_box.height;

    t_x = drawing_box.x;

    t_y = drawing_box.y;

    //窗口自适应的中间量

   if(iniWidth  < 2*0.9*drawing_box.width)

   {

        t_w_adj_smlr = 0.9*drawing_box.width;

        t_x_adj_smlr = drawing_box.x+0.05*drawing_box.width;

   }

   else

   {

       t_w_adj_smlr = t_w;

       t_x_adj_smlr = t_x;

   }




   if(iniHigh < 2*0.9* drawing_box.height)

   {

        t_h_adj_smlr  =0.9* drawing_box.height;

        t_y_adj_smlr = drawing_box.y+0.05* drawing_box.height;

   }

   else

   {

        t_h_adj_smlr = t_h;

        t_y_adj_smlr = t_y;

   }




   if(2*iniWidth > 1.1*drawing_box.width)

   {

       t_w_adj_bg = 1.1*drawing_box.width;

       t_x_adj_bg = drawing_box.x-0.05*drawing_box.width;

   }

   else

    {

       t_w_adj_bg = t_w;

       t_x_adj_bg = t_x;

   }




   if(2*iniHigh > 1.1* drawing_box.height)

   {

       t_h_adj_bg  =1.1* drawing_box.height;

       t_y_adj_bg = drawing_box.y-0.05* drawing_box.height;

   }

   else

   {

       t_h_adj_bg  =t_h;

       t_y_adj_bg = t_y;

   }

这是做上下限之后修改窗口大小的结果。




    //=====band width generation=====




     bandNorm = pow(((double)t_w)/2,2) + pow(((double)t_h)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    bandSml = pow(((double)t_w_adj_smlr)/2,2) + pow(((double)t_h_adj_smlr)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    bandBg = pow(((double)t_w_adj_bg)/2,2) + pow(((double)t_h_adj_bg)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    //============bandwith generation over========

由于窗口大小的改变，带宽也会变化，所以也需要更新。一开始没有考虑到这一点，所以老是运行错误。

    //===========C权值归一化更新==========

    //normal uodate

    for (i = 0;i < t_h; i++)

    {

        for (j = 0;j < t_w; j++)

        {

            dist = pow(i - (double)t_h/2,2) + pow(j - (double)t_w/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiNorm[i * t_w + j] = 1- sqrt(dist / bandNorm);//uniform核函数

            normCofKeep += m_weiNorm[i * t_w + j] ;//统计权值矩阵的总和？

        }

    }

    //smaller update

    for (i = 0;i < t_h_adj_smlr; i++)

    {

        for (j = 0;j < t_w_adj_smlr; j++)

        {

            dist = pow(i - (double)t_h_adj_smlr/2,2) + pow(j - (double)t_w_adj_smlr/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiSml[i * t_w_adj_smlr + j] = 1- sqrt(dist / bandSml);//uniform核函数

            normCofSml += m_weiSml[i * t_w_adj_smlr + j] ;//统计权值矩阵的总和？

        }

    }

    //bigger uodate

    for (i = 0;i < t_h_adj_bg; i++)

    {

        for (j = 0;j < t_w_adj_bg; j++)

        {

            dist = pow(i - (double)t_h_adj_bg/2,2) + pow(j - (double)t_w_adj_bg/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiBg[i * t_w_adj_bg + j] = 1- sqrt(dist / bandBg);//uniform核函数

            normCofBg += m_weiBg[i * t_w_adj_bg + j] ;//统计权值矩阵的总和？

        }

    }

//=======update over=============




投票的总数也与窗口大小有关。所以需要分别计算。




    //Bhattacharyya系数比较

    for (i=0;i<4096;i++)

    {

        Bhattacharyya1 = Bhattacharyya1 +sqrt(hist1[i]*hist2[i]) ;

        Bhattacharyya2 = Bhattacharyya2 +sqrt(hist1[i]*hist3[i]) ;

        Bhattacharyya3 = Bhattacharyya3 +sqrt(hist1[i]*hist4[i]) ;

    }

这一个循环用来计算三个窗口与目标的Bhattacharyya系数，这个系数，昨天转载了一个博客，里面说了这个系数的意义。http://en.wikipedia.org/wiki/Bhattacharyya_distance可以去wiki百科上看一下。反正，就是，系数越大，和目标的相似度匹配度越高。所以，利用这一个特点决定，我们的核函数窗口是变大还是变小，或者不变。




不说什么了，上个优酷链接，是目前的效果视频。



http://v.youku.com/v_show/id_XOTMyNTE2MTg0.html








void MeanShift_Tracking(IplImage *current)

{

    int num = 0, i = 0, j = 0;

    int num_adj_smlr = 0,num_adj_bg = 0;

    double normCofKeep= 0.0,normCofSml = 0.0,normCofBg = 0.0;

    double bandNorm = 0.0,bandSml = 0.0,bandBg = 0.0;

    double dist = 0.0;

    double Bhattacharyya1 = 0,Bhattacharyya2 = 0,Bhattacharyya3 = 0;

    int t_w = 0, t_h = 0, t_x = 0, t_y = 0;

    int t_w_adj_smlr = 0,t_h_adj_smlr = 0,t_x_adj_smlr = 0,t_y_adj_smlr = 0;

    int t_w_adj_bg = 0,t_h_adj_bg = 0,t_x_adj_bg = 0,t_y_adj_bg = 0;




    double sum_w = 0, x1 = 0, x2 = 0,y1 = 2.0, y2 = 2.0;

    double sum_w_adj_smlr = 0, x1_adj_smlr = 0, x2_adj_smlr = 0,y1_adj_smlr = 2.0, y2_adj_smlr = 2.0;

    double sum_w_adj_bg = 0, x1_adj_bg = 0, x2_adj_bg = 0,y1_adj_bg = 2.0, y2_adj_bg = 2.0;

    int q_r, q_g, q_b;

    int *q_temp;

    double *m_weiNorm;

    double *m_weiSml;

    double *m_weiBg;




    t_w = drawing_box.width;

    t_h = drawing_box.height;

    t_x = drawing_box.x;

    t_y = drawing_box.y;

    //窗口自适应的中间量

   if(iniWidth  < 2*0.9*drawing_box.width)

   {

        t_w_adj_smlr = 0.9*drawing_box.width;

        t_x_adj_smlr = drawing_box.x+0.05*drawing_box.width;

   }

   else

   {

       t_w_adj_smlr = t_w;

       t_x_adj_smlr = t_x;

   }




   if(iniHigh < 2*0.9* drawing_box.height)

   {

        t_h_adj_smlr  =0.9* drawing_box.height;

        t_y_adj_smlr = drawing_box.y+0.05* drawing_box.height;

   }

   else

   {

        t_h_adj_smlr = t_h;

        t_y_adj_smlr = t_y;

   }




   if(2*iniWidth > 1.1*drawing_box.width)

   {

       t_w_adj_bg = 1.1*drawing_box.width;

       t_x_adj_bg = drawing_box.x-0.05*drawing_box.width;

   }

   else

    {

       t_w_adj_bg = t_w;

       t_x_adj_bg = t_x;

   }




   if(2*iniHigh > 1.1* drawing_box.height)

   {

       t_h_adj_bg  =1.1* drawing_box.height;

       t_y_adj_bg = drawing_box.y-0.05* drawing_box.height;

   }

   else

   {

       t_h_adj_bg  =t_h;

       t_y_adj_bg = t_y;

   }










    m_weiNorm =  (double *)malloc(sizeof(double)*(current->width)*(current->height));

    m_weiSml =  (double *)malloc(sizeof(double)*(current->width)*(current->height));

    m_weiBg =  (double *)malloc(sizeof(double)*(current->width)*(current->height));




    q_temp = (int *)malloc(sizeof(int)*(current->width)*(current->height));




    //=====band width generation=====




     bandNorm = pow(((double)t_w)/2,2) + pow(((double)t_h)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    bandSml = pow(((double)t_w_adj_smlr)/2,2) + pow(((double)t_h_adj_smlr)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    bandBg = pow(((double)t_w_adj_bg)/2,2) + pow(((double)t_h_adj_bg)/2,2); //带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）

    //============bandwith generation over========




    //===========C权值归一化更新==========

    //normal uodate

    for (i = 0;i < t_h; i++)

    {

        for (j = 0;j < t_w; j++)

        {

            dist = pow(i - (double)t_h/2,2) + pow(j - (double)t_w/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiNorm[i * t_w + j] = 1- sqrt(dist / bandNorm);//uniform核函数

            normCofKeep += m_weiNorm[i * t_w + j] ;//统计权值矩阵的总和？

        }

    }

    //smaller update

    for (i = 0;i < t_h_adj_smlr; i++)

    {

        for (j = 0;j < t_w_adj_smlr; j++)

        {

            dist = pow(i - (double)t_h_adj_smlr/2,2) + pow(j - (double)t_w_adj_smlr/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiSml[i * t_w_adj_smlr + j] = 1- sqrt(dist / bandSml);//uniform核函数

            normCofSml += m_weiSml[i * t_w_adj_smlr + j] ;//统计权值矩阵的总和？

        }

    }

    //bigger uodate

    for (i = 0;i < t_h_adj_bg; i++)

    {

        for (j = 0;j < t_w_adj_bg; j++)

        {

            dist = pow(i - (double)t_h_adj_bg/2,2) + pow(j - (double)t_w_adj_bg/2,2);//d= x^2+y^2

           // m_wei[i * t_w + j] = 1 - dist / h;//Epanechnikov核函数

           // m_wei[i * t_w + j] = exp(- dist / h/2);//高斯核函数

            m_weiBg[i * t_w_adj_bg + j] = 1- sqrt(dist / bandBg);//uniform核函数

            normCofBg += m_weiBg[i * t_w_adj_bg + j] ;//统计权值矩阵的总和？

        }

    }

//=======update over=============

//不变窗口

    while ((pow(y2,2) + pow(y1,2) > 0.5)&& (num < NUM))

    {

        num++;

        memset(q_temp,0,sizeof(int)*(current->width)*(current->height));//memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法

    //最后的向量权值矩阵的初始化

        for (i = 0;i<4096;i++)

        {

            w[i] = 0.0;

            hist2[i] = 0.0;

        }

    //未自适应窗口直方图生成

        for (i = t_y;i < t_h + t_y;i++)

        {

            for (j = t_x;j < t_w + t_x;j++)

            {

                //rgb颜色空间量化为16*16*16 bins//int widthStep;

                q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;

                q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;

                q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;

                q_temp[(i - t_y) *t_w + j - t_x] = q_r * 256 + q_g * 16 + q_b;

                hist2[q_temp[(i - t_y) *t_w + j - t_x]] =  hist2[q_temp[(i - t_y) *t_w + j - t_x]] +  m_weiNorm[(i - t_y) * t_w + j - t_x] ;

            }

        }

        //归一化直方图

        for (i=0;i<4096;i++)

        {

            hist2[i] = hist2[i] / normCofKeep;

        }

    //向量权值的生成

            for (i = 0;i < 4096;i++)

            {

                if (hist2[i] != 0)//有这样的色彩

                {

                    w[i] = sqrt(hist1[i]/hist2[i]);//其实是比较了两个都有的情况。只要有一个没有这种色彩，那么w的值就是零

                }else

                {

                    w[i] = 0;

                }

            }







        sum_w = 0.0;

        x1 = 0.0;

        x2 = 0.0;

    //生成MeanShift向量

        for (i = 0;i < t_h; i++)

        {

            for (j = 0;j < t_w; j++)

            {

                sum_w = sum_w + w[q_temp[i * t_w + j]];//q_temp[i * t_w + j]色彩状况的一个表征，具体还没有弄清楚

                x1 = x1 + w[q_temp[i * t_w + j]] * (i - t_h/2);//y direction

                x2 = x2 + w[q_temp[i * t_w + j]] * (j - t_w/2);

            }

        }

        y1 = x1 / sum_w;

        y2 = x2 / sum_w;




        //MeanShitf左上角位置更新

        t_x += y2;//y2为x方向偏移量

        t_y += y1;//y1为y方向偏移量

        //===========Output the offset value

       //qDebug( "x axis offset = %f", y2);

       //qDebug( "y axis offset = %f", y1);

    }

//缩小窗口=====================

    while ((pow(y2_adj_smlr,2) + pow(y1_adj_smlr,2) > 0.5)&& (num_adj_smlr < NUM))

    {

        num_adj_smlr++;

        memset(q_temp,0,sizeof(int)*(current->width)*(current->height));//memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法

        //  权值矩阵初始化

        for (i = 0;i<4096;i++)

        {

            w_adj_smlr[i] = 0.0;

            hist3[i] = 0.0;

        }




        //==========自适应hist3生成==========

        for (i = t_y_adj_smlr;i < t_h_adj_smlr + t_y_adj_smlr;i++)

        {

            for (j = t_x_adj_smlr;j < t_w_adj_smlr + t_x_adj_smlr;j++)

            {

                //rgb颜色空间量化为16*16*16 bins//int widthStep;

                q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;

                q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;

                q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;

                q_temp[(i - t_y_adj_smlr) *t_w_adj_smlr + j - t_x_adj_smlr] = q_r * 256 + q_g * 16 + q_b;

                hist3[q_temp[(i - t_y_adj_smlr) *t_w_adj_smlr + j - t_x_adj_smlr]] =  hist3[q_temp[(i - t_y_adj_smlr) *t_w_adj_smlr + j - t_x_adj_smlr]] +  m_weiSml[(i - t_y_adj_smlr) * t_w_adj_smlr + j
 - t_x_adj_smlr] ;

            }

        }

//=====================================

        //归一化直方图

        for (i=0;i<4096;i++)

        {

            hist3[i] = hist3[i] / normCofSml;

        }




    //向量权值矩阵生成

            for (i = 0;i < 4096;i++)

            {

                if (hist3[i] != 0)//有这样的色彩

                {

                    w_adj_smlr[i] = sqrt(hist1[i]/hist3[i]);//其实是比较了两个都有的情况。只要有一个没有这种色彩，那么w的值就是零

                }else

                {

                    w_adj_smlr[i] = 0;

                }

            }










        sum_w = 0.0;

        x1 = 0.0;

        x2 = 0.0;




        for (i = 0;i < t_h_adj_smlr; i++)

        {

            for (j = 0;j < t_w_adj_smlr; j++)

            {

                sum_w = sum_w + w_adj_smlr[q_temp[i * t_w_adj_smlr + j]];//q_temp[i * t_w + j]色彩状况的一个表征，具体还没有弄清楚

                x1 = x1 + w_adj_smlr[q_temp[i * t_w_adj_smlr + j]] * (i - t_h_adj_smlr/2);//y direction

                x2 = x2 + w_adj_smlr[q_temp[i * t_w_adj_smlr + j]] * (j - t_w_adj_smlr/2);

            }

        }

        y1_adj_smlr = x1_adj_smlr / sum_w;

        y2_adj_smlr = x2_adj_smlr / sum_w;




        //MeanShitf左上角位置更新

        t_x_adj_smlr += y2_adj_smlr;//y2为x方向偏移量

        t_y_adj_smlr += y1_adj_smlr;//y1为y方向偏移量

        //===========Output the offset value

     //  qDebug( "x axis offset = %f", y2);

      // qDebug( "y axis offset = %f", y1);

    }

 //大窗口============

    while ((pow(y2_adj_bg,2) + pow(y1_adj_bg,2) > 0.5)&& (num_adj_bg < NUM))

    {

        num_adj_bg++;

        memset(q_temp,0,sizeof(int)*current->width*current->height);//memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法

        //  权值矩阵初始化

        for (i = 0;i<4096;i++)

        {

            w_adj_bg[i] = 0.0;

            hist4[i] = 0.0;

        }




        //==========自适应hist4生成==========

        for (i = t_y_adj_bg;i < t_h_adj_bg + t_y_adj_bg;i++)

        {

            for (j = t_x_adj_bg;j < t_w_adj_bg + t_x_adj_bg;j++)

            {

                //rgb颜色空间量化为16*16*16 bins//int widthStep;

                q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;

                q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;

                q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;

                q_temp[(i - t_y_adj_bg) *t_w_adj_bg + j - t_x_adj_bg] = q_r * 256 + q_g * 16 + q_b;

                hist4[q_temp[(i - t_y_adj_bg) *t_w_adj_bg + j - t_x_adj_bg]] =  hist4[q_temp[(i - t_y_adj_bg) *t_w_adj_bg + j - t_x_adj_bg]] +  m_weiBg[(i - t_y_adj_bg) * t_w_adj_bg + j - t_x_adj_bg] ;

            }

        }

//=====================================

        //归一化直方图

        for (i=0;i<4096;i++)

        {

            hist4[i] = hist4[i] / normCofBg;

        }




    //向量权值矩阵生成

            for (i = 0;i < 4096;i++)

            {

                if (hist4[i] != 0)//有这样的色彩

                {

                    w_adj_bg[i] = sqrt(hist1[i]/hist4[i]);//其实是比较了两个都有的情况。只要有一个没有这种色彩，那么w的值就是零

                }else

                {

                    w_adj_bg[i] = 0;

                }

            }




        sum_w = 0.0;

        x1 = 0.0;

        x2 = 0.0;




        for (i = 0;i < t_h_adj_bg; i++)

        {

            for (j = 0;j < t_w_adj_bg; j++)

            {

                sum_w = sum_w + w_adj_bg[q_temp[i * t_w_adj_bg + j]];//q_temp[i * t_w + j]色彩状况的一个表征，具体还没有弄清楚

                x1 = x1 + w_adj_bg[q_temp[i * t_w_adj_bg + j]] * (i - t_h_adj_bg/2);//y direction

                x2 = x2 + w_adj_bg[q_temp[i * t_w_adj_bg + j]] * (j - t_w_adj_bg/2);

            }

        }

        y1_adj_bg = x1_adj_bg / sum_w;

        y2_adj_bg = x2_adj_bg / sum_w;




        //MeanShitf左上角位置更新

        t_x_adj_bg += y2_adj_bg;//y2为x方向偏移量

        t_y_adj_bg += y1_adj_bg;//y1为y方向偏移量

        //===========Output the offset value

     //  qDebug( "x axis offset = %f", y2);

      // qDebug( "y axis offset = %f", y1);

}

    //Bhattacharyya系数比较

//     qDebug( "x axis offset = %d", current->width);

//   qDebug( "y axis offset = %d", current->height);

    for (i=0;i<4096;i++)

    {

        Bhattacharyya1 = Bhattacharyya1 +sqrt(hist1[i]*hist2[i]) ;

        Bhattacharyya2 = Bhattacharyya2 +sqrt(hist1[i]*hist3[i]) ;

        Bhattacharyya3 = Bhattacharyya3 +sqrt(hist1[i]*hist4[i]) ;

    }




    if(Bhattacharyya1 >= Bhattacharyya2 && Bhattacharyya1 >= Bhattacharyya3)//不用调节核函数窗口

    {

       drawing_box.x = t_x;

       drawing_box.y = t_y;

    }

    else

    {

        if( Bhattacharyya2 >= Bhattacharyya1 && Bhattacharyya2 >= Bhattacharyya3 )

        {

            drawing_box.x = t_x_adj_smlr;

            drawing_box.y = t_y_adj_smlr;

            drawing_box.height = t_h_adj_smlr;

            drawing_box.width = t_w_adj_smlr;

            qDebug( "smaller");

        } else

        {

            drawing_box.x = t_x_adj_bg;

            drawing_box.y = t_y_adj_bg;

            drawing_box.height = t_h_adj_bg;

            drawing_box.width = t_w_adj_bg;

            qDebug( "bigger");

        }

    }




    free(m_weiNorm);

    free(m_weiSml);

    free(m_weiBg);







    free(q_temp);

    //显示跟踪结果

   cvRectangle(current,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0),2);

}




