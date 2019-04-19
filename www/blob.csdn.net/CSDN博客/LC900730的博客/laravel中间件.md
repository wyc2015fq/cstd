# laravel中间件 - LC900730的博客 - CSDN博客
2017年05月25日 23:11:14[lc900730](https://me.csdn.net/LC900730)阅读数：322标签：[中间件																[laravel](https://so.csdn.net/so/search/s.do?q=laravel&t=blog)](https://so.csdn.net/so/search/s.do?q=中间件&t=blog)
个人分类：[laravel](https://blog.csdn.net/LC900730/article/category/6939506)
## 中间件
Laravel可以把HTTP中间件看做是“装饰器”，在最终请求到来之前对请求进行过滤和处理。在Laravel中有着广泛应用，如用户认证、日志、维护模式、开始Session、从Session中获取错误信息，以及Csrf攻击。中间件默认放在app/Http/Middleware目录下。
## 中间件创建及其使用
自定义中间件类，只需要定义一个handler方法即可，然后我们将主要业务逻辑放在该方法中，如果我们在请求处理前执行业务逻辑，那么$next闭包执行前执行业务逻辑操作：
```xml
<?php
namespace App\Http\Middleware;
use Closure;
class BeforeMiddleware{
    public function handler($request,Closure $next){
        //执行业务逻辑操作
        return $next($request);
    }
}
```
如果在请求处理后在执行中间件业务逻辑，则在$next闭包执行后执行操作：
```php
<?php
namespace App\Http\Middleware;
use Closure;
class AfterMiddleware{
    public function handler($request,Closure $next){
        $response=$next($request);
        //执行动作
        return $next($request);
    }
}
?>
```
我们处理的大部分操作都是第一种，即在请求处理前执行操作，比如用户认证、CSRF验证、维护模式都是这样；
