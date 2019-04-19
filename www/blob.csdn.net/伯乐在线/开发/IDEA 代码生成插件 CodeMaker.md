# IDEA 代码生成插件 CodeMaker - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [肖汉松](http://www.jobbole.com/members/Yonah) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 前言
Java 开发过程中经常会遇到手工编写重复代码的事情，例如说：编写领域类和持久类的时候，大部分时候它们的变量名称，类型是一样的，在编写领域类的时候常常要重复写类似的代码。所以开发了一个 IDEA 的代码生成插件，通过 Velocity 支持自定义代码模板来生成代码。
![demonstration](http://7xjtfr.com1.z0.glb.clouddn.com/codemaker.gif)
**项目地址**：[CodeMaker](https://github.com/x-hansong/CodeMaker)
## 主要功能
- 支持增加自定义代码模板（Velocity）
- 支持选择多个类作为代码模板的上下文
## 安装
下载插件：[CodeMaker.zip](https://github.com/x-hansong/CodeMaker/releases/download/1.0/CodeMaker.zip)
- 打开设置，选择“Plugin”
- 在右边的框中点击“Install plugin from disk”
- 选择上面下载的“CodeMaker.zip”
- 点击“Apply”，然后重启 IDEA。
## 使用
在 Java 类编辑界面右键“Generate”，选择对应模板即可自动生成代码到当前类的包，然后进行修改，并且移动到合适的位置。
![codemaker0](http://7xjtfr.com1.z0.glb.clouddn.com/codemaker0.png)
如果代码模板需要除了当前类之外的类作为上下文，可以通过类选择框进行选择。
![codemaker1](http://7xjtfr.com1.z0.glb.clouddn.com/codemaker1.png)
目前自带的两个模板：
- **Model**：根据当前类生成一个与其拥有类似属性的类，用于自动生成持久类对应的领域类
- **Converter**：该模板需要两个类作为输入的上下文，用于自动生成领域类与持久类的转化类。
上面两个模板是我自己工作中常用的模板，大家可以参考其写法，自己定义新的代码模板。
## 模板配置
![codemaker3](http://7xjtfr.com1.z0.glb.clouddn.com/codemaker3.png)
- **增加模板**：点击“Add Template”后，填写相关配置（都不能为空），点击保存后重启 IDEA 才能生效。
- **删除模板**：点击“Delete Template”就能将该模板删除，同样需要重启才能生效。
![codemaker2](http://7xjtfr.com1.z0.glb.clouddn.com/codemaker2.png)
- **Template Name**：在生成菜单中显示的名称，英文命名
- **Class Number**：该模板需要的输入上下文类的数量，例如：如果为 1，,将当前的类作为输入：`$class0`；如果为 2，需要用户再选择一个类作为输入：`$class0, $class1`。
- **Class Name**：生成的类的名称，支持通过 Velocity 进行配置，上下文为跟代码模板的相同。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/5bf123eeb2d2e5d53b487415f6c0555c.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/c7fcb7a46ab889ed5518728242187ab5.jpg)
