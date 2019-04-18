# fancybox2设置弹出层透明背景 - weixin_33985507的博客 - CSDN博客
2016年10月10日 22:23:20[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
fancybox2之后，在设置中没有了opacity的设置属性，需要对css进行自定义才可以将背景透明变成没有颜色的状态
```
$(selector).fancybox({
    helpers: {
        overlay: {
            css: {
                'background': 'url("")'
            }
        }
    }
});
```
