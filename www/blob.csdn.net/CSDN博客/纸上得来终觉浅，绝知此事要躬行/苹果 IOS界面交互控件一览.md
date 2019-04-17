# 苹果 IOS界面交互控件一览 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月04日 14:22:22[boonya](https://me.csdn.net/boonya)阅读数：1871








文章来自百度百科：[http://baike.baidu.com/link?url=4OUX5cErT66qGsbRgSt6eewzcTsdndubuuSdHYMDwU3JB4Hv9c2sYlH-Y1eL0o2nfP-uxH9OOAb1HWwP8NxNclzHX1CbAlyYHWt3UySfyze](http://baike.baidu.com/link?url=4OUX5cErT66qGsbRgSt6eewzcTsdndubuuSdHYMDwU3JB4Hv9c2sYlH-Y1eL0o2nfP-uxH9OOAb1HWwP8NxNclzHX1CbAlyYHWt3UySfyze)

iPhone的 iOS 系统的开发需要用到控件。开发者在[iOS](http://baike.baidu.com/view/158983.htm)平台会遇到界面和交互如何展现的问题，[控件](http://baike.baidu.com/view/185331.htm)解决了这个问题。使得iPhone的用户界面相对于老式手机，更加友好灵活，并便于用户使用。下面介绍下iPhone常用的[控件](http://baike.baidu.com/view/185331.htm)：

### 窗口

UIWindow，iPhone的规则是一个窗口，多个视图，窗口是你在app显示出来你看到的最底层，他是固定不变的，基本上可以不怎么理会，但要知道每层是怎样的架构。

### 视图

UIView，是用户构建界面的基础，所有的控件都是在这个页面上画出来的，你可以把它当成是一个画布，你可以通过UIView增加控件，并利用控件和用户进行交互和传递数据。

窗口和视图是最基本的类，创建任何类型的用户界面都要用到。窗口表示屏幕上的一个几何区域，而视图类则用其自身的功能画出不同的控件，如导航栏，按钮都是附着视图类之上的，而一个视图则链接到一个窗口。

### 视图控制器

视图控制器UIViewController，你可以把他当成是对你要用到视图UIView进行管理和控制，你可以在这个UIViewController控制你要显示的是哪个具体的UIView。另外，视图控制器还增添了额外的功能，比如内建的旋转屏幕，转场动画以及对触摸等事件的支持。

### UIKit
- 
显示数据的视图


UITextView：将文本段落呈现给用户，并允许用户使用键盘输入自己的文本。

UILabel：实现短的只读文本，可以通过设置视图属性为标签选择颜色，字体和字号等。

UIImageView：可以通过UIImage加载图片赋给UIImageView，加载后你可以指定显示的位置和大小。

UIWebView：可以提供显示HTML.PDF等其他高级的Web内容。包括xls，word等文档等。

MKMapView：可以通过MKMapView向应用嵌入地图。很热门的LBS应用就是基于这个来做的。还可以结合MKAnnotationView和MKPinAnnotationView类自定义注释信息注释地图。

UIScrollView：一般用来呈现比正常的程序窗口大的一些内容。可以通过水平和竖直滚动来查看全部的内容，并且支持缩放功能。
- 
做出选择的视图


UIAlertView：通过警告视图让用户选择或者向用户显示文本。

UIActionSheet：类似UIAlertView，但当选项比较多的时候可以操作表单，它提供从屏幕底部向上滚动的菜单。

**其他**

UIButton：主要是我们平常触摸的按钮，触发时可以调用我们想要执行的方法。

UISegmentControl：选择按钮，可以设置多个选择项，触发相应的项调用不同的方法。

UISwitch：开关按钮，可以选择开或者关。

UISlideer：滑动按钮，常用在控制音量等。

UITextField：显示文本段，显示所给的文本。

UITableView：表格视图，可以定义你要的表格视图，表格头和表格行都可以自定义。

UIPickerView：选择条，一般用于日期的选择。

UISearchBar：搜索条，一般用于查找的功能。

UIToolBar：工具栏：一般用于主页面的框架。

UIActivityIndicatorView：进度条，一般用于显示下载进度。

UIProgressView：进度条，一般用于显示下载的进度条。

但是随着IPhone的流行发展，iPhone原生的界面控件无法满足产品日益增长的功能需要，成为大的矛盾。IPhone鼓励用户创新，因此出现了更多的IPhone[控件](http://baike.baidu.com/view/185331.htm)，使得开发者可以将现有的技术应用在iPhone平台，并创建完美的桌面、WEB和移动应用程序。 其他基于IPhone的[控件](http://baike.baidu.com/view/185331.htm)，如：[ComponentOne
 Studio](http://baike.baidu.com/view/4665482.htm)for iPhone等。




