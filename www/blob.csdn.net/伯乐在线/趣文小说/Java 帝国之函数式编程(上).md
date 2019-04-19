# Java 帝国之函数式编程(上) - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [码农老刘](http://www.jobbole.com/members/onlyliuxin) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
前言：最近看了点Java 函数式编程， 写了个小故事和大家分享下心得，不妥之处，欢迎拍砖
### 1. 游行示威
Java 帝国成立20多年了，  20多年来帝国给每个程序员都提供了趁手的工具箱， 里边装着面向对象， 自动内存管理，泛型 ， 类型安全等强大的工具，
大家用这些工具开发了无数的框架和系统，都夸Java大法好。
（微信公众号“码农翻身”注： 请参考文章《[Java 一个帝国的诞生](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513103&idx=1&sn=56d832072a92e3f41f5e6796676993e0&scene=21#wechat_redirect)》 《[Java帝国之Java Bean(上)](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513115&idx=1&sn=da30cf3d3f163d478748fcdf721b6414&scene=21#wechat_redirect)》 《[Java帝国之Java Bean (下）](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513118&idx=1&sn=487fefb8fa7efd59de6f37043eb21799&scene=21#wechat_redirect)》）
可以最近出现了一点新情况， 一群函数式编程的狂热份子不停的到总部来游行， 要求帝国向LISP学习， 支持函数式编程， 他们的理由也很充分：
“很多语言都支持函数式编程了，比如 Ruby , Python…
“人家微软的C#很早就支持了…”
“同样是运行在Java 虚拟机上的语言，那些杂牌军Scala , Clojure 都有函数式编程 ,   我们的精锐，中央军Java 怎么就不支持呢？”
这样的声音虽然不是主流，但越来越多也让我不胜其烦， 我把我最得力的助手小码哥叫来商量对策：
“小码， 外边那一群狂热分子叫嚣的函数式编程到底是什么东西？”
“唉， 老大， 其实我也不知道函数式编程的确切定义， 我只知道LISP是函数式编程的鼻祖，并且像LISP这样的语言学习起来很难，和我们的面向对象完全是两个世界， 但奇怪的是很多掌握了LISP的人都无可救药的爱上了她。”
“那它肯定有什么过人之处 ！”  我说
“像C,C++，Java， 用他们编程是一种命令式的风格， 告诉计算机如何如何做， 而用LISP编程是一种声明式的风格，就像SQL一样， 只是描述我要什么， 不去具体的描述怎么去做”
“听起来很不错啊 ”   我觉得一门语言能用声明式的方式来编程， 对程序员绝对是福音啊。
“此外， 面向对象编程更多的是对数据进行抽象， 而函数式编程是对行为进行抽象， 在LISP世界里，函数是一等公民， 函数可以当做参数传递给另外一个函数，  函数也可以作为另外一个函数的返回值。”
“只用函数进行编程， 听起来不可思议啊！”
“是啊， 所以如果你学过面向对象的语言，像咱们Java,  再去学LISP, 会感到一片迷茫， 无所适从， 真气浑身乱窜， 好久才能平息下来。 ”
### 2. 强类型
“那我们能不能学一下Ruby , Python 在咱们Java 中添加一点函数式编程的特性， 至少让这些刁民， 奥不， 狂热分子安静下来？ ”
小码想了一会说： “有点难度， 因为Java 有一个特点，就是强类型的。”
“强类型？ ，这是什么意思？ “我问他
“是啊， 强类型， 简单的说无论任何变量都得有个类型， 并且不能变。”
小码哥没料到我还不知道这个概念， 开始兴致勃勃的给我普及。
”比如你可以声明一个变量
Person p = new Person();
可是如果你想让p指向另外一个对象 ：
p = new House();
我们伟大的java编译器立刻就能告诉你错误：  ‘类型不匹配， 不能把House转化成 Person’
强类型的好处是显而易见的， 我们在编译期间就可以消灭大部分的错误，并且还可以在你用IDE编程的时候做出很‘智能’的提示， 帮你自动补全代码。”
“与之相对的就是弱类型，例如在Ruby 中一个变量根本不需要声明类型：
p = Person.new
p = House.new
这是没有任何问题的， 变量p 可以指向任何对象， 但是也丧失了提前发现错误的可能性， 很多错误只能在运行时暴露出来。
def do_something(p)
p.walk(); // 假设walk只是Person独有的方法。
end
系统检查不出来你到底是传递进去一个什么类型的对象，  如果p 指向一个House,  House是不能walk的，  那在运行时就会报错： undefined method `walk’ for #<House:0x00000002f40590>”
“但是弱类型也有好处， 例如
p = Animal.new
这个Animal 也有一个walk()方法。 那在调用do-something方法的时候完全没有问题！   ”
我有点懂了： “是不是说由于Java 是强类型的， 如果想支持函数式编程， 我们也得给这些函数找个类型 ？”
小码哥说：　“是啊，不过这有点难度”
”咱们能不能重新的设计下Java 的类型系统 ？“ 我有点不甘心
“肯定不行， 我们Java 帝国为了吸引程序员，并且为了维稳，  曾经对外郑重的承诺过： 帝国要保持向后兼容性，你就是用Jdk 1.1编译的代码， 在我们jdk 8中也能运行， 要是为了函数式编程，改动太大，估计很多程序员就要抛弃我们了。”
“不管如何， 你得想想办法，在Java 中加上函数式编程， 帝国已经这么大了， 再加一点东西也没什么大不了的。”
我给了小码哥三天时间。
### 3. Lambda 表达式 和 类型推断
过了两天，小码哥就兴冲冲的来找我了： “老大，你看看，我设计出了java 中的Lambda表达式”
我接过纸一看， 上面写着：


```
() -> System.out.println("Hello Lambda");
s-> s.toUpperCase();
(x,y) -> x +y ;
```
“这看起来像是个函数啊？ 还起个这么时髦的名字Lambda !”
“对，Lambda可以说就是**匿名函数**， 我定义的语法是这样的， 箭头(->) 左边是**函数的参数列表**， 右边是**方法体**”
我看了看， 确实是， 第一个表达式 没有参数，  函数体是打印”Hello Lambda”
第二个函数有一个参数 ,  函数体似乎是把这个参数变成大写字符。
第三个函数有两个参数， 函数体是把两个参数相加
“小码，既然它们是函数， 按照函数式编程风格， 就能把这些函数当做参数传递给另外一些函数了？ ”
“那是自然， 我给你举个例子“  小码哥说
例如我们有这么个接口：


```
public interface StringFuction{
public String apply(String s);
}
```
还有这么个函数：


```
public String  run (StringFuction f){
return f.apply("Hello World");
}
```
现在就可以把“**匿名函数**”当做参数传递给另外一个函数了
run (s -> s.toUpperCase()) ;
返回值就是大写字符串： HELLO WORLD
如果我们传递进去一个别的Lambda 表达式：
run (s -> s.toLowerCase()) ;
返回值就是小写字符串：　hello world
”我有个问题， 上次你说过咱们Java 都是强类型的， 所有的东西都得有类型， 那这个Lambda表达式s -> s.toUpperCase()  的类型是什么？ ”
小码哥说： “唉， 为了这个问题， 我可是费劲了， 我搞了个‘**类型推断**’， 由编译器智能的推断出s -> s.toUpperCase()的类型， 其实在上面的例子中， 这个Lambda表达式的类型就是StringFunction!”
“注意”小码哥接着说， “这个StringFunction 的apply方法接收一个字符串参数， 然后返回另外一个字符串的值。
所以当我们用s -> s.toUpperCase() 去调用的时候， 就会知道 s 就是apply的参数， s.toUpperCase()其实就是这个方法的实现了， 这个方法的返回值也是String ”
“奥，我明白了， 其实就是我们之前的匿名类嘛，我们完全可以这么写“


```
run(new StringFuction(){
public String apply(String s) {
return s.toUpperCase();
}
});
```
“对的，本质就是这样， 对于Lambda表达式，我们只是会做更多的类型推断，如果你这么调用 : “
run(s -> s.length())；
“那编译就不通过了， 因为这个匿名函数的返回值是int ,  它不符合接口(StringFunction）的方法(apply)的语义， 接受一个字符串, 然后返回一个字符串。 ”
“那个函数接口的方法名称（apply）重要吗？” 我接着问
“不重要， **关键是它的输入类型和返回值**， 至于名称，尽可能的好一点吧”
“唉，小码，我有点懂了， 为了维护Java 的强类型， 还得定义一个函数接口， 然后编译器会把这些匿名函数（Lambda表达式） 和这个接口做匹配，  但是你让程序员们定义函数接口， 太麻烦了吧？ ”
小码哥不慌不忙 ： “这个问题我考虑过了，我已经对咱的JDK做了增强， 特别引入了一个叫java.util.function 的包， 里边现成的函数接口足够大部分人用了”
说着， 小码哥给我展示了一个文档：
(1)   Function函数接口： 传入一个类型为T的参数， 返回一个类型为R 的参数


```
public interface Function<T,R>{
R apply(T t);
......
}
```
(2)  Predicate<T> 函数接口  ：传入一个类型为Ｔ　的参数，　返回boolean


```
public interface Predicate<T> {
boolean test(T t);
......
}
```
(3)  Consumer<T>函数接口  ： 传入一个类型为T的参数，没有返回值， 执行你自定义的操作


```
public interface Consumer<T> {
void accept(T t);
......
}
```
例如
s -> s.length()  就可以匹配 (1)
x -> x>5   就可以匹配 (2)
s ->  System.out.println(s)  就可以匹配 (3)
我不仅暗暗赞叹：还是抽象大法好。
“小码，我明白你的苦心， 这些确实都是高度的抽象啊”
“老大， 这是没办法的事啊， 想想我们java 帝国那么多工具，框架， 咱必须得保持向后兼容性啊。 所以一定得坚持强类型， 即使是匿名函数， 也得给它找个类型，即函数接口 ！“
“但是有了这么东西还是不够， 似乎没有体现出像SQL那样‘声明式’编程的好处啊？ ”
“确实不够， 我们得把类库好好改改， 真正体现函数式编程的特性， 我打算引入一个叫做Stream的概念。”
(未完待续)
**声明：原创文章，未经授权，禁止转载**
你看到的只是冰山一角， 更多精彩文章，尽在“码农翻身” 微信公共号
“码农翻身”公众号由工作15年的前IBM架构师创建，分享编程和职场的经验教训。
- [我是一个线程](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=416915373&idx=1&sn=f80a13b099237534a3ef777d511d831a&scene=21#wechat_redirect)[我是一个Java class](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=416976590&idx=1&sn=22823ada76d8cfd26a43e8d3a7b7a60e&scene=21#wechat_redirect)
- [Javascript: 一个屌丝的逆袭](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513059&idx=1&sn=a2eaf97d9e3000d15a33681d1b720463&scene=21#wechat_redirect)
- [Java ： 一个帝国的诞生](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513103&idx=1&sn=56d832072a92e3f41f5e6796676993e0&scene=21#wechat_redirect)
- [Basic : 一个老兵的自述](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513109&idx=1&sn=9ff4cee221d4a4d4124c3a316612c53d&scene=21#wechat_redirect)
- [小王的架构师之路](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513091&idx=1&sn=1a1d245416b274ceea9243ff3e3ebb16&scene=21#wechat_redirect)
- [程序员在工作中必备的能力](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513097&idx=1&sn=3801a4159fdab84571d7049ec1c6bfb8&scene=21#wechat_redirect)
- [码农需要知道的潜规则](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513106&idx=1&sn=ec36fe633c0b782deade3b695576c8ee&scene=21#wechat_redirect)
- [TCP/IP 之 大明王朝的邮差](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513094&idx=1&sn=a2accfc41107ac08d74ec3317995955e&scene=21#wechat_redirect)
- [CPU 阿甘](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513017&idx=1&sn=5550ee714abd36d0b580713f673e670b&scene=21#wechat_redirect)
- [IE为什么把Chrome和火狐打伤了](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=417017336&idx=1&sn=b3b5691b136457fe1c0a27d4b6548a54&scene=21#wechat_redirect)
- [Node.js :我只需要一个店小二](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513044&idx=1&sn=9b8526e9d641b970ee5ddac02dae3c57&scene=21#wechat_redirect)
- [假如我是计算机系老师](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=413878948&idx=1&sn=70986470ce1957e302680da0ad71f22b&scene=21#wechat_redirect)
- [假如时光倒流，我会这么学Java](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=415513252&idx=1&sn=1c1211e23c507c34f9befbf5282a85c8&scene=21#wechat_redirect)
- [学会编程，而不是学会Java](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=417056471&idx=1&sn=0ef5cbe0e336f34fc267f209b20d5379&scene=21#wechat_redirect)
- [15年编程生涯，资深架构师总结的7条经验](http://mp.weixin.qq.com/s?__biz=MzAxOTc0NzExNg==&mid=2665513065&idx=1&sn=b82b7f98553d8a8e3c4256c8f4ecf742&scene=21#wechat_redirect)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/b5ab6960764500d8ba0d3b92952b9a29.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/2bd72445d0c2a44f906dc114002733eb.jpg)
