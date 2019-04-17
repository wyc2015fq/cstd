# Leetcode 1002. 查找常用字符 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1002. 查找常用字符

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-126/problems/find-common-characters/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-126/)







- 用户通过次数301
- 用户尝试次数324
- 通过次数303
- 提交次数480
- 题目难度Easy



给定仅有小写字母组成的字符串数组 `A`，返回列表中的每个字符串中都显示的全部字符（包括重复字符）组成的列表。例如，如果一个字符在每个字符串中出现 3 次，但不是 4 次，则需要在最终答案中包含该字符 3 次。

你可以按任意顺序返回答案。



示例 1：
输入：["bella","label","roller"]
输出：["e","l","l"]

示例 2：
输入：["cool","lock","cook"]
输出：["c","o"]



提示：
- `1 <= A.length <= 100`
- `1 <= A[i].length <= 100`
- `A[i][j]` 是小写字母

```
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
        vector<vector<int>> m(A.size(),vector<int>(26,0));
        for(int i=0;i < A.size();i++){
            for(int j=0;j < A[i].size();j++){
                m[i][A[i][j]-97]++;
                // cout << A[i][j] << " " << A[i][j]-97 << " ";
            }
        }
        vector<string> res;
        for(int i=0;i < A[0].size();i++){
            
            int flag = 1;
            for(int k=0;k < m.size();k++){
                if(!m[k][A[0][i]-97]){flag=0;break;} //没有这个char
            }
            if(flag){
                for(int k=0;k < m.size();k++){
                    m[k][A[0][i]-97]--;
                }
                cout << A[0][i] << " ";
                string temp = " ";temp[0] = A[0][i];
                res.push_back(temp);
            }
        }
        
        
        return res;
        
    }
};
```

_屎山？？

```
class Solution {
public:
    int cnt[26],num[26];
    vector<string> commonChars(vector<string>& A) {
        int i;
        for (i=0;i<26;++i) cnt[i]=100000;
        for (auto s:A){
            for (i=0;i<26;++i) num[i]=0;
            for (int i=0;i<(int)s.length();++i){
                num[s[i]-'a']++;
            }
            for (i=0;i<26;++i) cnt[i]=min(cnt[i],num[i]); // 数到的重复的个数取最少的
        }
        vector<string> vec;
        for (i=0;i<26;++i)if(cnt[i]>0){
            string s="";
            char ch='a'+i;
            s=s+ch;
            for (int j=0;j<cnt[i];++j){
                vec.push_back(s);
            }
        }
        return vec;
    }
};
```

——这个复杂度比我低挺多了。














