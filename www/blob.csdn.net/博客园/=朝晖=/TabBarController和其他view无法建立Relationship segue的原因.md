# TabBarController和其他view无法建立Relationship segue的原因 - =朝晖= - 博客园
# [TabBarController和其他view无法建立Relationship segue的原因](https://www.cnblogs.com/dhcn/p/7130560.html)
拖拽怎么也没有那个出现，最后看sourcecode发现是那个那个viewcrontroler的XML 元素不是TabBarController.在Sourcecode里面改了一下，解决了这个问题。
总结：[iOS](http://lib.csdn.net/base/ios)这玩意的界面一定要尽量从拖拽里面出，自己手改并不一定合乎底层约束要求。

