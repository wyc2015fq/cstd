# 做 CSS 实现宽度和浏览器窗口宽度一样的正方形 - weixin_33985507的博客 - CSDN博客
2018年06月08日 18:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：38
方法1: 用 `rem`
配合动态设置 html 根节点的 font-size 的 JS，使 浏览器窗口宽度 的值为 `6.4rem;`。
```
.tar {
    width:6.4rem;
    height:6.4rem;
}
```
方法2: 用 `vw`
```
.tar {
    width: 100vw;
    height: 100vw;
}
```
方法3: 用 `padding-top: 100%`
```
.tar {
    width: 100vw;
    padding-top: 100%;
}
```
`padding-top` 设置百分数是基于元素的宽度的。
