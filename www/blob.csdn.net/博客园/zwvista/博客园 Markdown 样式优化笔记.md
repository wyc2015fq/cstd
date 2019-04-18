# 博客园 Markdown 样式优化笔记 - zwvista - 博客园

## [博客园 Markdown 样式优化笔记](https://www.cnblogs.com/zwvista/p/9404089.html)

### 配置博客园

进入博客园后台管理界面，按“设置”页。
- 在“页面定制CSS代码”定制 CSS。
- 在“页脚Html代码”添加相应的JavaScript标签与代码。

[优化博客园Markdown样式（代码行号、版权信息、章节目录等）](https://www.cnblogs.com/gzdaijie/p/5187171.html)
[如何自定义博客园代码高亮主题，同时分享自己使用的黑色主题](https://www.cnblogs.com/liuxianan/p/custom-cnblogs-highlight-theme.html)

### 怎样为 Kotlin 这样的非“常用语言”添加语法高亮？

博客园使用 hightlightjs 插件渲染代码着色高亮。博客页面的源码中可以看到对 hightlightjs 的引用。

hightlightjs 着色的原理是将`<pre><code>`标签范围内代码中的关键字，数字，字符串等元素套上`<span>`标签并加上专用的 class 。

hightlightjs 当前支持 176 种语言，但是非“常用语言”需要引用相应的语言包。

这里非“常用语言”是指 hightlightjs 官网下载页中 Common 类 23 种常用语言之外的语言。

使用方法：[How to use highlight.js](https://highlightjs.org/usage/)

下载页：[Getting highlight.js](https://highlightjs.org/download/)

语法高亮演示：[highlight.js demo](https://highlightjs.org/static/demo/)

Kotlin 以及其他语言包：[highlight.js - cdnjs.com](https://cdnjs.com/libraries/highlight.js/)

引用 Kotlin 语言包的方法：
`<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/languages/kotlin.min.js"></script>`
※在“页脚Html代码”添加以上代码

博客页面加载后虽然加上了语法高亮，但由于不是常用语言缺乏语言包，Kotlin 语言代码被误认为 Nginx 脚本，代码已经渲染完毕被加上了`<span>`标签，因此要再次渲染的话首先需要将代码还原。

以下 JavaScript 代码清除 pre 标签中除 code 以外的标签，在引用了相应语言包的前提下让 hightlightjs 重新渲染。

```
<script type='text/javascript'>
    var setMyBlog = {
        resetPreCode: function(){
            var pre = $("pre"); //选中需要更改的部分
            if(pre && pre.length){
                pre.each(function() {
                    var item = $(this);
                    // 将<pre>标签中的html标签去掉 然后再套上原来的code标签
                    // 这是因为我们需要重新加上语法高亮，而 hightlightjs 渲染的范围是<pre><code>中的代码
                    item.html("<code>" + item.html().replace(/<[^>]+>/g,"") + "</code>"); 
                })
            }
        },
        runAll: function() {
            /* 运行所有方法
             * resetPreCode() 清除代码中的标签
             */ 
            this.resetPreCode();
            hljs.initHighlightingOnLoad(); // 重新渲染，添加语法高亮
        }
    }
    setMyBlog.runAll();
</script>
```

※在“页脚Html代码”添加以上代码

看看效果。以下代码采用 Markdown 标记````kotlin`来标识 Kotlin 语言代码。

```
fun <T, R> Collection<T>.fold(
    initial: R, 
    combine: (acc: R, nextElement: T) -> R
): R {
    var accumulator: R = initial
    for (element: T in this) {
        accumulator = combine(accumulator, element)
    }
    return accumulator
}
```

### 怎样为代码加上行号?

答案是使用 [highlightjs-line-numbers.js](https://github.com/wcoder/highlightjs-line-numbers.js/)

该插件将 hightlightjs 插件已经渲染完毕的代码插入到表格中，然后加上行号。
`<script src="https://cdnjs.cloudflare.com/ajax/libs/highlightjs-line-numbers.js/2.3.0/highlightjs-line-numbers.min.js"></script>`
※在“页脚Html代码”添加以上代码

```
// ...
        runAll: function() {
            this.resetPreCode();
            hljs.initHighlightingOnLoad(); // 重新渲染，添加语法高亮
            hljs.initLineNumbersOnLoad(); // 为代码加上行号
        }
// ...
```

※在“页脚Html代码”添加以上代码


