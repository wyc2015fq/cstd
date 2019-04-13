
# Intent - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月14日 17:49:29[Jlins](https://me.csdn.net/dyllove98)阅读数：2998


在android中，intent就像是一个邮差，辛勤高效的在各个组件之间来回穿梭。我们可以通过它启动一个Activity或者Service，或者是发送给广播组件，又或者是与后台的Service进行通信。所谓的Intent,字面意思就是"意图，目的"，在android中的定义就是一个动作的抽象描述，类似于接口是抽象的行为协议一样，但这两者在实现上是不同的东西。
即使是好的邮差，如果没有邮递地址，依然无法正确的将货物送到指定的地方。这个完全交给系统来处理，它会帮Intent寻找合适的邮递地址，像是发送给Activity的Intent就会准确的发送给该Activity，发送个各个广播的Intent就会发送给指定的广播，从不会出错。
**一.Intent的组成**
Intent所包含的信息主要包括两个方面：action和data。
action就是所要执行的动作，像是我们经常在根文件中声明的Activity的动作：ACTION_MAIN，表示应用程序的入口。action通常是一个大写表示的字符串，用来简要的描述动作，当然，我们也可以自定义自己应用的Intent，不过在使用的时候必须指定全称，也就是包名。
正如方法签名包含了参数和返回值一样，action的名字也必须尽量包含了该action的信息，所以在自定义自己的action的时候，名字是非常重要的，必须尽可能的与其他的action区分开来，而且还要将该名字与intent的其他部分紧密的结合在一起，简而言之，不是单独的定义action，而是要定义其他组件能够处理的完整协议。
我们可以通过setAction()来设置action和通过getAction()来获取action。
一般的action像是这样：
publicstaticfinalString ACTION_DIAL = "android.intent.action.DIAL"
data是我们的动作所要操作的数据，通常是以Uri的形式表示。data并不是单独的出现，它们经常和action成对出现，像是这样:ACTION_VIEW content://contacts/people/1，就是用于展现手机中通讯录标识为1的人的信息，而且还提示了数据的存放地点是在设备上，并且是由content provider所控制。
当一个Intent被发送给适合的组件时，我们除了要知道data的Uri之外，还必须知道数据的类型，也就是MIME type。我们可以显式地指明data的Uri和MIME type，可以通过setData()指明数据的uri，setType()指明数据的MIME type，setDataAndType()指明数据的Uri和MIME type，getData()用于获取Uri，而getType()用于获取type。
除了上面的主要属性外，Intent还可以包含其他信息：category,type,component和extras。
category给出了action执行的其他信息，像是CATEGORY_LAUNCHER表示目标Activity是应用程序中最优先被执行的Activity，也就是所谓的top-level Activity。还有CATEGORY_HOME,表示的就是手机开机启动后或者按下HOME键后显示的Activity，CATEGORY_BROWSABLE表示的就是能通过在网页浏览器中点击链接而激活的Activity，而CATEGORY_PREFERENCE表示Activity是用于设置的Activity。
type是显式的指明data的MIME type，一旦指明了type，就只能使用该类型的data。
component是显式的指明了使用Intent的组件，这时其他属性的设置就是可选的，因为它们的设置都是为了方便寻找合适的组件。
extras正如字面意思，就是额外的信息，它们使用Bundle类型进行数据传递，也就是键值对，像是time-zone代表的就是新的时间区域。通常它代表的是Intent的扩展信息，像是我们的action如果能够发送邮件,那么我们可以将邮件的标题和内容放在extras里面。
像是这样：
Bundle bundle =newBundle();
intent.putExtras(bundle);
flags的作用像是告诉系统应该如何启动Activity，或者在启动后应该如何处理该Activity等。
**二.Intent的使用**
Intent可以分为两类：显式的Intent和隐式的Intent。
显式的Intent指定了使用Intent的组件，所以它并不需要其他信息。通常它使用在应用程序的内部，像是启动应用程序的其他Activity或者内部定义的Service。
隐式的Intent并没有指定组件，所以它需要更加详细的信息以确保系统能够发送给正确的组件。但是这些还是不够的，组件本身还是要提供信息来确保它接收到正确的Intent，这就是intent-filter的作用。我们经常在根文件中注册一个组件的时候，指明它的intent-filter，像是在程序中使用广播组件的时候，我们可以显式的指明intent-filter，像是这样：registerReceiver(BroadcastReceiver, IntentFilter)。
我们在声明intent-filter的时候，通常只需要指定三个属性：action,data和category，但是一般情况下我们有时候也不会完整的设定这三个属性，像是只设定action和category，那么这时的filter就只会匹配没有data的intent。在intent-filter中的data还进一步划分成几个属性:type,scheme,authority和path。type就是我们熟悉的MIME type，我们简单的讲解一下其他我们不熟悉的属性。
所谓的sheme就是我们上面讲的content:Uri等信息，值得注意的是，如果我们只是声明了scheme而没有声明type的话，那么filter就只会匹配没有type的Intent，而像是content:Uri是不会被匹配的，因为content provider在存储的时候是有保存MIME type的信息，所以如果没有提供type的话，是无法从里面提取出data。如果我们只提供type而没有提供scheme的话，那就表示匹配没有Uri或者包含content:Uri和file:Uri的Intent。如果scheme和type都没有，那么匹配的将会是没有data或者type的Intent。值得注意的是，当我们需要指定authority的时候，authority的scheme列表和我们的intent-filter的scheme列表必须相同，而path则需要包括authority和scheme列表。
我们来一个例子：
content://com.example.project:200/folder/subfolder/etc
其中，content就是scheme，com.example.project就是host,而200是port，path就是folder/subfolder/etc，host和port加起来就是URI authority，但注意，如果host没有指定，那么port也是没有必要的。
以上介绍的属性都是可选的，但这并不意味着它们是独立的：如果想要声明一个authority，那么scheme就是必要的。如果想要声明一个path，scheme和authority都是必要的。
我们再来看一个例子：
<intent-filter>
     <data android:mimeType="audio/mpeg"/>
</intent-filter>
这里我们可以使用audio/*，这就表示audio的所有子类型都可以匹配。
像是这样：
<data android:scheme="http" android:type="video/*"/>
表示我们的数据类型是video，然后可以通过网络来获取。
在实际的应用中，我们使用隐式的Intent比较多，之所以使用隐式的Intent，是因为我们想要让系统来决定哪个组件是最适合该Intent的，通常系统会比人更加清楚什么才是最适合的，只要我们给出的信息足够详细。
最好的情况就是我们在注册一个组件的时候，如果希望它接收Intent,那么就必须在根文件中声明它的intent-filter，这样无论是显式的还是隐式的Intent它都能接收到，如果没有，那么它只会接收显式的Intent。
IntentFilter并不像它的名字一样，具有过滤保护的作用，因为它无法阻止显式的Intent发送给组件，它只能阻止隐式的Intent发送给该组件。
一个intent-filter可以包含多个action:
<intent-filter>
      <action android:name="android.intent.action.VIEW"/>
      <action android:name="android.intent.action.EDIT"/>
      <action android:name="android.intent.action.PICK"/>
      <category android:name="android.intent.category.DEFAULT"/>
      <data android:mimeType="vnd.android.cursor.dir/vnd.google.note"/>
</intent-filter>
这个filter声明了很多action，它允许用户view或者edit目录，也允许从目录中pick一个note出来。
值得注意的是，filter声明了DEFAULT这个category。之所以声明为DEFAULT，是因为Context.startActivity()和Activity.startActivityForResult()这两个方法都默认Intent包含DEFAULT这个category，除了两个例外情况：显式的声明目标activity的名字和包含action-MAIN和category-LAUNCHER的Intent。
这里还有一个特别的东西：data的mimeType。我们看到，这里的的mimeType声明为vnd.android.cursor.dir/vnd.google.note，说明我们可以从Content Provider中获取Cursor并进一步获取Note Pad的数据。
最后发送给Activity的Intent大概像是这样子:
{action:android.intent.action.VIEW data:content://com.google.provider.NotePad/notes}
{action:android.intent.action.PICK data:content://com.google.provider.NotePad/notes}
{action:android.intent.action.EDIT data:content://com.google.provider.NotePad/notes}
当然，这里的Intent并不是完整的，还得考虑一下Activity的情况。如果该Activity用于展示所有的数据，Intent就是上面这样子， 但如果只是单独作用于一条数据的Activity，就算intent-filter是一样的，实际上的Intent应该像是这样子：
{action:android.intent.action.VIEW data:content://com.google.provider.NotePad/notes/ID}
再来看一个例子：
<intent-filter android:label="@string/resolve_title">
     <action android:name="com.android.notepad.action.EDIT_TITLE"/>
     <category android:name="android.intent.category.DEFAULT"/>
     <category android:name="android.intent.category.ALTERNATIVE"/>
     <category android:name="android.intent.category.SELECTED_ALTERNATIVE"/>
     <data android:mimeType="vnd.android.cursor.item/vnd.google.note"/>
</intent-filter>
这个Filter声明的action是修改title。为了能够支持DEFAULT这个category，我们还必须支持其他两个标准的categories:ALTERNATIVE和SELECTED_ALTERNATIVE。这个Intent大概就是这样子：
{action:com.android.notepad.action.EDIT_TITLE data:content://com.google.provider.NotePad/notes/ID}
android系统本身就定义了大量的标准action和category，如果有需要的话，可以自己查阅官方文档。
接下来我们就针对常见的使用场景来分析如何使用Intent。
1.Activity的跳转:
Intent intent =newIntent(Activity1.this, Activity2.class);
startActivity(intent);
关于Activity的跳转，还有一个知识点需要补充。我们知道，在android中有一个Task(栈)，专门用于存放Activity，它遵循的是"先进后出"的原则，但如果我们不想要遵循这个原则，想要取出指定的Activity，我们就可以利用一个东西:Flag,也就是上面提到的Flag：
Intent intent =newIntent(Activity1.this, Activity2.class);//如果Activity在Task中存在，并且是在最顶端，不会启动新的Activityintent.addFlags(Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT);//如果Activity在Task中存在，将Activity上面的所有Activity结束掉intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);//默认的跳转类型。将Activity放到一个新的Task中intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);//如果Activity已经运行到Task，再次跳转不会再运行这个Activity
intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
2.向另一个Activity传递数据:
Intent intent =newIntent(Activity1.this, Activity2.class);
Bundle bundle=newBundle();
bundle.putString("name", "Jack");
intent.putExtras(bundle);
startActivity(intent);
至于接收数据：
Bundle bundle =newBundle();
String name= bundle.getString("name");
3.向上一个Activity返回结果，这种情况针对的是startActivityForResult(intent, requestCode)启动的Activity:
Intent intent =getIntent();
Bundle bundle=newBundle();
bundle.putString("name", "Jack");
intent.putExtras(bundle);
setResult(RESULT_OK, intent);
4.回调上一个Activity的结果处理函数:
@OverrideprotectedvoidonActivityResult(intrequestCode,intresultCode, Intent data){super.onActivityResult(requestCode, resultCode, data);if(requestCode ==REQUEST_CODE){if(resultCode ==RESULT_OK){
               String temp=null;
               Bundle bundle=data.getExtras();
               ....
          }
     }
}
接下来就是一些调用官方应用程序的使用场景：
1.显示网页:
Uri uri = Uri.parse("http://google.com");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri);  
startActivity(intent);
2.显示地图:
Uri uri = Uri.parse("geo:38.899533,-77.036476");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri);   
startActivity(intent);
3.路径规划:
Uri uri = Uri.parse("http://maps.google.com/maps?f=d&saddr=startLat%20startLng&daddr=endLat%20endLng&hl=en");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri);  
startActivity(intent);
4.打电话:
(1)叫出拨号程序：
Uri uri = Uri.parse("tel:0800000123");  
Intent intent=newIntent(Intent.ACTION_DIAL, uri);
(2)直接打电话：
Uri uri = Uri.parse("tel:0800000123");  
Intent intent=newIntent(Intent.ACTION_CALL, uri);  
startActivity(intent);
这些都要在根文件中声明权限：<user-permission id="android.permission.CALL_PHONE"/>
5.发送SMS(短信)和MMS(彩信):
//调用短信程序Intent intent =newIntent(Intent.ACTION_VIEW, uri);  
intent.putExtra("content", "你好");   
it.setType("vnd.android-dir/mms-sms");  
startActivity(intent);//发送短信Uri uri = Uri.parse("smsto://0800000123");  
Intent intent=newIntent(Intent.ACTION_SENDTO, uri);  
intent.putExtra("content", "你好");  
startActivity(intent);//发送彩信Uri uri = Uri.parse("content://media/external/images/media/23");  
Intent intent=newIntent(Intent.ACTION_SEND);   
intent.putExtra("content", "。。。");   
intent.putExtra(Intent.EXTRA_STREAM, uri);  
intent.setType("image/png");   
startActivity(intent);
6.发送Email:
Uri uri = Uri.parse("mailto:xxx@abc.com");  
Intent intent=newIntent(Intent.ACTION_SENDTO, uri);  
startActivity(intent);  

Intent intent=newIntent(Intent.ACTION_SEND);  
intent.putExtra(Intent.EXTRA_EMAIL,"me@abc.com");  
intent.putExtra(Intent.EXTRA_TEXT,"The email body text");  
intent.setType("text/plain");  
startActivity(Intent.createChooser(intent,"Choose Email Client"));  

Intent intent=newIntent(Intent.ACTION_SEND);    
String[] tos={"me@abc.com"};    
String[] ccs={"you@abc.com"};    
intent.putExtra(Intent.EXTRA_EMAIL, tos);    
intent.putExtra(Intent.EXTRA_CC, ccs);    
intent.putExtra(Intent.EXTRA_TEXT,"The email body text");    
intent.putExtra(Intent.EXTRA_SUBJECT,"The email subject text");    
intent.setType("message/rfc822");    
startActivity(Intent.createChooser(intent,"Choose Email Client"));//传送附件Intent intent =newIntent(Intent.ACTION_SEND);  
intent.putExtra(Intent.EXTRA_SUBJECT,"The email subject text");  
intent.putExtra(Intent.EXTRA_STREAM,"file:///sdcard/mysong.mp3");  sendIntent.setType("audio/mp3");  
startActivity(Intent.createChooser(intent,"Choose Email Client"));
7.播放多媒体:
Uri uri = Uri.parse("file:///sdcard/song.mp3");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri);  
intent.setType("audio/mp3");  
startActivity(intent); 
Uri uri= Uri.withAppendedPath(MediaStore.Audio.Media.INTERNAL_CONTENT_URI, "1");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri);  
startActivity(intent);
8.Market相关:
//寻找某个应用Uri uri = Uri.parse("market://search?q=pname:pkg_name"); 
Intent intent=newIntent(Intent.ACTION_VIEW, uri);  
startActivity(intent);//显示某个应用的相关信息Uri uri = Uri.parse("market://details?id=app_id");  
Intent intent=newIntent(Intent.ACTION_VIEW, uri); 
startActivity(intent);
9.Uninstall应用程序:
Uri uri = Uri.fromParts("package", strPackageName,null); 
Intent intent=newIntent(Intent.ACTION_DELETE, uri);   
startActivity(intent);








