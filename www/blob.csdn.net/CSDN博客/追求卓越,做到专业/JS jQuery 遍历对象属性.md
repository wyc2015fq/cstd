
# JS-jQuery 遍历对象属性 - 追求卓越,做到专业 - CSDN博客


2014年02月25日 10:34:47[Waldenz](https://me.csdn.net/enter89)阅读数：31034


Javascript For/In 循环： 循环遍历对象的属性

```python
var person={fname:"John",lname:"Doe",age:25};
for (x in person)
  {
    txt=txt + person[x];
  }
结果：JohnDoe25
```
jQuery jQuery.each() 遍历对象属性

```python
var arr = ["one", "two", "three", "four", "five"];
var obj = { one: 1, two: 2, three: 3, four: 4, five: 5 };
```
遍历数组

```python
var text = "Array ";
jQuery.each(arr, function(i, val) {
	text = text + " #Index:" + i + ":" + val;
});
console.log(text);
//Array  #Index:0:one #Index:1:two #Index:2:three #Index:3:four #Index:4:five
```
遍历对象

```python
text = "Object ";
jQuery.each(obj, function(i, val) {
	text = text + "Key:" + i + ", Value:" + val;
});
console.log(text);
//Object Key:one, Value:1Key:two, Value:2Key:three, Value:3Key:four, Value:4Key:five, Value:5
```

