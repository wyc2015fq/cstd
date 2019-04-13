
# ndroid学习笔记-Activity&Intent - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月12日 21:26:26[Jlins](https://me.csdn.net/dyllove98)阅读数：1375


![wKioL1RjGFeTbepJAAEOViC79jE926.jpg](http://s3.51cto.com/wyfs02/M00/51/46/wKioL1RjGFeTbepJAAEOViC79jE926.jpg)
---
![wKiom1RjF_7x7LZxAACPDZma5-s442.jpg](http://s3.51cto.com/wyfs02/M00/51/48/wKiom1RjF_7x7LZxAACPDZma5-s442.jpg)
---
![wKiom1RjGA-TzPJ3AAFblpDx5lY001.jpg](http://s3.51cto.com/wyfs02/M00/51/48/wKiom1RjGA-TzPJ3AAFblpDx5lY001.jpg)

---
界面activity_main.xml
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|<|RelativeLayout|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|xmlns:tools|=|"http://schemas.android.com/tools"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"match_parent"
|android:paddingBottom|=|"@dimen/activity_vertical_margin"
|android:paddingLeft|=|"@dimen/activity_horizontal_margin"
|android:paddingRight|=|"@dimen/activity_horizontal_margin"
|android:paddingTop|=|"@dimen/activity_vertical_margin"
|tools:context|=|"com.example.activity02.MainActivity"|>
|<!--
|<TextView
|android:layout_width="wrap_content"
|android:layout_height="wrap_content"
|android:text="@string/hello_world" />
|-->
|<|EditText
|android:id|=|"@+id/usernameEditText"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:hint|=|"username"|/>
|<|Button
|android:id|=|"@+id/myButton"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:layout_below|=|"@+id/usernameEditText"|/>
|<|Button
|android:id|=|"@+id/sendButton"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:layout_below|=|"@+id/myButton"|/>
|</|RelativeLayout|>
|

界面otheractivity.xml
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|LinearLayout|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"match_parent"
|android:orientation|=|"vertical"|>
|<|TextView
|android:id|=|"@+id/myTextView"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|/>
|</|LinearLayout|>
|


MainActivity.java
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|public|class|MainActivity|extends|ActionBarActivity {
|private|Button myButton;
|private|Button sendButton;
|private|EditText usernameEditText;
|@Override
|protected|void|onCreate(Bundle savedInstanceState) {
|super|.onCreate(savedInstanceState);
|setContentView(R.layout.activity_main);
|myButton = (Button) findViewById(R.id.myButton);
|myButton.setText(|"传值"|);
|sendButton = (Button) findViewById(R.id.sendButton);
|sendButton.setText(|"发短信"|);
|usernameEditText = (EditText) findViewById(R.id.usernameEditText);
|myButton.setOnClickListener(|new|OnClickListener() {
|@Override
|public|void|onClick(View v) {
|//Intent intent = new Intent(MainActivity.this, OtherActivity.class);
|Intent intent =|new|Intent();
|intent.setClass(MainActivity.|this|, OtherActivity.|class|);
|String username = usernameEditText.getText().toString();
|intent.putExtra(|"username"|, username);
|MainActivity.|this|.startActivity(intent);
|//startActivity(intent);
|}
|});
|sendButton.setOnClickListener(|new|OnClickListener() {
|@Override
|public|void|onClick(View v) {
|//调用发端信的程序
|Uri uri = Uri.parse(|"smsto:18513430780"|);
|Intent intent =|new|Intent(Intent.ACTION_SENDTO, uri);
|intent.putExtra(|"sms_body"|,|"The SMS text"|);
|startActivity(intent);
|}
|});
|}
|}
|


OtherActivity.java
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|public|class|OtherActivity|extends|Activity{
|private|TextView myTextView;
|@Override
|protected|void|onCreate(Bundle savedInstanceState) {
|super|.onCreate(savedInstanceState);
|Toast.makeText(getApplicationContext(),|"click"|, Toast.LENGTH_SHORT).show();
|setContentView(R.layout.otheractivity);
|myTextView = (TextView) findViewById(R.id.myTextView);
|String username = getIntent().getStringExtra(|"username"|);
|myTextView.setText(|"传过来的值是："|+ username);
|}
|}

|

![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

