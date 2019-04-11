# OpenCV学习笔记（三十六）——Kalman滤波做运动目标跟踪

2011年12月08日 09:26:56

yang_xian521

阅读数：42999

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

kalman滤波大家都很熟悉，其基本思想就是先不考虑输入信号和观测噪声的影响，得到状态变量和输出信号的估计值，再用输出信号的估计误差加权后校正状态变量的估计值，使状态变量估计误差的均方差最小。具体它的原理和实现，我想也不用我在这里费口舌，但这个理论基础必须的有，必须得知道想用kalman滤波做跟踪，必须得先建立运动模型和观察模型，不是想用就能用的。如果不能建立运动模型，也就意味着你所要面对的问题不能用kalman滤波解决。

我结合一下OpenCV自带的kalman.cpp这个例程来介绍一下如何在OpenCV中使用kalman滤波吧，OpenCV已经把Kalman滤波封装到一个类KalmanFilter中了。使用起来非常方便，但那繁多的各种矩阵还是容易让人摸不着头脑。这里要知道的一点是，想要用kalman滤波，要知道前一时刻的状态估计值x，当前的观测值y，还得建立状态方程和量测方程。有了这些就可以运用kalman滤波了。

OpenCV自带了例程里面是对一个1维点的运动跟踪，虽然这个点是在2维平面中运动，但由于它是在一个圆弧上运动，只有一个自由度，角度，所以还是1维的。还是一个匀速运动，建立匀速运动模型，设定状态变量x  = [ x1, x2 ] = [ 角度，角速度 ]，则运动模型为

x1（k+1） = x1（k）+x2（k）*T

x2（k+1）= x2（k）

则状态转移方程为

x* = Ax + w

这里设计的噪声是高斯随机噪声，则量测方程为：

z = Cx + v

看了代码，对应上以上各项：

状态估计值x --> state

当前观测值z --> measurement

KalmanFilter类内成员变量transitionMatrix就是状态转移方程中的矩阵A

KalmanFilter类内成员变量measurementMatrix就是量测方程中矩阵C



```cpp
    Mat statePre;           //!< predicted state (x'(k)): x(k)=A*x(k-1)+B*u(k)
    Mat statePost;          //!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
    Mat transitionMatrix;   //!< state transition matrix (A)
    Mat controlMatrix;      //!< control matrix (B) (not used if there is no control)
    Mat measurementMatrix;  //!< measurement matrix (H)
    Mat processNoiseCov;    //!< process noise covariance matrix (Q)
    Mat measurementNoiseCov;//!< measurement noise covariance matrix (R)
    Mat errorCovPre;        //!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/
    Mat gain;               //!< Kalman gain matrix (K(k)): K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)
    Mat errorCovPost;       //!< posteriori error estimate covariance matrix (P(k)): P(k)=(I-K(k)*H)*P'(k)
```

 我想就不用我再翻译了吧。相信有了以上的注释，大家都能找到它们的对应项。 

使用的时候，除了初始化我刚刚初始化过的transitionMatrix和measurementMatrix外，还需要初始化processNoiseCov，measurementNoiseCov和errorCovPost。

把它们初始化好之后，接下来的动作就很简单了，分两步走，第一步调用成员函数predict得到当前状态变量的估计值，第二步调用成员函数correct用观测值校正状态变量。再更新状态变量做下一次估计。听着好简单啊，代码就不上传坑爹了，在opencv2.3.1\samples\cpp\kalman.cpp中其义自见。