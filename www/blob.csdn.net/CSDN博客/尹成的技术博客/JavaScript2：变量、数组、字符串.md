
# JavaScript2：变量、数组、字符串 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:08:18[尹成](https://me.csdn.net/yincheng01)阅读数：21


\#变量
JavaScript 是一种弱类型语言，javascript的变量类型由它的值来决定。 定义变量需要用关键字 ‘var’
`var a = 123;
 var b = 'asd';`//同时定义多个变量可以用","隔开，公用一个‘var’关键字
`var c = 45,d='qwe',f='68';`\#\#变量类型
5种基本数据类型：
`number、string、boolean、undefined、null`1种复合类型：
`object`\#\#变量、函数、属性、函数参数命名规范
1、区分大小写
2、第一个字符必须是字母、下划线（_）或者美元符号（$）
3、其他字符可以是字母、下划线、美元符或数字
\#数组
数组就是一组数据的集合，javascript中，数组里面的数据可以是不同类型的。
\#\#定义数组的方法
`//对象的实例创建
var aList = new Array(1,2,3);
//直接量创建
var aList2 = [1,2,3,'asd'];`\#\#操作数组中数据的方法
1、获取数组的长度：aList.length;
`var aList = [1,2,3,4];
alert(aList.length); // 弹出4`2、用下标操作数组的某个数据：aList[0];
`var aList = [1,2,3,4];
alert(aList[0]); // 弹出1`3、join() 将数组成员通过一个分隔符合并成字符串
`var aList = [1,2,3,4];
alert(aList.join('-')); // 弹出 1-2-3-4`4、push() 和 pop() 从数组最后增加成员或删除成员
`var aList = [1,2,3,4];
aList.push(5);
alert(aList); //弹出1,2,3,4,5
aList.pop();
alert(aList); // 弹出1,2,3,4`5、unshift()和 shift() 从数组前面增加成员或删除成员
`var aList = [1,2,3,4];
aList.unshift(5);
alert(aList); //弹出5,1,2,3,4
aList.shift();
alert(aList); // 弹出1,2,3,4`6、reverse() 将数组反转
`var aList = [1,2,3,4];
aList.reverse();
alert(aList);  // 弹出4,3,2,1`7、indexOf() 返回数组中元素第一次出现的索引值
`var aList = [1,2,3,4,1,3,4];
alert(aList.indexOf(1));`8、splice() 在数组中增加或删除成员
`var aList = [1,2,3,4];
aList.splice(2,1,7,8,9); //从第2个元素开始，删除1个元素，然后在此位置增加'7,8,9'三个元素
alert(aList); //弹出 1,2,7,8,9,4`\#\#多维数组
多维数组指的是数组的成员也是数组的数组。
`var aList = [[1,2,3],['a','b','c']];
alert(aList[0][1]); //弹出2;`\#\#获取元素的第二种方法
document.getElementsByTagName(’’),获取的是一个选择集，不是数组，但是可以用下标的方式操作选择集里面的dom元素。
\#字符串
\#\#字符串处理方法
1、字符串合并操作：“ + ”
2、parseInt() 将数字字符串转化为整数
3、parseFloat() 将数字字符串转化为小数
4、split() 把一个字符串分隔成字符串组成的数组
5、charAt() 获取字符串中的某一个字符
6、indexOf() 查找字符串是否含有某字符
7、substring() 截取字符串 用法： substring(start,end)（不包括end）
8、toUpperCase() 字符串转大写
9、toLowerCase() 字符串转小写
\#\#字符串反转
`var str = 'asdfj12jlsdkf098';
var str2 = str.split('').reverse().join('');
alert(str2);`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

