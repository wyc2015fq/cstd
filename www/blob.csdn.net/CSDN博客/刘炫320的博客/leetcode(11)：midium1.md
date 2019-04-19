# leetcode(11)：midium1 - 刘炫320的博客 - CSDN博客
2018年03月20日 15:37:45[刘炫320](https://me.csdn.net/qq_35082030)阅读数：94标签：[最长回文子串																[数组最值																[字符数字转换																[快慢指针](https://so.csdn.net/so/search/s.do?q=快慢指针&t=blog)](https://so.csdn.net/so/search/s.do?q=字符数字转换&t=blog)](https://so.csdn.net/so/search/s.do?q=数组最值&t=blog)](https://so.csdn.net/so/search/s.do?q=最长回文子串&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
这次我们进入到中级的水平的题目。
# 1leetcode#5. Longest Palindromic Substring
## 1.1题目描述
> 
Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.
Example:
> 
Input: “babad” 
  Output: “bab” 
  Note: “aba” is also a valid answer. 
  Example: 
  Input: “cbbd” 
  Output: “bb”
## 1.2思路
这是找最长回文子串的任务，主要思想就是，一定有一个记录最长长度和回文的地方，另外就是需要去判断是否回文。最朴素的方法是，对于每一个数字，对左和右进行比较，记录最长的即可。一个加速的小trick就是当两个一致的时候需要从最边上看，并且下一次也不用看相同的数字了。
## 1.3代码
```java
public String longestPalindrome(String s) {
     if (s.length()<=1) return s;
        //开始和结束位置
        int[] range= new int[]{0,1};
        char[] ca= s.toCharArray();
        for (int i=0; i<ca.length; i++) {
            //可以加速查找
            i=helper(ca, range, i);
        }
        return s.substring(range[0], range[1]);
    }
    private int helper(char[] ca, int[] range, int index){
        int lo=index, hi=index;
        //往前找到第一个不一样的地方
        while(hi<ca.length-1 && ca[hi]==ca[hi+1]) hi++;
        //下一个搜索的位置
        int ret=hi;
        //从两边对比看是否一致
        while(hi<ca.length-1 && lo>0 && ca[hi+1]==ca[lo-1]){
            hi++;
            lo--;
        }
        //如果大于最长的那个
        if (hi-lo+1>range[1]-range[0]){
            range[0]=lo;
            range[1]=hi+1;
        }
        //返回下次寻找地址
        return ret;
    }
```
# 2.leetcode#11. Container With Most Water
## 2.1问题描述
> 
Given n non-negative integers a1, a2, …, an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water. 
  Note: You may not slant the container and n is at least 2.
## 2.2思路
这道题是讲两条线与X轴所组成的区域所装的最多水。也就是说，装最多水取决于两条线中短的那个以及两条线之间的距离。根据这两个条件，我们可以做一下思路，从首尾开始遍历，之所以这样是因为首尾的距离最长，然后记录一个最大值，再看左右两边那个短，记录最短的，在缩短距离的时候，只有最短的那个更长，才有可能比最多水装的更多，需要比较一下，否则直接跳过即可。
## 2.3代码
```java
public int maxArea(int[] height) {
         int left = 0; int right = height.length-1;
        int maxArea = 0;
        while (left < right) {
            //获得高
            int h = Math.min(height[left], height[right]);
            //获得当前容量
            int area = h*(right-left);
            //如果容量大于最大值，就替换掉
            if (area > maxArea) maxArea = area;
            //如果左边小于右边的
            if (height[left] < height[right]) {
                //当前最小值为curr
                int curr = height[left];
                //如果左边小于右边，并且前面一直小于curr，则左边移动
                while (left < right && height[left] <= curr) ++left;
            }
            else if (height[left] > height[right]) {
                //否则右边移动
                int curr = height[right];
                while (left < right && height[right] <= curr) --right;
            }
            else {
                //两边都相等的话，随便抽取一个
                int curr = height[left];
                //如果左边变小就加左边的
                while (left < right && height[left] <= curr) ++left;
                //有过右边变小，就加右边的。
                while (left < right && height[right] <= curr) --right;
            }
        }
        return maxArea;
    }
```
# 3.leetcode#17. Letter Combinations of a Phone Number
## 3.1问题描述
> 
Given a digit string, return all possible letter combinations that the number could represent. 
  A mapping of digit to letters (just like on the telephone buttons) is given below. 
## 3.2思路
这道题是比较简单的，使用简单的递推就可以完成。
## 3.3代码
```java
public List<String> letterCombinations(String digits) {
        List<String> result=new ArrayList<>();
        if(digits==null||digits.length()==0) return result;
        char[] string=new char[digits.length()];
        createString(digits, 0, string, result);
        return result;
    }
    public void createString(String digits,int start,char[] string,List<String> result){
        int number=Integer.parseInt(String.valueOf(digits.charAt(start)));
        char[] condition=getChars(number);
        //如果到了最后一个
        if(start==digits.length()-1){
            //填充位置
            for(char chars : condition){
                string[start]=chars;
                result.add(new String(string));
            }
        }
        else{
            for(char chars : condition){
                string[start]=chars;
                createString(digits, start+1, string, result);
            }
        }
    }
    public char[] getChars(int number){
        switch (number) {
            case 2:
                return new char[]{'a','b','c'};
            case 3:
                return new char[]{'d','e','f'};
            case 4:
                return new char[]{'g','h','i'};
            case 5:
                return new char[]{'j','k','l'};
            case 6:
                return new char[]{'m','n','o'};
            case 7:
                return new char[]{'p','q','r','s'};
            case 8:
                return new char[]{'t','u','v'};
            case 9:
                return new char[]{'w','x','y','z'};
            default:
                return new char[]{};
            }
    }
```
# 4.leetcode#19. Remove Nth Node From End of List
## 4.1问题描述
> 
Given a linked list, remove the nth node from the end of list and return its head.
Example
> 
For example, 
     Given linked list: 1->2->3->4->5, and n = 2. 
     After removing the second node from the end, the linked list becomes 1->2->3->5. 
  Note: 
  Given n will always be valid. 
  Try to do this in one pass.
## 4.2思路
使用快慢指针就可以完美解决，先让快指针走N个，然后在同步，当快指针走到头时，慢指针就到了N个了。这里需要注意的是，它需要去掉第K个，而不是取第K个，因此需要指向K个前一个节点。
## 4.3代码
```java
public ListNode removeNthFromEnd(ListNode head, int n) {
        ListNode start = new ListNode(0);
        ListNode slow = start, fast = start;
        slow.next = head;
        //快指针先走n步
        for(int i=1; i<=n+1; i++)   {
            fast = fast.next;
        }
        //快慢指针同时走
        while(fast != null) {
            slow = slow.next;
            fast = fast.next;
        }
        //把跳过下一个节点
        slow.next = slow.next.next;
        //返回head节点
        return start.next;
    }
```
# 5.小结
这4题都是Medium难度的题目，但是看起来似乎还没有前面几个Easy的题目难，主要讲了回文的判断、数组的特定最值、字符转换、快慢指针等。还是要融会贯通一下。
