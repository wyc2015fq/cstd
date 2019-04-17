# ElementUI腾讯云开发者手册指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月05日 14:19:42[boonya](https://me.csdn.net/boonya)阅读数：286








原文地址:[https://cloud.tencent.com/developer/chapter/18050](https://cloud.tencent.com/developer/chapter/18050)

**目录**

[设计原则](#%E8%AE%BE%E8%AE%A1%E5%8E%9F%E5%88%99)

[一致性 Consistency](#%E4%B8%80%E8%87%B4%E6%80%A7%20Consistency)

[反馈 Feedback](#%E5%8F%8D%E9%A6%88%20Feedback)

[效率 Efficiency](#%E6%95%88%E7%8E%87%20Efficiency)

[可控 Controllability](#%E5%8F%AF%E6%8E%A7%20Controllability)

[导航](#%E5%AF%BC%E8%88%AA)

[选择合适的导航](#%E9%80%89%E6%8B%A9%E5%90%88%E9%80%82%E7%9A%84%E5%AF%BC%E8%88%AA)

[侧栏导航](#%E4%BE%A7%E6%A0%8F%E5%AF%BC%E8%88%AA)

[顶部导航](#%E9%A1%B6%E9%83%A8%E5%AF%BC%E8%88%AA)

# 设计原则

设计原则

![](https://img-blog.csdnimg.cn/20181205141633861.png)

一致

Consistency

![](https://img-blog.csdnimg.cn/20181205141633880.png)

反馈

Feedback

![](https://img-blog.csdnimg.cn/20181205141633883.png)

效率

Efficiency

![](https://img-blog.csdnimg.cn/20181205141633888.png)

可控

Controllability

### 一致性 Consistency
- **与现实生活一致：**与现实生活的流程、逻辑保持一致，遵循用户习惯的语言和概念；
- **在界面中一致：**所有的元素和结构需保持一致，比如：设计样式、图标和文本、元素的位置等。

### 反馈 Feedback
- **控制反馈：**通过界面样式和交互动效让用户可以清晰的感知自己的操作；
- **页面反馈：**操作后，通过页面元素的变化清晰地展现当前状态。

### 效率 Efficiency
- **简化流程：**设计简洁直观的操作流程；
- **清晰明确：**语言表达清晰且表意明确，让用户快速理解进而作出决策；
- **帮助用户识别：**界面简单直白，让用户快速识别而非回忆，减少用户记忆负担。

### 可控 Controllability
- **用户决策：**根据场景可给予用户操作建议或安全提示，但不能代替用户进行决策；
- **结果可控：**用户可以自由的进行操作，包括撤销、回退和终止当前操作等。

# 导航

导航

导航可以解决用户在访问页面时：在哪里，去哪里，怎样去的问题。一般导航会有「侧栏导航」和「顶部导航」2 种类型。

### 选择合适的导航

选择合适的导航可以让用户在产品的使用过程中非常流畅，相反若是不合适就会引起用户操作不适（方向不明确），以下是「侧栏导航」和 「顶部导航」的区别。

### 侧栏导航

可将导航栏固定在左侧，提高导航可见性，方便页面之间切换；顶部可放置常用工具，如搜索条、帮助按钮、通知按钮等。适用于中后台的管理型、工具型网站。

![](http://element-cn.eleme.io/static/navbar_1.499e088.png)

一级类目

适用于结构简单的网站：只有一级页面时，不需要使用面包屑。

![](http://element-cn.eleme.io/static/navbar_2.59ecae2.png)

二级类目

侧栏中最多可显示两级导航；当使用二级导航时，我们建议搭配使用面包屑，方便用户定位自己的位置和快速返回。

![](http://element-cn.eleme.io/static/navbar_3.8dcec92.png)

三级类目

适用于较复杂的工具型后台，左侧栏为一级导航，中间栏可显示其对应的二级导航，也可放置其他的工具型选项。

### 顶部导航

顺应了从上至下的正常浏览顺序，方便浏览信息；顶部宽度限制了导航的数量和文本长度。

![](http://element-cn.eleme.io/static/navbar_0.b608f86.png)

适用于导航较少，页面篇幅较长的网站。





