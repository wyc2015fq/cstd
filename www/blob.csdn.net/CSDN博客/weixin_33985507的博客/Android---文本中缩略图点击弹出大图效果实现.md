# Android---文本中缩略图点击弹出大图效果实现 - weixin_33985507的博客 - CSDN博客
2011年09月02日 23:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
　　很久没有写博客了，这两天一直忙于一个关于考试的项目，将其中的一些效果实现的经验写下来，希望给看到的人或者给有这方面需求的人帮助。
　　首先来张效果图，没有经过美工处理的 实现基本功能
![](https://pic002.cnblogs.com/images/2011/85880/2011022517243144.png)
　　其实做这个项目复习了很多内容，将之前单个项目中用到的某些功能综合到一起了，例如1、自定义标题栏2、Java和JavaScript的互调3、Activity实现仿Dialog样式4、多线程实现考试倒计时5、退出Activity时保存配置信息（考试剩余时间）6、熟悉UI布局
　　上面这些效果中，讲讲通过Java和JavaScript互调实现点击文本中缩略图弹出一个大图。
　　最开始想实现这种效果的时候就想到了TextView控件，因为TextView通过Html这个类可以在文本中插入图片。但是有个问题困扰着我，一个文本中的图片个数是不确定的，我怎样在一个TextView中添加多个图片（这个好解，通过TextView的append方法可以拼接任意张，因为这里的缩略图都是相同的，但这却无法定位我点击了那个缩略图，应该弹出那个大图）。最后实在没办法，我想到了WebView这个控件，通过它加载一个本地html页面，在其中通过给<image />对象添加onclick事件，传递一个imgSrc参数给Java方法，透过Java方法打开一个Activity来显示图片，ok,整个要实现的效果就完成了。So Easy，虽然最后实现这个效果没多少代码，但是从TextView这个控件一路走来到WebView,也耗费了将近一天的功夫，但是这个过程却非常值得享受，喔，我又懂得了更多。
　　下面通过代码一步一步来解析：首先是准备asset中的本地html文件。
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body onload="window.JsUseJave.onLoad()">
Write a recount for your newsletter using the notes given _____
<a onClick='setIv("/sdcard/IMG001.png")' > <img id="ig"/></a>
<script language="javascript">
function load(src){
      document.getElementById('ig').src=src;
}
function setIv(src){
    window.JsUseJave.setImgSrc(src)
}
</script>
</body>
</html>
```
　　1、最开始的时候是想在进入Activity的时候就自动加载图片
```
wv.loadUrl("javascript:load('"+imgSrc+"'");
```
　　但是发现一个问题在body的onload()中直接调用js:load(src)方法并没有执行。没办法只能用通过点击按钮给图片加载资源。后来突然想到可以JavaScript和Java互调，就把这一步添加到JavaScriptInterface类中，这个类要自己定义
```
final class JsUseJaveInterface{
public void setImgSrc(String imgSrc){
            Intent intent =new Intent(E6.this,ImageShow.class);
            Bundle bundle=new Bundle();
            bundle.putString("imgSrc", imgSrc);
            intent.putExtras(bundle);
            startActivity(intent);
        }
//登录加载图片
        public void onLoad(){
            wv.loadUrl("javascript:load('"+src+"')");
        }
    }
```
　　设置JavaScript可调用Java
```
wv.addJavascriptInterface(new JsUseJaveInterface(), "JsUseJave");
```
　　在html文件body的onload事件中通过java转一步调用javascript方法中的load(src)事件。这样就能在加载Activity的时候将缩略图同时显示出来了。
　　2、有了前面的经验，点击缩略图弹出大图就好实现了。给<image/>添加点击事件间接的去调用Java中的一个方法重新打开一个Activity显示大图，就是上面的自定义的JsUseJavaInteface中的setImgSrc()方法。
**js调用Java中方法：window.JsUseJave.onLoad()，JsUseJave是wv.addJavascriptInterface(new JsUseJaveInterface(), "JsUseJave");中的别名，onLoad()则是JsUseJaveInterface这个类中定义的一个方法。
