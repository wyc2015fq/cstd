# Eclipse C/C++环境配置 - DoubleLi - 博客园






一、C/C++环境配置：

Window - Preferences - C/C++


1. Editor - Content Assist - Auto-Activation - Delay(ms)，原值为500，改为100，则智能提示速度将非常快！
2. Enviroment，添加三个环境变量:
  INCLUDE=${system:ECLIPSE_HOME}/include
  LIB=${system:ECLIPSE_HOME}/lib
  PATH=${system:ECLIPSE_HOME}/bin;${system:ECLIPSE_HOME}/tool/svn
3. Indexer，清空Files to index up-front的内容，修改Cache limits的值，尽可能大，可以提高速度！
4. Net CDT project wizard - Makefile Project
  Binary Parsers，选中PE Windows Parser
  Error Parsers，只选中CDT Visual C Error Parser
  Discovery Options，取消Automate discovery of paths and symbols，因为VC不支持。
  Builder Settings，取消Use default build command选项，将Build command的值从make改为nmake（注：也可不改，使用make）
5. Code Style，通过New来新建一个代码风格，命名为qpsoft，且初始化为BSD/Allmain，然后按习惯修改一些细节。
6. Editor - Templates，选中Use code formatter，这样快速完成的代码是自己自己设计好的风格。
7. Appearance，选中Group method definitions in the Outline view
二、让工程支持智能提示
由于VC编译器并没有将头文件路径编译进可执行文件，所以无法知道相关头文件的路径，需要手动设置，才能使用智能提示功能。方法：Project - Properties - C/C++ General - Paths and Symbols - Includes，Add，从Variables中选择INCLUDE，选中Add to all configuration、Add to all languages。

三、修改智能提示的快捷键
Window - Preferences - General - Keys，输入Content，找到Content Assist，在Binding中修改为Shift+Space，要顺手的多。

四、其他设置
1. Window - Preferences - General - Editors - Text Editors，选中Show line numbers，设置Print margin column为110
2. Window - Preferences - General - Workspace，取消Build automatically，选中Save automatically before build









