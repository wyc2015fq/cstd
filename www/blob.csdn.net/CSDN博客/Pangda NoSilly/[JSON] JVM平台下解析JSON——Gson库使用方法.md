# [JSON] JVM平台下解析JSON——Gson库使用方法 - Pangda NoSilly - CSDN博客

2017年09月26日 22:31:20[hepangda](https://me.csdn.net/hepangda)阅读数：142
个人分类：[java																[scala](https://blog.csdn.net/hepangda/article/category/7196887)](https://blog.csdn.net/hepangda/article/category/7196886)


## 介绍

Gson是目前被广泛使用的一个JSON库，它由Google公司发布，完全开放源代码，主要用途为序列化Java对象为JSON字符串，或反序列化JSON字符串成Java对象。当然，虽然它由Java编写，但它可以被JVM平台下的一切语言使用，以下以Scala作为描述语言，简单的说明如何使用Gson。

## 安装Gson

Gson是一个开放源代码的项目，它的源代码可以在项目的[github仓库](https://github.com/google/gson)找到，如果你并不关心它的源代码是如何实现的，只是想简单的使用它而已，可以在[Maven Central](http://repo1.maven.org/maven2/com/google/code/gson/gson/2.8.2/)页面中，点击`gson-2.8.2.jar`，下载Gson的当前版本的jar，然后我们将该jar加入到项目依赖中即可。这里我们以Scala SBT为例，加入下面的语句即可：
`libraryDependencies += com.google.code.gson" % "gson" % "2.8.2"``
## 使用Gson

### 从类中生成字符串

这里我们使用一个简单的例子，假如我们想要将一个点的坐标序列化为字符串，形如：`{"x":3,"y":4}`，先写一个简单的例子试一下：

```
import com.google.gson_

class Point(var x: Int = 0, var y: Int = 0)

object Main {
    def main(args: Array[String]): Unit = {
        val json = new Gson()
        val pt = new Point(x = 2, y = 3)
        val str = json.toJson(pt, pt.getClass)

        println(str)
    }
}
```

得到了输出：
`{"x":2,"y":3}`
看来Gson自动的识别类中的元素并将之输出了出来，那么如果成员的访问级别是private呢？我们尝试着在Point类中添加更多的成员：

```
class Point(var x: Int = 0, var y: Int = 0) {
    private var z: Int = 0
}
```

修改了程序之后，输出发生了改变：
`{"x":2,"y":3,"z":0}`
这说明成员的访问级别并不影响Gson将他放入生成字符串中。

那如果我们不再想要这样的格式的输出，而是想要形如`{"X-Value":2,"Y-Value":3,"Z-Value":0}`的输出，难道只能选择将类成员名作修改吗？或者我们不想要将z的值输出到字符串，那该怎么办呢？Gson为我们提供了对应的工具，利用到了“注解”。要使用这个特性，我们必须在代码中加入：

```
//Expose用于支持是否可序列化、反序列化
//SerializedName用于重新“命名”
import com.google.gson.annotations.{Expose, SerializedName}
```

我们将代码修改为这样,简明起见，这里略去了z：

```
class Point() {
    @Expose(serialize = true)
    @SerializedName("X-Value")
    var x: Int = 0

    @Expose(serialize = false)
    @SerializedName("Y-Value")
    var y: Int = 0
}

object Main {
    def main(args: Array[String]): Unit = {
        val json = new Gson()
        val p1 = new Point()
        val str = json.toJson(p1, p1.getClass)

        println(str)
    }
}
```

运行程序，得到输出：
`{"X-Value":0,"Y-Value":0}`
看起来`@SerializedName`注解生效了，但`@Expose`注解似乎没有任何效果。这里的原因是我们使用了默认的`Gson`对象，采用了默认的设置，因此没有检测是否使用了`@Expose`注解，我们将`Main`类修改为这样：

```
object Main {
    def main(args: Arrag[String]): Unit = {
        val json = new GsonBuilder()
                    .excludeFieldsWithoutExposeAnnotation
                    .create
        val p1 = new Point()
        val str = json.toJson(p1, p1.getClass)

        println(str)
    }
}
```

这时再次编译运行，得到了我们想要的输出结果：
`{"X-Value":0}`
除此之外，我们还可以通过`GsonBuilder`设置更多的特性，这里不多做介绍了，可以查阅Gson的官方文档([http://www.javadoc.io/doc/com.google.code.gson/gson/2.8.2](http://www.javadoc.io/doc/com.google.code.gson/gson/2.8.2))来获得更多特性的使用方法。

### 从字符串生成类

根据上面的例子，假如我现在拿到了一串字符串`{"x":3,"y":5}`，我怎样才可以将它之中的各个元素提取出来，还原成之前的`Point`类呢？

和从类生成字符串一样，我们可以很轻易的将字符串还原成类：

```
class Point(var x: Int = 0, var y: Int = 0)

object Main {
    def main(args: Array[String]): Unit = {
        val json = new Gson()
        val str = """{"x":3,"y":4}"""
        val p: Point = json.fromJson(str, new Point().getClass)

        println(s"从字符串中获得到了类: x = ${p.x}, y = ${p.y}")
    }
}
```

运行之后，得到了输出：
`从字符串中获得到了类: x = 3, y = 4`
说明解析没有任何问题。此外，如果想要重命名或者不想从字符串中导入某个成员的话，也完全可以使用之前介绍的`@SerializedName`注解和`@Expose`注解，这里不再赘述。

