# leetcode_299. Bulls and Cows - wjheha的博客 - CSDN博客
2018年02月08日 16:28:59[wjheha](https://me.csdn.net/wjheha)阅读数：89
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
You are playing the following Bulls and Cows game with your friend: You write down a number and ask your friend to guess what the number is. Each time your friend makes a guess, you provide a hint that indicates how many digits in said guess match your secret number exactly in both digit and position (called “bulls”) and how many digits match the secret number but locate in the wrong position (called “cows”). Your friend will use successive guesses and hints to eventually derive the secret number.
For example:
Secret number:  “1807” 
Friend’s guess: “7810” 
Hint: 1 bull and 3 cows. (The bull is 8, the cows are 0, 1 and 7.) 
Write a function to return a hint according to the secret number and friend’s guess, use A to indicate the bulls and B to indicate the cows. In the above example, your function should return “1A3B”.
Please note that both secret number and friend’s guess may contain duplicate digits, for example:
Secret number:  “1123” 
Friend’s guess: “0111” 
In this case, the 1st 1 in friend’s guess is a bull, the 2nd or 3rd 1 is a cow, and your function should return “1A1B”. 
You may assume that the secret number and your friend’s guess only contain digits, and their lengths are always equal.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/bulls-and-cows/description/](https://leetcode.com/problems/bulls-and-cows/description/)
**思路**：这个题目大意很简单，就是统计两个字符串对应位字符相等的个数，即为bull；剩下的对应位不相等的字符中，再统计出出现的公有字符的个数，记为cows。
代码如下：
```
public String getHint(String secret, String guess) {
        if(secret == null || guess == null) {
            return String.valueOf(-1);
        }
        HashMap<Character,Integer> hm = new HashMap<Character,Integer>();
        int bull= 0;
        int cows = 0;
        for(int i = 0; i < secret.length(); i++) {
            if(secret.charAt(i) == guess.charAt(i)) {
                bull++;
            } else {
                if(!hm.containsKey(secret.charAt(i))) {
                    hm.put(secret.charAt(i), 1);
                } else {
                    hm.put(secret.charAt(i), hm.get(secret.charAt(i)) + 1);
                }
            }
        }
        for(int i = 0; i < guess.length(); i++) {
            if(hm.containsKey(guess.charAt(i)) && guess.charAt(i) != secret.charAt(i)) {
                cows++;
                hm.put(guess.charAt(i), hm.get(guess.charAt(i)) - 1);
                if(hm.get(guess.charAt(i)) == 0) {
                    hm.remove(guess.charAt(i));
                }
            }
        }
        String s = String.valueOf(bull) + "A" +  String.valueOf(cows) + "B";
        return s;
}
```
代码中使用了hash表来存储，也可以直接使用一个长度为10的数组，因为字符串中只有字母0-9，把数字作为数组下表，数组的值secret中对应字母出现的次数，这种做法本质上就是构建了一个hash表，只是会更简单一些！
