# laravel学习 - LC900730的博客 - CSDN博客
2017年05月25日 21:00:54[lc900730](https://me.csdn.net/LC900730)阅读数：200标签：[laravel																[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=laravel&t=blog)
个人分类：[laravel](https://blog.csdn.net/LC900730/article/category/6939506)
## 隐藏index.php
在nginx下的server里面 
location / { 
    try_files $uri  $uri/   /index.php?$query_string 
}
## 1.路由
Laravel中的路由全部定义在/app/Http/routes.php中
```
Route::get('/testPost', function () {
    $csrf_token=csrf_token();
    $form=<<<FORM
    <form action='/hello' method="post">
    <input type="hidden" name="_token" value="{$csrf_token}">
    <input type="submit" value="Test"/>
    </form>
FORM;
    return $form;
});
Route::post('/hello', function () {
    return "hello world";
});
```
- 1.1.match:可以指定匹配多种
```
PHP 
Route::match(['get','post'],'/hello',function(){ 
        return "Hello Laravel!"; 
})
```
PHP 
- 1.2.any:匹配所有
## 2.路由参数
```php
Route::get('/hello/{name}',function($name){
    return "hello {$name}!";
})
```
### 2.1可选参数
```php
Route::get('/hello/{name?}',function($name="Laravel"){
    return "Hello {$name}!";
})
```
### 2.3正则约束
```php
Route::get('/hello/{$name?}',function($name="Laravel"{
    return "Hello {$name}!";
})->where('name','[A-Za-z]+'));
//该条件约束意味着$name只能包含大小写字母
```
//如果我们在全局范围内对参数进行条件约束，可以在RouteServiceProvider的boot方法中定义
```php
public function boot(Router $router)
{
    $router->pattern('name','[A-Za-z]+');
    parent::boot($router);
}
```
服务提供者的boot方法在所有服务提供者的register方法执行完毕后开始执行，也就是说我们可以在boot方法对任意服务容中的对象进行依赖注入。
## 3.路由命名
### 3.1as关键字
我们使用as关键字来为路由命名：
```
Route::get('/hello/laravelacademy',['as'=>'academy',function(){
    return 'Hello LaravelAcademy!';
}]);
//路由命名可以让我们在使用route函数生成指向该路由的URL或者生成跳转到该路由的重定向链接时更加方便：
Route::get('/testNameRoute',function(){
    return route('academy');
})
```
那么我们在[http://laravel.yqg.com/testNameRoute](http://laravel.yqg.com/testNameRoute)时输出： 
[http://laravel.yqg.com/hello/laravelacademy](http://laravel.yqg.com/hello/laravelacademy)
### 3.2:redirect+别名
```
Route::get('/testNameRoute',function(){
    return redirect()->route('academy');
})
```
### 3.3带参数的路由
```php
Route::get('/hello/laravelacademy/{id}',['as'=>'academy',function($id){
    return 'Hello LaravelAcademy'.$id.'!';
})
```
[http://laravel.yqg.com/testNameRoute](http://laravel.yqg.com/testNameRoute)时会跳转到 
[http://laravel.yqg.com/hello/laravelacademy/1](http://laravel.yqg.com/hello/laravelacademy/1)
### 3.4路由分组时路由命名方式
```php
Route::group(['as'=>'admin::'],function(){
    Route::get('dashboard',['as'=>'dashboard',function(){
    }]);
});
```
## 4.路由分组
路由分组就是一组拥有相同属性(中间件、命名空间、子域名、路由前缀等)的路由使用Route门面的group方法聚合起来；
### 4.1中间件
1. 
php artisan make::middleware TestMiddleWare 
这样会在/app/Http/Middleware目录下生成一个TestMiddleware.php 
如限制18岁不能访问
```php
public function handler($request,Closure $next ){
    if($request->input('age')<18)
        return redirect()->route('refuse');
    return $next($request);
}
```
2.然后我们打开/app/Http/Kernel.php文件中，新增TestMiddleware到kernel的$routeMiddleware属性
```
protected $routeMiddleware=[
    'auth'=>\App\Http\Middleware\Authenticate::class,
    'auth.basic' => \Illuminate\Auth\Middleware\AuthenticateWithBasicAuth::class,
    'guest' => \App\Http\Middleware\RedirectIfAuthenticated::class,
    'test' => \App\Http\Middleware\TestMiddleware::class,
]
```
3.route.php
```
Route::group(['middleware'=>'test'],function(){
    Route::get('/write/laravelacademy',function(){
    });
    Route::get('/update/laravelacademy',function(){
    });
});
Route::get('/age/refuse',['as'=>'refuse',function(){
    return "未成年人禁止入内;"
}]);
```
那么我们访问http:://laravel.yqg.com/write/laravelacademy?age=15或者http:://laravel.yqg.com/update/laravelacademy?age=15的时候就会跳转到http:://laravel.yqg.com/age/refuse
## 4.2命名空间
默认情况下，routes.php中的定义的控制器位于App\Http\Controllers命名空间下，所以如果指定命名空间，只需要指定App\Http\Controllers之后部分即可
```php
Route::group(['namespace'=>'laravelAcademy']，                   function(){
    //控制器在"App\Htpp\Controllers\LaravelAcademy”命名空间下
    Route::group(['namespace'=>'DOCS'],function(){
        //控制器在"App\Htpp\Controllers\LaravelAcademy\DOCS”命名空间下
    })  
})
```
## 4.3子域名
子域名通过设置domain关键字来设置
```php
Route::group(['domain'=>'{service}.laravel.app'],function(){
  Route::get('/write/laravelacademy',function($service){
        return "Write FROM {$service}.laravel.app";
});
    Route::get('/update/laravelacademy',function($service){
        return "Update FROM {$service}.laravel.app";
    });
});
```
访问[http://write.laravel.app/write/laravelacademy](http://write.laravel.app/write/laravelacademy)则输出 
write FROM write.laravel.app 
访问[http://write.laravel.app/update/laravelacademy](http://write.laravel.app/update/laravelacademy)则输出 
write FROM update.laravel.app
## 4.3路由前缀
如果路由群组中的所有路由包统一前缀，则我们可以通过group方法中 设置prefix属性指定前缀：
```
Route::group(['prefix'=>'laravelacademy'],function(){
    Route::get('write',function(){
        return "Write LaravelAcademy";
    });
    Route::get('update',function(){
        return "Update LaravelAcademy";
    });
});
```
我们就可以通过[http://laravel.app:8000/laravelacademy/write](http://laravel.app:8000/laravelacademy/write)或者[http://laravel.app:8000/laravelacademy/update](http://laravel.app:8000/laravelacademy/update)
## 路由前缀加参数
```php
Route::group(['prefix'=>'laravelacademy/{version}'],function(){
    Route::get('write',function($version){
        return "Write LaravelAcademy {$version}";
    });
    Route::get('update',function($version){
        return "Update LaravelAcademy {$version}";
    });
});
```
那么访问http:://laravel.app:8000/laravelacademy/5.1/write则输出： write LaravelAcademy 5.1
