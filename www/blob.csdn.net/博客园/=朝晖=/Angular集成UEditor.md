# Angular集成UEditor - =朝晖= - 博客园
# [Angular集成UEditor](https://www.cnblogs.com/dhcn/p/7778307.html)
    1、Ueditor的集成主要通过把UEditor做成一个Component来实现，先上Component代码：      
```
import {
  AfterContentInit, Component, Input, OnDestroy, OnInit
} from '@angular/core';
import {DomSanitizer, SafeHtml} from '@angular/platform-browser';
@Component({
  selector: 'app-ueditor',
  template: '<div [innerHTML]="trustedHtml"></div>'
})
export class UeditorComponent implements OnInit, OnDestroy, AfterViewInit {
  ngOnDestroy(): void {
    this.ueditor.destroy();
    this.ueditor = null;
  }
  @Input() content: string;
  ueditor: any;
  trustedHtml: SafeHtml;
  constructor(private sanitizer: DomSanitizer) {
    // javascript: URLs are dangerous if attacker controlled.
    // Angular sanitizes them in data binding, but you can
    // explicitly tell Angular to trust this value:
  }
  ngOnInit(): void {
    const html = '<script id="textdescription" name="content"  style="display: inline-block;" type="text/plain">' + this.content + '</script>';
    this.trustedHtml = this.sanitizer.bypassSecurityTrustHtml(html);
  }
  ngAfterViewInit(): void {
    this.ueditor = UE.getEditor('textdescription', {'initialFrameHeight': 580});
    //console.log(this.ueditor);
  }
}
```
   简单解释一下，这个代码干了啥，用DomSanitizer这个组件把本来模板中不合法的Script标签合法化，而且只能通过属性绑定的赋值，才能让模板把它渲染出来。Ng的模板自带XSS过滤，像Script标签会被直接省略掉，导致的结果是UE找不到holder的位置，执行出错。
   2、上面这个代码里面的UE是一个全局库，有个比较直接懒的只是让其可见的声明方式是如下，细致的接口声明，同志们自己搞吧：
```
declare var UE: any;
```
      3、把Ueditor的那两个js文件ueditor.config.js、ueditor.all.js加进angular-cli的scripts配置项。
      4、要把Ueditor用到的静态资源扔进assets
      5、ueditor.config.js中的UEDITOR_HOME_URL改成静态文件URL父目录，serverUrl改成后端服务器URL。
      最后补一句后端修改点，由于SPA往往跨域部署，后端正常的CORS配置以外，Ueditor会自动把某些调用(config)改成jsonp调用，后端需要根据callback参数做对应额jsonp方式返回响应。最后做个广告：如果你用Django，推荐DUEditor插件：https://github.com/dhcn/DUEditor

