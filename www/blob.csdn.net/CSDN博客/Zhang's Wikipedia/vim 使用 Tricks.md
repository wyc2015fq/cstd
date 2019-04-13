
# vim 使用 Tricks - Zhang's Wikipedia - CSDN博客


2018年05月13日 19:16:40[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：119个人分类：[vim](https://blog.csdn.net/lanchunhui/article/category/6251546)



vim + /etc/fstab：打开文档时直接光标直接置于最后一行首部；
## 1. 最小影响的修改：非 insert mode 修改
del/x：删除光标所在字符；
r：replace，替换光标所在的字符；
xp：交换光标处字符及光标后的下一个字符的相对顺序；
cw：修改光标所对应的整个单词；
c：change，w：word
2. 搜索
全字匹配：
/int⇒ print
/int\>⇒ 以 int 为结尾
/\<int\>⇒ 全字匹配；


