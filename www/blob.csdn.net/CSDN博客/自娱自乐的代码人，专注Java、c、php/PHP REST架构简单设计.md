# PHP REST架构简单设计 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月25日 18:58:25[initphp](https://me.csdn.net/initphp)阅读数：8612







- REST是什么？

> 
REST(Representational State Transfer表述性状态转移)是一种针对网络应用的设计和开发方式，可以降低开发的复杂性，提高系统的可伸缩性。

- 
REST的特点：

- 网络上的所有事物都被抽象为资源（resource）
- 每个资源对应一个唯一的资源标识（resource identifier）
- 通过通用的连接器接口（generic connector interface）对资源进行操作；
- 对资源的各种操作不会改变资源标识；
- 所有的操作都是无状态的（stateless）。
- 
REST的通俗说明：

- REST是通过HTTP请求的状态，去服务器端请求不同的服务方法，但是请求地址是相同的。比如请求一个[http://xxxxx.com/user/](http://xxxxx.com/user/)的接口为例
- 例如POST方法，一般提供数据新增的功能，如果客户端是POST方法，请求上面的接口，那就是告诉服务器，是新增的操作。
- 例如GET方法，一般是获取数据，因为GET方法是默认的方法，不会对数据造成改变，所以一般是获取数据，通过GET方法请求上面的接口，就是获取用户数据
- 例如PUT方法，一般是更新操作，告诉服务器，更新用户信息。
- 例如DEL方法，一般是删除操作。
- 通过HTTP的4中请求方法，就可以在同一个接口地址上产生四种不同的请求，对接口的扩展性提供了巨大的帮助。
- REST PHP端代码简单实现：

```php
/**
 * DEMO
 * @author zhuli
 */
class indexController extends Controller {
    
   public $initphp_list = array('test', 'user'); //Action白名单
    public $isRest = array('user');
    //请求index.php?c=index&a=user接口，通过CURL的不同请求状态
    public function run() { 
        $curl = $this->getLibrary('curl');
        $a = $curl->put('http://10.9.11.1/initphp_32/demo/www/?c=index&a=user', array('username' => 'hello'));
        print_r($a);
    }
    //当请求方式是GET方法的时候
    public function user_get() {
        echo 'get';
    }
    //POST方法
    public function user_post() {
        $username = $this->controller->get_gp('username', 'P');
        echo $username;
        echo 'get';
    }
    //PUT
    public function user_put() {
        $username = $this->controller->get_gp('username', 'U');
        echo $username;
        echo 'put';
    }
    //DEL
    public function user_del() {
        $username = $this->controller->get_gp('username', 'D');
        echo $username;
        echo 'del';
    }
}
```




参考：百度，InitPHP框架REST实现






