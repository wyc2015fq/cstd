
# Java实用方法整理（十八）——递归 - manduner的博客 - CSDN博客


2018年10月26日 22:42:13[Manduner_TJU](https://me.csdn.net/manduner)阅读数：92


# 1，递归方法注意事项
（1）递归一定要有出口，否则就是死递归
（2）递归的次数不能太多，否则就内存溢出
（3）构造方法不能递归使用
# 2，递归方法创建技巧
（1）递归是一个方法
（2）方法中要有出口（例如：return，输出命令等）
（3）寻找规律，以代码实现规律
# 3，案例+源码
**（1）有一对兔子，从出生后第3个月起每个月都生1对兔子，小兔子长到第3个月后每个月又生1对兔子，假如兔子都不死，那么第N个月时兔子的对数是多少？**
分析前6个月兔子对数规律如下：从第三个月开始，每个月兔子数等于前两个月兔子数之和。
|月数|1|2|3|4|5|6|
|兔子数|1|1|2|3|5|8|
```python
package test;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月26日下午7:32:43
*/
//不死神兔问题
public class Test2 {
	public static void main(String[] args) {
		int n = 20;
		System.out.println(digui(n));
	}
	
	public static int digui(int n) {
		if(n == 1 || n == 2) {
			return 1;
		}else {
			return digui(n-2) + digui(n-1);
		}
	}
}
```
**（2）输出我的电脑G盘目录下所有以“.mkv”结尾的文件的绝对路径**
```python
package test;
import java.io.File;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月26日下午9:42:39
*/
public class Test4 {
	public static void main(String[] args) {
		File file = new File("G:\\");
		getAviPath(file);
	}
	private static void getAviPath(File file) {
		File[] ff = file.listFiles();
		//由于我的电脑G盘目录下有系统自带的隐藏文件夹，会导致ff为null，所以做出了以下判断
		//本人建议不管是遍历磁盘根目录还是某个文件夹，都要加上下面的判断，否则会报空指针错误
		if (ff == null) {
			return;
		}
		//增强for的循环目标不能是null，否则会报空指针错误
		for(File f : ff) {
			if(f.isFile()) {
				if(f.getName().endsWith(".mkv")) {
					System.out.println(f.getAbsolutePath());
				}
			}else {
				getAviPath(f);
			}
		}
		
	}
}
```
**（3）删除我的电脑G盘目录下Test文件夹（注意，此功能慎用，万一删错文件夹了，可就恢复不了了）**
```python
package test;
import java.io.File;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月26日下午10:31:41
*/
public class Test5 {
	public static void main(String[] args) {
		File file = new File("G:\\Test");
		deleteTestFile(file);
	}
	private static void deleteTestFile(File file) {
		File[] ff = file.listFiles();
		if(ff==null) {
			return;
		}else {
			for(File f : ff) {
				if(f.isDirectory()) {
					deleteTestFile(f);
				}else {
					System.out.println(f.getName()+"----"+f.delete());
				}
			}
		}
		
		System.out.println(file.getName()+"----"+file.delete());
		
	}
}
```


