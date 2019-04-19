# laravel学习之路2： jwt集成 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月11日 11:20:12[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：346
"tymon/jwt-auth":"^1.0@dev",
执行 composer update
'providers'=>[.... Tymon\JWTAuth\Providers\LaravelServiceProvider::class,// 上文已经提到过，这里的provider已经不是JWTauthServiceProvider],'aliases'=>[....'JWTAuth'=> Tymon\JWTAuth\Facades\JWTAuth::class],
发布配置文件[#](https://laravel-china.org/topics/2811/laravel-52-uses-jwt-to-complete-multi-user-authentication#%E5%8F%91%E5%B8%83%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)
php artisan vendor:publish
--provider="Tymon\JWTAuth\Providers\LaravelServiceProvider"
php artisan jwt:secret

*/***
** Get the value of the model's primary key.*
***
******@return****mixed*
**/*
**public function**getKey()
{
**return**$this->getAttribute($this->getKeyName());
}
这个一般是得到*id*

调用Auth::*guard*('your
 jwt guard name')->attempt($credentials)
实际是调用了JWTGuard.php里面的attempt方法
*oh yeah,终于生成了jwt token了*
{"register_result1":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwOi8vbGFyYXZlbF9hcGkuYXBwL2FwaS9yZWdpc3RlciIsImlhdCI6MTUwMjM1NjE1MCwiZXhwIjoxNTAyMzU5NzUwLCJuYmYiOjE1MDIzNTYxNTAsImp0aSI6ImJSSHZsUXB5ZzN1WGtTR2MiLCJzdWIiOjksInBydiI6IjM3ODdmYmExNjE4YTkzMDUyNmFjYTZjOGJiOWI0NGI4M2YyOTc3MjYifQ.JpqCVjZggb2BHsCEXzITdnX70HbYIAfQY-iYSpkfHSw"}
$credentials我感觉不应该先用bcrypt，不然attempt会验证失败，只有插入数据库的时候才需要bcrypt

**protected function**getClaimsForSubject(JWTSubject
$subject)
{
**return**[
'sub'
=>
$subject->getJWTIdentifier(),
 //主键id
'prv'
=>
$this->hashProvider($subject),
];
}
如何自定义customClaims
**public function**getJWTCustomClaims()
{
//
*TODO: Implement getJWTCustomClaims() method.*
**return**['key1'
=>
'elesos',
'key2'
=>
'test'];
}
上面是静态的，如何动态的加呢？
$client
= DB::*select*('select
* from clients where email = ?',
[$email]);
//return $client;
//var_dump($client);return;//数组里面是对象元素
$customClaims
= ['name'
=>
$client[0]->name,
'vip_level'
=>
'1'];
Auth::guard('client')->customClaims($customClaims);

接口如何访问
Route::*get*('/test1',
**function**() {
**return**['state'
=>
1,
'data'
=>
'sucess'];
})->middleware('auth:client');
或者
http://api.mysite.com/me?token={yourtokenhere}
验证token信息。
**public function**validate_test(){
//echo 'validate_test';
//$token = JWTAuth::getToken();
//return $token;
**try**{
**if**(!
$user
= JWTAuth::*parseToken*()->authenticate())
 {
**return**response()->json(['user_not_found'],
404);
}
}
**catch**(Tymon\JWTAuth\Exceptions\TokenExpiredException
$e) {
**return**response()->json(['token_expired'],
$e->getStatusCode());
}
**catch**(Tymon\JWTAuth\Exceptions\TokenInvalidException
$e) {
**return**response()->json(['token_invalid'],
$e->getStatusCode());
}
**catch**(Tymon\JWTAuth\Exceptions\JWTException
$e) {
**return**response()->json(['token_absent'],
$e->getStatusCode());
}
// the token
 is valid and we have found the user via the sub claim
**return**response()->json(compact('user'));
}
下一步要实现错误时返回json,而不是错误页面
Add the following code to the render method within app/Exceptions/Handler.php
publicfunctionrender($request,
Exception $e){if
 ($e instanceofTymon\JWTAuth\Exceptions\TokenExpiredException)
 {return response()->json(['token_expired'],
 $e->getStatusCode());}
elseif ($e
instanceofTymon\JWTAuth\Exceptions\TokenInvalidException)
 {return response()->json(['token_invalid'],
 $e->getStatusCode());}returnparent::render($request, $e);}
或全部
// 这是我自己错定义的错误
return response()->json(array('error_code' => $e->getStatusCode()));
// 这是默认的错误返回，已注释了
//return parent::render($request, $e);

**return**response()->json(['errcode'
=>
4000,
'errmsg'
=>
$exception->getMessage()],
200);

开发环境，当 APP_DEBUG
 = true 时，使用默认错误页面；
生产环境，当 APP_DEBUG = false 时，使用自定义错误页面，异步请求返回json异常信息
修改app/Exceptions/Handler.php
- 
public function render($request, Exception $exception)
- 
{
- 
$debug = config('app.debug', false);
- 
if($debug) {
- 
return parent::render($request, $exception);
- 
}
- 
if ($exception instanceof HttpException) {
- 
$code = $exception->getStatusCode();
- 
$message = $exception->getMessage();
- 
if ($request->expectsJson()) {
- 
return response()->json(['error' => $message], $code);
- 
}
- 
if (view()->exists('errors.custom' . $code)) {
- 
return response()->view('errors.custom' . $code, ['message'=>$message], $code);
- 
}
- 
}
- 
return parent::render($request, $exception);
- 
}

已知laravel5的默认Exceptions\Handler会优先匹配404异常，所以建议在Handler进行处理。
修改app/Exceptions/Handler.php的render方法如下
/** * Render an exception into an HTTP response. * *
@param \Illuminate\Http\Request $request *
@param \Exception $exception *
@return \Illuminate\Http\Response */**public****function****render**($request, Exception $exception){
**if** (is_a($exception, \Symfony\Component\HttpKernel\**Exception**\NotFoundHttpException::class)
 && $request->expectsJson()) { **return** response()->json(['msg'=>'NotFound']);
 } **else** {
**return****parent**::render($request, $exception); }}


[https://laravel-china.org/topics/2811/laravel-52-uses-jwt-to-complete-multi-user-authentication](https://laravel-china.org/topics/2811/laravel-52-uses-jwt-to-complete-multi-user-authentication)
