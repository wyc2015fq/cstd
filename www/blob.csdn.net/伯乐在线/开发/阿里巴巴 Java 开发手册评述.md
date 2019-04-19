# 阿里巴巴 Java 开发手册评述 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [ThoughtWorks](http://www.jobbole.com/members/thoughtworkschina) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
注：本文基于阿里巴巴技术手册的1.0.2版本编写
2016年底，阿里巴巴公开了其在内部使用的Java编程规范。随后进行了几次版本修订，笔者当时看到的版本为v1.0.2版。下载地址可以在其官方社区——云栖社区找到。
笔者作为一名有数年工作经验的Java程序员，仔细研读了这份手册，觉得是一份不可多得的好材料。正如阿里巴巴在发布时所说，“阿里巴巴集团推出的《阿里巴巴Java开发手册（正式版）》是公司近万名开发同学集体智慧的结晶，以开发视角为中心，详细列举了如何开发更加高效、更加容错、更加有协作性，力求知其然，更知其不然。结合正反例，让Java开发者能够提升协作效率、提高代码质量。” 同时，阿里巴巴也期望这套Java统一规范标准将有助于提高行业编码规范化水平，帮助行业人员提高开发质量和效率、大大降低代码维护成本。
![](http://insights.thoughtworkers.org/wp-content/uploads/2017/04/1-alibaba-java.jpg)
（图片来自：http://t.cn/R63jrWi）
其实早在多年前，Google就已经把公司内部采用的所有语言的编码规范(其称为[Style Guide](https://github.com/google/styleguide))都开源在Github上。这份清单中包括了[C++](https://google.github.io/styleguide/cppguide.html)、[Objective-C](https://google.github.io/styleguide/objcguide.xml)、[Java](https://google.github.io/styleguide/javaguide.html)、[Python](Python%20Style%20Guide)、[R](https://google.github.io/styleguide/Rguide.xml)、[Shell](https://google.github.io/styleguide/shell.xml)、[HTML/CSS](https://google.github.io/styleguide/htmlcssguide.xml)、[JavaScript](https://google.github.io/styleguide/jsguide.html)、[AngularJS](https://google.github.io/styleguide/angularjs-google-style.html)、[Common Lisp](https://google.github.io/styleguide/lispguide.xml)、[Vimscript](https://google.github.io/styleguide/vimscriptguide.xml)等语言的编程规范。并且Google还发布了一个用于检查样式合规性的工具[cpplint](https://github.com/google/styleguide/tree/gh-pages/cpplint)以及在Emacs中使用Google编程样式的配置文件[google-c-style.el](https://raw.githubusercontent.com/google/styleguide/gh-pages/google-c-style.el)。看来Google中Emacs粉比Vim粉要强势的多。
Google为什么要发布这样的Style Guide呢？因为它认为几乎所有的开源项目都需要有一组约定来规范如何编写代码。如果项目中的代码都能保持一致的风格，那么即使代码再多也会很容易的被人理解。
Google的这份编程规范包含了很多方面，从”对变量使用camelCase命名法”到”绝不要使用全局变量”到”绝不允许例外“等。其Java编程规范包含7大部分，分别为介绍、源文件基本要求、源文件结构、格式化、命名、编程实践和Javadoc。每一部分又细分为很多子条目。如果采取条规范的原因不是很容易理解，都会配有相应的示例或者引用文章。
由于Google的这份编程规范目前只有英文版本，所以在中国的程序员中只有少部分人知道它的存在。并且只有更少的团队在真正的应用它，其中就包括我的团队。我们团队根据Google的[Java style guide](https://google.github.io/styleguide/javaguide.html)也演化出了自己的团队版本，放置在团队共享wiki上供大家随时查阅。我们根据自身的项目特点丰富了”编程实践”里的内容，并且新加入一个章节来描述编写Java代码的一些原则，比如简洁代码、组合优于继承、stream优于for循环等。
我想阿里巴巴发布的Java开发手册之所以叫做”开发手册”，而不是像Google那样叫做“Style Guide(样式风格)”，是因为它不仅仅局限于style guide这一方面，而是以Java开发者为中心视角，划分为编程规约、异常日志规约、MYSQL规约、工程规约、安全规约五大块，再根据内容特征，细分成若干二级子目录。根据约束力强弱和故障敏感性，规约依次分为强制、推荐、参考三大类。
该开发手册中的每一条都值得了解。限于篇幅原因，这里只列出”编程规约“中有感受的几条来评述一下。
> 
15. 【参考】各层命名规约：
A) Service/DAO 层方法命名规约
1） 获取单个对象的方法用 get 做前缀。
2） 获取多个对象的方法用 list 做前缀。
3） 获取统计值的方法用 count 做前缀。
4） 插入的方法用 save（推荐）或 insert 做前缀。
5） 删除的方法用 remove（推荐）或 delete 做前缀。
6） 修改的方法用 update 做前缀。
B) 领域模型命名规约
1） 数据对象：xxxDO，xxx 即为数据表名。
2） 数据传输对象：xxxDTO，xxx 为业务领域相关的名称。
3） 展示对象：xxxVO，xxx 一般为网页名称。
4） POJO 是 DO/DTO/BO/VO 的统称，禁止命名成 xxxPOJO。
命名规约的第15条描述了在Service/DAO层对于资源的操作的命名规范。这一条的参考价值极大，因为我所有待过的团队对于这一点都没有明显的约束，每个团队都有五花八门的实现。如果能遵守这一点，那么我们在操作资源时就会减少一些困扰。
> 
2. 【强制】long 或者 Long 初始赋值时，必须使用大写的 L，不能是小写的 l，小写容易跟数字1混淆，造成误解。
说明：Long a = 2l; 写的是数字的 21，还是 Long 型的 2?
这是常量定义的第2条。从这一点可以看出阿里巴巴对代码可读性的细节扣的很严格。我也很赞同这一点。代码只需编写一次，而会被查看无数次，所以要力争在第一次编写的时候尽可能少的引入歧义。
> 
1. 【强制】大括号的使用约定。如果是大括号内为空，则简洁地写成{}即可，不需要换行；如果是非空代码块则：
1） 左大括号前不换行。
2） 左大括号后换行。
3） 右大括号前换行。
4） 右大括号后还有 else 等代码则不换行；表示终止右大括号后必须换行。
格式规约的第1条终于终结了括号之争。这一条需要强制遵守，那么左大括号换行一派则被彻底排除在阿里巴巴之外。有人说不推荐左大括号换行，可以减少行数，增加单个屏幕可以显示的代码行数。而有的人反驳说现在屏幕已经足够大，不换行则破坏了对称之美。其实对于我来说两种格式都有各自的好处，我都可以接受，只要团队能够坚持使用其中之一即可。
> 
5. 【强制】缩进采用 4 个空格，禁止使用 tab 字符。
说明：如果使用 tab 缩进，必须设置 1 个 tab 为 4 个空格。IDEA 设置 tab 为 4 个空格时，请勿勾选 Use tab character；而在 eclipse 中，必须勾选 insert spaces for tabs。
正例： （涉及 1-5 点）


```
public static void main(String[] args) {
    // 缩进 4 个空格
    String say = "hello";
    // 运算符的左右必须有一个空格
    int flag = 0;
    // 关键词 if 与括号之间必须有一个空格，括号内的 f 与左括号，0 与右括号不需要空格
    if (flag == 0) {
        System.out.println(say);
    }
    // 左大括号前加空格且不换行；左大括号后换行
    if (flag == 1) {
        System.out.println("world");
        // 右大括号前换行，右大括号后有 else，不用换行
    } else {
        System.out.println("ok");
        // 在右大括号后直接结束，则必须换行
    }
}
```
使用空格代替tab字符进行缩进已经成为了编程界的共识。其主要原因是不同的平台甚至不同的编辑器下tab字符的长短是不一样的。不过Google在其《java style guide》中规定缩进为2个空格，而阿里巴巴约定为4个空格。由于4个空格的缩进比2个空格的缩进长一倍，所以如果在代码嵌套过深的情况下可能会很快超过单行最多字符数(阿里巴巴规定为120个)的限制。不过这个问题可以从另一个方面进行思考，如果由于缩进的原因导致单行字符数超标，这很可能是代码设计上有坏味道而导致嵌套过深。所以最好从调整代码结构的方面下手。
> 
6. 【强制】单行字符数限制不超过 120 个，超出需要换行，换行时遵循如下原则：
1） 第二行相对第一行缩进 4 个空格，从第三行开始，不再继续缩进，参考示例。
2） 运算符与下文一起换行。
3） 方法调用的点符号与下文一起换行。
4） 在多个参数超长，逗号后进行换行。
5） 在括号前不要换行，见反例。
正例：


```
StringBuffer sb = new StringBuffer();
    //超过 120 个字符的情况下，换行缩进 4 个空格，并且方法前的点符号一起换行
    sb.append("zi").append("xin")...
        .append("huang")...
        .append("huang")...
        .append("huang");
```
反例：


```
StringBuffer sb = new StringBuffer();
//超过 120 个字符的情况下，不要在括号前换行
sb.append("zi").append("xin")...append
("huang");
//参数很多的方法调用可能超过 120 个字符，不要在逗号前换行
method(args1, args2, args3, ...
, argsX);
```
关于换行，Google并没有给出明确的要求，而阿里巴巴则给出了强制性的要求。Google特别提示可以通过一些重构手法来减少单行字符长度从而避免换行，这一点我颇为认同。关于参数，很多方法调用超过120个字符需要换行，这暴露除了过长参数列的代码坏味道，解决方式之一就是使用重构手法的Replace Parameter With Method的方式把一次方法调用化为多次方法调用，或者使用Introduce Parameter Object手法创造出参数对象并进行传递。
> 
17. 【推荐】循环体内，字符串的联接方式，使用 StringBuilder 的 append 方法进行扩展。 反例：


```
String str = "start";
    for (int i = 0; i
```
说明：反编译出的字节码文件显示每次循环都会 new 出一个 StringBuilder 对象，然后进行append 操作，最后通过 toString 方法返回 String 对象，造成内存资源浪费。
这是《Effective Java》以及其他文章中经常提及的优化方式，而且面试初级Java工程师时几乎是一个必考点。其实不仅是在循环体内，所有需要进行多次字符串拼接的地方都应该使用StringBuilder对象。
> 
20. 【推荐】类成员与方法访问控制从严：
1） 如果不允许外部直接通过 new 来创建对象，那么构造方法必须是 private。
2） 工具类不允许有 public 或 default 构造方法。
3） 类非 static 成员变量并且与子类共享，必须是 protected。
4） 类非 static 成员变量并且仅在本类使用，必须是 private。
5） 类 static 成员变量如果仅在本类使用，必须是 private。
6） 若是 static 成员变量，必须考虑是否为 final。
7） 类成员方法只供类内部调用，必须是 private。
8） 类成员方法只对继承类公开，那么限制为 protected。
说明：要严控类、方法、参数、变量的访问范围。过宽泛的访问范围不利于模块解耦。思 考：如果是一个 private 的方法，想删除就删除，可是一个 public 的 Service 方法，或者一个 public 的成员变量，删除一下，不得手心冒点汗吗？变量像自己的小孩，尽量在自己的视线内，变量作用域太大，会无限制的到处跑，那么你会担心的。
这其实就是经典的原则‘Principle of least privilege’ 的体现。我们必须遵循这一原则，但不知为何阿里巴巴将其级别列为“推荐”。
> 
7. 【参考】方法中需要进行参数校验的场景：
1） 调用频次低的方法。
2） 执行时间开销很大的方法，参数校验时间几乎可以忽略不计，但如果因为参数错误导致 中间执行回退，或者错误，那得不偿失。
3） 需要极高稳定性和可用性的方法。
4） 对外提供的开放接口，不管是 RPC/API/HTTP 接口。
5） 敏感权限入口。
8. 【参考】方法中不需要参数校验的场景：
1） 极有可能被循环调用的方法，不建议对参数进行校验。但在方法说明里必须注明外部参 数检查。
2） 底层的方法调用频度都比较高，一般不校验。毕竟是像纯净水过滤的最后一道，参数错 误不太可能到底层才会暴露问题。一般 DAO 层与 Service 层都在同一个应用中，部署在同一台服务器中，所以 DAO 的参数校验，可以省略。
3） 被声明成 private 只会被自己代码所调用的方法，如果能够确定调用方法的代码传入参数已经做过检查或者肯定不会有问题，此时可以不校验参数。
编写代码时，对参数进行校验是不可避免的。详细说又扯到“防御式编程”和“契约式编程”的话题上。这两项之所以列为参考，并没有强迫大家遵守。
> 
6. 【推荐】与其“半吊子”英文来注释，不如用中文注释把问题说清楚。专有名词与关键字保持英文原文即可。
反例：“TCP 连接超时”解释成“传输控制协议连接超时”，理解反而费脑筋。
看到这一条我已经笑出来了。这一条说的很好，注释是用来阐述问题的，如果看了注释还一头雾水，那么这样的注释不要也罢。使用中文没什么可丢人的，解决问题才是王道。
> 
7. 【推荐】代码修改的同时，注释也要进行相应的修改，尤其是参数、返回值、异常、核心逻辑等的修改。
说明：代码与注释更新不同步，就像路网与导航软件更新不同步一样，如果导航软件严重滞后， 就失去了导航的意义。
阿里巴巴对该条的说明非常到位。其实我们团队在编写代码时默认是没有任何注释的，因为我们追求的是self-explanatory methods。即代码本身已经就能说明它的用途。只有在很少的情况下需要添加注释。
编程规约的第九部分都是很好的tips，值得去了解和学习。
除了编程规约之外，日志规约、MySQL规约、工程规约和安全规约也都有极高的参考价值，这也是比Google的[Java Style Guide](https://google.github.io/styleguide/javaguide.html)出色的地方。这里就不再评述了。
阿里巴巴公布这个Java开发手册绝对是值得赞赏的事情。最后我也想给其提几点建议：
- 建议使用公开wiki的方式发布该手册，而不是采用pdf的方式。因为如果像google那样是公开wiki方式的话，可以方便大家参与修正和改进，并且可以看到版本历史。
- 该手册并没有明确的版权许可，只是在页脚处加入了“禁止用于商业用途，违者必究”的字样。Google的style guide的版权为CC-By 3.0 License，建议阿里巴巴能够指明其版权。
- 手册中的部分示例代码并没有遵守其列出的编程规约，有点打脸之嫌。比如以下示例代码：


```
Iterator it = a.iterator();
 while(it.hasNext()){
     String temp = it.next();
     if(删除元素的条件){
         it.remove();
     }
 }
```
其while和if关键字与小括号之间并没有空格，违反了该手册中`3. 【强制】if/for/while/switch/do 等保留字与左右括号之间都必须加空格。`这一规则。
- 集合处理中可以多推荐一些Java8的集合操作方法。
- 有些名词没有过多解释，比如很多人可能都不知道什么叫一方库、二方库。
- 希望除了这份开发手册以外，阿里巴巴也可以推出对应的checkstyle配置文件以及Intellij、Eclipse的配置文件。毕竟格式化这些事都可以交由IDE来解决，通过在构建时使用checkstyle插件也可以防止不合规的代码迁入到仓库，从源头上保证代码样式的一致性。
最后，希望这份Java开发手册可以持续改进，吸纳百家之长，成为每个入门程序员必看的手册。
