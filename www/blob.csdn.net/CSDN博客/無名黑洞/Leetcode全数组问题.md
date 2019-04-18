# Leetcode全数组问题 - 無名黑洞 - CSDN博客
2014年02月06日 03:32:47[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：8009
**目录1、编号2 Median of Two Sorted Arrays2、编号15 Long Common Prefix 3、编号25 Remove Element (双指针)4、编号26 Remove Duplicates from Sorted Array (双指针)5、编号32 Search for a Range (折半查找)6、编号33 Search in Rotated Sorted Array7、编号35 Search Insert Position (折半查找)8、编号40 First Missing Positive (桶)9、编号51 Anagrams (Map)10、编号67 Plus One11、编号68 Text Justification12、编号 Sort Colors (三指针)13、编号80 Remove Duplicates from Sorted Array II (双指针)14、编号81 Search in Rotated Sorted Array II15、编号89 Merge Sorted Array16、编号 Longest Consecutive Sequence  (哈希表)17、编号137 Single Number  (位运算)18、编号138 Single Number II (位运算)1、编号2 Median of Two Sorted Arrays**
There are two sorted arrays A and B of size m and n respectively. Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
由于要求时间复杂度是O(log (m+n))，所有带log的时间复杂度一般都要折半查找，这个就是这样。因为有两个数组，一个四个部分，那么每次就把这四个部分丢掉一个。所有要求每个数组的中间元素。代码比较长。注意长度为2或者1的特殊情况。更多信息点这里
```cpp
public:
    double findMedianSortedArrays(int A[], int m, int B[], int n) {
        return FindMedian(A, 0, m, B, 0, n);
    }
    
    double GetMedian(int X[], int start, int end){
        int l = end - start;
        if(l == 0) return 0;
        if(l == 1) return X[start];
        if(l%2 == 0){
            return (X[start+l/2-1] + X[start+l/2])/2.0f;
        }else{
            return X[start+l/2];
        }
    }
    
    double GetMedian(int X[], int start, int end, int extrax){
        int l = end - start;
        int Y[l+1];
        int j = 0;
        bool flag = true;
        for(int i = 0; i < l; i++) {
            if(extrax < X[start + i] && flag){
                Y[j++] = extrax;
                flag = false;
            }
            Y[j++] = X[start+i];
        }
        
        if(flag) Y[j++] = extrax;
        return GetMedian(Y, 0, l+1);
    }
    
    double GetMedian(int X[], int start, int end, int extrax, int extray){
        int l = end - start;
        int Y[l+2];
        int j = 0;
        bool flagX = true;
        bool flagY = true;
        for(int i = 0; i < l; i++) {
            if(extrax < X[start + i] && flagX){
                Y[j++] = extrax;
                flagX = false;
            }    
            if(extray < X[start + i] && flagY){
                Y[j++] = extray;
                flagY = false;
            } 
            Y[j++] = X[start+i];
        }
        
        if(flagX) Y[j++] = extrax;
        if(flagY) Y[j++] = extray;
        
        return  GetMedian(Y, 0, l+2);
    }
    
    int Min(int m, int n){
	return m>n?n:m;
    }
    
    double SortMedian(int x1, int x2, int y1, int y2){
        int mid_max = x2;
        int mid_min = x1;
        
        if(y2 < x2) mid_max = y2;
        if(y1 > x1) mid_min = y1;
       
        return (mid_min+mid_max)/2.0f; 
    }
    
    double FindMedian(int A[], int m_start, int m_end, int B[], int n_start, int n_end) {
        double mA = GetMedian(A, m_start, m_end);
        double mB = GetMedian(B, n_start, n_end);
        int m = m_end - m_start;
        int n = n_end - n_start;
        int min_mn = Min(m,n);
        int r= min_mn/2-1;
        if(r == 0) r = 1;
        
        if(m == 0) return mB;
        if(n == 0) return mA;
        if(m==1 && n==1) return (mA+mB)/2.0f;
        if(m==2 && n==2) return SortMedian(A[m_start], A[m_start+1],B[n_start], B[n_start+1]);
        
        if(m==1) return GetMedian(B, n_start, n_end, A[m_start]);
        if(n==1) return GetMedian(A, m_start, m_end, B[n_start]);
        if(m==2) return GetMedian(B, n_start, n_end, A[m_start], A[m_start+1]);
        if(n==2) return GetMedian(A, m_start, m_end, B[n_start], B[n_start+1]);
        if(mA == mB){
            return mA;
        }else if(mA > mB){
            return FindMedian(A, m_start, m_end-r, B, n_start+r, n_end);
        }else{
            return FindMedian(A, m_start+r , m_end, B, n_start, n_end-r);
        }         
    }
```
**2、编号15 Long Common Prefix **
Write a function to find the longest common prefix string amongst an array of strings. 
求很多数组的最长公共前缀。方法是把第一个数组作为参考数组，以下每一个数组都跟参考数组比较。要是它们的共同数组比参考数组的短，那么就更新共同数组的长度；否则就继续比较下一个数组。
```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string> &strs) {
        if(strs.size() == 0) return "";
        string finalResult;
        int finalResultCount = strs[0].size();/*Use strs[0] as reference*/
        for(int i = 1; i < strs.size(); i++){
            int tmpCount = 0;
            for(int j = 0; j < strs[i].size(); j++){
                if(strs[0].size() >= j){
                    if(strs[0][j] == strs[i][j]) tmpCount++;
                    else break;
                }else tmpCount++;
            }
            if(tmpCount < finalResultCount) finalResultCount = tmpCount;
        }
        /*Generate result*/
        for(int i = 0; i < finalResultCount; i++) finalResult += strs[0][i];
        return finalResult;
    }
};
```
**3、编号25 Remove Element **
Given an array and a value, remove all instances of that value in place and return the new length. The order of elements can be changed. It doesn't matter what you leave beyond the new length. 
比较简单的题。方法是双指针。建立一个insertIndex指针跟i一起遍历。当i遇到要删除的字符时，i走而insertIndex不走。到下一步把i的值盖在insertIndex的地方。
```cpp
class Solution {
public:
    int removeElement(int A[], int n, int elem) {
        int insertIndex = 0;
        int length = n;
        for(int i = 0; i < n; i++){
            A[insertIndex] = A[i];
            if(A[i] == elem)length--;
            else insertIndex++;
        }
        return length;
    }
};
```
**4、编号26 Remove Duplicates from Sorted Array**
Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.
Do not allocate extra space for another array, you must do this in place with constant memory.
For example, Given input array A = [1,1,2],
Your function should return length = 2, and A is now [1,2].
跟上一题差不多的简单题。因为没有比较的数字了，所以用上面一个数字来比较。
```cpp
class Solution {
public:
    int removeDuplicates(int A[], int n) {
        int insertIndex = 1;
        int length = n;
        int lastElem = A[0];
        for(int i = 1; i < n; i++){
            A[insertIndex] = A[i];
            if(A[i] == lastElem)length--;
            else insertIndex++;
            lastElem = A[i];
        }
        return length; 
    }
};
```
**5、编号32 Search for a Range**
Given a sorted array of integers, find the starting and ending position of a given target value.
Your algorithm's runtime complexity must be in the order of O(log n).
If the target is not found in the array, return [-1, -1].
For example, Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4].
时间复杂度为 O(log n)，则用折半查找。
```cpp
class Solution {
public:
    vector<int> searchRange(int A[], int n, int target) {
        vector<int> result;
        result.push_back(Recursive(A, target, 0, n, false));
        result.push_back(Recursive(A, target, 0, n, true));
        return result;
    }
    
    int Recursive(int A[], int target, int start, int end, bool findMax){
        if((end-start)==1 || (end-start)==0){
            if(A[start] == target) return start;
            else return -1;
        }
        int m = (end-start)/2;
            
        if(A[start+m] == target){
            if(!findMax){/*For find min*/
                if(A[start+m-1] == target)
                    return Recursive(A, target, start, start+m, findMax);
                else return start+m;
            }else{/*For find max*/
                if(A[start+m+1] == target)
                    return Recursive(A, target, start+m, end, findMax);
                else return start+m;
            }
        }
        
        /*Goes here if the mid point element is not equal to the target*/
        if(A[start+m] < target)
            return Recursive(A, target, start+m, end, findMax);
        else
            return Recursive(A, target, start, start+m, findMax);
    }
};
```
**6、编号33 Search in Rotated Sorted Array**
Suppose a sorted array is rotated at some pivot unknown to you beforehand.
(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
You are given a target value to search. If found in the array return its index, otherwise return -1.
You may assume no duplicate exists in the array.
题目貌似是要考一个特殊的折半查找。但是没有规定时间效率，所以用O(n)的简单方法就可以过。
```cpp
class Solution {
public:
    int search(int A[], int n, int target) {
        for(int i = 0; i < n; i++){
            if(A[i] == target) return i;
        }
        return -1;
    }
};
```
**7、编号35 Search Insert Position**
Given a sorted array and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.
You may assume no duplicates in the array.
Here are few examples.
[1,3,5,6], 5 → 2
[1,3,5,6], 2 → 1
[1,3,5,6], 7 → 4
[1,3,5,6], 0 → 0
比较简单，用折半查找来做。注意边界条件就可以了。
```cpp
class Solution {
public:
    int searchInsert(int A[], int n, int target) {
       return Recursive(A, 0, n, target); 
    }
    
    int Recursive(int A[], int start, int end, int target){
        int l = end - start;
        if(l==1 || l==0){
            if(A[start] >= target) return start;
            if(A[start] < target) return start+1; /*Becareful here*/
        }
        
        int m = l / 2;
        if(A[start + m] == target) return start + m; 
        if(A[start + m] > target)
            return Recursive(A, start, start+m, target); /*Remeber dont use +-1 here*/
        else
            return Recursive(A, start+m, end, target);
    }
};
```
**8、编号40 First Missing Positive**
Given an unsorted integer array, find the first missing positive integer.
For example,
Given [1,2,0] return 3,
and [3,4,-1,1] return 2.
Your algorithm should run in O(n) time and uses constant space.
比较经典的一类题。因为这题有两个限制，一个是运行时间必须线性，而是不能使用额外的数组空间。但是也提供了有力条件，就是找到最先确实的正整数。所以把原数组当作木桶。如果没有元素缺失的话，每个木桶i都刚好能装大小为i+1的那个数。那么只要尝试把所有的数字装进对应的桶里面，然后再次从前往后遍历整个桶，看哪个桶装的不是正确的数字，那个桶所代表的数字就是缺失的数字。
```cpp
class Solution {
public:
     int firstMissingPositive(int A[], int n) {
         int i = 0;
         while (i < n){
             if (A[i] != (i+1) && A[i] >= 1 && A[i] <= n && A[A[i]-1] != A[i])
                 swap(A[i], A[A[i]-1]);
             else
                 i++;
         }
         for (i = 0; i < n; ++i)
             if (A[i] != (i+1))  return i+1;
         return n+1;
    }
};
```
**9、编号51 Anagrams**
Given an array of strings, return all groups of strings that are anagrams.
Note: All inputs will be in lower-case.
Anagrams题的特点是只考虑字母，不考虑顺序。那么要比较两个字符串是否相等的话，可以先排序，然后Anagrams的字符都会变成一样的字符串。从前往后遍历字符串数组，如果发现没找到过的排序后字符串，则记录。当两次发现同样的排序字符串之后，把它们对应的原字符串加入结果中。如果再次找到一个相同的就只用输入新的那个字符串。所以使用影射map来做，排序后的字符串对应字符串的次序。如果该字符串已经输出过了，则标记为-1。
```cpp
class Solution {
public:
    vector<string> anagrams(vector<string> &strs) {  
        map<string,int> m;  
        vector<string> result;  
        for(int i = 0; i < strs.size(); i++) {  
            string a = strs[i];  
            sort(a.begin(), a.end());  
            if(m.find(a) == m.end()) m[a] = i; 	
            else{  
                int index = m[a];  
                if(index >= 0)  {  
                    result.push_back(strs[index]);  
                    m[a] = -1;  
                }  
                result.push_back(strs[i]);  
            }  
        }  
        return result;  
    }  
};
```
**10、编号67 Plus One**
Given a number represented as an array of digits, plus one to the number.
链表类也有一个类似的加一的题。处理好进位就可以了。
```cpp
class Solution {
public:
    vector<int> plusOne(vector<int> &digits) {
        bool carry = false;
        for(int i = digits.size()-1; i >= 0; i--){
            int x;
            if(i == digits.size()-1) x = digits[i] + 1; /*!!!!*/
            else x = digits[i];
            
            if(carry) x++;
            
            if(x >= 10) {
                digits[i] = x - 10;
                carry = true;
            }else {
                digits[i] = x;
                carry = false;
            }
        }
        
        if(carry) digits.insert(digits.begin(), 1);
        
        return digits;
    }
};
```
**11、编号68 Text Justification**
Given an array of words and a length L, format the text such that each line has exactly L characters and is fully (left and right) justified.
You should pack your words in a greedy approach; that is, pack as many words as you can in each line. Pad extra spaces ' ' when necessary so that each line has exactly L characters.
Extra spaces between words should be distributed as evenly as possible. If the number of spaces on a line do not divide evenly between words, the empty slots on the left will be assigned more spaces than the slots on the right.
For the last line of text, it should be left justified and no extra space is inserted between words.
For example,
words: ["This", "is", "an", "example", "of", "text", "justification."]
L: 16.
Return the formatted lines as:
[
   "This    is    an",
   "example  of text",
   "justification.  "
]
Note: Each word is guaranteed not to exceed L in length.
用贪心算法，在每行放入尽可能多的词组。每次放入一个词组时，有三种情况：1、该行刚好放满。假入这不是最后一个词的话，下一个词就要重起一行。2、该行长度超了。那么要退出这一个单词，新起一行，再把单词放进去。3、该行长度小于最大长度。这个最简单，只要加入这个单词就可以了。另外要注意的是加入单词后要如题目所述调整间距，所以写了SettingSpace()这个函数。应该可以减小代码长度的，但是这题有点恶心。
```cpp
class Solution {
public:
    vector<string> fullJustify(vector<string> &words, int L) {
        vector<string> r;
        string tmp = "";
        r.push_back(tmp);
        int lineCount = 0;
        int stringInLineCount = 0;
        int wordsInLineCount = 0;
        
        if(L == 0) return r;
        /*Even words = [""], it is still words.size() = 1*/
        if(words[0] == ""){
            r[0] = SettingSpace(r[0], 0, 0, L, false);
            return r;
        }
        
        for(int i = 0; i < words.size(); i++){
            stringInLineCount += words[i].length();
            if(stringInLineCount == L){
                /*Add word*/
                r[lineCount] += words[i];
                wordsInLineCount++;
                /*SettingSpace*/
                r[lineCount] = SettingSpace(r[lineCount], stringInLineCount, wordsInLineCount, L, false);
                /*Start a new line if this word is not the last word*/
                if(i != words.size()-1){
                    string tmp = "";
                    r.push_back(tmp);
                    stringInLineCount = 0;
                    wordsInLineCount = 0;
                    lineCount++;
                }
            }else if(stringInLineCount > L){
                /*Remove words*/
                stringInLineCount -= words[i].length();
                /*SettingSpace*/
                r[lineCount] = SettingSpace(r[lineCount], stringInLineCount, wordsInLineCount, L, false);
                /*Start a new line and Add word*/
                string tmp = words[i];
                r.push_back(tmp);
                stringInLineCount = words[i].length();
                wordsInLineCount = 1;
                lineCount++;
                /*After adding word, add a space, if there is word next*/
                if((i != words.size()-1) && (stringInLineCount < L)){
                    stringInLineCount++;
                    r[lineCount] += ' ';
                }
            }else{
                /*Add word*/
                r[lineCount] += words[i];
                wordsInLineCount++;
                /*After adding word, add a space, if there is word next*/
                if((i != words.size()-1) && (stringInLineCount < L)){
                    stringInLineCount++;
                    r[lineCount] += ' ';
                }
            }
        }
        
        /*Need setting space for the last line*/
        r[lineCount] = SettingSpace(r[lineCount], stringInLineCount, wordsInLineCount, L, true);
        
        return r;
    }
    
    string SettingSpace(string x, int stringInLineCount, int wordsInLineCount, int L, bool lastLine){
        int spaceCount = 0;
        int n = x.length();
        //if(x[n-1] == ' ') n--;
        for(int i = 0; i < n; i++) 
            if(x[i] == ' ') spaceCount++;
        spaceCount += (L - stringInLineCount);
        
        if(wordsInLineCount < 2 || lastLine){
            for(int i = stringInLineCount; i < L; i++ ){
                x += ' ';
            }
            return x;
        }else{
            int spaceBetween = spaceCount / (wordsInLineCount-1);
            int extraSpace = spaceCount % (wordsInLineCount-1);
            string tmp;
            int count = 0;
            int spaceNum = 0;
            for(int i = 0; i < L; i++) {
                if(spaceNum > 0){
                    tmp += ' ';
                    spaceNum--;
                    continue;
                }
                if(x[count] != ' '){
                    tmp += x[count];
                    count++;
                }else{
                    tmp += ' ';
                    spaceNum = spaceBetween-1;
                    if(extraSpace > 0) {spaceNum++; extraSpace--;}
                    count++;
                }
            }
            return tmp;
        }
    }
};
```
**12、编号 Sort Colors**
Given an array with n objects colored red, white or blue, sort them so that objects of the same color are adjacent, with the colors in the order red, white and blue.
Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.
Note: You are not suppose to use the library's sort function for this problem.
Follow up:
A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
Could you come up with an one-pass algorithm using only constant space?
一个特殊的排序问题。只扫描一遍，不用辅助数组，把所有0移到前面，并把所有2移到最后。使用三指针法。目标是，第一个指针指向0的终点，第二个指针指向2，最后一个指针用来遍历。中间的位置是1。遍历开始，当三号指针找到0的时候，把0和一号指针的位置交换，然后一号指针往后走一步。当三号指针找到2的时候，把2和二号指针的位置交换，然后二号指针往前走一步。指导二号和三号指针相遇，颜色就排好了。
```cpp
class Solution {
public:
    void sortColors(int A[], int n) {
        int zeroIndex = 0;
        int twoIndex = n-1;
        int i = 0;/*One Index*/
        
        while(i < (twoIndex+1)){
            if(A[i] == 0){
                Swap(A[i], A[zeroIndex]);
                zeroIndex++;
                i++;
                continue;
            }
            if(A[i] == 2){
                Swap(A[i], A[twoIndex]);
                twoIndex--;
                continue;
            }
            i++;
        }
    }
    
    void Swap(int &x, int &y){
        int tmp = x;
        x = y;
        y = tmp;
    }
};
```
**13、编号80 Remove Duplicates from Sorted Array II**
Follow up for "Remove Duplicates":
What if duplicates are allowed at most twice?
For example, Given sorted array A = [1,1,1,2,2,3],
Your function should return length = 5, and A is now [1,1,2,2,3].
编号26的升级版，允许出现两个的复制。思路会有些不同。对于每一个元素，必须同时比较它的上一个和再上一个元素，如果它们都相等，才能删掉其中一个元素。定义一个offset变量，在每个step开始的时候，根据这个变量覆盖原数组。用这个方法，允许出现三个复制的也可以做。
```cpp
class Solution {
public:
    int removeDuplicates(int A[], int n) {
        if(n <= 2) return n;
        
        int lastElem = A[0];
        int offset = 0;
        
        for(int i = 2; i < n;){
            A[i] = A[i+offset];
            if(A[i] == lastElem && A[i-1] == lastElem) {
                offset++;
                n--;
            }else i++;
            lastElem = A[i-2];
        }
        
        return n;
    }
};
```
**14、编号81 Search in Rotated Sorted Array II**
Follow up for "Search in Rotated Sorted Array":
What if duplicates are allowed?
Would this affect the run-time complexity? How and why?
Write a function to determine if a given target is in the array.
编号33的变种。允许两个复制存在。返回的是数据是否存在的bool变量。但是简单的方法仍然可以通过大数据...
```cpp
class Solution {
public:
    bool search(int A[], int n, int target) {
        for(int i = 0; i < n; i++){
            if(A[i] == target) return true;
        }
        return false;      
    }
};
```
**15、编号89 Merge Sorted Array**
Given two sorted integer arrays A and B, merge B into A as one sorted array.
Note:
You may assume that A has enough space to hold additional elements from B. The number of elements initialized in A and B are m and n respectively.
把B数组merge到A数组里面。由于A会被覆盖，所以建立一个辅助数组v，先把merge后的数据放进v，然后再把A同步到v一样就可以了。
```cpp
class Solution {
public:
    void merge(int A[], int m, int B[], int n) {
        int i = 0;
        int j = 0;
        vector<int> v;
        for(int count = 0; count < m+n; count++){
            if(i == m)
                v.push_back(B[j++]);
            else if(j == n)
                v.push_back(A[i++]);
            else{
                if(A[i]<B[j]) v.push_back(A[i++]);
                else v.push_back(B[j++]);
            }
        }
        
        for(int i = 0; i< v.size(); i++) A[i] = v[i];
    }
};
```
**16、编号 Longest Consecutive Sequence**
Given an unsorted array of integers, find the length of the longest consecutive elements sequence.
For example, Given [100, 4, 200, 1, 3, 2],
The longest consecutive elements sequence is [1, 2, 3, 4]. Return its length: 4.
Your algorithm should run in O(n) complexity.
要求了运行时间必须是线性，但没有限制空间。则用哈希表来实现。
```cpp
class Solution {
public:
    int longestConsecutive(vector<int> &num) {
        int result = 0;
        
        unordered_map<int, int> hashMap;
        
        for(int i = 0; i < num.size(); i++) hashMap[num[i]] = 1;
        
        for(int i = 0; i < num.size(); i++){
            if(hashMap[num[i]] == 1){
                hashMap[num[i]] = 0;
                int len = 1;
                int left = num[i]-1;
                int right = num[i]+1;
                while(hashMap.count(left) && hashMap[left]){
                    hashMap[left--] = 0;
                    len++;
                }
                while(hashMap.count(right) && hashMap[right]){
                    hashMap[right++] = 0;
                    len++;
                }
                if(len > result) result = len;
            }
        }
        return result;
    }
};
```
**17、编号137 Single Number**
Given an array of integers, every element appears twice except for one. Find that single one.
Note: Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
找到之出现一次的那个点。无论是排序也好，哈希表也好，都很容易做。但是要同时限制时间和空间，一般方法就不行了。搜了一下，只能用位运算来做。用一个整型result来记录，result^=A[i], 出现两次的话，两次亦或消失为0，只出现一次的被result记录下来，最后返回0^result。
^异或运算 XOR
    1   ^   1   ==   0
    1   ^   0   ==   1
    0   ^   1   ==   1
    0   ^   0   ==   0
```cpp
class Solution {
public:
    int singleNumber(int A[], int n) {
        int result = 0;
        for(int i = 0; i < n; i++) result ^= A[i];
        return result;
    }
};
```
**18、编号138 Single Number II**
Given an array of integers, every element appears three times except for one. Find that single one.
Note: Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
上一题的升级版，更加困难了。方法也还是只能用位运算，如下。
```cpp
class Solution {
public:
    int singleNumber(int A[], int n) {
        int one = 0, two = 0, three = 0;
        for (int i = 0; i < n; ++i) {
            two |= one & A[i]; //出现两次的 就加到B里面
            one ^= A[i]; //从A里面删除 出现两次的
            three = one & two;  //如果是三次的 就会同时出现在A和B里面， 
            one &= ~three; //然后删除A里三次的
            two &= ~three;  //删除B里三次的
        }
        return one;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
