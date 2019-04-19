# GPS时间 - 左直拳的马桶_日用桶 - CSDN博客
2014年09月26日 16:58:36[左直拳](https://me.csdn.net/leftfist)阅读数：3193
GPS信息里面包含一个时间戳。
phonegap(即cordova)的地理位置API Geolocation 提供了对设备GPS传感器的访问，返回的数据中，除了坐标，还有一个时间戳timestamp。这是一个UTC时间（国际标准时间），即距 GMT 时间（格林威治时间） 1970 年 1 月 1 日午夜的毫秒数。根据这个时间，可以划算回我们本地的时间。
比如我们从Geolocation.TimeStamp里获得UTC时间为 1346984871162，在javascript里这样转换：
```java
<script>
	var date = new Date(1346984871162);
	alert(date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + ' ' + date.getHours() + ':' + date.getMinutes());
</script>
```
得到本地时间为：2012-8-7 10:27
UTC时间是国际标准时间，而javascript的Date对象估计是与本地计算机或设备的时区挂钩，使用UTC时间构造Date对象的时候，已经考虑到了时区的差异，因此用date对象getYear()等方法得到的时间，就是本地的时间。
