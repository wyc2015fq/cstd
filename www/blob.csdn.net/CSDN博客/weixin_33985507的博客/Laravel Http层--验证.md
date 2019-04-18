# Laravel Http层--验证 - weixin_33985507的博客 - CSDN博客
2017年08月14日 14:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
## 1、简介
**Laravel** 提供了多种方法来[验证](https://link.jianshu.com?t=http://laravelacademy.org/tags/%E9%AA%8C%E8%AF%81)应用输入数据。默认情况下，Laravel 的控制器基类使用`ValidatesRequests`
trait，该`trait`提供了便利的方法通过各种功能强大的验证[规则](https://link.jianshu.com?t=http://laravelacademy.org/tags/%E8%A7%84%E5%88%99)来验证输入的 HTTP [请求](https://www.jianshu.com/p/3856cd83b350)。
## 2、快速入门
要掌握 Laravel 强大的验证特性，让我们先看一个完整的验证[表单](https://link.jianshu.com?t=http://laravelacademy.org/tags/%E8%A1%A8%E5%8D%95)并返回[错误](https://link.jianshu.com?t=http://laravelacademy.org/tags/%E9%94%99%E8%AF%AF)信息给用户的例子。
### 定义路由
首先，我们假定在 `routes/web.php` 文件中包含如下路由：
```php
// 显示创建博客文章表单...
Route::get('post/create', 'PostController@create');
// 存储新的博客文章...
Route::post('post', 'PostController@store');
```
当然，`GET` 路由为用户显示了一个创建新的博客文章的表单，`POST` 路由将新的博客文章存储到数据库。
### 创建控制器
接下来，让我们看一个处理这些路由的简单控制器示例。我们先将 `store` 方法留空：
```php
<?php
namespace App\Http\Controllers;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
class PostController extends Controller
{
    /**
     * 显示创建新的博客文章的表单
     *
     * @return Response
     */
    public function create()
    {
        return view('post.create');
    }
    /**
     * 存储新的博客文章
     *
     * @param  Request  $request
     * @return Response
     */
    public function store(Request $request)
    {
        // 验证并存储博客文章...
    }
}
```
### 编写验证逻辑
现在我们准备用验证新博客文章输入的逻辑填充 `store` 方法。如果你检查应用的控制器基类（`App\Http\Controllers\Controller`），你会发现该类使用了`ValidatesRequests` trait，这个`trait` 在所有控制器中提供了一个便利的 `validate` 方法。
`validate` 方法接收一个 **HTTP 请求**输入和一系列验证规则，如果验证规则通过，代码将会继续往下执行；不过，如果验证失败，将会抛出一个异常，相应的错误响应也会自动发送给用户。在这个传统的 **HTTP 请求** 案例中，将会生成一个重定向响应，如果是 **AJAX** 请求则会返回一个 **JSON** 响应。
要更好的理解 `validate` 方法，让我们回到 `store` 方法：
```php
/**
 * 存储博客文章
 *
 * @param  Request  $request
 * @return Response
 */
public function store(Request $request){
    $this->validate($request, [
        'title' => 'required|unique:posts|max:255',
        'body' => 'required',
    ]);
    // 验证通过，存储到数据库...
}
```
正如你所看到的，我们只是传递输入的 HTTP 请求和期望的验证规则到 `validate` 方法。再强调一次，如果验证失败，相应的响应会自动生成。如果验证通过，控制器将会继续正常执行。
**首次验证失败后中止后续规则验证**
有时候你可能想要在首次验证失败后停止检查该属性的其它验证规则，要实现这个功能，可以在属性中分配 `bail` 规则：
```php
$this->validate($request, [
    'title' => 'bail|required|unique:posts|max:255',
    'body' => 'required',]
);
```
在这个例子中，如果 `title` 属性上的 `required` 规则验证失败，则不会检查`unique` 规则，规则会按照分配顺序依次进行验证。
**嵌套属性注意事项**
如果 HTTP 请求中包含“嵌套”参数，可以使用“.”在验证规则中指定它们：
```php
$this->validate($request, [
    'title' => 'required|unique:posts|max:255',
    'author.name' => 'required',
    'author.description' => 'required',
]);
```
### 显示验证错误信息
那么，如果请求输入参数没有通过给定验证规则怎么办？正如前面所提到的，Laravel 将会自动将用户重定向回上一个位置。此外，所有验证错误信息会自动 一次性存放到 `Session`。
注意我们并没有在 `GET` 路由中明确绑定错误信息到视图。这是因为 Laravel 总是从 `Session` 数据中检查错误信息，而且如果有的话会自动将其绑定到视图。所以，值得注意的是每次请求的所有视图中总是存在一个`$errors` 变量，从而允许你在视图中方便而又安全地使用。`$errors` 变量是一个`Illuminate\Support\MessageBag` 实例。
> 
注：`$errors` 变量会通过 `web` 中间件组中的 `Illuminate\View\Middleware\ShareErrorsFromSession` 中间件绑定到视图，如果使用了该中间件，那么 `$errors` 变量在视图中总是有效，从而方便你随时使用。
所以，在我们的例子中，验证失败的话用户将会被重定向到控制器的 `create`方法，从而允许我们在视图中显示错误信息：
```php
<!-- /resources/views/post/create.blade.php -->
<h1>Create Post</h1>
@if (count($errors) > 0)
    <div class="alert alert-danger">
        <ul>
            @foreach ($errors->all() as $error)
                <li>{{ $error }}</li>
            @endforeach
        </ul>
    </div>
@endif
<!-- Create Post Form -->
```
### 可选字段注意事项
默认情况下，Laravel自带了 `TrimStrings` 和 `ConvertEmptyStringsToNull` 中间件，这两个中间件位于`App\Http\Kernel` 类 的全局中间件堆栈中，因为这个原因，你需要经常将“可选”的请求字段标记为 `nullable` ，如果你不想让验证器将 `null` 判定为无效的话。例如：
```php
$this->validate(
        $request,
       [
           'title' => 'required|unique:posts|max:255',
           'body' => 'required',
           'publish_at' => 'nullable|date',
      ]
);
```
在这个例子中，我们指定 `publish_at` 字段可以为 `null` 或者有效的日期格式。如果 `nullable`
没有被添加到验证规则，验证器会将 `null` 判定为无效日期。
**自定义错误格式**
如果你想要自定义保存在 **Session** 中的验证错误信息的格式，需要在控制器基类中重写`formatValidationErrors` 方法（不要忘了在该控制器类的顶部导入 `Illuminate\Contracts\Validation\Validator`类）：
```php
<?php
namespace App\Http\Controllers;
use Illuminate\Foundation\Bus\DispatchesJobs;
use Illuminate\Contracts\Validation\Validator;
use Illuminate\Routing\Controller as BaseController;
use Illuminate\Foundation\Validation\ValidatesRequests;
abstract class Controller extends BaseController{
    use DispatchesJobs, ValidatesRequests;
    /**
     * {@inheritdoc}
     */
    protected function formatValidationErrors(Validator $validator)
    {
        return $validator->errors()->all();
    }
}
```
**AJAX请求&验证**
在这个例子中，我们使用传统的表单来发送数据到应用。然而，很多应用使用 **AJAX** 请求。在 **AJAX** 请求中使用 `validate` 方法时，**Laravel**  不会生成重定向响应。取而代之的，**Laravel**  生成一个包含验证错误信息的 **JSON** 响应。该 **JSON** 响应会带上一个 **HTTP 状态码**`422`。
## 3、表单请求验证
### 创建表单请求
对于更复杂的验证场景，你可能想要创建一个“表单请求”。表单请求是包含验证逻辑的自定义请求类，要创建表单验证类，可以使用`Artisan` 命令 `make:request`：
```
php artisan make:request StoreBlogPost
```
生成的类位于 `app/Http/Requests` 目录下，如果该目录不存在，运行 `make:request` 命令时会替我们生成。接下来我们添加少许验证规则到 `rules` 方法：
```php
/**
 * 获取应用到请求的验证规则
 *
 * @return array
 */
public function rules(){
    return [
        'title' => 'required|unique:posts|max:255',
        'body' => 'required',
    ];
}
```
那么，验证规则如何生效呢？你所要做的就是在控制器方法中类型提示该请求。表单输入请求会在控制器方法被调用之前被验证，这就是说你不需要将控制器和验证逻辑杂糅在一起：
```php
/**
 * 存储输入的博客文章
 *
 * @param  StoreBlogPostRequest  $request
 * @return Response
 */
public function store(StoreBlogPost $request){
    // The incoming request is valid...
}
```
如果验证失败，重定向响应会被生成并将用户退回上一个位置，错误信息也会被存储到一次性 **Session** 以便在视图中显示。如果是 **AJAX** 请求，带 `422` 状态码的 **HTTP** 响应将会返回给用户，该响应数据中还包含了 **JSON** 格式的验证错误信息。
**添加验证后钩子到表单请求**
如果你想要添加“验证后”钩子到表单请求，可以使用 `withValidator` 方法。该方法接收完整的构造验证器，从而允许你在验证规则执行前调用任何验证器方法：
```php
/**
 * 配置验证器实例.
 *
 * @param  \Illuminate\Validation\Validator  $validator
 * @return void
 */
public function withValidator($validator)
{
    $validator->after(function ($validator) {
        if ($this->somethingElseIsInvalid()) {
            $validator->errors()->add('field', 'Something is wrong with this field!');
        }
    });
}
```
### 授权 表单请求
表单请求类还包含了一个 `authorize` 方法，你可以检查认证用户是否有权限更新指定资源。例如，如果用户尝试更新一个博客评论，那么他是否是该评论的所有者呢？举个例子：
```php
/**
 * 判定用户是否被授权发起请求.
 *
 * @return bool
 * @translator laravelacademy.org
 */
public function authorize()
{
    $comment = Comment::find($this->route('comment'));
    return $comment && $this->user()->can('update', $comment);
}
```
由于所有请求都继承自 **Laravel** 请求基类，我们可以使用`user` 方法获取当前认证用户，还要注意上面这个例子中对`route` 方法的调用。该方法赋予用户访问被调用路由 **URI** 参数的权限，比如下面这个例子中的 `{comment}` 参数：
```php
Route::post('comment/{comment}');
```
如果 `authorize` 方法返回 `false`，一个包含 `403` 状态码的 **HTTP** 响应会自动返回而且控制器方法将不会被执行。
如果你计划在应用的其他部分包含授权逻辑，只需在`authorize` 方法中简单返回 `true` 即可：
```php
/**
 * 判断请求用户是否经过认证
 *
 * @return bool
 */
public function authorize(){
    return true;
}
```
### 自定义错误格式
如果你想要自定义验证失败时存储到一次性 **Session** 中验证错误信息的格式，重写请求基类（`App\Http\Requests\Request`）中的 `formatErrors` 方法即可。不要忘记在文件顶部导入 `Illuminate\Contracts\Validation\Validator` 类：
```php
/**
 * {@inheritdoc}
 */
protected function formatErrors(Validator $validator){
    return $validator->errors()->all();
}
```
### 自定义错误消息
你可以通过重写 `messages` 方法自定义表单请求使用的错误消息，该方法应该返回属性/规则对数组及其对应错误消息：
```php
/**
 * 获取被定义验证规则的错误消息.
 *
 * @return array
 * @translator laravelacademy.org
 */
public function messages(){
    return [
        'title.required' => 'A title is required',
        'body.required'  => 'A message is required',
    ];
}
```
## 4、 手动创建验证器
如果你不想使用 `ValidatesRequests` trait提供的`validate` 方法，可以使用 `Validator`[Facade](https://www.jianshu.com/p/a96715975d4e)手动创建一个验证器实例，该门面上的 `make` 方法用于生成一个新的验证器实例：
```php
<?php
namespace App\Http\Controllers;
use Validator;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
class PostController extends Controller{
    /**
     * 存储新的博客文章
     *
     * @param  Request  $request
     * @return Response
     */
    public function store(Request $request)
    {
        $validator = Validator::make($request->all(), [
            'title' => 'required|unique:posts|max:255',
            'body' => 'required',
        ]);
        if ($validator->fails()) {
            return redirect('post/create')
                        ->withErrors($validator)
                        ->withInput();
        }
        // 存储博客文章...
    }
}
```
传递给 `make` 方法的第一个参数是需要验证的数据，第二个参数是要应用到数据上的验证规则。
检查请求是够通过验证后，可以使用 `withErrors` 方法将错误数据存放到一次性 **Session**，使用该方法时，`$errors` 变量重定向后自动在视图间共享，从而允许你轻松将其显示给用户，`withErrors` 方法接收一个验证器、或者一个 `MessageBag` ，又或者一个 **PHP** 数组。
### 自动重定向
如果你想要手动创建一个验证器实例，但仍然使用`ValidatesRequest` trait提供的自动重定向，可以调用已存在验证器上的 `validate` 方法，如果验证失败，用户将会被自动重定向，或者，如果是 **AJAX** 请求的话，返回 **JSON**  响应：
```php
Validator::make($request->all(), [
    'title' => 'required|unique:posts|max:255',
    'body' => 'required',
])->validate();
```
### 命名错误包
如果你在单个页面上有多个表单，可能需要命名错误的 `MessageBag`，从而允许你为指定表单获取错误信息。只需要传递名称作为第二个参数给 `withErrors` 即可：
```php
return redirect('register')
    ->withErrors($validator, 'login');
然后你就可以从 $errors 变量中访问命名的 MessageBag 实例：
{{ $errors->login->first('email') }}
```
### 验证钩子之后
验证器允许你在验证完成后添加回调，这种机制允许你轻松执行更多验证，甚至添加更多错误信息到消息集合。使用验证器实例上的 `after` 方法即可：
```php
$validator = Validator::make(...);
$validator->after(function($validator) {
    if ($this->somethingElseIsInvalid()) {
        $validator->errors()->add('field', 'Something is wrong with this field!');
    }
});
if ($validator->fails()) {
    //
}
```
## 5、处理错误信息
调用 **Validator** 实例上的 `errors` 方法之后，将会获取一个 `Illuminate\Support\MessageBag` 实例，该实例中包含了多种处理错误信息的便利方法。在所有视图中默认有效的 `$errors` 变量也是一个 `MessageBag` 实例。
**获取某字段的第一条错误信息**
要获取指定字段的第一条错误信息，可以使用 `first` 方法：
```php
$errors = $validator->errors();
echo $errors->first('email');
```
**获取指定字段的所有错误信息**
如果你想要简单获取指定字段的所有错误信息数组，使用 `get` 方法：
```php
foreach ($errors->get('email') as $message) {
    //
}
```
如果是一个数组表单字段，可以使用 `*` 获取所有数组元素错误信息：
```php
foreach ($errors->get('attachments.*') as $message) {
    //
}
```
**获取所有字段的所有错误信息**
要获取所有字段的所有错误信息，可以使用 `all` 方法：
```php
foreach ($errors->all() as $message) {
    //
}
```
**判断消息中是否存在某字段的错误信息**
`has` 方法可用于判断错误信息中是否包含给定字段：
``php
if ($errors->has(’email’)) {
//
}
```
**获取指定格式的错误信息**
```php
echo $errors->first(’email’, ‘
:message
‘);
```
**获取指定格式的所有错误信息**
```php
foreach ($errors->all('<li>:message</li>') as $message) {
    //
}
```
### 自定义错误信息
如果需要的话，你可以使用自定义错误信息替代默认的，有多种方法来指定自定义信息。首先，你可以传递自定义信息作为第三个参数给 `Validator::make` 方法：
```php
$messages = [
    'required' => 'The :attribute field is required.',
];
$validator = Validator::make($input, $rules, $messages);
```
在本例中，`:attribute` 占位符将会被验证时实际的字段名替换，你还可以在验证消息中使用其他占位符，例如：
```php
$messages = [
    'same'    => 'The :attribute and :other must match.',
    'size'    => 'The :attribute must be exactly :size.',
    'between' => 'The :attribute must be between :min - :max.',
    'in'      => 'The :attribute must be one of the following types: :values',
];
```
**为给定属性指定自定义信息**
有时候你可能只想为特定字段指定自定义错误信息，可以通过`“.”`来实现，首先指定属性名，然后是规则：
```php
$messages = [
    'email.required' => 'We need to know your e-mail address!',
];
```
**在语言文件中指定自定义消息**
在很多案例中，你可能想要在语言文件中指定自定义消息而不是将它们直接传递给 **Validator**。要实现这个，添加消息到 `resources/lang/xx/validation.php` 语言文件的`custom` 数组：
```php
'custom' => [
    'email' => [
        'required' => 'We need to know your e-mail address!',
    ],
],
```
**在语言文件中指定自定义属性**
如果你想要将验证消息的 `:attribute` 部分替换成自定义属性名称，可以在语言文件 `resources/lang/xx/validation.php` 的 `attributes` 数组中指定自定义名称：
```php
'attributes' => [
    'email' => 'email address',
],
```
## 6、验证规则大全
下面是有效规则及其函数列表：
- Accepted
- Active URL
- After (Date)
- After Or Equal（Date）
- Alpha
- Alpha Dash
- Alpha Numeric
- Array
- Before (Date)
- Before Or Equal（Date）
- Between
- Boolean
- Confirmed
- Date
- Date Format
- Different
- Digits
- Digits Between
- Dimensions（图片文件）
- Distinct
- E-Mail
- Exists (Database)
- File
- Filled
- Image (File)
- In
- In Array
- Integer
- IP Address
- JSON
- Max
- MIME Types (File)
- MIME Type By File Extension
- Min
- Nullable
- Not In
- Numeric
- Present
- Regular Expression
- Required
- Required If
- Required Unless
- Required With
- Required With All
- Required Without
- Required Without All
- Same
- Size
- String
- Timezone
- Unique (Database)
- URL
**accepted**
验证字段的值必须是*yes*、*on*、*1*或*true*，这在“同意服务协议”时很有用。
**active_url**
验证字段必须是基于 PHP 函数 `dns_get_record` 的，有A 或 AAAA 记录的值。
**after:date**
验证字段必须是给定日期之后的一个值，日期将会通过 PHP 函数 `strtotime`
传递：
```php
'start_date' => 'required|date|after:tomorrow'
```
你可以指定另外一个与日期进行比较的字段，而不是传递一个日期字符串给 `strtotime`
执行：
```php
'finish_date' => 'required|date|after:start_date'
```
**after_or_equal:date**
验证字段必须是大于等于给定日期的值，更多信息，请参考`after:date`
规则。
**alpha**
验证字段必须是字母。
**alpha_dash**
验证字段可以包含字母和数字，以及破折号和下划线。
**alpha_num**
验证字段必须是字母或数字。
**array**
验证字段必须是 PHP 数组。
**before:date**
验证字段必须是指定日期之前的一个数值，日期将会传递给**PHP**`strtotime`
函数。
**before_or_equal:date**
验证字段必须小于等于给定日期。日期将会传递给 **PHP** 的 `strtotime`
函数。
**between:min,max**
验证字段大小在给定的最小值和最大值之间，字符串、数值和文件都可以像使用 `size`
规则一样使用该规则。
**boolean**
验证字段必须可以被转化为布尔值，接收`true`, false, 1 , 0, "1" 和 "0" 等输入。
**confirmed**
验证字段必须有一个匹配字段 `foo_confirmation`，例如，如果验证字段是`password`，必须输入一个与之匹配的`password_confirmation` 字段。
**date**
验证字段必须是一个基于 **PHP**`strtotime`函数的有效日期
**date_format:format**
验证字段必须匹配指定格式，可以使用 **PHP** 函数`date` 或 `date_format` 验证该字段。
**different:field**
验证字段必须是一个和指定字段不同的值。
**digits:value**
验证字段必须是数字且长度为`value`指定的值。
**digits_between:min,max**
验证字段数值长度必须介于最小值和最大值之间。
**dimensions**
验证的图片尺寸必须满足该规定参数指定的约束条件：
```php
'avatar' => 'dimensions:min_width=100,min_height=200'
```
有效的约束条件包括：*min_width*, *max_width*, *min_height*, *max_height*, *width*, *height*, *ratio*。*ratio* 约束应该是宽度/高度，这可以通过表达式3/2或浮点数1.5 来表示：
```php
'avatar' => 'dimensions:ratio=3/2'
```
由于该规则要求多个参数，可以使用 `Rule::dimensions`方法来构造该规则：
```php
use Illuminate\Validation\Rule;Validator::make($data, [ 'avatar' => [ 'required', Rule::dimensions()->maxWidth(1000)->maxHeight(500)->ratio(3 / 2), ],]);
```
**distinct**
处理数组时，验证字段不能包含重复值：
```
'foo.*.id' => 'distinct'
```
**email**
验证字段必须是格式化的电子邮件地址
**exists:table,column**
验证字段必须存在于指定数据表基本使用：
```
'state' => 'exists:states'
```
指定自定义列名：
```
'state' => 'exists:states,abbreviation'
```
还可以添加更多查询条件到`where` 查询子句：
```
'email' => 'exists:staff,email,account_id,1'
```
这些条件还可以包含`!`：
```
'email' => 'exists:staff,email,role,!admin'
```
还可以传递 `NULL` 或 `NOT NULL` 到 `where` 子句：
```
'email' => 'exists:staff,email,deleted_at,NULL'
'email' => 'exists:staff,email,deleted_at,NOT_NULL'
```
有时，你可能需要为 `exists` 查询指定要使用的数据库连接，这可以通过在表名前通过`.`前置数据库连接来实现：
```
'email' => 'exists:connection.staff,email'
```
如果你想要自定义验证规则执行的查询，可以使用 `Rule` 类来定义规则。在这个例子中，我们还以数组形式指定了验证规则，而不是使用 `|` 字符来限定它们：
```php
use Illuminate\Validation\Rule;
Validator::make($data, [
    'email' => [
        'required',
        Rule::exists('staff')->where(function ($query) {
            $query->where('account_id', 1);
        }),
    ],
]);
```
**file**
验证字段必须是上传成功的文件。
**filled**
验证字段如果存在则不能为空。
**image**
验证文件必须是图片（jpeg、png、bmp、gif或者svg）
**in:foo,bar…**
验证字段值必须在给定的列表中，由于该规则经常需要我们对数组进行`implode`，可以使用`Rule::in` 来构造这个规则：
```php
use Illuminate\Validation\Rule;
Validator::make($data, [
    'zones' => [
        'required',
        Rule::in(['first-zone', 'second-zone']),
    ],
]);
```
**in_array:另一个字段**
验证字段必须在另一个字段值中存在。
**integer**
验证字段必须是整型。
**ip**
验证字段必须是IP地址。
**ipv4**
验证字段必须是IPv4地址。
**ipv6**
验证字段必须是IPv6地址。
**json**
验证字段必须是有效的JSON字符串
**max:value**
验证字段必须小于等于最大值，和字符串、数值、文件字段的`size` 规则一样使用
**mimetypes：text/plain…**
验证文件必须匹配给定的 `MIME`文件类型之一：
```php
'video' => 'mimetypes:video/avi,video/mpeg,video/quicktime'
```
为了判断上传文件的 MIME 类型，框架将会读取文件内容来猜测 MIME 类型，这可能会和客户端 MIME 类型不同。
**mimes:foo,bar,…**
验证文件的 MIME 类型必须是该规则列出的 扩展 类型中的一个 MIME规则的基本使用：
```php
'photo' => 'mimes:jpeg,bmp,png'
```
尽管你只需要指定扩展，该规则实际上验证的是通过读取文件内容获取到的文件`MIME`类型。
完整的`MIME`类型列表及其相应的扩展可以在这里找到：[http://svn.apache.org/repos/asf/httpd/httpd/trunk/docs/conf/mime.types](https://link.jianshu.com?t=http://svn.apache.org/repos/asf/httpd/httpd/trunk/docs/conf/mime.types)
**min:value**
验证字段必须大于等于最小值，对字符串、数值、文件字段而言，和`size` 规则使用方式一致。
**nullable**
验证字段可以是`null`，这在验证一些可以为 `null` 的原生数据如整型或字符串时很有用。
**not_in:foo,bar,…**
验证字段值不能在给定列表中
**numeric**
验证字段必须是数值
**present**
验证字段必须出现在输入数据中但可以为空。
**regex:pattern**
验证字段必须匹配给定正则表达式
> 
注：使用 `regex` 模式时，规则必须放在数组中，而不能使用管道分隔符，尤其是正则表达式中已经使用了管道符号时。
**required**
验证字段值不能为空，以下情况字段值都为空：
- 值为`null`
- 值是空字符串
- 值是空数组或者空的`Coutable`对象
- 值是上传文件但路径为空
**required_if:anotherfield,value,…**
验证字段在另一个字段等于指定值*value*时是必须的
**required_unless:anotherfield,value,…**
除非 *anotherfield* 字段等于*value*，否则验证字段不能空
**required_with:foo,bar,…**
验证字段只有在任一其它指定字段存在的情况才是必须的
**required_with_all:foo,bar,…**
验证字段只有在所有指定字段存在的情况下才是必须的
**required_without:foo,bar,…**
验证字段只有当任一指定字段不存在的情况下才是必须的
**required_without_all:foo,bar,…**
验证字段只有当所有指定字段不存在的情况下才是必须的
**same:field**
给定字段和验证字段必须匹配
**size:value**
验证字段必须有和给定值*value*相匹配的尺寸，对字符串而言，*value*是相应的字符数目；对数值而言，*value*是给定整型值；对文件而言，*value*是相应的文件字节数
**string**
验证字段必须是字符串，如果允许字段为空，需要分配`nullable` 规则到该字段。
**timezone**
验证字符必须是基于 PHP 函数`timezone_identifiers_list` 的有效时区标识
**unique:table,column,except,idColumn**
验证字段在给定数据表上必须是唯一的，如果不指定`column` 选项，字段名将作为默认`column`。
指定自定义列名：
```
'email' => 'unique:users,email_address'
```
**自定义数据库连接**
有时候，你可能需要自定义验证器生成的数据库连接，正如上面所看到的，设置`unique:users` 作为验证规则将会使用默认数据库连接来查询数据库。要覆盖默认连接，在数据表名后使用“.”指定连接：
```
'email' => 'unique:connection.users,email_address'
```
**强制一个唯一规则来忽略给定ID：**
有时候，你可能希望在唯一检查时忽略给定ID，例如，考虑一个包含用户名、邮箱地址和位置的”更新属性“界面，当然，你将会验证邮箱地址是唯一的，然而，如果用户只改变用户名字段而并没有改变邮箱字段，你不想要因为用户已经拥有该邮箱地址而抛出验证错误，你只想要在用户提供的邮箱已经被别人使用的情况下才抛出验证错误。
要告诉验证器忽略用户`ID`，可以使用 `Rule` 类类定义这个规则，我们还要以数组方式指定验证规则，而不是使用`|`来界定规则：
```php
use Illuminate\Validation\Rule;
Validator::make($data, [
    'email' => [
        'required',
        Rule::unique('users')->ignore($user->id),
    ],
]);
```
如果你的数据表使用主键字段不是id，可以在调用`ignore` 方法的时候指定字段名称：
```php
'email' => Rule::unique('users')->ignore($user->id, 'user_id')
```
**添加额外的where子句：**
使用`where`方法自定义查询的时候还可以指定额外查询约束，例如，下面我们来添加一个验证`account_id`为1 的约束：
```php
'email' => Rule::unique('users')->where(function ($query) {
    $query->where('account_id', 1);
})
```
**url**
验证字段必须是有效的URL。
## 7、添加条件规则
**存在时验证**
在某些场景下，你可能想要只有某个字段存在的情况下进行验证检查，要快速实现这个，添加 `sometimes` 规则到规则列表：
```php
$v = Validator::make($data, [
    'email' => 'sometimes|required|email',
]);
```
在上例中，`email` 字段只有存在于 `$data` 数组时才会被验证。
**复杂条件验证**
有时候你可能想要基于更复杂的条件逻辑添加验证规则。例如，你可能想要只有在另一个字段值大于100时才要求一个给定字段是必须的，或者，你可能需要只有当另一个字段存在时两个字段才都有给定值。添加这个验证规则并不是一件头疼的事。首先，创建一个永远不会改变的静态规则到 **Validator** 实例：
```php
$v = Validator::make($data, [
    'email' => 'required|email',
    'games' => 'required|numeric',
]);
```
让我们假定我们的 Web 应用服务于游戏收集者。如果一个游戏收集者注册了我们的应用并拥有超过 100 个游戏，我们想要他们解释为什么他们会有这么多游戏，例如，也许他们在运营一个游戏二手店，又或者他们只是喜欢收集。要添加这种条件，我们可以使用 `Validator` 实例上的 `sometimes`方法：
```php
$v->sometimes('reason', 'required|max:500', function($input) {
    return $input->games >= 100;
});
```
传递给 `sometimes` 方法的第一个参数是我们需要有条件验证的名称字段，第二个参数是我们想要添加的规则，如果作为第三个参数的闭包返回`true`，规则被添加。该方法让构建复杂条件验证变得简单，你甚至可以一次为多个字段添加条件验证：
```php
$v->sometimes(['reason', 'cost'], 'required', function($input) {
    return $input->games >= 100;
});
```
> 
注：传递给闭包的 `$input` 参数是 `Illuminate\Support\Fluent` 的一个实例，可用于访问输入和文件。
## 8、验证数组输入
验证表单数组输入字段不再是件痛苦的事情，例如，要验证给定数组输入中每个 `email` 是否是唯一的，可以这么做：
```php
$validator = Validator::make($request->all(), [
    'person.*.email' => 'email|unique:users',
    'person.*.first_name' => 'required_with:person.*.last_name',
]);
```
类似地，在语言文件中你也可以使用`*`字符指定验证消息，从而可以使用单个消息定义基于数组字段的验证规则：
```php
'custom' => [
    'person.*.email' => [
        'unique' => 'Each person must have a unique e-mail address',
    ]
],
```
## 9、自定义验证规则
Laravel 提供了多种有用的验证规则；不过，你可能还是想要指定一些自己的验证规则。注册验证规则的一种方法是使用`Validator`[Facade](https://www.jianshu.com/p/a96715975d4e)的 `extend` 方法。让我们在[服务提供者](https://www.jianshu.com/p/e62582697761)中使用这种方法来注册一个自定义的验证规则：
```php
<?php
namespace App\Providers;
use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Facades\Validator;
class AppServiceProvider extends ServiceProvider
{
    /**
     * 启动应用服务
     *
     * @return void
     */
    public function boot()
    {
        Validator::extend('foo', function($attribute, $value, $parameters, $validator) {
            return $value == 'foo';
        });
    }
    /**
     * 注册服务提供者
     *
     * @return void
     */
    public function register()
    {
        //
    }
}
```
自定义验证器闭包接收四个参数：要验证的属性名称、属性值、传递给规则的参数数组以及`Validator`
实例。你还可以传递类和方法到 `extend` 方法而不是闭包：
```php
Validator::extend('foo', 'FooValidator@validate');
```
**定义错误信息**
你还需要为自定义规则定义错误信息。你可以使用内联自定义消息数组或者在验证语言文件中添加条目来实现这一目的。消息应该被放到数组的第一维，而不是在只用于存放属性指定错误信息的 custom
数组内：
```php
"foo" => "Your input was invalid!",
"accepted" => "The :attribute must be accepted.",
// 验证错误信息其它部分...
```
当创建一个自定义验证规则时，你可能有时候需要为错误信息定义自定义占位符，可以通过创建自定义验证器然后调用`Validator` Facade上的`replacer` 方法来实现。可以在 服务提供者 的`boot` 方法中编写代码：
```php
/**
 * 启动应用服务
 *
 * @return void
 * @translator laravelacademy.org
 */
public function boot(){
    Validator::extend(...);
    Validator::replacer('foo', function($message, $attribute, $rule, $parameters) {
        return str_replace(...);
    });
}
```
**隐式扩展**
默认情况下，被验证的属性如果没有提供或者验证规则为`required` 而值为空，那么正常的验证规则，包括自定义扩展将不会执行。例如 `unique`规则将不会检验`null` 值：
```php
$rules = ['name' => 'unique'];
$input = ['name' => null];
Validator::make($input, $rules)->passes();
 // true
```
如果要求即使为空时也要验证属性，则必须要暗示属性是必须的，要创建一个隐式扩展，可以使用`Validator::extendImplicit()` 方法：
```php
Validator::extendImplicit('foo', function($attribute, $value, $parameters, $validator) {
    return $value == 'foo';
});
```
> 
注：一个隐式扩展仅仅暗示属性是必须的，至于它到底是缺失的还是空值这取决于你。
