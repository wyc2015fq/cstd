# JavaScript之forEach()、map()、reduce(),filter() 的区别详解 - 零度的博客专栏 - CSDN博客
2018年03月20日 09:54:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：174
首先我给出一段代码：
```java
<!DOCTYPE html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<script type="text/javascript" src="./jquery.js"></script> 
<script type="text/javascript">  
    var people = [{name:"张三",money:90.0},{name:"李四",money:190.0},{name:"王五",money:290.0}];
	//forEach方法
	people.forEach(function(person){
		console.log(person);
	});
	
	//map方法
	var wallets = people.map(function (person) {
	  return person.money;
	});
	console.log(wallets);
	
	//reduce ---100为countedMoney的参数值
	var totalMoney = wallets.reduce(function (countedMoney, wallet){
             return countedMoney + wallet;
    }, 100);
    console.log(totalMoney);
	
	
	//filter ---实现过滤
	var fatPeople = people.filter(function (person) {
           return person.money > 100;
    });
    console.log(fatPeople);
	
    var fatWallets = wallets.filter(function (money) {
           return money > 100;
    });
    console.log(fatWallets);
	
</script>
</head>
<body>
</body>
<html>
```
运行结果如下：
![](https://img-blog.csdn.net/20180320095039078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面对上述做一下解释，网上有一个比较形象的理解，这里我引用一下：假设我们有一个数组，每个元素是一个人，也可参考上面例子的对象建立。
foreach 就是你按顺序一个一个跟他们做点什么，具体做什么，随便:
```
people.forEach(function (dude) {
  dude.pickUpSoap();
});
```
map 就是你手里拿一个盒子（一个新的数组），一个一个叫他们把钱包扔进去。结束的时候你获得了一个新的数组，里面是大家的钱包，钱包的顺序和人的顺序一一对应。
```
var wallets = people.map(function (dude) {
  return dude.wallet;
});
```
reduce 就是你拿着钱包，一个一个数过去看里面有多少钱啊？每检查一个，你就和前面的总和加一起来。这样结束的时候你就知道大家总共有多少钱了。
```
var totalMoney = wallets.reduce(function (countedMoney, wallet) {
  return countedMoney + wallet.money;
}, );
```
 filter 就是你一个个钱包数过去的时候，里面钱少于 100 块的不要（留在原来的盒子里），多于 100 块的丢到一个新的盒子里。这样结束的时候你又有了一个新的数组，里面是所有钱多于 100 块的钱包：
```
var fatWallets = wallets.filter(function (wallet) {
  return wallet.money > ;
});
```
最后要说明一点这个类比和实际代码的一个区别，那就是 map 和 filter 都是 immutable methods，也就是说它们只会返回一个新数组，而不会改变原来的那个数组，所以这里 filter 的例子是和代码有些出入的（原来的盒子里的钱包减少了），但为了形象说明，大家理解就好。

