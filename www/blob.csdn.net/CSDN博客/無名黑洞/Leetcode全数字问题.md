# Leetcode全数字问题 - 無名黑洞 - CSDN博客
2014年02月07日 12:05:02[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：3007
**目录**
**1、编号7 Reverse integer.2、编号8 String to Integer ATOI3、编号10 Container With Most Water4、编号12 Integer To Roman5、编号13 Roman To Integer6、编号27 Divide Two Integers7、编号37 Count and Say8、编号49 Pow(x, n)9、编号55 Merge Intervals10、编号56 Insert Interval11、编号66 Valid Number12、编号69 Sqrt(x)13、编号78 Subsets14、编号82 Evaluate Reverse Polish Notation15、编号83 Largest Rectangle in Histogram16、编号86 Maximal Rectangle17、编号90 Gray Code18、编号93 Subsets II19、编号118 Pascal's Triangle20、编号119 Pascal's Triangle II21、编号134 Clone Graph22、编号150 Max Points on a Line**
**1、编号7 Reverse integer.**
Reverse digits of an integer.
Example1: x = 123, return 321
Example2: x = -123, return -321 
Have you thought about this? 
Here are some good questions to ask before coding. Bonus points for you if you have already thought through this!
If the integer's last digit is 0, what should the output be? ie, cases such as 10, 100.
Did you notice that the reversed integer might overflow? Assume the input is a 32-bit integer, then the reverse of 1000000003 overflows. How should you handle such cases?
Throw an exception? Good, but what if throwing an exception is not an option? You would then have to re-design the function (ie, add an extra parameter).
比较容易，对10取模可以获得最低位，除以10可以把最低位移去。注意要考虑正负号。
```cpp
class Solution 
{
public:
	int reverse(int x) {
		int sign = 1;
		int y = 0;
		if(x < 0){ x *= -1; sign = -1; }
		/*Processing*/
		int i = 0;
		do{
			/*Find the lowest digit*/
			i++;
			int a = x % 10;
			/*Put the digit to the result*/
			y *= 10;
			y += a;
        
			/*Remove the last digit*/
			x = x / 10;
			if(x == 0) break;
        
		}while(1);
		return y * sign;
	}
};
```
**2、编号8 String to Integer ATOI**
Implement atoi to convert a string to an integer.
Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
Notes: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front. 
spoilers alert... click to show requirements for atoi.
Requirements for atoi: 
The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and
 interprets them as a numerical value.
The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.
If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.
If no valid conversion could be performed, a zero value is returned. If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned. 
实际处理起来要注意几点。一是用long long型来避免overflow。第二是要在一开始除掉空格和判断符号。
```cpp
class Solution 
{
	public:
	int ReturnValue(long long x){
		if(x>INT_MAX) x = INT_MAX;
		if(x<INT_MIN) x = INT_MIN;
		return x;
	}
	int atoi(const char *str) {
		const char* p = str;
		int l = 0;
        
		while(p!=NULL && (*p)!='\0'){
			p++;
			l++;
		};
        
		long long r = 0;/*Longer than integer*/
		int sign = 1;
		bool checkSpace = true;
		bool checkSign = true;
    
		for(int i = 0; i < l; i++){
			char c = str[i];
			/*Remove Space*/
			if(checkSpace){
				if(c == ' ') continue;
				else checkSpace = false;
			}
            
			/*Check Sign*/
			if(checkSign){
				checkSign = false;
				if(c == '-') {
					sign = -1;
					continue;
				}
         
				else if(c == '+'){
					sign = 1;
					continue;
				}
			}
            
			/*Processing*/
			if(c <= '9' && c >= '0'){
				r *= 10;
				r += (c - '0');
			}else break;
		}
		return ReturnValue(r*sign);
    
	}
};
```
**3、编号10 Container With Most Water**
Given n non-negative integers a1, a2, ..., an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such
 that the container contains the most water. 
Note: You may not slant the container. 
只要找两个高，所以从两边往中间找。
```cpp
class Solution {
public:
    int maxArea(vector<int> &height) {
        int l = height.size();
        
        if(l < 2) return 0;
        /*Performance Priority O(n)*/
        int maxArea = 0;
        int i = 0; 
        int j = l-1;
        
        while(i<j){
            
            int area = min(height[i], height[j]) * (j-i);
            if(area > maxArea) maxArea = area;
            
            if(height[i] < height[j]) i++;
            else j--;
            
        }
        
        return maxArea;
    }
};
```
**4、编号12 Integer To Roman**
Given an integer, convert it to a roman numeral. 
Input is guaranteed to be within the range from 1 to 3999.
把阿拉伯数字转化为罗马数字。比如9，首先转化为VIIII的形式，然后合并为IX。合并的时候先补充#号，随后再把#去掉。
```cpp
class Solution {
public:
    string intToRoman(int num) {
        string Romans = "IVXLCDM";
        int RomanIntegers[7] = {1, 5, 10, 50, 100, 500, 1000};
        string result;
        
        /*Compute Basic Roman*/
        for(int i = 6; i >= 0; i--){
            int n = num / RomanIntegers[i];
            for(int j = 0; j < n; j++) result += Romans[i];
            num = num % RomanIntegers[i];
        }
        
        /*Combine all characters to higher level*/
        int l = result.size();
        char lastChar = '#';
        int charCount = 0;
        for(int i = 0; i < l; i++){
            if(result[i] == lastChar) charCount++;
            else charCount = 0;
            
            bool continueEnd = true;
            if(i <= (l-1) && result[i+1] == lastChar) continueEnd = false;
            
            if(continueEnd){
                if(charCount == 4){
                    result[i-4] = GetHigherLevel(lastChar);
                    result[i-3] = '#';
                    result[i-2] = '#';
                    result[i-1] = '#';
                result[i  ] = '#';
                }else if(charCount == 3){
                    if((i >= 4) && result[i-4] == GetHigherLevel(lastChar)){
                        result[i-4] = lastChar;
                        result[i-3] = GetHigherLevel(GetHigherLevel(lastChar));
                        result[i-2] = '#';
                    }else{
                        result[i-3] = lastChar;
                        result[i-2] = GetHigherLevel(lastChar);
                    }
                    result[i-1] = '#';
                    result[i  ] = '#';
                }
                charCount = 0;
            }
            
            lastChar = result[i];
        }
        
        /*Remove # and return*/
        string finalResult;
        for(int i = 0; i < l; i++) if(result[i] != '#') finalResult += result[i];
        return finalResult;
    }
    
    char GetHigherLevel(char x){
        string Romans = "IVXLCDM";
        for(int i = 0; i < Romans.size()-1; i++) if(Romans[i] == x) return Romans[i+1];
        return '#';
    }
};
```
**5、编号13 Roman To Integer**
Given an integer, convert it to a roman numeral. 
Input is guaranteed to be within the range from 1 to 3999.
注意要考虑IV这种情况。也就是一次要综合考虑两个字母。
```cpp
class Solution {
public:
    int romanToInt(string s) {
        string Romans = "IVXLCDM";
        int RomanIntegers[7] = {1, 5, 10, 50, 100, 500, 1000};
        int result = 0;
        
        for(int i = 0; i < s.size(); i++){
            char nextChar = 'N';
            if(i != (s.size()-1)) nextChar = s[i+1];
            result += convert(s[i], nextChar, Romans, 7, RomanIntegers);
        }
        
        return result;
    }
    
    int convert(char x, char x_next, string Romans, int RomanCount, int* RomanIntegers){
        int currentX = 0;
        int nextX = 0;
        
        for(int j = 0; j < RomanCount; j++){
             if(x == Romans[j]) currentX = RomanIntegers[j];
             if(x_next == Romans[j]) nextX = RomanIntegers[j];
        }
        
        if(currentX >= nextX) return currentX;
        else return - currentX;
    }
};
```
**6、编号27 Divide Two Integers**
Divide two integers without using multiplication, division and mod operator.
对dividend和divisor都先转化成long long类型，防止负数转化成正整数时溢出。 dividend/divisor。注意考虑符号。只用减法可以做，但是会超时。这里注意power的用途。
```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        int dividendSign = 1;
        int divisorSign = 1;
        if(dividend < 0) dividendSign = -1;
        if(divisor < 0) divisorSign = -1;
        
        long long dividend_long = abs((double)dividend);
        long long divisor_long = abs((double)divisor);
        int result = 0;
        while (dividend_long >= divisor_long) {
            int count = 0;
            for (long long i = divisor_long; i <= dividend_long; i *= 2) {
                dividend_long -= i;
                result += pow(2, count++);/*Alternative to power: 1 << count*/
            }
        }
        return result * dividendSign * divisorSign;
    }
};
```
**7、编号37 Count and Say**
The count-and-say sequence is the sequence of integers beginning as follows:
1, 11, 21, 1211, 111221, ...
1 is read off as "one 1" or 11.
11 is read off as "two 1s" or 21.
21 is read off as "one 2, then one 1" or 1211.
Given an integer n, generate the nth sequence.
Note: The sequence of integers will be represented as a string.
没啥好说的。
```cpp
class Solution {
public:
    string countAndSay(int n) {
        string result = "1";
        for(int i = 0; i < n-1; i++)
            result = CountAndSayLoop(result);
        return result;
    }
    
    string CountAndSayLoop(string input){
        string result = "";
        char oldNum =  input[0];
        int count = 1;
        for(int i = 1; i < input.length(); i++){
            if(oldNum == (input[i])) count++;
            else{
                result += ('0' + count);
                result += oldNum;
                
                oldNum = input[i];
                count = 1;
            }
            
        }
        
        result += ('0' + count);
        result += oldNum;
        
        return result;
    }
};
```
**8、编号49 Pow(x, n)**
Implement pow(x, n).
这一题用递归做比较快。2^10=(2^5)*(2^5), 2^5 = (2^2)*(2^2)*2
```cpp
class Solution {  
public:  
    double pow(double x, int n) {  
        if(n==0)return 1;  
        if(n==1)return x;  
        double temp = pow(x,abs(n/2));  
        if(n > 0) {  
            if(n&1) return temp*temp*x;  
            else return temp*temp;  
        }  
        else{  
            if(n&1) return 1.0/(temp*temp*x);  
            else return 1.0/(temp*temp);  
        }  
    }  
};
```
**9、编号55 Merge Intervals**
Given a collection of intervals, merge all overlapping intervals.
For example,
Given [1,3],[2,6],[8,10],[15,18],
return [1,6],[8,10],[15,18].
注意先按第一个数字sort。然后比较当前数字的开始和上一个数字的结尾。有两种情况，上个结尾比这个开头大，则merge update。如果上个结尾小，则增加新元素到结果里。
```cpp
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> merge(vector<Interval> &intervals) {
        vector<Interval> r;
        int n = intervals.size();
        if(n == 0 || n == 1) return intervals;
        
        /*Initial*/
        sort(intervals.begin(), intervals.end(), compareFunc);
        addR(r, intervals[0].start, intervals[0].end);
        int lastEnd = intervals[0].end;
                
        /*Core*/
        for(int i = 1; i < n; i++){
            if(intervals[i].start <= lastEnd) 
                lastEnd = updateR(r, intervals[i].end);
            else lastEnd = addR(r, intervals[i].start, intervals[i].end);
        }
    
        return r;
    }
    
    /*Utility*/
    int addR(vector<Interval> &r, int start, int end){
        Interval tmp(start, end);
        r.push_back(tmp);
        return end;
    }
    int updateR(vector<Interval> &r, int end){
        int n = r.size();
        if(end > r[n-1].end){
            r[n-1].end = end;
            return end;
        }
        return r[n-1].end;
    }
    static bool compareFunc(Interval x, Interval y)  {  /*Need use static*/
        return x.start < y.start;  
    }
};
```
**10、编号56 Insert Interval**
Given a set of non-overlapping intervals, insert a new interval into the intervals (merge if necessary).
You may assume that the intervals were initially sorted according to their start times.
Example 1:
Given intervals [1,3],[6,9], insert and merge [2,5] in as [1,5],[6,9].
Example 2:
Given [1,2],[3,5],[6,7],[8,10],[12,16], insert and merge [4,9] in as [1,2],[3,10],[12,16].
This is because the new interval [4,9] overlaps with [3,5],[6,7],[8,10].
这是上一题的第二问。不过答案只是增加了一句话而已。
```cpp
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> insert(vector<Interval> &intervals, Interval newInterval) {
        intervals.push_back(newInterval);
        return merge(intervals);
    }
    
    /*Use old code!*/
    vector<Interval> merge(vector<Interval> &intervals) {
        vector<Interval> r;
        int n = intervals.size();
        if(n == 0 || n == 1) return intervals;
        
        /*Initial*/
        sort(intervals.begin(), intervals.end(), compareFunc);
        addR(r, intervals[0].start, intervals[0].end);
        int lastEnd = intervals[0].end;
                
        /*Core*/
        for(int i = 1; i < n; i++){
            if(intervals[i].start <= lastEnd) 
                lastEnd = updateR(r, intervals[i].end);
            else lastEnd = addR(r, intervals[i].start, intervals[i].end);
        }
    
        return r;
    }
    
    /*Utility*/
    int addR(vector<Interval> &r, int start, int end){
        Interval tmp(start, end);
        r.push_back(tmp);
        return end;
    }
    int updateR(vector<Interval> &r, int end){
        int n = r.size();
        if(end > r[n-1].end){
            r[n-1].end = end;
            return end;
        }
        return r[n-1].end;
    }
    static bool compareFunc(Interval x, Interval y)  {  /*Need use static*/
        return x.start < y.start;  
    }
};
```
**11、编号66 Valid Number**
Validate if a given string is numeric.
Some examples:
"0" => true
" 0.1 " => true
"abc" => false
"1 a" => false
"2e10" => true
Note: It is intended for the problem statement to be ambiguous. You should gather all requirements up front before implementing one.
这一题难在可能会遗漏。特别是科学记数法，7.823E5和1.2e−4这种都是有效的。
```cpp
class Solution {
public:
    bool isNumber(const char *s) {
        int eNum = 0;
        int dotNum = 0;
        char lastChar = '\0';
        bool isBegin = true;
        bool isEnd = false;
        bool hasE = false;
        
        while(*s != '\0'){
            if(!CheckCharValid(*s)) return false;
            
            /*Check Dot*/
            if(*s == '.') {
                if(hasE) return false;
                if(!CheckNumValid(lastChar) && !CheckNumValid(*(s+1))) return false;
                dotNum++;
            }
            if(dotNum > 1) return false;
            
            /*Check e*/
            if(*s == 'e') {
                hasE = true;
                if(lastChar == '\0' || lastChar == ' ' || lastChar == '+' || lastChar == '-' 
                    || !CheckNumPlusMinusValid(*(s+1))) 
                    return false;
                eNum++;
            }
            if(eNum > 1) return false;
            
            /*Check plus minus*/
            if(*s == '-' || *s == '+'){
                if(lastChar != '\0' && lastChar != ' ' && lastChar != 'e') return false;
                if(!CheckNumDotValid(*(s+1))) return false;
            }
            
            /*Check space*/
            if(*s == ' '){ 
                if(!isBegin) isEnd = true; 
            }else{
                if(isEnd == true) return false;
                isBegin = false;
            }
            
            lastChar = *s;
            s++;
        }
        
        if(lastChar == 'e') return false;
        if(isBegin) return false;
        
        return true;
    }
    
     /*Check if it contains all valid char: number, dot, e*/
    char CheckCharValid(char x){
        if((x >= '0' && x <= '9') || (x == '.') || (x == 'e') || (x == ' ') \
            || (x == '-') || (x == '+')) return true;
        else return false;    
    }
    
    char CheckNumValid(char x){
        if(x >= '0' && x <= '9') return true;
        else return false;
    }
    
    char CheckNumPlusMinusValid(char x){
        if((x >= '0' && x <= '9') || (x == '+') || (x == '-')) return true;
        else return false;
    }
    
    char CheckNumDotValid(char x){
        if((x >= '0' && x <= '9') || (x == '.')) return true;
        else return false;
    }
```
**12、编号69 Sqrt(x)**
Implement int sqrt(int x).
[分析点这里](http://blog.csdn.net/wangxiaojun911/article/details/18203333)
```cpp
class Solution {
public:
    const float EPS = 0.00001;
    int sqrt(int x) {
        if(x == 0) return 0;
        
        double result = x; /*Use double to avoid possible overflow*/
        double lastValue;
        
        do{
            lastValue = result;
            result = result / 2.0f + x / 2.0f / result;
        }while(abs(result - lastValue) > EPS);
        
        return (int)result;
    }
};
```
**13、编号78 Subsets**
Given a set of distinct integers, S, return all possible subsets.
Note:
Elements in a subset must be in non-descending order.
The solution set must not contain duplicate subsets.
找子集的一般就DFS好了。
```cpp
class Solution {
public:
    vector<vector<int> > subsets(vector<int> &S) {
        vector<vector<int> > result;
        vector<int> oneResult;
        
        vector<int> empty;
        result.push_back(empty);
        
        sort(S.begin(), S.end());
        Insert(S, 0, oneResult, result);
            
        return result;
    }
    
    void Insert(vector<int> S, int level, vector<int> &oneResult,  vector<vector<int> > &result){
        for(int i = level; i < S.size(); i++){
            oneResult.push_back(S[i]);
            result.push_back(oneResult); /*Change in the insert function to make it faster*/
            if(i < S.size()-1)  Insert(S, i+1, oneResult, result);
            oneResult.pop_back();
        }
    }
};
```
**14、编号82 Evaluate Reverse Polish Notation**
Evaluate the value of an arithmetic expression in Reverse Polish Notation.
Valid operators are +, -, *, /. Each operand may be an integer or another expression.
Some examples:
  ["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) -> 9
  ["4", "13", "5", "/", "+"] -> (4 + (13 / 5)) -> 6
笔者写这篇文章时，这是Leetcode的最后一题（第一百五十题）。但是因为一些错误，被编号成了82，那么就放在这里吧。虽然是最后一题，但是只要知道polish notation是什么东西的话，比第一题还容易。
```cpp
class Solution {
public:
    int evalRPN(vector<string> &tokens) {
        if(tokens.size() == 0) return 0;
        
        stack<int> s;
        for(int i = 0; i < tokens.size(); i++){
            if(tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/"){
                int x = s.top();
                s.pop();
                int y = s.top();
                s.pop();
                if(tokens[i] == "+") s.push(y + x);
                if(tokens[i] == "-") s.push(y - x);
                if(tokens[i] == "*") s.push(y * x);
                if(tokens[i] == "/") s.push(y / x);
            }else{
                int x = StringToNumber(tokens[i]);
                //int x = stoi(tokens[i]);  //stoi() has the same effect
                s.push(x);
            }
        }
        
        return s.top();
    }
    
    int StringToNumber(string x){//!!!!!!!!!!handle negative number
        int result = 0;
        bool negative = false;
        for(int i = 0; i < x.length(); i++){
            if(x[i] == '-') negative = true;
            else result = result * 10 + x[i] - '0';
        }
        return result * (negative?-1:1);
    }
};
```
**15、编号83 Largest Rectangle in Histogram**
Largest Rectangle in Histogram
Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
Above is a histogram where width of each bar is 1, given height = [2,1,5,6,2,3].
The largest rectangle is shown in the shaded area, which has area = 10 unit.
For example,
Given height = [2,1,5,6,2,3],
return 10.
找直方图里面的最大矩形。要线形答案很困难。以下就是一个方法。
```cpp
class Solution {
public:
    /*Easy to find out O(n*n) method, but.. */
    /*Using stack to get O(n) result: Stack 消除直方图法*/
    int largestRectangleArea(vector<int> &height) {
        vector<int> stack; /*Store index instead of height values*/
        height.push_back(0);/*!!!*/
        int result = 0;
        
        for(int i = 0; i < height.size(); i++){
            if(stack.empty() || height[stack.back()] < height[i]){
                stack.push_back(i);//如果当前元素大于栈顶元素，则入栈
            }else{//如果当前元素小于等于栈顶元素,合并现有栈（到result），直至栈顶元素小于当前元素
                int top = stack.back();/*!!!!!!!*/
                stack.pop_back();
                
                int length;
                if(stack.empty()) length = i;
                else length = i - stack.back() - 1;
                
                result = max(result, height[top] * length);
                i--;
            }
        }
        return result;
    }
};
```
**16、编号86 Maximal Rectangle**
Maximal Rectangle
Given a 2D binary matrix filled with 0's and 1's, find the largest rectangle containing all ones and return its area.
找1组成的最大矩形。其实就是二维的直方图，直接用上一题的结论。
```cpp
class Solution {
public:
    /*Use function in Largest Rectangle in Histogram*/
    /*!!!!!Input is char!!!!!*/
    int maximalRectangle(vector<vector<char> > &matrix) {
        int m = matrix.size();
        if(m == 0) return 0;
        int n = matrix[0].size();/*!!!!forgot [0]!!!!*/
        vector<vector<int>> height;
        
        for(int i = 0; i < m ;i++){
            vector<int> row;
            height.push_back(row);
            /*Construct height. Each row is a histogram*/
            for(int j = 0; j < n; j++){
                if(matrix[i][j] == '0') height[i].push_back(0);
                else if(i == 0) height[i].push_back(1);
                else height[i].push_back(height[i-1][j]+1);
            }
        }
        
        int maxResult = 0;
        for(int i = 0; i < m; i++){
            int r = largestRectangleArea(height[i]);
            if(r > maxResult) maxResult = r;
        }
        return maxResult;
    }
    
    int largestRectangleArea(vector<int> height) {
        vector<int> stack;
        height.push_back(0);
        int result = 0;
        
        for(int i = 0; i < height.size(); i++){
            if(stack.empty() || height[stack.back()] < height[i]){
                stack.push_back(i);
            }else{
                int top = stack.back();
                stack.pop_back();
                
                int length;
                if(stack.empty()) length = i;
                else length = i - stack.back() - 1;
                
                result = max(result, height[top] * length);
                i--;
            }
        }
        
        return result;
    }
};
```
**17、编号90 Gray Code**
The gray code is a binary numeral system where two successive values differ in only one bit.
Given a non-negative integer n representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.
For example, given n = 2, return [0,1,3,2]. Its gray code sequence is:
00 - 0
01 - 1
11 - 3
10 - 2
Note:
For a given n, a gray code sequence is not uniquely defined.
For example, [0,2,3,1] is also a valid gray code sequence according to the above definition.
For now, the judge is able to judge based on one instance of gray code sequence. Sorry about that.
主要是找规律。n=k时的Gray Code，相当于n=k-1时的Gray Code的逆序 加上 1<<k。
```cpp
class Solution {
public:
  vector<int> grayCode(int n) {  
      vector<int> result;      
      result.push_back(0);  
      for(int i=0; i< n; i++) {  
        int highestBit = 1<<i;  
        int len = result.size();  
        for(int i = len-1; i>=0; i--)  
          result.push_back(highestBit + result[i]);  
      }  
      return result;  
    }  
};
```
**18、编号93 Subsets II**
Given a collection of integers that might contain duplicates, S, return all possible subsets.
Note:
Elements in a subset must be in non-descending order.
The solution set must not contain duplicate subsets.
编号78的后续。同样是DFS，不过要加至关重要的一句话。
```cpp
class Solution {
public:
    /*Recursive*/
    vector<vector<int> > subsetsWithDup(vector<int> &S) {
        vector<vector<int> > result;
        vector<int> oneResult;
        
        /*Special*/
        vector<int> empty;
        result.push_back(empty);
    
        sort(S.begin(), S.end());
        Insert(S, 0, oneResult, result);
        
        return result;
    }
    
    void Insert(vector<int> S, int level, vector<int> &oneResult, vector<vector<int>> &result){
        for(int i = level; i < S.size(); i++){
            oneResult.push_back(S[i]);
            result.push_back(oneResult);
            if(i < S.size()-1) Insert(S, i+1, oneResult, result);
            oneResult.pop_back();
            while(i < S.size()-1 && S[i] == S[i+1]) i++;  /*The only different from subset*/
        }
    }
};
```
**19、编号118 Pascal's Triangle**
Given numRows, generate the first numRows of Pascal's triangle.
For example, given numRows = 5,
Return
[
     [1],
    [1,1],
   [1,2,1],
  [1,3,3,1],
 [1,4,6,4,1]
]
这种往两个方向走格子的题目用DP就好。
```cpp
class Solution {
public:
    vector<vector<int> > generate(int numRows) {
        vector<vector<int> > result;
        
        if(numRows == 0) return result;
        
        vector<int> tmp;
        tmp.push_back(1);
        result.push_back(tmp);
        
        if(numRows == 1) return result; 
  
        vector<int> tmp2;
        tmp2.push_back(1);
        tmp2.push_back(1);
        result.push_back(tmp2);
   
        if(numRows == 2) return result;
        
        for(int i = 3; i <= numRows; i++){
            vector<int> tmp;
            tmp.push_back(1);
            for(int j = 2; j < i; j++)
                tmp.push_back(result[result.size()-1][j-2] + result[result.size()-1][j-1]);
            tmp.push_back(1);
            result.push_back(tmp);
        }
        
        return result;
    }
};
```
**20、编号119 Pascal's Triangle II**
Given an index k, return the kth row of the Pascal's triangle.
For example, given k = 3,
Return [1,3,3,1].
Note:
Could you optimize your algorithm to use only O(k) extra space?
一开始用排列组合的数学公式去算。但是总是卡在30/34这个地方，后来分析了一下，是中间计算一个数字的时候overflow了。（已经用了long long）。所以后来老老实实一行一行加。
```cpp
class Solution {
public:
vector<int> getRow(int rowIndex) {  
      vector<int> result;  
      result.resize(rowIndex+2);  
      for(int i = 0; i< rowIndex+2; i++)  result[i] = 0;  
      result[1] = 1;  
      for(int i =0; i< rowIndex; i++)
        for(int j = rowIndex+1; j>0; j--)  
            result[j] = result[j-1] + result[j];  
      result.erase(result.begin());  
      return result;  
    }
};
```
**21、编号134 Clone Graph**
Clone an undirected graph. Each node in the graph contains a label and a list of its neighbors.
OJ's undirected graph serialization:
Nodes are labeled uniquely.
We use # as a separator for each node, and , as a separator for node label and each neighbor of the node.
As an example, consider the serialized graph {0,1,2#1,2#2,2}.
The graph has a total of three nodes, and therefore contains three parts as separated by #.
First node is labeled as 0. Connect node 0 to both nodes 1 and 2.
Second node is labeled as 1. Connect node 1 to node 2.
Third node is labeled as 2. Connect node 2 to node 2 (itself), thus forming a self-cycle.
Visually, the graph looks like the following:
       1
      / \
     /   \
    0 --- 2
         / \
         \_/
这个其实不是数字类的题，但是暂时放在这里吧。图的拷贝用哈希表。
```cpp
class Solution {
public:
    UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node) {
        if(node == NULL) return NULL;
        
        //queue用来做BFS来clone graph
        //queue stores all old nodes
        queue<UndirectedGraphNode *> q;
        //hashmap用来mark已经visited的node
        //hashmap is also a way to get access to new node
        //hashmap<old node, new node>
        unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> h; 
        
        UndirectedGraphNode *newHead = new UndirectedGraphNode(node->label);
        q.push(node);
        h[node] = newHead;
        
        while(!q.empty()){
            node = q.front();
            q.pop();
            for(int i = 0; i < node->neighbors.size(); i++){
                UndirectedGraphNode *neighbor = node->neighbors[i];
                if(h.count(neighbor) == 0){//if node is not in hashmap
                    UndirectedGraphNode *newNode = new UndirectedGraphNode(neighbor->label);
                    h[node]->neighbors.push_back(newNode);
                    h[neighbor] = newNode;
                    q.push(neighbor);
                }else//if node is in hashmap, establish connection
                    h[node]->neighbors.push_back(h[neighbor]);
            }
        }
        
        return newHead;
    }
};
```
**22、编号150 Max Points on a Line**
Given n points on a 2D plane, find the maximum number of points that lie on the same straight line.
这题有点儿意思。首先弄清楚平面几何里直线如何表示。任意一条直线都可以表述为 y = ax + b。假设，有两个点(x1,y1), (x2,y2)，如果他们都在这条直线上则有 y1 = kx1 + b y2 = kx2 + b 由此可以得到关系，k = (y2-y1)/(x2-x1)
另外要明白哈希表的遍历到底怎么实现，下面是有关的方法。
unordered_map<Key,T>::iterator it; 
(*it).first; // the key value (of type Key) 
(*it).second; // the mapped value (of type T) 
(*it); // the "element value" (of type pair<const Key,T>)
```cpp
/**
 * Definition for a point.
 * struct Point {
 *     int x;
 *     int y;
 *     Point() : x(0), y(0) {}
 *     Point(int a, int b) : x(a), y(b) {}
 * };
 */
class Solution {
public:
    int maxPoints(vector<Point> &points) {
		unordered_map<float,int> hashmap;
		int result = 0;
		
		for(int i = 0; i < points.size(); i++){//O(n^2)
			hashmap.clear(); //!!!!!!!!!!
			hashmap[INT_MIN] = 0;
			int duplicate = 1;//!!!!!!!!!
			
			for(int j = 0; j < points.size(); j++){
				if(j == i) continue;
				if(points[i].x == points[j].x && points[i].y == points[j].y){
					duplicate++;
					continue;
				}
				
				//compute k
				float k;
				if(points[i].x == points[j].x) k = INT_MAX;
				else k = (float)(points[j].y - points[i].y)/(points[j].x - points[i].x);
				
				hashmap[k]++;
			}
			
			//!!!!!iterate the hashmap
			for(unordered_map<float, int>::iterator it = hashmap.begin(); it != hashmap.end(); it++)
				if(it->second + duplicate > result)	result = it->second + duplicate;
		}
		
		return result;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
