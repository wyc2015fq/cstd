# CSRF攻击即防护 - LC900730的博客 - CSDN博客
2017年05月25日 22:38:43[lc900730](https://me.csdn.net/LC900730)阅读数：287
## CSRF
跨站请求伪造 
Laravel自动为每个用户session生成了一个CSRF Token，该Token可以验证登录用户和发起请求者是否是同一个人 
laravel提供了一个全局函数csrf_token来获取该Token值，因此只需要在提交表单中添加按钮如下HTML中即可在请求中带上Token：
`<input type='hidden' name="_token" value="<?php echo csrf_token();?>">`
该段代码还可以等同于全局帮助函数csrf_field的输出：
```xml
<?php echo csrf_field(); ?>
```
在Blade模板引擎下还可以使用如下方式： 
{!!!csrf_field()!!}
### 从CSRF验证中排除某些指定URL
并不是所有请求都需要避免CSRF攻击 
可以通过VerifyCsrfToken(app/Http/Middleware/VerifyCsrfToken.php)中间件中将要排除的请求URL添加到$excepts属性数组中：
```xml
<?php
    <?php
    namespace App\Http\Middleware;
    use Illuminate\Foundation\Http\Middleware\VerifyCsrfToken as BaseVerifier;
    class VerifyCsrfToken extends BaseVerifier
    {
        /**
         * 指定从 CSRF 验证中排除的URL
         *
         * @var array
         */
        protected $except = [
            'testCsrf'
        ];
    }
?>
```
那么[http://laravel.app:8000/testCsrf](http://laravel.app:8000/testCsrf)就不会有验证csrf
### X-CSRF-Token及其应用
如果使用Ajax提交Post表单，我们可以使用Token设置在meta中：
```php
<meta name="csrf-token" content="{{csrf_token()}}">
```
然后在全局Ajax中使用这种方式设置X-CSRF-Token请求头并提交
```
$.ajaxSetup({
    headers:{
        'X-CSRF-TOKEN':$('meta[name="csrf-token"]').attr('content')
    }
})
```
## CSRF验证原理分析
1.首先Laravel开启session时会生成一个token值并存放在Session中(illuminate\Session\Store.php第90行start方法)
```
public function start()
{
    $this->loadSession();
    if (! $this->has('_token')) {
        $this->regenerateToken();
    }
    return $this->started = true;
}
```
2.然后重点分析VerifyToken中间件的handle方法，该方法中先通过isReading方法来判断请求方式，如果请求方式是HEAD、GET、OPTIONS其中一种，则不做CSRF验证 
3.通过shouldPassThrough方法判断路由请求是否在$except属性数组中进行了排除，如果排除了也不做验证； 
4.最后通过tokenMatch方法判断请求参数中的CSRF TOKEN值和Session中的Token值是否相等，相等则通过验证，否则抛出TokenMismatchException异常
```
public function handle($request, Closure $next)
{
    if ($this->isReading($request) || $this->shouldPassThrough($request) || $this->tokensMatch($request)) {
        return $this->addCookieToResponse($request, $next($request));
    }
    throw new TokenMismatchException;
}
```
注：tokensMatch方法首先从Request中获取_token参数值，如果请求中不包含该参数则获取X-CSRF-TOKEN请求头的值，如果该请求头也不存在则获取X-XSRF-TOKEN请求头的值，需要注意的是X-XSRF-TOKEN请求头的值需要调用Encrypter的decrypt方法进行解密。
