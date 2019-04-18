# Android WebView 与JS的数据交互 - z69183787的专栏 - CSDN博客
2015年08月28日 15:13:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：402
## 关于WebView
我们知道目前android市场上的一些应用采用的开发方式大致分为三种：Native App、Web App、Hybrid App。本文主要是Hybrid
 App中实现的主要技术native组件与js的数据交互的理解以及实现。
Android API中提供了WebView组件来实现对html的渲染。所谓的HybridApp开发方式即是汇集了HTML5、CSS3、jS的相关开发技术，以及数据交换格式json/XML。这显然是Web开发工程师的技能。正是因为如此，众多中小企业选择了这种开发方式来减少对android开发工程师的过度依赖，至于这三种开发方式的比较与优劣不在本文考虑之列。
有了WebView这个组件，Android应用开发技术也就转嫁到html与java数据交互上来。说白了就是js与WebView的数据交互，这就是本文所要讨论的。
## WebView与js的数据交互
1.WebView中载入静态页面
将WebView添加到应用中。和原生控件一样，在layout引入WebView控件。代码片段如下：
**[html]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:id="@+id/linearLayout"
- android:layout_width="match_parent"
- android:layout_height="match_parent"
- android:background="#000"
- android:orientation="horizontal">
- <WebView
- android:id="@+id/webview"
- android:layout_width="match_parent"
- android:layout_height="match_parent"
- />
- </LinearLayout>
载入页面：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- webView = (WebView) findViewById(R.id.webview);  
- webView.loadUrl("file:///file:///android_asset/page.html");  
page.html存储在工程文件的assets根目录下。
2.        引入jquery mobile
引入js框架让我们编写的html页面更接近于原生控件的显示效果。目前主流的移动应用js框架有：jquery mobile和sencha
 touch（jquery mobile与sencha touch的选型不在本文讨论范围）。本文选择使用jquery mobile。
首先，在webview添加对js的支持：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- WebSettings setting = webView.getSettings();  
- setting.setJavaScriptEnabled(true);//支持js
增加对中文的支持:
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- WebSettings setting = webView.getSettings();  
- setting.setDefaultTextEncodingName("GBK");//设置字符编码
设置页面滚动条风格：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- webView.setScrollBarStyle(0);//滚动条风格，为0指滚动条不占用空间，直接覆盖在网页上
jquery mobile提供的标准页面模板TemplateForJQuery.html：
**[html]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- <!DOCTYPE html>
- <html>
- <head>
- <title>Page Title</title>
- 
- <metaname="viewport"content="width=device-width, initial-scale=1">
- 
- <linkrel="stylesheet"href="css/jquery.mobile-1.1.1.min.css"/>
- <scriptsrc="js/jquery.js"></script>
- <scriptsrc="js/jquery.mobile-1.1.1.min.js"></script>
- </head>
- <body>
- 
- <divdata-role="page">
- 
- <divdata-role="header">
- <h1>Page Title</h1>
- </div><!-- /header -->
- 
- <divdata-role="content">
- <p>Page content goes here.</p>
- </div><!-- /content -->
- 
- <divdata-role="footer">
- <h4>Page Footer</h4>
- </div><!-- /footer -->
- </div><!-- /page -->
- 
- </body>
- </html>
页面依赖的js库、css等均放在assets目录下,目录组织结构如下：
![](https://img-my.csdn.net/uploads/201212/05/1354718697_2736.png)
运行应用后的截图：
![](https://img-my.csdn.net/uploads/201212/05/1354718779_7885.png)
下面是button 的截图，与原生控件没什么明显区别，有种以假乱真的感觉：
![](https://img-my.csdn.net/uploads/201212/05/1354718847_2149.png)
3.        良好的用户体验
运行我们的应用发现，在拥有大量js的页面被载入时，一直处于等待中，这是很糟糕的用户体验。可以加入进度条解决。注意到webview提供的两个方法：setWebViewClient和setWebChromeClient。其中setWebChromeClient方法正是可以处理progress的加载，此外，还可以处理js对话框，在webview中显示icon图标等。对于处理progress的代码片段如下：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- webView.setWebChromeClient(new WebChromeClient() {  
- publicvoid onProgressChanged(WebView view, int progress) {// 载入进度改变而触发
- if (progress == 100) {  
-                     handler.sendEmptyMessage(1);// 如果全部载入,隐藏进度对话框
-             }  
- super.onProgressChanged(view, progress);  
-         }  
- });  
其中通过handler 消息机制来处理UI线程的更新：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- handler = new Handler() {  
- publicvoid handleMessage(Message msg) {// 定义一个Handler，用于处理下载线程与UI间通讯
- if (!Thread.currentThread().isInterrupted()){  
- switch (msg.what) {  
- case0:  
-                 pd.show();// 显示进度对话框
- break;  
- case1:  
-                 pd.hide();// 隐藏进度对话框，不可使用dismiss()、cancel(),否则再次调用show()时，显示的对话框小圆圈不会动。
- break;  
-             }  
-         }  
- super.handleMessage(msg);  
-     }  
- };  
对于setWebViewClient方法，一般用来处理html的加载（需要重载onPageStarted(WebView view, String url, Bitmap favicon)）、关闭（需要重载onPageFinished（WebViewview,
 String url）方法）。
setWebViewClient和setWebChromeClient的作用：前者主要用于处理webView的控制问题，如加载、关闭、错误处理等；后者主要处理js对话框、图标、页面标题等。
4.        获取java中的数据
单独构建一个接口，作为处理js与java的数据交互的桥梁，本文封装的代码AndroidToastForJs.java如下:
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- publicclass AndroidToastForJs {  
- 
- private Context mContext;  
- 
- public AndroidToastForJs(Context context){  
- this.mContext = context;  
-     }  
- 
- //webview中调用toast原生组件
- publicvoid showToast(String toast) {  
-         Toast.makeText(mContext, toast, Toast.LENGTH_SHORT).show();  
-     }  
- 
- //webview中求和
- publicint sum(int a,int b){  
- return a+b;  
-     }  
- 
- //以json实现webview与js之间的数据交互
- public String jsontohtml(){  
-         JSONObject map;  
-         JSONArray array = new JSONArray();  
- try {  
-             map = new JSONObject();  
-             map.put("name","aaron");  
-             map.put("age", 25);  
-             map.put("address", "中国上海");  
-             array.put(map);  
- 
-             map = new JSONObject();  
-             map.put("name","jacky");  
-             map.put("age", 22);  
-             map.put("address", "中国北京");  
-             array.put(map);  
- 
-             map = new JSONObject();  
-             map.put("name","vans");  
-             map.put("age", 26);  
-             map.put("address", "中国深圳");  
-             map.put("phone","13888888888");  
-             array.put(map);  
-         } catch (JSONException e) {  
-             e.printStackTrace();  
-         }  
- return array.toString();  
-     }  
- }  
Webview提供的传入js的方法：
**[java]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- webView.addJavascriptInterface(new AndroidToastForJs(mContext), "JavaScriptInterface");  
Html页面jsonData.html设计的部分代码如下：
**[html]**[view
 plain](http://blog.csdn.net/cappuccinolau/article/details/8262821#)[copy](http://blog.csdn.net/cappuccinolau/article/details/8262821#)
- <scripttype="text/javascript">
-     var result = JavaScriptInterface.jsontohtml();  
-     var obj = eval("("+result+")");//解析json字符串  
-     function showAndroidToast(toast)   
-     {          
-         JavaScriptInterface.showToast(toast);   
-     }  
-     function getjsonData(){  
-         var result = JavaScriptInterface.jsontohtml();  
-         var obj = eval("("+result+")");//解析json字符串  
-         for(i=0;i<obj.length;i++){  
-             var user=obj[i];  
-             document.write("<p>姓名:"+user.name+"</p>");  
-             document.write("<p>年龄:"+user.age+"</p>");  
-             document.write("<p>地址:"+user.address+"</p>");  
-             if(user.phone!=null){  
-                 document.write("<p>手机号码:"+user.address+"</p>");  
-             }  
-         }  
-     }     
-     function list(){  
-         document.write("<divdata-role='header'><p>another</p></div>");  
-     }  
- </script>
- </head>
- <body>
- <divdata-role="page">
- <divdata-role="header"data-theme="c">
- <h1>Android via Interface</h1>
- </div><!-- /header -->
- <divdata-role="content">
- <buttonvalue="say hello"onclick="showAndroidToast('Hello,Android!')"data-theme="e"></button>
- <buttonvalue="get json data"onclick="getjsonData()"data-theme="e"></button>
- </div><!-- /content -->
- <divdata-role="collapsible"data-theme="c"data-content-theme="f">
- <h3>I'm <script>document.write(obj[0].name);</script>,click to see my info</h3>
- <p><script>document.write("<p>姓名："+obj[0].name+"</p>");</script></p>
- <p><script>document.write("<p>年龄："+obj[0].age+"</p>");</script></p>
- <p><script>document.write("<p>地址："+obj[0].address+"</p>");</script></p>
- </div>
- <divdata-role="footer"data-theme="c">
- <h4>Page Footer</h4>
- </div><!-- /footer -->
- </div><!-- /page -->
- </body>
点击say hello按钮运行的截图如下：
![](https://img-my.csdn.net/uploads/201212/05/1354719179_9146.png)![](https://img-my.csdn.net/uploads/201212/05/1354719230_8207.png)
