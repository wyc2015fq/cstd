# Angular4问题记录 - weixin_33985507的博客 - CSDN博客
2018年05月01日 14:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
**1.No template specified for component 'RightsComponent'**
组件没有被ngmodule引用,一般是路由或者配置出现了问题,检查即可
**2.Cannot find module '@angular-devkit/core'**
更改package.json文件中devDependencies的@angular/cli版本,然后执行npm i 即可
**3.使用ngx-weui 的weui-infiniteloader组件时,导致(scroll)无效**
weui-infiniteloader是ngx-weui的上拉加载组件,使用该组件时,无法再外层节点或者该节点使用(scroll)监听滑动距离,暂时不清楚原因.
解决办法:使用jq的监听方法,监听.weui-infiniteloader__content样式即可
```
$(".weui-infiniteloader__content").scroll((e) =>{
    // ....
});
```
**4.Safari和其它老式浏览器不支持Date和Currency管道需要ECMAScript国际化（I18n）API**
该问题可以用垫片（Polyfill）解决。
```
<script src="https://cdn.polyfill.io/v2/polyfill.min.js?features=Intl.~locale.en"></script>
```
**5 文件上传字节流转换后打包过程报类型不符合**
```
let _file: File = event.target.files[0];
    let reader = new FileReader();
    reader.readAsDataURL(_file);
    reader.onload = (e)=>{
      let obj : any = e.target.result;   
      $('#triggerImg').attr("src", obj);
    };
    question:Property 'result' does not exist on type 'EventTarget'.
因为readAsDataURL会将图片转换成base64码,再通过FileReader对象的onload方法返回回来
但是TyrScript编译器不知道你正在返回一个什么类型的东西,
e.target默认为EventTarget类型,但是result为string类型，因此会报错。
解决办法：  let obj : any = (e.target as any).result;
```
**6 H5图片上传转base64显示于页面上出现sanitizing unsafe URL value data **
应该是angular4自带的检查机制引起的,解决办法
```
import { DomSanitizer } from '@angular/platform-browser';
let base64_Url = (e.target as any).result;
this.sanitizer.bypassSecurityTrustUrl(base64_Url);
```
