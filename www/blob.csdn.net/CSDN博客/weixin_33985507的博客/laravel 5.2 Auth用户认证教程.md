# laravel 5.2 Auth用户认证教程 - weixin_33985507的博客 - CSDN博客
2016年10月13日 03:17:27[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
官方文档：[Laravel 5.2文档服务——用户认证](http://laravelacademy.org/post/3074.html)
如果你看官方文档不太懂，那么请看下文操作。
### 说明
框架版本：laravel 5.2 
laravel 5.2内置了auth用户认证服务，所以做网站时用户认证似乎变得简单了很多，再加上框架的中间件机制，实现路由保护功能也更加方便快捷了。
laravel 框架源自外国人开发，许多思想和我们的不太一样，刚开始看的时候我还是一头雾水，可能接触少的缘故；后来看了部分源码才知道具体的过程，所以这里写篇教程帮助入门需要的朋友。
### 初始化Auth
laravel 5.2内置了auth用户认证服务，laravel有专门的命令来快速创建auth用户认证的一些东西，比如路由注册，默认视图等。
初始化（请开启cmd命令行，cd到laravel框架目录）
`php artisan make:auth`
这个命令执行了的操作：
![clipboard.png](https://image-static.segmentfault.com/323/172/3231725400-57ff44b511626_articlex)
生成了注册、登录、重置密码、主页等视图，请在 `resources/views` 文件夹中查看；
创建了HemoController.php文件，在 `app/Http/Controllers` 文件夹查看；
还有就是更新了路由，其实就是注册了路由 `app/Http/routes.php`,打开文件你会看到增加了两行代码：
![clipboard.png](https://image-static.segmentfault.com/117/953/1179533062-57ff46ee555b5_articlex)
`Route::auth();` 这一句是注册相关的路由,具体的源码可以看 `vendor/laravel/framework/srcIlluminate/Routing/Router.php` 中的 `auth`方法
![clipboard.png](https://image-static.segmentfault.com/240/716/2407165227-57ff4824f20a2_articlex)
`Route::get('/home', 'HomeController@index');`这句注册个home路由，指向Home控制器index方法，这是需要登陆的用户才能访问的路由，是因为Home控制器加了个中间件 `auth`。
![clipboard.png](https://image-static.segmentfault.com/804/111/804111052-57ff497b7ad08_articlex)
以上是 `php artisan make:auth`命令的详细说明，接下来是数据库创建。
### 创建数据库
如果你是通过composer安装工具安装的laravel框架，默认会在 `database/migrations`目录中存在两个文件
![clipboard.png](https://image-static.segmentfault.com/367/363/36736332-57ff4a6636a94_articlex)
保持默认就好，不用修改。
如果不存在这样的文件，可以通过
`php artisan make:migration create_users_table`和
`php artisan make:migration create_password_table`生成，
然后编辑文件
```php
//create_users_table.php
<?php
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;
class CreateUsersTable extends Migration
{
    public function up()
    {
        Schema::create('users', function (Blueprint $table) {
            $table->increments('id');
            $table->string('name');
            $table->string('email')->unique();
            $table->string('password');
            $table->rememberToken();
            $table->timestamps();
        });
    }
    public function down()
    {
        Schema::drop('users');
    }
}
```
```php
//create_password_table.php
<?php
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;
class CreatePasswordResetsTable extends Migration
{
    public function up()
    {
        Schema::create('password_resets', function (Blueprint $table) {
            $table->string('email')->index();
            $table->string('token')->index();
            $table->timestamp('created_at');
        });
    }
    public function down()
    {
        Schema::drop('password_resets');
    }
}
```
到这里请开启数据库，并在`.env`文件配置好数据库连接，创建相关数据；；然后执行命令创建表
`php artisan migrate`
命令会创建三个表
![clipboard.png](https://image-static.segmentfault.com/404/575/404575442-57ff4ff78b311_articlex)
到这里工作基本完毕，接下来看看登录注册效果。
### 测试
cd到项目目录，执行 `php -S localhost:3000 -t public` ,如果没有错误，就可以在浏览器输入：`localhost:3000`看效果了。
![clipboard.png](https://image-static.segmentfault.com/378/621/3786210052-57ff51c04ad8c_articlex)
如果你发现开启一致在转圈圈，那是因为模板里引用了cdn的boostrap文件，把它改成为本地的就快了。
![clipboard.png](https://image-static.segmentfault.com/384/044/3840440167-57ff5292ba2e5_articlex)
点击注册
![clipboard.png](https://image-static.segmentfault.com/545/323/545323457-57ff52c6479ab_articlex)
登录
![clipboard.png](https://image-static.segmentfault.com/115/819/1158194926-57ff52e853448_articlex)
重置密码，如果需要重置密码，可以在浏览器地址栏输入 `localhost:3000/password/reset`
![clipboard.png](https://image-static.segmentfault.com/716/725/716725748-57ff5372b9eaa_articlex)
填写需要重置密码，点击发送重置密码链接到邮箱，这里的邮箱是你注册时的邮箱，点击后会在邮箱里收到信息
![clipboard.png](https://image-static.segmentfault.com/312/246/3122467463-57ff5403f005d_articlex)
打开链接就可以重置密码了。
注：这里发送邮件需要在`config/email.php`中或者`.env`文件配置邮箱服务，具体的可以看官方文档[Laravel 5.2 服务——邮件](http://laravelacademy.org/post/3239.html)
### 认证相关方法
可能在流程中会用到一些查看验证信息，所以一些必备的方法获取认证信息是必要的。
一些与认证有关的方法
```php
Auth::guard('admin')    //指定看守 返回Auth对象
    Auth::user();            //获取通过验证的用户 Auth::user()->name
    Auth::check();            //检查是否验证
    Auth::viaRemember();    //判断用户是否使用“记住我”cookie进行认证
    Auth::login($user);        //将一个已存在的用户实例登录到应用中,传入实例必须是Illuminate\Contracts\Auth\Authenticatable 契约的实现
    Auth::loginUsingId($userid);//通过用户ID登录到应用
    Auth::once($credentials);//只在单个请求中将用户登录到应用，而不存储任何 Session 和 Cookie
    Auth::attempt($credentials);//登录用户 ，$credentials是['email' => $email, 'password' => $password],这个方法会和数据库对比
    Auth::onceBasic();
    Auth::provider();
    Auth::logout();            //注销验证用户
    Auth::extend();            //自定义看守
    Auth::provider();        //自定义用户提供者
```
### 提出问题
- 如果实现前后台用户验证？
- 如果laravel自带的用户表结构不符合我的项目需求怎么办？
对于以上似乎网上已经有答案，但我还是发现了些问题，比如 laravel auth的源码大部分已经写死是通过 email,password字段来验证的了；如何得到解答我还在测试中，如果测试完成我会另开文档编写auth源码解读相关教程。如果你已经有了解决办法，希望在评论告知下，谢谢。
