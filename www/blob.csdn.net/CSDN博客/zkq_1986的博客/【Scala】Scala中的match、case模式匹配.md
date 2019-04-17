# 【Scala】Scala中的match、case模式匹配 - zkq_1986的博客 - CSDN博客





2017年12月20日 11:42:12[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：401









scala中的case语法与java中的switch语法类似，但比switch更强大：


例子一，正则匹配：

```
val Pattern="(s.*)".r

    val v1="spark";

    val r=v1 match  {
      case Pattern(v1)=> "begin s*"
      case "1"=> "1"
      case "2"=> "2"
      case _=> "default"
    }

    println(r) // begin s*
```


例子二，等值匹配：

```
val v1=1

    val r=v1 match  {
      case 1=> "1"
      case 2=> "1"
      case 3=> "2"
      case _=> "default"
    }

    println(r)// 1
```


例子三，范围匹配：

```
val v1=3

    val r=v1 match  {
      case v1 if 1 until 5 contains v1=> "1-5"
      case v1 if 5 until 10 contains v1=> "5-10"
      case _=> "not found"
    }

    println(r)//1-5
```


变形语法：

```
val v1=3

    val r=v1 match  {
      case v1 if (v1>0 && v1<=5) => "1-5"
      case v1 if (v1>5 && v1<=10)=> "5-10"
      case _=> "not found"
    }

    println(r)
```


例子四，多值匹配：

```
def glob(x:Any):Any= x match {
    case   1 | "1" | "one"  => "one "
    case "two"=> 2
    case s:String => "String"
    case y:Int=>"Int 类型 "
    case _ => "其他"
  }
  
    println(glob(4))//Int 类型
```




例子五，正则多值匹配：

```
val Pattern1="(quest_.*)".r
  val Pattern2="(kp_max_.*)".r
  val Pattern3="(ukq_.*)".r
  
    /***
    * 根据提供的key返回value数据,屏蔽底层差异
    * @param key
    */
  def get(key:String):Any= key match {
      case Pattern1(_) | Pattern2(_)  =>  r.get(key) //返回string
      case Pattern3(_) => r.lrange(key,0,29).get //返回list
      case _ => ""

  }
  
    println(get("kp_max_1000168"))
    println(get("quest_494bdc1bd1c34cfa8064d8d38382659f"))
    println(get("ukq_1001_10034"))
    println(get("xfd"))
```




2 case类


//case class 是一个模式类，而case object 是全局的一个唯一的实例，



//模式类之间不能互相继承，必须统一继承一个抽象类或者trait

class DataFrameWork

//模式类，与普通类没什么区别，只是一定要有参数

case class ComputationFramework(name:String,popular:Boolean)extends DataFrameWork

case class StorageFramework(name:String,popular:Boolean)extends DataFrameWork






转载自：http://blog.csdn.net/u010454030/article/details/53790751



