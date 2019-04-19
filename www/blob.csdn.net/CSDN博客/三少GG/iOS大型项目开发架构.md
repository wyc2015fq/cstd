# iOS大型项目开发架构 - 三少GG - CSDN博客
2016年02月15日 21:30:56[三少GG](https://me.csdn.net/scut1135)阅读数：7372
http://www.jianshu.com/p/921ab32c3c71#
首先说说编程语言的选择，Objecive-C还是Swift？我还没有在项目中使用Swift，因为我说服不了自己去用它，它的优势在哪里，你也不能强迫队友去学习Swift。当然，不用不代表不会，一入行就用Swift开发无意义产品的人没资格戴着有色眼镜鄙视不会Swift的同行。你知道Objecive-C与Swift混编有多少坑吗？你知道Swift也是跟Objecive-C共用一个Runtime环境吗？你知道使用了Swift会使得ipa包大10多M吗？Swift代表未来，Objecive-C是当下。Swift能做的Objective-C都能做，比如Closure完全就可用Blocks来代替，Tuple完全可以用结构体来代替。生产环境用Objective-C，业余观望Swift，这就是我的态度。Raywenderlich已经出了一堆Swift的教程，在前沿科技的使用上国内总是慢一个节拍，很大程度上那堵墙阻碍了国内IT从业者的发展，墙的开发者（包括我的信息安全工程老师）终有一天会受到历史的审判。
再来说说应用架构，真是成也MVC,败也MVC。如果放任团队中的小朋友按他们所理解的MVC来开发，一般情况下出现的恶果就是类之间高耦合，Controller胖得不像话简直就成了百宝箱......每一次的版本迭代都会痛苦不堪，若功能不多还能勉强维持功能多了势必崩塌，就比如世博园中国馆吧，再按比例多盖5层试试看，呵呵。到头来开发人员实在受不了就只能选择重构要么跑路，后者更现实大家都懂，尤其是业务为王的企业里，代码质量算个屁只要能work，可是好的程序员都是有尊严的，deadline或是拍脑袋的政治任务通常会让程序员疲于应付，厌倦了也就该撤了。言归正传，既然MVC显得臃肿，那就是瘦身呗。通常瘦身后的MVC在iOS界被叫成MVCS或MVVM,这2个当然不是同一个东西，项目选用MVCS还是MVVM还是得看你的业务特性。MVCS与MVVM就那么完美吗？当然不，MVCS要注意Service/Store的滥用，其中的数据是否会被不同的模块污染。MVVM用得不好也会增加项目的维护难度，我说的是View和ViewModel之间松散的绑定关系，在iOS开发中一提到MVVM大家就想到ReactiveCocoa，其实没有ReactiveCocoa也可以啊，只是ReactiveCocoa的好处多多，如代码收敛不必写得到处都是，其他好处自己去挖掘吧。
关于工程文件组织结构，很多人是Controllers/Views/Models/Vender...这样归类，其实这有个问题，比如你要找ControllerA的TableView用到的cellB类，你还要去Views里面一个个找，太痛苦了，就算search也还是苦毕竟不能所见即所得。我一般是按 Module来划分，每个Module里面有Controllers/Views/Models/Stores/API这样的分类，API是每一个接口的请求的封装，供Store调用，得到的数据解析实例化为Model再通过block传递给Controller去刷新View，很绕吗？使用RAC,Controller订阅Store里创建的RACSignal也能做到，U
 can make a try。还有就是Helper与Utility，与业务无关，具有对象性质，提供支持功能的代码放到Helper，比如创建一个自定义对象的封装。如果只是属于函数或算法，不是对象而且很多地方能用到，就放到Utility，比如排序/加密算法。
![](http://upload-images.jianshu.io/upload_images/673581-3faaa1a616cd0e65.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
工程文件组织结构
关于网络通信模块的设计，我个人认为应该是每一个HTTP请求都应该独立互不干扰，就是你封装的POST/GET方法都会创建一个临时的对象，而长连接一般只维持一个实例对象采用单例的方式创建。我会为每一个HTTP 接口请求创建一个API对象，在里面请求数据，当然了为了避免请求代码的重复编写可以建立一个BASE API类，子类调用父类的请求方法就行了，不同的只是接口与参数。思想就是这样，以后有时间再来讲讲API类的设计。
关于多线程，在iOS开发中用得最多的就是GCD和NSOperation了，在大部分情况下GCD就够用了。但是NSOperation也有它的优势，比如可以设置并发个数，可以设置线程间的依赖，可以暂停和恢复，比较灵活，多线程下载就经常用它。面试中也经常会问GCD与NSOperation的区别，这个自己去查查，资料还是比较丰富的，底下也有篇关于NSOperation的参考链接。GCD虽然强大，可是还是有很多人瞎用，真替他们着急，随便说几点：
另外，单元测试真的有必要吗？单元测试可以使逻辑清晰化，当你仅仅阅读单元测试代码的话，你会发现它们写的好像编程教科书里的伪代码。当TDD的时候，这一点尤其有用。通过写单元测试，你可以很快的把逻辑梳理清楚，然后用代码来实现它。单元测试可以降低代码的耦合度。我们知道，耦合度高的代码很难做单元测试，反过来，如果你必须做单元测试，你是不会把代码写的耦合度很高的。单元测试可以让你知道你对代码的修改是否影响到了原来就有的功能，但是这也是所有的回归测试都可以做的。单元测试的特点在于：它测试的东西足够小从而在代码重构后仍能复用。单元测试是唯一一个可能使覆盖率达到100%的测试。单元测试开始难，持续做的话会越来越容易，因为难主要是因为环境的搭建和桩函数的缺失。单元测试很容易定位Bug，它好像在你的程序中打了无数的断点。单元测试很费时间，不过，后续改Bug更费时间。
讲了这么多，最后说说需求吧。我认为在需求评审时应该尽量抛出细节问题给pm，他们不懂技术，如果需求不确定就盲目开发，然后中途再改需求，这是很伤士气的，尤其是涉及到架构的修改，这让我想到那张pm改需求被程序员拍板砖的图，嘿！需求不稳定，就别动工，宁愿打酱油看博客。好了，啰嗦了这么多，有多少人会看到这里呢？希望以后有空回头来增加些内容。
**###########################长篇分享#################################**
**http://casatwy.com/iosying-yong-jia-gou-tan-kai-pian.html**
**http://casatwy.com/iosying-yong-jia-gou-tan-viewceng-de-zu-zhi-he-diao-yong-fang-an.html**
**http://casatwy.com/iosying-yong-jia-gou-tan-viewceng-de-zu-zhi-he-diao-yong-fang-an.html**
# [怎么面试架构师](http://casatwy.com/zen-yao-mian-shi-jia-gou-shi.html)http://casatwy.com/zen-yao-mian-shi-jia-gou-shi.html
**#################################**
# IOS应用架构思考一（网络层）
 http://blog.cnbluebox.com/blog/2015/05/07/architecture-ios-1/
# IOS应用架构思考二（网络图片库）
http://blog.cnbluebox.com/blog/2015/07/10/architecture-ios-2/
###############
http://geek.csdn.net/news/detail/56517
## 被误解的 MVC 和被神化的 MVVM 
## [http://www.infoq.com/cn/articles/rethinking-mvc-mvvm](http://www.infoq.com/cn/articles/rethinking-mvc-mvvm)
说完 View 和 Model 了，那我们想想 Controller，Controller 有多少可以复用的？我们写完了一个 Controller 之后，可以很方便地复用它吗？结论是：非常难复用。在某些场景下，我们可能可以用`addSubViewController` 之类的方式复用 Controller，但它的复用场景还是非常非常少的。
如果我们能够意识到 Controller 里面的代码不便于复用，我们就能知道什么代码应该写在 Controller 里面了，那就是那些不能复用的代码。在我看来，Controller 里面就只应该存放这些不能复用的代码，这些代码包括：
- 
在初始化时，构造相应的 View 和 Model。
- 
监听 Model 层的事件，将 Model 层的数据传递到 View 层。
- 
监听 View 层的事件，并且将 View 层的事件转发到 Model 层。
如果 Controller 只有以上的这些代码，那么它的逻辑将非常简单，而且也会非常短。
但是，我们却很难做到这一点，因为还是有很多逻辑我们不知道写在哪里，于是就都写到了 Controller 中了，那我们接下来就看看其它逻辑应该写在哪里。
## 如何对 ViewController 瘦身？
[objc.io](https://www.objc.io/) 是一个非常有名的 iOS 开发博客，它上面的第一课 [《Lighter
 View Controllers》](https://www.objc.io/issues/1-view-controllers/lighter-view-controllers/) 上就讲了很多这样的技巧，我们先总结一下它里面的观点：
- 
将 UITableView 的 Data Source 分离到另外一个类中。
- 
将数据获取和转换的逻辑分别到另外一个类中。
- 
将拼装控件的逻辑，分离到另外一个类中。
你想明白了吗？其实 MVC 虽然只有三层，但是它并没有限制你只能有三层。所以，我们可以将 Controller 里面过于臃肿的逻辑抽取出来，形成新的可复用模块或架构层次。
我个人对于逻辑的抽取，有以下总结。
