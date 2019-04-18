# Matlab安装MinGW-w64实现c，c++以及fortran编译（亲测有效） - 心纯净，行致远 - CSDN博客





2018年03月08日 15:05:01[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：9116








    matlab固然强大，其中的simulink更是理工科仿真、建模研究中不可缺少的工具，一般而言matlab/simulink能满足多数使用要求，但是往往在研究中会使用User-difinition s-function或者调用c,c++的库函数，这时候就需要matlab能进行c，c++以及fortran语言的编译。

    但是matlab本身是没有包含上述语言的编译器，因此通常需要安装MinGW，至于是w64还是w32要根据具体系统。出于项目要求，我也需要安装这个编译器，前后折腾了大概一天的时间，才终于安装成功。

    首先在命令行窗口运行
`mex -setup`
如果matlab缺少编译器的话，命令行窗口会提示缺少SDK,，如下：

```
错误使用 mex

未找到支持的编译器或 SDK。有关选项，请访问  http://www.mathworks.com/support/compilers/R2014a/win64。
```

    本文就是解决了这个问题，希望可以帮助更多的人少犯错误，为下一次解决这样的问题节约时间。

    网上安装的方法主要有两种，下面是本人亲测有效的其中一种方法：

1. 安装2015b以上的matlab版本，可以不卸载之前的matlab版本，我的电脑好像两个并不冲突。

2.下载TDM-GCC（注意看清是32位还是64位），TDM-GCC的安装路径不要包括空格； 
[http://tdm-gcc.tdragon.net/download](http://tdm-gcc.tdragon.net/download)


3. 新建环境变量MW_MINGW64_LOC，设置为TDM-GCC-64的安装位置。具体步骤为：


- 
确保您有管理权限。

- 
从“开始”菜单选择**计算机**。

- 
从上下文菜单中选择**系统属性**。

- 
点击**高级系统设置** > **高级**选项卡。

- 
点击**环境变量**。

- 
在**系统变量**下面，选择**新建**。

- 
在**新建系统变量**对话框中的**变量名称**字段中键入 `MW_MINGW64_LOC`。

- 
在**变量值**字段中，键入 MinGW-w64 编译器的安装位置，例如 `C:\TDM-GCC-64`。 

- 
点击**确定**关闭对话框，然后关闭**控制面板**对话框


4. 在MATLAB命令行内运行命令setenv(‘MW_MINGW64_LOC’,folder)，folder为TDM-GCC的安装位置，要加单引号； 


5. 完成安装。

安装成功，命令行会提示：

```
>> mex -setup
MEX 配置为使用 'MinGW64 Compiler (C)' 以进行 C 语言编译。
警告: MATLAB C 和 Fortran API 已更改，现可支持
	包含 2^32-1 个以上元素的 MATLAB 变量。不久以后，
	 您需要更新代码以利用
	 新的 API。您可以在以下网址找到相关详细信息:
	 http://www.mathworks.com/help/matlab/matlab_external/upgrading-mex-files-to-use-64-bit-api.html。

要选择不同的语言，请从以下选项中选择一种命令:
 mex -setup C++ 
 mex -setup FORTRAN
```




参考资料：MATLAB官方文档
[http://cn.mathworks.com/help/matlab/matlab_external/compiling-c-mex-files-with-mingw.html?requestedDomain=uk.mathworks.com](http://cn.mathworks.com/help/matlab/matlab_external/compiling-c-mex-files-with-mingw.html?requestedDomain=uk.mathworks.com)
**果然，详细的官方说明文档才是学软件的最好资料。**




