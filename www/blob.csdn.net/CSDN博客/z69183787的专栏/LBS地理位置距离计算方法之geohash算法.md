# LBS地理位置距离计算方法之geohash算法 - z69183787的专栏 - CSDN博客
2016年10月19日 11:38:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4932
随着移动终端的普及，很多应用都基于LBS功能，附近的某某（餐馆、银行、妹纸等等）。
基础数据中，一般保存了目标位置的经纬度；利用用户提供的经纬度，进行对比，从而获得是否在附近。这里需要在设置出一个字段，是关于编码的字段，一会看下文哈……
**地理位置距离实现目标：**
查找附近多少公里内的人或者商家
比如：微信、陌陌、美团、基于O2O的一些APP这些应用或者移动网页都需要用到地理位置计算
目前来说：移动地理位置距离计算比较好的算法是**geohash**，特此整理分享。
**geohash有以下几个特点：**
**第一：**geohash用一个字符串表示经度和纬度两个坐标。
某些情况下无法在两列上同时应用索引 （例如MySQL 4之前的版本，Google App Engine的数据层等），利用geohash，只需在一列上应用索引即可。
(这里插一句：我们的mysql为字段创建的索引，其实原理就是利用二分法算法来做路径查询简化，快速查找出想要的字段位置)
**第二：**geohash表示的并不是一个点，而是一个矩形区域。比如编码wx4g0ec19，它表示的是一个矩形区域。
使用者可以发布地址编码，既能表明自己位于北海公园附近，又不至于暴露自己的精确坐标，有助于隐私保护。
**第三：**编码的前缀可以表示更大的区域。
例如wx4g0ec1，它的前缀wx4g0e表示包含编码wx4g0ec1在内的更大范围。 这个特性可以用于附近地点搜索。首先根据用户当前坐标计算geohash（例如wx4g0ec1）然后取其前缀进行查询 （**SELECT * FROM place WHERE geohash LIKE 'wx4g0e%'**），即可查询附近的所有地点。
Geohash比直接用经纬度的高效很多。
**Geohash的原理**
**Geohash的最简单的解释就是：将一个经纬度信息，转换成一个可以排序，可以比较的字符串编码**
geohash能做到:
- [https://github.com/CloudSide/geohash](https://github.com/CloudSide/geohash)
- 
[https://github.com/lyokato/objc-geohash](https://github.com/lyokato/objc-geohash)
- 
例如: 假设我的数据库里存储着1亿条包含经纬度的用户数据，用iPhone/android手机定位得到 新浪总部(理想国际大厦)的经纬度: 39.98123848, 116.30683690 然后去数据库查找附近的妞 
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
require_once('geohash.class.php');
$geohash = new Geohash;
//得到这点的hash值
$hash = $geohash->encode(39.98123848, 116.30683690);
//取前缀，前缀约长范围越小
$prefix = substr($hash, 0, 6);
//取出相邻八个区域
$neighbors = $geohash->neighbors($prefix);
array_push($neighbors, $prefix);
print_r($neighbors);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
- 得到9个geohash值
```
```php
//得到9个geohash值
```
```php
Array
```
```php
(
```
```php
```
```php
[top]
 => wx4eqx
```
```php
```
```php
[bottom]
 => wx4eqt
```
```php
```
```php
[right]
 => wx4eqy
```
```php
```
```php
[left]
 => wx4eqq
```
```php
```
```php
[topleft]
 => wx4eqr
```
```php
```
```php
[topright]
 => wx4eqz
```
```php
```
```php
[bottomright]
 => wx4eqv
```
```php
```
```php
[bottomleft]
 => wx4eqm
```
```php
```
```php
[0]
 => wx4eqw
```
```php
)
```
```
- 范围如图:
- ![](http://images2015.cnblogs.com/blog/381809/201511/381809-20151124213837171-989101042.png)
- 用sql语句查询
- 
```
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqw%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqx%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqt%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqy%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqq%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqr%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqz%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqv%'``;`
`SELECT``* ``FROM``xy ``WHERE``geohash ``LIKE``'wx4eqm%'``;`
```
- 看一下是否用上索引 (一共有50多万行测试数据):
- 
索引:
```php
```php
```
```
其他资料：
- geohash演示: [http://openlocation.org/geohash/geohash-js/](http://openlocation.org/geohash/geohash-js/)
- wiki: [http://en.wikipedia.org/wiki/Geohash](http://en.wikipedia.org/wiki/Geohash)
- 原理: [https://github.com/CloudSide/geohash/wiki](https://github.com/CloudSide/geohash/wiki)
