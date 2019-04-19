# leetcode_552.Student Attendance Record II ? 待解决 - wjheha的博客 - CSDN博客
2017年10月25日 09:28:08[wjheha](https://me.csdn.net/wjheha)阅读数：97
Student Attendance Record II
Given a positive integer n, return the number of all possible attendance records with length n, which will be regarded as rewardable. The answer may be very large, return it after mod 109 + 7.
A student attendance record is a string that only contains the following three characters:
‘A’ : Absent. 
‘L’ : Late. 
‘P’ : Present. 
A record is regarded as rewardable if it doesn’t contain more than one ‘A’ (absent) or more than two continuous ‘L’ (late).
Example 1: 
Input: n = 2 
Output: 8  
Explanation: 
There are 8 records with length 2 will be regarded as rewardable: 
“PP” , “AP”, “PA”, “LP”, “PL”, “AL”, “LA”, “LL” 
Only “AA” won’t be regarded as rewardable owing to more than one absent times.  
Note: The value of n won’t exceed 100,000.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/student-attendance-record-ii/description/](https://leetcode.com/problems/student-attendance-record-ii/description/)
***思路***：
