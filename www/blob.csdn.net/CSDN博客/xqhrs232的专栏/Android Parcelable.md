# Android Parcelable - xqhrs232的专栏 - CSDN博客
2012年02月29日 15:19:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：594标签：[android																[javadoc																[string																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=javadoc&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/fuuckwtu/article/details/6518600](http://blog.csdn.net/fuuckwtu/article/details/6518600)
非原创，摘自JAVA EYE
android提供了一种新的类型：Parcel。本类被用作封装数据的容器，封装后的数据可以通过Intent或IPC传递。 除了基本类型以
外，只有实现了Parcelable接口的类才能被放入Parcel中。
Parcelable实现要点：需要实现三个东西
1）writeToParcel 方法。该方法将类的数据写入外部提供的Parcel中.声明如下：
writeToParcel (Parcel dest, int flags) 具体参数含义见javadoc
2）describeContents方法。没搞懂有什么用，反正直接返回0也可以
3）静态的Parcelable.Creator接口，本接口有两个方法：
createFromParcel(Parcel in) 实现从in中创建出类的实例的功能
newArray(int size) 创建一个类型为T，长度为size的数组，仅一句话（return new T[size])即可。估计本方法是供外部类反序列化本类数组使用。
测试用的接收信息Activity
- 
**import** android.app.Activity;   
- 
- **import** android.content.Intent;   
- 
- **import** android.os.Bundle;   
- **import** android.os.Parcelable;   
- 
- **public****class** Test **extends** Activity {
   
- /** Called when the activity is first created. */
- @Override
- **public****void** onCreate(Bundle savedInstanceState) {   
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.main);   
-         Intent i = getIntent();   
-         Person p = i.getParcelableExtra("yes");   
-         System.out.println("---->"+p.name);   
-         System.out.println("---->"+p.map.size());   
-     }   
- }  
发送的Activity
- **import** java.util.HashMap;   
- 
- **import** android.app.Activity;   
- **import** android.content.Intent;   
- **import** android.os.Bundle;   
- 
- **public****class** TestNew **extends** Activity {
   
- /** Called when the activity is first created. */
- @Override
- **public****void** onCreate(Bundle savedInstanceState) {   
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.main);   
-         Intent intent = **new** Intent();   
-         Person p = **new** Person();   
-         p.map = **new** HashMap<String,String>();   
-         p.map.put("yes", "ido");   
-         p.name="ok";   
-         intent.putExtra("yes", p);   
-         intent.setClass(**this**, Test.**class**);   
-         startActivity(intent);   
-     }   
- }  
Parcelable的实现类
- **import** java.util.HashMap;   
- 
- **import** android.os.Parcel;   
- **import** android.os.Parcelable;   
- 
- **public****class** Person **implements** Parcelable {
   
- 
- **public** HashMap<String,String> map = **new** HashMap<String,String> ();
   
- 
- **public** String name ;   
- @Override
- **public****int** describeContents() {   
- **return**0;   
-     }   
- 
- @Override
- **public****void** writeToParcel(Parcel dest, **int** flags) {
   
- 
-         dest.writeMap(map);   
-         dest.writeString(name);   
-     }   
- **public****static****final** Parcelable.Creator<Person> CREATOR = **new** Parcelable.Creator<Person>() {   
- //重写Creator 
- 
- @Override
- **public** Person createFromParcel(Parcel source) {   
-             Person p = **new** Person();   
-             p.map=source.readHashMap(HashMap.**class**.getClassLoader());   
-             p.name=source.readString();   
- **return** p;   
-         }   
- 
- @Override
- **public** Person[] newArray(**int** size) {   
- // TODO Auto-generated method stub
- **return****null**;   
-         }   
-     };   
- 
- }  
分享到：
