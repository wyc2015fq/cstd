# FileInputStream读取文本注意事项 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月22日 15:01:45[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：947


使用 read(byte[]) 方法读取文本的时候，要用 String str = new String(byte[],int offset,int len) 来将数组中的元素转换为String字符串然后打印。 否则会出现 在 文本的最后多出 几个英文字母 的情况

![](https://img-blog.csdn.net/20180122145156472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180122145213239?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180122145224398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解释：

第二张 图  控制台的结果 末尾 多出了 wo 两个 字母， 原因是 byte 输出 倒数第二次 存入的 数据 为 [l,o, ,w,o]，最后一次读取文本的时候 只剩下了 r，l和 d 三个 字母， 所以数组的0,1和2索引位 的 l，o，空格，被替换了 即变成了[r,l,d,w,o] 所以 最后 多出了w，o 两个字母， 为了解决这种 情况， 我们需要用 String str = new String(byte[],int
 offset,int len) 来 控制 读取数组的位数。  offset 是 从 指定的 索引位 开始 读取， len 是 读取的位数， 在这个例子中， 最后一次 读取， 读了 3个 字母，所以 read（byte b） 返回的值 是 3， 然后存入len 中， 所以 最后一次的str 只 读取了 byte 数组中的 前三个 元素。从而避免了 多余数据的产生。 

