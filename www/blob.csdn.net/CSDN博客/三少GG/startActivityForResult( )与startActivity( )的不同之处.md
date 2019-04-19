# startActivityForResult( )与startActivity( )的不同之处 - 三少GG - CSDN博客
2011年11月25日 01:21:06[三少GG](https://me.csdn.net/scut1135)阅读数：2841标签：[android																[string																[任务](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
       startActivityForResult与startActivity的不同之处在于：
1, startActivity( )     
             仅仅是跳转到目标页面，若是想跳回当前页面，则必须再使用一次startActivity( )。
2, startActivityForResult( ) 
             可以一次性完成这项任务，当程序执行到这段代码的时候，页面会跳转到下一个Activity，
而当这个Activity被关闭以后(this.finish())，程序会自动跳转会第一个Activity，并调用前一个Activity的onActivityResult( )方法。 
# [android中使用startActivityForResult回传数据(转)](http://www.cnblogs.com/leon19870907/archive/2011/04/09/2010701.html)
假设：我这里有两个Activity：A和B，从A中向B中传递数据的时候采用的是Bundle封装数据，然后从A中跳转到B中，当B有需求将数据封装起来回传给A并跳转回A。那么A中接收数据时还要先判断Bundle是否为空，因为第一次访问A的时候(即B还没有回传的时候)，Bundle是为空的，这样显然是比较麻烦的，不明智的做法。
还好startActivityForResult来做跳转给了我们更好的解决办法。
#### 1.跳转的时候不是采用startActivity(intent) 这个方法，而是startActivityForResult(intent, 0)。
```
1 Intent intent=new Intent();
2 intent.setClass(A.this, B.class);
3 Bundle bundle=new Bundle();
4 String str1="aaaaaa";
5 bundle.putString("str1", str1);
6 intent.putExtras(bundle);
7 startActivityForResult(intent,0);//这里采用startActivityForResult来做跳转，此处的0为一个依据，可以写其他的值，但一定要>=0
```
#### 2.重写onActivityResult方法，用来接收B回传的数据。
```
1 protectedvoid onActivityResult(int
 requestCode,int resultCode, Intent data) {
2 switch (resultCode) {//resultCode为回传的标记，我在B中回传的是RESULT_OK
3  case RESULT_OK:
4 Bundle b=data.getExtras();//data为B中回传的Intent
5   String str=b.getString("str1");//str即为回传的值
6 break;
7 default:
8 break;
9 }
10 }
```
#### 3.在B中回传数据时采用setResult方法，并且之后要调用finish方法。
```
setResult(RESULT_OK, intent); //intent为A传来的带有Bundle的intent，当然也可以自己定义新的Bundle
finish();//此处一定要调用finish()方法
```
这样当B中调用finish方法的时候，跳转到A时会自动调用onActivityResult方法，来获取B中回传的intent了。 
详细的代码不贴了，那纯属浪费时间，网上关于startActivityForResult的全是些转帖的，都是代码，看着真心烦。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://wiki.emsym.com/index.php?title=Android_MediaPlayer&oldid=5476](http://wiki.emsym.com/index.php?title=Android_MediaPlayer&oldid=5476)
[http://book.51cto.com/art/201110/296447.htm](http://book.51cto.com/art/201110/296447.htm)
