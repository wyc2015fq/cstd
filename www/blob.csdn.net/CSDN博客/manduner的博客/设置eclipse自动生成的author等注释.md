
# 设置eclipse自动生成的author等注释 - manduner的博客 - CSDN博客


2018年05月30日 22:17:04[Manduner_TJU](https://me.csdn.net/manduner)阅读数：82


转自：https://blog.csdn.net/fenglibing/article/details/4099454

# 1，转载内容
每新建一个类上面都会有一行注释：
/**
* @author 马海宝 E-mail:rigger21@126.com
* @version 创建时间：2007-8-30 下午04:58:52
* 类说明
*/
感觉这个应该很好，多个人一起开发的时候能够看见谁是谁的， 我就想这个怎么弄的，上网一找还真有，但是自己配置就是错误的， 找了半天都是这样的，我晕了。 怎么回事情，难道真是大家说的人品的事情吗？ 我就不信这个邪了。我必须弄出来，我上网大量的搜索，但是还是那样，只能自己改了。试了好半天，最终上天终于不负我这有心人，我终于修改成功了，操作步骤和大家分享一下：
window－>preference－>java－>code     styple－>code     template 当你选择到这部的时候就会看见右侧有一个框显示出code这个选项，你点开这个选项，点一下他下面的New Java    files 然后你点edit按钮，把他的内容换成你的就可以了：（例如）
${filecomment}
${package_declaration}
/**
* @author 陈红华 E-mail:cenghonho@126.com
* @version 创建时间：${date} ${time}
* 类说明
*/
${typecomment}
${type_declaration}
最后点apply ----> OK 成功。。

# 2，实践后总结的注意点
在code Tempaltes界面下方有个选择框“Automatically add comments for new methods and types”,不要勾选这个；实践证明，如果勾选这个选项，在你创建新的java类时，会出现多余的注释。
![](https://img-blog.csdn.net/20180530221646575)



