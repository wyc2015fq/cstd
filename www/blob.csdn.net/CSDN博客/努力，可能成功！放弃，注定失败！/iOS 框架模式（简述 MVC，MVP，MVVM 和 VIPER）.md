# iOS 框架模式（简述 MVC，MVP，MVVM 和 VIPER） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月08日 17:28:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：915
## 写在前面的话
最近看了一篇国外大牛写的关于 iOS 框架模式的文章，内容涉及目前 iOS 端诸多主流的框架模式，个人感觉文章写的很不错，收获匪浅，希望能够通过翻译原文的方式更好的体会一下，也分享给更多的人参考。原文地址在[这里](https://medium.com/ios-os-x-development/ios-architecture-patterns-ecba4c38de52#.63t4qslet)，浏览原文可能需要科学上网。
## 正文
![](http://upload-images.jianshu.io/upload_images/1148179-84a68614eedc6858.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原文配图，感觉不错
在 iOS 中使用 MVC 感觉怪怪的？想要投奔 MVVM 但是又感到迟疑？听说过 VIPER，但是不确定是否值得花费时间在上面？
接着向下阅读就会找到上面这些问题的答案，如果没有找到满意的答案，在评论区随便抱怨好了。
你马上要把你关于框架模式（architectural patterns）的知识在 iOS 环境中组织成体系。我们将会简要的回顾一些流行的框架，并且在理论上进行比较，然后通过几个小例子进行练习。如果你想要了解任何一个框架的详细资料，点击框架名的链接。
> 
掌握涉设计模式（design patterns）是一件让人着迷的事情，需要小心的是：你可能在阅读这本篇文章之前已经不再问自己类似于下面的这样的问题了：
1、是 model 还是 controller 应该去持有网络访问请求？
2、我应该怎样把一个 model 传到一个新 view 的 view model 中？
3、是 router 还是 presenter 创建新的 VIPER 单元？
![](http://upload-images.jianshu.io/upload_images/1148179-e35de48e411d54f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
思考状
## 为什么要关心选择框架模式的问题
原因就是如果你不关心的话，总有一天，当调试着一个巨大无比、有着各种各样乱七八糟东西的类的时候，你会发现在这个类里面你根本无法发现和修复 bug。
通常来说，把这样的类作为一个整体记在脑子里是非常困难的，因此，你总是会忘记一些重要的细节。如果你发现在你的应用中已经出现了这种情况，很有可能是这样的：
- 这个类是一个 UIViewController 的子类；
- 你的数据直接保存在 UIViewController 中；
- 你的 UIView 基本不做什么事情；
- model 中的数据结构很傻；
- 你的单元测试什么也覆盖不了；
其实不必觉得很难过，因为即使你遵循了苹果指导意见并实现了苹果的 MVC 模式，这种情况还是可能会发生。苹果的 MVC 有点问题，我们回头再说这件事情。
我们来看一下一个好的框架应该有的特征（这里我翻译的不好，将原文贴上）：
- 平衡并且严格的划分实体之间的角色和责任（Balanced distribution of responsibilities among entities with strict roles）
- 可测试通常作为第一优先级（不必担心：对于一个合适的架构来说这个很简单）
- 便于使用并且维护成本低
### 为什么要划分（distribution）呢？
当我们试图搞明白是（程序中）怎么工作的时候，划分（distribution）能够让我们想的不会那么累。想得越多开发的越好，大脑会适应理解复杂的事物，确实是这样。但是这个能力不是线性增长的，也不是能够快速提升的。所以能够击败事物复杂性的最简单的办法就是在遵循[单一责任原则](https://en.wikipedia.org/wiki/Single_responsibility_principle)的前提下把责任划分到多个实体。
### 为什么要可测试？
这个问题通常不是那些没有能够成功添加新功能或者重构类中的复杂内容，而对单元测试感到感激的人问出来的。这意味着单元测试帮助这些开发者免于在运行时发现问题，而这些问题可能会在用户的设备上发生，并且需要好几个周才能让用户看到修改。
### 为什么要易用？
这个问题不需要回答，但是需要提一下，最好的代码就是不需要写的代码，所以说代码越少越不容易出错。也就是说，想要少写代码的想法并不仅仅是因为开发者的懒惰，此外，你也不要被一个更加聪明的解决方案所蒙蔽而忽略了它的维护成本。
## MV(X)的基本要素
现在当我们面对框架设计模式的时候有很多选择：
- [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller)
- [MVP](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter)
- [MVVM](https://en.wikipedia.org/wiki/Model_View_ViewModel)
- [VIPER](https://www.objc.io/issues/13-architecture/viper/)
前面三种把 App 中的所有实体都分成下面三种类型：
- **Models**-拥有数据或者说是操作数据的[数据访问层（DAL）](https://en.wikipedia.org/wiki/Data_access_layer)，可以联想一下 Person 类或者 PersonDataProvider 类
- **Views**-对应展现层（GUI），对于 iOS 来说可以想一下所有的以‘UI’作为前缀的东西
- **Controller/Presenter/ViewModel **-Model 和 View 的粘合剂或中转站，通常来说，用来处理作为用户在 View 上操作的反馈来更改 Model，或者因为 Model 的改变来更新 View 上的展示
把实体进行划分能够让我们：
- 理解的更好（我们已经知道了）
- 重用（通常都是 View 和 Model 比较适合重用）
- 独立进行测试
*我们先从 MV(X) 模式开始学习，后面再介绍 VIPER*
## MVC
### MVC原来是什么样的？
在讨论苹果版本的 MVC 之前我们先来看看传统的 MVC 是什么样的？
![](http://upload-images.jianshu.io/upload_images/1148179-ef2ec126626430b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Traditional MVC
这样的话，View 是没有归属的（stateless），只要 Model 变化了，就会被Controller 提供（渲染？）。想想 web 网页，只要你点击链接到别的地方的网页，网页就会彻底重新加载。虽然这种 MVC 结构在 iOS 应用中很容易实现，但是因为在结构上，这三种实体是紧紧耦合在一起的，每一种实体都和其他两种实体有联系，所以这种结构也起不到什么作用，并且还戏剧化的降低了其可复用性，所以这种结构不会是想要在你的应用中使用的。由于以上原因，我们在这里就不写这种 MVC 的例子了。
> 
传统的 MVC 结构看起来并不适合现在的 iOS 开发工作。
### 苹果的 MVC
#### 期望（Expectation）
控制器来作为视图和模型中间的中转站，这样视图和模型之间互相就没有直接联系了。这样的话，控制器的可复用性就变得最低，但是这个对于我们来说也是可接受的，因为我们需要有一个地方来放那些不方便放在模型中的复杂业务逻辑。
![](http://upload-images.jianshu.io/upload_images/1148179-303cb98d82dd2d02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Cocoa MVC
理论上来讲，这种结构看起来非常直接，但是是不是觉得有点不对劲？甚至听到过有人叫 MVC 为重控制器模式。此外，对于 iOS 开发者来说，给控制器减轻负担已经成为一个重要的话题。为什么苹果会采用仅仅改进过一点点的传统 MVC 模式呢？
#### 现实（Reality）
![](http://upload-images.jianshu.io/upload_images/1148179-bbc93a2c148f7549.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Realistic Cocoa MVC
Cocoa MVC 鼓励你写重控制器是因为它们互相之间在视图的生命周期中互相牵扯，以至于很难将它们分开。虽然你也可能有一些办法把一些业务逻辑和数据转模型的工作放到 Model 中来，但是对于把负担分摊到 View 上却没有什么办法，大多数情况中，View 的所有功能就是给 Controller 发送动作（action），而 Controller 则会成为所有东西的代理或者数据源，并且通常会发送或者取消网络请求等等等等，你可以尽情想象。
你应该经常看到这种代码：
```
var userCell = tableView.dequeueReusableCellWithIdentifier("identifier") as UserCell
userCell.configureWithUser(user)
```
cell 作为一个视图直接通过 Model 进行配置，MVC 的原则被违反了，但是这种情况一直在发生，并且大家也没觉得有什么错了。如果你严格的遵守 MVC，那么你就会想要通过 Controller 对 cell 进行配置，并且不把 Model 传进 View 中，然而这种行为会更进一步的增加 Controller 的负担。
> 
把 MVC 称作重控制器模式还是挺有道理的。
问题也许直到[单元测试](http://nshipster.com/unit-testing/)的时候才暴露出来（但愿在你的项目中是这样的）。你的控制器紧紧和视图耦合在一起，你需要非常有创造力才能模拟这些视图和它们的生命周期，所以会变非常难以测试，而且用这种方式编写代码，你的业务逻辑代码也会尽可能的和页面布局代码分分开。
让我们来看下面一段 playground 例子：
```
import UIKit
struct Person { // Model
    let firstName: String
    let lastName: String
}
class GreetingViewController : UIViewController { // View + Controller
    var person: Person!
    let showGreetingButton = UIButton()
    let greetingLabel = UILabel()
    override func viewDidLoad() {
        super.viewDidLoad()
        self.showGreetingButton.addTarget(self, action: "didTapButton:", forControlEvents: .TouchUpInside)
    }
    func didTapButton(button: UIButton) {
        let greeting = "Hello" + " " + self.person.firstName + " " + self.person.lastName
        self.greetingLabel.text = greeting
    }
    // layout code goes here
}
// Assembling of MVC
let model = Person(firstName: "David", lastName: "Blaine")
let view = GreetingViewController()
view.person = model;
```
> 
MVC 架构可以在视图控制器中进行（各层次的）拼接组装
这个看起来不好测试吧？我们可以把 greeting 的生成方法移到一个新的 GreetingModel 类中单独测试，但是我们在不直接调用 UIView 的相关方法（viewDidLoad, didTapButton）的前提下，没有办法测试所有的业务逻辑，这些方法有可能加载所有 View，这个对于单元测试来讲是不利的。
事实上，在一个模拟器上（例如：iPhone 4S）测试 UIViews 无法保证在其他设备上（例如：iPad）也运行良好，所以我建议在你的 Unit Test target 配置中去掉Host Application，让你的测试用例不要在程序跑在模拟器上的时候运行。
> 
View 和 Controller 之间的交互在单元测试中是无法被真正测试的。
从上面来看，Cocoa MVC 是一个非常不好的选择，但是让我们就这文章开头提到那些特征而言来对它进行一个评估：
- 划分（Distribution）--View 和 Model 确实是分开了，但是View 和 Controller 紧紧的联系在一起
- 可测试性--由于功能划分的不好，你可能只能测试你的 Model
- 易用性--与其他模式相比代码量最小，另外，每个人都对他很熟悉，即使是一个不是非常有经验的开发者也能进行维护
如果你不希望花费特别多的时间在你的架构上，并且你觉得对于你的小项目来说，更高的维护成本并不值得的话，Cocoa MVC 是你要选择的模式。
> 
就开发速度而言，Cocoa MVC 是最好的架构模式。
## MVP
### Cocoa MVC’s promises delivered
![](http://upload-images.jianshu.io/upload_images/1148179-c9ff8bbdf04891ac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Passive View variant of MVP
它看上去是不是非常像 Apple’s MVC ？是的，确实很像，并且叫做[MVP](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter)（Passive View variant）。但是等一下，这意味着 Apple’s MVC 实质上就是 MVP 吗？不是的，还记得 View 是紧紧和 Controller 联系在一起的吧，而在 MVP
 中，作为中转站的 Presenter 与视图控制器的生命周期没有任何关联，并且 View 很容易被模拟，所以在 Presenter 中没有任何页面布局的代码，但是 Presenter 有责任通过数据和状态来更新 View。
![](http://upload-images.jianshu.io/upload_images/1148179-c7899f02eff7b0b0.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
如果我告诉你，UIViewController 是 UIView 会怎么样？
就 MVP 而言，UIViewController的那些子类实际上是视图而不是 Presenters。这种区别提供了极好的可测性，这是以开发速度为代价的，因为你必须手工准备数据和做事件绑定，你可以看下面这个例子：
```
import UIKit
struct Person { // Model
    let firstName: String
    let lastName: String
}
protocol GreetingView: class {
    func setGreeting(greeting: String)
}
protocol GreetingViewPresenter {
    init(view: GreetingView, person: Person)
    func showGreeting()
}
class GreetingPresenter : GreetingViewPresenter {
    unowned let view: GreetingView
    let person: Person
    required init(view: GreetingView, person: Person) {
        self.view = view
        self.person = person
    }
    func showGreeting() {
        let greeting = "Hello" + " " + self.person.firstName + " " + self.person.lastName
        self.view.setGreeting(greeting)
    }
}
class GreetingViewController : UIViewController, GreetingView {
    var presenter: GreetingViewPresenter!
    let showGreetingButton = UIButton()
    let greetingLabel = UILabel()
    override func viewDidLoad() {
        super.viewDidLoad()
        self.showGreetingButton.addTarget(self, action: "didTapButton:", forControlEvents: .TouchUpInside)
    }
    func didTapButton(button: UIButton) {
        self.presenter.showGreeting()
    }
    func setGreeting(greeting: String) {
        self.greetingLabel.text = greeting
    }
    // layout code goes here
}
// Assembling of MVP
let model = Person(firstName: "David", lastName: "Blaine")
let view = GreetingViewController()
let presenter = GreetingPresenter(view: view, person: model)
view.presenter = presenter
```
### 关于组装（assembly）的重要说明
MVP 有三个真正分开的层（three actually separate layers），所以可能会引起拼装的问题，MVP 是第一个披露有这种问题的架构模式。我们不希望 View 和 Model 进行联系，所以在当前的 view controller（其实就是 View） 中进行组装是不正确的，所以我们需要在别的地方做这件事情。例如，我们可以做一个app 范围（app-wide）内的路由服务，由它来处理组装的工作和视图到视图的呈现（presentation）。这个问题不仅仅是在 MVP 中有，下面介绍的所有架构模式都存在这个问题。
我们来看一下 MVP 的特点：
- 划分（distribution）--大部分的任务都被划分到 Presenter 和 Model 中，而 View不太灵活（例子中的 Model 也不太灵活）
- 可测试性--非常出色，我们可以通过 View 来测试大部分的业务逻辑
- 易用性--在我们那个不切实际的小例子里，MVP 的理念是非常清晰的，但是代码量是 MVC 模式的两倍
> 
MVP 在 iOS 中使用意味着非常好的可测试和非常多的代码
### With Bindings and Hooters
MVP 还有另外一种特色（的变种）--Supervising Controller MVP（SoC MVP）。这个变种包括了 View 和 Model 的的直接绑定，与此同时 Presenter（The Supervising Controller）仍然控制对 View 上操作的响应，并且能够改变 View （的展示）。
![](http://upload-images.jianshu.io/upload_images/1148179-d000e02e1bb5c53e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Supervising Presenter variant of the MVP
像前面我们认知的那样，模糊的职责划分以及 View 和 Model 的紧耦合是一件不好的事情，这个也跟 Cocoa 桌面端程序开发相似。
跟传统的 MVC 一样，我对于有着致命错误的架构没什么兴趣写一个例子。
## MVVM
### 最新的也是最好的MV(X)种类
[MVVM](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93viewmodel) 是最新的 MV(X) 系列的架构，让我们希望它出现的时候已经考虑到了 MV(X) 以前遇到的那些问题。
从理论上来看，Model-View-ViewModel 的架构模式看起来不错，View 和 Model 对于我们来说都比较熟悉了，但是对于中转站这个角色是由 ViewModel 来充当。
![](http://upload-images.jianshu.io/upload_images/1148179-0ee7870e02cc072b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MVVM
它和 MVP 非常像：
- MVVM 把 View Controller 作为 View
- View 和 Model 之间没有紧耦合
另外，MVVM 在数据绑定方面有点像 SoC 版本的 MVP，但是不是在 View 和 Model 之间进行绑定，MVVM 是在 View 和 ViewModel 之间进行绑定。
那么，在 iOS 的开发场景中，什么是 ViewModel 呢？基本上来说，它是 View 和 View 状态的独立于 UIKit 外的一个呈现，ViewModel 调用 Model 中的的变化，根据 Model 的变化进行调整，并且通过 View 和 ViewModel 的绑定，同步调整 View。
### 绑定
虽然我简单的在 MVP 的部分提到过这个内容，但是我们还是要在这里对这个话题进行一下讨论。“绑定”来源于 OS X 开发，但是在 iOS 的工具中并没有这个。虽然我们有 KVO 和通知机制，但是都不如绑定来的方便。
那么如果我们不想自己写一套的话，我们有两种选择：
- 选一个基于 KVO 的绑定框架，如：[RZDataBinding](https://github.com/Raizlabs/RZDataBinding)或者[SwiftBond](https://github.com/SwiftBond/Bond)
- 使用全量级的函数式响应编程的框架，如[ReactiveCocoa](https://github.com/ReactiveCocoa/ReactiveCocoa)、[RxSwift](https://github.com/ReactiveX/RxSwift/)或者[PromiseKit](https://github.com/mxcl/PromiseKit)
实际上，现在我们提起到 MVVM 就会想到 ReactiveCocoa，反之亦然。虽然，我们可以通过简单的绑定来使用 MVVM，但是 RAC（或者同类框架）能够让你更大程度的使用 MVVM。
关于响应式的框架有一个问题，就像更大权利对应着更大的责任，使用响应式的变成，非常容易把事情搞得一团糟，换句话说如果你出了错误，可能会需要花非常多的时间去进行调试，看看下面一张调用堆栈图你就明白了：
![](http://upload-images.jianshu.io/upload_images/1148179-fd6859e571ec0cdb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Reactive Call Stack
在我们下面这个简单的例子中，无论是响应式编程的框架还是 KVO 都太重了，我们用另外一种方式进行替代：通过 *showGreeting* 方法以及利用
*greetingDidChange* 方法的闭包参数，直接调用 ViewModel。
```
import UIKit
struct Person { // Model
    let firstName: String
    let lastName: String
}
protocol GreetingViewModelProtocol: class {
    var greeting: String? { get }
    var greetingDidChange: ((GreetingViewModelProtocol) -> ())? { get set } // function to call when greeting did change
    init(person: Person)
    func showGreeting()
}
class GreetingViewModel : GreetingViewModelProtocol {
    let person: Person
    var greeting: String? {
        didSet {
            self.greetingDidChange?(self)
        }
    }
    var greetingDidChange: ((GreetingViewModelProtocol) -> ())?
    required init(person: Person) {
        self.person = person
    }
    func showGreeting() {
        self.greeting = "Hello" + " " + self.person.firstName + " " + self.person.lastName
    }
}
class GreetingViewController : UIViewController {
    var viewModel: GreetingViewModelProtocol! {
        didSet {
            self.viewModel.greetingDidChange = { [unowned self] viewModel in
                self.greetingLabel.text = viewModel.greeting
            }
        }
    }
    let showGreetingButton = UIButton()
    let greetingLabel = UILabel()
    override func viewDidLoad() {
        super.viewDidLoad()
        self.showGreetingButton.addTarget(self.viewModel, action: "showGreeting", forControlEvents: .TouchUpInside)
    }
    // layout code goes here
}
// Assembling of MVVM
let model = Person(firstName: "David", lastName: "Blaine")
let viewModel = GreetingViewModel(person: model)
let view = GreetingViewController()
view.viewModel = viewModel
```
我们再对 MVVM 的几个特征进行一下评估：
- 划分（distribution）--也许在我们的小例子中表现得不是太清楚，但是实际上 MVVM 的 View 比 MVP 的 View 要做的时期要多一些，因为，通过设置绑定，第一个 View 由 ViewModel 来更新状态，然而第二个只需要将所有事件传递到 Presenter 就行了，不需要更新它的状态
- 可测试性-- ViewModel 并不持有 View，这让我们可以轻松的对它进行测试，View 也可以进行测试，但是它是依赖于 UIKit 的，你可能会忽略它
- 易用性--在我们的例子里，MVVM 的代码量跟 MVP 差不多，但是在实际的 app 中，需要把事件从 View 传递到 Presenter ，并且要手动的更新 View，如果使用绑定的话，MVVM 将会瘦身不少
> 
MVVM 非常具有吸引力，因为它结合了上述几种框架方法的好处，另外得益于 View 中的绑定机制，它也不需要额外的代码，并且可测试性也处在一个相当不错的层次。
## VIPER
### 从乐高玩具的搭建经验转换到 iOS app 的设计
[VIPER](https://www.objc.io/issues/13-architecture/viper/)是我们最后一个要介绍的架构，它不是MV(X)系列的架构。
到现在为止，我们应该都觉得职责划分的颗粒度还是不错的。在 VIPER 中对于职责的划分提出了另一种方式，这次我们有五层：
![](http://upload-images.jianshu.io/upload_images/1148179-21867694c59fded7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
VIPER
- Interactor--包括和数据相关的业务逻辑（Entities）或者网络请求，比如创建entities 类的对象或者把它们从服务器中抓取出来。为了达到这些目的你会用到通常会被看做外部依赖而不被看做 VIPER 单元的一些服务（Services）和管理者（Managers）
- Presenter--包括 UI 相关（UIKit 之外）的一些业务逻辑，调用 Interactor 中的一些方法
- Entities--纯粹的数据对象，并非是数据访问层，数据访问是 Interactor 层的任务
- Router--负责 VIPER 模块之间的切换
> 
对于 iOS 应用来说，用一个合适的方式doing routing是一个挑战，然而 MV(X) 模式并没有定位到这个问题。
下面的例子没有涉及到 routing 或者单元之间的相互作用，反正这些问题在 MV(X) 中也没有被涉及。
```
import UIKit
struct Person { // Entity (usually more complex e.g. NSManagedObject)
    let firstName: String
    let lastName: String
}
struct GreetingData { // Transport data structure (not Entity)
    let greeting: String
    let subject: String
}
protocol GreetingProvider {
    func provideGreetingData()
}
protocol GreetingOutput: class {
    func receiveGreetingData(greetingData: GreetingData)
}
class GreetingInteractor : GreetingProvider {
    weak var output: GreetingOutput!
    func provideGreetingData() {
        let person = Person(firstName: "David", lastName: "Blaine") // usually comes from data access layer
        let subject = person.firstName + " " + person.lastName
        let greeting = GreetingData(greeting: "Hello", subject: subject)
        self.output.receiveGreetingData(greeting)
    }
}
protocol GreetingViewEventHandler {
    func didTapShowGreetingButton()
}
protocol GreetingView: class {
    func setGreeting(greeting: String)
}
class GreetingPresenter : GreetingOutput, GreetingViewEventHandler {
    weak var view: GreetingView!
    var greetingProvider: GreetingProvider!
    func didTapShowGreetingButton() {
        self.greetingProvider.provideGreetingData()
    }
    func receiveGreetingData(greetingData: GreetingData) {
        let greeting = greetingData.greeting + " " + greetingData.subject
        self.view.setGreeting(greeting)
    }
}
class GreetingViewController : UIViewController, GreetingView {
    var eventHandler: GreetingViewEventHandler!
    let showGreetingButton = UIButton()
    let greetingLabel = UILabel()
    override func viewDidLoad() {
        super.viewDidLoad()
        self.showGreetingButton.addTarget(self, action: "didTapButton:", forControlEvents: .TouchUpInside)
    }
    func didTapButton(button: UIButton) {
        self.eventHandler.didTapShowGreetingButton()
    }
    func setGreeting(greeting: String) {
        self.greetingLabel.text = greeting
    }
    // layout code goes here
}
// Assembling of VIPER module, without Router
let view = GreetingViewController()
let presenter = GreetingPresenter()
let interactor = GreetingInteractor()
view.eventHandler = presenter
presenter.view = view
presenter.greetingProvider = interactor
interactor.output = presenter
```
让我们回过头再来看一下它的特性：
- 划分（distribution）--毫无疑问，在层次职责划分方面，VIPER 是最棒的
- 可测试性--理所当然的，非常好的层次划分带来好的可测试性
- 易用性--想你想的那样，上述两个方面都牺牲（系统）可维护性的，你需要编写许多的仅有少量功能的接口
### 那乐高又是什么鬼？
当使用 VIPER 作为架构的时候，你可能会感觉在使用乐高积木搭建帝国大厦，这标志着你也许遇到了问题。可能对于你的 app 来说还没到采用 VIPER 的时候，你应该把一些事情考虑的简单一点。许多人忽略了这一点，然后继续用大炮打蚊子（麻雀）。我猜想可能是因为，他们相信虽然现在的维护成本高的不合理，但是他们的 app 至少会在将来从 VIPER 架构上获得收益。如果你也是像他们一样考虑的，我推荐你试一下[Generamba](https://github.com/rambler-ios/Generamba)--一个生成
 VIPER 框架的工具。虽然我个人理解也就相当于在大炮上装了一个自动瞄准系统。
## 总结
我们简单了解了几个架构模式，对于那些困扰你的问题，希望你已经找到了答案，但是毫无疑问，你已经意识到了（架构的选择）没有什么银色子弹（喻指杀手锏），其实架构模式的选择就是对于你的特定处境的权衡考量。
因此在同一个 app 中存在着多个架构也是一件比较正常的事情，举个例子来说，一开始项目用的 MVC 架构，然后你发现有一个特殊的场景如果使用 MVC 会难以维护，你可以在仅仅只这个场景中使用 MVVM。没必要对于那些 MVC 架构执行的很好的模块进行重构，因为这两种架构兼容的很好。
> 
Make everything as simple as possible, but not simpler
-- Albert Einstein
文／CoCodeDZ（简书作者）
原文链接：http://www.jianshu.com/p/87ac2f075a5b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
