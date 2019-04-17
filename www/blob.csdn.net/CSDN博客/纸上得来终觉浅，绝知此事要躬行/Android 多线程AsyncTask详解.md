# Android 多线程AsyncTask详解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月02日 22:17:57[boonya](https://me.csdn.net/boonya)阅读数：1825标签：[Android																[asynctask																[thread																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





原文地址：[http://www.cnblogs.com/xiaoluo501395377/p/3430542.html](http://www.cnblogs.com/xiaoluo501395377/p/3430542.html)


本篇随笔将讲解一下Android的多线程的知识，以及如何通过AsyncTask机制来实现线程之间的通信。


一、Android当中的多线程


在Android当中，当一个应用程序的组件启动的时候，并且没有其他的应用程序组件在运行时，Android系统就会为该应用程序组件开辟一个新的线程来执行。默认的情况下，在一个相同Android应用程序当中，其里面的组件都是运行在同一个线程里面的，这个线程我们称之为Main线程。当我们通过某个组件来启动另一个组件的时候，这个时候默认都是在同一个线程当中完成的。当然，我们可以自己来管理我们的Android应用的线程，我们可以根据我们自己的需要来给应用程序创建额外的线程。


二、Main Thread 和 Worker Thread


在Android当中，通常将线程分为两种，一种叫做Main Thread，除了Main Thread之外的线程都可称为Worker Thread。


当一个应用程序运行的时候，Android操作系统就会给该应用程序启动一个线程，这个线程就是我们的Main Thread，这个线程非常的重要，它主要用来加载我们的UI界面，完成系统和我们用户之间的交互，并将交互后的结果又展示给我们用户，所以Main Thread又被称为UI Thread。


Android系统默认不会给我们的应用程序组件创建一个额外的线程，所有的这些组件默认都是在同一个线程中运行。然而，某些时候当我们的应用程序需要完成一个耗时的操作的时候，例如访问网络或者是对数据库进行查询时，此时我们的UI Thread就会被阻塞。例如，当我们点击一个Button，然后希望其从网络中获取一些数据，如果此操作在UI Thread当中完成的话，当我们点击Button的时候，UI线程就会处于阻塞的状态，此时，我们的系统不会调度任何其它的事件，更糟糕的是，当我们的整个现场如果阻塞时间超过5秒钟(官方是这样说的)，这个时候就会出现
 ANR (Application Not Responding)的现象，此时，应用程序会弹出一个框，让用户选择是否退出该程序。对于Android开发来说，出现ANR的现象是绝对不能被允许的。


另外，由于我们的Android UI控件是线程不安全的，所以我们不能在UI Thread之外的线程当中对我们的UI控件进行操作。因此在Android的多线程编程当中，我们有两条非常重要的原则必须要遵守：
- 绝对不能在UI Thread当中进行耗时的操作，不能阻塞我们的UI Thread
- 不能在UI Thread之外的线程当中操纵我们的UI元素

三、如何处理UI Thread 和 Worker Thread之间的通信


既然在Android当中有两条重要的原则要遵守，那么我们可能就有疑问了？我们既不能在主线程当中处理耗时的操作，又不能在工作线程中来访问我们的UI控件，那么我们比如从网络中要下载一张图片，又怎么能将其更新到UI控件上呢？这就关系到了我们的主线程和工作线程之间的通信问题了。在Android当中，提供了两种方式来解决线程直接的通信问题，一种是通过Handler的机制(这种方式在后面的随笔中将详细介绍),还有一种就是今天要详细讲解的 AsyncTask 机制。


四、AsyncTask


AsyncTask：异步任务，从字面上来说，就是在我们的UI主线程运行的时候，异步的完成一些操作。AsyncTask允许我们的执行一个异步的任务在后台。我们可以将耗时的操作放在异步任务当中来执行，并随时将任务执行的结果返回给我们的UI线程来更新我们的UI控件。通过AsyncTask我们可以轻松的解决多线程之间的通信问题。


怎么来理解AsyncTask呢？通俗一点来说，AsyncTask就相当于Android给我们提供了一个多线程编程的一个框架，其介于Thread和Handler之间，我们如果要定义一个AsyncTask，就需要定义一个类来继承AsyncTask这个抽象类，并实现其唯一的一个 doInBackgroud 抽象方法。要掌握AsyncTask，我们就必须要一个概念，总结起来就是: 3个泛型，4个步骤。


3个泛型指的是什么呢？我们来看看AsyncTask这个抽象类的定义，当我们定义一个类来继承AsyncTask这个类的时候，我们需要为其指定3个泛型参数：

```
AsyncTask　<Params, Progress, Result>
```
- Params: 这个泛型指定的是我们传递给异步任务执行时的参数的类型
- Progress: 这个泛型指定的是我们的异步任务在执行的时候将执行的进度返回给UI线程的参数的类型
- Result: 这个泛型指定的异步任务执行完后返回给UI线程的结果的类型


 我们在定义一个类继承AsyncTask类的时候，必须要指定好这三个泛型的类型，如果都不指定的话，则都将其写成Void，例如：

```
AsyncTask <Void, Void, Void>
```


4个步骤：当我们执行一个异步任务的时候，其需要按照下面的4个步骤分别执行
- onPreExecute(): 这个方法是在执行异步任务之前的时候执行，并且是在UI Thread当中执行的，通常我们在这个方法里做一些UI控件的初始化的操作，例如弹出要给ProgressDialog
- doInBackground(Params... params): 在onPreExecute()方法执行完之后，会马上执行这个方法，这个方法就是来处理异步任务的方法，Android操作系统会在后台的线程池当中开启一个worker
 thread来执行我们的这个方法，所以这个方法是在worker thread当中执行的，这个方法执行完之后就可以将我们的执行结果发送给我们的最后一个 onPostExecute 方法，在这个方法里，我们可以从网络当中获取数据等一些耗时的操作
- onProgressUpdate(Progess... values): 这个方法也是在UI Thread当中执行的，我们在异步任务执行的时候，有时候需要将执行的进度返回给我们的UI界面，例如下载一张网络图片，我们需要时刻显示其下载的进度，就可以使用这个方法来更新我们的进度。这个方法在调用之前，我们需要在
 doInBackground 方法中调用一个 publishProgress(Progress) 的方法来将我们的进度时时刻刻传递给 onProgressUpdate 方法来更新
- onPostExecute(Result... result): 当我们的异步任务执行完之后，就会将结果返回给这个方法，这个方法也是在UI Thread当中调用的，我们可以将返回的结果显示在UI控件上

为什么我们的AsyncTask抽象类只有一个 doInBackground 的抽象方法呢？？原因是，我们如果要做一个异步任务，我们必须要为其开辟一个新的Thread，让其完成一些操作，而在完成这个异步任务时，我可能并不需要弹出要给ProgressDialog，我并不需要随时更新我的ProgressDialog的进度条，我也并不需要将结果更新给我们的UI界面，所以除了 doInBackground 方法之外的三个方法，都不是必须有的，因此我们必须要实现的方法是 doInBackground
 方法。


五、通过AsyncTask来从网络上下载一张图片


下面我们就通过两个代码示例，来看看如何通过AsyncTask来从网络上下载一张图片，并更新到我们的ImageView控件上。


①下载图片时，弹出一个ProgressDialog，但是不显示实时进度


我们来看看布局文件：





**[java]**[view
 plain](http://blog.csdn.net/qingdujun/article/details/42751099#)[copy](http://blog.csdn.net/qingdujun/article/details/42751099#)[print](http://blog.csdn.net/qingdujun/article/details/42751099#)[?](http://blog.csdn.net/qingdujun/article/details/42751099#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
-     xmlns:tools="http://schemas.android.com/tools"
-     android:layout_width="match_parent"
-     android:layout_height="match_parent" >  
- 
-     <ImageView  
-         android:id="@+id/imageView"
-         android:layout_width="wrap_content"
-         android:layout_height="200dp"
-         android:layout_alignParentRight="true"
-         android:layout_alignParentTop="true"
-         android:scaleType="fitCenter"/>  
- 
-     <Button  
-         android:id="@+id/button"
-         android:layout_width="wrap_content"
-         android:layout_height="wrap_content"
-         android:layout_below="@+id/imageView"
-         android:layout_centerHorizontal="true"
-         android:layout_marginTop="41dp"
-         android:text="从网络上下载一张图片" />  
- 
- </RelativeLayout>  



就是很简单的一个ImageView控件和一个Button控件，当点击Button控件时，弹出一个ProgressDialog，然后开启一个异步任务，从网络中下载一张图片，并更新到我们的ImageView上。这里还要注意一点，如果我们要使用手机访问网络，必须还要给其授权才行，在后续的学习当中，将会详细讲解Android当中的授权的知识。我们来看看




AndroidManifest.xml文件：



**[html]**[view
 plain](http://blog.csdn.net/qingdujun/article/details/42751099#)[copy](http://blog.csdn.net/qingdujun/article/details/42751099#)[print](http://blog.csdn.net/qingdujun/article/details/42751099#)[?](http://blog.csdn.net/qingdujun/article/details/42751099#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <?xmlversion="1.0"encoding="utf-8"?>
- <manifestxmlns:android="http://schemas.android.com/apk/res/android"
- package="com.xiaoluo.android_asynctast"
- android:versionCode="1"
- android:versionName="1.0">
- 
- <uses-sdk
- android:minSdkVersion="8"
- android:targetSdkVersion="18"/>
- 
- <!-- 授权手机能够访问网络 -->
- <uses-permissionandroid:name="android.permission.INTERNET"/>
- 
- <application
- android:allowBackup="true"
- android:icon="@drawable/ic_launcher"
- android:label="@string/app_name"
- android:theme="@style/AppTheme">
- <activity
- android:name="com.xiaoluo.android_asynctast.MainActivity"
- android:label="@string/app_name">
- <intent-filter>
- <actionandroid:name="android.intent.action.MAIN"/>
- 
- <categoryandroid:name="android.intent.category.LAUNCHER"/>
- </intent-filter>
- </activity>
- </application>
- 
- </manifest>
接下来我们来看看我们的Activity代码：




**[java]**[view
 plain](http://blog.csdn.net/qingdujun/article/details/42751099#)[copy](http://blog.csdn.net/qingdujun/article/details/42751099#)[print](http://blog.csdn.net/qingdujun/article/details/42751099#)[?](http://blog.csdn.net/qingdujun/article/details/42751099#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MainActivity extends Activity  
- {  
- private Button button;  
- private ImageView imageView;  
- private ProgressDialog progressDialog;  
- privatefinal String IMAGE_PATH = "http://developer.android.com/images/home/kk-hero.jpg";  
- //    private final String IMAGE_PATH2 = "http://ww2.sinaimg.cn/mw690/69c7e018jw1e6hd0vm3pej20fa0a674c.jpg";
- @Override
- protectedvoid onCreate(Bundle savedInstanceState)  
-     {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_main);  
- 
-         button = (Button)findViewById(R.id.button);  
-         imageView = (ImageView)findViewById(R.id.imageView);  
- //    弹出要给ProgressDialog
-         progressDialog = new ProgressDialog(MainActivity.this);  
-         progressDialog.setTitle("提示信息");  
-         progressDialog.setMessage("正在下载中，请稍后......");  
- //    设置setCancelable(false); 表示我们不能取消这个弹出框，等下载完成之后再让弹出框消失
-         progressDialog.setCancelable(false);  
- //    设置ProgressDialog样式为圆圈的形式
-         progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);  
- 
-         button.setOnClickListener(new View.OnClickListener()  
-         {  
- @Override
- publicvoid onClick(View v)  
-             {  
- // 在UI Thread当中实例化AsyncTask对象，并调用execute方法
- new MyAsyncTask().execute(IMAGE_PATH);  
-             }  
-         });  
-     }  
- 
- /**
-      * 定义一个类，让其继承AsyncTask这个类
-      * Params: String类型，表示传递给异步任务的参数类型是String，通常指定的是URL路径
-      * Progress: Integer类型，进度条的单位通常都是Integer类型
-      * Result：byte[]类型，表示我们下载好的图片以字节数组返回
-      * @author xiaoluo
-      *
-      */
- publicclass MyAsyncTask extends AsyncTask<String, Integer, byte[]>  
-     {  
- @Override
- protectedvoid onPreExecute()  
-         {  
- super.onPreExecute();  
- //    在onPreExecute()中我们让ProgressDialog显示出来
-             progressDialog.show();  
-         }  
- @Override
- protectedbyte[] doInBackground(String... params)  
-         {  
- //    通过Apache的HttpClient来访问请求网络中的一张图片
-             HttpClient httpClient = new DefaultHttpClient();  
-             HttpGet httpGet = new HttpGet(params[0]);  
- byte[] image = newbyte[]{};  
- try
-             {  
-                 HttpResponse httpResponse = httpClient.execute(httpGet);  
-                 HttpEntity httpEntity = httpResponse.getEntity();  
- if(httpEntity != null && httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK)  
-                 {  
-                     image = EntityUtils.toByteArray(httpEntity);  
-                 }  
-             }  
- catch (Exception e)  
-             {  
-                 e.printStackTrace();  
-             }  
- finally
-             {  
-                 httpClient.getConnectionManager().shutdown();  
-             }  
- return image;  
-         }  
- @Override
- protectedvoid onProgressUpdate(Integer... values)  
-         {  
- super.onProgressUpdate(values);  
-         }  
- @Override
- protectedvoid onPostExecute(byte[] result)  
-         {  
- super.onPostExecute(result);  
- //    将doInBackground方法返回的byte[]解码成要给Bitmap
-             Bitmap bitmap = BitmapFactory.decodeByteArray(result, 0, result.length);  
- //    更新我们的ImageView控件
-             imageView.setImageBitmap(bitmap);  
- //    使ProgressDialog框消失
-             progressDialog.dismiss();  
-         }  
-     }  
- 
- @Override
- publicboolean onCreateOptionsMenu(Menu menu)  
-     {  
-         getMenuInflater().inflate(R.menu.main, menu);  
- returntrue;  
-     }  
- 
- }  




我们来看看效果图：

![](http://images.cnitblog.com/blog/432441/201311/18232844-72265b841d2748279768abb96d56531c.jpg)![](http://images.cnitblog.com/blog/432441/201311/18232903-4cf9c0d6fed340118cabc7bb04377196.jpg)



②带有进度条更新的下载一张网络图片


下面这个代码示例，将会在下载图片的时候，显示进度条的更新，配置文件都不变，我们来看看Activity代码：



**[java]**[view
 plain](http://blog.csdn.net/qingdujun/article/details/42751099#)[copy](http://blog.csdn.net/qingdujun/article/details/42751099#)[print](http://blog.csdn.net/qingdujun/article/details/42751099#)[?](http://blog.csdn.net/qingdujun/article/details/42751099#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MainActivity extends Activity  
- {  
- private Button button;  
- private ImageView imageView;  
- private ProgressDialog progressDialog;  
- privatefinal String IMAGE_PATH = "http://developer.android.com/images/home/kk-hero.jpg";  
- //    private final String IMAGE_PATH2 = "http://ww2.sinaimg.cn/mw690/69c7e018jw1e6hd0vm3pej20fa0a674c.jpg";
- @Override
- protectedvoid onCreate(Bundle savedInstanceState)  
-     {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_main);  
- 
-         button = (Button)findViewById(R.id.button);  
-         imageView = (ImageView)findViewById(R.id.imageView);  
- //    弹出要给ProgressDialog
-         progressDialog = new ProgressDialog(MainActivity.this);  
-         progressDialog.setTitle("提示信息");  
-         progressDialog.setMessage("正在下载中，请稍后......");  
- //    设置setCancelable(false); 表示我们不能取消这个弹出框，等下载完成之后再让弹出框消失
-         progressDialog.setCancelable(false);  
- //    设置ProgressDialog样式为水平的样式
-         progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);  
- 
-         button.setOnClickListener(new View.OnClickListener()  
-         {  
- @Override
- publicvoid onClick(View v)  
-             {  
- new MyAsyncTask().execute(IMAGE_PATH);  
-             }  
-         });  
-     }  
- 
- /**
-      * 定义一个类，让其继承AsyncTask这个类
-      * Params: String类型，表示传递给异步任务的参数类型是String，通常指定的是URL路径
-      * Progress: Integer类型，进度条的单位通常都是Integer类型
-      * Result：byte[]类型，表示我们下载好的图片以字节数组返回
-      * @author xiaoluo
-      *
-      */
- publicclass MyAsyncTask extends AsyncTask<String, Integer, byte[]>  
-     {  
- @Override
- protectedvoid onPreExecute()  
-         {  
- super.onPreExecute();  
- //    在onPreExecute()中我们让ProgressDialog显示出来
-             progressDialog.show();  
-         }  
- @Override
- protectedbyte[] doInBackground(String... params)  
-         {  
- //    通过Apache的HttpClient来访问请求网络中的一张图片
-             HttpClient httpClient = new DefaultHttpClient();  
-             HttpGet httpGet = new HttpGet(params[0]);  
- byte[] image = newbyte[]{};  
- try
-             {  
-                 HttpResponse httpResponse = httpClient.execute(httpGet);  
-                 HttpEntity httpEntity = httpResponse.getEntity();  
-                 InputStream inputStream = null;  
-                 ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();  
- if(httpEntity != null && httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK)  
-                 {  
- //    得到文件的总长度
- long file_length = httpEntity.getContentLength();  
- //    每次读取后累加的长度
- long total_length = 0;  
- int length = 0;  
- //    每次读取1024个字节
- byte[] data = newbyte[1024];  
-                     inputStream = httpEntity.getContent();  
- while(-1 != (length = inputStream.read(data)))  
-                     {  
- //    每读一次，就将total_length累加起来
-                         total_length += length;  
- //    边读边写到ByteArrayOutputStream当中
-                         byteArrayOutputStream.write(data, 0, length);  
- //    得到当前图片下载的进度
- int progress = ((int)(total_length/(float)file_length) * 100);  
- //    时刻将当前进度更新给onProgressUpdate方法
-                         publishProgress(progress);  
-                     }  
-                 }  
-                 image = byteArrayOutputStream.toByteArray();  
-                 inputStream.close();  
-                 byteArrayOutputStream.close();  
-             }  
- catch (Exception e)  
-             {  
-                 e.printStackTrace();  
-             }  
- finally
-             {  
-                 httpClient.getConnectionManager().shutdown();  
-             }  
- return image;  
-         }  
- @Override
- protectedvoid onProgressUpdate(Integer... values)  
-         {  
- super.onProgressUpdate(values);  
- //    更新ProgressDialog的进度条
-             progressDialog.setProgress(values[0]);  
-         }  
- @Override
- protectedvoid onPostExecute(byte[] result)  
-         {  
- super.onPostExecute(result);  
- //    将doInBackground方法返回的byte[]解码成要给Bitmap
-             Bitmap bitmap = BitmapFactory.decodeByteArray(result, 0, result.length);  
- //    更新我们的ImageView控件
-             imageView.setImageBitmap(bitmap);  
- //    使ProgressDialog框消失
-             progressDialog.dismiss();  
-         }  
-     }  
- 
- @Override
- publicboolean onCreateOptionsMenu(Menu menu)  
-     {  
-         getMenuInflater().inflate(R.menu.main, menu);  
- returntrue;  
-     }  
- 
- }  


我们来看看效果图：

![](http://images.cnitblog.com/blog/432441/201311/18234326-d5e8986d39a7485a965f78d835ba0db1.jpg)![](http://images.cnitblog.com/blog/432441/201311/18234334-ad8531cda8c149078d9242c59be87c9a.jpg)
![](http://images.cnitblog.com/blog/432441/201311/18234343-e20462be737d4059910d6e0b34185522.jpg)




这样我们就能够通过AsyncTask来实现从网络中下载一张图片，然后将其更新到UI控件中，并时时刻刻的更新当前的进度这个功能了。


六、AsyncTask的重要知识点


在上面两节已经详细讲解了AsyncTask的工作原理了，这里我们还要补充一下AsyncTask的一些其他知识点：


1.Cancelling a Task


我们可以在任何时刻来取消我们的异步任务的执行，通过调用 cancel(boolean)方法，调用完这个方法后系统会随后调用 isCancelled() 方法并且返回true。如果调用了这个方法，那么在 doInBackgroud() 方法执行完之后，就不会调用 onPostExecute() 方法了，取而代之的是调用 onCancelled() 方法。为了确保Task已经被取消了，我们需要经常调用 isCancelled() 方法来判断，如果有必要的话。


2.在使用AsyncTask做异步任务的时候必须要遵循的原则：
- AsyncTask类必须在UI Thread当中加载，在Android Jelly_Bean版本后这些都是自动完成的
- AsyncTask的对象必须在UI Thread当中实例化
- execute方法必须在UI Thread当中调用
- 不要手动的去调用AsyncTask的onPreExecute, doInBackground, publishProgress, onProgressUpdate, onPostExecute方法，这些都是由Android系统自动调用的
- AsyncTask任务只能被执行一次




到此，有关AsyncTask的总结就到此为止了，本篇随笔主要讲解了Android中的多线程知识，并且详细地讲解了 AsyncTask 异步任务的概念和实现机制，并通过实例来了解 AsyncTask 的执行过程，最后还补充了 AsyncTask 的一些重要知识点，包括如何取消一个 AsyncTask 以及，我们在使用 AsyncTask 时所必须遵循的规则。](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=asynctask&t=blog)](https://so.csdn.net/so/search/s.do?q=Android&t=blog)




