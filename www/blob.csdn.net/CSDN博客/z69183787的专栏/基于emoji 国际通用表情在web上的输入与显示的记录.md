# 基于emoji 国际通用表情在web上的输入与显示的记录 - z69183787的专栏 - CSDN博客
2015年09月15日 12:09:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：26774
定义：
　　emoji 即国际通用表情
场景：
1，ios，android，wp上emoji表情输入与显示
2，web也需作为支撑平台对emoji表情就行输入与显示（解析）
问题：
1，app端输入的表情在web端显示的是乱码（比如方括号，问号，和实际编码有关）
2，mysql 存储emoji报异常（0xF0 0x9F 0x8F 0x8），此记录不做问题处理，请参考http://blog.csdn.net/qdkfriend/article/details/7576524
解决：暂且对web显示（问题1）的处理
所需js库，按顺序依赖
emoji-list-with-image.js 存储emoji编码和表情图片数据，格式如["e415","1f604","iVBORw0KGg..."]
punycode.js 编码解析库
emoji.js  解析emoji编码 
示例用法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 //解析存储的emoji表情
function parse(arg) {
    if (typeof ioNull !='undefined') {
        return  ioNull.emoji.parse(arg);    
    }
    return '';
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 //反解析（web上，图片数据转为emoji字符编码存储）
function decode(htmlStr) {
    //todo 正则替换性能更优？unicode16="1f603"
    if (typeof ioNull == 'undefined') {
        return '';
    }
    var tempStr = htmlStr, unis = '', $imgs = $('<div>').append(htmlStr).find('img');
    $.each($imgs , function (i, o) { 
        var $img = $(o);
        var unicode16 = '0x' + $img.attr('unicode16'); //十六进制
        unicode16 = ioNull.emoji.decodeChar(unicode16);
        //unis += unicode16;
        tempStr = tempStr.replace($('<div>').append($img).html(), unicode16);
    });
    //System.log(unis)
    return tempStr;
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
emoji.js 提供了getEmojiList 方法，获取所有类别的emoji数组集合，其中单项格式为[0: "e415",1: "1f604",2: "emoji对应的base64图片数据"]
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//示例生成emoji图片输入 
function renderEmoji()
{
var emos = getEmojiList()[0];//此处按需是否生成所有emoji
            var html = '<div >常用表情</div><ul>';
            for (var j = 0; j < emos.length; j++) {
                var emo = emos[j];
                var data = 'data:image/png;base64,' + emo[2];
                if (j % 20 == 0) {
                    html += '<li class="">';
                } else {
                    html += '<li>';
                }
                html += '<img style="display: inline;vertical-align: middle;" src="' + data + '"  unicode16="' + emo[1] + '" /></li>';
            }
//.......
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
表情输入示例图
![](http://images.cnitblog.com/blog/347242/201409/041754182978440.jpg)
至此主要emoji解析与反解析已得到处理
 附上js库压缩包下载http://files.cnblogs.com/jtans/emoji-lib.zip，
参考网址 http://www.iemoji.com/
