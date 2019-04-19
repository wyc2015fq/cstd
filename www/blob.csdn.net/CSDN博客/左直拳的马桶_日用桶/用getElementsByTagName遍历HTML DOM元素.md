# 用getElementsByTagName遍历HTML DOM元素 - 左直拳的马桶_日用桶 - CSDN博客
2011年03月02日 15:24:00[左直拳](https://me.csdn.net/leftfist)阅读数：6545
var eles = document.getElementsByTagName("div");
    for (var i = 0; i < eles.length; i++) {
        if (eles[i].id.indexOf("ddd_") != -1) {
            alert("找到div：" + eles[i].id);
        }
    } 
