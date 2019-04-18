# [置顶] Android事件—单选按键和下拉按键 - weixin_33985507的博客 - CSDN博客
2013年07月30日 18:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
在平常的开发中单选按键和下拉按键是非常常用的2个点击事件。首先介绍下单选按键
       1：单选按键，单选的主键是radiogroup 这个主键也是很重要的
首先介绍下主键的布局
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
xmlns:android="http://schemas.android.com/apk/res/android"
android:layout_width="fill_parent"
android:layout_height="fill_parent"
android:orientation="vertical">
<TextView android:id="@+id/myRadioListenerTV"
android:layout_width="fill_parent"
android:layout_height="wrap_content"
android:textSize="20px"
android:text="您选择的性别是："/>
<RadioGroup android:id="@+id/myRadioListenerRG"
 android:layout_width="wrap_content"
 android:layout_height="wrap_content"
 android:checkedButton="@+id/myRadioListenerMale">
<RadioButton android:id="@+id/myRadioListenerMale"
android:text="男"/>
<RadioButton android:id="@+id/myRadioListenerFemale"
android:text="女"/>
</RadioGroup>
</LinearLayout>
在单选RadioGroup 中里面的选择项主要是由RadioButton 组成，这和一般的主键区别不大 radiogroup里面的属性就不一一介绍了。
**private** RadioGroup radioGroup;
**private** TextView textView;
**private** RadioButton btMale,btFemale;
**protectedvoid** onCreate(Bundle savedInstanceState) {
**super**.onCreate(savedInstanceState);
**super**.setContentView(R.layout.*myradiolistener*);
**this**.radioGroup=(RadioGroup)**super**.findViewById(R.id.*myRadioListenerRG*);
**this**.textView=(TextView)**super**.findViewById(R.id.*myRadioListenerTV*);
**this**.btMale=(RadioButton)**super**.findViewById(R.id.*myRadioListenerMale*);
**this**.btFemale=(RadioButton)**super**.findViewById(R.id.*myRadioListenerFemale*);
this.radioGroup.setOnCheckedChangeListener(new CheckedChanegdListener());
}
**public**RadioGroup getRadioGroup() {
**return**radioGroup;
}
**publicvoid** setRadioGroup(RadioGroup radioGroup) {
**this**.radioGroup = radioGroup;
}
**public** TextView getTextView() {
**return**textView;
}
**publicvoid** setTextView(TextView textView) {
**this**.textView = textView;
}
**public** RadioButton getBtMale() {
**return**btMale;
}
**publicvoid** setBtMale(RadioButton btMale) {
**this**.btMale = btMale;
}
**public** RadioButton getBtFemale() {
**return**btFemale;
}
**publicvoid** setBtFemale(RadioButton btFemale) {
**this**.btFemale = btFemale;
}
this.radioGroup.setOnCheckedChangeListener(new CheckedChanegdListener()); 
就是单选按键事件的重要点，我们不推荐在里面直接写内部类的方式
在单选RadioGroup 主要是实现CheckedChanegdListener方法
这个类是实现OnCheckedChangeListener这个接口的
**publicvoid** onCheckedChanged(RadioGroup group, **int** checkedId) {
**if**(group.getId()==R.id.*myRadioListenerRG*){
String temp="";
myRadioListener my1=(myRadioListener)group.getContext();
**if**(my1.getBtMale().getId()==checkedId){
temp="您选择的性别是："+my1.getBtMale().getText();
}**elseif**(my1.getBtFemale().getId()==checkedId){
temp="您选择的性别是："+my1.getBtFemale().getText();
}
my1.getTextView().setText(temp);
}
在onCheckedChanged 这个方法中有2个参数第一个是当前的单选按键，第二个是被选中的radiobutton的id
group.getId()==R.id.*myRadioListenerRG 判断当前是对那个单选按键操作*
1：下拉列表（Spinner）
下拉列表的的主键是Spinner 这个主键和其他的主键有点区别，主要是数据源的来源，可以在代码中实现，也可以用配置的方式
下面的程序是用代码实现数据源的
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
xmlns:android="http://schemas.android.com/apk/res/android"
android:layout_width="fill_parent"
android:layout_height="fill_parent"
android:orientation="vertical">
<TextView android:id="@+id/mySpinnerClickTV"
 android:layout_width="fill_parent"
 android:layout_height="wrap_content"
 android:text="您选择的颜色是："/>
<Spinner android:id="@+id/mySpinnerClickSpinner"
android:layout_width="fill_parent"
android:layout_height="wrap_content"/>
</LinearLayout>
**package** com.bruce;
**import** com.bruce.OnClickListener.ItemClickListenerImpl;
**import** com.bruce.OnClickListener.ItemSelectedListenerImpl;
**import** android.app.Activity;
**import** android.os.Bundle;
**import** android.widget.ArrayAdapter;
**import** android.widget.Spinner;
**import** android.widget.TextView;
**publicclass** mySpinnerClick **extends** Activity{
**private** TextView tv;
**private** Spinner spinner;
**private** ArrayAdapter<String> adapterColor=**null**;
**private** String [] colorArray={"红","黄","蓝"};
**public** TextView getTv() {
**return**tv;
}
**publicvoid** setTv(TextView tv) {
**this**.tv = tv;
}
**public** Spinner getSpinner() {
**return**spinner;
}
**publicvoid** setSpinner(Spinner spinner) {
**this**.spinner = spinner;
}
**protectedvoid** onCreate(Bundle savedInstanceState) {
**super**.onCreate(savedInstanceState);
**super**.setContentView(R.layout.*myspinnerclick*);
**this**.tv=(TextView)**super**.findViewById(R.id.*mySpinnerClickTV*);
**this**.spinner=(Spinner)**super**.findViewById(R.id.*mySpinnerClickSpinner*);
**this**.adapterColor=**new** ArrayAdapter<String>(**this**,android.R.layout.*simple_spinner_item*,colorArray);
**this**.adapterColor.setDropDownViewResource(android.R.layout.*simple_spinner_dropdown_item*);
**this**.spinner.setAdapter(adapterColor);
**this**.spinner.setPrompt("您选择的颜色是:");
**this**.spinner.setOnItemSelectedListener(**new** ItemSelectedListenerImpl());
}
}
在下拉列表中主要装载数据的是ArrayAdapter 这个组件里面有很多方法在这里主要用了其中的2个
**this**.adapterColor=**new** ArrayAdapter<String>(**this**,android.R.layout.*simple_spinner_item*,colorArray);
创建一个ArrayAdapter 并把数据和其放在了一起android.R.layout.*simple_spinner_item是怎么在主键中展现*
**his**.adapterColor.setDropDownViewResource(android.R.layout.*simple_spinner_dropdown_item*);打开下拉列表是用什么的方式
spinner其自身也有很多的方法比如设置下拉列表头**this**.spinner.setPrompt("您选择的颜色是:");
其选择事件是**this**.spinner.setOnItemSelectedListener(**new** ItemSelectedListenerImpl())
ItemSelectedListenerImpl是实现了OnItemSelectedListener接口要实现2个方法
**publicvoid**onItemSelected(AdapterView<?> parent, View view, **int** position,
**long** id) {
System.*out*.println(parent.getId());
System.*out*.println(view.getId());
System.*out*.println(R.id.*mySpinnerClickSpinner*);
System.*out*.println(view.getId() == R.id.*mySpinnerClickSpinner*);
**if**(parent.getId()==R.id.*mySpinnerClickSpinner*){
String value = parent.getItemAtPosition(position).toString();
mySpinnerClick my1 = (mySpinnerClick) view.getContext();
my1.getTv().setText("您选择的颜色是：" + value);
}
**publicvoid** onNothingSelected(AdapterView<?> arg0) {
// **TODO** Auto-generated method stub
}
onItemSelected有4个参数，parent当前下拉列表，view当前视图，position下拉列表id，id选择的下拉列表值
onItemSelected是对下拉列表操作的时候做的事
onNothingSelected 不做任何操作的时候做的事
