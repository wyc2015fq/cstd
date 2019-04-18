# 二进制化(二)Framework - weixin_33985507的博客 - CSDN博客
2018年05月12日 23:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
# 一、Framework说明
- 1、Framework也是代码二进制的一种方案，Framework与静态库的区别可以移步到[二进制化(一)静态库](https://www.jianshu.com/p/50f73cd706ee)
- 2、Framework可以是静态库也可以是动态库
# 二、Framework制作
- 
1、创建Framework
![2338406-c183eaf806d271f5.png](https://upload-images.jianshu.io/upload_images/2338406-c183eaf806d271f5.png)
创建Framework.png
- 
2、编写代码并且编译生成Framework，其中RevanFramework是可执行文件RevanFramework.h是暴露出去的头文件，但是并没有发现我们创建的RevanFrameWorkLog.h，这样的话是无法使用RevanFrameWorkLog中的方法的，所以还需要设置使得RevanFrameWorkLog.h暴露出来
![2338406-068563f4da3f45ff.png](https://upload-images.jianshu.io/upload_images/2338406-068563f4da3f45ff.png)
编译生成的Framework.png
- 
设置自定义的文件暴露头文件给外界调用
![2338406-1f58044bf8f958cd.png](https://upload-images.jianshu.io/upload_images/2338406-1f58044bf8f958cd.png)
暴露头文件.png
- 
3、查看当前Framework的架构是x86_64
![2338406-bd00e0b2e995c292.png](https://upload-images.jianshu.io/upload_images/2338406-bd00e0b2e995c292.png)
模拟器iPhone8编译出来的Framework支持架构.png
- 
4、通过测试这个RevanFramework，运行会崩溃。这是因为创建出来的Framework是一个动态库，如果想使用动态库需要在工程中进行设置
![2338406-5fba1ddc59e0138d.png](https://upload-images.jianshu.io/upload_images/2338406-5fba1ddc59e0138d.png)
创建出来的Framework是动态库.png
- 
工程中设置加载动态库Framework
![2338406-28ba1a393c23ca2c.png](https://upload-images.jianshu.io/upload_images/2338406-28ba1a393c23ca2c.png)
工程中设置加载动态库.png
- 
5、查看Framework是否为动态库
![2338406-b4548faccddbe5a5.png](https://upload-images.jianshu.io/upload_images/2338406-b4548faccddbe5a5.png)
通过file命令来查看Framework是否为动态库.png
- 
6、设置Framework为静态库
![2338406-61121a676036a6f0.png](https://upload-images.jianshu.io/upload_images/2338406-61121a676036a6f0.png)
设置Framework为静态库.png
- 
7、当使用iPhone5模拟器编译时，项目报RevanFrameworkLog找不到i386
![2338406-6535cf9048a468dc.png](https://upload-images.jianshu.io/upload_images/2338406-6535cf9048a468dc.png)
缺少i386架构.png
- 
8、设置支持所有模拟器中的架构，首先要确定指针的Framework支持的最低版本，当最低版本为v11.0的时候，编译只会生成x86_64；如果最低版本为v8.0时，编译会生成x86_64和i386
![2338406-acaece3029e6f706.png](https://upload-images.jianshu.io/upload_images/2338406-acaece3029e6f706.png)
支持模拟器所有架构.png
# 三、制作真机Framework
### 制作真机的Framework和制作模拟器Framework步骤是一样的，只是在编译的时候不能选择模拟器而是选择Generic iOS Device 或者是真机
![2338406-320330f1557f2e39.png](https://upload-images.jianshu.io/upload_images/2338406-320330f1557f2e39.png)
制作真机Framework.png
# 四、制作release环境下的 Framework
### 制作过程和上面是一样的，只需要把编译环境配置成release
![2338406-a01166e3e901f9d0.png](https://upload-images.jianshu.io/upload_images/2338406-a01166e3e901f9d0.png)
release环境下的Framework.png
# 五、小结：不同机型的CPU，对应的架构不同
- 
模拟器
- 4s - 5：i386
- 5s - iPhone X：x86_64
- 
真机
- 3gs - 4s：armv7
- 5 / 5c：armv7s (armv7兼容armv7s)
- 5s - iPhone X：arm64
