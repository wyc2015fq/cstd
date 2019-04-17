# Android热更新方案Robust——美团热更新（热修复）使用介绍 - DEVELOPER - CSDN博客





2017年05月18日 16:26:19[学术袁](https://me.csdn.net/u012827205)阅读数：5164








> 
Android热更新方案Robust

[http://tech.meituan.com/android_robust.html](http://tech.meituan.com/android_robust.html)


> 
Android热更新方案Robust开源，新增自动化补丁工具

[http://tech.meituan.com/android_autopatch.html](http://tech.meituan.com/android_autopatch.html)


> 
美团 Robust 的 github demo 地址

[https://github.com/Meituan-Dianping/Robust](https://github.com/Meituan-Dianping/Robust)

## Robust 的原理

Robust插件对产品的每个函数在编译打包阶段都插入了一段代码。当我们需要对已上线的app进行bug代码修复时，这时如果存在patch.jar，就会调用patch.jar中作为修复bug的代码而跳过原先的代码片段，由此达到修复的目的；而对产品的每个函数进行插入一段代码的工作是由插件**apply plugin : 'robust'**来完成的；对我们需要修复代码操作而实现修复功能的patch.jar是由插件**apply plugin : 'auto-patch-plugin'**生成的。

## 大致应用流程

![](https://img-blog.csdn.net/20170518141247362)


![](https://img-blog.csdn.net/20170518140254984)






```java
/**
* 1.集成了 Robust 后，生成 apk。保存期间的混淆
*   文件 mapping.txt + Robust 生成记录文件 methodMap.robust ；
* 2.使用注解 @Modify 和 @Add 标注需要修复和用*于修复的方法 ；
* 3.开启补丁插件（apply plugin:'auto-patch-plugin'），
*   执行生成 apk 命令，获得补丁包patch.jar ；
* 4.通过推送或者接口的形式，通知 app 有补丁，需要修复；
* 5.加载补丁文件不需要重新启动应用（即时生效）。
*/
```

## 制作补丁前准备一


配置方面有两个地方：1.是在project级别的配置；2.是在module级别的配置；3.robust.xml复制到工程目录下；把如下图所示




![](https://img-blog.csdn.net/20170518142212805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### project级别的配置


![](https://img-blog.csdn.net/20170518142605407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### module级别的配置

![](https://img-blog.csdn.net/20170518143248436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### robust.xml配置

``







## 制作补丁前准备二

首先了解一下这个测试demo的逻辑；打开APP进入到MainActivity，在MainActivity中主要有两个按钮：点击进入HotFixActivity页面和点击执行修复功能；在HotFixActivity页面只有一个TextView显示文字，当可以进行代码修复功能时，在MainActivity点击修复按钮之后再点击进入HotFixActivity页面，你会发现TextView显示的内容变成了我们要修复的内容。

看一下代码：

package com.draem.application20170516;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.meituan.robust.Patch;
import com.meituan.robust.PatchExecutor;
import com.meituan.robust.RobustCallBack;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {

    @BindView(R.id.btn_go)
    Button btnGo;
    @BindView(R.id.btn_hotfix)
    Button btnHotfix;
    @BindView(R.id.tvShow2)
    TextView tvShow2;
    @BindView(R.id.tvShow3)
    TextView tvShow3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        SystemClock.sleep(2*1000);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        File file = getDir();//创建文件夹  /mnt/sdcard/HotFix
    }


    @OnClick({R.id.btn_go, R.id.btn_hotfix})
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_go:
                Intent it = new Intent(MainActivity.this, HotFixActivity.class);
                startActivity(it);
                break;
            case R.id.btn_hotfix://执行修复
                new PatchExecutor(getApplicationContext(),
                        new PatchManipulateImp(),
                        new RobustCallBack() {
                            @Override
                            public void onPatchListFetched(boolean result, boolean isNet) {
                                Log.e("error-hot", "打印 onPatchListFetched：" + "isNet=" + isNet );
                            }

                            @Override
                            public void onPatchFetched(boolean result, boolean isNet, Patch patch) {
                                Log.e("error-hot", "打印 onPatchFetched：" + "result=" + result+"isNet="+isNet + "--->" + "patch=" + patch);
                            }

                            @Override
                            public void onPatchApplied(boolean result, Patch patch) {
                                Log.e("error-hot", "打印 onPatchApplied：" + "result=" + result + "--->" + "patch=" + patch);
                            }

                            @Override
                            public void logNotify(String log, String where) {
                                Log.e("error-hot", "打印 logNotify：" + "log=" + log + "--->" + "where=" + where);
                            }

                            @Override
                            public void exceptionNotify(Throwable throwable, String where) {
                                Log.e("error-hot", "打印 exceptionNotify：" + "throwable=" + throwable.toString() + "--->" + "where=" + where);
                            }
                        }).start();
                break;
        }
    }

    int count = 0;

    private File getDir() {
        StringBuilder path = new StringBuilder();
        if (isSDAvailable()) {
            path.append(Environment.getExternalStorageDirectory()
                    .getPath());
            path.append(File.separator);// '/'
            path.append("HotFix");// /mnt/sdcard/HotFix
          
            Log.e("error-hotfix", "如果SD卡可用就在SD卡创建");
            tvShow3.setText("SD卡可用就在sd创建");
        } else {
            //如果SD卡不可用就在内存创建
            File filesDir = getApplication().getCacheDir();    //  cache  getFileDir file
            path.append(filesDir.getAbsolutePath());
           
            tvShow3.setText("SD卡不可用就在内存创建");
            Log.e("error-hotfix", "SD卡不可用就在内存创建");
        }
        File file = new File(path.toString());
        if (!file.exists() || !file.isDirectory()) {
            file.mkdirs();// 创建文件夹
            count += 10;
        }
        Toast.makeText(this, "file=" + file, Toast.LENGTH_SHORT).show();
        Log.e("error-hotfix", count+" ==>file地址=" + file.toString() + "-->" + file.getAbsolutePath());
        tvShow2.setText(file.toString()+ "\n" + file.getAbsolutePath());
        return file;

    }

    private boolean isSDAvailable() {
        if (Environment.getExternalStorageState().equals(
                Environment.MEDIA_MOUNTED)) {
            Toast.makeText(this, "sd 有效", Toast.LENGTH_SHORT).show();
            return true;
        } else {
            return false;
        }
    }
}





```java
package com.draem.application20170516;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;

public class HotFixActivity extends AppCompatActivity {

    @BindView(R.id.tvShow)
    TextView tvShow;

    @Override
//    @Modify
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hot_fix);
        ButterKnife.bind(this);

        tvShow.setText(getText());//加载错误的代码
//        tvShow.setText(getInfo());//加载正确的代码
    }


    private String getText(){
        return "Hot-Fix, this just an error";
    }


//    @Add
//    public String getInfo(){
//        return "Hot-Fix, 已经对含有error的代码进行了修改！";
//    }
}
```


```java
package com.draem.application20170516;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.meituan.robust.Patch;
import com.meituan.robust.PatchManipulate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by yuanjunhua on 2017/5/17.
 */

public class PatchManipulateImp extends PatchManipulate {
    @Override
    protected List<Patch> fetchPatchList(Context context) {

        Patch patch = new Patch();
        patch.setName("test patch");

        StringBuilder path = new StringBuilder();
        path.append(Environment.getExternalStorageDirectory()
                .getPath());
        path.append(File.separator);// '/'
        path.append("HotFix");// /mnt/sdcard/HotFix
        path.append(File.separator);
        path.append("patch");// /mnt/sdcard/HotFix/patch

        patch.setLocalPath(path.toString());
        Log.e("error-hotfix", "PatchManipulateImp 地址="+path);

        patch.setPatchesInfoImplClassFullName("com.draem.application20170516.PatchesInfoImpl");
        List<Patch> patchList = new ArrayList<>();
        patchList.add(patch);
        return patchList;
    }


    @Override
    protected boolean verifyPatch(Context context, Patch patch) {
        //do your verification, put the real patch to patch
        //放到app的私有目录
        StringBuilder path = new StringBuilder();
        path.append(context.getCacheDir());
        path.append(File.separator);// '/'
        path.append("HotFix");// /mnt/sdcard/HotFix
        path.append(File.separator);
        path.append("patch");// /mnt/sdcard/HotFix/patch
        patch.setTempPath(path.toString());
        //in the sample we just copy the file
        try {
            Log.e("error-hotfix", "patch.getLocalPath="+patch.getLocalPath()+"--->patch.getTempPath="+patch.getTempPath());
            copy(patch.getLocalPath(), patch.getTempPath());
        }catch (Exception e){
            e.printStackTrace();
            throw new RuntimeException("copy source patch to local patch error, no patch execute in path "+patch.getTempPath());
        }

        return true;
    }

    @Override
    protected boolean ensurePatchExist(Patch patch) {
        return true;
    }



    public void copy(String srcPath,String dstPath) throws IOException {
        File src = new File(srcPath);
        if(!src.exists()){
            try {
                Log.e("error-hitfix", "资源不存在哦  srcPath="+srcPath);
                Log.e("error-hitfix", "资源不存在哦  srcPath="+src.toString());
                Log.e("error-hitfix", "资源不存在哦  srcPath="+src.length());
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            throw new RuntimeException("source patch does not exist ");
        }
        File dst=new File(dstPath);
        if(!dst.getParentFile().exists()){
            dst.getParentFile().mkdirs();
        }
        InputStream in = new FileInputStream(src);
        try {
            OutputStream out = new FileOutputStream(dst);
            try {
                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
            } finally {
                out.close();
            }
        } finally {
            in.close();
        }
    }
}
```


然后就保持上面的代码 + module级别gradle中保持设置
```java
//在生成 apk 的时候使用 apply plugin:’robust’
apply plugin: 'robust'


////开启打补丁那个，关闭生成apk那个
//apply plugin: 'auto-patch-plugin'
```


这样准备二阶段就要告一段落了，然后我们执行终端命令生成apk：` gradlew clean assembleRelease --stacktrace --no-daemon`




打包成功之后我们可以看到（并按照图示操作）：




![](https://img-blog.csdn.net/20170518153349905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











操作完之后，将上面的源代码该解注释的解注释该加注解的加上注解：




![](https://img-blog.csdn.net/20170518154031440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20170518154048456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








最后就是执行终端代码生成patch.jar：（打包apk会失败，没关系我们是为了生产jar）

`gradlew clean assembleRelease --stacktrace --no-daemon`![](https://img-blog.csdn.net/20170518154920756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





到此为止，制作补丁前的准备已经完成；

## 开始修复工作

先要执行终端，把patch.jar文件copy到手机SD目录：/sdcard/HotFix/patch.jar

`adb push C:\studio_workspace\Application20170516\app\build\outputs\robust\patch.jar /sdcard/HotFix/patch.jar`
成功之后，我们只需要点击一下修复按钮然后跳转到HotFixActivity页面就好了，这时候会发现textview内容变化了！




![](https://img-blog.csdn.net/20170518161705668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170518161731185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





还有就是在使用终端命令的时候可能会遇到“不是内部命令的错误”，解决如下：原Android Sdudio 2.2和AndroidSdudio 2.3不一样
 ，Android Sdudio 2.3 的 adb.exe是放在android-sdk\platform-tools目录下面的，而2.2是放在tools目录下面的，所以需要把环境配置path的路径指到platform-tools下面。然后终端输入adb，能够显示相关的信息。当然如果你不想要使用终端命令来执行上面的操作完全可以使用“Build/Generate/Signed
 APK...”来代替上面的终端命令；

注意：1，测试安装到手机上的apk必须是签过名了的版本；2，配置SD卡的读写权限；










