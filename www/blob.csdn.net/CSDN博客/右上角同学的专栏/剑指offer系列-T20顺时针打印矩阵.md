# 剑指offer系列-T20顺时针打印矩阵 - 右上角同学的专栏 - CSDN博客
2016年07月12日 22:23:33[右上角徐](https://me.csdn.net/u011032983)阅读数：193
        本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.ArrayList;
/**
 * @author xhl
 *	顺时针打印矩阵
 *	题目描述
     输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，
     如果输入如下矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次
     打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10
 */
public class offerT20 {
	/**
	 * @param args
	 */
	/*通过观察发现每一圈开始的元素行号和列号相同，定为start，通过控制start，控制有几个圈*/
	public ArrayList<Integer> printMatrix(int [][] matrix) {
		if(matrix==null){
			return null;
		}
		int start=0;
		ArrayList<Integer> list=new ArrayList<Integer>();
		//圈的个数控制：最里面的圈满足start<<0x1<matrix.length&&start<<0x1<matrix[0].length
		while(start<<0x1<matrix.length&&start<<0x1<matrix[0].length){//此处需要注意不能用start<matrix.length/2,因为start偶数时start<matrix.length/2，奇数时start<=matrix.length/2
			printMatrixCircle(matrix,start,list);
			start++;
		}
		return list;
	       
    }
	/*每一圈的实现，分为4个步骤，分别得到四个边上的值，因为一直start，即上边界和左边界，还需得到右边和下边的边界*/
	void printMatrixCircle(int [][]matrix,int start,ArrayList<Integer> list){
		int endx=matrix.length-1-start;
		int endy=matrix[0].length-1-start;
		//第一层
		for(int i=start;i<=endy;i++){
			list.add(matrix[start][i]);
		}
		//第二层
		for(int i=start+1;i<=endx;i++){
			list.add(matrix[i][endy]);
		}
		//第三层,要判断是否为单行，单行就不需要第3层了
		if(start<endx){
		for(int i=endy-1;i>=start;i--){
			list.add(matrix[endx][i]);
		}
		}
		//第四层，要判断是否为单列，单行就不需要第4层了
		if(start<endy){
		for(int i=endx-1;i>start;i--){
			list.add(matrix[i][start]);
		}
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT20 o = new offerT20();
		int[][] matrix={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
		ArrayList<Integer> list=new ArrayList<Integer>();
		list=o.printMatrix(matrix);
		System.out.println(list);
	}
}
```
