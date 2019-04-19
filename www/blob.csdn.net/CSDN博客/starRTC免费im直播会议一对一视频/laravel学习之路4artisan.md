# laravel学习之路4artisan - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月17日 18:37:03[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：195
个人分类：[php-laravel](https://blog.csdn.net/elesos/article/category/7108608)
php artisan list
php artisan
help
migrate
Tinker 让你可以在命令行中与 Laravel 应用进行交互php
 artisan tinker
在routes/console.php里面可以加闭包命令
Artisan::*command*('build
 {project}',
**function**($project)
 {
$this->info("Building
{$project}!");
})->describe('Build
 the project, yeah');
php artisan build elesos
参数及选项都包在大括号中
不接收值的选项，当在命令中写出时，值为true,不写表示false
注册app/Console/Kernel.php
protected$commands=[ Commands\SendEmails::class];
[程序内部调用命令](http://d.laravel-china.org/docs/5.4/artisan#programmatically-executing-commands)
Route::get('/foo',function(){$exitCode= Artisan::call('email:send',['user'=>1,'--queue'=>'default']);//});
还可以命令中调用其它命令

