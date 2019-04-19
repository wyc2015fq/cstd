# Yii2自动加载 - LC900730的博客 - CSDN博客
2017年10月17日 21:46:53[lc900730](https://me.csdn.net/LC900730)阅读数：228
index.php中，new yii\web\Application（$config）->run()
class yii\web\Application extends \yii\base\Application 
在\yii\base\Application构造方法中，Yii::$app=this.  static::setInstance($this);
static：如果在子类中重写了父类中的static方法、属性，父类就会去访问了子类的static方法； 
如调用父类方法，但是重写了父类属性，那么即使是在父类方法中，也访问的是子类属性。
self: 是类内指针，不管子类有没有重写过父类中的方法、属性都指向本类的静态方法、属性
服务定位者ServiceLocator在yii\di中
```
在Application中，public function getSession(){
    $this->get（'session'）;
```
在ClassLoader.php中
```php
public function register($prepend=false){
    spl_autoload_register(array($this,'loadClass',true,$prepend));
}
```
### yii2中class  Model
yii\base\Model   extends   Component实现ArrayAccess，Arrayable，ItertorAggregate
### Yii中的自动加载
BaseYii.php
```php
public static function autoload($className){
    $classmap....
}
```
在 yiisoft/yii2/classes.map中，维持着命名空间与文件的对应关系 
如果是@那么就可能设置了别名。 
要不是\分割的 
在入口脚本中，除了Yii自己的autoloader，还有一个第三方的autoloader:
require(**DIR** . ‘/../../vendor/autoload.php’); 
这个其实是Composer提供的autoloader。Yii使用Composer来作为包依赖管理器，
