# recycleView中item里对象引用错误导致的bug - weixin_33985507的博客 - CSDN博客
2018年06月05日 22:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
今天写代码时，出现了一个让我很郁闷的bug，接下来就简单说下，
1，bug产生的现象
2，一开始进入的误区
3，debug过程
4，bug产生的原因
5，解决方案
6，教训与总结
bug产生的现象:
在recycleView的item里，有一个按钮，点击时会弹窗一个选择PopupWindow对话框，效果如下：

![4334234-0c24e73a1d9f8e3b.gif](https://upload-images.jianshu.io/upload_images/4334234-0c24e73a1d9f8e3b.gif)
效果图
但是，当时那个PopupWindow的位置却很飘忽，一会显示在正确的位置，一会在左上角，一会在左下角。我当时很武断的认为是recycleview回收机制的锅
