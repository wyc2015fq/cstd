# 想写无Bug的安全代码？看防御性编程的艺术 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [一杯哈希不加盐](http://www.jobbole.com/members/snowhere) 翻译，[刘唱](http://www.jobbole.com/members/liuchang) 校稿。未经许可，禁止转载！
英文出处：[Diego Mariani](https://dev.to/0x13a/the-art-of-defensive-programming)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
为什么开发者不编写安全的代码？我们在这并不是要再一次讨论「整洁代码」。我们要从纯粹的实用观点出发，讨论软件的安全性和保密性。是的，因为不安全的软件不仅无用，而且还可怕。我们来看看什么是不安全的软件。
- 1996年6月4日，欧洲航天局的 Ariane 5 Flight 501 在起飞后 40 秒被引爆。因为导航软件里的一个 bug，这个价值 10 亿美金的运载火箭不得不自毁。
![](http://ww2.sinaimg.cn/mw690/7cc829d3gw1f6r1wqy5zdj20b408cwem.jpg)
- 1991年2月25日，MIM-104 Patriot（爱国者）里的一个软件错误使它的系统每一百小时有三分之一秒的时钟偏移，导致定位拦截入侵导弹失败。结果伊拉克的飞毛腿导弹击中宰赫兰（沙特阿拉伯东北部城市）的一个美军军营，28 人死亡，100 多人受伤。
- 其他案例，请参见《[Bug 引发的 18 次重大事故](http://blog.jobbole.com/104583/)》。
这应该能够说明编写安全软件的重要性了，尤其在特定的环境中。当然也包括其他用例中，我们也应该意识到我们的软件 bug 会导致什么后果。
## 防御性编程初窥
为什么我认为在特定种类的工程中，防御性编程是解决这些问题好办法？
> 
抵御那些不可能的事，因为看似不可能的事也会发生。
防御性编程中有很多防御方式，这也取决于你的软件项目所需的「安全」级别和资源级别。
> 
防御性编程是防御式设计的一种形式，用来确保软件在未知的环境中能继续运行。防御性编程的实践往往用于需要高可用性、安全性、保密性的地方。—— 维基百科
我个人相信这种方法适合很多人参与的大型、长期的项目。例如，一个需要大量维护的开源项目。
我们来探索一下我提出的关键点，来完成一个防御性编程的实现。
## 永远不要相信用户输入
设想你总是获取到你不想要的东西。因为像我们说过的，我们预期的是异常情况的出现，（所以）要时刻防备用户输入以及通常会传入你系统的东西，这是你成为一个防御性程序员的方法。试着做到尽可能的严格，确保输入的值就是你所期望的值。
*![](https://res.cloudinary.com/practicaldev/image/fetch/s--Pic7qAkP--/c_limit,f_auto,fl_progressive,q_auto,w_725/https://medium2.global.ssl.fastly.net/max/2000/1%2AwJBEFQ8XcNR7RzlMnTF_fw.png)*
*进攻是最好的防守*
设置白名单而不是黑名单。举个例子，当你验证图像扩展名时，不要检查非法的类型，而是检查合法的类型并排除其他类型。在 PHP 有无数的开源校验库可以让你的工作变简单。
进攻是最好的防守。共勉
## 数据库抽象化
在 [OWASP Top 10 Security Vulnerabilities](https://www.veracode.com/directory/owasp-top-10) 排首位的是注入攻击。这意味着有些人（很多人）还没有使用安全的工具来查询数据库。请使用数据库抽象包或库。在 PHP 里你可以使用 [PDO](http://php.net/manual/en/book.pdo.php) 来[确保基本的注入攻击防范](http://stackoverflow.com/questions/134099/are-pdo-prepared-statements-sufficient-to-prevent-sql-injection)。
## 不要重复发明轮子
你不用框架（或微框架）吗？好吧恭喜你，你喜欢毫无理由地做额外的工作。这并不仅跟框架有关，也意味着你可以方便地[使用已经存在的、经过测试的、受万千开发者信任的、稳定的](https://packagist.org/)新特性，而不是你只为了自己从中受益而制作的东西。你自己创建方法的唯一原因是你需要的东西不存在，或存在但不符合你的需求（性能差、缺失特性等等）。
这就是所谓的智能代码重用。拥抱它吧。
## 不要相信开发者
防御性编程与防御性驾驶相关联。在防御性驾驶中，我们假设周围的每个人都可能犯错。所以我们要留意别人的行为。相同概念也适用于防御性编程，我们作为开发者不要相信其他开发者的代码。我们同样也不要相信我们的代码。
在很多人参与的大型项目中，我们有许多方式编写并组织代码。这也导致混乱甚至更多的 bug。这也是我们需要加强规范代码风格和代码检查的原因，让生活更轻松。
## 编写符合 SOLID 原则的代码
这是（防御性）编程最困难的部分——[编写不糟糕的代码](https://medium.com/web-engineering-vox/how-to-write-solid-code-that-doesnt-suck-2a3416623d48)。这也是很多人知道并一直在讨论的，但没有人真正关心或将注意力和精力放在实现符合 SOLID 原则的代码上。
让我们看一些糟糕的例子
> 
避免：未初始化的属性

```
<?php
class BankAccount
{
    protected $currency = null;
    public function setCurrency($currency) { ... }
    public function payTo(Account $to, $amount)
    {
        // sorry for this silly example
        $this->transaction->process($to, $amount, $this->currency);
    }
}
// I forgot to call $bankAccount->setCurrency('GBP');
$bankAccount->payTo($joe, 100);
```
在这个例子中，我们需要牢记签发付款前要先调用 setCurrency。这是很糟糕的事情，一个像这样的改变状态的操作（签发付款）不应该分两步来完成，且使用两个公开的方法。我们还可以用很多方法付款，但我们必须只有一个公开的方法来改变状态（对象不应该存在不一致的状态）。
在这个例子中，我们把它改进，将未初始化的属性封装进 Money 对象。


```
<?php
class BankAccount
{
    public function payTo(Account $to, Money $money) { ... }
}
$bankAccount->payTo($joe, new Money(100, new Currency('GBP')));
```
使它万无一失。不要使用未初始化的对象属性。
> 
避免：在类的作用域外泄露状态

```
<?php
class Message
{
    protected $content;
    public function setContent($content)
    {
        $this->content = $content;
    }
}
class Mailer
{
    protected $message;
    public function __construct(Message $message)
    {
        $this->message = $message;
    }
    public function sendMessage(
    {
        var_dump($this->message);
    }
}
$message = new Message();
$message->setContent("bob message");
$joeMailer = new Mailer($message);
$message->setContent("joe message");
$bobMailer = new Mailer($message);
$joeMailer->sendMessage();
$bobMailer->sendMessage();
```
在这个例子中，Message 是通过引用传递的，两个实例的输出都是 “joe message”。一个解决方案是复制 Mailer 构造函数中的 message 对象。但是我们应该做的是试着使用（不可变的）[值对象](https://en.wikipedia.org/wiki/Value_object)，而不是简单可变的 Message 对象。尽可能使用不可变的对象。


```
<?php
class Message
{
    protected $content;
    public function __construct($content)
    {
        $this->content = $content;
    }
}
class Mailer 
{
    protected $message;
    public function __construct(Message $message)
    {
        $this->message = $message;
    }
    public function sendMessage()
    {
        var_dump($this->message);
    }
}
$joeMailer = new Mailer(new Message("bob message"));
$bobMailer = new Mailer(new Message("joe message"));
$joeMailer->sendMessage();
$bobMailer->sendMessage();
```
## 编写测试
这点我们很还需要再说吗？编写单元测试可以帮助你秉承一般的原则，比如高内聚、单一职责、低耦合和正确的对象组合。它不仅帮助你测试小的单元用例，也能测试你组织对象的方式。确实，当测试你的小功能时，你会清晰的看到你需要测试多少情况和需要模拟多少对象，来达到 100% 的覆盖率。
## 结论
希望你喜欢这篇文章。记住这些仅仅是建议而已，由你决定何时、何处以及是否应用。
感谢阅读！
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/01/5085cce1bd4e144b7d148079a34b20c1.jpg)![](http://jbcdn2.b0.upaiyun.com/2017/01/02b09a29fdb3ab5fe03d4bd1afe95664.jpg)
