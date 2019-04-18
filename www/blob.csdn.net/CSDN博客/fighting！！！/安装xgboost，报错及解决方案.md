# 安装xgboost，报错及解决方案 - fighting！！！ - CSDN博客
2019年01月08日 19:58:33[dujiahei](https://me.csdn.net/dujiahei)阅读数：238
看了一些教程，直接在anaconda prompt 中输入“”，结果报错：
UnsatisfiableError: The following specifications were found to be in conflict:
  - numpy-base
  - py-xgboost -> numpy==1.12.1 -> mkl=2017.0
Use "conda info <package>" to see the dependencies for each package.
于是，又转入pycharm 终端安装，输入“pip install C:\ProgramData\Anaconda3\mywhl\xgboost-0.81-cp36-cp36m-win_amd64.whl
”, 然后又报错:
![](https://img-blog.csdnimg.cn/20190108195048198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
于是，我冷静下来，决定把prompt 中提示的 numpy-base 卸载，然后安装xgboost.
![](https://img-blog.csdnimg.cn/2019010819530799.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
显示安装完毕，在pycharm中查看
![](https://img-blog.csdnimg.cn/20190108195554547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
不过，又出错了。。。。。。
添加了动态链接库以后，又报错
 AttributeError: function 'XGBRegisterLogCallback' not found。
这次怎么百度都找不到答案。。。。于是卸载了xgboost，打算重新安装，卸载的时候发现它安装在
![](https://img-blog.csdnimg.cn/20190108210300681.png)
但是我调用的python解释器不在这里，所以，可能是这里带来的问题吗？
再次安装的时候，决定不勾选那个选项
![](https://img-blog.csdnimg.cn/20190108210451303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
然后点击安装，发现这次安装在了Anaconda下
![](https://img-blog.csdnimg.cn/20190108210538350.png)
然后，发现可以调用了！！！
后来，发现在stackOverflow 上，有人对这个问题进行过提问并得到了解答，所以，以后要多看英文论坛啊！
[https://stackoverflow.com/questions/40005093/attributeerror-module-xgboost-has-no-attribute-xgbregressor](https://stackoverflow.com/questions/40005093/attributeerror-module-xgboost-has-no-attribute-xgbregressor)
![](https://img-blog.csdnimg.cn/20190108210723572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
