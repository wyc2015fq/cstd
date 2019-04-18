# Android 简单计算器实现源码 - weixin_33985507的博客 - CSDN博客
2013年07月12日 19:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
1.string.xml代码
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
 1 <?xml version="1.0" encoding="utf-8"?>
 2 <resources>
 3 
 4     <string name="hello">Hello World, CalculatorActivity!</string>
 5     <string name="app_name">计算器</string>
 6     <string name="zero">0</string>
 7     <string name="one">1</string>
 8     <string name="two">2</string>
 9     <string name="three">3</string>
10     <string name="four">4</string>
11     <string name="five">5</string>
12     <string name="six">6</string>
13     <string name="seven">7</string>
14     <string name="eight">8</string>
15     <string name="nine">9</string>
16     
17     <string name="add">+</string>
18     <string name="cut">-</string>
19     <string name="multiply">×</string>
20     <string name="divide">÷</string>
21     <string name="spot">.</string>
22     <string name="equal">=</string>
23     <string name="reset">清除</string>
24    
25 </resources>
View Code
```
2.main.xml  布局代码
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
  1 <?xml version="1.0" encoding="utf-8"?>
  2 <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
  3     android:layout_width="fill_parent"
  4     android:layout_height="fill_parent"
  5     android:orientation="vertical" >
  6 
  7     <EditText
  8         android:id="@+id/text"
  9         android:layout_width="fill_parent"
 10         android:layout_height="130dp"
 11         android:textSize="45sp"
 12         android:maxLength="10"
 13         android:textColor="#FFF" 
 14         android:layout_gravity="center"
 15         android:cursorVisible="false"
 16         android:gravity="right"
 17          />
 18 
 19     <TableLayout
 20         android:id="@+id/tablelayout"
 21         android:layout_width="fill_parent"
 22         android:layout_height="wrap_content" >
 23 
 24         <LinearLayout
 25             android:layout_width="fill_parent"
 26             android:layout_height="fill_parent" >
 27 
 28                <Button
 29                 android:layout_width="240dp"
 30                 android:layout_height="60dp"
 31                 android:background="#222"
 32                 android:textColor="#FFF" 
 33                 android:text="" />
 34                         
 35             <Button
 36                 android:layout_width="80dp"
 37                 android:layout_height="60dp"
 38                 android:textColor="#FFF" 
 39                 android:text="@string/reset" 
 40                 android:id="@+id/reset"
 41                 />
 42         </LinearLayout>
 43 
 44         <LinearLayout
 45             android:layout_width="fill_parent"
 46             android:layout_height="fill_parent" >
 47 
 48             <Button
 49                 android:layout_width="80dp"
 50                 android:layout_height="wrap_content"
 51                 android:textSize="40sp" 
 52                 android:textColor="#FFF" 
 53                 android:text="@string/seven" 
 54                  android:id="@+id/seven"/>
 55 
 56             <Button
 57                 android:layout_width="80dp"
 58                 android:layout_height="wrap_content"
 59                 android:textSize="40sp" 
 60                 android:textColor="#FFF" 
 61                 android:text="@string/eight" 
 62               android:id="@+id/eight"/>
 63 
 64             <Button
 65                 android:layout_width="80dp"
 66                 android:layout_height="wrap_content"
 67                 android:textSize="40sp" 
 68                 android:textColor="#FFF" 
 69                 android:text="@string/nine" 
 70                 android:id="@+id/nine" />
 71 
 72             <Button
 73                 android:layout_width="80dp"
 74                 android:layout_height="fill_parent"
 75                 android:textSize="40sp" 
 76                 android:textColor="#FFF" 
 77                 android:text="@string/divide" 
 78                 android:id="@+id/divide" />
 79         </LinearLayout>
 80 
 81         <LinearLayout
 82             android:layout_width="fill_parent"
 83             android:layout_height="fill_parent" >
 84 
 85             <Button
 86                 android:layout_width="80dp"
 87                 android:layout_height="wrap_content"
 88                 android:textSize="40sp" 
 89                 android:textColor="#FFF" 
 90                 android:text="@string/four" 
 91                 android:id="@+id/four"
 92                 />
 93 
 94             <Button
 95                 android:layout_width="80dp"
 96                 android:layout_height="wrap_content"
 97                 android:textSize="40sp" 
 98                 android:textColor="#FFF" 
 99                 android:text="@string/five" 
100                 android:id="@+id/five"
101                 />
102 
103             <Button
104                 android:layout_width="80dp"
105                 android:layout_height="wrap_content"
106                 android:textSize="40sp" 
107                 android:textColor="#FFF" 
108                 android:text="@string/six" 
109                 android:id="@+id/six"
110                 />
111 
112             <Button
113                 android:layout_width="80dp"
114                 android:layout_height="fill_parent"
115                 android:textSize="40sp" 
116                 android:textColor="#FFF" 
117                 android:text="@string/multiply" 
118                 android:id="@+id/multiply"
119                 />
120         </LinearLayout>
121 
122         <LinearLayout
123             android:layout_width="fill_parent"
124             android:layout_height="fill_parent" >
125 
126             <Button
127                 android:layout_width="80dp"
128                 android:layout_height="wrap_content"
129                 android:textSize="40sp"
130                 android:textColor="#FFF"  
131                 android:text="@string/one" 
132                 android:id="@+id/one"
133                 />
134 
135             <Button
136                 android:layout_width="80dp"
137                 android:layout_height="wrap_content"
138                 android:textSize="40sp" 
139                 android:textColor="#FFF" 
140                 android:text="@string/two" 
141                 android:id="@+id/two"
142                 />
143 
144             <Button
145                 android:layout_width="80dp"
146                 android:layout_height="wrap_content"
147                 android:textSize="40sp" 
148                 android:textColor="#FFF" 
149                 android:text="@string/three" 
150                  android:id="@+id/three"
151                 />
152 
153             <Button
154                 android:layout_width="80dp"
155                 android:layout_height="fill_parent"
156                 android:textSize="40sp" 
157                 android:textColor="#FFF" 
158                 android:text="@string/cut" 
159                 android:id="@+id/cut"
160                 />
161         </LinearLayout>
162 
163         <LinearLayout
164             android:layout_width="fill_parent"
165             android:layout_height="fill_parent" >
166 
167             <Button
168                 android:layout_width="80dp"
169                 android:layout_height="wrap_content"
170                 android:textSize="40sp" 
171                 android:textColor="#FFF" 
172                 android:text="@string/spot" 
173                 android:id="@+id/spot"
174                 />
175 
176             <Button
177                 android:layout_width="80dp"
178                 android:layout_height="wrap_content"
179                 android:textSize="40sp" 
180                 android:textColor="#FFF" 
181                 android:text="@string/zero" 
182                 android:id="@+id/zero"
183                 />
184 
185             <Button
186                 android:layout_width="80dp"
187                 android:layout_height="wrap_content"
188                 android:textSize="40sp" 
189                 android:textColor="#FFF" 
190                 android:text="@string/equal" 
191                 android:id="@+id/equal"
192                 />
193 
194             <Button
195                 android:layout_width="80dp"
196                 android:layout_height="fill_parent"
197                 android:textSize="40sp" 
198                 android:textColor="#FFF" 
199                 android:text="@string/add"
200                 android:id="@+id/add"
201                  />
202         </LinearLayout>
203         
204     </TableLayout>
205 
206 </LinearLayout>
View Code
```
3.CalculatorActivity.java 代码
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
  1 package FosgeIT.Calculator;
  2 import android.app.Activity;
  3 import android.os.Bundle;
  4 import android.view.View;
  5 import android.view.View.OnClickListener;
  6 import android.widget.Button;
  7 import android.widget.TextView;
  8 import android.widget.Toast;
  9 
 10 /**
 11  * 计算器
 12  * 
 13  * @author YinRQ 2013-07-12 13:25:04
 14  */
 15 
 16 public class CalculatorActivity extends Activity {
 17 
 18     // 私有变量
 19     private int option = 0;
 20     private boolean newdigital = true;
 21     private double a = 0, b = 0;
 22     private Button btnOne;
 23     private Button btnTwo;
 24     private Button btnThree;
 25     private Button btnFour;
 26     private Button btnFive;
 27     private Button btnSix;
 28     private Button btnSeven;
 29     private Button btnEight;
 30     private Button btnNine;
 31     private Button btnZero;
 32     private Button btnAdd;
 33     private Button btnCut;
 34     private Button btnMultiply;
 35     private Button btnDivide;
 36     private Button btnEqual;
 37     private Button btnSpot;
 38     private Button btnReset;
 39 
 40     @Override
 41     public void onCreate(Bundle savedInstanceState) {
 42         super.onCreate(savedInstanceState);
 43         setContentView(R.layout.main);
 44 
 45         // 初始化控件
 46         btnOne = (Button) findViewById(R.id.one);
 47         btnTwo = (Button) findViewById(R.id.two);
 48         btnThree = (Button) findViewById(R.id.three);
 49         btnFour = (Button) findViewById(R.id.four);
 50         btnFive = (Button) findViewById(R.id.five);
 51         btnSix = (Button) findViewById(R.id.six);
 52         btnSeven = (Button) findViewById(R.id.seven);
 53         btnEight = (Button) findViewById(R.id.eight);
 54         btnNine = (Button) findViewById(R.id.nine);
 55         btnZero = (Button) findViewById(R.id.zero);
 56         btnAdd = (Button) findViewById(R.id.add);
 57         btnCut = (Button) findViewById(R.id.cut);
 58         btnMultiply = (Button) findViewById(R.id.multiply);
 59         btnDivide = (Button) findViewById(R.id.divide);
 60         btnEqual = (Button) findViewById(R.id.equal);
 61         btnSpot = (Button) findViewById(R.id.spot);
 62         btnReset = (Button) findViewById(R.id.reset);
 63 
 64         btnOne.setOnClickListener(lisenter);
 65         btnTwo.setOnClickListener(lisenter);
 66         btnThree.setOnClickListener(lisenter);
 67         btnFour.setOnClickListener(lisenter);
 68         btnFive.setOnClickListener(lisenter);
 69         btnSix.setOnClickListener(lisenter);
 70         btnSeven.setOnClickListener(lisenter);
 71         btnEight.setOnClickListener(lisenter);
 72         btnNine.setOnClickListener(lisenter);
 73         btnZero.setOnClickListener(lisenter);
 74         btnAdd.setOnClickListener(lisenter);
 75         btnCut.setOnClickListener(lisenter);
 76         btnMultiply.setOnClickListener(lisenter);
 77         btnDivide.setOnClickListener(lisenter);
 78         btnEqual.setOnClickListener(lisenter);
 79         btnReset.setOnClickListener(lisenter);
 80         btnSpot.setOnClickListener(lisenter);
 81     }
 82 
 83     
 84     private OnClickListener lisenter = new OnClickListener() {
 85 
 86         public void onClick(View v) {
 87 
 88             TextView text = (TextView) findViewById(R.id.text);
 89             String s = text.getText().toString();
 90             Button btn = (Button) v;
 91             String t = (String) btn.getText();
 92             if (btn.getId() == R.id.zero || btn.getId() == R.id.one
 93                     || btn.getId() == R.id.two || btn.getId() == R.id.three
 94                     || btn.getId() == R.id.four || btn.getId() == R.id.five
 95                     || btn.getId() == R.id.six || btn.getId() == R.id.seven
 96                     || btn.getId() == R.id.eight || btn.getId() == R.id.nine) {
 97                 if (newdigital) {
 98                     text.setText(s + t);
 99                 } else {
100                     text.setText(s);
101                     newdigital = false;
102                 }
103                 return;
104             }
105 
106             // 加
107             if (btn.getId() == R.id.add) {
108                 a = Double.parseDouble(s);
109                 option = 1;
110                 text.setText("");
111                 return;
112                 
113             }
114 
115             // 减
116             if (btn.getId() == R.id.cut) {
117                 a = Double.parseDouble(s);
118                 option = 2;
119                 text.setText("");
120                 return;
121             }
122 
123             // 乘
124             if (btn.getId() == R.id.multiply) {
125                 a = Double.parseDouble(s);
126                 option = 3;
127                 text.setText("");
128                 return;
129             }
130             
131             // 除
132             if (btn.getId() == R.id.divide){
133                 a = Double.parseDouble(s);
134                 option = 4;
135                 text.setText("");
136                 return;
137             }
138 
139             // 清除
140             if (btn.getId() == R.id.reset){
141                 a = 0;
142                 b = 0;
143                 option = 0;
144                 newdigital = true;
145                 text.setText("");
146                 return;
147             }
148 
149             // .
150             if (btn.getId() == R.id.spot){
151                 if (s.indexOf(".") == -1)
152                     if (s.trim().startsWith("0")) {
153                         text.setText("0.");
154                         newdigital = true;
155                     } else {
156                         text.setText(s + ".");
157 
158                     }
159                 return;
160             }
161 
162             // =
163             if (btn.getId() == R.id.equal){
164                 b = Double.parseDouble(s);
165                 switch (option) {
166                 case 1:
167                     text.setText(String.valueOf(a + b));
168                     break;
169                 case 2:
170                     text.setText(String.valueOf(a - b));
171                     break;
172                 case 3:
173                     text.setText(String.valueOf(a * b));
174                     break;
175                 case 4: {
176                     if (b != 0) {
177                         text.setText(String.valueOf(a / b));
178                     } else {
179                         Toast.makeText(CalculatorActivity.this, "除数不能为0",
180                                 Toast.LENGTH_SHORT).show();
181                         text.setText("");
182                         a = 0;
183                         b = 0;
184                         option = 0;
185                         newdigital = true;
186                         return;
187                     }
188                     break;
189                 }
190                 case 5:
191                     text.setText(String.valueOf(Math.pow(a, b)));
192                     break;
193 
194                 }
195                 return;
196             }
197         }
198 
199     };
200 
201 }
View Code
```
4.效果截图
![](https://images0.cnblogs.com/blog/543714/201307/12195421-3a211134a29b42a1aee9f43c9c0eab1a.jpg)
