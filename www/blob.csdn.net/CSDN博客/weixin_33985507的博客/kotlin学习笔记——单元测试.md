# kotlin学习笔记——单元测试 - weixin_33985507的博客 - CSDN博客
2017年10月13日 14:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Kotlin学习笔记系列：[http://blog.csdn.net/column/details/16696.html](https://link.jianshu.com?t=http://blog.csdn.net/column/details/16696.html)
**Unit Test**
kotlin也可以进行unit testing，如果项目中之前没有，那么需要做一些准备工作。
首先引入依赖
testCompile 'junit:junit:4.12'
这里注意不能是androidTestCompile，否则会报错Unresolved reference: xxxx
然后创建目录
在src目录下（main的同级）创建test/java目录，创建完会发现java目录的颜色自动为绿色，表示ide知道我们要使用unit testing模式。
在java目录下创建package（与项目主包名一致）
创建测试代码
在package下创建测试类编写代码即可，例如：
```
import org.junit.Test
import kotlin.test.assertTrue
class SimpleTest {
 @Test fun unitTestingWorks() {
     assertTrue(true)
 }
}
```
运行即可
**Instrumentation Test**
与unit testing一样，首先引入依赖
```
defaultConfig {
    ...
    testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
}
```
```
androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
    exclude group: 'com.android.support', module: 'support-annotations'
})
androidTestCompile ("com.android.support.test.espresso:espresso-contrib:2.2.1"){
    exclude group: 'com.android.support', module: 'appcompat'
    exclude group: 'com.android.support', module: 'support-v4'
    exclude group: 'com.android.support', module: 'support-annotations'
    exclude module: 'recyclerview-v7'
}
```
exclude去掉一些依赖，防止重复引入
（contrib这个增加了一些额外功能，比如测试recyclerview）
然后创建目录，与unit一样，只不过根目录不是test而是androidTest，其他一样。
创建测试代码
```
import android.support.test.espresso.Espresso.onView
import android.support.test.espresso.action.ViewActions.click
import android.support.test.espresso.assertion.ViewAssertions.matches
import android.support.test.espresso.contrib.RecyclerViewActions
import android.support.test.espresso.matcher.ViewMatchers.isAssignableFrom
import android.support.test.espresso.matcher.ViewMatchers.withId
import android.support.test.rule.ActivityTestRule
import android.support.v7.widget.RecyclerView
import android.widget.TextView
import org.junit.Rule
import org.junit.Test
class SimpleActivityTest {
    @get:Rule
    val activity = ActivityTestRule(MainActivity::class.java)
    @Test fun testItem(){
        onView(withId(R.id.recyclerview)).perform(RecyclerViewActions.actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))
        onView(withId(R.id.textView)).check(matches(isAssignableFrom(TextView::class.java)))
    }
}
```
testitem中第一行代码是模拟点击recyclerview的第一个item。第二行是判断id是textview的组件是否是TextView。
