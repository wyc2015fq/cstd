# Laravel系列 目录结构 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月02日 14:36:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：358
**Where Is The****Models****Directory?**
app directory by default
其中
app:,core code of your application,
 almost all of the classes in your application will be in this directory.
By default, this directory is namespaced under App and
 is autoloaded by Composer using the [PSR-4 autoloading standard](http://www.php-fig.org/psr/psr-4/).
 Think of the Console and Http directories
 as providing an API into the core of your application. 
The Console directory
 contains all of your Artisan commands, while the Http directory contains
 your controllers, middleware, and requests.
A variety of other directories
will be generated
inside the app directory
 as you use the make Artisan commands to generate classes. for example, the app/Jobs directory
 will not exist until you execute the make:job Artisan
 command to generate a job class.
查看命令
 To review the available commands, run the php
 artisan list make command in your terminal.
The Console directory
 contains all of the custom Artisan commands for your application. These commands may be generated
 using the make:command .
 This directory also houses your console kernel, which is where your custom Artisan commands are
registered
and your [scheduled tasks](https://laravel.com/docs/5.3/scheduling) are
 defined.
**Events Directory**
This directory does not exist by default, but will be created for you by the event:generate andmake:event Artisan
 commands. The Events directory, as you might expect, houses[event
 classes](https://laravel.com/docs/5.3/events). Events may be used to
alert
other parts of your application that a given action has
occurred, providing a great deal of flexibility and decoupling**去耦合**.
The Exceptions directory
 contains your application's exception handler
and is also a good place to place any exceptions thrown by your application. If you would like to customize how your exceptions are
 logged or rendered, you should modify the Handler class
 in this directory.
The Http directory
 contains your controllers, middleware, and form requests.Almost all of the logic 所有逻辑 to handle requests entering your application will be placed in this
 directory.
**Jobs Directory**
This directory does not exist by default, but will be created for you if you execute the make:jobArtisan
 command. The Jobs directory houses the [queueable
 jobs](https://laravel.com/docs/5.3/queues) for your application. Jobs may be queued by your application or run synchronously** 同步地**
 within the current request lifecycle. Jobs that run synchronously during the current request are sometimes referred to as "commands" since they are an implementation of the [command
 pattern](https://en.wikipedia.org/wiki/Command_pattern).
**Listeners Directory**
This directory does not exist by default, but will be created for you if you execute the event:generateormake:listener Artisan
 commands. The Listeners directory contains the classes that
handle your[events](https://laravel.com/docs/5.3/events).
 Event listeners receive an event instance and perform logic in response to the event being fired. For example, a UserRegistered event
 might be handled by a SendWelcomeEmail listener.
**Mail Directory**
This directory does not exist by default, but will be created for you if you execute the make:mailArtisan
 command. The Mail directory contains all of your classes that represent emails sent by
 your application. Mail objects allow you to encapsulate all of the logic of building an email in a single, simple class that may be sent using the Mail::send method.
**Notifications Directory**
This directory does not exist by default, but will be created for you if you execute themake:notificationArtisan
 command. The Notifications directory contains all of the "transactional**事务性的**"
 notifications that are sent by your application, such as simple notifications about events that happen within your application. Laravel's notification features abstracts sending notifications over a variety of drivers such as email, Slack, SMS, or stored in
 a database.
**Policies Directory**
This directory does not exist by default, but will be created for you if you execute the make:policyArtisan
 command. The Policies directory contains the
authorization policy classes for your application. Policies are used to
determine if a user can perform a given action against a resource. For more information, check
 out the [authorization documentation](https://laravel.com/docs/5.3/authorization).
** Providers Directory**
The Providers directory
 contains all of the [service providers](https://laravel.com/docs/5.3/providers)for
 your application. Service providers bootstrap your application by binding services in the service
 container, registering events, or performing any other tasks to
prepare your application for incoming requests.
In a fresh Laravel application, this directory will already contain several providers. You are free to add your own providers
 to this directory as needed.
===================================================================================
**Bootstrap :** files
 that bootstrap the framework and configure autoloading. 里面有个cache directory which contains
framework generated files for performance optimization such as the
route and services cache files.
**Config :配置文件 目录**
**Database ：**database
 migration and seeds.
**Public ：**contains the index.php file,
 which is the entry point for all requests entering your application. This directory also houses your assets such as images, JavaScript, and CSS.
**Resources ：**
contains your
views as well as your raw, un-compiled assets such as LESS, SASS, or JavaScript. This directory also houses all of your language files.
**Routes ：**
contains all of the route definitions for your application. By
default, three route files are included with Laravel: web.php, api.php,
 and console.php.
The web.php 
 contains routes that the RouteServiceProvider places in the web middleware
 group, which provides session state, CSRF protection, and cookie encryption. If your application does not offer a stateless, RESTful API,all of your routes will most likely be defined in the web.phpfile.
The api.php file
 contains routes that the RouteServiceProvider places in the api middleware
 group, which provides rate limiting. These routes are intended to be stateless, so requests entering the application
 through these routes are intended to be authenticated via tokens and will not have access to session state.
The console.php file
 is where you may define all of your Closure based console commands. Each Closure is bound to a command instance allowing a simple approach to interacting with each command's IO methods. Even though this file does not define HTTP routes, it defines console
 based entry points (routes) into your application.
**Storage **
contains your
compiled Blade templates, file based sessions, file caches, and other files generated by the framework. This directory
 is segregated into app, framework,
 and logs
directories. The app directory
 may be used to store any files generated by your application. Theframework directory
 is used to store framework generated files and caches. Finally, the logsdirectory contains
 your application's log files.
The storage/app/public directory
 may be used to store user-generated files, such as profile avatars, that should be publicly accessible.
 You should create a symbolic link
at public/storage which
 points to this directory. You may create the link using the php artisan storage:link command.

**Tests**
test class should be suffixed with the word TestYou
 may run your tests using the phpunit orphp
 vendor/bin/phpunit commands.
**Vendor **
contains your [Composer](https://getcomposer.org/) dependencies.


[https://laravel.com/docs/5.3/structure](https://laravel.com/docs/5.3/structure)
