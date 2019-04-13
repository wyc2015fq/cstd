
# Android学习笔记-EditText&TextView&Button&菜单栏 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月12日 21:28:52[Jlins](https://me.csdn.net/dyllove98)阅读数：1593


![wKiom1RjO_PwjxwTAAGPEtBxRo0993.jpg](http://s3.51cto.com/wyfs02/M02/51/5A/wKiom1RjO_PwjxwTAAGPEtBxRo0993.jpg)
界面文件activity_main.xml
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
|<|RelativeLayout|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|xmlns:tools|=|"http://schemas.android.com/tools"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"match_parent"
|android:paddingBottom|=|"@dimen/activity_vertical_margin"
|android:paddingLeft|=|"@dimen/activity_horizontal_margin"
|android:paddingRight|=|"@dimen/activity_horizontal_margin"
|android:paddingTop|=|"@dimen/activity_vertical_margin"
|tools:context|=|"com.example.activity_03.MainActivity"|>
|<|EditText
|android:id|=|"@+id/factor_one"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"|/>
|<|TextView
|android:id|=|"@+id/symbol"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:layout_below|=|"@+id/factor_one"|/>
|<|EditText
|android:id|=|"@+id/factor_two"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:layout_below|=|"@+id/symbol"|/>
|<|Button
|android:id|=|"@+id/multiply"
|android:layout_width|=|"fill_parent"
|android:layout_height|=|"wrap_content"
|android:layout_below|=|"@+id/factor_two"|/>
|</|RelativeLayout|>
|

界面文件result.xml
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
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|LinearLayout|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"match_parent"
|android:orientation|=|"vertical"|>
|<|TextView
|android:id|=|"@+id/result"
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
|49
|50
|51
|52
|53
|54
|55
|56
|57
|public|class|MainActivity|extends|ActionBarActivity {
|private|EditText factor_one;
|private|EditText factor_two;
|private|TextView symbol;
|private|Button multiply;
|@Override
|protected|void|onCreate(Bundle savedInstanceState) {
|super|.onCreate(savedInstanceState);
|setContentView(R.layout.activity_main);
|factor_one = (EditText) findViewById(R.id.factor_one);
|factor_two = (EditText) findViewById(R.id.factor_two);
|symbol = (TextView) findViewById(R.id.symbol);
|multiply = (Button) findViewById(R.id.multiply);
|symbol.setText(R.string.symbol);
|multiply.setText(|"计算"|);
|multiply.setOnClickListener(|new|OnClickListener() {
|@Override
|public|void|onClick(View v) {
|// TODO Auto-generated method stub
|String factor_one_str = factor_one.getText().toString();
|String factor_two_str = factor_two.getText().toString();
|Intent intent =|new|Intent();
|intent.putExtra(|"factor_one_str"|, factor_one_str);
|intent.putExtra(|"factor_two_str"|, factor_two_str);
|intent.setClass(MainActivity.|this|, ResultActivity.|class|);
|MainActivity.|this|.startActivity(intent);
|}
|});
|}

|@Override
|public|boolean|onCreateOptionsMenu(Menu menu) {|//设置菜单
|//MenuItem android.view.Menu.add(int groupId, int itemId, int order, int titleRes)
|menu.add(|0|,|1|,|1|, R.string.about);
|menu.add(|0|,|2|,|2|, R.string.exit);
|return|super|.onCreateOptionsMenu(menu);
|}
|@Override
|public|boolean|onOptionsItemSelected(MenuItem item) {|//选中菜单选项之后进行的操作
|if|(item.getItemId() ==|1|) {|//关于
|}|else|if|(item.getItemId() ==|2|) {|//退出
|finish();
|}
|return|super|.onOptionsItemSelected(item);
|}
|}
|


ResultActivity.java
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
|public|class|ResultActivity|extends|Activity{
|private|TextView result;
|@Override
|protected|void|onCreate(Bundle savedInstanceState) {
|super|.onCreate(savedInstanceState);
|setContentView(R.layout.result);
|String factor_one_str = getIntent().getStringExtra(|"factor_one_str"|);
|String factor_two_str = getIntent().getStringExtra(|"factor_two_str"|);
|int|factor_one = Integer.parseInt(factor_one_str);
|int|factor_two = Integer.parseInt(factor_two_str);
|result = (TextView) findViewById(R.id.result);
|result.setText(factor_one * factor_two +|""|);
|}
|}
|

strings.xml
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
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|resources|>
|<|string|name|=|"app_name"|>Activity_03</|string|>
|<|string|name|=|"hello_world"|>Hello world!</|string|>
|<|string|name|=|"action_settings"|>Settings</|string|>
|<|string|name|=|"symbol"|>乘以</|string|>
|<|string|name|=|"exit"|>退出</|string|>
|<|string|name|=|"about"|>关于</|string|>
|</|resources|>
|

AndroidManifest.xml 中注册Activity
|1
|<|activity|android:name|=|".ResultActivity"|android:label|=|"ResultActivity"|/>
|

![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

