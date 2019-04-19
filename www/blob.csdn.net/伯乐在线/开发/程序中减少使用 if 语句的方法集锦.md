# 程序中减少使用 if 语句的方法集锦 - 文章 - 伯乐在线
原文出处： [Joe Wright](http://code.joejag.com/2016/anti-if-the-missing-patterns.html)   译文出处：[CSDN-孙薇](http://www.iteye.com/news/31641)
大约十年前，我听说了[反if的活动](http://antiifcampaign.com/)，觉得这个概念非常荒谬。如果不用if语句，又怎么能写出有用的程序呢？这简直太荒谬了。
但之后你会开始思考：是否还记得上周你拼命想读懂的深度嵌套代码？糟透了对么？要是有办法能简化它该多好。
反if活动的网站上没给出多少实用性建议，因此在本文中，作者将会提供一系列模式，也许你会用得上。但首先我们来关注一下if语句到底造成了什么问题。
# **if语句的问题**
if语句的第一个问题在于，通常出现if语句的代码很容易越改越糟。我们试着写个新的if语句：


```
public void theProblem(boolean someCondition) {
        // SharedState
        if(someCondition) {
            // CodeBlockA
        } else {
            // CodeBlockB
        }
}
```
这时候还不算太糟，但已经存在一些问题了。在阅读这段代码时，我必须得去查看对同一个SharedState来说，CodeBlockA和CodeBlockB有什么改动。最开始这段代码很容易阅读，但随着CodeBlock越来越多，耦合越来越复杂之后，就会很难读。
上面这种CodeBlock进一步嵌套if语句与本地return的滥用情况也很常见，很难搞懂业务逻辑是选择了哪种路径。
if语句的第二个问题在于：复制时会有问题，也就是说，if语句缺失domain的概念。很容易由于在不需要的情况下，由于将内容放在一起而增加耦合性，造成代码难读难改。
而第三个问题在于：开发者必须在头脑中模拟执行实现情况——你得让自己变成一台小型电脑，从而造成脑细胞浪费。开发者的精力应当用来思考如何解决问题，而不是浪费在如何将复杂的代码分支结构编织在一起之上。
虽然想要直截了当地写出替代方案，但首先我得强调这句话：
凡事中庸而行，尤其是中庸本身
if语句通常会让代码更加复杂，但这不代表我们要完全抛弃if语句。我曾经看到过一些非常糟糕的代码，只是为了消除所有的if语句而刻意避开if语句。我们想要绕开这个误区，
下面我给出的每种模式，都会给出使用范围。
单独的if语句如果不复制到其他地方，也许是不错的句子。在复制if语句时，我们会希望预知危险的第六感起效。
在代码库之外，在与危险的外部世界交流时，我们会想要验证incoming response，并根据其作出相应的修改。但在自己的代码库中，由于有可靠的gatekeeper把关，我觉得这是个很好的机会，我们可以尝试使用简单、更为丰富与强大的替代方案来实现。
# **模式1：布尔参数（Boolean Params）**
背景： 有方法在修改行为时使用了boolean。


```
public void example() {
    FileUtils.createFile("name.txt", "file contents", false);
    FileUtils.createFile("name_temp.txt", "file contents", true);
}
public class FileUtils {
    public static void createFile(String name, String contents, boolean temporary) {
        if(temporary) {
            // save temp file
        } else {
            // save permanent file
        }
    }
}
```
问题： 在看到这段代码时，实际上你是将两个方法捆绑到一起，布尔参数的出现让你有机会在代码中定义一个概念。
适用范围： 通常看到这种情况，如果在编译时我们可以算出代码要采用哪种路径，就可以放心使用这种模式。
解决方案： 将这个方法拆分成两个新的方法，然后if就不见了。


```
public void example() {
    FileUtils.createFile("name.txt", "file contents");
    FileUtils.createTemporaryFile("name_temp.txt", "file contents");
}
public class FileUtils {
    public static void createFile(String name, String contents) {
        // save permanent file
    }
    public static void createTemporaryFile(String name, String contents) {
        // save temp file
    }
}
```
# **模式2：使用多态（Polymorphism）**
背景： 根据类型switch时。


```
public class Bird {
    private enum Species {
        EUROPEAN, AFRICAN, NORWEGIAN_BLUE;
    }
    private boolean isNailed;
    private Species type;
    public double getSpeed() {
        switch (type) {
            case EUROPEAN:
                return getBaseSpeed();
            case AFRICAN:
                return getBaseSpeed() - getLoadFactor();
            case NORWEGIAN_BLUE:
                return isNailed ? 0 : getBaseSpeed();
            default:
                return 0;
        }
    }
    private double getLoadFactor() {
        return 3;
    }
    private double getBaseSpeed() {
        return 10;
    }
}
```
问题： 在添加新的类型时，我们必须要记得更新switch语句，此外随着不同bird的概念添加进来，bird类的凝聚力越来越糟。
适用范围：根据类型做单次切换是可行的，如果switch太多，在添加新类型时如果忘记更新现有隐藏类型中的所有switch，就会导致bug出现，[8thlight](https://blog.8thlight.com/wai-lee-chin-feman/2013/08/11/anti-anti-if.html)博客关于这种情况有详细描述。
解决方案： 使用多态，添加新类型时大家都不会忘记添加相关行为。
注意：上例为了简洁只写了一个方法，但在有多个switch时更有用。


```
public abstract class Bird {
    public abstract double getSpeed();
    protected double getLoadFactor() {
        return 3;
    }
    protected double getBaseSpeed() {
        return 10;
    }
}
public class EuropeanBird extends Bird {
    public double getSpeed() {
        return getBaseSpeed();
    }
}
public class AfricanBird extends Bird {
    public double getSpeed() {
        return getBaseSpeed() - getLoadFactor();
    }
}
public class NorwegianBird extends Bird {
    private boolean isNailed;
    public double getSpeed() {
        return isNailed ? 0 : getBaseSpeed();
    }
}
```
# **模式3：NullObject/Optional**
背景： 当外部请求理解代码库的主要用途时，回答“查一下null的情况”。


```
public void example() {
    sumOf(null);
}
private int sumOf(List numbers) {
    if(numbers == null) {
        return 0;
    }
    return numbers.stream().mapToInt(i -> i).sum();
}
```
# **模式4：将内联语句（Inline statements）转为表达式**
背景： 在计算布尔表达式时，包含if语句树。


```
public boolean horrible(boolean foo, boolean bar, boolean baz) {
    if (foo) {
        if (bar) {
            return true;
        }
    }
    if (baz) {
        return true;
    } else {
        return false;
    }
}
```
问题： 这种代码会导致开发者必须用大脑来模拟计算机对方法的处理。
适用范围：很少有不适用的情况，像这样的代码可以合成一行，或者拆成不同的部分。
解决方案： 将if语句树合成单个表达式。


```
public boolean horrible(boolean foo, boolean bar, boolean baz) {
    return foo && bar || baz;
}
```
# **模式5：给出应对策略**
背景：在调用一些其他代码时，无法确保路径是成功的。


```
public class Repository {
    public String getRecord(int id) {
        return null; // cannot find the record
    }
}
public class Finder {
    public String displayRecord(Repository repository) {
        String record = repository.getRecord(123);
        if(record == null) {
            return "Not found";
        } else {
            return record;
        }
    }
}
```
问题： 这类if语句增加了处理同一个对象或者数据结构的时间，其中包含隐藏耦合——null的情况。其它对象可能会返回其他代表没有结果的magic value。
适用范围：最好将这类if语句放在一个地方，由于不会重复，我们就能将为空对象的magic value删除。
解决方案：针对被调用代码，给出应对策略。Ruby的[Hash#fetch](http://ruby-doc.org/core-2.2.0/Hash.html#method-i-fetch)就是很好的案例，[Java也用到了类似的方法](https://docs.oracle.com/javase/8/docs/api/java/util/Map.html#getOrDefault-java.lang.Object-V-)。这种模式也可以用在[删除例外](https://silkandspinach.net/2014/11/06/on-paperboys-newsagents-and-exceptions/)情况时。


```
private class Repository {
    public String getRecord(int id, String defaultValue) {
        String result = Db.getRecord(id);
        if (result != null) {
            return result;
        }
        return defaultValue;
    }
}
public class Finder {
    public String displayRecord(Repository repository) {
        return repository.getRecord(123, "Not found");
    }
}
```
# **祝探索愉快**
希望这些模式对你现在处理的问题有帮助。我在重构代码增进理解时，发现这些方法都很有用。要记得并非所有if语句都是魔鬼，不过现代编程语言还有很多功能值得我们探索并使用。
