# 手势识别（一）--手势基本概念和ChaLearn Gesture Challenge - 工作笔记 - CSDN博客





2016年10月05日 20:54:25[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：15979
个人分类：[手势识别](https://blog.csdn.net/App_12062011/article/category/7337142)

所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿以下转自：

[http://blog.csdn.net/qq1175421841/article/details/50312565](http://blog.csdn.net/qq1175421841/article/details/50312565)

像点击(clicks)是GUI平台的核心，轻点(taps)是触摸平台的核心那样，手势(gestures)是Kinect应用程序的核心。和图形用户界面中的数字交互不同，手势是现实生活中存在的动作。如果没有电脑我们就不需要鼠标，但是没了Kinect，手势依然存在。从另一方面讲，手势是日常生活中人与人之间相互交流的一部分。手势能够增强演讲的说服力，能够用来强调和传递情感。像挥手(waving)或者指向(pointing)这些手势都是某种无声的演讲。


    Kinect应用程序的设计和开发者的任务就是将这些现实生活中存在的手势映射到计算机交互中去以传达人的想法。尝试从鼠标或触摸式的GUI设计移植基于手势的自然交互界面要做很多工作。借鉴过去30多年来对于这一概念的研究，以及从一些Kinect for Xbox的体感游戏中获取一些设计理念，计算机工程师和交互设计师一起为Kinect创建了一系列新的手势库。


    本文将会介绍用户体验的一些知识，并讨论如何将手势应用到Kinect应用程序中。我们将展示Kinect如何作为自然交互界面(Natural User Interface)的人机交互模型的一部分。我们将讨论一些具体的使用Kinect来进行手势识别及交互的例子。更重要的是，将会展示一些已经作为Kinect手势识别库中的手势

## 1. 什么是手势


    在许多不同的学科中，手势(gesture)有着其独特的含义，可能这些含义之间有某些异同。在艺术领域，手势被用来传达舞蹈中最富表现力的部分，特别是在亚洲舞蹈艺术中，手势被作为某些宗教符号或者象征。在交互设计领域，在基于触摸的自然交互界面中手势和操控有很大区别。


    以上这些说明手势在不同的学科领域都有自己独特的含义。在学术领域都试图对手势定义一个抽象的概念。在用户体验设计领域使用最广泛的关于手势的定义实在Eric Hulteen 和Gord Kurtenbach 1990年发表的一篇名为人机交互中的手势(Gestures in Human-Computer Communication),定义如下：”手势是身体的运动，他包含一些信息。挥手道别是一种手势。敲击键盘不是手势，因为用手指的运动去敲击按键没有被观察，也不重要，他只表达的键盘被按下这一动作。(A gesture
 is a motion of the body that contains information. Waving goodbye is a gesture. Pressing a key on a keyboard is not a gesture because the motion of a finger on its way to hitting a key is neither observed nor significant. All that matters is which key was
 pressed)”


    这个定义既解释了什么是手势也解释了什么不是手势。像这样的下一个正式的定义通常有两个方面的困难，既要避免太具体也要避免太抽象。如果一个定义太具体-如，定义某项技术-可能会随着UI技术的变化会变得模糊不清。作为一种学术定义而不是以常见的用法为基础的定义，它也必须足够一般，并且符合或者说广大的研究机构先前已发表在HCI的研究成果及艺术中符号学。另一方面，定义过于宽泛，也会有有无关紧要的风险：如果一切都是一种姿态，那么就什么都不是了。


    Eric Hulteen 和Gord Kurtenbach关于手势的定义的中心在于手势能够用来交流，手势的意义在于讲述而不是执行。


有趣的是将语言和行为引入到人机交互接口中来，这是一种彻底的变革。我们与计算机交互语音变为无声的语言(mute)：我们通过指向和手势而不是语言与计算设备进行沟通。当和计算机进行交互时，我们点击键盘按键或触摸屏幕。我们似乎更喜欢这种形式的静音通信即使当前的技术能够支持更简单的语音指令。我们没有操作(manipulation)的力量，和虚拟的对象而不是真实的物体进行交互，因而没有持久性。运动成为纯粹的手势。


    基于Eric Hulteen 和Gord Kurtenbach的定义，我们都明白什么是 UI 操作 ——暂时不是一种手势 ——理解什么是手势以及手势表示"重大"行为或者符号仍然有很大的困难。移动交互的含义是什么？手势进行沟通和语言进行沟通的最明显不同是什么？我们做手势的象征意义往往很抽象简单。


    在人机交互领域，手势通常被作为传达一些简单的指令而不是交流某些事实、描述问题或者陈述想法。使用手势操作电脑通常是命令式的，这通常不是人们使用手势的目的。例如，挥手(wave)这一动作，在现实世界中通常是打招呼的一种方式，但是这种打招呼的方式在人机交互中却不太常用。通常第一次写程序通常会显示“hello”,但我们对和电脑打招呼并不感兴趣。


    但是，在一个繁忙的餐馆，挥手这一手势可能就有不同的含义了。当向服务员招收时，可能是要引起服务员注意，需要他们提供服务。在计算机中，要引起计算机注意有时候也有其特殊意义，比如，计算机休眠时，一般都会敲击键盘或者移动鼠标来唤醒，以提醒计算机“注意”。当使用Kinect时，可以使用更加直观的方式，就行少数派报告阿汤哥那样，抬起双手，或者简单的朝计算机挥挥手，计算机就会从休眠状态唤醒。


    在人机交互领域，手势通常有一些含义，表示有意让某些事情发生。手势是一种指令。当通过鼠标或者触控板去点击UI界面上的按钮时，我们希望按钮会触发其背后的事件。通常，按钮上会有一个标签来指示按钮的功能如：开始、取消、打开、关闭。我们的手势操作就是想要实现这些事件。


    上面的定义中的第一点可以得出，手势的另一个特点是比较随意(arbitrary)。手势有限定的领域，那么在该领域之外没有任何意义。令人惊讶的是除了指向(pointing)和耸肩(shurg)，人类学家没有发现任何东西我们可以称之为一种通用的手势。然而，在计算机的UI中，指向(pointing)通常被认为是直接操作因为它牵涉到跟踪，同时耸肩的含义太微妙而不好辨识。因此，我们想要使用的任何Kinect手势必须基于应用程序的用户 和应用程序的设计和开发者之间就某种手势代表的含义达成一致。


    因为手势是任意的(arbitrary)所以他们也是基于约定的(conventional)。应用程序的设计者必须告诉用户正在使用的手势的意义，或者是这些手势是约定俗称大家都知道的。此外，这些约定不是基于语言文化，而是对已确定的技术规则。我们知道如何使用鼠标 （行为学习） 并不是因为这是我们已经从我们的文化导入的东西，而是因为这是基于特定的图形用户界面的跨文化约定。同样地，我们知道如何点击或滑动智能手机，不是因为这些都是文化的约定，而是因为这些都是跨文化自然用户界面项约定。有趣的是，我们在一定程度上知道如何点击平板电脑，因为我们以前学习了如何使用鼠标单击。技术约定之间可以相互转化，这是因为语言和手势可以通过不同的语言和文化之间来转换。


    然而，手势的这种任意性和基于约定的特性也带来了误解性（misunderstanding），这是在设计任何用户界面，尤其是像Kinect这样的没有任何预先设定好的操作约定的用户界面时需要关注的风险。就像在有些国家，点头表示否定摇头表示可能。手势，或者任何身体的运动，都有可能产生误解。


   总之，在人机交互领域，手势是：
- 表达一种简单的命令
- 天生有随意性
- 基于某种协定
- 可能被误解


注意：实际的直接操作（manipulation）不是手势。

## 2. 自然交互界面(NUI)


   讨论手势而不讨论自然用户界面显然不完整。自然用户界面是一系列技术的合计，他包括：语音识别，多点触控以及类似Kinect的动感交互界面，他和Windows和Macs[操作系统](http://lib.csdn.net/base/operatingsystem)中鼠标和键盘交互这种很常见图形交互界面不同。就像图像交互界面和之前的命名行交互界面不同那样。


    自然交互界面自然在哪儿呢？早期自然交互界面的发起者认为交互界面的设计应该对用户非常直观，使用用户先天就会的行为来进行交互操作。他的目标是不需要操作由图标和菜单构成的基于GUI 的应用程序界面，因为这种界面通常具有陡峭的学习曲线。相反，理想化的状态是，用户应该能够走到应用程序前面，就能够开始使用它。在过去的几年里随着触摸功能的智能手机和平板电脑的流行，逐渐取代了键盘鼠标，当我们看到孩子们开始走到任何触摸屏设备面前，用手去触摸它，期待它的响应，在这一点上看这一理念已经实现。


    虽然自然用户界面的自然性似乎是直接操作的最佳写照，当使用手指来进行触摸交互时，先天自然和后天学习行为之间的对立被打破。一些手势，如轻触屏幕，在某种意义上就是先天就会的动作。其他的动作比如说双击，获得点击然后拖拉等，没有先天就会。而且随着不同的设备制造商开始支持不同触摸手势，为了使得相同的手势在不同的触摸平台上有相同的意义和行为，为某些手势定义一些约定显得更加重要。


    自然用户界面（NUI）的自然性更多的是一种相对自然的概念。对于NUI的更现代的理解受Bill Buxton所影响。他认为NUI界面的设计充分利用了用户预先就会的技能，用户和UI进行交互感到很自然，使得他们甚至忘了是从哪里学到这些和UI进行交互所需的技能的。换句话说，第一次操作时，我们不记得我们曾经学过这些知识。例如，轻点(tap)这个手势早平板电脑和手机中使用的很频繁，这个技能是从我们之前在传统的人机交互界面上使用鼠标来指向并点击某一个界面上的元素学来的。点击（click）和轻点（tap）的最主要区别在于，点击需要鼠标，对于触摸屏，不需要额外的设备，只需要用手指轻轻触摸一下屏幕就可以了。


    这引出了自然用户界面的另一个特点。用户和计算机之间的交互看起来不需要任何媒介，这种相互作用的媒介是不可见的。例如，在语音识别界面中，人机交互是通过具有复杂电子过滤去噪的麦克风实现的，其内部有解析发音语义单元的各种[算法](http://lib.csdn.net/base/datastructure)，将这些语义传递给其它软件来进行将特定的短语解释为命令，并将该命令映射到某种软件功能操作。但是，内部的这一切，对用户是不可见的。当用户对计算机发出这样的命令，"嘿，注意我"，她会认为计算机会像类似大多数人的本能那样的响应这个命令。


    自然用户界面的 依赖于先验知识和不需要媒介的交互这两个特征是每一种NUI界面的共同特征，其他方面如触摸，语音和动态交互界面则因设备的不同而各异。目前，大多数关于NUI的设计都是基于多点触控体验的。这就是为什么前面对于手势的标准定义是那样定义的。它是将多点触摸的场景进行修改并将手势和操作区分开来。


    关于手势（gesture）和操作（manipulation）的争论也存在于语音交互界面中，命令等同于手势，语音等同于直接操作，在动态交互界面中，将手或者身体追踪展示在可视化界面上手和身体的运动等同于直接操作。自由形式的运动像挥手这一动作就属于手势。


    但是Kinect还有第三种交互界面，他和触摸和语音交互不同。那就上一篇文章中所讲的姿势（pose），姿势是身体的某一部分和其他部分之间的一种静态关系，他不是运动的。Kinect中的姿势和日常生活中的姿势是一样的，例如，左臂伸出45度表示将当前的窗口变为活动的交互窗体，右臂伸出45度或者135度表示垂直滚动工具栏。


    另外，交互方式可以从一种类型的交互界面转换到另外一种交互界面。以按钮为例，按钮其实就是一个符号，这是一个先验的图形用户界面。从最基本的功能来讲，按钮就是一个通过鼠标点击在一个可视化元素的文字或者图像上触发一些命令的工具。在过去15年，按钮被作为人机交互界面的一个集成部分，被转换到多点触摸界面，以及Kinect用户界面中来。


    自然用户界面设计师所追求的是的是自然，按钮恰好提供了这一点。但是按钮在每一种用户界面中的转换都面临着一些挑战。


图形用户界面中按钮的一个通常的特征是他提供了一个悬浮状态来指示用户光标已经悬停在的按钮上方的正确位置。这种悬浮状态将点（click）这个动作离散开来。悬浮状态可以为按钮提供一些额外的信息。当将按钮移植到触摸屏交互界面时，按钮不能提供悬浮状态。触摸屏界面只能响应触摸。因此，和电脑上的图像用户界面相比，按钮只能提供“击”（click）操作，而没有“点”（point）的能力。


    当将按钮移植到基于Kinect的用户界面上时，按钮的行为就变得更加特殊了。基于Kinect的图形界面中，按钮的行为和触摸界面中的刚好相反，他只提供了悬浮（hover）的“点”（point）的能力，没有“击”（click）的能力。按钮这种更令用户体验设计者感到沮丧的弱点，在过去的几年里，迫使设计者不断的对Kinect界面上的按钮进行改进，以提供更多巧妙的方式来点击视觉元素。这些改进包括：悬停在按钮上一段时间、将手掌向外推(笨拙地模仿点击一个按钮的行为）等。


    虽然触摸界面也有手势，但Kinect 界面有些互动不是手势，不过软件的开发和设计者倾向于以 Kinect 手势操作作为交互界面。这似乎是因为使用手势作为物理操作是 Kinect 应用程序的最大的特点。与此相反的是，触摸界面的突出特点是直接操作。虽然可能不准确，人们通常将自然交互界面划分为三类：语音交互界面，触摸交互界面和手势交互界面。


   然而，在关于Kinect的相关介绍文档中，你会发现有时候姿势(pose)和操作(manipulation)都被描述为手势。这些都没有错。要记住的是，当我们讨论Kinect中的一些术语，如挥手(wave),滑动(swipe),我们会作为纯粹的手势，而姿势和操控只有在隐喻意义上才称之为手势。


    以上的讨论都很重要，因为我们会进一步设计Kinect互动的语意，我们将最终移除从其他图形界面上借鉴过来的关于按钮的语意，然后尝试建立基于Kinect的先验的语意。挥手(wave)这是Kinect中纯粹的手势，是最早的这种尝试。乔治亚技术研究所的研究人员正在利用 Kinect 来解释美国手语。相反，其他研究人员，正在利用 Kinect 解释身体语言——另一种预先形成的手势和姿势的沟通。诸如此类的研究可以视为对于NUI的第二层研究。这些逐渐接近了最初NUI人机交互的原始的梦想，不只是看不见，而且NUI能够自适应以理解我们的行为，而不是迫使我们了解我们和电脑的人机交互。

## 3. 手势从哪里来


    在手势交互界面中，纯粹的手势，姿势和追踪以及他们之间的组合构成了交互的基本术语。对于Kinect来说，目前可以使用的有8个通用的手势：挥手(wave)，悬浮按钮(hover button),磁吸按钮(magnet button),推按钮(push button)，磁吸幻灯片(magnetic slide)，通用暂停(universal pause)，垂直滚动条(vertical scrolling)和滑动(swipping)。其中的一些术语是微软自己引入的，有一些是游戏代理商设计的，还有一些是Kinect
 for PC开发人员为了开发应用而引入的。


    很少情况下会为人际交互界面术语进行定制。通常要将这8种手势区分开来，并在一些应用中通用也不常见。相似的情况在web术语和手机手势中设计新的界面时也会遇到，其中只有部分的设计能够变成标准。在网页设计领域，走马灯和光标动画流行一时，并在一片鄙夷声中迅速消失。在手机设计领域由于苹果公司在触摸屏领域的早期地位这种术语得到了很好的规范。苹果引入了一些触摸手势术语，如轻点（tap）,点住不放(tap and hold)，滑动swipe及pinch。

![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204212305297469.png)


    交互术语形成规范有几个障碍。第一个就是为了获得利益而避免标准化。在90年代后期的浏览器大战中，尽管各大厂商在口头上说标准化协议很重要，但是在浏览器开发上依旧不停的开发自己的HTML版本，以吸引开发者使用他们的技术。设备制造商可以利用市场占有率的优势来锁定消费者，通过在他们的手机上实现自己定义语意的触屏，来推行自己的手势操作。这些都是不自然的行为，因为不同厂商对于同一手势的语意都不同，并且他们看起来不自然，使用不同厂商的产品需要再学习。


    另一种形成规范化的障碍是上下文手势的专利。例如，苹果公司不能对“滑动”(swipe)操作申请专利，但是它可以对“滑动解锁手机”这个手势申请专利，这使得其他公司需要使用这一技术或者设计理念时要么给苹果公司支付专利费，要么将苹果告上法庭以避免专利费，或则干脆不使用这一上下文手势。如果不使用这一上下文手势，那么产品就破坏了之前我们学习到使用很自然的方式滑动解锁手机，音乐播放器，平板电脑等这一约定了。


    最后一个障碍是，设计一个手势很困难。手势术语会面对一些App Store中手机应用程序和YouTube中视频应用所遇到的一些问题：人们要么会要么不会。手势需要思考如何定义的简单使得人们能够去用，这就是长尾理论留下来的问题。


    那么什么样的手势术语才是好的呢。如果一个手势易于使用，那么他就被认为是设计良好的。在交互设计中，易用性有两个方面：可用(affordance)和反馈(feedback)。反馈就是说用户知道当前正在进行的操作。在网页中,点击按钮会看到按钮有一点偏移,这就表示交互成功。鼠标按键按下时的声音在某种意义上也是一种反馈,他表示鼠标在工作。对于Winodw Phone Metro风格的界面上的磁贴，开发这认为这些按钮应该足够大，以容下大面积的触摸区域，但是他们也认为过大的触摸区域会使得用户触摸到区域外面也会触发注册的事件。另外，状态信息或者确认对话框会在应用程序中弹出以提示用户发生了一些事情。在
 Xbox 的仪表板中，使用Kinect传感器产生的光标悬停在的热点上开始动画播放。


    如果说反馈发生在操作进行中或者之后，那么可用性(affordance)就发生在操作之前了。可用性就是一种提示或者引导，告诉用户某一个可视化元素是可以交互的，指示用户该元素的用处。在GUI交互界面中，按钮是能够最好的完成这些理念的元素。按钮通过文字或者图标提示来执行一些函数操作。GUI界面上的按钮通过悬浮状态可以提示用户其用途。最好的可用性-可能有点绕圈-就是约定俗成。用户知道某一个可视化元素的用途，因为之前在其他应用中使用过类似的可视化控件，或者是在其他设备中执行过类似的操作。但是，这一点对于基于Kinect的手势交互界面来说有点困难，因为一切都是新的。


    通常的做法就是使用借用其他类型交互界面中的约定。在触摸交互界面中，一个轻点(tap)手势和通常的鼠标点击是等同的。响应轻点事件的两个可视化元素，图标和按钮，也被设计的和传统的GUI界面上的图标和按钮一样，来达到提示用户该元素的作用这一目的。Kinect也使用按钮和图标来使得用户能够更加容易使用。因为Kinect基本上是基于”点”（pointing）而原生不支持“击”(clicking)。在此之前，软件界面设计者和开发者的花费了很多精力来对手势交互界面进行定制以实现“击”这一动作。


    和触摸交互界面不一样，手势交互界面可以从社会中人的一般手势中借用一些手势操作。这就使得挥手(wave)成为Kinect应用程序的经典手势。因为这一姿势和现实生活中的姿势有象征性联系使得非常容易理解和使用。轨迹追踪，虽然在技术上不是手势，但是他是另一个在现实生活中和指向有联系的术语。当在电视机或者显示器前挥动手时，好的Kinect应用程序应该能够追踪到手的运动，并显示一个光标随着手一起运动。当我们在现实生活中指向物体时，Kinect中的手部追踪显示的手形图标的反馈使得程序更加易用。


    目前，现实生活中的易用性手势在Kinect交互界面中用的比较少，大部分的易用性都是从传统的GUI界面上的可用性移植过来的。随着时间的改变，这一点会得到改善。在触摸屏设备上新的手势通过在传统的已经建立的约定中添加手指来形成。两指轻点和一指轻点有些不同，使用两个手指或者多个手指进行滑动有其独特的含义。最终，触摸手势全部由手指完成。另一方面，真正的手势用户界面，有一个近乎无限的语意库，使得我们可以基于现实生活中相关联的手势进行改进。


    本文接下来从理论到实现，讨论如何实现手势识别，并展示了Kinect中八中基本手势中的挥手(wave)手势的识别。

## 4. 实现手势识别


    Microsoft Kinect SDK并没有包含手势识别引擎。因此需要开发者来定义和手势识别。从SDK的Beta版放出以来，一些第三方开发者创建的手势引擎已初见端倪。但是，微软没有将他们作为标准的引擎。看来这可能还要等微软将手势识别引擎添加到SDK中来，或者指明可替代的解决方案。本节对手势识别技术进行了简单介绍，希望能够帮助开发者在标准的手势识别引擎出来之前，可以自己动手开发手势识别引擎。


    手势识别相对来说可以简单也可以很复杂，这取决与要识别的手势。有三种基本的方法可以用来识别手势：基于算法，基于神经网络和基于手势样本库。每一种方法都有其优缺点。开发者具体采用那种方法取决与待识别的手势、项目需求，开发时间以及开发水平。基于算法的手势识别相对简单容易实现，基于神经网络和手势样本库则有些复杂。

### 4.1 基于算法的手势识别


    算法是解决软件开发中几乎所有问题的最基本方法。使用算法的基本流程就是定义处理规则和条件，这些处理规则和条件必须符合处理结果的要求。在手势识别中，这种算法的结果要求是一个二值型对象，某一手势要么符合预定的手势要么不符合。使用算法来识别手势是最基本的方法，因为对于有一点编程能力的开发这来说，手势识别的代码易于理解，编写，维护和调试。


但是，最简单直接的方法也有其缺点。算法的简单性限制了其能识别到的手势的类别。对于挥手(wave)识别较好的算法不能够识别扔(throw)和摆(swing)动作。前者动作相对简单和规整，后者则更加细微且多变。可能能够写一个识别摆动(swing)的算法，但是代码可能比较费解和脆弱。

算法还有一个内在的扩展性问题。虽然一些代码可以重用，但是每一种手势必须使用定制的算法来进行识别。随着新的手势识别算法加入类库，类库的大小会迅速增加。这就对程序的性能产生影响，因为需要使用很多算法来对某一个手势进行识别以判断该手势的类型。


最后，每一个手势识别算法需要不同的参数，例如时间间隔和阈值。尤其是在依据流程识别特定的手势的时候这一点显得尤其明显。开发者需要不断测试和实验以为每一种算法确定合适的参数值。这本身是一个有挑战也很乏味的工作。然而每一种手势的识别有着自己特殊的问题。

### 4.2 基于神经网络的手势识别


    当用户在做手势时，手势的形式并不总是足够清晰到能够判断用户的意图。例如跳跃手势，跳跃手势就是用户短暂的跳起来，脚离开地面。这个定义不能够提供足够的信息来识别这一动作。


    咋一看，这个动作似乎足够简单，使得可以使用算法来进行识别。首先，考虑到有很多种不同形式的跳跃：基本跳跃（basic jumping）、 跨栏(hurdling)、 跳远(long jumping)、 跳跃(hopping)，等等。但是这里有一个大的问题就是，由于受到Kinect视场区域的限制，不可能总是能够探测到地板的位置，这使得脚部何时离开地板很难确定。想象一下，用户在膝盖到下蹲点处弯下，然后跳起来。手势识别引擎应该认为这是一个手势还是多个手势：下蹲或 下蹲跳起或者是跳起？如果用户在蹲下的时间和跳跃的时间相比过长，那么这一手势可能应被识别为下蹲而不是跳跃。


    看到这些，最开始对跳跃的定义就会变得模糊。这一姿势很难定义清楚，使得不能够通过定义一些算法来进行识别，同时这些算法由于需要定义过多的规则和条件而变得难以管理和不稳定。使用对或错的二值策略来识别用户手势的算法太简单和不够健壮，不能够很好的识别出类似跳跃，下蹲等动作。


    神经网络的组织和判断是基于统计和概率的，因此使得像识别手势这些过程变得容易控制。基于什么网络的手势识别引擎对于下蹲然后跳跃动作，80%的概率判断为跳跃，10%会判定为下蹲。


    除了能够识别复杂和精细的手势，神经网络方法还能解决基于算法手势识别存在的扩展性问题。神经网络包含很多神经元，每一个神经元是一个好的算法，能够用来判断手势的细微部分的运动。在神经网络中，许多手势可以共享神经元。但是每一中手势识别有着独特的神经元的组合。而且，神经元具有高效的[数据结构](http://lib.csdn.net/base/datastructure)来处理信息。这使得在识别手势时具有很高的效率。


    使用基于神经网络进行手势识别的缺点是方法本身复杂。虽然神经网络以及在计算机科学中对其的应用已经有了好几十年，建立一个好的神经网络对于大多数程序员来说还是有一些困难的。大多数开发者可能对数据结构中的图和树比较熟悉，而对神经网络中尺度和模糊逻辑的实现可能一点都不了解。这种缺乏建立神经网络的经验是一个巨大的困难，即使能够成功的构建一个神经网络，程序的调试相当困难。


    和基于算法的方法相比，神经网络依赖大量的参数来能得到精确的结果。参数的个数随着神经元的个数增长。每一个神经元可以用来识别多个手势，每一个神经远的参数的变化都会影响其他节点的识别结果。配置和调整这些参数是一项艺术，需要经验，并没有特定的规则可循。然而，当神经网络配对[机器学习](http://lib.csdn.net/base/machinelearning)过程中手动调整参数，随着时间的推移，系统的识别精度会随之提高。

### 4.3 基于样本的识别


   基于样本或者基于模版的手势识别系统能够将人的手势和已知的手势相匹配。用户的手势在模板库中已经规范化了，使得能够用来计算手势的匹配精度。有两种样本识别方法，一种是存储一系列的点，另一种方法是使用类似的Kinect SDK中的骨骼追踪系统。在后面的那个方法中，系统中包含一系列骨骼数据和景深帧数据，能够使用统计方法对产生的影像帧数据进行匹配以识别出已知的帧数据来。


这种手势识别方法高度依赖于机器学习。识别引擎会记录，处理，和重用当前帧数据，所以随着时间的推移，手势识别精度会逐步提高。系统能够更好的识别出你想要表达的具体手势。这种方法能够比较容易的识别出新的手势，而且较其他两种方法能够更好的处理比较复杂的手势。但是建立这样一个系统也不容易。首先，系统依赖于大量的样本数据。数据越多，识别精度越高。所以系统需要大量的存储资源和CPU时间的来进行查找和匹配。其次系统需要不同高度，不同胖瘦，不同穿着（穿着会影响景深数据提取身体轮廓）的样本来进行某一个手势。

## 5.识别常见的手势


    选择手势识别的方法通常是依赖于项目的需要。如果项目只需要识别几个简单的手势，那么使用基于算法或者基于神经网络的手势识别就足够了。对于其他类型的项目，如果有兴趣的话可以投入时间来建立可复用的手势识别引擎，或者使用一些人家已经写好的识别算法，接下来本文介绍几种常用的手势，并演示如何使用算法的方法来识别他们，手势识别的另外两种方法由于其复杂性本文不做介绍。


    不论选择哪种手势识别的方法，都必须考虑手势的变化范围。系统必须具有灵活性，并允许某一个手势有某个范围内的变动。很少有人能够每次都做一模一样的手势。例如，考虑周伯通当前左右手画圆圈这个手势，重复这一手势10次，圆形的中心每次都在一个点吗，圆形的起点和重点每次都在相同的地方吗？每次画圆的时长都一样吗？然后使用右手做这个动作，最后比较结果。或者拉几个朋友或者家人来做，然后观察。也可以站在镜子前面看自己做，或者使用录像设备录下来再看。技巧就是对于某一手势，让尽可能多的人来做，然后试图标准化这一手势。手势识别一个比较好的方式就是关注手势最核心的部分而不是哪些外在的细枝末节。

### 5.1 挥动(wave)手势


    只要玩过Xbox上的体感游戏，可能都使用过挥手这个手势。挥手这一手势不论年龄大小都能够做的一个简单动作。这是一个友好的，快乐的手势，人们通常挥手或者招手来打招呼或者道别。在应用开发的上下文中，挥手手势通常告诉应用程序已经准备好了，可以开始体验应用了。


    挥手是最简单最基本的手势。使用算法方法能够很容易识别这一手势，但是之前讲到的任何方法也能够使用。虽然挥手是一个很简单的手势，但是如何使用代码来识别这一手势呢？读者可以在镜子前做向自己挥手，然后仔细观察手的运动，尤其注意观察手和胳膊之间的关系。继续观察手和胳膊之间的关系，然后观察在做这个手势事身体的整个姿势。有些人保持身体和胳膊的不动，使用手腕左右移动来挥手。有些人保持身体和胳膊不动使用手腕前后移动来挥手。可以通过观察这些姿势来了解其他各种不同挥手的方式。


    XBOX中的挥手动作定义为：从胳膊开始到肘部弯曲。用户以胳膊肘为焦点来回移动前臂，移动平面和肩部在一个平面上，并且胳膊和地面保持平行,在手势的中部（下图1），前臂垂直于后臂和地面。下图展示了这一姿势。



![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204212305297502.png)


    从图中可以观察得出一些规律，第一个规律就是，手和手腕都是在肘部和肩部之上的，这也是大多是挥手动作的特征。这也是我们识别挥手这一手势的第一个标准。


    第一幅图展示了挥手这一姿势的中间位置，前臂和后臂垂直。如果用户手臂改变了这种关系，前臂在垂直线左边或者右边，我们则认为这是该手势的一个片段。对于挥手这一姿势，每一个姿势片段必须来回重复多次，否则就不是一个完整的手势。这一运动规律就是我们的第二个准则：当某一手势是挥手时，手或者手腕，必须在中间姿势的左右来回重复特定的次数。使用这两点通过观察得到的规律，我们可以通过算法建立算法准则，来识别挥动手势了。


    算法通过计算手离开中间姿势区域的次数。中间区域是一个以胳膊肘为原点并给予一定阈值的区域。算法也需要用户在一定的时间段内完成这个手势，否则识别就会失败。这里定义的挥动手势识别算法只是一个单独的算法，不包含在一个多层的手势识别系统内。算法维护自身的状态，并在识别完成时以事件形式告知用户识别结果。挥动识别监视多个用户以及两双手的挥动手势。识别算法计算新产生的每一帧骨骼数据，因此必须记录这些识别的状态。


    下面的代码展示了记录手势识别状态的两个枚举和一个结构。第一个名为WavePosition的枚举用来定义手在挥手这一动作中的不同位置。手势识别类使用WaveGestureState枚举来追踪每一个用户的手的状态。WaveGestureTracker结构用来保存手势识别中所需要的数据。他有一个Reset方法，当用户的手达不到挥手这一手势的基本动作条件时，比如当手在胳膊肘以下时，可调用Reset方法来重置手势识别中所用到的数据。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private enum </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WavePosition
</span></span>{
    None = 0,
    Left = 1,
    Right = 2,
    Neutral = 3
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private enum </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WaveGestureState
</span></span>{
    None = 0,
    Success = 1,
    Failure = 2,
    InProgress = 3
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private struct </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WaveGestureTracker
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public int </span></span>IterationCount;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WaveGestureState </span></span>State;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public long </span></span>Timestamp;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WavePosition </span></span>StartPosition;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WavePosition </span></span>CurrentPosition;

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>Reset()
    {
        IterationCount = 0;
        State = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WaveGestureState</span></span>.None;
        Timestamp = 0;
        StartPosition = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.None;
        CurrentPosition = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.None;
    }
}

    下面代码显示了手势识别类的最基本结构：它定义了五个常量：中间区域阈值，手势动作持续时间，手势离开中间区域左右移动次数，以及左手和右手标识常量。这些常量应该作为配置文件的配置项存储，在这里为了简便，所以以常量声明。WaveGestureTracker数组保存每一个可能的游戏者的双手的手势的识别结果。当挥手这一手势探测到了之后，触发GestureDetected事件。


当主程序接收到一个新的数据帧时，就调用WaveGesture的Update方法。该方法循环遍历每一个用户的骨骼数据帧，然后调用TrackWave方法对左右手进行挥手姿势识别。当骨骼数据不在追踪状态时，重置手势识别状态。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">WaveGesture
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private const float </span></span>WAVE_THRESHOLD = 0.1f;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private const int </span></span>WAVE_MOVEMENT_TIMEOUT = 5000;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private const int </span></span>LEFT_HAND = 0;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private const int </span></span>RIGHT_HAND = 1;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private const int </span></span>REQUIRED_ITERATIONS = 4;

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>WaveGestureTracker[,] _PlayerWaveTracker = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">new </span></span>WaveGestureTracker[6, 2];

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">EventHandler </span></span>GestureDetected;

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>Update(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton</span></span>[] skeletons, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">long </span></span>frameTimestamp)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(skeletons != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
        {
            <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton </span></span>skeleton;

            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">for </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>i = 0; i < skeletons.Length; i++)
            {
                skeleton = skeletons[i];

                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(skeleton.TrackingState != <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">SkeletonTrackingState</span></span>.NotTracked)
                {
                    TrackWave(skeleton, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref this</span></span>._PlayerWaveTracker[i, LEFT_HAND], frameTimestamp);
                    TrackWave(skeleton, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref this</span></span>._PlayerWaveTracker[i, RIGHT_HAND], frameTimestamp);
                }
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
                </span></span>{
                    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._PlayerWaveTracker[i, LEFT_HAND].Reset();
                    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._PlayerWaveTracker[i, RIGHT_HAND].Reset();
                }
            }
        }
    }
}




    下面的代码是挥手姿势识别的主要逻辑方法TrackWave的主体部分。它验证我们先前定义的构成挥手姿势的条件，并更新手势识别的状态。方法识别左手或者右手的手势，第一个条件是验证，手和肘关节点是否处于追踪状态。如果这两个关节点信息不可用，则重置追踪状态，否则进行下一步的验证。


    如果姿势持续时间超过阈值且还没有进入到下一步骤，在姿势追踪超时，重置追踪数据。下一个验证手部关节点是否在肘关节点之上。如果不是，则根据当前的追踪状态，挥手姿势识别失败或者重置识别条件。如果手部关节点在Y轴上且高于肘部关节点，方法继续判断手在Y轴上相对于肘关节的位置。调用UpdatePosition方法并传入合适的手关节点所处的位置。更新手关节点位置之后，最后判断定义的重复次数是否满足，如果满足这些条件，挥手这一手势识别成功，触发GetstureDetected事件。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>TrackWave(Skeleton skeleton, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">bool </span></span>isLeft, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref </span></span>WaveGestureTracker tracker, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">long </span></span>timestamp)
{
    JointType handJointId = (isLeft) ? JointType.HandLeft : JointType.HandRight;
    JointType elbowJointId = (isLeft) ? JointType.ElbowLeft : JointType.ElbowRight;
    Joint hand = skeleton.Joints[handJointId];
    Joint elbow = skeleton.Joints[elbowJointId];


    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(hand.TrackingState != JointTrackingState.NotTracked && elbow.TrackingState != JointTrackingState.NotTracked)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(tracker.State == WaveGestureState.InProgress && tracker.Timestamp + WAVE_MOVEMENT_TIMEOUT < timestamp)
        {
            tracker.UpdateState(WaveGestureState.Failure, timestamp);
            System.Diagnostics.Debug.WriteLine(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"Fail!"</span></span>);
        }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else if </span></span>(hand.Position.Y > elbow.Position.Y)
        {
            <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//使用 (0, 0) 作为屏幕的中心.  从用户的视角看, X轴左负右正.
            </span><span style="margin: 0px; padding: 0px; color: blue;">if </span></span>(hand.Position.X <= elbow.Position.X - WAVE_THRESHOLD)
            {
                tracker.UpdatePosition(WavePosition.Left, timestamp);
            }
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else if </span></span>(hand.Position.X >= elbow.Position.X + WAVE_THRESHOLD)
            {
                tracker.UpdatePosition(WavePosition.Right, timestamp);
            }
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
            </span></span>{
                tracker.UpdatePosition(WavePosition.Neutral, timestamp);
            }


            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(tracker.State != WaveGestureState.Success && tracker.IterationCount == REQUIRED_ITERATIONS)
            {
                tracker.UpdateState(WaveGestureState.Success, timestamp);
                System.Diagnostics.Debug.WriteLine(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"Success!"</span></span>);

                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(GestureDetected != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
                {
                    GestureDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">new </span></span>EventArgs());
                }
            }
        }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
        </span></span>{
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(tracker.State == WaveGestureState.InProgress)
            {
                tracker.UpdateState(WaveGestureState.Failure, timestamp);
                System.Diagnostics.Debug.WriteLine(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"Fail!"</span></span>);
            }
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
            </span></span>{
                tracker.Reset();
            }
        }
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
    </span></span>{
        tracker.Reset();
    }
}

下面的代码添加到WaveGestureTracker结构中：这些帮助方法维护结构中的字段，使得TrackWave方法易读。唯一需要注意的是UpdatePosition方法。TrackWave调用该方法判断手的位置已经移动。他的最主要目的是更新CurrentPosition和Timestamp属性，该方法也负责更新InterationCount字段合InPorgress状态。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>UpdateState(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WaveGestureState </span></span>state, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">long </span></span>timestamp)
{
    State = state;
    Timestamp = timestamp;
}

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>Reset()
{
    IterationCount = 0;
    State = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WaveGestureState</span></span>.None;
    Timestamp = 0;
    StartPosition = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.None;
    CurrentPosition = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.None;
}

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>UpdatePosition(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition </span></span>position, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">long </span></span>timestamp)
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(CurrentPosition != position)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(position == <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.Left || position == <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WavePosition</span></span>.Right)
        {
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(State != <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WaveGestureState</span></span>.InProgress)
            {
                State = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">WaveGestureState</span></span>.InProgress;
                IterationCount = 0;
                StartPosition = position;
            }

            IterationCount++;
        }

        CurrentPosition = position;
        Timestamp = timestamp;
    }
}

    上述代码片段就可以实现挥动(wave)手势识别的逻辑了。

## 6. 基本的手势追踪


    手部追踪在技术上和手势识别不同，但是它和手势识别中用到的一些基本方法是一样的。在开发一个具体的手势控件之前，我们先建立一个可重用的追踪手部运动的类库以方便我们后续开发。这个手部追踪类库包含一个以动态光标显示的可视化反馈机制。手部追踪和手势控件之间的交互高度松耦合。


    首先在Visual Studio中创建一个WPF控件类库项目。然后添加四个类： KinectCursorEventArgs.cs，KinectInput.cs，CusrorAdorner.cs和KinectCursorManager.cs这四个类之间通过相互调用来基于用户手所在的位置来完成光标位置的管理。KinectInput类包含了一些事件，这些事件可以在KinectCursorManager和一些控件之间共享。KinectCursorEventArgs提供了一个属性集合，能够用来在事件触发者和监听者之间传递数据。KinectCursorManager用来管理从Kinect传感器中获取的骨骼数据流，然后将其转换到WPF坐标系统，提供关于转换到屏幕位置的可视化反馈，并寻找屏幕上的控件，将事件传递到这些控件上。最后CursorAdorner.cs类包含了代表手的图标的可视化元素。


     KinectCursorEventArgs继承自RoutedEventArgs类，它包含四个属性：X、Y、Z和Cursor。X、Y、Z是一个小数，代表待转换的用户手所在位置的宽度，高度和深度值。Cursor用来存储CursorAdorner类的实例，后面将会讨论，下面的代码展示了KinectCursorEventArgs类的基本结构，其中包含了一些重载的构造器。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEventArgs
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>X { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>Y { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>Z { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; }
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">CursorAdorner </span></span>Cursor { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>x, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>y)
    {
        X = x;
        Y = y;
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point </span></span>point)
    {
        X = point.X;
        Y = point.Y;
    }
}

    RoutedEventArgs基类有一个构造函数能够接收RoutedEvent作为参数。这是一个有点特别的签名，WPF中的UIElement使用这种特殊的语法触发事件。下面的代码是KinectCursorEventArgs类对这一签名的实现，以及其他一些重载方法。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent) : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent) { } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>x, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>y, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>z) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent) { X = x; Y = y; Z = z; } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>point) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent) { X = point.X; Y = point.Y; } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>point,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>z) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent) { X = point.X; Y = point.Y; Z = z; } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>source) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent, source) {} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>source,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>x,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>y,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>z) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent, source) { X = x; Y = y; Z = z; } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>source, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>point) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent, source) { X = point.X; Y = point.Y; } <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public</span></span>KinectCursorEventArgs(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutedEvent</span></span>routedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>source, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>point,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>z) <br style="margin: 0px; padding: 0px;" />    : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(routedEvent, source) { X = point.X; Y = point.Y; Z = z; } 



    接下来，要在KinectInput类中创建事件来将消息从KinectCursorManager中传递到可视化控件中去。这些事件传递的数据类型为KinectCursorEventArgs类型。


    在KinectInput类中添加一个KinectCursorEventHandler的代理类型：(1) 添加一个静态的routed event声明。(2) 添加KinectCursorEnter，KinectCursorLeave，KinectCursorMove，KinectCursorActive和KinectCursorDeactivated事件的add和remove方法。下面的代码展示了三个和cursor相关的事件，其他的如KinectCursorActivated和KinectCursorDeactivated事件和这个结构相同：


<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public delegate void </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e);

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectInput
</span></span>{
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorEnterEvent=<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventManager</span></span>.RegisterRoutedEvent(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"KinectCursorEnter"</span></span>,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutingStrategy</span></span>.Bubble,
                                                                                    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler</span></span>),<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>));
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>AddKinectCursorEnterHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
    {
        ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).AddHandler(KinectCursorEnterEvent, handler);
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>RemoveKinectCursorEnterHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
    {
        ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).RemoveHandler(KinectCursorEnterEvent, handler);
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorLeaveEvent=<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventManager</span></span>.RegisterRoutedEvent(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"KinectCursorLeave"</span></span>,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutingStrategy</span></span>.Bubble,
                                                                                        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler</span></span>),<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>));
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>AddKinectCursorLeaveHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
    {
        ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).AddHandler(KinectCursorEnterEvent,handler);
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>RemoveKinectCursorLeaveHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
    {
        ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).RemoveHandler(KinectCursorEnterEvent, handler);
    }
}

    注意到以上代码中没有声明任何GUI编程中的Click事件。这是因为在设计控件类库时，Kinect中并没有点击事件，相反Kinect中两个重要的行为是enter和leave。手势图标可能会移入和移出某一个可视化控件的有效区域。如果要实现普通GUI控件的点击效果的话，必须在Kinect中对这一事件进行模拟，因为Kinect原生并不支持点击这一行为。


    CursorAdorner类用来保存用户手势图标可视化元素，它继承自WPF的Adorner类型。之所以使用这个类型是因为它有一个特点就是总是在其他元素之上绘制，这在我们的项目中非常有用，因为我们不希望我们的光标会被其他元素遮挡住。代码如下所示，我们默认的adorner对象将绘制一个默认的可视化元素来代表光标，当然也可以传递一个自定义的可视化元素。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">CursorAdorner</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Adorner
</span></span>{
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIElement </span></span>_adorningElement;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">VisualCollection </span></span>_visualChildren;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Canvas </span></span>_cursorCanvas;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">protected </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">FrameworkElement </span></span>_cursor;
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">StroyBoard </span></span>_gradientStopAnimationStoryboard;

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">readonly static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Color </span></span>_backColor = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Colors</span></span>.White;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">readonly static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Color </span></span>_foreColor = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Colors</span></span>.Gray;

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>CursorAdorner(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>adorningElement)
        : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(adorningElement)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._adorningElement = adorningElement;
        CreateCursorAdorner();
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.IsHitTestVisible = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>;
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>CursorAdorner(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>adorningElement, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>innerCursor)
        : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>(adorningElement)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._adorningElement = adorningElement;
        CreateCursorAdorner(innerCursor);
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.IsHitTestVisible = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>;
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">FrameworkElement </span></span>CursorVisual
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>_cursor; }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>CreateCursorAdorner()
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>innerCursor = CreateCursor();
        CreateCursorAdorner(innerCursor);
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">protected </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">FrameworkElement </span></span>CreateCursor()
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>brush = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">LinearGradientBrush</span></span>();
        brush.EndPoint = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(0, 1);
        brush.StartPoint = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(0, 0);
        brush.GradientStops.Add(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">GradientStop</span></span>(_backColor, 1));
        brush.GradientStops.Add(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">GradientStop</span></span>(_foreColor, 1));

        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>cursor = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Ellipse</span></span>()
        {
            Width=50,
            Height=50,
            Fill=brush
        };
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>cursor;
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>CreateCursorAdorner(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>innerCursor)
    {
        _visualChildren = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">VisualCollection</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>);
        _cursorCanvas = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Canvas</span></span>();
        _cursor = innerCursor;
        _cursorCanvas.Children.Add(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._cursorCanvas);
        _visualChildren.Add(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._cursorCanvas);
        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">AdornerLayer </span></span>layer = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">AdornerLayer</span></span>.GetAdornerLayer(_adorningElement);
        layer.Add(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>);
    }
}

    因为继承自Adorner基类，我们需要重写某些基类的方法，下面的代码展示了基类中的方法如何和CreateCursorAdorner方法中实例化的_visualChildren和_cursorCanvas字段进行绑定。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override int </span></span>VisualChildrenCount
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get
    </span></span>{
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>_visualChildren.Count;
    }
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">protected override </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Visual </span></span>GetVisualChild(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>index)
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>_visualChildren[index];
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">protected override </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Size </span></span>MeasureOverride(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Size </span></span>constraint)
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._cursorCanvas.Measure(constraint);
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return this</span></span>._cursorCanvas.DesiredSize;
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">protected override </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Size </span></span>ArrangeOverride(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Size </span></span>finalSize)
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>._cursorCanvas.Arrange(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Rect</span></span>(finalSize));
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>finalSize;
}

    CursorAdorner对象也负责找到手所在的正确的位置，该对象的UpdateCursor方法如下，方法接受X，Y坐标位置作为参数。然后方法在X，Y上加一个偏移量以使得图像的中心在X，Y之上，而不是在图像的边上。另外，我们提供了该方法的一个重载，该重载告诉光标对象一个特殊的坐标会传进去，所有的普通方法调用UpdateCursor将会被忽略。当我们在磁性按钮中想忽略基本的手部追踪给用户更好的手势体验时很有用。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>UpdateCursor(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>position, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">bool</span></span>isOverride) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    _isOverriden = isOverride; <br style="margin: 0px; padding: 0px;" />    _cursor.SetValue(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty,position.X-(_cursor.ActualWidth/2)); <br style="margin: 0px; padding: 0px;" />    _cursor.SetValue(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty, position.Y - (_cursor.ActualHeight / 2)); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>UpdateCursor(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>position) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(_isOverriden) <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>; <br style="margin: 0px; padding: 0px;" />    _cursor.SetValue(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty, position.X - (_cursor.ActualWidth / 2)); <br style="margin: 0px; padding: 0px;" />    _cursor.SetValue(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty, position.Y - (_cursor.ActualHeight / 2)); <br style="margin: 0px; padding: 0px;" />}

    最后，添加光标对象动画效果。当Kinect控件需要悬浮于一个元素之上，在用户等待的时候，给用户反馈一些信息告知正在发生的事情，这一点很有好处。下面了的代码展示了如何使用代码实现动画效果:
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public virtual void </span></span>AnimateCursor(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>milliSeconds) { <br style="margin: 0px; padding: 0px;" />    CreateGradientStopAnimation(milliSeconds); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(_gradientStopAnimationStoryboard != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) <br style="margin: 0px; padding: 0px;" />        _gradientStopAnimationStoryboard.Begin(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public virtual void </span></span>StopCursorAnimation(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>milliSeconds) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(_gradientStopAnimationStoryboard != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) <br style="margin: 0px; padding: 0px;" />        _gradientStopAnimationStoryboard.Stop(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public virtual void </span></span>CreateGradientStopAnimation(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>milliSeconds) { <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">NameScope</span></span>.SetNameScope(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">NameScope</span></span>()); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>cursor = _cursor <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">as</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Shape</span></span>; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(cursor == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>brush = cursor.Fill <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">as</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">LinearGradientBrush</span></span>; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>stop1 = brush.GradientStops[0]; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>stop2 = brush.GradientStops[1]; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.RegisterName(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"GradientStop1"</span></span>, stop1); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.RegisterName(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"GradientStop2"</span></span>, stop2); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>offsetAnimation = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(); <br style="margin: 0px; padding: 0px;" />    offsetAnimation.From = 1.0; <br style="margin: 0px; padding: 0px;" />    offsetAnimation.To = 0.0; <br style="margin: 0px; padding: 0px;" />    offsetAnimation.Duration = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(milliSeconds); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetName(offsetAnimation, <span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"GradientStop1"</span></span>); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(offsetAnimation, <br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GradientStop</span></span>.OffsetProperty)); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>offsetAnimation2 = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(); <br style="margin: 0px; padding: 0px;" />    offsetAnimation2.From = 1.0; <br style="margin: 0px; padding: 0px;" />    offsetAnimation2.To = 0.0; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    offsetAnimation2.Duration = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(milliSeconds); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetName(offsetAnimation2, <span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"GradientStop2"</span></span>); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(offsetAnimation2, <br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GradientStop</span></span>.OffsetProperty)); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    _gradientStopAnimationStoryboard = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Storyboard</span></span>(); <br style="margin: 0px; padding: 0px;" />    _gradientStopAnimationStoryboard.Children.Add(offsetAnimation); <br style="margin: 0px; padding: 0px;" />    _gradientStopAnimationStoryboard.Children.Add(offsetAnimation2); <br style="margin: 0px; padding: 0px;" />    _gradientStopAnimationStoryboard.Completed += <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">delegate</span></span>{ _gradientStopAnimationStoryboard.Stop(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>); }; <br style="margin: 0px; padding: 0px;" />}

    为了实现KinectCursorManager类,我们需要几个帮助方法,代码如下，GetElementAtScreenPoint方法告诉我们哪个WPF对象位于X,Y坐标下面，在这个高度松散的结构中，GetElementAtScreenPoint方法是主要的引擎，用来从KinectCurosrManager传递消息到自定义控件，并接受这些事件。另外，我们使用两个方法来确定我们想要追踪的骨骼数据以及我们想要追踪的手。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIElement </span></span>GetElementAtScreenPoint(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point </span></span>point, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window) 
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!window.IsVisible)
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return null</span></span>;
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point </span></span>windowPoint = window.PointFromScreen(point);
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">IInputElement </span></span>element = window.InputHitTest(windowPoint);
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(element <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">is </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIElement</span></span>)
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)element;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
        return null</span></span>;
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Skeleton </span></span>GetPrimarySkeleton(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">IEnumerable</span></span><<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton</span></span>> skeletons)
{
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton </span></span>primarySkeleton = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">foreach </span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton </span></span>skeleton <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">in </span></span>skeletons)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(skeleton.TrackingState != <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">SkeletonTrackingState</span></span>.Tracked)
        {
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">continue</span></span>;
        }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(primarySkeleton == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
            primarySkeleton = skeleton;
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else if </span></span>(primarySkeleton.Position.Z > skeleton.Position.Z)
            primarySkeleton = skeleton;
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>primarySkeleton;
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Joint</span></span>? GetPrimaryHand(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton </span></span>skeleton)
{ 
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Joint </span></span>leftHand=skeleton.Joints[<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">JointType</span></span>.HandLeft];
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Joint </span></span>rightHand=skeleton.Joints[<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">JointType</span></span>.HandRight];
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(rightHand.TrackingState == <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">JointTrackingState</span></span>.Tracked)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(leftHand.TrackingState != <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">JointTrackingState</span></span>.Tracked)
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>rightHand;
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else if </span></span>(leftHand.Position.Z > rightHand.Position.Z)
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>rightHand;
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
            return </span></span>leftHand;
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(leftHand.TrackingState == <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">JointTrackingState</span></span>.Tracked)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>leftHand;
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
        return null</span></span>;
}

    KinectCursorManager应该是一个单例类。这样设计是能够使得代码实例化起来简单。任何和KinectCursorManager工作的控件在KinectCursorManager没有实例化的情况下可以独立的进行KinectCursorManager的实例化。这意味着任何开发者使用这些控件不需要了解KinectCursorManager对象本身。相反，开发者能够简单的将控件拖动到应用程序中，控件负责实例化KinectCursorManager对象。为了使得这种自服务功能能和KinectCursorMange类一起使用，我们需要创建一个重载的Create方法来将应用程序的主窗体类传进来。下面的代码展示了重载的构造函数以及特殊的单例模式的实现方法。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager
</span></span>{
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectSensor </span></span>kinectSensor;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">CursorAdorner </span></span>cursorAdorner;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Window </span></span>window;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIElement </span></span>lastElementOver;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private bool </span></span>isSkeletonTrackingActivated;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private static bool </span></span>isInitialized;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager </span></span>instance;

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>Create(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!isInitialized)
        {
            instance = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager</span></span>(window);
            isInitialized = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
        }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>Create(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>cursor)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!isInitialized)
        {
            instance = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager</span></span>(window,cursor);
            isInitialized = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
        }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>Create(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor </span></span>sensor) 
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!isInitialized)
        {
            instance = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager</span></span>(window, sensor);
            isInitialized = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
        }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>Create(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor </span></span>sensor, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>cursor)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!isInitialized)
        {
            instance = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager</span></span>(window, sensor, cursor);
            isInitialized = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
        }
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorManager </span></span>Instance
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>instance; }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>KinectCursorManager(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window) : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>(window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor</span></span>.KinectSensors[0]) { }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>KinectCursorManager(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>cursor) : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>(window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor</span></span>.KinectSensors[0], cursor) { }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>KinectCursorManager(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor </span></span>sensor) : <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>(window, sensor, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) { }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>KinectCursorManager(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window </span></span>window, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor </span></span>sensor, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement </span></span>cursor)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.window = window;
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectSensor</span></span>.KinectSensors.Count > 0)
        {
            window.Unloaded += <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">delegate
            </span></span>{
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor.SkeletonStream.IsEnabled)
                    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor.SkeletonStream.Disable();
            };
            window.Loaded += <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">delegate
            </span></span>{
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(cursor == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
                    cursorAdorner = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">CursorAdorner</span></span>((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement</span></span>)window.Content);
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else
                    </span></span>cursorAdorner = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">CursorAdorner</span></span>((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">FrameworkElement</span></span>)window.Content, cursor);

                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor = sensor;
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor.SkeletonFrameReady += SkeletonFrameReady;
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor.SkeletonStream.Enable(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">TransformSmoothParameters</span></span>());
                <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.kinectSensor.Start();
            };
        }
    }
……

    下面的代码展示了KinectCursorManager如何和窗体上的可视化元素进行交互。当用户的手位于应用程序可视化元素之上时，KinectCursorManager对象始终保持对当前手所在的可视化元素以及之前手所在的可视化元素的追踪。当这一点发生改变时，KinectCursorManager会触发之前控件的leave事件和当前控件的enter事件。我们也保持对KinectSensor对象的追踪，并触发activated和deactivated事件。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>SetSkeletonTrackingActivated()
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(lastElementOver != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null </span></span>&& isSkeletonTrackingActivated == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>)
    {
        lastElementOver.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorActivatedEvent));
    }
    isSkeletonTrackingActivated = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
}

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>SetSkeletonTrackingDeactivated()
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(lastElementOver != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null </span></span>&& isSkeletonTrackingActivated == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>)
    {
        lastElementOver.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorDeactivatedEvent));
    }
    isSkeletonTrackingActivated = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false </span></span>;
}

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>HandleCursorEvents(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point </span></span>point, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>z)
{
    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement </span></span>element = GetElementAtScreenPoint(point, window);
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(element != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
    {
        element.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorMoveEvent, point, z) {Cursor=cursorAdorner });
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(element != lastElementOver)
        {
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(lastElementOver != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
            {
                lastElementOver.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorLeaveEvent, point, z) { Cursor = cursorAdorner });
            }
            element.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorEnterEvent, point, z) { Cursor = cursorAdorner });
        }
    }
    lastElementOver = element;
}

    最后需要两个核心的方法来管理KinectCursorManger类。SkeletonFrameReady方法与之前一样，用来从Kinect获取骨骼数据帧时触发的事件。在这个项目中，SkeletonFrameReady方法负责获取合适的骨骼数据，然后获取合适的手部关节点数据。然后将手部关节点数据传到UpdateCusror方法中，UpdateCursor方法执行一系列方法将Kinect骨骼空间坐标系转化到WPF的坐标系统中，Kinect SDK中MapSkeletonPointToDepth方法提供了这一功能。SkeletonToDepthImage方法返回的X,Y值，然后转换到应用程序中实际的宽和高。和X,Y不一样，Z值进行了不同的缩放操作。简单的从Kinect深度摄像机中获取的毫米数据。代码如下，一旦这些坐标系定义好了之后，将他们传递到HandleCursorEvents方法然后CursorAdorner对象将会给用户以反馈。相关代码如下：
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>SkeletonFrameReady(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">SkeletonFrameReadyEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">using</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">SkeletonFrame</span></span>frame = e.OpenSkeletonFrame()) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(frame == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>|| frame.SkeletonArrayLength == 0) <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton</span></span>[] skeletons = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Skeleton</span></span>[frame.SkeletonArrayLength]; <br style="margin: 0px; padding: 0px;" />        frame.CopySkeletonDataTo(skeletons); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Skeleton</span></span>skeleton = GetPrimarySkeleton(skeletons); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(skeleton == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) <br style="margin: 0px; padding: 0px;" />        { <br style="margin: 0px; padding: 0px;" />            SetHandTrackingDeactivated(); <br style="margin: 0px; padding: 0px;" />        } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else <br style="margin: 0px; padding: 0px;" />       </span></span>{ <br style="margin: 0px; padding: 0px;" />            <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Joint</span></span>? primaryHand = GetPrimaryHand(skeleton); <br style="margin: 0px; padding: 0px;" />            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(primaryHand.HasValue) <br style="margin: 0px; padding: 0px;" />            { <br style="margin: 0px; padding: 0px;" />                UpdateCursor(primaryHand.Value); <br style="margin: 0px; padding: 0px;" />            } <br style="margin: 0px; padding: 0px;" />            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else <br style="margin: 0px; padding: 0px;" />           </span></span>{ <br style="margin: 0px; padding: 0px;" />                SetHandTrackingDeactivated(); <br style="margin: 0px; padding: 0px;" />            } <br style="margin: 0px; padding: 0px;" />        } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void</span></span>SetHandTrackingDeactivated() <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    cursorAdorner.SetVisibility(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(lastElementOver != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>&& isHandTrackingActivated == <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>) <br style="margin: 0px; padding: 0px;" />    {lastElementOver.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorDeactivatedEvent)); }; <br style="margin: 0px; padding: 0px;" />    isHandTrackingActivated = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>; <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void</span></span>UpdateCursor(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Joint</span></span>hand) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>point = kinectSensor.MapSkeletonPointToDepth(hand.Position, kinectSensor.DepthStream.Format); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float</span></span>x = point.X; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float</span></span>y = point.Y; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float</span></span>z = point.Depth; <br style="margin: 0px; padding: 0px;" />    x = (<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float</span></span>)(x * window.ActualWidth / kinectSensor.DepthStream.FrameWidth); <br style="margin: 0px; padding: 0px;" />    y = (<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float</span></span>)(y * window.ActualHeight / kinectSensor.DepthStream.FrameHeight); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Point</span></span>cursorPoint = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(x, y); <br style="margin: 0px; padding: 0px;" />    HandleCursorEvents(cursorPoint, z); <br style="margin: 0px; padding: 0px;" />    cursorAdorner.UpdateCursor(cursorPoint); <br style="margin: 0px; padding: 0px;" />}

     至此，我们已经简单实现了一些基础结构，这些仅仅是实现了将用户手部的运动显示在屏幕上。现在我们要创建一个基类来监听光标对象的事件，首先创建一个KinectButton对象，该对象继承自WPF Button类型。定义三个之前在KinectInput中定义好的事件，同时创建这些事件的添加删除方法，代码如下：
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectButton</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Button <br style="margin: 0px; padding: 0px;" /></span></span>{ <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent</span></span>KinectCursorEnterEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorEnterEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton</span></span>)); <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent</span></span>KinectCursorLeaveEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorLeaveEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton</span></span>)); <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent</span></span>KinectCursorMoveEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorMoveEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton</span></span>)); <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent</span></span>KinectCursorActivatedEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorActivatedEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton</span></span>)); <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent</span></span>KinectCursorDeactivatedEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorDeactivatedEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton</span></span>)); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>KinectCursorEnter <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorEnterEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorEnterEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>KinectCursorLeave <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorLeaveEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorLeaveEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>KinectCursorMove <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorMoveEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorMoveEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventHandler</span></span>KinectCursorActivated <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorActivatedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorActivatedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventHandler</span></span>KinectCursorDeactivated <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorDeactivatedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorDeactivatedEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />}

    在KinectButton的构造函数中，首先检查当前控件是否运行在IDE或者一个实际的应用程序中。如果没有在设计器中，如果KinectCursorManager对象不存在，我们实例化KinectCursorManager对象。通过这种方式，我们可以在同一个窗体上添加多个Kinect 按钮。这些按钮自动创建KinectCursorManager的实例而不用开发者去创建。下面的代码展示了如何实现这一功能。KinectCursorManager类中的HandleCursorEvents方法负责处理这些事件。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>KinectButton() <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(!System.ComponentModel.<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DesignerProperties</span></span>.GetIsInDesignMode(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>)) <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorManager</span></span>.Create(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Application</span></span>.Current.MainWindow); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.KinectCursorEnter+=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(OnKinectCursorEnter); <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.KinectCursorLeave+=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(OnKinectCursorLeave); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.KinectCursorMove+=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(OnKinectCursorMove); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected virtual void</span></span>OnKinectCursorLeave(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected virtual void</span></span>OnKinectCursorMove(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ }

     下面的代码中，KinectCursorEnter事件中触发ClickEvent，将其改造成了一个标准的点击事件。使得KinectButton能够在鼠标移入时触发Click事件。Kinect中应用程序的交互术语还是使用之前GUI交互界面中的术语，这使得读者能够更容易理解。更重要的是，也能够使得开发者更容易理解，因为我们之前有很多使用按钮来构造用户界面的经验。当然终极的目标是舍弃这些各种各样的控件，改而使用纯粹的手势交互界面，但是按钮在现阶段的交互界面中还是很重要的。另外，这样也能够使用按钮来布局图形用户界面，只需要将普通的按钮换成Kinect按钮就可以了。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected virtual void </span></span>OnKinectCursorEnter(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)
{
    RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(ClickEvent));
}

    这种控件有一个最大的问题，在大多数基于Kinect的应用程序中你看不到这个问题，那就是，你不能区分开是有意的还是无意的点击。在传统的基于鼠标的GUI应用中也有类似的倾向，每一次将鼠标移动到按钮上不用点击就会激活按钮。这种用户界面很容易不能使用，这也提醒了一个潜在的值得注意的问题，那就是将按钮从图形用户界面中移植到其他界面中可能存在的问题。悬浮按钮是微软试图解决这一特殊问题的一个尝试。

## 7. 剩余七种常见手势的识别


    前面的文章中已经讲述了挥手手势的识别，本文接下来讲解余下7中常见手势的识别。

### 7.1悬浮按钮(Hover Button)


    悬浮按钮是微软在2010年为Kinect对Xbox的操纵盘进行改进而引入的。 悬浮按钮通过将鼠标点击换成悬浮然后等待(hover-and-wait)动作，解决了不小心点击的问题。当光标位于按钮之上时，意味着用户通过将光标悬浮在按钮上一段时间来表示想选中按钮。另一个重要特点是悬浮按钮在用户悬浮并等待时，多少提供了视觉反馈。


    在Kinect中实现悬浮按钮和在Windows Phone开发中实现轻点然后维持(tap-and-hold)这一手势在技术上比较类似。必须使用一个计时器来记录当前用户光标停留在按钮上的时间。一旦用户的手的光标和按钮的边界交叉就开始计时。如果某一个时间阈值内用户光标还没有移除，那么就触发点击事件。


    创建一个名为HoverButton的类，他继承自之前创建的KinectButton类，在类中添加一个名为hoverTimer的DispatcherTime实例，代码如下。另外创建一个布尔型的timerEnable字段，将其设置为true。虽然目前不会用到这个字段，但是在后面部分将会用到，当我们想使用HoverButton的某些功能，但是不需要DispatcherTimer时就会非常有用。最后创建一个HoverInterval的依赖属性，使得运行我们将悬浮时间用代码或者xaml进行定义。默认设置为2秒，这是在大多是Xbox游戏中的时间。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">HoverButton</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton <br style="margin: 0px; padding: 0px;" /></span></span>{ <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DispatcherTimer</span></span>hoverTimer = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DispatcherTimer</span></span>(); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected bool</span></span>timerEnabled = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double</span></span>HoverInterval <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(HoverIntervalProperty); } <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set <br style="margin: 0px; padding: 0px;" />       </span></span>{ <br style="margin: 0px; padding: 0px;" />            SetValue(HoverIntervalProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); <br style="margin: 0px; padding: 0px;" />        } <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty</span></span>HoverIntervalProperty = <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"HoverInterval"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">HoverButton</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(2000d)); <br style="margin: 0px; padding: 0px;" />……
}

    要实现悬浮按钮的核心功能，我们必须覆写基类中的OnKinectCursorLeave和OnKinectCursorEnter方法，所有和KinectCursorManger进行交互的部分在KinectButton中已经实现了，因此我们在这里不用操心。在类的构造方法中，只需要实例化DispathcerTimer对象，HoverInterval依赖属性和注册hoverTimer_Tick方法到计时器的Tick事件上即可。计时器在一定的间隔时间会触发Tick事件，该事件简单的处理一个Click事件，在OnKinectCursorEnter方法中启动计数器，在OnKinectCursorLeave事件中停止计数器。另外，重要的是，在enter和leave方法中启动和停止鼠标光标动画效果。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>HoverButton() <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    hoverTimer.Interval = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(HoverInterval); <br style="margin: 0px; padding: 0px;" />    hoverTimer.Tick += <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">EventHandler</span></span>(hoverTimer_Tick); <br style="margin: 0px; padding: 0px;" />    hoverTimer.Stop(); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">void</span></span>hoverTimer_Tick(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    hoverTimer.Stop(); <br style="margin: 0px; padding: 0px;" />    RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(ClickEvent)); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void</span></span>OnKinectCursorLeave(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(timerEnabled) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        e.Cursor.StopCursorAnimation(); <br style="margin: 0px; padding: 0px;" />        hoverTimer.Stop(); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void</span></span>OnKinectCursorEnter(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(timerEnabled) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        hoverTimer.Interval = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(HoverInterval); <br style="margin: 0px; padding: 0px;" />        e.Cursor.AnimateCursor(HoverInterval); <br style="margin: 0px; padding: 0px;" />        hoverTimer.Start(); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />}

     悬浮按钮在基于Kinect的Xbox游戏中几乎无处不在。悬浮按钮唯一存在的问题是，光标手势悬停在按钮上时会抖动，这可能是Kinect中骨骼识别本身的问题。当在运动状态时，Kinect能够很好的对这些抖动进行平滑，因为即使在快速移动状态下，Kinect中的软件使用了一系列预测和平滑技术来对抖动进行处理。姿势，和上面的悬停一样，因为是静止的，所以可能存在抖动的问题。另外，用户一般不会保持手势静止，即使他们想哪样做。Kinect将这些小的运动返回给用户。当用户什么都没做时，抖动的手可能会破坏手势的动画效果。对悬浮按钮的一个改进就是磁性按钮(Magnet
 Button)，随着体感游戏的升级，这种按钮逐渐取代了之前的悬浮按钮,后面我们将看到如何实现磁性按钮。

### 7.2 下压按钮(Push Button)


    就像悬浮按钮在Xbox中那样普遍一样，一些Kinect开发者也想创建一些类似PC上的那种交互方式的按钮，这种按钮称之为下压按钮(push button)。下压按钮试图将传统的GUI界面上的按钮移植到Kinect上去。为了代替鼠标点击，下压按钮使用一种将手向前推的手势来表示按下这一动作。


    这种手势，手掌张开向前，在形式上有点像动态鼠标。下压按钮的核心算法就是探测手势在Z轴上有一个向负方向的运动。另外，相符方向必须有一个距离阈值，使得超过这一阈值就认为用户想要执行下压指令。代码如下所示：下压按钮有一个称之为Threshold的依赖属性，单位为毫米，这个值可以由开发者来根据动作的灵敏度来进行设置。当用户的手移动到下压按钮的上方时，我们记录一下当前位置手的Z值，以此为基准，然后比较手的深度值和阈值，如果超过阈值，就触发点击事件。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PushButton</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectButton
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected double </span></span>handDepth;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>PushThreshold
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(PushThresholdProperty); }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(PushThresholdProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>PushThresholdProperty =
        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"PushThreshold"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">PushButton</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(100d));

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void </span></span>OnKinectCursorMove(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(e.Z < handDepth - PushThreshold)
        {
            RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(ClickEvent));
        }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void </span></span>OnKinectCursorEnter(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)
    {
        handDepth = e.Z;
    }
}
### 7.3 磁性按钮(Magnet Button)


    如前面所讨论的，磁性按钮是对悬浮按钮的一种改进。他对用户悬浮在按钮上的这一体验进行了一些改进。他试图追踪用户手的位置，然后自动将光标对齐到磁性按钮的中间。当用户的手离开磁性按钮的区域是，手势追踪又恢复正常。在其他方面磁性按钮和悬浮按钮的行为一样。考虑到磁性按钮和悬浮按钮在功能方面差异很小，而我们将他单独作为一个完全不同的控件来对待可能有点奇怪。但是，在用户体验设计领域(UX)，这一点差异就是一个完全不同的概念。从编码角度看，这一点功能性的差异也使得代码更加复杂。


首先，创建一个继承自HoverButton的名为MagnetButton的类。磁性按钮需要一些额外的事件和属性来管理手进入到磁性按钮区域和手自动对齐到磁性按钮中间区域的时间。我们需要在KinectInput类中添加新的lock和unlock事件，代码如下：
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorLockEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventManager</span></span>.RegisterRoutedEvent(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"KinectCursorLock"</span></span>, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutingStrategy</span></span>.Bubble,
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>));

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>AddKinectCursorLockHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
{
    ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).AddHandler(KinectCursorLockEvent, handler);
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorUnlockEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventManager</span></span>.RegisterRoutedEvent(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"KinectCursorUnlock"</span></span>, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutingStrategy</span></span>.Bubble,
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>));

<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public static void </span></span>RemoveKinectCursorUnlockHandler(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject </span></span>o, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler </span></span>handler)
{
    ((<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">UIElement</span></span>)o).RemoveHandler(KinectCursorUnlockEvent, handler);
}

<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">MagnetButton </span></span>: <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">HoverButton
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected bool </span></span>isLockOn = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorLockEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorUnlockEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagnetButton</span></span>));
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>KinectCursorUnlockEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>.KinectCursorLockEvent.AddOwner(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagnetButton</span></span>));
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Storyboard </span></span>move;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler </span></span>KinectCursorLock
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorLockEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorLockEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler </span></span>KinectCursorUnLock
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.AddHandler(KinectCursorUnlockEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.RemoveHandler(KinectCursorUnlockEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>LockInterval
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(LockIntervalProperty); }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(LockIntervalProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>LockIntervalProperty =
        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"LockInterval"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagnetButton</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(200d));

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>UnlockInterval
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(UnlockIntervalProperty); }
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(UnlockIntervalProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }
    }

    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>UnlockIntervalProperty =
        <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"UnlockInterval"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagnetButton</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(80d));

……}

    磁性按钮的代码中，核心地方在于光标从当前位置移动到磁性按钮的中心位置。看起来很简单，实际上实现起来有点麻烦。需要重写基类中的OnKinectCursorEnter和OnKinectCursorLeave方法。确定磁性按钮的锁定位置第一步需要找到磁性按钮本身所处的位置。代码如下，我们使用WPF中最常见名为FindAncestor帮助方法来遍历可视化对象树来进行查找，需要找到承载该磁性按钮的Windows对象，匹配磁性按钮的当前实例到Windows上，然后将其赋给名为Point的变量。但是point对象只保存了当前磁性按钮的左上角的位置。所以，我们需要给在这个点上加一个磁性按钮一半长宽的偏移值，才能获取到磁性按钮的中心位置x,y。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private </span></span>T FindAncestor<T>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject</span></span>dependencyObject) <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">where</span></span>T:<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">class <br style="margin: 0px; padding: 0px;" /></span></span>{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyObject</span></span>target=dependencyObject; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">do <br style="margin: 0px; padding: 0px;" />   </span></span>{ <br style="margin: 0px; padding: 0px;" />        target=<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">VisualTreeHelper</span></span>.GetParent(target); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">while</span></span>(target!=<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>&&!(target <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">is</span></span>T)); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>target <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">as</span></span>T; <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void </span></span>OnKinectCursorEnter(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//获取按钮位置 <br style="margin: 0px; padding: 0px;" />     </span><span style="margin: 0px; padding: 0px; color: blue;">var</span></span>rootVisual=FindAncestor<<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window</span></span>>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>point=<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.TransformToAncestor(rootVisual).Transform(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(0,0)); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>x=point.X+<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.ActualWidth/2; <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>y=point.Y+<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.ActualHeight/2; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>cursor=e.Cursor; <br style="margin: 0px; padding: 0px;" />        cursor.UpdateCursor(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X,e.Y),<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//找到目的位置 <br style="margin: 0px; padding: 0px;" />        </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>lockPoint=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(x-cursor.CursorVisual.ActualWidth/2,y-cursor.CursorVisual.ActualHeight/2); <br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//当前位置 <br style="margin: 0px; padding: 0px;" />        </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>cursorPoint=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X-cursor.CursorVisual.ActualWidth/2,e.Y-cursor.CursorVisual.ActualHeight/2); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: green;"><span style="background-color: rgb(238, 238, 238);">//将光标从当前位置传送到目的位置 <br style="margin: 0px; padding: 0px;" />        </span></span>AnimateCursorToLockPosition(e,x,y,cursor,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref</span></span>lockPoint,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref</span></span>cursorPoint); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.OnKinectCursorEnter(sender,e); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void </span></span>OnKinectCursorLeave(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object</span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />     <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.OnKinectCursorLeave(sender, e); <br style="margin: 0px; padding: 0px;" />    e.Cursor.UpdateCursor(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X,e.Y),<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>rootVisual=FindAncestor<<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Window</span></span>>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>point=<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.TransformToAncestor(rootVisual).Transform(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(0,0)); <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>x=point.X+<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.ActualWidth/2; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>y=point.Y+<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.ActualHeight/2; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var</span></span>cursor=e.Cursor; <br style="margin: 0px; padding: 0px;" />           <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//找到目的位置 <br style="margin: 0px; padding: 0px;" />    </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>lockPoint=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(x-cursor.CursorVisual.ActualWidth/2,y-cursor.CursorVisual.ActualHeight/2); <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//当前位置 <br style="margin: 0px; padding: 0px;" />    </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>cursorPoint=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X-cursor.CursorVisual.ActualWidth/2,e.Y-cursor.CursorVisual.ActualHeight/2); <br style="margin: 0px; padding: 0px;" />           <br style="margin: 0px; padding: 0px;" />    AnimateCursorAwayFromLockPosition(e,cursor,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref</span></span>lockPoint,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">ref</span></span>cursorPoint); <br style="margin: 0px; padding: 0px;" />}

    接下来，我们用手所在的X,Y位置替换手势图标的位置。然而，我们也传入了第二个参数，告诉手势图标自动停止追踪手的位置一段时间。当用户看到光标不听手的使唤自动对齐到磁性按钮的中心，这可能有点不太友好。


    虽然我们现在有了磁性按钮的中心位置，但是我们仍不能很好的将手势光标定位到中心。我们必须额外的给手势光标本身给一个一半长宽的偏移值，以使得手在光标的中心位置而不是在左上角。在完成这些操作之后，我们将最终的值赋给lockPoint变量。我们也执行了同样的操作来查找光标目前的左上角位置以及偏移量，并将其赋值给cursorPoint变量。有了这两个值，我们就可以从当前的位置使用动画移动到目标位置了。动画方法代码如下：
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>AnimateCursorAwayFromLockPosition(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">CursorAdorner</span></span>cursor,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">ref</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>lockPoint,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">ref</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>cursorPoint) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />     <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>moveLeft = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(lockPoint.X, cursorPoint.X, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Duration</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(UnlockInterval))); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTarget(moveLeft, cursor.CursorVisual); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(moveLeft, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty)); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>moveTop = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(lockPoint.Y, cursorPoint.Y, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Duration</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(UnlockInterval))); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTarget(moveTop, cursor.CursorVisual); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(moveTop, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.TopProperty)); <br style="margin: 0px; padding: 0px;" />    move = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Storyboard</span></span>(); <br style="margin: 0px; padding: 0px;" />    move.Children.Add(moveTop); <br style="margin: 0px; padding: 0px;" />    move.Children.Add(moveLeft); <br style="margin: 0px; padding: 0px;" />    move.Completed += <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">delegate</span></span>{ <br style="margin: 0px; padding: 0px;" />        move.Stop(cursor); <br style="margin: 0px; padding: 0px;" />        cursor.UpdateCursor(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X, e.Y), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(KinectCursorUnlockEvent, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(e.X, e.Y), e.Z) { Cursor = e.Cursor }); <br style="margin: 0px; padding: 0px;" />    }; <br style="margin: 0px; padding: 0px;" />    move.Begin(cursor, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void</span></span>AnimateCursorToLockPosition(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs</span></span>e,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>x,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>y,<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">CursorAdorner</span></span>cursor,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">ref</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>lockPoint,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">ref</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>cursorPoint) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />     <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>moveLeft=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(cursorPoint.X,lockPoint.X,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Duration</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(LockInterval))); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTarget(moveLeft,cursor.CursorVisual); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(moveLeft,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.LeftProperty)); <br style="margin: 0px; padding: 0px;" />            <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DoubleAnimation</span></span>moveTop=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DoubleAnimation</span></span>(cursorPoint.Y,lockPoint.Y,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Duration</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">TimeSpan</span></span>.FromMilliseconds(LockInterval))); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTarget(moveTop,cursor.CursorVisual); <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Storyboard</span></span>.SetTargetProperty(moveTop,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">PropertyPath</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Canvas</span></span>.TopProperty)); <br style="margin: 0px; padding: 0px;" />    move=<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Storyboard</span></span>(); <br style="margin: 0px; padding: 0px;" />    move.Children.Add(moveTop); <br style="margin: 0px; padding: 0px;" />    move.Children.Add(moveLeft); <br style="margin: 0px; padding: 0px;" />    move.Completed+=<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">delegate <br style="margin: 0px; padding: 0px;" />   </span></span>{ <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(KinectCursorLockEvent,<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new</span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(x,y),e.Z){Cursor=e.Cursor}); <br style="margin: 0px; padding: 0px;" />    }; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if</span></span>(move!=<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>) <br style="margin: 0px; padding: 0px;" />        move.Stop(e.Cursor); <br style="margin: 0px; padding: 0px;" />    move.Begin(cursor,<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>); <br style="margin: 0px; padding: 0px;" />}

    在上面的lock和unlock动画中，我们等到动画结束时触发KinectCursorLock和KinectCursorUnlock事件。对于磁性按钮本身，这些事件用处不大。但是在后面可以给磁性幻灯片按钮提供一些帮助。

### 7.4 划动(Swipe)


    划动手势和挥手(wave)手势类似。识别划动手势需要不断的跟踪用户手部运动，并保持当前手的位置之前的手的位置。因为手势有一个速度阈值，我们需要追踪手运动的时间以及在三维空间中的坐标。下面的代码展示了存储手势位置点的X,Y,Z坐标以及时间值。如果熟悉图形学中的矢量计算，可以将这个认为是一个四维向量。将下面的结构添加到类库中。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public struct </span></span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint <br style="margin: 0px; padding: 0px;" /></span></span>{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>X { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; } <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>Y { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; } <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>Z { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; } <br style="margin: 0px; padding: 0px;" />    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DateTime </span></span>T { <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get</span></span>; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set</span></span>; } <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public override bool </span></span>Equals(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>obj) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>o = (<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint</span></span>)obj; <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(X == o.X) && (Y == o.Y) && (Z == o.Z)&&(T==o.T); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public override int </span></span>GetHashCode() <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return base</span></span>.GetHashCode(); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />}

    我们将在KinectCursorManager对象中实现划动手势识别的逻辑，这样在后面的磁吸幻灯片按钮中就可以复用这部分逻辑。实现代码如下，代码中为了支持划动识别，需要向KinectCurosrManager对象中添加几个字段。GesturePoints集合存储路径上的所有点，虽然我们会一边移除一些点然后添加新的点，但是该集合不可能太大。SwipeTime和swipeDeviation分别提供了划动手势经历的时间和划动手势在y轴上的偏移阈值。划动手势经历时间过长和划动手势路径偏移y值过大都会使得划动手势识别失败。我们会移除之前的路径上的点，然后添加新的划动手势上的点。SwipeLength提供了连续划动手势的阈值。我们提供了两个事件来处理划动手势识别成功和手势不合法两种情况。考虑到这是一个纯粹的手势，与GUI界面无关，所以在实现过程中不会使用click事件。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">private </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">List</span></span><<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint</span></span>> gesturePoints; <br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private bool </span></span>gesturePointTrackingEnabled; <br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private double </span></span>swipeLength, swipeDeviation; <br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private int </span></span>swipeTime; <br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler </span></span>swipeDetected; <br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler </span></span>swipeOutofBoundDetected; <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private double </span></span>xOutOfBoundsLength; <br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private static double </span></span>initialSwipeX;

    xOutOfBoundsLength和initialSwipeX用来设置划动手势的开始位置。通常，我们并不关心挥划动手势的开始位置，只用在gesturePoints中寻找一定数量连续的点，然后进行模式匹配就可以了。但是有时候，我们只从某一个划动开始点来进行划动识别也很有用。例如如果在屏幕的边缘，我们实现水平滚动，在这种情况下，我们需要一个偏移阈值使得我们可以忽略在屏幕外的点，因为这些点不能产生手势。


  下面的代码展示了一些帮助方法以及公共属性来管理手势追踪。GesturePointTrackingInitialize方法用来初始化各种手势追踪的参数。初始化好了划动手势之后，需要调用GesturePointTrackingStart方法。自然需要一个相应的GesturePointTrackingStop方法来结束挥动手势识别。最后我们需要提供两个重载的帮助方法ResetGesturePoint来管理一系列的我们不需要的手势点。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>GesturePointTrackingInitialize(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>swipeLength, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>swipeDeviation, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>swipeTime, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double </span></span>xOutOfBounds) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.swipeLength = swipeLength; <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.swipeDeviation = swipeDeviation; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.swipeTime = swipeTime; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>.xOutOfBoundsLength = xOutOfBounds; <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>GesturePointTrackingStart() <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(swipeLength + swipeDeviation + swipeTime == 0) <br style="margin: 0px; padding: 0px;" />        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">throw new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">InvalidOperationException</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"挥动手势识别参数没有初始化！"</span></span>); <br style="margin: 0px; padding: 0px;" />    gesturePointTrackingEnabled = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>; <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public void </span></span>GesturePointTrackingStop() <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    xOutOfBoundsLength = 0; <br style="margin: 0px; padding: 0px;" />    gesturePointTrackingEnabled = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>; <br style="margin: 0px; padding: 0px;" />    gesturePoints.Clear(); <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public bool </span></span>GesturePointTrackingEnabled <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>gesturePointTrackingEnabled ; } <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>ResetGesturePoint(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint </span></span>point) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">bool </span></span>startRemoving = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">for </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>i= gesturePoints.Count; i >=0; i--) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(startRemoving) <br style="margin: 0px; padding: 0px;" />            gesturePoints.RemoveAt(i); <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">else <br style="margin: 0px; padding: 0px;" />            if </span></span>(gesturePoints[i].Equals(point)) <br style="margin: 0px; padding: 0px;" />                startRemoving = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">true</span></span>; <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />} <br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>ResetGesturePoint(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>point) <br style="margin: 0px; padding: 0px;" />{ <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(point < 1) <br style="margin: 0px; padding: 0px;" />        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>; <br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">for </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int </span></span>i = point-1; i >=0; i--) <br style="margin: 0px; padding: 0px;" />    { <br style="margin: 0px; padding: 0px;" />        gesturePoints.RemoveAt(i); <br style="margin: 0px; padding: 0px;" />    } <br style="margin: 0px; padding: 0px;" />}

    划动(swipe)手势识别的核心算法在HandleGestureTracking方法中，代码如下。将KinectCursorManager中的UpdateCursor方法和Kinect中的骨骼追踪事件绑定。每一次当获取到新的坐标点时，HandGestureTracking方法将最新的GesturePoint数据添加到gesturePoints集合中去。然后执行一些列条件检查，首先判断新加入的点是否以手势开始位置为起点参考，偏离Y轴过远。如果是，抛出一个超出范围的事件，然后将所有之前累积的点清空，然后开始下一次的划动识别。其次，检查手势开始的时间和当前的时间，如果时间差大于阈值，那么移除开始处手势点，然后将紧接着的点作为手势识别的起始点。如果新的手的位置在这个集合中，就很好。紧接着，判断划动起始点的位置和当前位置的X轴上的距离是否超过了连续划动距离的阈值，如果超过了，则触发SwipeDetected事件，如果没有，我们可以有选择性的判断，当前位置的X点是否超过了划动识别的最大区间返回，然后触发对于的事件。然后我们等待新的手部点传到HandleGestureTracking方法中去。
<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>HandleGestureTracking(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float </span></span>x, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float </span></span>y, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">float </span></span>z)
{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(!gesturePointTrackingEnabled)
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>;
    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">// check to see if xOutOfBounds is being used
    </span><span style="margin: 0px; padding: 0px; color: blue;">if </span></span>(xOutOfBoundsLength != 0 && initialSwipeX == 0)
    {
        initialSwipeX = x;
    }

    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint </span></span>newPoint = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">GesturePoint</span></span>() { X = x, Y = y, Z = z, T = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DateTime</span></span>.Now };
    gesturePoints.Add(newPoint);

    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">GesturePoint </span></span>startPoint = gesturePoints[0];
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>point = <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">Point</span></span>(x, y);


    <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//check for deviation
    </span><span style="margin: 0px; padding: 0px; color: blue;">if </span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">Math</span></span>.Abs(newPoint.Y - startPoint.Y) > swipeDeviation)
    {
        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//Debug.WriteLine("Y out of bounds");
        </span><span style="margin: 0px; padding: 0px; color: blue;">if </span></span>(swipeOutofBoundDetected != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
            swipeOutofBoundDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(point) { Z = z, Cursor = cursorAdorner });
        ResetGesturePoint(gesturePoints.Count);
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>;
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>((newPoint.T - startPoint.T).Milliseconds > swipeTime) <span style="margin: 0px; padding: 0px; color: green;"><span style="background-color: rgb(238, 238, 238);">//check time
    </span></span>{
        gesturePoints.RemoveAt(0);
        startPoint = gesturePoints[0];
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>((swipeLength < 0 && newPoint.X - startPoint.X < swipeLength) <span style="margin: 0px; padding: 0px; color: green;"><span style="background-color: rgb(238, 238, 238);">// check to see if distance has been achieved swipe left
        </span></span>|| (swipeLength > 0 && newPoint.X - startPoint.X > swipeLength)) <span style="margin: 0px; padding: 0px; color: green;"><span style="background-color: rgb(238, 238, 238);">// check to see if distance has been achieved swipe right
    </span></span>{
        gesturePoints.Clear();

        <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: green;">//throw local event
        </span><span style="margin: 0px; padding: 0px; color: blue;">if </span></span>(swipeDetected != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
            swipeDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(point) { Z = z, Cursor = cursorAdorner });
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>;
    }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(xOutOfBoundsLength != 0 &&
        ((xOutOfBoundsLength < 0 && newPoint.X - initialSwipeX < xOutOfBoundsLength) <span style="margin: 0px; padding: 0px; color: green;"><span style="background-color: rgb(238, 238, 238);">// check to see if distance has been achieved swipe left
        </span></span>|| (xOutOfBoundsLength > 0 && newPoint.X - initialSwipeX > xOutOfBoundsLength))
        )
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(swipeOutofBoundDetected != <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">null</span></span>)
            swipeOutofBoundDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">this</span></span>, <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(point) { Z = z, Cursor = cursorAdorner });
    }
}
### 7.5 磁性幻灯片(Magnetic Slide)


    磁性幻灯片是Kinect手势中的精华(holy grail)。他由Harmonix公司的交互设计师们在开发《舞林大会》（Dance Central）这一款游戏时创造的。最初被用在菜单系统中，现在作为一种按钮在很多地方有应用，包括Xbox自身的操作面板。他比磁性按钮好的地方就是，不需要用户等待一段时间。在Xbox游戏中，没有人愿意去等待。而下压按钮又有自身的缺点，最主要的是用户体验不是很好。磁性幻灯片和磁性按钮一样，一旦用户进入到按钮的有效区域，光标就会自定锁定到某一点上。但是在这一点上，可以有不同的表现。除了悬停在按钮上方一段时间触发事件外，用户可以划动收来激活按钮。


    从编程角度看，磁性幻灯片基本上是磁性按钮和划动手势(swipe)的组合。要开发一个磁性幻灯片按钮，我们可以简单的在可视化树中的悬浮按钮上声明一个计时器，然后再注册滑动手势识别事件。下面的代码展示了磁性幻灯片按钮的基本结构。其构造函数已经在基类中为我们声明好了计时器。InitializeSwipe和DeinitializeSwipe方法负责注册KinectCursorManager类中的滑动手势识别功能。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public class </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">MagneticSlide</span></span>:<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagnetButton
</span></span>{
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private bool </span></span>isLookingForSwipes;
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public </span></span>MagneticSlide()
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.isLockOn = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>;
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>InitializeSwipe()
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">if </span></span>(isLookingForSwipes)
            <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return</span></span>;
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>kinectMgr = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorManager</span></span>.Instance;
        kinectMgr.GesturePointTrackingInitialize(SwipeLength, MaxDeviation, MaxSwipeTime, xOutOfBoundsLength);

        kinectMgr.swipeDetected += <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(kinectMgr_swipeDetected);
        kinectMgr.swipeOutofBoundDetected += <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(kinectMgr_swipeOutofBoundDetected);
        kinectMgr.GesturePointTrackingStart();
    }

    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">private void </span></span>DeInitializeSwipe()
    {
        <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">var </span></span>KinectMgr = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorManager</span></span>.Instance;
        KinectMgr.swipeDetected -= <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(kinectMgr_swipeDetected);
        KinectMgr.swipeOutofBoundDetected -= <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventHandler</span></span>(kinectMgr_swipeOutofBoundDetected);
        KinectMgr.GesturePointTrackingStop();
        isLookingForSwipes = <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">false</span></span>;
    }

    另外，我们也需要将控件的滑动手势的初始化参数暴露出来，这样就可以根据特定的需要进行设置了。下面的代码展示了SwipeLength和XOutOfBoundsLength属性，这两个都是默认值的相反数。这是因为磁性幻灯片按钮一般在屏幕的右侧，需要用户向左边划动，因此，相对于按钮位置的识别偏移以及边界偏移是其X坐标轴的相反数。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>SwipeLengthProperty =<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"SwipeLength"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagneticSlide</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(-500d));<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>SwipeLength<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(SwipeLengthProperty); }<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(SwipeLengthProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>MaxDeviationProperty =<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"MaxDeviation"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagneticSlide</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(100d));<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>MaxDeviation<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(MaxDeviationProperty); }
    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(MaxDeviationProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>XOutOfBoundsLengthProperty =<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"XOutOfBoundsLength"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagneticSlide</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(-700d));<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public double </span></span>XOutOfBoundsLength<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">double</span></span>)GetValue(XOutOfBoundsLengthProperty); }<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(XOutOfBoundsLengthProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">DependencyProperty </span></span>MaxSwipeTimeProperty =<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">DependencyProperty</span></span>.Register(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"MaxSwipeTime"</span></span>, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">MagneticSlide</span></span>), <span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">UIPropertyMetadata</span></span>(300));<br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">public int </span></span>MaxSwipeTime<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">get </span></span>{ <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">return </span></span>(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">int</span></span>)GetValue(MaxSwipeTimeProperty); }<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">set </span></span>{ SetValue(MaxSwipeTimeProperty, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />}

    要实现磁性幻灯片按钮的逻辑，我们只需要处理基类中的enter事件，以及划动手势识别事件即可。我们不会处理基类中的leave事件，因为当用户做划动手势时，极有可能会不小心触发leave事件。我们不想破坏之前初始化好了的deactivate算法逻辑，所以取而代之的是，我们等待要么下一个划动识别成功，要么在关闭划动识别前划动手势超出识别范围。当探测到划动时，触发一个标准的click事件。
<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public static readonly </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEvent </span></span>SwipeOutOfBoundsEvent = <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">EventManager</span></span>.RegisterRoutedEvent(<span style="margin: 0px; padding: 0px; color: rgb(163, 21, 21);"><span style="background-color: rgb(238, 238, 238);">"SwipeOutOfBounds"</span></span>, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">RoutingStrategy</span></span>.Bubble,<br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventHandler</span></span>), <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">typeof</span></span>(<span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectInput</span></span>));<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">public event </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventHandler </span></span>SwipeOutOfBounds<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">add </span></span>{ AddHandler(SwipeOutOfBoundsEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">remove </span></span>{ RemoveHandler(SwipeOutOfBoundsEvent, <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">value</span></span>); }<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">void </span></span>KinectMgr_swipeOutofBoundDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    DeInitializeSwipe();<br style="margin: 0px; padding: 0px;" />    RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">KinectCursorEventArgs</span></span>(SwipeOutOfBoundsEvent));<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">void </span></span>KinectMgr_swipeDetected(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    DeInitializeSwipe();<br style="margin: 0px; padding: 0px;" />    RaiseEvent(<span style="background-color: rgb(238, 238, 238);"><span style="margin: 0px; padding: 0px; color: blue;">new </span><span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);">RoutedEventArgs</span></span>(ClickEvent));<br style="margin: 0px; padding: 0px;" />}<br style="margin: 0px; padding: 0px;" /><br style="margin: 0px; padding: 0px;" /><span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">protected override void </span></span>OnKinectCursorEnter(<span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">object </span></span>sender, <span style="margin: 0px; padding: 0px; color: rgb(43, 145, 175);"><span style="background-color: rgb(238, 238, 238);">KinectCursorEventArgs </span></span>e)<br style="margin: 0px; padding: 0px;" />{<br style="margin: 0px; padding: 0px;" />    InitializeSwipe();<br style="margin: 0px; padding: 0px;" />    <span style="margin: 0px; padding: 0px; color: blue;"><span style="background-color: rgb(238, 238, 238);">base</span></span>.OnKinectCursorEnter(sender, e);<br style="margin: 0px; padding: 0px;" />}



### 7.6 垂直滚动条(Vertical Scroll)


    并不是所有的内容都能够在一屏之内显示完。有时候可能有一些内容会大于屏幕的实际尺寸，这就需要用户来滚动屏幕或者列表控件来显示在屏幕之外的内容。传统上，垂直滚动条一直是交互界面设计的一个禁忌。但是垂直滚动条在划动触摸界面中得到了很好的应用。所以Xbox和Sony PlayStation系统中都使用了垂直滚动条来构建菜单。Harmonix’s的《舞林大会》（Dance Central）这一系列游戏使用了垂直滚动条式的菜单系统。Dance Central第一次成功的使用了垂直滚动界面作为手势交互界面。在下面的手势交互图中，当用户抬起或者放下手臂时会使得屏幕的内容垂直滚动。胳膊远离身体，抬起手臂会使得屏幕或者菜单从下往上移动，放下手臂会使得从上往下移动。



![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204280001267577.png)


    水平的划动在Kinect应用中似乎很常见(尤其是在Metro风格的Xbox游戏交互界面中，水平划动是占主导的手势)，但是垂直滚动用户体验更加友好，也是用户交互界面更好的选择。水平或者垂直划动手势有一些小的用户体验问题。另外，划动手势在识别上也较困难，因为挥动的形式和动作因人而异，且差别很大。就算同一个人，划动手势也不是一直不变的。划动手势在触摸屏设备上能够较好的工作是因为除非不触摸到屏幕，那么动作就不会发生。但是在手势识别界面上，用户的手是和视觉元素进行交互的，这时手就是在某一特定的坐标空间中的视觉元素。


     当用户做划动手势时，在整个手的划动过程中会手的位置在水平方向会保持相对一致。这就使得如果想进行多次连续的划动手势时会产生一些问题。有时候会产生一些比较尴尬的场景，那就是会无意中撤销前一次的划动手势。例如，用户使用右手从右向左进行划动手势，使得页面会跳转到下一页，现在用户的右手在身体的左边位置了，然后用户想将手移动回原始的开始位置以准备下一次的从右向左的挥动手势。但是，如果用于依然保持手在水平位置大致一致的话，应用程序会探测到一次从左向右的划动操作然后又将界面切换到了之前的那一页。这就使得用户必须创建一个循环的运动来避免不必要的误读。更进一步，频繁的划动手势也容易使得用户疲劳，而垂直方向的划动也只会加剧这一问题。


    但是垂直滚动条则不会有上述的这些用户体验上的缺点。他比较容易使用，对用户来说也更加友好，另外，用户也不需要为了保持手在水平或者垂直方向一致而导致的疲劳。从技术方面来讲，垂直滚动操作识别较划动识别简单。垂直滚动在技术上是一个姿势而不是手势。滚动操作的探测是基于当前手臂的位置而不是手臂的运动。滚动的方向和大小由手臂和水平方向的夹角来确定。下图演示了垂直滚动。



![](http://pic002.cnblogs.com/images/2012/94031/2012042800282797.png)


     使用之前的[姿势识别](http://www.cnblogs.com/yangecnu/archive/2012/04/16/KinectSDK_AdvanceSkeletonTracking_part2.html)那篇文章中的内容，我们能够计算从用户的身体到肩部和手腕的夹角，定义一个角度区间作为中间姿势，当用户手臂在这一区间内时，不会产生任何动作，如上图中的，当手臂自然处于-5度或者355度时，作为偏移的零点。建议在实际开发中，将零点的偏移上下加上20度左右。当用户的手臂离开这一区域时，离开的夹角及变化的幅度根据需求而定。但是建议至少在0度区间上下有两个区间来表示小幅和大幅的增加。这使得能够更好的实现传统的人机交互界面中的垂直滚动条的逻辑。

### 7.7 通用暂停按钮(Universal Pause)


    暂停按钮，通常作为引导手势或者退出手势，是微软建议在给用户提供引导时很少的几个手势之一。这个手势是通过将左臂保持和身体45度角来完成的。在很多Kinect的游戏中都使用到了这一手势，用来暂停动作或者唤出Xbox菜单。和本文之前介绍的手势不一样，这个手势并没有什么符号学上的含义，是一个认为设计的动作。通用暂停手势很容易实现，也不一定要限制手臂，并且不容易和其他手势混淆。


通用暂停手势的识别和垂直滚动手势的识别有点类似，就是计算左臂和身体的夹角，然后加上一个阈值即可，相信很简单，在这里就不再赘述了。

### 2.8测试Demo


    结合前篇文章中的挥动(wave)手势识别，以及上文将的几种按钮，做了一个小的例子，使用之前开发的手势识别库，以及手势识别按钮。这部分代码很简单，直接引用之前在类库中定义好的控件即可。大家可以下载本文后面的代码自己回去实验一下。 截图如下：

![未标题-2w](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204280001261514.png)

![未标题-1w](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/20120428000126152.png)

## 8. 手势识别的未来


    我们进入商场可能对各种商品不会有很深的印象，同样，随着时间的流逝，Kinect也会变得不那么令人印象深刻，甚至被大多数人忘记。但是，随着软硬件技术的发展，Kinect或者相似的技术会融入到生活的各个方面，让你看不到Kinect的存在。


    当我们进入一个购物商场时，当你靠近入口时，门会自动打开。这在当时很让人印象深刻，但没有人会注意，观察甚至意识到这一特性。在未来的某一天，Kinect也会像自动门一样融入生活的方方面面。使得我们感受不到他的存在。


    Kinect以及NUI的世界才刚刚开始，随着时间的推移，这种交互体验会发生巨大变化。在电影《少数派报告》(Minority Report)中，汤姆克鲁斯使用手势浏览和打开大屏幕上的各种文件和影像资料，这一场景现在已经是基于Kinect的应用程序的要实现目标之一。 有时候，科幻片中比现实有更好的想像力，也能提供比现实生活中更好的科技。在星际迷航(Star Trek),星球大战(Star Wars)或者 2001：太空漫游(2001: A Space Odyssey)这些科幻电影中，电脑能够看到和感应人的存在。在这些电影中，用户使用语音和手势无缝的和电脑进行交互。当然这种交互也有负面因素，应该设置一些限制。


    虽然科幻电影变成现实的这一前景会引发一些理性的担忧，但是这种变化正在到来。意识到这种变化带来的好处很重要。Kinect及其类似的技术使得我们的生活环境更加智能化。它使得应用程序能够识别用户的手势，进而能够分析出用户的意图，而不需要用户明确的给出这些信息或者指令。现在的Kinect游戏是基于应用程序查找特定的用户的手势而进行操作的，用户必须主动的和应用程序进行交流或者发送指令。但是，还是有很多用户的信息没有被捕捉和处理。如果应用程序能够探测到其他信息，确切的说，如用户的情绪，那么就可以提供更加人性化的定制服务了。现在我们所识别的姿势都很简单，我们只是在学习如何建立交互界面，可以预见在未来，随着基于手势交互的应用程序越来越多，这种用户界面就会消失，就像移动设备中触摸界面逐渐取代光标那样。


    想象一下，下班后回到家，走到卧室，说一句“电脑，放点music”。于是电脑就会识别语音指令开始播放音乐。但是，电脑也能够识别到你工作了一天，需要一些音乐来放松心情，于是电脑自动的根据你的心情选择一些歌曲。语音成了首要的发送指令的形式，手势来对一些指令进行增强。在上面的例子中，电脑能够根据你的身体语言，识别你的情绪，这样，手势是一种主动的，有上下文情景的和电脑进行交互的方法。这并不意味这手势变得不那么重要，相反重要性增加了，只不过是以一种间接的方式。


    如今有一些声控传感器，例如能够根据声音探测到人进入到房间，然后开灯或者关灯的声控开关。这是一种比较笨拙的系统，他没有上下文提供。如果使用Kinect技术，它能够识别用户的运动，然后根据情形调整灯光的亮度。例如，如果在凌晨2点，你想起床喝点水，电脑可能会将灯光调整的比较暗，以至于不会太刺眼。但是如果某一天你凌晨2点钟从外面回来，Kinect识别到你是清醒的，就会把灯全部打开。


    目前，Kinect仍然是一种比较新的技术，我们仍然试图理解然后能够更好的发挥它的潜能。在最开始Kinect出来的时候只是观看或者体验。随着他作为Xbox外设的发布，游戏的主题也有了一些限制。大多数游戏都是运动型的，这些游戏都只能识别用户的一些基本手势，如跑，跳，踢，划动，扔等这些手势或动作。早期的Kinect体感游戏也只有一些简单的菜单系统，使用手来进行操作。


虽然用户体验设计发生了巨大变化，但是目前基于手势的游戏和应用比较简单。我们仍然处在学习如何定义和识别手势的阶段。这使得我们的手势有些粗糙。我们仍需要扩大手势的幅度才能达到好的识别效果。当我们能够识别到手势的细微方面时，应用程序所带来的沉浸感将会大大提升。


    现在的足球游戏只能识别到基本的踢球动作，游戏不能够判断用户是使用脚趾，脚背，脚踝还是脚跟跟球进行交互的。这些不同的姿势可能对球产生完全不同的影响，从而使得游戏产生不同的结果。更进一步，游戏应该能够根据用户踢球的动作，脚的位置，应用一些物理特性，给予球一些真实的加速度，旋转，弧度等特性，这样会使得游戏更加真实，玩家也更加有沉浸感。


    目前的这些限制主要是由Kinect摄像头的分辨率决定的。下一代的Kinect硬件设备可能会使用更高分辨率的摄像头来提供更好的深度影像数据。微软已经放出了关于第二代Kinect硬件方面的相关信息。这使得更精确的手势识别变为可能，给基于Kinect的应用程序开发带来了两个方面的改进。首先是骨骼关节精度的提升，这不但能够提升手势识别的精度，也能够扩大手势识别的种类。另一个改进是使得能够产生一些额外的关节点，如手指的信息，以及一些非关节点如嘴唇，鼻子，耳朵，眼睛等位置信息。如今这些信息都能够识别的出来，只是需要使用一些第三方的类库，官方的SDK中没有原生的对这些特征进行支持。


    对手指进行追踪和识别能够大大的提高符号语言的作用。如果应用程序能够识别手指的运动，用户就能够使用手指进行更高精度和更自然的操作。手指手势交互信息很复杂，在用户进行交流的过程中能够提供丰富的上下文信息。即使能够识别到手指手势，今天的基于Kinect的间交互体验也没有发生革命性的变化，这是因为用户依然必须意识到Kinect的存在，还要知道如何和这些硬件交互，需要做什么样的手势才能达到想要的结果。当你看别人玩Kinect游戏时，你会注意到他是如何在Kinect前面做动作的。用户的姿势有时候比较僵硬和不自然。很多姿势并不能识别，一些需要重复多次，更坏的情况下姿势会被错误识别，这样就会导致一些意想不到的操作。更进一步，用户的手势通常需要过分的夸大才能被kinect识别的到。这些问题可能都是暂时的。在未来，随着软硬件的不断升级和改进，在使用基于手势的人机交互界面时，用户会越来越感到舒服和自然。从这方面讲，Kinect是一个神奇的设备，以至于会像自动门那样被广大的用户所知道和接受。


手势识别数据集：

本文转载自http://www.cnblogs.com/tornadomeet/archive/2013/01/10/2854421.html




ChaLearn Gesture Challenge挑战赛是手势识别中比较新的(2011年开始的)一个挑战赛，属于[机器学习](http://lib.csdn.net/base/machinelearning)挑战赛中的一个，其初衷是进行One-Shot learning的挑战，当然也不局限在此。官网为：[http://gesture.chalearn.org/](http://gesture.chalearn.org/) 
 ,2012年分别主办了两轮比赛，一个是在CVPR2012中进行，另一个是在ICPR2012中进行。关于本次比赛的一些规则，以及已经完成比赛的作者的识别方法的一些细节在官网上都能查看得到。大家有兴趣13年也可以参加下。因为在日常生活中会遇到各种各样的手势，如果对这些手势的识别能取得很大成功的话，那么离AI又更近一步了，下面这张图是生


活中常见产生手势的场所：

![](http://images.cnitblog.com/blog/381513/201301/10112735-2eb2d82cfd4d457c866da2e6bb5e7663.png)


　　本文主要是简单介绍下关于这个挑战赛的[数据库](http://lib.csdn.net/base/mysql)，数据库的英文名称为ChaLearn Gesture Data，简称CGD，或者CGD2011。数据库可以从[http://gesture.chalearn.org/data/cgd2011](http://gesture.chalearn.org/data/cgd2011)这里下载，如果下载压缩版的则大小约5G，否则无压缩版的则达到30G。考虑到即使压缩过后，其实也不会对识别结果有太大的影响，且本人电脑承受能力有限，所以我这里下载了个压缩版的。


　　当然了，下载下来后的数据库都是视频格式，分为深度图像视频和彩色图像视频，因为在录制视频的时候是用Kinect录制的。由此可见，这些手势识别既可以能有深度信息又可以利用色彩信息，这对搞深度数据的行为识别的人算是一个很好的公共数据库了。


　　下面是这个数据库的一些特征：

**1.** 共有30个左右的手势单词，也就是说有30个基本的手势动作。

**2.** 视频数据分为500个batch，每个batch含有94个视频，视频分为2种，每种各47个。如果视频名称为M_x.avi的话，则表示是RGB图像的视频，否则名称为K_x.avi表示的是深度图像的视频。每个视频有可能有多个基本手势动作，一般是1~5个，且每个batch最终是100个手势，很明显这中间有重复的手势(因为总共才有30个手势单词)。因此，最终有50000=500*100个手势。每个batch如下图显示：

![](http://images.cnitblog.com/blog/381513/201301/10112834-913c5c8dda7b48c6af4660ce4932ae53.png)


　　当然了，其中还包含了2个csv文件，用来描述训练和测试数据用的（具体应用还没弄过）。

**3.**视频数据的录制是由20个人完成的。取出一个人的某一个手势的深度图和RGB图，显示如下：

![](http://images.cnitblog.com/blog/381513/201301/10112912-d07d2b31e060423d9f8e7e7b843e35bd.png)![](http://images.cnitblog.com/blog/381513/201301/10112920-c3ede62f314348b59b478069b7e1bdfd.png)
**4.** 视频中每一张图片的大小为240*320.

**5.** 视频的帧率是每秒10帧。

**6.** 这500个batch又分为2种，即”devel”和”valid”,其中”devel”有480个，”valid”有20个。”devel”表示可用于挑战赛开发(development)的数据库，这些数据提供了训练数据和测试数据的标注。”valid”表示用于验证(validation)的，只提供了训练数据的标签。测试结果需要在网络上提交。

**7.**深度图像是以灰阶的形式显示出来的，其计算公式如下所示：


　　f(x)=(x-mini)/(maxi-mini)，max为最大距离，min为最小距离。X是当前点的距离，f(x)为深度图像显示时的灰度值。

**8.**深度图像也是有不同精度的，因为录制数据时的软件版本不同。这里分为3个精度，0表示一般（Mediocre），1表示比较好(Good),2表示非常好(Very Good). 比如说前面几个batch的精度显示如下（num表示序号，acc表示精度， miss表示漏洞的文件）：

![](http://images.cnitblog.com/blog/381513/201301/10112934-10bd2eb3fab043ccbaa04b34fc7e4432.png)


　　如果大家是研究手势识别的话，特别是基于Kinet深度信息的，可以一起交流下，挑战下这个数据库！




　　参考资料：

[http://gesture.chalearn.org/](http://gesture.chalearn.org/)







﻿﻿

﻿﻿




