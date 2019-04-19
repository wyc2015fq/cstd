# Text组件 - LC900730的博客 - CSDN博客
2017年09月15日 18:10:46[lc900730](https://me.csdn.net/LC900730)阅读数：149标签：[qml](https://so.csdn.net/so/search/s.do?q=qml&t=blog)
个人分类：[qml](https://blog.csdn.net/LC900730/article/category/7141083)
Text项目可以显示纯文本
## clip属性
clip属性用于设置文本是否被裁剪。如果该值设置为true，那么当文本与Text项目的边界矩形不符合时候会被裁剪。
## 文本换行
通过设置Text的wrapMode属性可以实现文本的自动换行。只有在明确设置了Text的width属性换行才会起作用。可用的换行模式 
- Text.NoWrap(默认)：不进行换行
- Text.WordWrap：单词边界进行换行
- Text.WordAnywhere:只要到达边界，会在任意点进行换行，甚至在一个单词中间
- Text.Wrap：如果可能，会尽量子啊单词边界进行换行，否则会在任意点换行
## 超链接信号
Text项目提供了一个Text：：onLinkActivated(stringlink)处理器。会在用户点击超文连接时候调用。
```
Item{
    Text{
        text:"欢迎<a href="xxxxxxx">Qt开源社区</a>"
        onLinkActivated:console.log(link+"link activated")
    }
}
```
## 信号处理器
TextInput提供了两个信号处理器:onAccepted()
