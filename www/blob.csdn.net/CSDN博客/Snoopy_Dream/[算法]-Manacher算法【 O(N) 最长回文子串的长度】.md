# [算法] - Manacher算法【 O(N) 最长回文子串的长度】 - Snoopy_Dream - CSDN博客





2019年01月15日 11:56:46[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：44
所属专栏：[编程感悟](https://blog.csdn.net/column/details/31618.html)









**目录**

[Manacher算法解决的问题字符串str中， 最长回文子串的长度如何求解？如何做到时间复杂度O(N)完成？](#Manacher%E7%AE%97%E6%B3%95%E8%A7%A3%E5%86%B3%E7%9A%84%E9%97%AE%E9%A2%98%E5%AD%97%E7%AC%A6%E4%B8%B2str%E4%B8%AD%EF%BC%8C%20%E6%9C%80%E9%95%BF%E5%9B%9E%E6%96%87%E5%AD%90%E4%B8%B2%E7%9A%84%E9%95%BF%E5%BA%A6%E5%A6%82%E4%BD%95%E6%B1%82%E8%A7%A3%EF%BC%9F%E5%A6%82%E4%BD%95%E5%81%9A%E5%88%B0%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6O(N)%E5%AE%8C%E6%88%90%EF%BC%9F%C2%A0%C2%A0)

[1. 大体理解：](#1.%20%E5%A4%A7%E4%BD%93%E7%90%86%E8%A7%A3%EF%BC%9A)

[2. 思路剖析](#2.%20%E6%80%9D%E8%B7%AF%E5%89%96%E6%9E%90)

[3. 时间复杂度](#3.%20%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6)

[4. 详细代码](#4.%20%E8%AF%A6%E7%BB%86%E4%BB%A3%E7%A0%81)

### Manacher算法解决的问题字符串str中， 最长回文子串的长度如何求解？如何做到时间复杂度O(N)完成？  



### 1. 大体理解：

![](https://img-blog.csdnimg.cn/20190115115256677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

9是指回文子串多的长度

常用的方法：字符串的问题，在两两中间加特殊字符。   然后/2



### 2. 思路剖析

![](https://img-blog.csdnimg.cn/20190115114949400.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190115115115445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



### 3. 时间复杂度

![](https://img-blog.csdnimg.cn/20190115115345465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

估计while的次数，每个分支必中一个，R变大的意思，就是遍历依次往下走，所以最坏情况是N

![](https://img-blog.csdnimg.cn/20190115115354767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 4. 详细代码

```java
public class Code02_Manacher {

	public static char[] manacherString(String str) {
		char[] charArr = str.toCharArray();
		char[] res = new char[str.length() * 2 + 1];
		int index = 0;
		for (int i = 0; i != res.length; i++) {
			res[i] = (i & 1) == 0 ? '#' : charArr[index++];
		}
		return res;
	}

	public static int maxLcpsLength(String str) {
		if (str == null || str.length() == 0) {
			return 0;
		}
		char[] charArr = manacherString(str);// 121 ——> #1#2#1#
		int[] pArr = new int[charArr.length];//r[i] 半径数组
		int C = -1; // 取得R的时候的中心
		int R = -1;// c..R-1是回文半径长度， R-1是代表回文右边界，R是边界的下一个位置
		int max = Integer.MIN_VALUE;// 最大回文半径

		for (int i = 0; i != charArr.length; i++) {
			//R > i表示i在R内，R <= i 表示i在R外，至少的回文半径是1
			//2 * C - i就是i' r[i']与R - i作比较，判别是②的哪个小情况
			// 返回四种情况下i处至少的回文半径是多少
			// 因为有些部分是不需要验证就知道相不相等，所以设置这个
            // 不担心数组越界，因为其实R > i已经有了判断
			pArr[i] = R > i ? Math.min(pArr[2 * C - i], R - i) : 1;
			while (i + pArr[i] < charArr.length && i - pArr[i] > -1) {
				if (charArr[i + pArr[i]] == charArr[i - pArr[i]])
					pArr[i]++;//
				else {
					break;
				}
			}
			if (i + pArr[i] > R) {//表示越界R，需要重新更新
				R = i + pArr[i];//更新R c
				C = i;
			}
			max = Math.max(max, pArr[i]);
		}
		//max是有#的回文半径，需要返回最长的回文子串，直接-1就是，
		//如果要返回没有#的会问半径，直接/2,floor就可以
		return max - 1;
	}
}
```

![](https://img-blog.csdnimg.cn/20190226211305337.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



