# PHP 设计模式之——观察者模式 - weixin_33985507的博客 - CSDN博客
2017年06月28日 03:04:32[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
```php
<?php
/**
 * Created by waterAndAir
 * title: 观察者模式
 * Date: 2016/12/21
 * Time: 15:57
 */
/**
 * 被观察者
 * SplSubject :The SplSubject interface is used alongside SplObserver to implement the Observer Design Pattern.
 * SplSubject接口用于与SplObserver实现观察者设计模式
 * Class User
 */
class User implements SplSubject{
  public $lognum;       //用户登录次数限制
  public $hobby;        //用户爱好
  protected $observers = null;       //存放观察者“们”
  public function __construct($hobby)
  {
    $this->lognum = rand(1, 10);    //实际应用中，这里应该是从缓存或数据库中读取的数据
    $this->hobby = $hobby;          //实际应用中，这里应该是从缓存或数据库中读取的数据
    $this->observers = new SplObjectStorage();    //存储一组对象
  }
  public function login(){
    //todo:操作session
    $this->notify();
  }
  /**
   * 实现接口函数，用于添加一个“观察者”
   * @param SplObserver $observer
   */
  public function attach(SplObserver $observer)
  {
    $this->observers->attach($observer);
  }
  /**
   * 实现接口函数，用于删除一个“观察者”
   * @param SplObserver $observer
   */
  public function detach(SplObserver $observer)
  {
    $this->observers->detach($observer);
  }
  /**
   * 实现接口函数，用于通知观察者
   */
  public function notify()
  {
    $this->observers->rewind();                    //指针指到第一位
    while($this->observers->valid()){             //不断获取有效对象
      $observer = $this->observers->current();     //获取当前对象
      $observer->update($this);                    //观察者处理业务
      $this->observers->next();                    //移动指针到下一对象
    }
  }
}
/**
 * 一个观察者 ：负责登录安全验证
 * Class Security
 */
class Security implements SplObserver{
  public function update(SplSubject $subject)
  {
    if($subject->lognum < 3){
      echo  "这是第".$subject->lognum."次安全登录";
    }else{
      echo  "这是第".$subject->lognum."次异常登录";
    }
  }
}
/**
 * 另一个观察者 ：负责获取用户特征
 * Class Character
 */
class Character implements SplObserver{
  public function update(SplSubject $subject){
    echo $subject->hobby."是个非常不错的爱好";
  }
}
//测试
$user = new User('学习');
$user->attach(new Security());
$user->attach(new Character());
$user->login();
#使用观察者模式，把登录的两种业务解耦。
```
