# 关于MATLAB界面语言的设置 - 硬曲奇小屋 - CSDN博客





2017年08月25日 16:44:14[HardCookies](https://me.csdn.net/james_616)阅读数：4783








最近在Windows上使用Matlab，界面自动设置成了中文。然而为了使编程的代码显得美观，编辑字体设置成英文字体。但是命令窗口的提示信息的中文无法显示，因此打算把界面语言直接设置成英文。 

在网上查了相关内容，在系统设置中要改环境变量。但是改完后仍不起作用，原来是在2014及之前版本适合这种方法，而我是2016的版本。 

完整的解决方案在MathWorks的社区中有介绍，链接如下： 
[https://cn.mathworks.com/matlabcentral/answers/129813-matlab-8-3-r2014a](https://cn.mathworks.com/matlabcentral/answers/129813-matlab-8-3-r2014a)
> 
在中文操作系统中，MATLAB默认为中文版，请参考以下步骤来显示英文版MATLAB 

  MATLAB R2015a以及新的版本： 

  点击MATLAB菜单栏 > 预设 > 常规 > 选择桌面和错误信息的语言 > 英文 

  保存后重启MATLAB。 

  MATLAB R2014a以及R2014b版本： 

  可以通过设置以下环境变量禁用中文内容： 

  MWLOCALE_TRANSLATED=OFF  

  如果您希望转换回中文版，请设置： 

  MWLOCALE_TRANSLATED=ON 

  Windows 7中环境变量的添加可以右击：我的电脑 > 属性 > 高级系统设置 > 高级 > 环境变量 > 系统环境变量。 

  在Mac系统中，可以按照以下步骤操作： 

  1) 打开实用工具 > 脚本编辑器（AppleScript Editor），输入以下命令： 

  do shell script “launchctl setenv MWLOCALE_TRANSLATED OFF” 

  2) 存储(⌘+s) 为格式为“应用程序(Application) ”的文件 

  3) 打开系统偏好设置（System Preferences）> 用户与群组（Users & Groups） > 登录项（Login Items），在其中将上一步创建的应用程序添加进去 

  4) 重启您的电脑 

  MATLAB R2014a之前的版本： 

  请联系MathWorks技术支持：[http://www.mathworks.com/support/contact_us/](http://www.mathworks.com/support/contact_us/)
希望能帮到有同样问题的人。






