# laravel学习之路1：认证相关 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月10日 10:46:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：212
[Laravel中Auth::guard()表示什么意思？](https://segmentfault.com/q/1010000007761167)
Auth::check() 是判断用户是否登录的方法，如果使用的默认用户系统，那这样使用没问题。
但是使用两组用户的话，如何使用各组用户的功能呢？ Auth::guard('users')->check() 就是用来判断前台用户是否登录，而 Auth::guard('admins')->check() 就是用来判断后台用户是否登录的。
默认的User model扩展自Illuminate\Foundation\Auth\User
**class**User
**extends**Model
**implements**
AuthenticatableContract,
AuthorizableContract,
CanResetPasswordContract
{
**use**Authenticatable,
Authorizable,
CanResetPassword;
}
use的trait实现了AuthorizableContract这些接口，所以不用自己写代码，就实现了接口。
middleware('guest:client')
应该是
通过冒号 : 来隔开中间件与参数
其实是
'guest'
=> \App\Http\Middleware\RedirectIfAuthenticated::*class*,
看其定义发现有个参数$guard, 应该传的是我们定义的guard

$this->middleware('guest')->except('logout');
其中这个except表示logout这个方法将不会受到这个中间件的影响
logout通常是登出方法
今天学习了多用户表登录：
[https://laravel-china.org/topics/2777/laravel-53-multi-user-login](https://laravel-china.org/topics/2777/laravel-53-multi-user-login)
[下一步要实现退出功能。](https://laravel-china.org/topics/2777/laravel-53-multi-user-login)
[**PHP**](http://lib.csdn.net/base/php) artisan
 make:auth，经常好奇这段代码到底干了什么，现在就来扒一扒。
[http://blog.csdn.net/yqxllwy/article/details/75738646](http://blog.csdn.net/yqxllwy/article/details/75738646)
Auth Facades里面有
**public static function**routes()
{
**static**::*$app*->make('router')->auth();
}
然后看*Illuminate\Routing\Router.*[***php***](http://lib.csdn.net/base/php)
**public function**auth()
{
// Authentication
 Routes...
$this->get('login',
'Auth\LoginController@showLoginForm')->name('login');
$this->post('login',
'Auth\LoginController@login');
$this->post('logout',
'Auth\LoginController@logout')->name('logout');
// Registration
 Routes...
$this->get('register',
'Auth\RegisterController@showRegistrationForm')->name('register');
$this->post('register',
'Auth\RegisterController@register');
// Password Reset
 Routes...
$this->get('password/reset',
'Auth\ForgotPasswordController@showLinkRequestForm')->name('password.request');
$this->post('password/email',
'Auth\ForgotPasswordController@sendResetLinkEmail')->name('password.email');
$this->get('password/reset/{token}',
'Auth\ResetPasswordController@showResetForm')->name('password.reset');
$this->post('password/reset',
'Auth\ResetPasswordController@reset');
}
**protected function**guard()
{
**return**Auth::*guard*();
}
复制 layouts 下面的 app.blade.php，改名为 admin.blade.php 并对该文件做出修改
Auth::guest() 改为 Auth::guard('admin')->guest()
Auth::*user*()->name
 ->Auth::*guard*('client')->user()->name

假设你的密码为**123456**，那么你保存在数据库中的值就是
password_hash('123456','PASSWORD_BCRYPT')
