# laravel学习之路3 数据库相关 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月17日 13:13:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：170
读写分离之多个读？
有 'host' => $readHosts[array_rand($readHosts)],
上面的好像有缓存问题php
 artisan config:cache
[https://www.neontsunami.com/posts/multiple-database-read-write-connections-in-laravel](https://www.neontsunami.com/posts/multiple-database-read-write-connections-in-laravel)
It turns out that Laravel will automatically pick a database connection at random if provided with an array,so there's no need to pick a random
 connection yourself. 
'mysql' => [ 'driver' => 'mysql', 'read' => [ 'host' =>** ['193.168.1.1', '194.168.1.1']** ], 'write' => [ 'host' => '196.168.1.2' ], //]
原生的用
DB::insert等
法二：
DB::table('users')->insert(['email'=>'john@example.com','votes'=>0]);
法二可以链式调用
可以在AppServiceProvider
 里面监听每一条sql语句
事务
DB::transaction(function(){ DB::table('users')->update(['votes'=>1]);
 DB::table('posts')->delete();//
删除记录, 如果要清空表并将自动递增置为0，用truncate});
$users= DB::table('users')->get();
 可以链式调用，最终用get获取结果
$client = DB::table('clients')->where('email', 'client1@qq.com')->get();
DB::*table*('clients')->count();
只返回部分字段
->select('name',
'email as user_email')
union合并查询时，第一个不能用get()
whereDate，
whereDay等可以直接对默认的created_at字段进行查询
inRandomOrder 将查询结果随机排序
插入记录并获取其 ID：
insertGetId
更新
$affected_rows
= DB::*table*('clients')
->where('id',
3)
->update(['name'
=>
'client3',
'email'=>
'client3@qq.com'
]);
分页
**return**DB::*table*('clients')->paginate(2);
App\User::paginate(15);

**current_page:1 当前页码**
**"from"**:
1**,**
 这个应该是本次返回的数据id范围
**"to"**:
2**,****"last_page"**:
25**,**
 总页数**"next_page_url"**:
"http://laravel_web.app/test_select?page=2"**,****"path"**:
"http://laravel_web.app/test_select"**,****"per_page"**:
2**,**
 每页面显示的记录数**"prev_page_url"**:
null**,****"total"**:
50 总记录条数
第2页面：
**"current_page"**:
2**,**
**"from"**:
3**,**
**"to"**:
4**,****"last_page"**:
25**,****"next_page_url"**:
"http://laravel_web.app/test_select?page=3"**,****"path"**:
"http://laravel_web.app/test_select"**,****"per_page"**:
2**,****"prev_page_url"**:
"http://laravel_web.app/test_select?page=1"**,****"total"**:
50
{{$users->fragment("ts=".time())->links()
}}
[http://laravel_web.app/test_select?page=5#ts=1502449118](/test_select?page=5#ts=1502449118)
eloquent
添加
$flight=new Flight;
$flight->name=$request->name;$flight->save();
firstOrCreate 
 先查找，找不到，会插入、
updateOrCreate 
 存在则更新，不存在就插入
删除
$flight->delete();或者App\Flight::destroy(1);
软删除
1，use
 SoftDeletes;
protected$dates=['deleted_at'];
2，数据库里加字段deleted_at
$table->softDeletes();
全局作用域允许我们为给定模型的所有查询添加条件约束。
Eloquent 模型会触发许多事件
如created，updated，deleted
关联关系
一对一hasOne， 是不是可以将方法名改为hasOnePhone
hasOne(类名， Phone里面的外键名称， 本表的主键id)
注意调用的时候不要加函数的括号！！！
查出来的是phone表的一条记录
一对多：
$users
= Client::*find*(1)->hasManyPhones;
$users
= Phone::*find*(1)->belongsToUser->name;
还可以链式调用，此时要加上括号
$users
= Client::*find*(1)->hasManyPhones()->where('id',
2)->first();
一对多的反向关联跟一对一的反向关联是一样的。

多对多:
role_user 表命名是以相关联的两个模型数据表来依照字母顺序命名，如果不是这种，就要通过
 第2个参数传递你的表名。如下：
belongsToMany ，第2个参数是关联表的名称，
第三个参数是本类的 id，第四个参数是第一个参数那个类的 id。
反向多对多：同上
远层一对多：Has Many Through 这个就是多级多层关联
countries id
- integerusers id
- integer
country_id
- integerposts id
- integer
user_id
- integer
虽然 posts 本身不包含 country_id 字段，但 hasManyThrough 关联通过 $country->posts 来让我们可以访问一个国家的文章。
return
 $this->hasManyThrough('App\Post','App\User');
第一个参数为我们希望最终访问的模型名称，而第二个参数为中间模型的名称。
第三个参数为中间模型的外键名称country_id
，而第四个参数为最终模型的外键名称user_id
，第五个参数则为本地键id。（好像一般最后一个参数都是id）
多态关联
一个模型在单个关联中从属一个以上其它模型
如：可以「评论」文章和视频，可以使用一个comments 数据表就可以同时满足两个使用场景
posts id
- integervideos id
- integercomments id
- integer commentable_id
- integer commentable_type
-string
commentable_id用于存放文章或者视频的
 id ，而 commentable_type 用于存放所属模型的类名
通过$comment->commentable;可以反向取回Post或Video的实例。
多态多对多
如，博客的 Post 和 Video 模型可以共用多态关联至 Tag 模型
posts id
- integervideos id
- integertags id
- integertaggables
tag_id
- integer taggable_id
- integer taggable_type
-string
morphToMany('App\Tag','taggable');
参数：第一个参数为要关联的model名， 第2个参数为tag 里面的方法？
// 获取那些至少拥有一条评论的文章...用has$posts= App\Post::has('comments')->get();
// 获取所有至少有三条评论的文章...$posts= Post::has('comments','>=',3)->get();
关联数据计数
$posts= App\Post::withCount('comments')->get();
foreach($postsas$post){echo$post->comments_count;}
预加载
$books= App\Book::all();foreach($booksas$book){echo$book->author->name;}
若存在着 25 本书，则会执行 26 次查找：1 次是查找所有书籍，其它 25 次则是在查找每本书的作者。可以使用预加载来将查找的操作减少至 2 次。
$books= App\Book::with('author')->get();
对于该操作则只会执行两条
 SQL 语句：
select
* from booksselect
* from authors where id in
(1,2,3,4,5,...)
延迟预加载
$books= App\Book::all();if($someCondition){$books->load('author','publisher');}
[写入关联模型](http://d.laravel-china.org/docs/5.4/eloquent-relationships#inserting-and-updating-related-models)
$comment=new App\Comment(['message'=>'A new comment.']);$post= App\Post::find(1);$post->comments()->save($comment);
最好不用Create 方法，需要设置批量赋值。
而且好像也要默认值，不然没有默认值的字段，如果不能批量赋值，sql语句会报错。
protected
 $attributes = [ 'name' =>
'value' ];
更新用associate
多对多保存用attach,
**更新中间表记录**updateExistingPivot
修改器mutators
当 Eloquent 尝试获取 first_name 的值时，将会自动调用访问器：
当我们尝试在模型上设置 first_name 的值时，将会自动调用此修改器
自动转换
protected$casts=['is_admin'=>'boolean',];
**临时修改属性的可见度**
return$user->makeVisible('attribute')->toArray();
return$user->makeHidden('attribute')->toArray();
也可以添加一个在数据库中没有对应字段的属性
去掉created_at 和 updated_at 字段
public$timestamps=false;
生成手机号
$phone_prefix_arr = array(
130,131,132,133,134,135,136,137,138,139,
144,147,
150,151,152,153,155,156,157,158,159,
176,177,178,
180,181,182,183,184,185,186,187,188,189,
);
'phone' => $phone_prefix_arr[array_rand($phone_prefix_arr)].mt_rand(1000,9999).mt_rand(1000,9999),

