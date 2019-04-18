# 字体大小变动js控制 - weixin_33985507的博客 - CSDN博客
2016年09月21日 10:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
```
window.onload = function() {
            fontSize();
                function fontSize () {
                    var w = window.innerWidth / 10;
                    var html = document.querySelector("html");
                    html.style.fontSize = w + "px";
                }
                window.onresize = fontSize;
            }
        </script>```
```
