# ReactiveX 学习笔记（20）使用 RxJava + RxBinding 进行 GUI 编程 - zwvista - 博客园
## [ReactiveX 学习笔记（20）使用 RxJava + RxBinding 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9545710.html)
### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。
### 创建工程
打开 Android Studio，File / New / Project...
在 New Project 向导的第1页，Application Name 填上 RxExample，选 Include Kotlin support。
在向导的第4页点击 Finish 按钮创建工程
build.gradle 内容如下
```
apply plugin: 'com.android.application'
apply plugin: 'kotlin-android'
apply plugin: 'kotlin-android-extensions'
android {
    compileSdkVersion 28
    defaultConfig {
        applicationId "com.zwstudio.rxexample"
        minSdkVersion 15
        targetSdkVersion 28
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
}
dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
    implementation"org.jetbrains.kotlin:kotlin-stdlib-jdk7:$kotlin_version"
    implementation 'com.android.support:appcompat-v7:28.0.0-rc01'
    implementation 'com.android.support.constraint:constraint-layout:1.1.2'
    testImplementation 'junit:junit:4.12'
    androidTestImplementation 'com.android.support.test:runner:1.0.2'
    androidTestImplementation 'com.android.support.test.espresso:espresso-core:3.0.2'
}
```
在 android 部分加上 buildToolsVersion
```
android {
    compileSdkVersion 28
    buildToolsVersion '28.0.2'
// ...
}
```
在 dependencies 部分加上对4个包的引用
RxAndroid, RxJava, RxKotlin, RxBinding
```
dependencies {
// ...
    implementation 'io.reactivex.rxjava2:rxandroid:2.0.2'
    implementation 'io.reactivex.rxjava2:rxjava:2.1.16'
    implementation 'io.reactivex.rxjava2:rxkotlin:2.2.0'
    implementation 'com.jakewharton.rxbinding2:rxbinding-kotlin:2.1.1'
}
```
### 配置 UI
打开 activity_main.xml
将以下"Hello World!"部分
```
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="Hello World!"
    app:layout_constraintBottom_toBottomOf="parent"
    app:layout_constraintLeft_toLeftOf="parent"
    app:layout_constraintRight_toRightOf="parent"
    app:layout_constraintTop_toTopOf="parent" />
```
改为
```
<GridLayout
    android:layout_width="120dp"
    android:layout_height="wrap_content"
    android:orientation="vertical"
    app:layout_constraintLeft_toLeftOf="parent"
    app:layout_constraintRight_toRightOf="parent"
    app:layout_constraintTop_toTopOf="parent"
    android:rowCount="5"
    android:columnCount="2"
    >
    <EditText
        android:id="@+id/etNumber1"
        android:layout_width="80dp"
        android:layout_height="wrap_content"
        android:gravity="end"
        android:layout_row="0"
        android:layout_column="1"
        android:inputType="number"
        android:hint="0"
        android:text="1" />
    <EditText
        android:id="@+id/etNumber2"
        android:layout_width="80dp"
        android:layout_height="wrap_content"
        android:gravity="end"
        android:layout_row="1"
        android:layout_column="1"
        android:inputType="number"
        android:hint="0"
        android:text="2" />
    <TextView
        android:id="@+id/tvAdd"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_row="2"
        android:layout_column="0"
        android:gravity="end"
        android:text="+"
        android:textAppearance="@style/TextAppearance.AppCompat.Large" />
    <EditText
        android:id="@+id/etNumber3"
        android:layout_width="80dp"
        android:layout_height="wrap_content"
        android:layout_row="2"
        android:layout_column="1"
        android:gravity="end"
        android:inputType="number"
        android:hint="0"
        android:text="3" />
    <TextView
        android:id="@+id/tvResult"
        android:layout_row="3"
        android:layout_column="1"
        android:layout_width="76dp"
        android:gravity="end"
        android:textAppearance="@style/TextAppearance.AppCompat.Medium"
        android:text="16"  />
</GridLayout>
```
### 控件变量
在 MainActivity 类中的 onCreate 方法中添加以下代码
```
val etNumber1 = findViewById<EditText>(R.id.etNumber1)
val etNumber2 = findViewById<EditText>(R.id.etNumber2)
val etNumber3 = findViewById<EditText>(R.id.etNumber3)
val tvResult = findViewById<TextView>(R.id.tvResult)
```
### 不使用 RxJava 的传统解决方案
在 MainActivity 类中的 onCreate 方法中添加以下代码
```
val textWatcher = object : TextWatcher {
    override fun afterTextChanged(s: Editable?) {
    }
    override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
    }
    override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
        val num1 = etNumber1.text.toString().toIntOrNull() ?: 0
        val num2 = etNumber2.text.toString().toIntOrNull() ?: 0
        val num3 = etNumber3.text.toString().toIntOrNull() ?: 0
        tvResult.text = (num1 + num2 + num3).toString()
    }
}
etNumber1.addTextChangedListener(textWatcher)
etNumber2.addTextChangedListener(textWatcher)
etNumber3.addTextChangedListener(textWatcher)
etNumber1.text = etNumber1.text
```
### 使用 RxJava 的解决方案
在 MainActivity 类中的 onCreate 方法中添加以下代码
```
Observables.combineLatest(RxTextView.textChanges(etNumber1), RxTextView.textChanges(etNumber2), RxTextView.textChanges(etNumber3))
    {s1, s2, s3 -> ((s1.toString().toIntOrNull() ?: 0) + (s2.toString().toIntOrNull() ?: 0) + (s3.toString().toIntOrNull() ?: 0)).toString()}
    .subscribeOn(Schedulers.io())
    .observeOn(AndroidSchedulers.mainThread())
    .subscribe(RxTextView.text(tvResult))
```
