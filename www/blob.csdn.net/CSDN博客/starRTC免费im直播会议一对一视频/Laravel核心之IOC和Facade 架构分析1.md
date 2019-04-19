# Laravel核心之IOC和Facade 架构分析1 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月04日 13:27:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：254
**控制反转（Inversion of Control）**
*缩写为IoC*
*最常见的方式叫做依赖注入*
简单说来，就是一个类把自己的的控制权交给另外一个对象，类间的依赖由这个对象去解决。
**Laravel 中的使用**
注入一个类：
App::bind('foo',
 function($app){ return new FooBar;});
这个例子的意思是创建一个别名为 foo 的类，使用时实际实例化的是 FooBar。
使用这个类的方法是：
$value = App::make('foo');
$value 实际上是 FooBar 对象。
如果希望使用单例模式来实例化类，那么使用：
App::singleton('foo', function(){ return new FooBar;});
这样的话每次实例化后的都是同一个对象。
为了让依赖注入的代码不至于写乱，Laravel 搞了一个 **服务提供器（Service Provider）**的东西，它将这些依赖聚集在了一块，统一申明和管理，让依赖变得更加容易维护。

为了让 Laravel 中的核心类使用起来更加方便，Laravel实现了门面模式。Facade
$value = Cache::get('key');
这些静态调用实际上调用的并不是静态方法，而是通过 PHP 的魔术方法 __callStatic() 讲请求转到了相应的方法上。
那么如何讲我们前面写的**服务提供器**也这样使用呢？方法很简单，只要这么写：
use Illuminate\Support\Facades\Facade;class Foo extends Facade { protected
static
function getFacadeAccessor() { return ‘foo’; }}
这样我们就可以通过 Foo::test() 来调用我们之前真正的 FooBar 类的方法了。
总结一下创建自己类库的方法：
- 在 app/library/MyFoo 下创建类 MyFoo.php
- 在 app/library/MyFoo/providers 下创建 MyFooServiceProvider.php
- 在 app/library/MyFoo/facades 下创建 MyFooFacade.php
- 在 app/config/app.php 中添加 providers 和 aliases
工厂模式的升华 —— IoC
 容器。
一个类需要绑定、注册至容器中，才能被“制造”。
 laravel 称这个容器叫做服务容器，那么我们需要某个服务，就得先注册、绑定这个服务到容器，那么提供服务并绑定服务至容器的东西，就是 服务提供者（ServiceProvider）。服务提供者主要分为两个部分，register（注册） 和 boot（引导、初始化）
app/Http/Kernel.php 中的
 $middleware 数组是全局中间件，也就是说，任何一条路由都会被应用这些中间件
有时候我们不需要全局中间件，这时候可以将某一个中间件注册至 app/Http/Kernel.php文件中的
 $routeMiddleware 数组，
首先会经过全局中间件，然后就是我们在 app/Http/Kernel.php 的
 $routeMiddleware 数组中定义的中间件。
return $next($request)将请求导向下一个中间件
用路由组，定义子域名变得十分容易：
- Route::group(['domain'
 => 'bbs.yourdomain.com']
比如我希望我的网站每一个用户都拥有自己的二级域名，类似于这样：userA.yourdomain.com，userB.yourdomain.com。这时候可以这样写：
- 
Route::group(['domain' => '{username}.myapp.com'], function()
- 
{
- 
Route::get('profile/{type}', function($username, $type)
- 
{
- 
//
- 
});
- 
});
[https://laravel-china.org/topics/3361/starting-with-the-1-line-of-code-with-a-systematic-understanding-of-the-core-concepts-of-service-container](https://laravel-china.org/topics/3361/starting-with-the-1-line-of-code-with-a-systematic-understanding-of-the-core-concepts-of-service-container)
class Container{protected$binds;protected$instances;publicfunctionbind($abstract,$concrete){//Todo: 向 container 添加一种对象的的生产方式//$abstract: 第一个参数 $abstract, 一般为一个字符串(有时候也会是一个接口),
 当你需要 make 这个类的对象的时候, 传入这个字符串(或者接口), 这样make 就知道制造什么样的对象了//$concrete: 第二个参数 $concrete, 一般为一个 Closure 或者 一个单例对象, 用于说明制造这个对象的方式if($concreteinstanceof Closure){ $this->binds[$abstract]=$concrete;}else{ $this->instances[$abstract]=$concrete;}}publicfunctionmake($abstract,$parameters=[]){//Todo: 生产一种对象//$abstract: 在bind方法中已经介绍过//$parameters: 生产这种对象所需要的参数if(isset($this->instances[$abstract])){return $this->instances[$abstract];}array_unshift($parameters,
 $this);returncall_user_func_array($this->binds[$abstract],$parameters);}}

比如
Cache::get('key');
 Route::get('/','HomeController@index');
Cache 和 Route 都是通过把他们各自的实现类 bind 到某个
 Laravel 的 Container 后，那个 Container 所 make 出的一个实例。
到底是在哪进行 bind 操作的，又是 bind 到哪一个
 Container 了，这个 Container 又是在什么地方 make 了他们？
**IOC Container 组成了 Laravel 的架构，是 Laravel 的核心机制。**在
 Laravel 中，他们把这个叫做 Laravel 的 Service Container
该如何使用 Laravel 的 IOC Container（ Service Container ）？[#](https://laravel-china.org/topics/3361/starting-with-the-1-line-of-code-with-a-systematic-understanding-of-the-core-concepts-of-service-container#%E8%AF%A5%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8-Laravel-%E7%9A%84-IOC-Container-Service-Container-)
如果想要使用 Laravel 的 IOC Container，也就是说想要用 IOC 的机制去 make 某种对象,
 那么你就必须先 bind 这个对象的类到 Laravel 的 IOC Container 中, 才能把这种对象 make 出来。
至此，可以请出今天第二个角色了，Service Provider
在 Laravel 中，我们大体可以上有2种方式去使用 IOC Container：
- 通过 Service Provider 来使用IOC Container
- 不通过 Service Provider 直接使用 IOC Container
bind
App::bind('post',function($app){return App::make('App\Post');});
最后 make
$post= App::make('post');return$post->d;//将会返回 "123"
Route::get('/','HomeController@index');
你是无法找到对 Route 类的声明的，为啥呢，因为使用了别名。别名是 PHP 的一个特性（ class_alias 方法 ）。
Route 是如何配置成为别名的呢， 在 app/config/app.php 中，
 我们可以看到 Laravel 把所有的别名配置都放在了这个数组中
'aliases'=>[...'Route'=> Illuminate\Support\Facades\Route::class,...

我们看到 Route 实际上是代表了 Illuminate\Support\Facades\Route::class 这个类，
 我们找到这个类
namespace Illuminate\Support\Facades;/**
 * @see \Illuminate\Routing\Router */class
 Route extends Facade{protectedstaticfunctiongetFacadeAccessor(){return'router';}}
看到这个类之后，并没有找到之前 Route 调用的 get 方法，此时我们再看里面的这行注释，@see
 \Illuminate\Routing\Router，他提示我们去找这个位置，那我们就去找一下，我们又发现了一个 Router 类，而这个 Router 类中，是有 get 方法的，看起来这里似乎就是
 Route 的真实身份了。
那 Laravel 是如何为 Illuminate\Support\Facades\Route::class 这个类找到他的真实身份的呢？
Facades。
**Facade 的作用是用一个简单易记的语法，让你从 Laravel 的 IOC Container 中方便的 ****make**** 出你想要的类的对象。**
在 Facade 类中，我们可以发现有一个 __callStatic()[魔术方法](http://blog.csdn.net/liutingxu1/article/details/17655049)，这个方法的作用就是：如果你想要调用的静态方法在类的定义中并没有声明，那么就会执行 __callStatic()。在我们当前的情景中，静态方法 get 并没有被声明，那么当然，我们的类就会转而调用 __callStatic() 。
看 __callStatic() 的执行过程
static::resolveFacadeInstance("router");
返回 static::$app[$name]
 返回了 static::$app['router'] 这个值。

而 $app 其实并没有 'router' 这个属性，那为什么可以这样调用呢？ 是因为 Application 继承了 Container， 而 Container 又继承了 ArrayAccess 这个类。正是由于 ArrayAccess 的存在，以及
 Container 实现了 ArrayAccess 的下面这个方法：
publicfunctionoffsetGet($key){return $this->make($key);}
所以，当我们使用 $app['router'] 时，实际上是执行了 $app->make('router')
就是从 $app 这个 IOC Container 中，make 了一个
 router 的实例。
在 config/app.php 中，很容易就能找到跟我们的
 Route 相关的， 也就是 App\Providers\RoutingServiceProvider::class,
命名空间位于 Illuminate\Contracts 之下，而这种接口在
 Laravel 中就被称为 Contract。普通的 interface没什么2样
Facades 是外观模式，类似代理，在 Laravel 里的作用其实就跟
[PHP: class_alias](https://link.zhihu.com/?target=http%3A//php.net/manual/zh/function.class-alias.php) 的作用类似
只是把框架里带命名空间的类变得更好记，比如：App => Illuminate\Support\Facades\App
**什么是 Trait ?**
其实说通俗一点，就是能把重复的**方法**拆分出去，通过
 use 引入以达到代码复用的目的。
trait 有什么优势？来看一段代码：
class User extends Model{ use Authenticate, SoftDeletes, Arrayable, Cacheable; ...}
这个用户模型类，我们引入了四个特性：注册与授权、软删除、数组式操作、可缓存。


[https://zhuanlan.zhihu.com/p/28231762?utm_source=tuicool&utm_medium=referral](https://zhuanlan.zhihu.com/p/28231762?utm_source=tuicool&utm_medium=referral)
[http://yansu.org/2014/12/06/ioc-and-facade-in-laravel.html](http://yansu.org/2014/12/06/ioc-and-facade-in-laravel.html)
[https://laravel-china.org/topics/789/laravel-learning-notes-the-magic-of-the-service-container](https://laravel-china.org/topics/789/laravel-learning-notes-the-magic-of-the-service-container)
[https://www.insp.top/article/7](https://www.insp.top/article/7)
