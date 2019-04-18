# Matlab高级绘图功能 - 算法与数学之美 - CSDN博客
2018年05月12日 08:16:15[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：1433
> 
温馨提示福利在最下方
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvEQs6EOouMOCnHLsHncfVicTmunOde3d11MmnaA2hxbew2aiakKH9a9CiarZzetFEtaWdSdQia2m2c4FQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
> 
d=[-1 1]; 
[x,y,z]=meshgrid(d,d,d);%定义一个立方体
x=[x(:);0];
y=[y(:);0]; 
z=[z(:);0];%[x,y,z]分别为加上中心的立方体顶点
X=[x(:) y(:) z(:)];
Tes=delaunayn(X);%返回 m×n 的数组值
tetramesh(Tes,X);%绘制四面体图
camorbit(20,0);%旋转摄像目标位置
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvEQs6EOouMOCnHLsHncfVicTLwjFUcd0x9DydkXXbF9GXaVcUCLNaXZO4aHylCxaBPUGNiax0cgvowg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
> 
load wind%打开保存的数据
lims=[100.64 116.67 17.25 28.75 -0.02 6.86];%定义坐标轴范围
[x,y,z,u,v,w]=subvolume(x,y,z,u,v,w,lims);%lims 来定义数据子集
[sx sy sz]=meshgrid(110,20:5:30,1:5);%定义网格点verts=stream3(x,y,z,u,v,w,sx,sy,sz,.5);%计算彩带顶点
cav=curl(x,y,z,u,v,w);%计算卷曲角速度
wind_speed=sqrt(u.^2+v.^2+w.^2);%计算流速h=streamribbon(verts,x,y,z,cav,wind_speed,2);%绘制流彩带图
view(3)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvEQs6EOouMOCnHLsHncfVicTdxSeX4MBKfUnJasysMU6wdPUL7xOCugicDZCWR4OuMB977rcXayCTQQ/640?wx_fmt=png)
> 
n=6%定义轮数
r=(0:n)'/n;%定义轮的半径
theta=pi*(-n:n)/n;%定义轮的扇区角
X=r*cos(theta); 
Y=r*sin(theta);%定义网格顶点
C=r*cos(2*theta);%定义色图
pcolor(X,Y,C)%绘制伪彩图 axis equal tight
**更多高级功能等你来解锁**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/uJcKyGGBIvEcIAcib5306USRvUH17CgUytN0dLuicGI8xSmjW3Myx9ZsyPbxn5PSHFBq3VCexRHpQ85tfFXRP4dQ/640?wx_fmt=jpeg)
**讲师介绍**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/uJcKyGGBIvFALGIl7ZMnnGia1Aj11KWiaU7braArjSKhQPVG55ibSuIMdfF1u5XNSy41EJCMGRibFRzlKaX97IGjpg/640?wx_fmt=jpeg)
**董辰辉**
**Matlab 畅销书主编、上市公司高级算法工程师。**使用 MATLAB 超过** 16 **年，精通各种算法及 MATLAB算法工具箱，出版有《MATLAB从入门到精通》、《MATLAB2008全程指南》、《MATLAB/Simulink通信系统建模与仿真实例精讲》等教程。2009年研究生毕业从事算法工程师工作至今，主要工作内容为数学建模、优化算法、预测算法等。在算法工作方面积累了非常丰富的经验。
**课程特色**
**1、**以全新的 Matlab2018a 为讲课软件；
**2、**涵盖 10大常用算法 +绘图+图像处理；
**3、**10套 原创作业习题+作业讲解
**4、**超 30学时 课程+ 6个月 超长答疑服务
**5、**累计超过1500多名学员，学员的好评，值得信赖
**课程目录**
**第一章：MATLAB基础技能**
第一节
                 MATLAB入门
                 数据类型
                 矩阵和数组技巧
第二节
                 可视化及其控制
                 新版本绘图的功能
                 绘图控件如何设置属性
第三节
                 图形对象
第四节
                 流程控制
                 函数的类型
                 串演算函数
第五节
                 函数变量
                 错误和异常的处理
                 matlab计算效率提升
                 调试模式，断点设置，如何查找bug修改bug
                 MATLAB编程规范
**第二章：Matlab常用算法及实践**
第六节
                 数据拟合、概率统计、随机数的产生、灵敏性检验
                 数据文件io、大数据处理
第七节     
                 优化工具箱介绍
                 模拟退火算法
第八节     
                 遗传算法实践
                 蒙特卡罗算法
第九节     
                 蚁群算法
                 tsp问题演示
                 时间序列
第十节     
                 神经网络
                 SVM算法 
第十一节     
                 图论
                 图像处理
                 经济与金融
第十二节
                 AppDesigner
**开课时间**
**1、**全程直播课程 5月12日 至 6月16日  连续 6 周（每周六、周日晚19-21点）；
**2、**直播过后有视频回放。
**限时优惠**
**1、**添加极值学院助教微信（jizhi-xingchen），发送“ **Matlab **”，现在课程已经超过100人，需要参加拼团享受** 150元**优惠（仅剩 40 个课报名名额，先到先得！）；
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/uJcKyGGBIvHflaGck6705pYXceGu5jOyfghleqEbz9hhkEekleVdmq7ZtW1DBdrKONAHnVmlrR0SsA9lXl93Ig/640?wx_fmt=jpeg)
极值学院助教微信
**2、**购课即可获赠 **100个G的 MATLAB 资料大礼包**。资料预览图如下：
精心整理的安装包和教程包▼
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvFyXib5jJSuHVpH46riborRRFgN3oicqAQaz6GZPGicUmuBuiblTpeI37L3ntGnQiaib4D4zWFPTnU9J9hHw/640?wx_fmt=png)
各种教程资料▼
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvFyXib5jJSuHVpH46riborRRFC5sCibYKk5bwI500vG15Exw8xMJiaRwtApbEFcULlRk0wPEy37ictZzow/640?wx_fmt=png)
**往期精彩回顾**
「Matlab从入门到算法实践」系列已经进行了四期。课程受到学员们的一直好评，董老师独特的授课方式和周到的答疑服务，让每个同学都受益匪浅，课程质量领先市场上同类课程，课后答疑服务周到全面，学员的问题全部能得到解决，而且绝大部分疑问在 1 小时之内就得到了老师的答复。
**1、部分课件预览：**
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvHxyRYAibAm7dJ7Ezia0kUytwodpBbIYhKg2n6mGpqc6qyRLQnd0bXUVibkxib12rzNcEy4A29ibkzSib7A/640?wx_fmt=png)
SVM算法
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvHxyRYAibAm7dJ7Ezia0kUytwPDBiaWfrWx0tgZf6PBdZ7GmaQbmoxcXX5UdicQeZ1b79SGBa7SrTfKbQ/640?wx_fmt=png)
神经网络算法
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvHxyRYAibAm7dJ7Ezia0kUytwHkcLjnBb5G5ScFvhXBiaexoG4HecTgLwYSTmSskxCBTXAD8INzKLEsw/640?wx_fmt=png)
蚁群算法
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvFBeSHrus4VsDkS18NhUMZVqLkAfFsg7O2HibqHnehdKx9cupaSHlzpH4XjwwHM3f0icW3xfBCuxs7g/640?wx_fmt=png)
最短路径问题
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvFBeSHrus4VsDkS18NhUMZVtxFH8ibcdehH3GH16ja6DJKwl4j4Pta6gmbPr3n6GFCkhUV13J6XrOg/640?wx_fmt=png)
图像处理
**2、课程配套作业预览：**
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvEBLFNVoJNp3kIBGvzWmv4I7fticxicaGEQic7SpYej3Z9xj1NSQBibRmiaZiaIpYAfzNaDjLUUl1IJvraQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/uJcKyGGBIvEBLFNVoJNp3kIBGvzWmv4IRyyiasUy2ia3RgicF65HFyL2j4JjKm5wfawQ1x7e0WdQVldHBYdp5ZCaw/640?wx_fmt=png)
**看到这里的都有福利**
**扫描下方二维码，发送“matlab”**
**免费领取电子资料**
▼
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/uJcKyGGBIvEQs6EOouMOCnHLsHncfVicTqlK945qk96iccJD1BstMSNvad9rmDFIdLanCryAHQNIuh3Lx0d25aNA/640?wx_fmt=jpeg)
