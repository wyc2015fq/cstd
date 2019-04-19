# Laravel 之Service Providers - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月02日 20:12:05[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：487
Service providers are the
central place of all Laravel application bootstrapping. Your own application, as well as all of
 Laravel's core services are bootstrapped via service providers.
what do we mean by "bootstrapped"? In general, we mean **registering**things,
 including registering service container bindings, event listeners, middleware, and even routes. Service providers are the central place to configure your application.
If you open the config/app.php file
 included with Laravel, you will see a providers array. These are all of the service provider
 classes that will be loaded for your application. Of course, many of these are "deferred**延迟的**"
 providers, meaning they will not be loaded on every request, but only when the services they provide are
actually needed.
本文中 you will learn
how to write your own service providers and
register
them with your Laravel application.
[Writing Service Providers](https://laravel.com/docs/5.3/providers#writing-service-providers)
All service providers
extend the Illuminate\Support\ServiceProvider class.
Most service providers contain a register and
 a boot method.
 Within the register method, you should **only bind things into the **[**service container**](https://laravel.com/docs/5.3/container).
 You should never attempt to register any event listeners, routes, or any other piece of functionality within the register method.
The Artisan CLI can generate a new provider via the make:provider command:
php artisan make:provider
 RiakServiceProvider
[Registering Providers](https://laravel.com/docs/5.3/providers#registering-providers)
registered in the config/app.php configuration
 file. 
