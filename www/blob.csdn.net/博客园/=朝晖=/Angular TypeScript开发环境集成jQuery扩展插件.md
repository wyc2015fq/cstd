# Angular TypeScript开发环境集成jQuery扩展插件 - =朝晖= - 博客园
# [Angular TypeScript开发环境集成jQuery扩展插件](https://www.cnblogs.com/dhcn/p/7773699.html)
      集成步骤：
     1、安装jquery极其扩展插件库ts定义文件
```
npm install jquery --save
npm install --save-dev @types/jquery
npm install datatables.net --save
npm install @types/datatables.net --save-dev
```
     2、页面加载jquery和扩展插件：
```
"apps": [{
  ...
  "scripts": [
    "../node_modules/jquery/dist/jquery.min.js",
    "../node_modules/datatables.net/js/jquery.dataTables.js",
  ],
  ...
}]
```
   3、把扩展插件module加入到tsconfig.app.json的types配置。
```
"types": [
      "echarts",
      "datatables.net",
      "bootstrap",
      "admin-lte"
    ]
```
   4、调用扩展插件的方法：
```
$("selector").pluginMethod();
```
   5、运行ng serve：
```
ng serve -o
```
     这个里面有个问题，就是我们不是直接去尝试调用jquery plugin，jquery接口的ts定义扩展是通过plugin的d.ts文件去扩展的，我们调用jquery本身，在编译时，编译器自动去寻找相关的plugin定义并扩展原始jquery的接口。搞清楚这一点，很多基于jQuery选择器函数的JS库集成就方便多了。
     最后分析几个错误：
     1、编译的时候出现
```
Property 'DataTable' does not exist on type 'JQuery<HTMLElement>'.
```
      是因为定义DataTable的datatables.net库没有放进types设置，导致编译时找不到这个方法定义。
   2、Lint的时候出现错误：
```
Error:(5, 22) TS2306:File '/Users/denghui/ng/ord/node_modules/@types/datatables.net/index.d.ts' is not a module.
```
       是因为JQuery和他的插件都是全局库，一是没有必要用import语法导入，二是它的types定义里面也没有module定义，所以import时出现这个错误，解决方法是全局库极其插件无需导入，直接使用jQuery释放的$接口即可。
参考资料：https://stackoverflow.com/questions/43934727/how-to-use-jquery-plugin-with-angular-4

