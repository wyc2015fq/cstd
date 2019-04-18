# Lua语法小贴士（八）协同程序 - 凯奥斯的注释 - CSDN博客





2016年11月01日 22:33:52[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：867
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua在主线程之外还可以运行一个协同程序。可以通过coroutine的create方法创建一个协同程序，会在yield方法的地方暂停，然后调用resume方法时继续执行。通过使用协程可以把多个方法以可控的方式运行。

coroutine方法：



|create(f)|创建一个协程，f为协程内执行的方法。返回协程的句柄。|
|----|----|
|resume(co[,var1,...])|执行协程方法，[]里是方法的参数。返回协程是否在运行，以及自定义的返回值。|
|running()|返回正在运行的协程句柄，在主线程调用返回nil。|
|status(co)|返回协程的状态，输入为协程句柄。返回值："running"（运行中）"suspended"（暂停）"normal"（活动但未启用）“dead”（死的）|
|wrap(f)|创建一个协程，但是返回的不是协程的句柄，而是调用协程方法的方法。|
|yield(...)|暂停协程，传递的参数为额外的返回值。|





示例：



```
local co = nil
co = coroutine.create(function (value1,value2)
   local tempvar3 =10
   print(coroutine.status(co))
   print("coroutine section 1", value1, value2, tempvar3)
   local tempvar1 = coroutine.yield(value1+1,value2+1)
   tempvar3 = tempvar3 + value1
   print("coroutine section 2",tempvar1 ,tempvar2, tempvar3)
   local tempvar1, tempvar2= coroutine.yield(value1+value2, value1-value2)
   tempvar3 = tempvar3 + value1
   print("coroutine section 3",tempvar1,tempvar2, tempvar3)
   return value2, "end"
end)

print(coroutine.status(co))
print("main", coroutine.resume(co, 3, 2))
print(coroutine.status(co))
print("main", coroutine.resume(co, 12,14))
print(coroutine.status(co))
print("main", coroutine.resume(co, 5, 6))
print(coroutine.status(co))
print("main", coroutine.resume(co, 10, 20))
print(coroutine.status(co))
```

打印输出：


```
suspended
running
coroutine section 1    1    2    10
main    true    2    3
suspended
coroutine section 2    11    nil    11
main    true    3    -1
suspended
coroutine section 3    5    6    12
main    true    2    end
dead
main    false    cannot resume dead coroutine
dead
```



wrap的例子：


```
local func = function() 
	coroutine.yield(1) 
	coroutine.yield(2)
	coroutine.yield(3)
	coroutine.yield(4)
	coroutine.yield(5)
end
local cofun = coroutine.wrap (func)
for i = 1, 7 do
	print(cofun())
end
```
打印结果：


```
1
2
3
4
5

LuaException: xxxxxxxxxx: cannot resume dead coroutine
```

注1：第六行显示的是空，因为func方法里实际上执行了六次，第六次的返回值为nil。
注2：最后一行显示报错信息，因为协程挂了，所以不能再执行。






