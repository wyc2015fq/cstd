# Creating Android Applications with HTML User Interface - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年03月20日 10:15:43[boonya](https://me.csdn.net/boonya)阅读数：888标签：[android																[html																[url																[layout																[webview](https://so.csdn.net/so/search/s.do?q=webview&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)






The article from here:[http://ramkulkarni.com/blog/creating-android-applications-with-html-user-interface/](http://ramkulkarni.com/blog/creating-android-applications-with-html-user-interface/)


I created my first two Android applications completely using Android APIs. However I realized that I can build better user interface quickly using HTML, JavaScript and CSS. Android SDK provides WebView controls which is a browser control. So I started looking
 for ways to make WebView render local HTML files. Most APIs of WebView are simple to understand and use, except one important method, loadUrl.


To begin with I was not sure where to store my HTML/JS files in the Android project and what URL to use to load them. JavaDoc help for WebView did not mention how to load local assets into WebView. It turned out that you need to keep all your HTML/JS files
 in the ‘assets’ folder of you project and load them with URL file://android_asset/.


Here are steps to load local HTML file in the Activity class -
- 
You first need to create a layout for your Activity in res/layout folder of your Android project
- 
In the left pallet, click ‘Composite’ view and drag and drop ‘WebView’ control on to the layout
![Adding WebView to layout](http://ramkulkarni.com/blog/wp-content/uploads/2012/03/20120310_layout.jpg)
- 
In your activity class, associate above layout with the activity using setContentView method -
public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main); // pass id of layout resource
    }
- 
In the onStart method of the Activity, get reference to the WebView we created above and load URL
public void onStart() {
        super.onStart();

        WebView webView =  (WebView)findViewById(R.id.webView1);
        //enable JavaScript
        webView.getSettings().setJavaScriptEnabled(true);
        webView.loadUrl("file:///android_asset/index.html");
    }
- 
Create index.html in the ‘assets’ folder of your project

## Accessing Java class from JavaScript


With HTML, JavaScript and CSS you can create many useful Android applications. If you use HTML5 APIs, you could even create and access SQLLite database from JavaScript. However sometimes you many want to get access to Android APIs from your JavaScript code.
 You can do that as follows.


Create a class that will act as an interface between Java code and JavaScript code
public class java2JSAgent
    {
        public String getContacts()
        {
            String jsonResponse = null;
            //call Android APIs to get contacts
            //serialize to JSON and assign to jsonResponse

            return jsonResponse;
        }
    }

Craeate and instance of this class in onStart method and associate it with the instance of WebView
webView.addJavascriptInterface(new java2JSAgent(), "java2JSAgentVar");

The second argument to addJavascriptInterface method is name

of the JavaScript variable

. You can now access the Java object in JavaScript code using this variable name

.
<script type="text/javascript">
    var contactsJson = java2JSAgentVar.getContacts();
</script>

So, using HTML, JavaScript, CSS and native Android APIs you can create quite powerful Android applications. And if you want to develop cross platform mobile applications using HTML and JS, then [PhoneGap](http://phonegap.com/) is
 your best bet.


-Ram Kulkarni

Update :

In Android SDK 4.2, you have to annotate (@JavascriptInterface) methods in Java class that you want to make available to JavaScript. (see description of [addJavascriptInterface](http://developer.android.com/reference/android/webkit/WebView.html#addJavascriptInterface(java.lang.Object,%20java.lang.String))).
 So in the above example the class java2JSAgent would be -
**import** android.webkit.JavascriptInterface;**public****class** java2JSAgent
{@JavascriptInterface 
        **public****String** getContacts(){**String** jsonResponse =**null**;//call Android APIs to get contacts//serialize to JSON and assign to jsonResponse**return** jsonResponse;}}

Also when running this code with the latest SDK and emulator, I had to sometimes call webView.setWebChromeClient(new WebChromeClient()) after enabling JavaScript in the WebView. Otherwise I observed that JavaScript code on the HTML page was not getting executed.
 However I observed that this behavior was inconsistent.
**public** void onStart() {**super**.onStart();

        WebView webView =(WebView)findViewById(R.id.webView1);//enable JavaScript
        webView.getSettings().setJavaScriptEnabled(**true**);
        webView.setWebChromeClient(**new** WebChromeClient());

        webView.loadUrl("file:///android_asset/index.html");}](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




