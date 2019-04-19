# Laravel之Service Container服务容器 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月02日 19:57:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：340
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
managing class dependencies and performing dependency injection.
Dependency injection is a fancy phrase that essentially
means this: class dependencies are "injected" into the class
via the constructor or, in some cases, "setter" methods.
[Binding](https://laravel.com/docs/5.3/container#binding)
Almost all of your service container bindings will be registered within[service
 providers](https://laravel.com/docs/5.3/providers), so most of these examples will demonstrate using the container in that context.
There is no need to bind classes into the container
if they
do not depend on any interfaces. The container does not need to be instructed on how to build these objects, since it can automatically resolve these objects using reflection.

**Simple Bindings**
Within a service provider, you always have
access to the container via the $this->app property.
 We can register a binding using the bind method,
 passing the class or interface name that we wish to register along with a Closure that
 returns an instance of the class:
$this->app->bind('HelpSpot\API',function($app){returnnew HelpSpot\API($app->make('HttpClient'));});
Note that we receive the container itself as an argument to the resolver. We can then use the container to resolve sub-dependencies
 of the object we are building.
**Binding A Singleton 单例**
The singleton method
 binds a class or interface into the container that should only be resolved one time. Once a singleton binding is resolved, the same object instance will be returned on subsequent
 calls into the container:
$this->app->singleton('HelpSpot\API',function($app){returnnew HelpSpot\API($app->make('HttpClient'));});
**Binding Instances**
You may also bind an existing object instance into the container using the instance method.
 The given instance will always be returned on subsequent calls into the container:
$api=new HelpSpot\API(new
 HttpClient);$this->app->instance('HelpSpot\Api',$api);

**Binding Primitives绑定常用的基本类型**
Sometimes you may have a class that receives some injected classes, but also needs an injected primitive value such as an integer.
 You may easily use contextual binding to inject any value your class may need:
$this->app->when('App\Http\Controllers\UserController')->needs('$variableName')->give($value);

Binding Interfaces To Implementations
$this->app->bind('App\Contracts\EventPusher','App\Services\RedisEventPusher');
Contextual Binding
解析：
You may use the make method
 to resolve a class instance out of the container. The make method
 accepts the name of the class or interface you wish to resolve:
$api= $this->app->make('HelpSpot\API');
[Container Events](https://laravel.com/docs/5.3/container#container-events)
The service container fires an event each time it resolves an object. You may listen to this event using the resolving method:
allowing you to set any additional properties on the object before it is given to its consumer.
[https://laravel.com/docs/5.3/container](https://laravel.com/docs/5.3/container)
