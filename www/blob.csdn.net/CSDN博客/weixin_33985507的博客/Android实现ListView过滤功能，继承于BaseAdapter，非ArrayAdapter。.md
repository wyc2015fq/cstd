# Android实现ListView过滤功能，继承于BaseAdapter，非ArrayAdapter。 - weixin_33985507的博客 - CSDN博客
2013年03月01日 10:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
其实实现ListView过滤功能最方便的便是使用ArrayAdapter，里面自带的getFilter()方法能很方便的实现此功能，但是在实际的开发中，一般都是继承于BaseAdapter。还有一种是利用控件AutoComplete，这种方式只是在输入框的下方重新显示一个列表，显然，很多时候这两种方式也满足不了我们的要求。
在Activity中定义一个类，让它实现TextWatcher接口，然后再onTextChanged方法中去过滤。然后常见相应的Pattern和match,来判断传入的参数时候符合列表中的数据，符合就加入一个新的列表中。
首先贴出实现的效果图
![](https://images0.cnblogs.com/blog/375789/201303/01101459-23d88147f2274a91b0d3ae3d5603bd56.jpg)![](https://images0.cnblogs.com/blog/375789/201303/01101542-5c429a94a85f47d8b4b108b4ba9b56a1.jpg)![](https://images0.cnblogs.com/blog/375789/201303/01101550-62ed6d875e834ef2ab7c05e6b95d0777.jpg)![](https://images0.cnblogs.com/blog/375789/201303/01101533-61f546bffd2f42b6956ad3441c15bbc1.jpg)
 下面贴出主要实现代码
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 
 1 package com.example.demo;
 2 
 3 import java.util.ArrayList;
 4 import java.util.List;
 5 import java.util.regex.Matcher;
 6 import java.util.regex.Pattern;
 7 import android.os.Bundle;
 8 import android.app.Activity;
 9 import android.text.Editable;
10 import android.text.TextWatcher;
11 import android.view.Menu;
12 import android.widget.EditText;
13 import android.widget.ListView;
14 
15 public class MainActivity extends Activity {
16     
17     List<People> people = new ArrayList<People>() ;
18     EditText editinput;
19     ListView listview;
20     Adapter adapter;
21     
22 
23     @Override
24     protected void onCreate(Bundle savedInstanceState) {
25         super.onCreate(savedInstanceState);
26         setContentView(R.layout.activity_main);
27         editinput = (EditText)findViewById(R.id.edit);
28         listview = (ListView)findViewById(R.id.ydlist);
29         initdata();
30         adapter = new Adapter(getApplicationContext(), people);
31         listview.setAdapter(adapter);
32         editinput.addTextChangedListener(new watcher());
33     }
34     
35      void initdata(){
36         
37         people.add(new People("张三","1374456"));
38         people.add(new People("张三小子", "12444455"));
39         people.add(new People("李一", "1345555"));
40         people.add(new People("王一", "1355555"));
41         people.add(new People("王二", "1365555"));
42         people.add(new People("李三", "13565555"));
43         people.add(new People("李一", "123555"));
44      }
45     
46     class watcher implements TextWatcher{
47 
48         @Override
49         public void afterTextChanged(Editable s) {
50             // TODO Auto-generated method stub
51             
52         }
53 
54         @Override
55         public void beforeTextChanged(CharSequence s, int start, int count,
56                 int after) {
57             // TODO Auto-generated method stub    
58             
59         }
60 
61         @Override
62         public void onTextChanged(CharSequence s, int start, int before,
63                 int count) {
64             // TODO Auto-generated method stub
65             String aa = s.toString();
66             Pattern p = Pattern.compile(aa);
67             List<People> we = new ArrayList<People>();        
68             for(int i=0;i<people.size();i++){
69                 People pp = people.get(i);
70             Matcher matcher = p.matcher(pp.getName()+pp.getPhome());
71             if(matcher.find()){
72                 we.add(pp);
73             }
74             }
75             adapter = new Adapter(getApplicationContext(), we);
76             listview.setAdapter(adapter);
77         }
78         
79     }
80     
81     @Override
82     public boolean onCreateOptionsMenu(Menu menu) {
83         // Inflate the menu; this adds items to the action bar if it is present.
84         getMenuInflater().inflate(R.menu.activity_main, menu);
85         return true;
86     }
87 
88 }
```
自定义Adapter
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 
 1 package com.example.demo;
 2 
 3 import java.util.ArrayList;
 4 import java.util.List;
 5 import android.content.Context;
 6 import android.view.LayoutInflater;
 7 import android.view.View;
 8 import android.view.ViewGroup;
 9 import android.widget.BaseAdapter;
10 import android.widget.TextView;
11 
12 public class Adapter extends BaseAdapter  {
13     private List<People> people = new ArrayList<People>();
14     Context ct;
15     private LayoutInflater inflater;
16     public Adapter(Context ct,List<People> people) {
17         // TODO Auto-generated constructor stub
18         this.people = people;
19         this.ct = ct;
20         inflater = (LayoutInflater) ct.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
21     }
22 
23     @Override
24     public int getCount() {
25         // TODO Auto-generated method stub
26         return people.size();
27     }
28 
29     @Override
30     public Object getItem(int position) {
31         // TODO Auto-generated method stub
32         return people.get(position);
33     }
34 
35     @Override
36     public long getItemId(int position) {
37         // TODO Auto-generated method stub
38         return position;
39     }
40 
41     @Override
42     public View getView(int position, View convertView, ViewGroup parent) {
43         // TODO Auto-generated method stub
44         People p = people.get(position);
45         if(convertView==null){
46             convertView = inflater.inflate(R.layout.yd_item, null);
47         }
48         TextView tv1=(TextView)convertView.findViewById(R.id.ydtext1);
49         TextView tv2=(TextView)convertView.findViewById(R.id.ydtext2);
50         tv1.setText(p.getName());
51         tv2.setText(p.getPhome());
52         return convertView;
53     }
54 
55     
56 
57 }
```
实体类
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 
 1 package com.example.demo;
 2 
 3 public class People {
 4     private String Name ;
 5     private String Phome;
 6     
 7     
 8     
 9     public String getName() {
10         return Name;
11     }
12 
13 
14 
15     public void setName(String name) {
16         Name = name;
17     }
18 
19 
20 
21     public String getPhome() {
22         return Phome;
23     }
24 
25 
26 
27     public void setPhome(String phome) {
28         Phome = phome;
29     }
30 
31 
32 
33     public People(String name,String phone){
34         super();
35         this.Name = name;
36         this.Phome = phone;    
37     }
38 }
```
