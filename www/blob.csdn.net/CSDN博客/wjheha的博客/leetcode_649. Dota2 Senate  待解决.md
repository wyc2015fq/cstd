# leetcode_649. Dota2 Senate ? 待解决 - wjheha的博客 - CSDN博客
2017年11月30日 09:06:11[wjheha](https://me.csdn.net/wjheha)阅读数：166
In the world of Dota2, there are two parties: the Radiant and the Dire.
The Dota2 senate consists of senators coming from two parties. Now the senate wants to make a decision about a change in the Dota2 game. The voting for this change is a round-based procedure. In each round, each senator can exercise one of the two rights:
Ban one senator’s right:  
A senator can make another senator lose all his rights in this and all the following rounds. 
Announce the victory:  
If this senator found the senators who still have rights to vote are all from the same party, he can announce the victory and make the decision about the change in the game. 
Given a string representing each senator’s party belonging. The character ‘R’ and ‘D’ represent the Radiant party and the Dire party respectively. Then if there are n senators, the size of the given string will be n.
The round-based procedure starts from the first senator to the last senator in the given order. This procedure will last until the end of voting. All the senators who have lost their rights will be skipped during the procedure.
Suppose every senator is smart enough and will play the best strategy for his own party, you need to predict which party will finally announce the victory and make the change in the Dota2 game. The output should be Radiant or Dire.
Example 1: 
Input: “RD” 
Output: “Radiant” 
Explanation: The first senator comes from Radiant and he can just ban the next senator’s right in the round 1.  
And the second senator can’t exercise any rights any more since his right has been banned.  
And in the round 2, the first senator can just announce the victory since he is the only guy in the senate who can vote.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/dota2-senate/description/](https://leetcode.com/problems/dota2-senate/description/)
***思路***：
