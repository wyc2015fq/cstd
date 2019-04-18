# Caffe: Vs13添加CUDA支持 - wishchinYang的专栏 - CSDN博客
2016年07月11日 18:43:08[wishchin](https://me.csdn.net/wishchin)阅读数：760
1.  右键工程
点击：Building Dependency
右击：Build Customizations
点击选项：CUDA 7.5 
2.添加C++依赖：
cudart.lib
kernel32.lib
user32.lib
gdi32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib
3. 对每个.cu文件,点击右键；替换编译器为：CUDA C/C++
 即 Item Type ：CUDA C/C++
4. 编译...
