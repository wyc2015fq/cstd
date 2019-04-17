# MinGW安装与环境变量配置和Sublime Text 2搭建C++编译环境 - DoubleLi - 博客园






## MinGW安装与环境变量配置

从http://sourceforge.net/projects/mingw/下载MinGW，安装到D:\MinGW。工具集选择安装(之后还可以进行安装卸载):至少需要安装mingw32-base, mingw32-gcc-g++, msys-base工具集。



安装完成后需要配置环境变量：

右击我的电脑，点属性->高级->环境变量。然后在用户变量一栏：
  1、在PATH里加入D:\MinGW\bin，如果里面还有其他的变量，记得要加个分号，分号得在英文输入模式下输入的。若没有PATH，则新建PATH变量。
  2、新建LIBRARY_PATH变量，如果有的话，在值中加入D:\MinGW\lib，这是标准库的位置。
  3、新建C_INCLUDE_PATH变量，值设为D:\MinGW\include。

环境变量已经配置完毕，我们打开一个CMD窗口，进行验证，看我们的环境变量有没有配置成功。在cmd下输入gcc -v



若能显示出gcc版本，说明MinGW安装配置成功。

# Sublime Text 2搭建C++编译环境

打开Sublime Text 2，选择工具--》编译系统--》新编译系统，然后在里面输入下面的代码：




**[plain]**[view plain](https://blog.csdn.net/lengbuleng1107/article/details/46814907#)[copy](https://blog.csdn.net/lengbuleng1107/article/details/46814907#)



- {  
-     "cmd": ["g++", "${file}", "-o", "${file_path}/${file_base_name}"],  
-     "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",  
-     "working_dir": "${file_path}",  
-     "selector": "source.c, source.c++",  
- 
-     "variants":  
-     [  
-         {  
-             "name": "Run",  
-             "shell": true,   
-             "cmd" : ["start", "cmd", "/k", "${file_path}/${file_base_name} &&echo. & pause && exit"]    
-         }  
-     ]  
- }  






然后保存为：C++builder.sublime-build，注意后缀一定为sublime-build。如果不想新建编译系统，也可以直接在保存的目录下直接修改C++.sublime-build这个文件，同样改成上面的代码。



## 测试

在Sublime新建一个a.cpp，写入代码：




**[cpp]**[view plain](https://blog.csdn.net/lengbuleng1107/article/details/46814907#)[copy](https://blog.csdn.net/lengbuleng1107/article/details/46814907#)



- #include<iostream>  
- #include<string>  
- using namespace std;  
- 
- int main(){  
-     string b="hello world!";  
-     cout<<b<<endl;  
- return 0;  
- }  


选择工具--》编译系统--》选择刚才新建的编译系统，按Ctrl+B编译程序生成a.exe；然后Ctrl+Shift+B运行程序结果如下：


hello world！

请按任意键继续...









