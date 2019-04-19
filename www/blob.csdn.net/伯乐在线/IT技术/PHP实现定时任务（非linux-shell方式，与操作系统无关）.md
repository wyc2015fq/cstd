# PHP实现定时任务（非linux-shell方式，与操作系统无关） - 文章 - 伯乐在线
原文出处： [大弹簧](https://my.oschina.net/u/3381391/blog/1510260)
页面交互效果
下面是写好的示例前端交互页面，主要是列表页面，编辑页面。
主要交互有：
1 开启定时任务进程
2 关闭定时任务进程
3 新增一项定时任务
4 编辑已有的定时任务
5 删除定时任务
6 开始一项定时任务
7 停止一项定时任务
定时任务分两种：
1 执行一次
2 循环执行
定时任务执行的内容：
定时任务执行的内容其实还是一个http请求。 通过定时的方式，指定时间执行或者循执行。
#### 1 列表页面
![图片名称](http://www.tkc8.com/images/sucai/img_crontab_1.png)
#### 2 新建\编辑页面
![图片名称](http://www.tkc8.com/images/sucai/img_crontab_2.png)
### 代码地址，演示地址
示例代码是基于Laravel，Alpaca-spa框架编写，并且作为 ‘Alpaca-Spa-Laravel后台管理平台’的一个模块儿集成于系统中。
代码 ： [http://git.oschina.net/cc-sponge/Alpaca-Spa-Laravel](http://git.oschina.net/cc-sponge/Alpaca-Spa-Laravel)
代码 ： [https://github.com/big-sponge/Alpaca-Spa-Laravel](https://github.com/big-sponge/Alpaca-Spa-Laravel)
Alpaca-Spa-Laravel ： [http://full.tkc8.com](http://full.tkc8.com/)
Alpaca-Spa ： [http://www.tkc8.com](http://www.tkc8.com/)
### 适用范围
定时精确时间不低于1秒。web服务重启、或者php重启。该定时任务不会自动重启。
### 实现原理
用PHP实现定时任务（非linux-shell方式，与操作系统平台无关），主要两个技术点：
1）PHP后台进程
2）PHP异步处理
#### 1 PHP后台进程
一般情况，用命令行可以开启一个php后台进程。而在浏览器中通过HTTP请求一个php处理，会因为浏览器关闭，或者请求超时，使得后台的php处理中断。但是php提供了两个方法可以会忽略浏览器关闭、请求超时：


```
ignore_user_abort(true);     // 忽略客户端断开
set_time_limit(0);           // 设置执行不超时
```
因此，我们可以使用这两个函数从浏览器以HTTP请求的方式开启一个php后台进程。
#### 2 PHP异步处理
php语言本身没有可以跨平台好用异步处理方法，但是可以通过curl或者fsockopen创建一个请求来实现异步处理。这里我们用fsockopen方法实现：


```
$fp = fsockopen("$ip", $port, $errno, $errstr,1);
        if (!$fp) {
            return 'worker error:'."$errstr ($errno)<br />\n";
        } else {
            $out = "POST $url HTTP/1.1\r\n";
            $out .= "Host: $ip\r\n";
            $out .= "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n";
            $out .= "Content-Length: " . strlen($postData) . "\r\n";
            $out .= "Connection: close\r\n";
            $out .="\r\n";
            $out .=$postData;
            fputs($fp, $out);
            fclose($fp);
        }
```
### 前台交互控制器
为了在前端界面方便控制定时任务的开启关闭，新增、编辑、删除，查看执行状态。 同时也可以添加权限控制。 主要有8个操作接口(下面是以Laravel 路由示例)：
1 查看定时任务进程状态
2 开始定时任务进程 3 停止定时任务进程
4 添加,或者编辑定时任务
5 设置定时任务状态
6 获取指定定时任务明细
7 删除定时任务
8 获取定时任务列表


```
/* crontab - status 查看定时任务守护进程状态 */
Route::any('crontab/status', "CrontabController@status");
/* crontab - start 开始定时任务 */
Route::any('crontab/start', "CrontabController@start");
/* crontab - stop  停止定时任务守护进程*/
Route::any('crontab/stop', "CrontabController@stop");
/* crontab - editTask  添加,或者编辑定时任务*/
Route::any('crontab/editTask', "CrontabController@editTask");
/* crontab - changeTaskStatus  设置定时任务状态 */
Route::any('crontab/changeTaskStatus', "CrontabController@changeTaskStatus");
/* crontab - getIndexTask  获取指定定时任务 */
Route::any('crontab/getIndexTask', "CrontabController@getIndexTask");
/* crontab - removeTask  删除定时任务 */
Route::any('crontab/removeTask', "CrontabController@removeTask");
/* crontab - listTask  获取定时任务列表 */
Route::any('crontab/listTask', "CrontabController@listTask");
```
完整的类代码如下：


```
<?php
namespace App\Modules\Manage\Controllers;
use Crontab\Library\Crontab\AlpacaCrontab;
use Crontab\Library\Crontab\AlpacaDaemon;
use Crontab\Library\Crontab\AlpacaWorker;
use App\Modules\Manage\Controllers\Base\BaseController;
use App\Common\Code;
use App\Common\Msg;
/**
 * 定时任务管理控制器
 * @author Chengcheng
 * @date 2016-10-19 15:50:00
 */
class CrontabController extends BaseController
{
    /**
     * 设置不需要登录的的Action,不加Action前缀
     * @author Chengcheng
     * @date   2016年10月23日 20:39:25
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    protected function noLogin()
    {
        return [];
    }
    /**
     * 设置不需要权限验证的Action,不加Action前缀
     * @author Chengcheng
     * @date   2016年10月23日 20:39:25
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    protected function noAuth()
    {
        // 以下Action不需要角色权限
        return [];
    }
    /**
     * 查看定时任务守护进程状态
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function status()
    {
        //查看守护进程状态
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = AlpacaDaemon::daemon()->status();
        //返回结果
        return $this->ajaxReturn($result);
    }
    /**
     * 开始定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function start()
    {
        //异步开启守护进程
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = AlpacaWorker::worker()->action(['REQUEST_URI' => "/crontab/index/start"]);
        //返回结果
        return $this->ajaxReturn($result);
    }
    /**
     * 停止定时任务守护进程
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function stop()
    {
        //停止守护进程
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = AlpacaDaemon::daemon()->stop();
        //返回结果
        return $this->ajaxReturn($result);
    }
    /**
     * 添加,或者编辑定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function editTask()
    {
        /*
         * 1 获取输入参数
         * BEGIN_TIME        开始时间
         * END_TIME          结束时间
         * INTERVAL          时间间隔
         * NAME              名称
         * STATUS            状态 1-ENABLED,   2-DISABLE
         * TASK_TYPE         类型 1-ONCE,      2-LOOP
         * ACTION            要执行的Action
         * INDEX             索引，null或者0时候，表示新建
         * */
        $this->requestData['NAME']       = $this->input('NAME', null);
        $this->requestData['BEGIN_TIME'] = $this->input('BEGIN_TIME', null);
        $this->requestData['END_TIME']   = $this->input('END_TIME', null);
        $this->requestData['INTERVAL']   = $this->input('INTERVAL', null);
        $this->requestData['TASK_TYPE']  = $this->input('TASK_TYPE', '1');
        $this->requestData['ACTION']     = $this->input('ACTION', null);
        $this->requestData['STATUS']     = $this->input('STATUS', '2');
        $this->requestData['INDEX']      = $this->input('INDEX', null);
        $this->requestData['LAST_TIME']  = $this->input('LAST_TIME', null);
        //2 检查参数
        if (empty($this->requestData['BEGIN_TIME'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'BEGIN_TIME');
            return $this->ajaxReturn($result);
        }
        if ($this->requestData['TASK_TYPE'] == 2 && empty($this->requestData['END_TIME'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'END_TIME');
            return $this->ajaxReturn($result);
        }
        if (empty($this->requestData['ACTION'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'ACTION');
            return $this->ajaxReturn($result);
        }
        if ($this->requestData['TASK_TYPE'] == 2 && empty($this->requestData['INTERVAL'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'INTERVAL');
            return $this->ajaxReturn($result);
        }
        //3 设置结束时间
        $now      = date('Y-m-d H:i:s', time());
        $nextTime = date('Y-m-d H:i:s', strtotime($this->requestData['INTERVAL'], strtotime($this->requestData['BEGIN_TIME'])));
        if ($this->requestData['TASK_TYPE'] == "1" || strtotime($now) < strtotime($this->requestData['BEGIN_TIME'])) {
            $nextTime = $this->requestData['BEGIN_TIME'];
        }
        //4 创建任务
        $task = array(
            'NAME'       => $this->requestData['NAME'],           //NAME
            'STATUS'     => $this->requestData['STATUS'],         // 1-ENABLED,   2-DISABLE
            'TYPE'       => $this->requestData['TASK_TYPE'],      // 1-ONCE,      2-LOOP
            'INTERVAL'   => $this->requestData['INTERVAL'],       //year（年），month（月），hour（小时）minute（分），second（秒）
            'BEGIN_TIME' => $this->requestData['BEGIN_TIME'],     //开始时间
            'NEXT_TIME'  => $nextTime,                            //下次执行时间
            'LAST_TIME'  => $this->requestData['LAST_TIME'],      //上次执行时间
            'ACTION'     => $this->requestData['ACTION'],         //执行的ACTION
            'END_TIME'   => $this->requestData['END_TIME'],       //截止时间2
        );
        //5 判断是新建还是修改
        if (empty($this->requestData['INDEX'])) {
            //新建
            $info = AlpacaCrontab::crontab()->addTask($task);
        } else {
            $this->requestData['INDEX'] -= 1;
            $info = AlpacaCrontab::crontab()->editTask($this->requestData['INDEX'], $task);
        }
        //5 返回结果
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = $info;
        return $this->ajaxReturn($result);
    }
    /**
     * 设置定时任务状态
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function changeTaskStatus()
    {
        /*
         * 1 获取输入参数
         * STATUS            状态 1-ENABLED,   2-DISABLE
         * INDEX             索引
         * */
        $this->requestData['STATUS'] = $this->input('STATUS', '2');
        $this->requestData['INDEX']  = $this->input('INDEX', null);
        //2 检查参数
        if (empty($this->requestData['STATUS'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'STATUS');
            return $this->ajaxReturn($result);
        }
        if (empty($this->requestData['INDEX'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'INDEX');
            return $this->ajaxReturn($result);
        }
        //3 修改状态
        $this->requestData['INDEX'] -= 1;
        $data = AlpacaCrontab::crontab()->editTaskStatus($this->requestData['INDEX'], $this->requestData['STATUS']);
        //4 返回结果
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = $data;
        return $this->ajaxReturn($result);
    }
    /**
     * 查找单条定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function getIndexTask()
    {
        /*
         * 1 获取输入参数
         * INDEX             索引
         * */
        $this->requestData['INDEX'] = $this->input('INDEX', null);
        //2 检查参数
        if (empty($this->requestData['INDEX'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'INDEX');
            return $this->ajaxReturn($result);
        }
        //3 删除
        $this->requestData['INDEX'] -= 1;
        $data = AlpacaCrontab::crontab()->getIndexTask($this->requestData['INDEX']);
        //4 返回结果
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = $data;
        return $this->ajaxReturn($result);
    }
    /**
     * 删除定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function removeTask()
    {
        /*
         * 1 获取输入参数
         * INDEX             索引
         * */
        $this->requestData['INDEX'] = $this->input('INDEX', null);
        //2 检查参数
        if (empty($this->requestData['INDEX'])) {
            $result["code"] = Code::SYSTEM_PARAMETER_NULL;
            $result["msg"]  = sprintf(Msg::SYSTEM_PARAMETER_NULL, 'INDEX');
            return $this->ajaxReturn($result);
        }
        //3 删除
        $this->requestData['INDEX'] -= 1;
        $data = AlpacaCrontab::crontab()->removeTask($this->requestData['INDEX']);
        //4 返回结果
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = $data;
        return $this->ajaxReturn($result);
    }
    /**
     * 查看定时任务列表
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function listTask()
    {
        //查找
        $data['task']   = AlpacaCrontab::crontab()->listTask();
        $data['total']  = count($data['task']);
        $data['status'] = AlpacaDaemon::daemon()->status();
        //返回结果
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = $data;
        return $this->ajaxReturn($result);
    }
}
```
### 实现后台进程类
使用 ignore_user_abort(true); set_time_limit(0); 可以从浏览器开启一个php后台进程。为了避免出现多个后台进程，需要借助一个配置来标识该后台进程是否已经启动


```
{"code":"1001","message":"Stop at:2017-02-24 11:29:43"}
```
当code是1001时候，表示后台进程未启动，这时通过http请求开启后台进程时，正常启动
当code是1000时候，表示后台进程已经启动，这时通过http请求开启后台进程时，不做任何操作，以为进程已经启动
当开启后台进程的请求到达后台时候，读取配置文件，如果code是1001，则启动进程，并且设置code为1000，保存配置文件。
当关闭后台进程的请求到达后台时候，读取配置文件，设置code为1001，保存配置文件。
后台进程在运行时候，每隔一秒读取配置文件，判断code状态，如果是1001，则结束执行； 如果是1000，则继续执行
完整类的代码如下：


```
<?php
namespace Crontab\Library\Crontab;
/**
 * 守护进程
 * @author Chengcheng
 * @date 2016年10月21日 17:04:44
 */
class AlpacaDaemon
{
    private $daemon_json = __DIR__ . '/deamon.json';
    private static $instance;
    private $events = [];
    public static function daemon()
    {
        return self::getInstance();
    }
    private static function getInstance()
    {
        if(!self::$instance){
            self::$instance = new self();
            self::$instance->daemon_json = base_path('storage') . '/crontab/deamon.json';
        }
        return self::$instance;
    }
    public function setDaemon($daemon_json)
    {
        $this->daemon_json = $daemon_json;
        return $this;
    }
    public function setEvents(array $events)
    {
        $this->events = $events;
        return $this;
    }
    public function status()
    {
        $data = json_decode(file_get_contents($this->daemon_json),true);
        if(empty($data)){
            $data = array();
        }
        return $data;
    }
    public function stop()
    {
        $data =new \stdClass();
        $data->code="1001";
        $data->message="Stop at:".date("Y-m-d H:i:s" ,time());
        file_put_contents($this->daemon_json,json_encode($data),LOCK_EX);
        $result["result_code"] = "1";
        $result["result_message"] = "操作成功";
        return $result;
    }
    public function start()
    {
        $data = json_decode(file_get_contents($this->daemon_json) , true);
        if(empty($data)){
            $data['code']="1001";
        }
        if($data['code'] == "1000" ){
            //die("Error - exit,   Already running !");
            return;
        }
        $data['code']="1000";
        $data['message']="Start";
        file_put_contents($this->daemon_json,json_encode($data),LOCK_EX);
        ignore_user_abort(true);     // 忽略客户端断开
        set_time_limit(0);           // 设置执行不超时
        while(true){
            $data = json_decode(file_get_contents($this->daemon_json) , true);
            if(empty($data) || empty($data['code']) || $data['code'] == "1001" ){
                break;
            }
            if(!empty($this->events)){
                foreach ($this->events as $e){
                    $e();
                }
            }
            $data['message'] = date("Y-m-d H:i:s" ,time())." : Working ...";
            file_put_contents($this->daemon_json, json_encode($data), LOCK_EX);
            sleep(1);
        }
        $this->stop();
    }
}
```
### 实现异步处理的类
完整类的代码如下：


```
<?php
namespace Crontab\Library\Crontab;
class AlpacaWorker
{        
    private static $instance;
    private $accessToken = '';
    public static function worker()
    {
        return self::getInstance();
    }
    private static function getInstance()
    {
        if(!self::$instance){
            self::$instance = new self();
            self::$instance->accessToken= 'VyKfohBbwlkTOqp2jvIWPW92';
        }
        return self::$instance;
    }
    public function action(array $worker = null)
    {
        //获取参数
        $ip   = empty($worker['SERVER_ADDR']) ? $_SERVER['SERVER_NAME'] : $worker['SERVER_ADDR'];     //服务器IP地址
        $port = empty($worker['SERVER_PORT']) ? $_SERVER['SERVER_PORT'] : $worker['SERVER_PORT'];     //服务器端口
        $url  = empty($worker['REQUEST_URI']) ? '/' :$worker['REQUEST_URI'];                          //服务器URL
        $data = empty($worker['REQUEST_DATA']) ? '' :$worker['REQUEST_DATA'];                         //请求参数
        //格式化请求参数
        $postData = "";
        $needChar = false;
        if(is_array($data)){
            foreach($data as $key => $val) {
                $postData .= ($needChar ? "&" : "") . urlencode($key) . "=" . urlencode($val);
                $needChar = true;
            }
        }else{
            $postData = $data;
        }
        $url=$url."?accessToken=".$this->accessToken;
        //使用fsockopen方式异步调用action
        $fp = fsockopen("$ip", $port, $errno, $errstr,1);
        if (!$fp) {
            return 'worker error:'."$errstr ($errno)<br />\n";
        } else {
            $out = "POST $url HTTP/1.1\r\n";
            $out .= "Host: $ip\r\n";
            $out .= "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n";
            $out .= "Content-Length: " . strlen($postData) . "\r\n";
            $out .= "Connection: close\r\n";
            $out .="\r\n";
            $out .=$postData;
            fputs($fp, $out);
            fclose($fp);
        }       
        return 'worker success!';
    }
}
```
### 定时任务处理类
定时任务处理类 主要是实现新增、编辑、删除定时任务，执行定时任务要处理的方法
定时任务的信息以json格式存放在下面的配置文件中


```
[{"NAME":"\u6d4b\u8bd5\u5b9a\u65f6\u4efb\u52a12","STATUS":"2","TYPE":"1","INTERVAL":"5 second","BEGIN_TIME":"2017-02-21 11:55:00","NEXT_TIME":"2017-02-21 11:55:00","LAST_TIME":null,"ACTION":"\/main\/crontab\/index2","END_TIME":"2017-02-10 15:55:00"},{"NAME":"TEST - log","STATUS":"2","TYPE":"2","INTERVAL":"5 second","BEGIN_TIME":"2017-08-10 09:00:53","NEXT_TIME":"2017-08-10 09:59:00","LAST_TIME":"2017-08-10 09:58:55","ACTION":"\/crontab\/task\/test","END_TIME":"2017-08-11 09:25:53"}]
```
主要的字段为：
INDEX 索引
BEGIN_TIME 开始时间
END_TIME 结束时间
INTERVAL 时间间隔
NAME 名称
STATUS 状态 1-ENABLED, 2-DISABLE
TASK_TYPE 类型 1-ONCE, 2-LOOP
ACTION 要执行的Action
类中主要有7个方法：
listTask() 查看定时任务列表 addTask() 添加定时任务 editTask() 编辑定时任务 editTaskStatus() 编辑定时任务状态 getIndexTask() 获取指定定时任务信息 removeTask() 删除定时任务 doTask() 执行定时任务指定的任务
完整类的代码如下：


```
<?php
namespace Crontab\Library\Crontab;
/**
 * 定时任务
 * @author Chengcheng
 * @date 2016年10月21日 17:04:44
 */
class AlpacaCrontab
{
    //定时任务文件
    private $task_json = __DIR__ .'/crontab.json';
    //单例
    private static $instance;
    //单例
    public static function crontab()
    {
        return self::getInstance();
    }
    //单例
    private static function getInstance()
    {
        if(!self::$instance){
            self::$instance = new self();
            self::$instance->task_json = base_path('storage') . '/crontab/crontab.json';
        }
        return self::$instance;
    }
    /**
     * 配置
     * @author Chengcheng
     * @param array $crontab
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function setConfig($crontab)
    {
        $this->task_json = $crontab;
        return $this;
    }
    /**
     * 查看定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function listTask()
    {
        $tasks = json_decode(file_get_contents($this->task_json));
        $i = 0;
        foreach ($tasks as $task)
        {
            $tasks[$i]->INTERVAL = $this->timeToStr($tasks[$i]->INTERVAL);
            $i++;
        }
        return $tasks;
    }
    /**
     * 添加定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function addTask($task)
    {
        $result["result_code"] = "1";
        $result["result_message"] = "添加成功";
        $tasks = json_decode(file_get_contents($this->task_json),true);
        $tasks[count($tasks)] = $task;
        file_put_contents($this->task_json, json_encode($tasks), LOCK_EX);
        return $result;
    }
    /**
     * 编辑定时任务
     * @author Chengcheng
     * @param string $index
     * @param string $task
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function editTask($index,$task)
    {
        $result["result_code"] = "1";
        $result["result_message"] = "修改成功";
        $tasks = json_decode(file_get_contents($this->task_json));
        $tasks[$index] = $task;
        file_put_contents($this->task_json, json_encode($tasks), LOCK_EX);
        return $result;
    }
    /**
     * 编辑定时任务状态
     * @author Chengcheng
     * @param string $index
     * @param string $status
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function editTaskStatus($index,$status)
    {
        $result_data["result_code"] = "1";
        $result_data["result_message"] = "修改状态成功[".$status."]";
        $tasks = json_decode(file_get_contents($this->task_json));
        $tasks[$index]->STATUS = $status;
        file_put_contents($this->task_json, json_encode($tasks), LOCK_EX);
        return $result_data;
    }
    /**
     * 获取定时任务
     * @author Chengcheng
     * @param string $index
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function getIndexTask($index)
    {
        $result_data["result_code"] = "1";
        $result_data["result_message"] = "获取任务成功【".$index."】";
        $tasks = json_decode(file_get_contents($this->task_json));
        $result_data["result_data"] = $tasks[$index];
        return $result_data;
    }
    /**
     * 删除定时任务
     * @author Chengcheng
     * @param string $index
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function removeTask($index)
    {
        $result_data["result_code"] = "1";
        $result_data["result_message"] = "删除任务【".$index."】成功";
        $tasks = json_decode(file_get_contents($this->task_json));
        array_splice($tasks, $index, 1);
        file_put_contents($this->task_json, json_encode($tasks), LOCK_EX);
        return $result_data;
    }
    /**
     * 执行定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    public function doTask()
    {
        $tasks = json_decode(file_get_contents($this->task_json) ,true);      
        if(empty($tasks)){ return ;}
        $now = date('Y-m-d H:i:s',time());
        foreach ($tasks as &$task){
            if(empty($task['STATUS']) || empty($task['TYPE'])  || empty($task['BEGIN_TIME']) || empty($task['ACTION']) )
            {
                continue;
            }
            if($task['STATUS'] != 1)
            {
                continue;
            }
            if(!empty($task['END_TIME']) && strtotime($now)>=strtotime($task['END_TIME'])){
                $task['NEXT_TIME']='END';
                continue;
            }
            if($task['TYPE'] == 1 && empty($task['NEXT_TIME']) )
            {
                continue;
            }
            if($task['TYPE'] == 2 && empty($task['INTERVAL']) )
            {
                continue;
            }
            if(!empty($task['NEXT_TIME']) && $task['NEXT_TIME']=='END' )
            {
                continue;
            }
            if($task['TYPE'] == 1 && (strtotime($now)>=strtotime($task['NEXT_TIME'])))
            {
                $task['LAST_TIME']= $now;
                $task['NEXT_TIME']='END';
                $task['STATUS']=2;
                AlpacaWorker::worker()->action(['REQUEST_URI'=>"{$task['ACTION']}"]);
                continue;
            }
            if($task['TYPE'] == 2)
            {
                if(empty($task['NEXT_TIME'])){
                    $task['NEXT_TIME'] = $task['BEGIN_TIME'];
                }
                if(strtotime($now)>=strtotime($task['NEXT_TIME'])){
                    $task['LAST_TIME']= $now;
                    $task['NEXT_TIME']= date('Y-m-d H:i:s',strtotime($task['INTERVAL']));
                    AlpacaWorker::worker()->action(['REQUEST_URI'=>"{$task['ACTION']}"]);
                }
                continue;
            }
        }
        file_put_contents($this->task_json, json_encode($tasks), LOCK_EX);
        return $tasks;
    }
    /**
     * 格式化时间
     * @author Chengcheng
     * @param string $interval
     * @date 2016-10-23 20:34:00
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    private function timeToStr($interval)
    {
        $result = "";
        if($interval != null && $interval != ""){
            $temp = explode(" ", $interval);
            $iNumTemp = $temp[0];
            $iType = $temp[1];
            $iNum = str_replace("+", "", $iNumTemp);
            $str = "";
            switch ($iType){
                case "year":
                    $str = "（年）";
                    break;
                case "month":
                    $str = "（月）";
                    break;
                case "day":
                    $str = "（日）";
                    break;
                case "hour":
                    $str = "（小时）";
                    break;
                case "minute":
                    $str = "（分）";
                    break;
                case "second":
                    $str = "（秒）";
                    break;
                default:
                    break;
            }
           $result = $iNum. $str;
        }
        return $result;
    }
}
```
### 定时任务进程入口控制器
主要是为了实现异步开启后台进程


```
<?php
namespace Crontab\Controllers;
use Crontab\Common\Code;
use Crontab\Common\Msg;
use Crontab\Controllers\Base\BaseController;
use Crontab\Library\Crontab\AlpacaCrontab;
use Crontab\Library\Crontab\AlpacaDaemon;
use Crontab\Library\Crontab\AlpacaWorker;
/**
 * index
 * @author Chengcheng
 * @date 2017-02-22 15:50:00
 */
class IndexController extends BaseController
{
    /**
     * 设置不需要登录的的Action,不加Action前缀
     * @author Chengcheng
     * @date   2016年10月23日 20:39:25
     * <a href='http://www.jobbole.com/members/wx1409399284'>@return</a> array
     */
    protected function withoutLoginActions()
    {
    }
    /**
     * 开始定时任务的守护进程
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function start()
    {
        //开始守护进程
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        //在守护进程中注入定时任务
        $events = ['0'=>function(){
            AlpacaWorker::worker()->action(['REQUEST_URI'=>"/crontab/index/task"]);
        }];
        AlpacaDaemon::daemon()->setEvents($events);
        AlpacaDaemon::daemon()->start();
        //返回结果
        return $this->ajaxReturn($result);
    }
    /**
     * 停止定时任务的守护进程
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function stop()
    {
        //停止守护进程
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = AlpacaDaemon::daemon()->stop();
        //返回结果
        return $this->ajaxReturn($result);
    }
    /**
     * 执行定时任务
     * @author Chengcheng
     * @date 2016-10-23 20:34:00
     */
    public function task()
    {
        //执行定时任务
        $result['code'] = Code::SYSTEM_OK;
        $result['msg']  = Msg::SYSTEM_OK;
        $result['data'] = AlpacaCrontab::crontab()->doTask();
        //返回结果
        return $this->ajaxReturn($result);
    }
}
```
以上是PHP实现定时任务的核心类与方法， 完整的代码请参看代码服务器中提供的源码。
