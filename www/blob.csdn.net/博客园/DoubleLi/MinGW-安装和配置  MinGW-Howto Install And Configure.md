# MinGW - 安装和配置 / MinGW - Howto Install And Configure - DoubleLi - 博客园






MinGW在线安装程序下载地址：http://sourceforge.net/projects/mingw/files/Automated%20MinGW%20Installer/mingw-get-inst/mingw-get-inst-20101030/mingw-get-inst-20101030.exe/download



## 一、运行环境 / Environment


Windows 2003 32bit / Windows Xp 32bit / Windows 7



## 二、MinGW安装 / MinGW - Howto Install


1、双击 mingw-get-inst-20101030.exe 运行，出现如下图所示界面。
     点击 “Next >” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/-AD0ma1Nl2vmSnhC2EAVow==/894246001011581770.png)


2、提示将在所有用户的开始菜单或桌面上建立快捷方式图标，如果只是安装给当前用户使用，点击“Cancel”按钮退出安装，以非Administrator权限重新运行安装程序。
     点击 “Next >” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/KFtuqm4iwSb5peTJY2vr0w==/894246001011581794.png)


3、提示安装选择：
      Use pre-packaged resossitory catalogues 表示安装20101030这个版本。
      Download lastest repository catalogues 表示安装当前最新版本。

      点击 “Next >” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/zIDMVPPeMp38xSnFsQyTSg==/894246001011581817.png)


4、展示软件协议。选择 I accept the agreement，并点击 “Next >” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/rYWIEoINyeCQgFn7mUbyWQ==/894246001011581848.png)


5、提示选择安装目录。默认为C:\MinGW，当然可以选择其他目录。点击 “Next >” 按钮。
建议：目录名称不要包含空格。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/zhqOC3dnbD8nS5sa3SFtsg==/894246001011581853.png)


6、提示会在程序菜单下建立程序的开始菜单目录，默认为 MinGW。
      如果不需要建立开始菜单目录，可以选择 Don't create a Start Menu folder。

      点击 “Next >” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/Z0gq9XexptOEJIjRsW8PJQ==/894246001011581871.png)


7、提示选择安装的组件。
      建议安装所有组件，一般情况下只要选择 C Compiler、C++ Compiler、MSYS Basic System 就够了。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/_-2-SCYB7x7VqlGbsexztQ==/894246001011581888.png)


8、展示确认信息。告诉你选择了安装哪些组件以及安装在哪个目录下。
     点击 “Install” 按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img460.ph.126.net/k1ZfcTfgrTqFxQo8ZatoQA==/29273397578997284.png)


系统会另外弹出一个DOS窗口，开始下载选择安装的组件。 

9、安装完成后，目录结果类似如下图所示：

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img844.ph.126.net/bvptrZQCuJtIvVfdwF9JGQ==/894246001011581752.png)





## 三、MinGW配置 / MinGW - Howto Configure


1、右击“我的电脑”图标，在弹出的菜单上选择“属性(R)”菜单项。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/Awn6rcHg-P6l6WPDWRSkgg==/2756202971952181333.png)

2、选择“高级”选项卡。点击“环境变量”按钮。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/dx88809P3ug9T8cjCXL9Fw==/2756202971952181345.png)


3、点击“新建(W)”按钮，新建环境变量：MINGW_HOME，变量值为MinGW的安装目录。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/rGjrrYeMDjpOF0Fw4GHPVg==/2756202971952181349.png)


4、选择环境变量Path，点击“编辑(I)”按钮。在尾部加入“%MINGW_HOME%\bin;”。点击“确定"按钮保存。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/JE_xVbKNeIYjMqQo3M6row==/2756202971952181367.png)


5、点击开始菜单上的”运行“，输入 cmd 打开命令行窗口。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/jciSZL0H-eOcDH-HdYzsXw==/2756202971952181371.png)


6、输入gcc -v 回车，再输入g++ -v 回车，如果出现窗口的提示，说明MinGW的安装和环境变量配置成功。

![MinGW - 安装与配置 / MinGW  - Howto Install And Configuration - 傲风 - 宝剑锋从磨砺出 梅花香自苦寒来](http://img854.ph.126.net/nIpIpaSQ4BlYxURn6MT-0Q==/2756202971952181312.png)



## 四、编译和运行C HelloWorld程序 / Compile and Run C HelloWorld


1、**在C盘下新建一个helloworld.c文件，源代码如下**：

#include <stdio.h>

main() {
    printf ("Hello World!\n");

    return 0;
}

**2、编译。**
C:\>gcc -o helloworld.exe helloworld.c

**3、运行。**
C:\>helloworld.exe
Hello World!This is my first c program!


## 五、编译和运行C++ HelloWorld程序 / Compile and Run C++ HelloWorld


1、**在C盘下新建一个helloworld.cpp文件，源代码如下**：

#include <iostream>

main() {
    std::cout << "Hello world!This is my first c++ program!" << std::endl;

    return 0;
}

**2、编译。**
C:\>g++ -o helloworld.exe helloworld.cpp

**3、运行。**
C:\>helloworld.exe
Hello world!This is my first c++ program!









