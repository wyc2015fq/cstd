# js模板引擎artTemplate快速上手 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月17日 20:21:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：531
腾讯的artTemplate
1，**编写模板**
**(采用script标签并带有属性id和type="text/html")**
<script id="test"
 type="text/html"> {{if isAdmin}} <h1>{{author}}</h1> <ul> {{each list as value i}} <li>{{i+1}}:{{value}}</li> {{/each}} </ul> {{/if}} </script>
2，** 渲染模板**
var data = { author: '宫崎骏', isAdmin: true, list: ['千与千寻', '哈尔的移动城堡', '幽灵公主', '风之谷', '龙猫'] };var html =
template('test', data); document.getElementById('content').innerHTML = html;
**模板语法**
有两个版本的模板语法可以选择。
      1.**简洁语法，采用"{{ }}"**
**原生语法(采用"<%= %>")**
使用原生语法，需要导入template-native.js文件。

[**template.js**](https://raw.githubusercontent.com/aui/artTemplate/master/dist/template.js)*(简洁语法版, 2.7kb)*
[***template-native.js***](https://raw.githubusercontent.com/aui/artTemplate/master/dist/template-native.js)* (**原生语法版**, 2.3kb)*
**方法**
**template(id, data)**
 根据 id 渲染模板。内部会根据document.getElementById(id)查找模板，如果没有
 data 参数，那么将返回一渲染函数。

[https://www.cnblogs.com/Leo_wl/p/5562012.html](https://www.cnblogs.com/Leo_wl/p/5562012.html)
[http://www.jianshu.com/p/483fa7f6f55b](http://www.jianshu.com/p/483fa7f6f55b)
[https://github.com/aui/art-template](https://github.com/aui/art-template)
