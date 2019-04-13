
# 设计优秀的iPhone应用的五个建议 - 雷锋网 - CSDN博客


2012年09月06日 15:06:42[leiphone](https://me.csdn.net/leiphone)阅读数：710


![](http://www.leiphone.com/wp-content/uploads/2012/09/iPhone-design.jpg)当用户在苹果应用商店里寻找新应用时，往往基于设计来考量是否购买。生活中，或许很多人告诫我们不要凭借封面去评判一本书；既然无法试用一款应用，那么截图成为我们评判一款应用质量好坏的重要依据。
既然用户无法通过浏览App Store来判断应用的易用性和代码健壮性，那么将设计作为评判依据是合理的，这意味着同类应用竞争时，往往具备优秀的体验设计的应用更受欢迎。
如何设计优秀的iPhone应用呢？有如下五点建议。
## 1. 应用线框图
当我们讨论应用设计时，通常包含两个主要因素：**用户体验设计（user experience design）**和**用户界面设计（user interface design）**。
> 体验设计包括应用的意图（如包含哪些功能点）以及用户如何完成这些意图。

> 用户界面设计主要专注于体验的可视化，包括颜色、纹理、字体等元素。
设计iPhone应用时，上述的两个因素都至关重要。当然，首先你得花时间来构建用户体验的实体，往往通过线框图来实现。线框图（wireframe）能对应用创意进行简单勾勒，专注于体验部分，而忽略视觉效果。（更多内容参考[benefits
 of wireframing your designs](http://sixrevisions.com/user-interface/wireframing-benefits/)）我喜欢用笔在纸上画草图，当然也有人喜欢用Illustrator或者是[Balsamiq
 Mockups](http://www.balsamiq.com/products/mockups)。
![](http://www.leiphone.com/wp-content/uploads/2012/09/iphone_facebook_wireframe.png)
图片来源：[MOObileFrame](http://moobileframes.tumblr.com/post/18381747048/iphone-wireframes-created-using-wireframesketcher)
需要注意的是，工具无关紧要，不必纠结。你需要集中精力去规划应用的功能，使之更加融洽和谐；你需要考虑用户第一次过来要如何呈现；你需要思考用户的操作逻辑。画按钮，写文字，尤其是竭力让学习的过程更直观。
只有开始时就注重风格，你才能确信你的线框图呈现的是一个清晰、易用的应用设计方案。
## 2. 手指大小的点击目标
这是提升应用易用性最显著的点：扩大每个按钮的可点击区域。你是为拇指设计的，不是精确点击的鼠标箭头。
![](http://www.leiphone.com/wp-content/uploads/2012/09/minimum_tappable_area.png)
苹果推荐的元素最小可点操作区域为44x44px。这并不意味着按钮需要很大。可点区域可以根据按钮的视觉大小进行扩展，这样可以消除用户多次尝试性点击带来的烦恼。得特别注意那些相互靠近的多个按钮，额外的可点区域不能盖住其他的按钮。
## 3. 每个界面只有一个主要的目标点
应用的一个界面，应该聚焦于你想传达的主要目的。例如，iPhone邮件应用中的邮件列表界面，用户的主要目的是浏览邮件。虽然存在撰写邮件的第二操作，但是按钮放置于角落，并不突出。
![](http://www.leiphone.com/wp-content/uploads/2012/09/iphone_mail_app_email.jpg)
设计师的一个任务就是进行决策：哪些是最重要的，然后突出她。降低次要元素的视觉比重，让主要操作更加明了。
阅读以下文章，可以帮助你在应用中创建优秀的可视化层次结构和优先级：
> [Working
 with Visual Weight in Your Designs](http://sixrevisions.com/graphics-design/visual-weight-designs/)

> [Creating
 Focal Points in Your Web Design](http://sixrevisions.com/web_design/creating-focal-points/)

> [Using
 Power Structure and Gestalt for Visual Hierarchy](http://sixrevisions.com/web_design/using-power-structure-and-gestalt-for-visual-hierarchy/)

> [The
 Art of Distinction in Web Design](http://sixrevisions.com/web_design/the-art-of-distinction-in-web-design/)

## 4. 避免默认的按钮风格
iPhone中UIButtons控件的默认风格可以被视为我的眼中钉（design pet peeves）。似乎所有的iOS默认控件都看起来不错。
![](http://www.leiphone.com/wp-content/uploads/2012/09/defaultui_buttons.jpg)
除非你的应用风格就是让人不爽（不排除有），否则尽量避免使用默认的按钮风格。让按钮与UI相融合会让你眼前一亮，同时让你远离蹩脚设计师的称号。
你可以利用背景图片来设置按钮风格，亦或是通过代码重画一个新按钮。
有许多教程和资料帮助你创建更具吸引力的按钮，如下：
[Designing
 Buttons in iOS 5](http://nathanbarry.com/designing-buttons-ios5/)
[UIButton
 Glossy ](https://github.com/GeorgeMcMullen/UIButton-Glossy)
## 5. 为臃肿的信息提供额外的视图
对于用户来说，转移到另外一个视图（通过推送或视图动画）是很容易的事儿。所以当你尝试在一个视图里添加更多信息时，不妨做做减法，将其转移到另外一个视图。
iPhone里，你经常可以在“创建”视图里碰到这种使用场景。例如，新联系人界面，选择铃声（ringtone）时，你会进入新的Ringtones视图，其中展示了可选择的铃声列表。
![](http://www.leiphone.com/wp-content/uploads/2012/09/newcontact_ringtone_view.png)
点击电话号码的标签，你可以设置标签名。
![](http://www.leiphone.com/wp-content/uploads/2012/09/newcontact_label_view.png)
通过添加额外的视图，仅展示需要的信息，这样可以避免用户因大量视图元素带来的困惑。这种设计理念亦被称为渐进式展开（progressive disclosure）。
### 译者注：
首先这几点可能很多实践中的人已经知晓，那么可以忽略。毕竟翻译它的目的，主要是自我确认。
大家看后，请不要移花接木，这几点仅限于iPhone，是否通用于Android或iPad，无法考量，至少我目前看来，有些点在iPad上不具其合理性。
另外，从作者推荐的资源和相关设计模式，可以发现，其实移动应用设计中，还是借鉴了很多Web设计的方法和理念精髓。如渐进式展开（progressive disclosure）可参见[10
 UI Design Patterns You Should Be Paying Attention To](http://uxdesign.smashingmagazine.com/2009/06/23/10-ui-design-patterns-you-should-be-paying-attention-to/)（译文：《[10个值得你关注的UI设计模式](http://hi.baidu.com/xxucd/item/9bd3120e1e0517d973e676d8)》）。所以，在学习移动设计时，并不能一味地摒弃Web的设计模式和相关方法，虽然场景不同，但处理技巧可以互相借鉴和更迭。
[译者博客](http://www.hoowolf.net/)via：[sixrevisions](http://sixrevisions.com/mobile/design-iphone-apps-better/)

**（****[huyipeng](http://www.leiphone.com/author/huyipeng)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

