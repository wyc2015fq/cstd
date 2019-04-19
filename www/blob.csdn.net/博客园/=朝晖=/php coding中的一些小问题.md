# php coding中的一些小问题 - =朝晖= - 博客园
# [php coding中的一些小问题](https://www.cnblogs.com/dhcn/p/7105395.html)
最近在SAE上写微博应用，碰到一些小问题，记下来，以供参考：
   1、出错提示：
```
Fatal error: Can't use function return value in write context
```
  问题原因很简单，$_GET、$_POST之类的预定义变量是数组不是函数，如果你用$_GET("ParamName")这样的方法来获取参数就会碰到这个错误，正确的做法是用方括号：$_GET['ParamName'].
    2、Ajax方式与[PHP](http://lib.csdn.net/base/php)程序之间传递中文的方法：
        2.1、Ajax提交中文数据给[php](http://lib.csdn.net/base/php)程序，我的网页采用UTF-8编码，请求提交前我用[jQuery](http://lib.csdn.net/base/jquery)的序列化函数对请求参数做了处理(以前做这方面处理都是对请求参数字符串做两次encodeuri处理，[jquery](http://lib.csdn.net/base/jquery)的这个函数大大简单了这个工作)：
```
var paramData=$('#formID').serialize();
```
  请求提交后用urldecode函数做一次处理就可以得到正确的UTF-8中文编码：
```
$paramValue=urldecode($_GET["paramName"]);
```
  如果网页是GBK编码的话，好像要用iconv函数再做一步处理。
         2.2、PHP程序给Ajax脚本返回中文就更简单了，加个设定编码方式的header头即可：由于我用的是UTF-8编码，所以加下面这条：
```
header('Content-type: text/html;charset=UTF-8');
```

