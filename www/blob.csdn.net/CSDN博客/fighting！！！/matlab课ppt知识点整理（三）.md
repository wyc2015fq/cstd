# matlab课ppt知识点整理（三） - fighting！！！ - CSDN博客
2018年07月13日 22:37:23[dujiahei](https://me.csdn.net/dujiahei)阅读数：113
转自：http://blog.sina.com.cn/s/blog_48707b76010100j1.html
*<9-Psychtoolbox.ppt>*
1、时间控制函数
  T = GetSecs;
 得到从电脑启动到当前的时间长度，以秒为单位。比如我输入(GetSecs)/3600,
得到7.3692，可以知道我电脑今天开了7个多小时了。。
  这个函数的应用在于计算反应时。在呈现刺激的时候，写下time1=GetSecs;
然后在被试按键的时候，写下time2=GetSecs; 那么，Reaction Time = time2-time1;
  WaitSecs(duration);
  就是等待一段时间了。比如你需要把实验刺激呈现300ms, 那么在呈现的Flip语句
之后，写一句：WaitSecs(0.3)；即可。注意单位是秒。
  tic  %开启计时
  whiletoc<2  %2秒之内一直循环，2秒之后跳出循环
  ;
 end   
 %以上计时器的作用和WaitSecs(2)一模一样。 
2、Input函数
  比如在实验开始之前，你希望能够输入被试的姓名，从而在实验中或者结果记录的
时候用到。那么就需要Input函数。
  subName = input('What is yourname?','s');
  这句话运行时，命令窗口就会出现 “What is yourname?”，然后需要输入、回车。
后面的‘s’表示把subName会把输入记录为字符串类型。
  需要注意，由于需要在命令窗口输入，所以必须在实验屏幕打开前进行。打开后就
看不到命令窗口了。
3、实验中记录按键反应
%=================
KbName('UnifyKeyNames');   %定义按键前最好都加上这一句
escapeKey = KbName('escape');
key_left = KbName('z');
key_right =KbName('m');    %这里定义了三个键，分别是键盘上的esc\z\m
   %…省略其他各种操作
reaction = 0;
while (reaction==0)  %进入循环，知道按了三个键中的某一个，才会跳出
    [KeyIsDown,secs, KeyCode] = KbCheck;  %这是关键语句。检查是否有按键、按了啥键
   reaction=0;
    ifKeyCode(key_left)   %key_left是z键，按了的话reaction被赋值-1
       reaction=-1;
    elseifKeyCode(key_right) %如果按了m键，就记录reaction = 1;
       reaction=1;
    elseifKeyCode(escapeKey)  %如果按了esc键，这儿就关闭屏幕退出程序了。
       reaction=3;
       Screen('CloseAll');
       break;
    end
end
%=====================================
  上面的方法是要一直等到被试按键才罢休；一般不会这么做，比如只等待两秒，
如果还没按键就继续。那么可以用GetSecs才实现。
 最后可以从reaction的值是-1还是1，来记录被试是按了z还是m。从而记录结果。
