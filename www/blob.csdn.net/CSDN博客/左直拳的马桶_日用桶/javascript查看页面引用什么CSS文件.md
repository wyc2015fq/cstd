# javascript查看页面引用什么CSS文件 - 左直拳的马桶_日用桶 - CSDN博客
2011年03月04日 18:06:00[左直拳](https://me.csdn.net/leftfist)阅读数：1497标签：[javascript																[css																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
        var cssStyle = document.styleSheets;
        for (var i = 0; i < cssStyle.length; i++) {
            if (cssStyle[i].href != null) {
                var csshref = cssStyle[i].href;
                if (csshref.indexOf("blue") > 0) {
                    tmpcssColor = "#F5FAFF";
                    break;
                }
                else if (csshref.indexOf("default") > 0) {
                    tmpcssColor = "#FFF8E8";
                    break;
                }
                else if (csshref.indexOf("green") > 0) {
                    tmpcssColor = "#F4FAF0";
                    break;
                }
            }
        }
