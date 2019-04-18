# java位移运算 - Orisun - 博客园







# [java位移运算](https://www.cnblogs.com/zhangchaoyang/articles/3728278.html)





	public static void main(String[] args) {
		int number = -1;
		System.out.printf("number=%x\n", number);// 计算机存储负数里用的是补码形式

		int a = 0x80000001;
		System.out.printf("a=%x\n", a);
		int b = a << 1; // 左移，如果是负数会把符号位移出去
		System.out.printf("b=%x\n", b);

		int c = a >> 1;
		System.out.printf("c=%x\n", c);// 右移，如果是负数最高位的1会向右移，且最高位始终保持为1

		int d = a >>> 1;
		System.out.printf("d=%x\n", d);// 循环右移，如果是负数最高位的1会向右移，且最高位变为0

		int e = 0x7fffffff;
		System.out.printf("e=%x\n", e);
		int f = e << 1;
		System.out.printf("f=%x\n", f);// 左移，正数左移之后有可能把最高位变为1，从而成为负数
	}


输出：

number=ffffffff
a=80000001
b=2
c=c0000000
d=40000000
e=7fffffff
f=fffffffe



来个练习，IP和Int互换

public static int ip2int(String ip) {
		String[] arr = ip.split("\\.");
		int part1 = Integer.parseInt(arr[0]);
		int part2 = Integer.parseInt(arr[1]);
		int part3 = Integer.parseInt(arr[2]);
		int part4 = Integer.parseInt(arr[3]);
		if (part1 >= 0 && part1 < 256 && part2 >= 0 && part2 < 256
				&& part3 >= 0 && part3 < 256 && part4 >= 0 && part4 < 256) {
			// 左移，正数左移之后有可能把最高位变为1，从而成为负数
			int rect = part1 << 24;
			rect += part2 << 16;
			rect += part3 << 8;
			rect += part4;
			return rect;
		} else {
			return -1;
		}
	}

	public static String int2ip(int number) {
		StringBuilder sb = new StringBuilder();
		int part1 = number >>> 24;// 右移，如果是负数最高位的1会向右移，且最高位变为0
		int part2 = (0x00ff0000 & number) >>> 16;// 位移的优先级高于与运算的优先级
		int part3 = (0x0000ff00 & number) >>> 8;
		int part4 = 0x000000ff & number;
		sb.append(String.valueOf(part1));
		sb.append(".");
		sb.append(String.valueOf(part2));
		sb.append(".");
		sb.append(String.valueOf(part3));
		sb.append(".");
		sb.append(String.valueOf(part4));
		return sb.toString();
	}















