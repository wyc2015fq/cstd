# Android studio (单元测试) - DEVELOPER - CSDN博客





2017年04月22日 13:17:41[学术袁](https://me.csdn.net/u012827205)阅读数：332
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)









此博客内容，仅供参考！

我的版本 Android studio 2.2.1；先看一下代码结构：

![](https://img-blog.csdn.net/20170422125907493)


再欣赏一下当前程序的build.gradle(Moudle:app)：<gradle 未做任何处理，原始的>

```java
apply plugin: 'com.android.application'

android {
    compileSdkVersion 25
    buildToolsVersion "25.0.2"
    defaultConfig {
        applicationId "com.junhua.junit.junitapplication"
        minSdkVersion 19
        targetSdkVersion 25
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
    compile fileTree(dir: 'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
    compile 'com.android.support:appcompat-v7:25.3.0'
    testCompile 'junit:junit:4.12'

    //FloatingActionButton(悬浮按钮)
    compile 'com.android.support:design:25.0.0'
}
```

然后是普通的Java单元测试<注意代码格式>：

```java
public class Calculator {
    public int add(int one, int another) {
        // 为了简单起见，暂不考虑溢出等情况。
        return one + another;
    }

    public int multiply(int one, int another) {
        // 为了简单起见，暂不考虑溢出等情况。
        return one * another;
    }
}
```

```java
public class UnitTest {
    @Test
    public void testAdd() throws Exception {
        Calculator calculator = new Calculator();
        int sum = calculator.add(1, 2);
        Assert.assertEquals(3, sum);
    }

    @Test
    public void testMultiply() throws Exception {
        Calculator calculator = new Calculator();
        int product = calculator.multiply(2, 4);
        Assert.assertEquals(8, product);
    }
}
```
![](https://img-blog.csdn.net/20170422130424012)


Android Espresso 测试：<注意代码格式>



```java
@RunWith(AndroidJUnit4.class)
@LargeTest
public class MainActivityInstrumentationTest {
    private static final String STRING_TO_BE_TYPED = "A gentle kiss, has touched my heart";

    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule<>(
            MainActivity.class);


    @Test
    public void goTest(){

        //获取Edittext 并输入字符串
        onView(withId(R.id.edttext)).perform(typeText(STRING_TO_BE_TYPED), closeSoftKeyboard()); //line 1


        //获取button并模拟点击
//        onView(withText("Say hello!")).perform(click()); //line 2
        onView(withId(R.id.btn)).perform(click());

        //获取floatbutton 并模拟点击弹出土司
        onView(withId(R.id.floatingActionButton)).perform(click());

        //比较应用中textView与期望字符自否匹配
        String expectedText = STRING_TO_BE_TYPED +"\n"+ "歌名－月亮代表我的心";
        onView(withId(R.id.textshow)).check(matches(withText(expectedText))); //line 3
    }
}
```

![](https://img-blog.csdn.net/20170422130700670)

效果是这样的：

![](https://img-blog.csdn.net/20170422131554037)







