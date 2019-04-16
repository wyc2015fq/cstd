# JavaScript学习总结(二十)——Javascript非构造函数的继承 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript学习总结(二十)——Javascript非构造函数的继承](https://www.cnblogs.com/xdp-gacl/p/3708309.html)



## **一、什么是"非构造函数"的继承？**

**　　比如，现在有一个对象，叫做"中国人"。**

```
1 var Chinese = {
2 　　nation:'中国'
3 };
```

　　还有一个对象，叫做"医生"。

```
1 var Doctor ={
2 　　career:'医生'
3 }
```

　　请问怎样才能让"医生"去继承"中国人"，也就是说，我怎样才能生成一个"中国医生"的对象？

　　这里要注意，这两个对象都是普通对象，不是构造函数，无法使用构造函数方法实现"继承"。

## **二、object()方法**

json格式的发明人Douglas Crockford，提出了一个object()函数，可以做到这一点。

```
1 function object(o) {
2 　　　　function F() {}
3 　　　　F.prototype = o;
4 　　　　return new F();
5 }
```

　　这个object()函数，其实只做一件事，就是把子对象的prototype属性，指向父对象，从而使得子对象与父对象连在一起。

　　使用的时候，第一步先在父对象的基础上，生成子对象：

```
1 var Doctor = object(Chinese);
```

　　然后，再加上子对象本身的属性：

```
1 Doctor.career = '医生';
```

　　这时，子对象已经继承了父对象的属性了。

```
1 alert(Doctor.nation); //中国
```

## **三、浅拷贝**

　　除了使用"prototype链"以外，还有另一种思路：把父对象的属性，全部拷贝给子对象，也能实现继承。

下面这个函数，就是在做拷贝：

```
1 function extendCopy(p) {
2 　　　　var c = {};
3 　　　　for (var i in p) {
4 　　　　　　c[i] = p[i];
5 　　　　}
6 　　　　c.uber = p;
7 　　　　return c;
8 }
```

　　使用的时候，这样写：

```
1 var Doctor = extendCopy(Chinese);
2 Doctor.career = '医生';
3 alert(Doctor.nation); // 中国
```

　　但是，这样的拷贝有一个问题。那就是，如果父对象的属性等于数组或另一个对象，那么实际上，子对象获得的只是一个内存地址，而不是真正拷贝，因此存在父对象被篡改的可能。

　　请看，现在给Chinese添加一个"出生地"属性，它的值是一个数组。

```
1 Chinese.birthPlaces = ['北京','上海','香港'];
```

　　通过extendCopy()函数，Doctor继承了Chinese。

```
1 var Doctor = extendCopy(Chinese);
```

　　然后，我们为Doctor的"出生地"添加一个城市：

```
1 Doctor.birthPlaces.push('厦门');
```

　　发生了什么事？Chinese的"出生地"也被改掉了！

```
1 alert(Doctor.birthPlaces); //北京, 上海, 香港, 厦门
2 alert(Chinese.birthPlaces); //北京, 上海, 香港, 厦门
```

　　所以，extendCopy()只是拷贝基本类型的数据，我们把这种拷贝叫做"浅拷贝"。这是早期jQuery实现继承的方式。

## **四、深拷贝**

所谓"深拷贝"，就是能够实现真正意义上的数组和对象的拷贝。它的实现并不难，只要递归调用"浅拷贝"就行了。

```
1 function deepCopy(p, c) {
 2 　　　　var c = c || {};
 3 　　　　for (var i in p) {
 4 　　　　　　if (typeof p[i] === 'object') {
 5 　　　　　　　　c[i] = (p[i].constructor === Array) ? [] : {};
 6 　　　　　　　　deepCopy(p[i], c[i]);
 7 　　　　　　} else {
 8 　　　　　　　　　c[i] = p[i];
 9 　　　　　　}
10 　　　　}
11 　　　　return c;
12 }
```

　　使用的时候这样写：

```
1 var Doctor = deepCopy(Chinese);
```

　　现在，给父对象加一个属性，值为数组。然后，在子对象上修改这个属性：

```
1 Chinese.birthPlaces = ['北京','上海','香港'];
2 Doctor.birthPlaces.push('厦门');
```

　　这时，父对象就不会受到影响了。

```
1 alert(Doctor.birthPlaces); //北京, 上海, 香港, 厦门
2 alert(Chinese.birthPlaces); //北京, 上海, 香港
```

　　目前，jQuery库使用的就是这种继承方法。









