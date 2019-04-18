# 有关Kotlin属性代理你需要知道的一切 - weixin_33985507的博客 - CSDN博客
2018年09月22日 13:38:53[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
**简述:**
今天继续Kotlin原创系列的第十一讲，一起来揭开Kotlin属性代理的漂亮外衣。属性代理可以说是Kotlin独有的强大的功能之一,特别是对于框架开发的小伙伴来说非常有用，因为会经常涉及到更改存储和修改属性的方式操作，例如Kotlin中的SQL框架[Exposed](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FJetBrains%2FExposed)源码就大量使用了属性代理。相信你已经在代码也使用了诸如Delegates.observable()、Delegates.notNull()、Delegates.vetoable()或者自定义的属性代理。也许你还停留用的阶段或者对它还有点陌生，不用担心这篇文章将会基本上解决你所有的疑惑。废话不多说，直接来看一波章节导图:
### 一、属性代理的基本定义
- 1、基本定义
属性代理是借助于代理设计模式，把这个模式应用于一个属性时，它可以将访问器的逻辑代理给一个辅助对象。
可以简单理解为属性的setter、getter访问器内部实现是交给一个代理对象来实现，相当于使用一个代理对象来替换了原来简单属性字段读写过程，而暴露外部属性操作还是不变的，照样是属性赋值和读取，只是setter、getter内部具体实现变了。
- 2、基本语法格式
```
class Student{
    var name: String by Delegate()
}
class Delegate{
    operator fun <T> getValue(thisRef: Any?, property: KProperty<*>): T{
        ...
    }
    operator fun <T> setValue(thisRef: Any?, property: KProperty<*>, value: T){
        ...
    }
}
复制代码
```
属性name将它访问器的逻辑委托给了Delegate对象，通过by关键字对表达式Delegate()求值获取这个对象。任何符合属性代理规则都可以使用by关键字。属性代理类必须要遵循getValue()，setValue()方法约定,getValue、setValue方法可以是普通方法也可以是扩展方法，并且是方法是支持运算符重载。如果是val修饰的属性只需要具备getValue()方法即可。
属性代理基本流程就是代理类中的getValue()方法包含属性getter访问器的逻辑实现，setValue()方法包含了属性setter访问器的逻辑实现。当属性name执行赋值操作时，会触发属性setter访问器，然后在setter访问器内部调用delegate对象的setValue()方法；执行读取属性name操作时，会在getter访问器中调用delegate对象的getValue方法.
- 3、by关键字
by关键字实际上就是一个属性代理运算符重载的符号，任何一个具备属性代理规则的类，都可以使用by关键字对属性进行代理。
### 二、常见属性代理基本使用
属性代理是Kotlin独有的特性，我们自己去自定义属性代理，当然Kotlin还提供了几种常见的属性代理实现。例如：Delegates.notNull(), Delegates.observable(), Delegates.vetoable()
- 1、Delegates.notNull()的基本使用
Delegate.notNull()代理主要用于可以不在构造器初始化时候初始化而是可以延迟到之后再初始化这个var修饰的属性,它和lateinit功能类似，但是也有一些不同，不过它们都需要注意的一点是属性的生命周期，开发者要做到可控，也就是一定要确保属性初始化是在属性使用之前，否则会抛出一个IllegalStateException.
```
package com.mikyou.kotlin.delegate
import kotlin.properties.Delegates
class Teacher {
    var name: String by Delegates.notNull()
}
fun main(args: Array<String>) {
    val teacher = Teacher().apply { name = "Mikyou" }
    println(teacher.name)
}
复制代码
```
可能有的人并没有看到notNull()有什么大的用处，先说下大背景吧就会明白它的用处在哪了？
**大背景:** 在Kotlin开发中与Java不同的是在定义和声明属性时必须要做好初始化工作，否则编译器会提示报错的，不像Java只要定义就OK了，管你是否初始化呢。我解释下这也是Kotlin优于Java地方之一，没错就是空类型安全，就是Kotlin在写代码时就让你明确一个属性是否初始化，不至于把这样的不明确定义抛到后面运行时。如果在Java你忘记了初始化，那么恭喜你在运行时你就会拿到空指针异常。
**问题来了:** 大背景说完了那么问题也就来了，相比Java，Kotlin属性定义时多出了额外的属性初始化的工作。但是可能某个属性的值在开始定义的时候你并不知道，而是需要执行到后面的逻辑才能拿到。这时候解决方式大概有这么几种：
**方式A: 开始初始化的时给属性赋值个默认值**
**方式B: 使用Delegates.notNull()属性代理**
**方式C: 使用lateinit修饰属性**
以上三种方式有局限性，方式A就是很暴力直接赋默认值，对于基本类型还可以，但是对于引用类型的属性，赋值一个默认引用类型对象就感觉不太合适了。方式B适用于基本数据类型和引用类型，但是存在属性初始化必须在属性使用之前为前提条件。方式C仅仅适用于引用类型，但是也存在属性初始化必须在属性使用之前为前提条件。
**优缺点分析:**
|属性使用方式|优点|缺点|
|----|----|----|
|方式A(初始化赋默认值)|使用简单，不存在属性初始化必须在属性使用之前的问题|仅仅适用于基本数据类型|
|方式B(Delegates.notNull()属性代理)|适用于基本数据类型和引用类型|1、存在属性初始化必须在属性使用之前的问题；2、不支持外部注入工具将它直接注入到Java字段中|
|方式C(lateinit修饰属性)|仅适用于引用类型|1、存在属性初始化必须在属性使用之前的问题；2、不支持基本数据类型|
**使用建议:** 如果能对属性生命周期做很好把控的话，且不存在注入到外部字段需求，建议使用方式B；此外还有一个不错建议就是方式A+方式C组合，或者方式A+方式B组合。具体看实际场景需求。
- 2、Delegates.observable()的基本使用
Delegates.observable()主要用于监控属性值发生变更，类似于一个观察者。当属性值被修改后会往外部抛出一个变更的回调。它需要传入两个参数，一个是initValue初始化的值，另一个就是回调lamba, 回调出property, oldValue, newValue三个参数。
```
package com.mikyou.kotlin.delegate
import kotlin.properties.Delegates
class Person{
    var address: String by Delegates.observable(initialValue = "NanJing", onChange = {property, oldValue, newValue ->
        println("property: ${property.name}  oldValue: $oldValue  newValue: $newValue")
    })
}
fun main(args: Array<String>) {
    val person = Person().apply { address = "ShangHai" }
    person.address = "BeiJing"
    person.address = "ShenZhen"
    person.address = "GuangZhou"
}
复制代码
```
运行结果:
```bash
property: address  oldValue: NanJing  newValue: ShangHai
property: address  oldValue: ShangHai  newValue: BeiJing
property: address  oldValue: BeiJing  newValue: ShenZhen
property: address  oldValue: ShenZhen  newValue: GuangZhou
Process finished with exit code 0
复制代码
```
- 3、Delegates.vetoable()的基本使用
Delegates.vetoable()代理主要用于监控属性值发生变更，类似于一个观察者，当属性值被修改后会往外部抛出一个变更的回调。它需要传入两个参数，一个是initValue初始化的值，另一个就是回调lamba, 回调出property, oldValue, newValue三个参数。与observable不同的是这个回调会返回一个Boolean值，来决定此次属性值是否执行修改。
```
package com.mikyou.kotlin.delegate
import kotlin.properties.Delegates
class Person{
    var address: String by Delegates.vetoable(initialValue = "NanJing", onChange = {property, oldValue, newValue ->
        println("property: ${property.name}  oldValue: $oldValue  newValue: $newValue")
        return@vetoable newValue == "BeiJing"
    })
}
fun main(args: Array<String>) {
    val person = Person().apply { address = "NanJing" }
    person.address = "BeiJing"
    person.address = "ShangHai"
    person.address = "GuangZhou"
    println("address is ${person.address}")
}
复制代码
```
### 三、常见属性代理的源码分析
以上我们介绍了常见的属性代理基本使用，如果仅仅停留在使用的阶段，确实有点low了, 那么让我们一起先来揭开它们的第一层外衣。先来看波Kotlin标准库源码中常见的属性代理包结构。
- 
1、源码包结构
- 
2、关系类图
**Delegates:** 是一个代理单例对象，里面有notNull、observable、vetoable静态方法，每个方法返回不同的类型代理对象
**NotNullVar:** notNull方法返回代理对象的类
**ObserableProperty:** observable、vetoable方法返回代理对象的类
**ReadOnlyProperty:** 只读属性代理对象的通用接口
**ReadWriteProperty:** 读写属性代理对象的通用接口
- 3、Delegates.notNull()源码分析
notNull()首先是一个方法，返回的是一个NotNullVar属性代理实例；那么它处理核心逻辑就是NotNullVar内部的setValue和getValue方法，一起来瞅一眼。
```
public override fun getValue(thisRef: Any?, property: KProperty<*>): T {
        return value ?: throw IllegalStateException("Property ${property.name} should be initialized before get.")
    }
    public override fun setValue(thisRef: Any?, property: KProperty<*>, value: T) {
        this.value = value
    }
复制代码
```
通过源码可以看到一旦getValue中的value是为null,那么就会抛出一个IllegalStateException，也就是在使用该属性之前没有做初始化。实际上可以理解在访问器getter加了一层判空的代理实现。
- 4、Delegates.observable()源码分析
observable()是一个方法，返回的是一个ObservableProperty属性代理实例；那它是怎么做到在属性值发生变化通知到外部的呢，其实很简单，首先在内部保留一个oldValue用于存储上一次的值，然后就在ObservableProperty类setValue方法执行真正赋值之后再向外部抛出了一个afterChange的回调，并且把oldValue,newValue,property回调到外部，最终利用onChange方法回调到最外层。
```
public override fun setValue(thisRef: Any?, property: KProperty<*>, value: T) {
        val oldValue = this.value
        if (!beforeChange(property, oldValue, value)) {
            return
        }
        this.value = value
        afterChange(property, oldValue, value)
    }
 public inline fun <T> observable(initialValue: T, crossinline onChange: (property: KProperty<*>, oldValue: T, newValue: T) -> Unit):
        ReadWriteProperty<Any?, T> = object : ObservableProperty<T>(initialValue) {
            override fun afterChange(property: KProperty<*>, oldValue: T, newValue: T) = onChange(property, oldValue, newValue)
 }    
复制代码
```
- 5、Delegates.vetoable()源码分析
vetoable()是一个方法，返回的是一个ObservableProperty属性代理实例；通过上面源码就可以发现，在setValue方法中执行真正赋值之前，会有一个判断逻辑，根据beforeChange回调方法返回的Boolean决定是否继续执行下面的真正赋值操作。如果beforChange()返回false就终止此次赋值，那么observable也不能得到回调，如果返回true则会继续此次赋值操作，并执行observable的回调。
### 四、属性代理背后的原理和源码反编译分析
如果说第三节是揭开属性代理第一层外衣，那么第四节将是揭开最后一层外衣了，你会看到属性代理真正背后的原理，看完你会发现其实挺简单的。不多说先上一个简单例子
```
class Teacher {
    var name: String by Delegates.notNull()
    var age: Int by Delegates.notNull()
}
复制代码
```
实际上，以上那行代码是经历了两个步骤:
```
class Teacher {
    private val delegateString: ReadWriteProperty<Teacher, String> = Delegates.notNull()
    private val delegateInt: ReadWriteProperty<Teacher, Int> = Delegates.notNull()
    var name: String by delegateString
    var age: Int by delegateInt
}
复制代码
```
Kotlin反编译后Java源码
```java
public final class Teacher {
   // $FF: synthetic field
   //关键点一
   static final KProperty[] $$delegatedProperties = new KProperty[]{(KProperty)Reflection.mutableProperty1(new MutablePropertyReference1Impl(Reflection.getOrCreateKotlinClass(Teacher.class), "name", "getName()Ljava/lang/String;")), (KProperty)Reflection.mutableProperty1(new MutablePropertyReference1Impl(Reflection.getOrCreateKotlinClass(Teacher.class), "age", "getAge()I"))};
   //关键点二
   @NotNull
   private final ReadWriteProperty name$delegate;
   @NotNull
   private final ReadWriteProperty age$delegate;
   //关键点三
   @NotNull
   public final String getName() {
      return (String)this.name$delegate.getValue(this, $$delegatedProperties[0]);
   }
   public final void setName(@NotNull String var1) {
      Intrinsics.checkParameterIsNotNull(var1, "<set-?>");
      this.name$delegate.setValue(this, $$delegatedProperties[0], var1);
   }
   public final int getAge() {
      return ((Number)this.age$delegate.getValue(this, $$delegatedProperties[1])).intValue();
   }
   public final void setAge(int var1) {
      this.age$delegate.setValue(this, $$delegatedProperties[1], var1);
   }
   public Teacher() {
      this.name$delegate = Delegates.INSTANCE.notNull();
      this.age$delegate = Delegates.INSTANCE.notNull();
   }
}
复制代码
```
分析过程:
- 1、首先, Teacher类的name和age属性会自动生成对应的setter,getter方法，并且会自动生成对应的name$delegate、age$delegate委托对象，如代码中标识的关键点二。
- 2、然后，$$delegatedProperties的KProperty数组中会保存通过Kotlin反射出当前Teacher类中的中name，age属性，反射出来每个属性单独对应保存在KProperty数组中。
- 2、然后，在对应属性setter,getter方法中是把具体的实现委托给对应的name$delegate、age$delegate对象的setValue、getValue方法来实现的，如代码中标识的关键点三。
- 3、最后在delegate对象中的setValue和getValue方法中的传入对应反射出来的属性以及相应的值。
### 五、自己动手实现属性代理
有以上的介绍，自己写个自定义的属性代理应该很简单了吧。实现一个简单的属性代理最基本架子就是setValue,getValue方法且无需实现任何的接口。
在Android中SharedPreferences实际上就是个很好场景，因为它涉及到了属性存储和读取。自定义属性代理实现Android中SharedPreferences可以直接实现自带的ReadWriteProperty接口，当然也可以自己去写一个类然后去定义相应的setValue方法和getValue方法。
```
class PreferenceDelegate<T>(private val context: Context, private val name: String, private val default: T, private val prefName: String = "default")
	: ReadWriteProperty<Any?, T> {
	private val prefs: SharedPreferences by lazy {
		context.getSharedPreferences(prefName, Context.MODE_PRIVATE)
	}
	override fun getValue(thisRef: Any?, property: KProperty<*>): T {
        println("setValue from delegate")
        return getPreference(key = name)
	}
	override fun setValue(thisRef: Any?, property: KProperty<*>, value: T) {
        println("setValue from delegate")
		putPreference(key = name, value = value)
	}
	private fun getPreference(key: String): T {
		return when (default) {
			is String -> prefs.getString(key, default)
			is Long -> prefs.getLong(key, default)
			is Boolean -> prefs.getBoolean(key, default)
			is Float -> prefs.getFloat(key, default)
			is Int -> prefs.getInt(key, default)
			else -> throw IllegalArgumentException("Unknown Type.")
		} as T
	}
	private fun putPreference(key: String, value: T) = with(prefs.edit()) {
		when (value) {
			is String -> putString(key, value)
			is Long -> putLong(key, value)
			is Boolean -> putBoolean(key, value)
			is Float -> putFloat(key, value)
			is Int -> putInt(key, value)
			else -> throw IllegalArgumentException("Unknown Type.")
		}
	}.apply()
}
复制代码
```
### 六、结语
到这里属性代理的内容就结束了，有没有觉得Kotlin语言糖设计还是很巧妙的。虽然很多人抵触语法糖，但不可否认的是它给我们开发在效率上带来了很大的提升。有时候我们更多地是需要透过语法糖外衣，看到其背后的原理，弄清整个语法糖设计思路和技巧，以一个全局眼光去看待它，就会觉得它也就那么回事。最后，感谢一波bennyHuo大佬，我是先看到他sharedPreferences的属性扩展例子，感觉很不错，然后决定去深入探究一下属性扩展，这下应该对Kotlin属性扩展有了比较深的认识了。
**欢迎关注Kotlin开发者联盟，这里有最新Kotlin技术文章，每周会不定期翻译一篇Kotlin国外技术文章。如果你也喜欢Kotlin，欢迎加入我们~~~**
