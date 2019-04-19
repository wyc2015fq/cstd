# iOS应用瘦身方法思路整理 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年11月13日 10:49:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：350
# 一、前言
前段时间注意到我们APP的包大小超过100MB了，所以随口跟吕老板说了下能否采用字体文件（.ttf）替代PNG图片，吕老板对应用瘦身很感兴趣因此让我做下技术调研。这篇文章主要是将我们的各个技术方案的思路做一下整理和总结，希望对大家有所帮助。
# 二、iOS内置资源的集中方式
在介绍技术方案前我们先来看下iOS内置图片资源都有哪些常见的方式：
## 1、将图片存放在bundle下
这是一种非常常见的方式，项目中各类文件分类放在各个bundle下，项目既整洁又能达到隔离资源的目的。我们项目中图片绝大多数都是这样内置的，其加载方式为[UIImage imageNamed:"xxx.bundle/xxx.png"]（请记住这个字符串的规则，因为这种规则非常非常重要！！！"xxx.bundle/xxx.png"）。但是这种方式有比较明显的缺点：首先使用bundle存储图片iOS系统不会对其进行压缩存储，造成了应用体积的增大。其次是使用bundle存储图片放弃了APP thinning，其明显的表现是使用2倍屏手机的用户和使用3倍屏手机的用户下载的应用包大小一样。如果能够实现APP thinning，那么往往2倍屏幕的手机包大小会小于3倍屏手机的包大小，起到差异性优化的目的。在调研过程中我们还发现，应用的体积与图片资源的数量密切相关（听起来好像是废话）。换句话说，iPhone的rom存在4K对齐的情况，一张498B大小的图片在应用包中也要占据4KB大小。因此项目中每添加一张图片就至少增大了4KB。为了证实这个观点特地创建空应用进行测试。首先创建空应用，其大小在7P上为213KB，引入一张498B的图片前后对比如下：
![](https://img-blog.csdn.net/20171113103630199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一张498B的图片
![](https://img-blog.csdn.net/20171113103642676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
占据4KB磁盘空间
![](https://img-blog.csdn.net/20171113103657632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
未添加资源的应用
![](https://img-blog.csdn.net/20171113103717711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
添加图片资源后的大小
上述实验未经过App Store上线认证，仅仅通过本地打包测试，因此观点仅供参考。
## 2、使用.ttf字体文件替代图标
使用字体文件替代图片也是一种比较常见的资源内置方式。很多应用都使用过这种方案，如淘宝、爱奇艺等知名应用，都采用过这种方式。使用字体文件的好处是显而易见的，如果APP中某个图片比较大，那么为了保证清晰度，UI可能会提供比较大的图标。使用字体文件会避免这个问题，而且不必导入@2x和@3x图片，一套字体文件就能保证UI的清晰度。关于如何生成.ttf文件在这里就不在赘述了（因为我并不喜欢这个方案），我们只要如何使用就可以了。字体文件使用起来比较简单，但是使用方法与png图片的使用方法有很大的不同，因为字体文件时机所展示的图标都是UTF8编码转来的字符串。因此当我们需要展示一个图标的时候不再是使用UIImageView了，而是UILabel。
![](https://img-blog.csdn.net/20171113103735407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
字体文件展示图片的代码示例
由于我们使用了字体来替代图片，所以我们可以通过设置字体的颜色来改变图标的颜色。我们之前经常会遇到一个场景，如两个一模一样的图标但是由于颜色不同，UI同学就需要提供2套图片，每套图片中包含@2x和@3x图片。如果采用了字体替代简单的图标，那么UI只需要提供一套字体即可，并且拉伸后也不会失真。使用字体文件的好处总结起来主要有两点：
1、可以降低应用图片内置资源的体积。
2、可以随意放缩和修改颜色。
但是其缺点也很明显：
1、图标的查找和替换比较麻烦，不如直接使用图片那样简单。
2、最重要的是如果在58同城APP中使用，则意味着无法替换之前存在的图片，只能起到缩小增量的目的，无法减小全量。
ps：任何一种需要大刀阔斧改革的优化都是一种不明智的行为。
## 3、图片存在Assets.xcassets下（苹果推荐，我也推荐）
使用Assets.xcassets是苹果推荐的一种方式。Assets.xcassets是iOS7推出的一种图片资源管理工具，将图片内置到Assets.xcassets下系统会对图片资源进行压缩，并且支持APP thinning。
![](https://img-blog.csdn.net/20171113103755376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
APP Slicing
项目优化不能脱离场景，很多很好的方案由于场景的限制并不能起到优化的作用。因此先简单介绍下我们的项目场景：为了达到跨团队快速开发的目的，我们项目很早就利用cocoapods实现组件化。项目中存在多个业务pod，每个pod都有各自的团队维护，各个团队的代码彼此不开放，各个pod最终会被编译为.a的形式。这里需要说明一下我为什么要强调.a，与.a相对应的还有一个.framework，他们之间有一个重要的区别就是资源的问题。framework中可以存放资源，但是.a却不可以，因此生成.a的pod下的资源会被转移到main bundle下，这为资源冲突造成了隐患，为了避免这种冲突我们之前采用的使用bundle管理资源，bundle名很少会重复这样就大大降低了资源冲突的可能性。优化的前提之一也是不破坏这种组件化开发的模式，换句话说也就是各个业务线不产生资源耦合、业务线的RD不必担心彼此资源的冲突、业务Pod下的资源文件彼此隔离。哪怕招聘团队中存在a.png，房产团队中也存在a.png也不会有什么问题。所以我们先要抛出两个问题：
1、cocoapods是否支持使用Assets.xcassets。
2、各个pod各自维护自己的Assets.xcassets会不会造成资源冲突。
为了弄清楚上面两个问题，我们先要看下podspec的几个重要参数：
![](https://img-blog.csdn.net/20171113103820820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
podspec
s.public_header_files ：表明了哪些路径下的文件可以在framework外被引用。
source_files ：源文件路径。
s.resources ：资源文件路径及文件类型。
s.resource_bundles ：资源文件路径及类型，同时资源文件会被打成bundle。（推荐使用）。
实验发现各个pod下都可以创建自己的xcassets，因此问题1不算问题是问题。如果我们在各个业务pod下都创建.xcassets文件内置图片，那么cocoapods的脚本会在编译时将各个目录下的xcassets文件内容提取出来，合并到一个xcassets中并生成一个.car文件。这样的话如果资源文件重名，那么很可能其中某一个文件会被覆盖替换。因此我们主要是要解决问题2。查看podspec的写法发现s.resource_bundles貌似是我们所需要的法宝。为此我们天真的以为问题马上就要解决了：
![](https://img-blog.csdn.net/20171113103839974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将指定路径下的资源打包成bundle
最终打包结果很理想，确实能够生成ImagesBundle.bundle，并且bundle下存在Assets.car。
![](https://img-blog.csdn.net/20171113103902919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
mainbundle下存在ImagesBundle
![](https://img-blog.csdn.net/20171113103925991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ImageBundle.bundle下存在Assets.car
事情到这里可能已经看到曙光了，但是我们发现通过
[UIImage imageNamed:@"ImagesBundle.bundle/1"];
加载不出来图片。必须使用
[UIImageimageNamed:@"1"inBundle:[WBIMViewControllericonBundle]compatibleWithTraitCollection:nil];
才能加载出来。
![](https://img-blog.csdn.net/20171113104004424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图片加载失败
![](https://img-blog.csdn.net/20171113104128933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
指定bundle后加载成功
也就是说只有Assets.car如果不在main bundle下，那么加载图片都需要指定bundle。
既然需要指定bundle加载图片，那么如何获取这个bundle呢？换句话说如何才能低成本的将现在项目中的图片放到特定bundle下的Assets.car文件中呢？对此我们提出了一个解决方案：
1. 在pod下新建一个空文件夹。找出该pod存放图片的所有bundle，在新建文件夹下创建与bundle数量相等的Asset。
2. 修改podspec文件，设置resource_bundles将Asset指定为资源，并指定bundle名称。如A.bundle,其对应的Asset最终资源bundle为A_Asset.bundle。
3. 新增方法，imageWithName:，从符合xxx.bundle/yyy.png特征的参数中获取bundle名和图片名xxx_Asset.bundle和yyy.png，获取图片并返回。
4. 查找并全部替换imageNamed: 和 imageWithContentOfFile:为imageWithName:
只要能拿到原来代码中imageNamed:的参数就能知道现在图片存在那个bundle下，这样就能通过imageNamed:inBundle:获取到图片，其思路如下图所示：
![](https://img-blog.csdn.net/20171113104239138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
imageWithName:方法内部处理
![](https://img-blog.csdn.net/20171113104259542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打包后bundle情况
看到这里老司机们已经应该能遇见这种优化的成本了。加载图片都需要指定bundle也就意味着成千上万处的API需要修改。我们最初探讨到这里的时候首先想到的是脚本，但是这个方案很快就被否定了，因为项目中存在大量的XIB，XIB中设置图片我们无法通过脚本替换API。
为了解决XIB设置图片的问题，我们首先想到了AOP。通过hook XIb加载图片的方法将方法偷偷替换为imageNamed:inBundle:  ,但是很遗憾我们hook了UIImage所有加载图片的方法，没有一个方法能拿到XIB上所设置的图片名称，也就意味着我们无法得知优化后的图片在哪个bundle下，也就不知道图片该如何加载。虽然有坎坷，但是我们始终坚信XIB一定是通过某些方法将图片加载出来的，我们一定能拿到这个过程！为了验证这个问题，首先定义一个UIImageView 的子类，并将XIB上的UIImageView指定为这个子类。大家都知道通过XIB加载的视图都一定会执行initWithCoder:方法
![](https://img-blog.csdn.net/20171113104335046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
UIImageView的子类加载
我们发现在得到执行[super initWithCoder:aDecoder]之前通过lldb查看slef.image是nil。当执行完这行代码后self.image就有值了。因此推断图片的信息（图片名称、路径等信息）都在aDecoder中！在网上搜索了一些资料后发现aDecoder有一些固定的key，可以通过这些固定的key得到一部分信息。如
![](https://img-blog.csdn.net/20171113104351503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
aDecoder可以通过某些key得到其中信息
很显然通过“UIImage”这个key能拿到图片，但是很遗憾经过多次尝试没能找到图片的路径信息。因此这个问题的关键是怎么找到合适的key，为了解决这个问题，最好是能拿到aDecoder的解码过程。因此hook aDecoder的解码方法decodeObjectForKey：是个不错的选择。如果能拿到xib上设置的图片名称那么我们就可以根据图片名称获取到正确的图片路径。经过断点查看aDecoder 是UINibDecoder（私有类）类型。
![](https://img-blog.csdn.net/20171113104425867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
aDecoder
![](https://img-blog.csdn.net/20171113104555009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
hook UINibDecoder的decode方法
打印系统decode的所有key 后发现有个key为**UIResourceName**,value为图片的名称。也就是说我们能得到XIB上设置的图片名称了。但是这个图片的名称怎么传递给这个XIB对应的UIImageView 对象呢？换句话说也就是说我们怎么把图片传给这个XIB对应的view呢？为了将图片名称传给UIImageView，需要给aDecoder添加一个block的关联引用。
![](https://img-blog.csdn.net/20171113104628923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
UIImageView在initWithCoder:的时候设置回调
在hook到的decodeObjectForKey:方法中将图片名称回传给initWithDecoder:方法：
![](https://img-blog.csdn.net/20171113104656370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
aDecoder hook到图片名称后回调给UIImageView类
这里需要注意的是一点是：XIB 默认设置图片是在rentun value之后，也就是说如果我们回调过早有可能图片被替换为nil。因此需要dispatch_after一下，等return 之后再回调图片名称并设置图片。受此启发，我们也可以hook UIImage 的imageNamed:方法，根据参数的规则到xxxCopy.bundle下获取图片，并返回图片。这就意味着放弃通过脚本修改API，减少了代码的改动。看到这里似乎是没有什么问题，但是我们忽略了一个很严重的问题aDecoder对象和UIImageView类型的对象是一一对应的吗？一个imageView它的aDecoder是它唯一拥有的吗？带着这个问题，我们先来看下打印信息：
![](https://img-blog.csdn.net/20171113104734387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
重复生成UIImageView对象和aDecoder对照关系
重复生成对象并打印后发现aDecoder的地址都相同，也就是说存在一个aDecoder对应多个UIImageView的现象。因此异步方案不适用，需要同步进行设置图片，因此全局变量最为合适。其实这一点很容易理解，aDecoder是与XIB对应的，XIB是不变的所以aDecoder是不变的。因此异步回调的方案不适用，需要同步进行设置图片，在这种情况（主线程串行执行）下跨类传值全局变量最为合适：
![](https://img-blog.csdn.net/20171113104801990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
hook UINibDecoder的decodeObjectForKey                                           
![](https://img-blog.csdn.net/20171113104822615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
hook UIImageView 的initWithCoder:
上面两段代码仅仅介绍思路，可能加载图片的代码并不是十分的严谨，请读者自己鉴别。同理hook 项目中UIImage 所用到的加载图片的API即可加载图片。如果将所有的hook方法放到一个类中，那么只要将这个类拖入到项目中，并将项目中所有的bundle下的图片都放到对应的Assets.xcassets文件下那么无需修改一行代码即可将所有的图片迁移到Assets.xcassets下，达到应用瘦身的目的。但是我们组内老练的架构师们指出：项目中hook如此重要的API对增加了项目维护的难度。这也引发了我对项目中AOP场景的思考，项目中到底hook 了多少API？可能在我场多年的老司机们都难以回答了，为此特地赶制了一个基于fishhook的一个hook打印工具，检测和统计项目中的AOP情况。但是缺点是必须调整编译顺序保证工具类最先被load。
![](https://img-blog.csdn.net/20171113104846641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
hook method_exchangeImplementations 方法 
![](https://img-blog.csdn.net/20171113104909582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
作者：皮拉夫大王在此
链接：http://www.jianshu.com/p/cb5a43bce796
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
