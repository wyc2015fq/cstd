# Laravel Permission 实现 RBAC 权限管理功能 - weixin_33985507的博客 - CSDN博客
2018年06月13日 18:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
### 版本
laravel5.6
### 安装
```
composer require spatie/laravel-permission
```
> 
配置文件
```
php artisan vendor:publish --provider="Spatie\Permission\PermissionServiceProvider" --tag="config"
```
```
<?php
return [
    'models' => [
          //权限规则模型
        'permission' => Spatie\Permission\Models\Permission::class,
        //角色模型
         'role' => Spatie\Permission\Models\Role::class,
    ],
    'table_names' => [
        //角色表
        'roles' => 'roles',
        //权限规则表
        'permissions' => 'permissions',
        //模型是否有权限规则表
        'model_has_permissions' => 'model_has_permissions',
        /*
         * 模型是否拥有角色权限中间表
         */
        'model_has_roles' => 'model_has_roles',
        /*
         *角色权限中间表
         */
        'role_has_permissions' => 'role_has_permissions',
    ],
    /*
     *权限缓存时间
     */
    'cache_expiration_time' => 60 * 24,
    /*
     错误消息是否将权限规则加入到里面
     */
    'display_permission_in_exception' => false,
];
```
> 
迁移文件
```
php artisan vendor:publish --provider="Spatie\Permission\PermissionServiceProvider" --tag="migrations"
```
```
<?php
use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;
class CreatePermissionTables extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        //导入刚才配置文件
        $tableNames = config('permission.table_names');
        //权限表，ID，名，描述，时间
        Schema::create($tableNames['permissions'], function (Blueprint $table) {
            $table->increments('id');
            $table->string('name');
            $table->string('guard_name');
            $table->timestamps();
        });
        //角色表，id，名字，描述，时间
        Schema::create($tableNames['roles'], function (Blueprint $table) {
            $table->increments('id');
            $table->string('name');
            $table->string('guard_name');
            $table->timestamps();
        });
        //model_has_permissions 模型权限表，model_id,model_type,外键permission_id。用于删除
        Schema::create($tableNames['model_has_permissions'], function (Blueprint $table) use ($tableNames) {
            $table->unsignedInteger('permission_id');
            $table->morphs('model');
            $table->foreign('permission_id')
                ->references('id')
                ->on($tableNames['permissions'])
                ->onDelete('cascade');
            //添加组合索引permission_id，model_id，model_type
            $table->primary(['permission_id', 'model_id', 'model_type']);
        });
        //model_has_roles 模型角色表，role_id,model_id,model_type,外键role_id
        Schema::create($tableNames['model_has_roles'], function (Blueprint $table) use ($tableNames) {
            $table->unsignedInteger('role_id');
            $table->morphs('model');
            $table->foreign('role_id')
                ->references('id')
                ->on($tableNames['roles'])
                ->onDelete('cascade');
            $table->primary(['role_id', 'model_id', 'model_type']);
        });
        //role_has_permissions 角色权限中间表 permission_id，role_id
        Schema::create($tableNames['role_has_permissions'], function (Blueprint $table) use ($tableNames) {
            $table->unsignedInteger('permission_id');
            $table->unsignedInteger('role_id');
            $table->foreign('permission_id')
                ->references('id')
                ->on($tableNames['permissions'])
                ->onDelete('cascade');
            $table->foreign('role_id')
                ->references('id')
                ->on($tableNames['roles'])
                ->onDelete('cascade');
            $table->primary(['permission_id', 'role_id']);
            app('cache')->forget('spatie.permission.cache');
        });
    }
    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        $tableNames = config('permission.table_names');
        Schema::drop($tableNames['role_has_permissions']);
        Schema::drop($tableNames['model_has_roles']);
        Schema::drop($tableNames['model_has_permissions']);
        Schema::drop($tableNames['roles']);
        Schema::drop($tableNames['permissions']);
    }
}
```
![10865887-cd72158de7f3fd15.png](https://upload-images.jianshu.io/upload_images/10865887-cd72158de7f3fd15.png)
image.png
> 
User增加HasRole Traits
```
<?php
namespace App;
use Illuminate\Notifications\Notifiable;
use Illuminate\Foundation\Auth\User as Authenticatable;
use  Spatie\Permission\Traits\HasRoles;
class User extends Authenticatable
{
    use HasRoles,Notifiable;
    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'name', 'email', 'password',
    ];
    /**
     * The attributes that should be hidden for arrays.
     *
     * @var array
     */
    protected $hidden = [
        'password', 'remember_token',
    ];
}
```
> 
HasRoles
文件内容比较多，可以自己查阅
### 安装html生成器laravelcollective/html
[https://packagist.org/packages/laravelcollective/html](https://packagist.org/packages/laravelcollective/html)
```
composer require laravelcollective/html
```
[https://laravelcollective.com/docs/master/html](https://laravelcollective.com/docs/master/html)
> 
config/app.php:
```
'providers' => [
    // ...
    Collective\Html\HtmlServiceProvider::class,
    // ...
  ],
'aliases' => [
    // ...
      'Form' => Collective\Html\FormFacade::class,
      'Html' => Collective\Html\HtmlFacade::class,
    // ...
  ],
```
### 基本使用
> 
路由
```
Route::get('/', ['uses'=>'UserController@index']);
Route::get('/getp/{user}', ['uses'=>'UserController@getPermission']);//取得是否有这个权限规则
```
> 
控制器
```
<?php
namespace App\Http\Controllers;
use App\User;
use Illuminate\Http\Request;
use Spatie\Permission\Models\Role;
use Spatie\Permission\Models\Permission;
class UserController extends Controller
{
    //
    public function index(){
      $role = Role::create(['name' => 'writer']);
      $permission = Permission::create(['name' => 'edit articles']);
    }
    public function getPermission(User $user){
      print_r($user->toArray());
      $permissions = $user->permissions;
      print_r($permissions);
    }
}
```
> 
测试
浏览器打开
```
http://192.168.91.130:86
//取得权限
http://192.168.91.130:86/getp/1
```
这个生成SQL语句如下
> 
getPermission
```
select `permissions`.*, 
`model_has_permissions`.`model_id` as `pivot_model_id`, 
`model_has_permissions`.`permission_id` as `pivot_permission_id`, 
`model_has_permissions`.`model_type` as `pivot_model_type`
 from `permissions` inner join `model_has_permissions`
 on `permissions`.`id` = `model_has_permissions`.`permission_id` 
where `model_has_permissions`.`model_id` = '1'
 and `model_has_permissions`.`model_type` = 'App\User'
```
这个就是多态关联的应用
> 
//获取所有//获取所有
```
$user->roles()
```
sql
```
select `roles`.*, 
`model_has_roles`.`model_id` as `pivot_model_id`, 
`model_has_roles`.`role_id` as `pivot_role_id`, 
`model_has_roles`.`model_type` as `pivot_model_type`  
from `roles` inner join `model_has_roles`  
on `roles`.`id` = `model_has_roles`.`role_id` 
where `model_has_roles`.`model_id` = '1'  
and `model_has_roles`.`model_type` = 'App\User'
```
> 
获取用户角色名称
```
$user->roles()->pluck('name')
```
Sql
```
select `name` from `roles` inner join `model_has_roles`  
on `roles`.`id` = `model_has_roles`.`role_id`  
where `model_has_roles`.`model_id` = '1'  
and `model_has_roles`.`model_type` = 'App\User'
```
### Blade
```
@role('writer')
    I'm a writer!
@else
    I'm not a writer...
@endrole
@hasrole('writer')
    I'm a writer!
@else
    I'm not a writer...
@endhasrole
@hasanyrole(Role::all())
   任意一个这个角色
@else
    I have none of these roles...
@endhasanyrole
@hasallroles(Role::all())
  所有这些角色
@else
    I don't have all of these roles...
@endhasallroles
```
> 
@can 指令验证用户是否拥有给定权限：
```
@can('Edit Post')
    I have permission to edit
@endcan
```
User模型增加密码加密
```
public function setPasswordAttribute($password) {
      $this->attributes['password'] = bcrypt($password);
    }
```
其他详细请看
[https://packagist.org/packages/spatie/laravel-permission](https://packagist.org/packages/spatie/laravel-permission)
下一篇写实操操作，这里不做长篇。
