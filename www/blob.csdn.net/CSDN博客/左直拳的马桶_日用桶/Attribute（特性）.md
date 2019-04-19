# Attribute（特性） - 左直拳的马桶_日用桶 - CSDN博客
2015年11月19日 19:42:00[左直拳](https://me.csdn.net/leftfist)阅读数：2163
一向都觉得.NET的Attribute好神秘。一个方框框住的东西，置身于类、方法的头部，本身不在类或方法里面，但又会起作用，有时作用还很大，仿佛充满了魔力。简直给人一种无冕之王，幕后之黑手的感觉！
某十几年前已经开始接触C#，却对这个Attribute充满了迷茫。原因固然跟本人懒惰成性，不思进取，从业十几年，不是“有十几年工作经验，而是一条经验用了十几年”有关，也因为我当初自学的C#教材，什么宝典，什么从入门到精通之类，都没有介绍这个东东。就算查，也不知道它叫啥。你看看： 
![这里写图片描述](https://img-blog.csdn.net/20151119192540090)
后来有人叫它：特性。总算知道了。
其实Attribute本身就是一个类。换言之，它就是代码。虽然没有放在类、方法里面，只放在头部，好像就是个说明而已，但其实，它们本身就是代码的一部分。编译器、CLR能够识别、并处理它们，做出不同的反应，所以说到底，这有啥好神秘的？我只是脑筋转不过弯来罢了。
就是在我们自己的代码里面，也可以直接使用的：
```
protected override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            bool hasPermission = true;
            var permissionAttributes = filterContext
                .ActionDescriptor
                .GetCustomAttributes(typeof(PermissionAttribute), false)
                .Cast<PermissionAttribute>();
            if (permissionAttributes == null || permissionAttributes.Count() == 0)
            {//如果action没有指定权限，则继承controller的指定
                permissionAttributes = filterContext
                    .ActionDescriptor
                    .ControllerDescriptor
                    .GetCustomAttributes(typeof(PermissionAttribute), false)
                    .Cast<PermissionAttribute>();
            }
        }
```
所谓的PermissionAttribute，是自己写的一个跟权限有关的特性
```
public class PermissionAttribute : FilterAttribute, IActionFilter
    {
        public List<int> Permissions { get; set; }
        public PermissionAttribute(params int[] parameters)
        {
            Permissions = parameters.ToList();
        }
    }
```
可以用在action或controller上
```
[Permission((int)EnumBusinessPermission.DictionaryAdmin)]
        public ActionResult Create()
        {
            SysField model = new SysField();
            return View("Edit", model);
        }
```
