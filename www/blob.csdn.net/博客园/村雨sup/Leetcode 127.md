# Leetcode 127 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        vector<int> vis(wordList.size(),0);
        int res = INT_MAX;
        DFS(beginWord,endWord,wordList,res,vis);
        if(res == INT_MAX) return 0;
        else return res+1;
    }
    bool comp(string s1,string s2){
        int cnt = 0;
        for(int i=0;i < s1.size();i++){
            if(s1[i] != s2[i]) cnt++;
        }
        if(cnt == 1) return true;
        else return false;
    }
    
    void DFS(string beginWord, string endWord, vector<string> wordList,int& res,vector<int> vis){
        if(beginWord == endWord){
            int cnt = 0;
            for(int i=0;i < vis.size();i++){
                if(vis[i] == 1)cnt++;
            }
            res = min(res,cnt);
        }
        for(int i=0;i < wordList.size();i++){
            if(!vis[i] && comp(beginWord,wordList[i])){
                vis[i] = 1;
                DFS(wordList[i],endWord,wordList,res,vis);
                vis[i] = 0;
            }
        }
    }
};
```

_DFS写炸了，研究一下BFS











