# MeanShift算法C++解析（五） - lyx的专栏 - CSDN博客





2015年11月03日 21:31:29[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：741









最近四旋翼高空坠落几乎完全报废，阻碍了四旋翼飞行平台的进展，于是顺便开始写论文和思考一下Mean shift算法的改进。觉得核函数是一个很值得改进的地方，于是改了下面的代码用来重新分配之前提到的投票的权利。之前和核函数，高斯也好，均匀核函数也好都值考虑了平面距离。突发奇想加入色彩空间的距离或许效果更好，简单一试，发现果然如此，而且这些代码只需要出现在目标初始化函数里面，并不影响算法的时间复杂度。





int qr_mean = 0,qg_mean = 0,qb_mean = 0;


for (i = t_y+t_h/3;i < t_y + (t_h*2)/3; i++)


{


for (j = t_x+t_w/3;j < t_x + (2*t_w)/3; j++)


{


//rgb颜色空间量化为16*16*16 bins


qr_mean += ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;//char *imageData;


qg_mean += ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;


qb_mean += ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;


}


}


    qr_mean=qr_mean*9/(t_w*t_h);

    qg_mean=qg_mean*9/(t_w*t_h);

    qb_mean=qb_mean*9/(t_w*t_h);

    qDebug("q_r=%d",qr_mean);

    qDebug("q_g=%d",qg_mean);

    qDebug("q_b=%d",qb_mean);



    for (i = 0;i < t_h; i++)

    {

        for (j = 0;j < t_w; j++)

        {

            //空间距离

            dist = pow(i - (double)t_h/2,2) + pow(j - (double)t_w/2,2);//d= x^2+y^2

            //色彩距离

            q_r = ((u_char)current->imageData[i * current->widthStep + j * 3 + 2]) / 16;//char *imageData;

            q_g = ((u_char)current->imageData[i * current->widthStep + j * 3 + 1]) / 16;

            q_b = ((u_char)current->imageData[i * current->widthStep + j * 3 + 0]) / 16;




            m_wei[i * t_w + j] = (1 - dist / h)*((double)abs(q_r-qr_mean)/16)*((double)abs(q_g-qg_mean)/16)*((double)abs(q_b-qb_mean)/16);

            //printf("%f\n",m_wei[i * t_w + j]);

            C += m_wei[i * t_w + j] ;//统计权值矩阵的总和？

        }

    }






这个加入的色彩空间距离的体现就是m_wei[i * t_w + j] = (1 - dist / h)*((double)abs(q_r-qr_mean)/16)*((double)abs(q_g-qg_mean)/16)*((double)abs(q_b-qb_mean)/16)这一句权值生成语句。后面分别是三个rgb通道的色彩距离分量。之前想通过图像预处理的方式来加强图像色彩的对比度，让Mean
 shift算法更加可靠一些，但是这个方法其实异曲同工，但是简单很多。

当然啦，这个优化方法的局限性其实很大，就是对色彩凸显程度要求高很多，而且对物体的非刚性更加敏感了。






