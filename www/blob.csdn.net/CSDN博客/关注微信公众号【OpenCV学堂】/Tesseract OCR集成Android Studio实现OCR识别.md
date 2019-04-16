# Tesseract OCR集成Android Studio实现OCR识别 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年11月21日 10:50:52[gloomyfish](https://me.csdn.net/jia20003)阅读数：20930
个人分类：[OpenCV4Android教程](https://blog.csdn.net/jia20003/article/category/6514666)









# Tesseract OCR集成Android Studio实现OCR识别

### 介绍

Tesseract OCR谷歌开源的OCR识别引擎，支持多国文字包括中文简体与繁体。最新的版本是3.x。可以通过安装程序安装在机器上然后通过命令行运行该程序识别各种图片中的文字、同时还提供二次开发包，支持二次开发包括C、C++语言。也可以被移植到Android平台实现移动应用领域的OCR识别APP。

### 下载

在Android平台上使用Tesseract OCR首先要下载Tess2工程，它是专门针对Android平台编译出来的，下载地址如下 - [https://github.com/rmtheis/tess-two](https://github.com/rmtheis/tess-two) 。下载解压缩之后你就会看到如下目录结构： 
![这里写图片描述](https://img-blog.csdn.net/20161121100131631)
然后打开Android Studio新建一个项目应用，选择导入Module之后选择导入红色圆圈中的tess-two文件夹，导入之后你就会看到如下： 
![这里写图片描述](https://img-blog.csdn.net/20161121101034955)

项目必须是支持NDK的，所以要在Project Structure中指明NDK的路径。原因是tess-two是个NDK项目，没有NDK支持无法完成编译。 
![这里写图片描述](https://img-blog.csdn.net/20161121101725067)
此外你可能还会遇到没有android-maven的错误，把下面的脚本加到build.gradle的最上面即可： 
buildscript { 

    repositories { 

        jcenter() 

    } 

    dependencies { 

        classpath ‘com.android.tools.build:gradle:2.1.2’ 

        classpath ‘org.codehaus.groovy:groovy-backports-compat23:2.3.5’ 

        classpath ‘com.jfrog.bintray.gradle:gradle-bintray-plugin:1.0’ 

        classpath ‘com.github.dcendents:android-maven-gradle-plugin:1.5’ 

    } 

} 
如果一切顺利，你就会完成导入与编译，看到导入成功的目录结构，上图第二张所示。下面就是在你的Activity中添加初始化代码如下： 
    private void initTessBaseData() { 

        mTess = new TessBaseAPI(); 

        String datapath = Environment.getExternalStorageDirectory() + “/tesseract/”; 

        // String language = “num”; 

        String language = “eng”; 

        File dir = new File(datapath + “tessdata/”); 

        if (!dir.exists()) 

            dir.mkdirs(); 

        mTess.init(datapath, language); 

    } 
然后通过监听【识别】按钮的Click事件响应识别动作如下： 
    public void onClick(View v) { 

        Bitmap bitmap = BitmapFactory.decodeResource(this.getResources(), R.drawable.textimage); 

        mTess.setImage(bitmap); 

        String result = mTess.getUTF8Text(); 

        TextView txtView = (TextView)this.findViewById(R.id.idCard_textView); 

        txtView.setText(“结果为:” + result); 

        ImageView imgView = (ImageView)this.findViewById(R.id.imageView); 

        imgView.setImageBitmap(bitmap); 

    } 

    显示结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161121104353827)
其中上部分为识别到文字显示，下面是对应加载的文字图片。 

这里特别要注意的是必须在你对应目录有相关tessdata文件才可以，关于tessdata可以到如下地址下载： 
[https://github.com/tesseract-ocr/tessdata](https://github.com/tesseract-ocr/tessdata)

手动或者通过程序Copy到你在初始化时候生成的目录下面即可。 
***此外，还不要忘记把tess-two作为依赖添加到app中去。***





