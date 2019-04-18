# Caffe RPN ：error C2220: warning treated as error - no 'object' file generated - wishchinYang的专栏 - CSDN博客
2018年03月08日 11:45:38[wishchin](https://me.csdn.net/wishchin)阅读数：613
       在 caffe里面添加rpn_layer.cpp之后，总是出现 error C2220: warning treated as error - no 'object' file generated 这种错误。
      后面跟着： warning 4819  存在不支持的非Unicode字符集
       寻找几种方法：
       删除掉所有无效字符；
       找到紧跟error C2220的第一个warning，本例是C4047，那么在驱动源文件的第一行，加入如下命令：
       #pragma warning(disable: 4047)  没卵用！
       在编译选项里面 把警告 降低到level3；
       在 general 选项里面 关掉wx开关；
      然并卵！
修改方法：
      用记事本打开 cpp文件，保存为Unicode格式。再次编译，通过。
