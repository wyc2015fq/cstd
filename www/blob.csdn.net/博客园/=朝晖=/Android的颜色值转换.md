# Android的颜色值转换 - =朝晖= - 博客园
# [Android的颜色值转换](https://www.cnblogs.com/dhcn/p/7125006.html)
    Android的颜色int值比较变态，是个负值，用计算机术语讲叫补码，手工转换比较麻烦，首先看看文档 https://developer.android.com/reference/android/graphics/Color.html，里面说的很清楚，白色常量值：int WHITE Constant Value: -1 (0xffffffff)，黄色常量值：int YELLOW Constant Value: -256 (0xffffff00)。计算方法就来了，首先把ffffffff的十进制值求出来是4294967295，然后把这个值加1得到是4294967296，然后把这个值和颜色的那个负值加上就可以得到颜色值的十进制值了，至于十进制怎么转十六进制，就不用说了吧。

