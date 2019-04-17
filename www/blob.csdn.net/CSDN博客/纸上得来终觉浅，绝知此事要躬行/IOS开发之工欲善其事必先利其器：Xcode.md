# IOS开发之工欲善其事必先利其器：Xcode - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月04日 14:41:11[boonya](https://me.csdn.net/boonya)阅读数：720标签：[ios开发																[xcode](https://so.csdn.net/so/search/s.do?q=xcode&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[IOS](https://blog.csdn.net/boonya/article/category/5257629)








       工欲善其事，必先利其器。正所谓，磨刀不误砍柴工。又所谓，欲练此功，必先自宫；当然也别自宫了，没那么严重，只是强调一下做事情必须要有个先后顺序，不然事情就不能很好地开展下去。Xcode是一款优雅而具有吸引力的IDE，摘自IOS developer网上的工具介绍：




## IOS说您会喜欢的工具

Xcode IDE 是 Apple 开发体验的核心。Xcode 与 Cocoa 和 Cocoa Touch 框架紧密集成，为构建精彩的 Mac、iPhone 和 iPad 版 app 带来难以置信的高效环境。

![](https://devimages.apple.com.edgekey.net/xcode/ide/images/ide.png)

### 从概念到代码再到客户，Xcode 带您顺利完成

一切都完美集成，让工作流程自然顺畅。当您编写新界面时，Assistant编辑器会在拆分窗格中直观地呈现相关源代码。只需拖动鼠标，即可将 UI 控件与实现代码关联起来。Apple LLVM 编译器技术可解析代码，使您在 LLDB 调试器中看到的每个符号都与编辑器和编译器中的内容保持一致。当您打字时，同一引擎持续处于工作状态，查找错误并提供代码错误修复。

Xcode 甚至能够与 Apple 开发者网站通信，因此您只需点击一下，即可在 app 中启用 Game Center 或 Passbook 等服务。Xcode 与 OS X Server 搭配使用，可以设置远程 bot，以便连续构建、分析、测试甚至封装 app。当 app 完成时，Xcode 将打包 app 并将其提交至 App Store。

### Assistant编辑器

Assistant 按钮将 Xcode 编辑器拆分为两部分，左侧为主工作文稿，右侧为智能 Assistant 编辑器窗格。根据您在主编辑器中执行的工作，Assistant编辑器会自动显示 Xcode 确定为对您最有帮助的文件。例如，如果您正在编辑器中编辑 MyClass.m，Assistant 将自动显示对应的 MyClass.h。

![](https://devimages.apple.com.edgekey.net/xcode/ide/images/assistant-editor.png)

### 跳转栏

点按位于每个编辑器窗格顶部的跳转栏，可以快速选择要在 Assistant编辑器中查看的信息。例如，当在主编辑器中编辑源代码时，Assistant 可以显示对应标题、子类或父类或者相关测试。

![](https://devimages.apple.com.edgekey.net/xcode/ide/images/jump-bar.png)

### Interface Builder

Interface Builder 设计画布完全集成在 Xcode IDE 中，可以轻松呈现完整用户界面的原型，而无需编写任何代码。在数分钟内创建出原型，然后以图形方式将界面关联到 Xcode 编辑器中的源，设置窗口、按钮和滑块的布局，从而创建能够正常运行的 Mac、iPhone 或 iPad 用户界面。使用 Assistant 编辑器，您可以使用实现源代码并行操作图形设计。只需用鼠标将 UI 控件拖入源窗格，即可创建代码与界面之间的关联，甚至可以为您创建代码存根。[了解详情](https://developer.apple.com/cn/xcode/interface-builder/)

![](https://devimages.apple.com.edgekey.net/xcode/interface-builder/images/interface-builder.png)
![](https://devimages.apple.com.edgekey.net/xcode/ide/images/source-control.png)
### 版本编辑器和源代码控制

版本编辑器可以轻松比较文件的两个版本，查看提交日志，检查谁更改了代码，甚至可以纵观提交时间线。版本编辑器拆分窗格，以显示同一文件的两个不同版本。当您查看拆分编辑器视图的时间线时，差异会突出显示。Xcode 还可为新项目创建本地 Git 存储库，或签出托管的 Subversion 或 Git 存储库。顶层源代码控制菜单可轻松执行分离和合并操作，非常适用于分散式团队。

### 测试

测试驱动的开发是 Xcode 中一流的工作流程。使用测试导航器，可以轻而易举地跳转到项目中的任何测试、执行一个单独的测试或者执行一组测试。Assistant 编辑器拥有针对测试的新型视图，可以自动跟踪哪些测试在执行当前编辑的代码，从而让您的测试和代码始终保持同步。

![](https://devimages.apple.com.edgekey.net/xcode/ide/images/testing.png)
![](https://devimages.apple.com.edgekey.net/xcode/ide/images/integration-bots.png)
Xcode 还与 OS X Server 集成，可创建连续集成 bot。这些 bot 在 Xcode 中配置，告诉服务器何时以及如何自动构建、分析、测试和归档项目。团队各成员可以轻松在 Xcode UI 中查看这些集成的历史结果。
[进一步了解 OS X Server](http://www.apple.com/osx/server/)

### 自定义

Xcode 环境可以配置，几乎可以满足所有工作流程，包括诸如标签、行为和代码段的自定义功能。

![](https://devimages.apple.com.edgekey.net/xcode/ide/images/customize.png)

#### 标签

使用标签为项目创建独一无二的视图。每个标签都有自己的导航器、编辑器、助理和实用工具布置区域。您可以为特定任务创建命名标签，重新排列标签，或撤销标签以创建独立的窗口。

#### 行为

告诉 Xcode 对事件（比如启动调试会话或构建时遇到错误）执行什么操作。结合标签，您可以为自己的编辑、设计、构建或调试任务创建自定义工作环境。自定义行为还可通过单一的组合键，彻底重新排列您的窗口。

#### 代码段

代码段库中有数十种预配置代码实现方式，比如定义新类或方法。通过自定义或添加代码段，只需键入少量字符即可插入那些频繁输入的代码。
![](https://devimages.apple.com.edgekey.net/xcode/ide/images/open-quickly.png)
### 快速打开

使用快速打开 (Command-Shift-O) 功能，可快速访问项目使用的任何文件。Xcode 即刻提供搜索的实现方式，您可以选择一个方式，然后点击 Return 来打开文件，或点击 Option-Return 在 Assistant 编辑器中打开文件。
![](https://devimages.apple.com.edgekey.net/xcode/ide/images/schemes.png)
### 方案

根据调试、剖析、执行代码分析或运行测试套件等具体操作，自定义 Xcode 构建和运行 app 的方式。例如，默认方案是运行时在“调试”模式下构建 app，执行剖析或归档命令时，将针对“释放”构建相同的方案设置。执行各个任务时，无需更改项目设置。Xcode 会自动配置方案，也可使用“Manage Schemes”（管理方案）菜单进行自定义。
[进一步了解 Interface Builder](https://developer.apple.com/cn/xcode/interface-builder/)










