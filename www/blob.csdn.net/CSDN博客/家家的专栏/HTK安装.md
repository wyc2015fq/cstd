# HTK安装 - 家家的专栏 - CSDN博客





2014年03月21日 10:39:23[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：788








安装HTK，大致流程如下：

1、下载HTK，地址：[http://htk.eng.cam.ac.uk/](http://htk.eng.cam.ac.uk/) 注册免费得密码下载

2、保证电脑上安装了VC，我安装的是6.0版本

3、开始——运行，输入CMD进入DOS界面

4、运用DOS命令CD将目录切换到HTK解压目录内

5、为tools和lib建立文件夹： 输入  mkdir bin.win32

6、运行VCVARS32：  输入 VCVARS32

7、build HTK library： CD HTKLib

                       nmake/f htk_htklib_nf.mkf all

                       cd..

8、build HTK Tools： CD HTKTools

                     nmake/f htk_htktools_nf.mkf all

                     cd..

                     CDHLMLib

                     nmake/f htk_hlmlib_nf.mkf all

                     cd..

                     CDHLMTools

                     nmake/f htk_hlmtools_nf.mkf all

                     cd..

这样，HTK工具就出现在bin.win32文件夹，最后环境变量path中添加路径.




转自：http://blog.sina.com.cn/s/blog_61c485330100iki0.html



