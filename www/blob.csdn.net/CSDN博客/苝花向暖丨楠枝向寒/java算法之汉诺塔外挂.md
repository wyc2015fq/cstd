# java算法之汉诺塔外挂 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月22日 22:01:59[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：24


```java
//算法
	/**
	 * 移动盘子
	 * topN：移动的盘子数
	 * inter：中间塔座
	 * to：目标塔座
	 */
	public static void doTower(int topN,char from,char inter,char to) {
		if(topN == 1) {
			System.out.println("盘子1，从"+from+"塔座到"+ to +"塔座");
		}else {
			doTower(topN - 1, from, to, inter);
			System.out.println("盘子"+topN+"从"+from+"塔座到"+ to +"塔座");
			doTower(topN - 1,inter,from,to);
		}
	}
```

```java
//测试
	public static void main(String[] args) {
		
		HanoiTower.doTower(5, 'A', 'B', 'C');
	}
```

结果：

盘子1，从A塔座到C塔座
盘子2从A塔座到B塔座
盘子1，从C塔座到B塔座
盘子3从A塔座到C塔座
盘子1，从B塔座到A塔座
盘子2从B塔座到C塔座
盘子1，从A塔座到C塔座
盘子4从A塔座到B塔座
盘子1，从C塔座到B塔座
盘子2从C塔座到A塔座
盘子1，从B塔座到A塔座
盘子3从C塔座到B塔座
盘子1，从A塔座到C塔座
盘子2从A塔座到B塔座
盘子1，从C塔座到B塔座
盘子5从A塔座到C塔座
盘子1，从B塔座到A塔座
盘子2从B塔座到C塔座
盘子1，从A塔座到C塔座
盘子3从B塔座到A塔座
盘子1，从C塔座到B塔座
盘子2从C塔座到A塔座
盘子1，从B塔座到A塔座
盘子4从B塔座到C塔座
盘子1，从A塔座到C塔座
盘子2从A塔座到B塔座
盘子1，从C塔座到B塔座
盘子3从A塔座到C塔座
盘子1，从B塔座到A塔座
盘子2从B塔座到C塔座
盘子1，从A塔座到C塔座


