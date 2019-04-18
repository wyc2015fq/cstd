# MATLAB视角下的七夕节 - 算法与数学之美 - CSDN博客
2018年08月16日 20:27:26[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：452
MATLAB是一款商业数学软件，全称为“矩阵实验室”，由美国MathWorks公司出品。主要功能有数值分析、符号计算、数字图像处理、数据可视化等等，应用领域极其广泛，可以说高精尖科研领域基本上都会用到它。
MATLAB使用起来简单上手，很多函数都封装好了，比如要在一系列数据中找最大值、最小值、平均值，MATLAB都有对应的函数可调用，为使用者节约了大量时间。
七夕节马上就到了，作为会一点点MATLAB编程的理工男来说，实不懂浪漫为何物，能送给大家的也就只有这些了，还望笑纳。
用MATLAB从“心”画起，用“心”制作。
> 
> 
ezplot('x^2+(y-(x^2)^(1/3))^2=9')
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msuz09vNaOlCpHudpWfvcDvTqkrd1oEtMiaSNSxMshHsAU0iaJk7Iibf0jVw/640?wx_fmt=jpeg)
额，上面的只是一个二维“心”形线，如果你觉得不满足，请看下面三维立体的“心”。
> 
> 
f=@(x,y,z)(x.^2+ (9./4).*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3 - (9./80).*y.^2.*z.^3;
[x,y,z]=meshgrid(linspace(-1.5,1.5));
val=f(x,y,z);
isosurface(x,y,z,val,0); 
axis equal;view(3);colormap([1 0.2 0.2])
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msuOt7gGUGPsWcPSyDjibzYLuQPmOvCIwN5lgdrVKVL4TvFysWmSQasCVQ/640?wx_fmt=png)
如果你觉得上面的配色太丑，还不满足的话，请看下面多彩缤纷的“心”。
> 
> 
[x,y,z]=meshgrid(linspace(-1.5,1.5)); 
f=@(x,y,z)(x.^2 + (9/4)*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3 - (9/80)*y.^2.*z.^3; 
p=patch(isosurface(x,y,z,f(x,y,z),0)); view(3);axis equal 
set(p,'FaceVertexCData',jet(size(get(p,'faces'),1)) ,'FaceColor', 'flat', 'EdgeColor', 'none');
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msubdwIlibKk7icB8LTappzYI28IvWAJnmGwMhsdCNZDGYUzj4icnVyRo1Yw/640?wx_fmt=png)
如果你觉得上面的“心”都是静态的，不要紧，请看下面这颗跳动着的“心”。
> 
> 
gcp=figure;
M=moviein(100,gcp);
heartSpeed=15;
radiusRate=3; 
for i = 1:100    
    axis([-20 20 -20 20]);
    cla(gcp);
    x=[];
    y=[];    
    for t = 0:0.01:2*pi        r=(sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5)-2*sin(t)+2;
        r = radiusRate * r;
        x=[x,r*cos(t)*(0.5+0.5*HeartRate(i,heartSpeed))];        y=[y,r*sin(t)*(0.5+0.5*HeartRate(i,heartSpeed))];
    end
    hold on;
    fill(x,y,'r');
    M(:,i)=getframe;
end
movie(M,1);
//函数文件
function ratio=HeartRate(i,heartSpeed)
i=mod(i,heartSpeed);
if i < heartSpeed / 2
    ratio=sin( i/heartSpeed*pi);
elseif i >= heartSpeed / 2 && i < heartSpeed * 3/4
    ratio=2*cos(i/heartSpeed*2/3*pi);
else
    ratio = 0;
end
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msuarM9dhfZEjlNyY2V8tbb9df8XrBuzd3WuPVvVKgeDgv1S04F9o5FfA/640?wx_fmt=gif)
如果你觉得上面都是“心”太low了，也不要紧，请看下面这朵玫瑰花。
> 
> 
[x,t] = meshgrid((0:24)/24,(0:.5:575)/575*17*pi-2*pi);p = (pi/2)*exp(-t/(8*pi));u = 1-(1-mod(3.6*t,2*pi)/pi).^4/2;y = 2*(x.^2-x).^2.*sin(p);r = u.*(x.*sin(p)+y.*cos(p));
figure('color','w'),view(-22,66), axis image off
surface(r.*cos(t),r.*sin(t),u.*(x.*cos(p)-y.*sin(p)),'EdgeC','n','FaceC','r')
light('pos',[-.25 -.25 1], 'style','local', 'col',[1 0.84 0.6]),lighting gouraud
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msu7iaVKibxSHRY29ic0CcQicUAJMicYFQQbBayVv8hWfZYwRjKdnRibfXnsoKA/640?wx_fmt=png)
如果你觉得上面都太太太low了，没有点心意，没有点小视频，更不要紧，请看下面这个带有祝福语的小视频吧。
> 
> 
clear;
clc;
%配置函数，建立图形
%f=@(x,y,z)(x.^2+y.^2+z.^2-1);%这是个球体的方程，测试时候用的，现在注释掉了
f=@(x,y,z)(x.^2+ (9./4).*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3.*2 - (9./80).*y.^2.*z.^3;%这个是心形的方程
[x,y,z]=meshgrid(-2:0.04:2); %设置网格密度，中间的数字越小，格越密，形状越逼真，但是太密可能导致内存不足而无法运行.我2GB内存，最高设置到0.03.
 val=f(x,y,z);
 [p,v]=isosurface(x,y,z,val,0);
 %配置显示环境
 figure('Color',[0 0 0],'Position',[200,100,800,500]);
 axes('Color',[0 0 0],'XColor',[1 1 1],'YColor',[1 1 1],'ZColor',[1 1 1],'FontSize',1,'Visible','off');
 patch('faces',p,'vertices',v,'facevertexcdata',jet(size(v,1)),'facecolor','none','edgecolor','interp');
 % 'color' 后面可以是 'r','g','b','c','m','y','w', or 'k'，对应不同的颜色，或者[x,y,z],x,y,z是MATLAB中对颜色的表述，具体自行查资料吧
 %'facecolor''edgecolor' 可以是 'flat' 'none', or 'interp'，亦或者上一条注释
 view(3);grid off;axis equal;
 %配置显示文本
 da=[31069 22825 19979 26377 24773 20154 32456 25104 30519 23646];%标题内容，每个字符都用数值表示，不运行看不出来什么内容，执行 double('字符串') 可以看到“字符串”对应的数值
 db=[102 114 111 109 32 68 101 102 117 32 76 105];%落款内容，原理同上
 msg1=text(0,0.3,2,char(da),'Color','r','FontSize',20,'HorizontalAlignment','center');%显示标题
 text(0,0,-1.5,char(db),'Color','r','FontSize',10,'HorizontalAlignment','left');%显示落款
 text(0,0.3,1.75,'\heartsuit \^_\^ \heartsuit','Color','r','FontSize',20,'HorizontalAlignment','center');
 %'\heartsuit\heartsuit'
h=gca;
axis vis3d
%配置文字闪烁效果/标题交替显示
sl=10;%减速倍数，控制文字闪烁速度
mn=4;%文字闪烁更替出现的颜色数，对应下面的if/elseif次数
%配置滚动字幕
msg2=text(0,0,-1.1,'','Color','y','FontSize',10,'HorizontalAlignment','right');%配置小字号显示属性和位置
msg3=text(0,0,-1.3,'','Color','y','FontSize',14,'HorizontalAlignment','right');%配置大字号显示属性和位置
txtmsg=char([
19971 22805 24555 20048 65281
19971 22805 24555 20048 65281
]);%滚动字幕默认内容，注意，每行字符数必须相同，字数不够的末尾补空格，即32
[msgLength,~]=size(txtmsg);
msgls=100;%减速倍数，控制滚动字幕速度
    for i=0:inf      
        %文字闪烁
        j=mod(fix(i/sl),mn);
        if j==0
            set(msg1,'Color','r');            
        elseif j==1
            set(msg1,'Color','g');            
        elseif j==2
            set(msg1,'Color','y');
        elseif j==3
            set(msg1,'Color','r');
        end     
        %滚动字幕
        msgl=mod(fix(i/msgls),msgLength+2);        
        if msgl>msgLength;
            set(msg2,'String','');
        elseif msgl~=0;
            set(msg2,'String',txtmsg(msgl,:));                
        end
        if msgl<msglength
            set(msg3,'String',txtmsg(msgl+1,:));
        else
            set(msg3,'String','');
        end     
        %自动旋转效果
        view(h,i,10);
        drawnow;
    end </msglength
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/JtvxAibjiaIySQxtx0Cj3kYvAZmO141msukDG4QPfHr2d5QUibvlNFU2XbGTfEFic2aYvLb4548Clmco3F2225TIVg/640?wx_fmt=gif)
祝天下有情人终成眷属。
注：以上源代码可直接复制到MATLAB命令行窗口中运行，保证效果一样（在此感谢互联网上各位作者）。
**文章参考自：**
*1、MATLAB中文论坛，【原创】使用matlab绘制菊花和玫瑰花。*
*http://www.ilovematlab.cn/thread-219886-1-1.html；*
*2、CSDN,matlab心形图大全，几个经典的函数图像，有趣的函数图像，matlab画图。*
*https://blog.csdn.net/robert_chen1988/article/details/53123462；*
# *3、CSDN,原来，数学也可以这么美的-采用matlab画爱心(附带 MatLab 代码)。*
# *https://blog.csdn.net/rs_network/article/details/8048895；*
# *4、知乎，如何用Matlab画一朵花？Falccm答。*
# *https://www.zhihu.com/question/45623721；*
# *5、CSDN,程序员的浪漫：教你如何用MATLAB向女生表白。*
# *https://blog.csdn.net/qq_39521554/article/details/79884849；*
# *6、CSDN,MATLAB旋转心形动画代码表白可用。*
# *https://download.csdn.net/download/windywoody/5485279；*
∑编辑 | 裴奕霖
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
