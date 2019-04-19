# guava处理字符串与List/Map之间的转换 - 零度的博客专栏 - CSDN博客
2017年11月15日 14:00:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：3648
## Guava 是什么?
Guava是一种基于开源的Java库，其中包含谷歌正在由他们很多项目使用的很多核心库。这个库是为了方便编码，并减少编码错误。这个库提供用于集合，缓存，支持原语，并发性，常见注解，字符串处理，I/O和验证的实用方法。
## Guava的好处
- 标准化 - Guava库是由谷歌托管。
- 高效 - 可靠，快速和有效的扩展JAVA标准库
- 优化 -Guava库经过高度的优化。
函数式编程 -增加JAVA功能和处理能力。
实用程序 - 提供了经常需要在应用程序开发的许多实用程序类。
验证 -提供标准的故障安全验证机制。
最佳实践 - 强调最佳的做法。
关于guava的使用提供一个教程：http://www.yiibai.com/guava/[点击guava教程](http://www.yiibai.com/guava/)
下面我们实现如下的代码片段：guava处理字符串与List/map之间的转换。
Java代码：
```java
package org.zwc.guavatest;
import com.google.common.base.Joiner;
import com.google.common.base.Splitter;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import java.util.List;
import java.util.Map;
/**
 * Created by zhangwenchao on 2017/11/15.
 */
public class GuavaTest {
    /**
     * list转换为字符串
     */
    public void joinTest(){
        List<String> names = Lists.newArrayList("John", "Jane", "Adam", "Tom");
        String result = Joiner.on(",").join(names);
        System.out.println(result.equals( "John,Jane,Adam,Tom"));
    }
    /**
     * map转换为字符串
     */
    public void whenConvertMapToString_thenConverted() {
        Map<String, Integer> salary = Maps.newHashMap();
        salary.put("John", 1000);
        salary.put("Jane", 1500);
        String result = Joiner.on(" , ").withKeyValueSeparator(" = ")
                .join(salary);
        System.out.println(result);
    }
    /**
     * list转String，跳过null
     */
    public void whenConvertListToStringAndSkipNull_thenConverted() {
        List<String> names = Lists.newArrayList("John", null, "Jane", "Adam", "Tom");
        String result = Joiner.on(",").skipNulls().join(names);
        System.out.println(result);
        System.out.println(result.equals( "John,Jane,Adam,Tom"));
    }
    /**
     * list转String，将null变成其他值
     */
    public void whenUseForNull_thenUsed() {
        List<String> names = Lists.newArrayList("John", null, "Jane", "Adam", "Tom");
        String result = Joiner.on(",").useForNull("nameless").join(names);
        System.out.println(result);
        System.out.println(result.equals( "John,nameless,Jane,Adam,Tom"));
    }
    /**
     * String to List
     */
    public void whenCreateListFromString_thenCreated() {
        String input = "apple - banana - orange";
        List<String> result = Splitter.on("-").trimResults().splitToList(input);
        System.out.println(result);
    }
    /**
     * String to Map
     */
    public void whenCreateMapFromString_thenCreated() {
        String input = "John=first,Adam=second";
        Map<String, String> result = Splitter.on(",")
                .withKeyValueSeparator("=")
                .split(input);
        System.out.println("first= "+result.get("John"));
        System.out.println("second= "+ result.get("Adam"));
    }
    /**
     * 多个字符进行分割
     */
    public void whenSplitStringOnMultipleSeparator_thenSplit() {
        String input = "apple.banana,,orange,,.";
        List<String> result = Splitter.onPattern("[.|,]")
                .omitEmptyStrings()
                .splitToList(input);
        System.out.println(result);
    }
    /**
     * 每隔多少字符进行分割
     */
    public void whenSplitStringOnSpecificLength_thenSplit() {
        String input = "Hello world";
        List<String> result = Splitter.fixedLength(3).splitToList(input);
        System.out.println(result);
    }
    /**
     * 限制分割多少字后停止
     */
    public void whenLimitSplitting_thenLimited() {
        String input = "a,b,c,d,e";
        List<String> result = Splitter.on(",")
                .limit(4)
                .splitToList(input);
        System.out.println(4==result.size());
        System.out.println(result);
    }
    public static void main(String[] args) {
        GuavaTest guavaTest = new GuavaTest();
        guavaTest.joinTest();
        guavaTest.whenConvertMapToString_thenConverted();
        guavaTest.whenConvertListToStringAndSkipNull_thenConverted();
        guavaTest.whenUseForNull_thenUsed();
        guavaTest.whenCreateListFromString_thenCreated();
        guavaTest.whenCreateMapFromString_thenCreated();
        guavaTest.whenSplitStringOnMultipleSeparator_thenSplit();
        guavaTest.whenSplitStringOnSpecificLength_thenSplit();
        guavaTest.whenLimitSplitting_thenLimited();
    }
}
```
运行结果：
```
true
Jane = 1500 , John = 1000
John,Jane,Adam,Tom
true
John,nameless,Jane,Adam,Tom
true
[apple, banana, orange]
first= first
second= second
[apple, banana, orange]
[Hel, lo , wor, ld]
true
[a, b, c, d,e]
```
