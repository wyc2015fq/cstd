# laravel自动加载 - LC900730的博客 - CSDN博客
2017年05月26日 10:26:28[lc900730](https://me.csdn.net/LC900730)阅读数：1033
## 类的自动加载
Composer\Autoload\ClassLoader 
类的自动加载函数是在Composer\Autoload\ClassLoader中实现的，实例化该类并将类的命名空间的与文件路径的对应关系注册到相应的属性中，然后通过实例方法register($prepend=false)注册一个类自动加载函数，即为该类实例的loadClass方法，并且将其注册在类自动加载函数队列的末尾，当使用一个为包含的类名时，会自动调用loadClass方法通过参数获取包含命令空间的类名的信息，接着根据类的命名空间与文件路径的相对应关系查找文件路径，最后通过includeFile()函数来包含该文件，从而实现类的自动加载。
```php
class ClassLoader{ 
    public function register($prepend=false){
        spl_autoload_register(array($this,'loadClass'),true,$prepend);
        public function loadClass($class){
            if($file=$this->findFile($class)){
                includeFile($file);
                return true;    
            }
        }
    }
}
```
```java
/**
spl_autoload_register([callable $autoload_function [,bool $throw=true[,bool $prepend=false]]])
$autoload_function:欲注册的自动装载函数
$throw:无法成功注册的时候是否抛出异常；
$prepend:如果是true，spl_autoload_register()会添加到函数到队列之首，而不是队列尾部
*/
```
## 反射机制
如果事先不知道需要实例化哪个类，而是在运行的时候根据动态信息确定，对于这种情况可以通过反射机制获取需要实例化类的构造函数信息并完成相应的实例化。在Laravel中，服务容器解析服务的过程中就用到了反射机制。
### 服务解析
- 在Laravel框架中，解析服务是通过build()函数实现的，一般是分为2种情况：一种是查找对应的服务是否被服务提供者注册为实例或者提供服务的匿名函数，如果是则直接进行服务解析
- 第二种是服务名称没有相应的服务绑定，通过反射机制来动态创建服务
第一步：通过反射机制来获取服务类构造函数信息； 
第二步：解决服务类的构造函数的依赖问题 
1️⃣$reflector=new ReflectionClass($concrete)来创建一个反射类的实例，其中$concrete是类的名称，然后通过$reflector->isInstantiable()来判断这个类是否可以实例化； 
2️⃣然后通过$constructor=$reflector->getConstructor()获取类的构造方法，当该类存在构造函数时，返回ReflectionMethod对象，相当于获取构造函数的反射类；不存在构造函数时候返回NULL，最后通过is_null($constructor)来判断是否存在构造函数，如果不存在则直接实例化该类；如果存在则通过$dependencies=$constructor->getParameters()来获取构造函数依赖的输入参数。
“`PHP 
public function build($concrete, array $parameters = []) 
    { 
        // If the concrete type is actually a Closure, we will just execute it and 
        // hand back the results of the functions, which allows functions to be 
        // used as resolvers for more fine-tuned resolution of these objects. 
        if ($concrete instanceof Closure) {                      return $concrete(  $this,  $parameters); 
        }
```
$reflector = new ReflectionClass($concrete);
    // If the type is not instantiable, the developer is attempting to resolve
    // an abstract type such as an Interface of Abstract Class and there is
    // no binding registered for the abstractions so we need to bail out.
    if (! $reflector->isInstantiable()) {
        $message = "Target [$concrete] is not instantiable.";
        throw new BindingResolutionContractException($message);
    }
    $this->buildStack[] = $concrete;
    $constructor = $reflector->getConstructor();
    // If there are no constructors, that means there are no dependencies then
    // we can just resolve the instances of the objects right away, without
    // resolving any other types or dependencies out of these containers.
    if (is_null($constructor)) {
        array_pop($this->buildStack);
        return new $concrete;
    }
    $dependencies = $constructor->getParameters();
    // Once we have all the constructor's parameters we can create each of the
    // dependency instances and then use the reflection instances to make a
    // new instance of this class, injecting the created dependencies in.
    $parameters = $this->keyParametersByArgument(
        $dependencies, $parameters
    );
    $instances = $this->getDependencies(
        $dependencies, $parameters
    );
    array_pop($this->buildStack);
    return $reflector->newInstanceArgs($instances);
}
```
//根据反射参数解决所有的参数依赖
```
## self与static
self::是直接调用本类中方法，而static是根据调用test()函数的类来决定static::的值，因此static::的值只有在调用时才能确定下来，而self则是在定义的时候就确定下来的。需要注意：static并不限于静态方法调用，同样适用于非静态函数的调用，调用的方式同上述静态函数一样，是在调用时候动态确定的。
