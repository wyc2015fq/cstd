# TortoiseSVN 锁的使用 - z69183787的专栏 - CSDN博客
2014年01月13日 20:17:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6249
刚才试验了一下，终于搞明白了TortoiseSVN里锁lock 的使用方法。
简单的说，如果压根没有锁lock，那么每个人都拥有一个本地copy，每个人都能自由地对本地copy编辑edit并提交commit，这样可能会导致冲突conflict，最后不得不合并Merge。这就是传统的 “拷贝-修改-合并”模型。
如果使用锁，每个用户编辑前先get-lock, 然后编辑，提交。这个过程中，其他人打开的时候开到的是read-only的文件，无法保持，这样就保证了单一性。这就是“锁定-编辑-解锁”模型，对不容易合并的非二进制文件很有好处。
更进一步，TortoiseSVN有个 needs-lock属性，设置这一属性后，文件将默认成为只读文件，要编辑，必须get-lock, 之后，本地copy变成了可写，编辑完提交后再次成为只读。 这过程中，其他人的副本还是只读，他也想编辑，也必须get-lock， 但是此时已经被lock了，他就知道已经有人编辑了，就不会再有编辑冲突了。
needs-lock 有专门的图标，所以，needs-lock 可以看成是一种提醒，提醒你编辑之前必须lock。如果没有该属性，则可能会忘记加lock，导致提交冲突。
还有几个问题
1： 如何设置 get-lock, unlock, needs-lock
get-lock, unlock, 右键菜单即可，get-lock 还可添加注释
needs-lock 可以由右键 TortoiseSVN -> Properties -> New -> needs-lock 即可。
另外，可以设置全局的新添加文件自动加上needs-lock属性，或者文件夹下所有文件
2： 如何查看lock 状态
TortoiseSVN -> Check for modifications, 默认显示本地修改文件，要查看所有的修改文件，再点
Check Repository 这样显示详细的修改信息，包括owner，lock info等。
3：如何偷锁
查看完lock，选择文件，右键可以 break lock, 另外还可以 steal lock. 不过除非lock的人联系不上，不要随意break 或者steal，不然文件冲突事小，破坏团结，丢掉工作事大，简单的交流更有效。
详情参考官方教程：
[http://tortoisesvn.net/docs/release/TortoiseSVN_zh_CN/tsvn-dug-locking.html](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Ftortoisesvn.net%2Fdocs%2Frelease%2FTortoiseSVN_zh_CN%2Ftsvn-dug-locking.html) 
 lock 页面
[http://tortoisesvn.net/docs/release/TortoiseSVN_zh_CN/tsvn-dug-propertypage.html#tsvn-dug-propertypage-auto-props](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Ftortoisesvn.net%2Fdocs%2Frelease%2FTortoiseSVN_zh_CN%2Ftsvn-dug-propertypage.html%23tsvn-dug-propertypage-auto-props) 
 自动设置页面
[http://tortoisesvn.net/docs/release/TortoiseSVN_zh_CN/index.html](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Ftortoisesvn.net%2Fdocs%2Frelease%2FTortoiseSVN_zh_CN%2Findex.html) 
 1.7 中文教程首页
