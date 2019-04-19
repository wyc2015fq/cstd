# Android获取ROOT权限方法汇总 - xqhrs232的专栏 - CSDN博客
2012年01月17日 10:06:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1134标签：[android																[exception																[破解																[system																[null																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=破解&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.metsky.com/archives/532.html](http://www.metsky.com/archives/532.html)
本文介绍Android系统上获取TF/SD卡ROOT权限方法，应用程序获取Root权限有个前提是该设备已经破解(ROOT过，比如使用SuperOneClick软件，参考地址：[http://forum.xda-developers.com/showthread.php?t=803682](http://forum.xda-developers.com/showthread.php?t=803682))，否则执行SU命令毫无效果，演示程序使用chmod方式修改某个设备（TF卡分区）的访问权限，比如要按照物理扇区访问读写TF卡或希望删除某些重要文件时，在Android系统上必须具备ROOT权限才可以。
### 一、使用SuperOneClick等软件进行ROOT破解
下载地址：[http://forum.xda-developers.com/showthread.php?t=803682](http://forum.xda-developers.com/showthread.php?t=803682)
用法参考：[http://hi.baidu.com/cy594/blog/item/190e8794021c150bd31b7041.html](http://hi.baidu.com/cy594/blog/item/190e8794021c150bd31b7041.html)
该ROOT破解软件的原理就是利用Android系统的漏洞进行提权，最终目的开启ROOT账户登录，且在下文执行SU命令时不被Android系统放鸽子（执行失败）。如果您对ROOT结果及潜在风险知之甚少，请谨慎而为。
### 二、应用程序获取ROOT权限代码
示例获取ROOT并修改mmcblk0分区权限为777：
```java
package com.android.myapp.utility;
import java.io.DataOutputStream;
import android.app.Activity;
public class MyTestPri extends Activity{
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    	//* Create a TextView and set its content.
        //* the text is retrieved by calling a native
        //* function.
        setContentView(R.layout.main);
        String apkRoot="chmod 777 /dev/block/mmcblk0";//SD卡分区路径，也可能是mmcblk1随系统版本定，当前程序路径请用getPackageCodePath();
        RootCmd(apkRoot);
    }
    public boolean RootCmd(String cmd){
        Process process = null;
        DataOutputStream os = null;
        try{
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(cmd+ "\n");
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e) {
            return false;
        } finally {
            try {
                if (os != null)   {
                    os.close();
                }
                process.destroy();
            } catch (Exception e) {
            }
        }
        return true;
    }
}
```
以上程序第一次运行时，会弹出类似“XXX应用程序想要获取root权限”这样提示的对话框。这时你只需确认许可即可，下次再次执行时，Android会自动授权该程序进行ROOT提权。
执行完成后，可以看到该设备权限已被修改为777，注：如果只是想按照扇区读写SD卡，修改为606就足够了，或者再尝试低一点的权限。
另外，关于TF/SD卡的挂载点问题，好像有的版本会挂载到mmcblk0，有的会挂载到mmcblk1上，具体还是要判断一下，有深入研究过的不妨留言补充。
### 三、补充
上文方法介绍首先需要SuperOneClick进行ROOT破解，这对很多非开发人员而言都会有难度，而且容易忽视潜在的风险，所以，我们还可以通过更好的方法进行解决，下文方法就是很好的例子：
[http://my.unix-center.net/~Simon_fu/?p=951](http://my.unix-center.net/~Simon_fu/?p=951)（Android应用程序获取ROOT权限）
该文章通过对Android的启动服务修改（Init.rc）从而实现ROOT权限提升，不过要实现该功能你得是个开发人员，懂得Android源码修改和编译才行。这个功能暂未测试，该文章似乎是只获取了System权限，但是ROOT权限跟System权限还是有差别的，比如直接读写DEV设备类（比如上文的SD卡扇区读写）则必须ROOT权限，只有System权限是不行的。
天缘只是刚接触到这里但仍未测试，以后再试，有经验的Android高手欢迎赐教。
**参考资料：**
[http://www.cnblogs.com/ycmoon/archive/2011/05/11/2042999.html](http://www.cnblogs.com/ycmoon/archive/2011/05/11/2042999.html)(Android应用程序获取ROOT权限的方法，本文主要参考来源)
