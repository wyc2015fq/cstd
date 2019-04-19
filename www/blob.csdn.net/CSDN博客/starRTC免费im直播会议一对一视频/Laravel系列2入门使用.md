# Laravel系列2入门使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 16:29:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：365
最好的教程是官方文档！
homestead安装好，就可以使用了。
laravel
new command will create a fresh Laravel installation in the directory you specify. For
 instance, laravel new blog will create a directory named blog containing
 a fresh Laravel installation with all of Laravel's dependencies already installed. 
其中app包含了站点的 controllers（控制器），models（模型），views（视图）和 assets（资源）。这些是网站运行的主要代码，你会将你大部分的时间花在这些上面。
bootstrap用来存放系统启动时需要的文件，这些文件会被如 index.php 这样的文件调用。
public文件夹是唯一外界可以看到的，是必须指向你 web 服务器的目录。它含有 laravel 框架核心的引导文件 index.php
vendor用来存放所有的第三方代码
此时浏览器访问public目录（[http://homestead.app/public/](http://homestead.app/public/)，为了方便我把blog目录改成之前配置好的public目录了，因为public目录指向的站点是homtstead.app，参见sites配置），如下图所示：
Laravel（下面简称L）内置了非常完善好用的简单用户登录注册功能,激活这个功能非常容易，运行以下命令：
phpartisanmake:auth
页面不正常，好像是因为**5.1 版本去掉了本系列教程主要讲解的元素(Auth 系统)**
composer create-project laravel/laravel learnlaravel5 5.0.22 //第三个参数中指定版本号
查看app/Http/routes.php 的代码
视图在resources\views下
一定要把网站定位到public目录下，不然访问/home会失败。可通过修改Homestead.yaml，然后vagrant
 provision重启。
==========================================
数据库：
php artisan make:migration
 create_tasks_table --create=tasks
database/migrations directory
 of your project.

可以把phpmyadmin放在public 目录下，然后修改脚本权限： chmod a-w *.php
Let's edit this file （如2016_08_05_051404_create_tasks_table.php）and add an additional string column 
$table->string('name');//add
php artisan migrate
This command will create all of our database tables. 
Eloquent Models
Usually, each Eloquent model corresponds directly with a single database table.
let's define a Task model
 that corresponds to our tasks database table we just created. 
php artisan make:model
 Task
The model will be placed in the app directory 
model name会假设对表的表是model name的复数形式，如Task model对应tasks表
[Routing](https://laravel.com/docs/5.2/quickstart#routing)
defined in the app/Http/routes.php
在此处，we will need at least three routes: a route to display a list of all of our tasks, a route to add new tasks, and a route
 to delete existing tasks.
Displaying A View
HTML templates are stored in the resources/views directory
像每个页面都会用到的导航，可以通过Blade **layouts**实现
let's define a new
layout view in resources/views/layouts/app.blade.php.
 The .blade.php extension
 instructs the framework to use the [Blade templating engine](https://laravel.com/docs/5.2/blade) to
 render the view. 
@yield('content')
是一个Blade**指令:**that specifies where all child pages that extend the layout can inject their
 own content.
Next, let's define the
child view that will use this layout and provide its
content.
Defining The Child View
resources/views/tasks.blade.php
 All of the content between @section('content') and @endsection will
 be injected
into the location of the@yield('content')
The @include('common.errors') directive
 will load the template
located atresources/views/common/errors.blade.php.
<!-- resources/views/common/errors.blade.php -->@if(count($errors)>0)<!-- Form Error List --><div class="alert
 alert-danger"><strong>Whoops!
 Something went wrong!</strong><br><br><ul>
 @foreach($errors->all()as$error)<li>{{$error}}</li>
 @endforeach</ul></div>@endif
测试错误输出：
将输入数据取不合要求的格式即可。
这个错误格式只有include它的view才能显示:@include('common.errors')
[Adding Tasks](https://laravel.com/docs/5.2/quickstart#adding-tasks)
检验输入数据：如少于255个字符
如果校验失败，将重定向用户到/目录
->withErrors($validator) 
 will flash
the errors from the given validator instance
into the session so that they can be accessed via the $errors variable
 in our view.
Creating The Task
To create the task, we may use the save method
 after creating and setting properties on a new Eloquent model:
$task = new Task;
$task->name = $request->name;
$task->save();
现在的样子：
Displaying Existing Tasks
we need to edit our / route
 to pass all of the existing tasks
to the view.
The view function
 accepts a second
argument which is an
array of data that will be made available to the view, where each
key in the array will become a
变量 within the view:

[Deleting Tasks](https://laravel.com/docs/5.2/quickstart#deleting-tasks)
Adding The Delete Button
When the button is clicked, a DELETE/task request
 will be sent to the application:
Note that the delete button's form method is
 listed as POST, even though we are responding to the request using a Route::delete route.
HTML forms only allow the GET and POST HTTP
 verbs, so we need a way to spoof a DELETE request from the form.
We can spoof a DELETE request
 by outputting the results of the method_field('DELETE') function
 within our form. This function generates a hidden form input that Laravel recognizes and will use to
override the actual HTTP request method. The generated field will look like the following:
<input
 type="hidden"
 name="_method"
 value="DELETE">
We can use [implicit
 model binding](https://laravel.com/docs/5.2/routing#route-model-binding) to automatically retrieve the Task model
 that corresponds to the {task} route parameter.
Route::delete('/task/{task}',function(Task
$task){$task->delete();returnredirect('/');});
效果：
