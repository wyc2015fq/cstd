# 图方法：寻找无向图联通子集的JAVA版本 - wishchinYang的专栏 - CSDN博客
2017年10月24日 10:23:56[wishchin](https://me.csdn.net/wishchin)阅读数：3148
图像处理中一般使用稠密方法，即对图像进行像素集合进行处理。在图像拓扑方面，更多地应用图计算方法。
寻找无向图联通子集的JAVA版本，代码：
```java
//查找无向图的所有连通子集//wishchin！！！
	public static ArrayList<Set<Integer>> findAllCons(Boolean adjM[][]) {
		ArrayList<Set<Integer>> conSets = new ArrayList<Set<Integer>>();
		Stack<Integer> tree = new Stack<Integer>();
		Boolean[] isTra = new Boolean[adjM.length];
		for (int i = 0; i < adjM.length; ++i) {
			isTra[i] = (false);
		}
		// 遍历每一行
		for (int i = 0; i < adjM.length;) {
			if (isTra[i] == true) {
				++i;
				continue;
			}
			tree.push(i);
			Set<Integer> sub = new HashSet<Integer>();
			sub.add(i);
			int j = tree.peek();
			while (!tree.isEmpty() && isTra[j] == false) {
				j = tree.pop();// 取出栈顶i
				isTra[j] = true;
				// 遍历每一个列
				for (int k = 0; k < adjM[j].length; ++k) {
					if (false == isTra[k]) {
						if (adjM[j][k] == true && isTra[k] == false) {
							tree.push(k);// 若K没有被遍历过，则压入栈
							sub.add(k);
						}
					}
				}
				j = tree.peek();
			}
			++i;
			conSets.add(sub);
		}
		return conSets;
	}
```
注意事项：
       在java中Set类是一个抽象类，不能直接实例化，必须实例化Set的一个子类，比如HashSet。
