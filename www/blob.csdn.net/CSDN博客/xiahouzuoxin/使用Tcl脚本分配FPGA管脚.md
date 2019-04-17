# 使用Tcl脚本分配FPGA管脚 - xiahouzuoxin - CSDN博客





2014年06月13日 10:33:28[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：5901








## 自动生成Tcl文件

```
Project -> Generate Tcl File for Project...
```


弹出如下对话框，设置脚本路径。

![](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A/Create-Tcl.png)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A.md#%E7%BC%96%E8%BE%91%E5%BC%95%E8%84%9A)编辑引脚


使用`set_location_assignment`分配管脚如下：

![](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A/Tcl.png)


第一次配制时，没有`set_location_assignment`语句，自已在`set_global_assignment`语句下一行添加即可。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A.md#%E6%89%A7%E8%A1%8Ctcl%E8%84%9A%E6%9C%AC)执行Tcl脚本

```
Tools -> Tcl Scripts...
```

![](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A/Tcl-Run.jpg)


选中刚新建的Tcl文件，点击Run运行即可！

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A.md#%E7%AD%94%E7%96%91%E8%A7%A3%E6%83%91)答疑解惑


1. 为什么Tcl中没有为每个引脚分配输入输出属性，也能保证引脚属性正确？


因为引脚输入输出属性都已经在Verilog语言的module中或原理图中设置了，如下图，

![Pin2](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A/Pin2.png)![Pin1](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Tcl%E8%84%9A%E6%9C%AC%E5%88%86%E9%85%8DFPGA%E7%AE%A1%E8%84%9A/Pin1.png)



