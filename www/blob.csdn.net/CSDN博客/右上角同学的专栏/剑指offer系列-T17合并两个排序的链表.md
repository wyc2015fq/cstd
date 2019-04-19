# 剑指offer系列-T17合并两个排序的链表 - 右上角同学的专栏 - CSDN博客
2016年07月11日 21:32:46[右上角徐](https://me.csdn.net/u011032983)阅读数：166
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 *合并两个排序的链表
 *题目描述
 *输入两个单调递增的链表，输出两个链表合成后的链表，
 *当然我们需要合成后的链表满足单调不减规则。
 *思路：本题用循环实现，也可用递归实现。循环实现时
 *要注意先找到头结点再进入循环，递归时也是先找到头
 *节点，因为这样才能求next节点，另外注意，要有两个指针指向合并后链表
 *一个指针做操作，另一个记录头结点！
 */
public class offerT17 {
	/**
	 * @param args
	 */
	 public ListNode Merge(ListNode list1,ListNode list2) {
		 /*当其中一个链表为空*/
		 if(list1==null){
			 return list2;
		 }
		 if(list2==null){
			 return list1;
		 }
		 
		 ListNode list3 = null;//合并后的链表
		 if(list2.val<list1.val){
				list3=list2;
				list2=list2.next;
				}
			 else{
				 list3=list1;
				 list1=list1.next;
				 }
		 ListNode plist3 =list3;//找到头结点，并定义指针plist3指向头结点
		 
		 while(list1!=null&&list2!=null){
				 if(list2.val<list1.val){
					plist3.next=list2;
					list2=list2.next;
					}
				 else{
					 plist3.next=list1;
					 list1=list1.next;
					 }
				 plist3 =plist3.next;
				 }
		 if(list1!=null){
			 plist3.next=list1;
		 }
		 if(list2!=null){
			 plist3.next=list2;
		 }
	        return list3;
	    }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT17 o = new offerT17();
		ListNode node1 = new ListNode(1);
		ListNode node2 = new ListNode(2);
		ListNode node3 = new ListNode(3);
		ListNode node4 = new ListNode(4);
		ListNode node5 = new ListNode(5);
		
		node1.next = node3;
		node2.next = node4;
		node3.next = node5;
		node4.next = null;
		node5.next = null;
		ListNode pnode = o.Merge(node1, node2);
		while(pnode!=null){
			System.out.println(pnode.val);
			pnode=pnode.next;
		}
		/*有趣，为说明ListNode plist3 =list3;后对plist3做操作，会影响list3的值,这涉及到Java值传递引用传递的特性*/
		int[] a={0};
		int[] b=a;
		b[0]=1;
		System.out.println("有趣的例子"+a[0]);
	}
}
```
