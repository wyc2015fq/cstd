# Lintcode——822. Reverse Order Storage - westbrook1998的博客 - CSDN博客





2018年03月25日 22:41:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：129








**题目要求：将所给链表中的值逆序存在列表中并返回**
**非递归方法：**

直接循环将链表中的值取出，再对列表进行翻转
```java
public class Solution {
    /**
     * @param head: the given linked list
     * @return: the array that store the values in reverse order 
     */
    public List<Integer> reverseStore(ListNode head) {
        // write your code here
        List<Integer> list=new ArrayList<>();
        while(head!=null){
            list.add(head.val);
            head=head.next;
        }
        int i=0;
        int len=list.size();
        while(i<len/2){
            int temp=list.get(i);
            list.set(i,list.get(len-i-1));
            list.set(len-i-1,temp);
            i++;
        }
        return list;
    }
}
```

**递归方法：**

递归到链表的末尾，再回调逆序添加到列表中

```java
public class Solution {
    /**
     * @param head: the given linked list
     * @return: the array that store the values in reverse order 
     */
    public List<Integer> reverseStore(ListNode head) {
        // write your code here
        List<Integer> list=new ArrayList<>();
        addList(head,list);
        return list;
    }

    private void addList(ListNode head,List list){
        if(head.next!=null){
            addList(head.next,list);
        }
        list.add(head.val);
    }
}
```




