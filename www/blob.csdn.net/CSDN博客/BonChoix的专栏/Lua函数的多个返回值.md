# Lua函数的多个返回值 - BonChoix的专栏 - CSDN博客





2012年10月13日 05:45:18[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：28475标签：[lua																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=lua&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/BonChoix/article/category/1251239)








                
       Lua中的函数的一个很特殊也很有用的性质，即可以有多个返回值。包括一些内置的函数就是这样。比如string.find函数，在给定的字符串中查找一个pattern，如果有匹配的部分，则返回对应的头、尾的两个索引值；如果不存在匹配，则返回nil。当然，用户定义的函数也可以有多个返回值，通过return关键字返回多个结果即可。比如一个返回数组中最大值及其索引的函数：
```
function max(a)
	local m = a[1];
	local mIndex = 1;
	for i,v in ipairs(a) do
		if v>m then
			m = v;
			mIndex = i;
		end
	end

	return m,mIndex;
end
```



       通常情况下，Lua会根据函数调用的不同情况自动调整返回值的有效个数。主要有以下几种情况：

       1. 如果函数调用作为一个单独的语句，比如max({1,2,3})，则忽略所有的返回值；

       2. 如果调用处在一串的表达式中：

           1)如果处于表达式串的首位或中间，则只有第一个返回值被返回；

           2)如果处于最后一个表达式，则所有的返回值都有效；

       拿下面三个函数作为例子：

```
function f0() end;
function f1() return "a" end
function f2() return "a","b" end
```

在多重赋值中，如果函数调用作为仅有的或最后一个表达式，则返回尽可能多的值来匹配左边的变量，如果多出，自动忽略；如果不够，赋为nil。比如，

```
x = f1()      		--x="a","b"被忽略
x,y = f1()	  		--x="a",y="b"
x,y,z = f2()	 	--x="a",y="b",z=nil
x,y,z = 10,f2() 	-- x=10, y="a", z="b"
x,y,z = f2(),10		-- x="a", y=10, z=nil, f2只返回一个值
```



同理， 作为另一个函数的参数时，规则一样：

```
print(f2()) 	--> a b
print(f2(), 1) 	--> a 1
```

还有用来构造数组时：

```
t = {f2()} 				-- t = {"a", "b"}
t = {f0(), f2(), 4} 	-- t[1] = nil, t[2] = "a", t[3] = 4
```



最后，当函数返回值作为另一个函数的返回值被返回时，则全部被返回。如果想强制只返回一个值 ，则可以用括号 "()" 把表达式括起来即可。比如：

```
function foo (i)
	if i == 0 then return foo0()
	elseif i == 1 then return foo1()
	elseif i == 2 then return foo2()
	end
end
```

```
print(foo(1)) --> a
print(foo(2)) --> a b
print(foo(0)) -- (无返回值)
print(foo(3)) -- (无返回值)
```

如果想强制只返回一个值，则可以用括号 "()" 把表达式括起来即可：

```
print((foo0())) --> nil
print((foo1())) --> a
print((foo2())) --> a
```



（见《Programming in Lua, 2rd》）








