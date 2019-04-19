# jieba 结巴，入门使用_用户5706034127_新浪博客
||分类：[自然语言处理](http://blog.sina.com.cn/s/articlelist_5706034127_3_1.html)|
在使用jieba.analyse之前一定要先加载analyse。
analyse加载和jieba加载不一样，
import
jieba
import jieba.analyse
之后可以看看，源代码。
之后总结下，python文件夹组织的模块，之间的关系
## 延迟加载机制
jieba 采用延迟加载，
```
import
jieba
```
和`jieba.Tokenizer()`不会立即触发词典的加载，一旦有必要才开始加载词典构建前缀字典。如果你想手工初始
jieba，也可以手动初始化。
```
import jieba
jieba.initialize()  # 手动初始化（可选）
```
在 0.28 之前的版本是不能指定主词典的路径的，有了延迟加载机制后，你可以改变主词典的路径:
`jieba.set_dictionary('data/dict.txt.big')`
所以你可以在真正执行分析前，，添加词典或者修改词典路径。
内部使用函数式编程的方式，实现延迟加载。
