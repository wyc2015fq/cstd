# Web 前端优化最佳实践之 CSS 篇

 

Web 前端优化最佳实践第四部分面向 CSS。目前共计有 6 条实践规则。另请参见 Mozilla 开发者中心的文章：[Writing Efficient CSS](http://developer.mozilla.org/en/docs/Writing_Efficient_CSS)

#### 1. 把 CSS 放到代码页上端 (Put Stylesheets at the Top)

官方的解释我觉得多少有点语焉不详。这一条其实和**用户访问期望**有关。CSS 放到最顶部，浏览器能够有针对性的对 HTML 页面从顶到下进行解析和渲染。没有人喜欢等待，而浏览器已经考虑到了这一点。

#### 2. 避免 CSS 表达式 (Avoid CSS Expressions)

个人认为通过 CSS 表达式能做到的事情，通过其它手段也同样能做到而且风险更小一些。

#### 3. 从页面中剥离 JavaScript 与 CSS (Make JavaScript and CSS External)

剥离后，能够有针对性的对其进行单独的处理策略，比如压缩或者缓存策略。

#### 4. 精简 JavaScript 与 CSS (Minify JavaScript and CSS)

如果没有 JavaScript 与 CSS 可能更好。但，这是不可能的，SO，尽量小点吧。语法能简写的简写。

#### 5. 使用 <link> 而不是@importChoose <link> over @import

在 IE 中 @import 指令等同于把 link 标记写在 HTML 的底部。而这与第一条相违背。

#### 6. 避免使用Filter (Avoid Filters)