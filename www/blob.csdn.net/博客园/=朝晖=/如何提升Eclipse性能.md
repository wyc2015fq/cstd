# 如何提升Eclipse性能 - =朝晖= - 博客园
# [如何提升Eclipse性能](https://www.cnblogs.com/dhcn/articles/7100261.html)
[http://pengfeng.iteye.com/blog/246537](http://pengfeng.iteye.com/blog/246537)
Eclipse作为开发人员强力推崇的开发工具之一,其与JBuilder相比,很多同盟都认为使用Eclipse比使用JBuilder省内存,而且Eclipse属于免费开源项目,而像JBuilder和IDea等均属于商业非免费工具,当然还有其他一些说法。。。
个人选择Eclipse作为主要开发工具的原因主要有三点:
1、免费且开源
2、有很多开源、好用的插件，而且这些插件都是一些经验人士的杰作，能够大大方便和提高我们的开发（这里对他们表示感谢）
3、有像IBM这样本人崇拜的大型公司的支持
本文主要根据个人在使用Eclipse过程中的一些经历，总结和收集一下Eclipse开发工具性能的提升的方法，目前主要包括如下几种，如果大家有什么更好的提升Eclipse性能的方法或者好用的Eclipse插件、技巧等，欢迎大家回复讨论。
### **一、取消自动validation**
validation有一堆，什么xml、jsp、jsf、[js](http://lib.csdn.net/base/javascript)等等，我们没有必要全部都去自动校验一下，只是需要的时候才会手工校验一下！
**取消方法：**
windows–>perferences–>Validation
除开Manual下面的复选框全部选中之外，其他全部不选
**手工验证方法：**
在要验证的文件上，单击鼠标右键–>validate
### **二、取消Eclipse拼写检查**
1、拼写检查会给我们带来不少的麻烦，我们的方法命名都会是单词的缩写，他也会提示有错，所以最好去掉，没有多大的用处
windows–>perferences–>general–>Editors->Text Editors->spelling
### **三、取消myeclipse的启动项**
随着集成的插件的增多，eclipse会有很多的启动项，而其中很多我们已经放弃使用或者都用不着，或者只用一两个，取消前面不用的就可以
windows–>perferences–>general–>startup and shutdown
### **四、更改jsp默认打开的方式**
该方法主要针对使用myeclipse插件的人员，安装了myeclipse后，编辑jsp页面，会打开他的编辑页面，同时也有预览页面，速度很慢，不适合开发。所以更改之
windows–>perferences–>general–>editors->file associations
五、更改代码提示快捷键
该方法主要针对使用myeclipse插件的人员，现在的代码提示快捷键，默认为ctrl+space，而我们输入法切换也是，所以会有冲突。谁叫myeclipse是外国人做的呢。。根本不需要切换输入法.
windows–>perferences–>general–>Keys
更改 content assist 为 alt+/
同时由于alt+/已经被word completion占用，所以得同时修改word completion的快捷键值

