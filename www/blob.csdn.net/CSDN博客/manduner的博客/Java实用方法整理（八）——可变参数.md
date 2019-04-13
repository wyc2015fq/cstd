
# Java实用方法整理（八）——可变参数 - manduner的博客 - CSDN博客


2018年05月27日 15:39:59[Manduner_TJU](https://me.csdn.net/manduner)阅读数：106


# 1，描述
定义方法的时候不知道应该定义多少个参数
# 2，格式
修饰符   返回值类型   方法名（数据类型...   变量名）{
方法体
}
# 3，注意事项
这里的变量其实是一个数组
如果一个方法有可变参数，并且有多个参数，那么可变参数肯定是最后一个
# 4，实例
```python
/**
* @author Manduner_TJU
* @version 创建时间：2018年5月27日下午3:30:52
*/
public class ParameterDemo {
	public static void main(String[] args) {
		 System.out.println(sum(1,2,3));
		 System.out.println(sum(1,2,3,4));
		 System.out.println(sum(1,2,3,4,5));
		 System.out.println(sum(1,2,3,4,5,6));
		
	}
	
	
	private static int sum(int... a) {
		int result = 0;
		
		for(int x : a) {
			result += x;
		}
		
		return result;
	}
}
```


