# [CareerCup] 11.7 Tower of People in Circus 马戏团的人塔 - Grandyang - 博客园







# [[CareerCup] 11.7 Tower of People in Circus 马戏团的人塔](https://www.cnblogs.com/grandyang/p/4891506.html)







11.7 A circus is designing a tower routine consisting of people standing atop one another's shoulders. For practical and aesthetic reasons, each person must be both shorter and lighter than the person below him or her. Given the heights and weights of each person in the circus, write a method to compute the largest possible number of people in such a tower.
 EXAMPLE:
 Input (ht,wt): (65, 100) (70, 150) (56, 90) (75, 190) (60, 95) (68, 110)
 Output:The longest tower is length 6 and includes from top to bottom: (56, 90) (60,95) (65,100) (68,110) (70,150) (75,190)



这道题说马戏团有一种人塔，上面的人要比下面的人既矮又轻，问我们最多能有多少个人组成人塔。那么就相当于求最长的递增子序列，我们的做法是先将所有的人按身高排个序，方法可参见我之前的博客[C++ sort vector<vector<int> > or vector<MyClass> 容器的排序](http://www.cnblogs.com/grandyang/p/4843528.html)。然后对于体重序列，就相当于找最长连续递增子序列Longest Increasing Subsequence (LIS)，找LIS的方法可参见我之前的博客[Longest Increasing Subsequence 最长递增子序列](http://www.cnblogs.com/grandyang/p/4891500.html)，参见代码如下：



```
class HtWt {
public:
    int Ht, Wt;
    HtWt(int h, int w): Ht(h), Wt(w) {}
    bool isBefore(HtWt *other) {
        if (Ht < other->Ht && Wt < other->Wt) return true;
        else return false;
    }
};

bool cmp(HtWt const *a, HtWt const *b) {
    return a->Ht < b->Ht;
}

class Solution {
public:
    vector<HtWt*> getIncreasingSequence(vector<HtWt*> &items) {
        sort(items.begin(), items.end(), cmp);
        return longestIncreasingSubsequence(items);
    }
    vector<HtWt*> longestIncreasingSubsequence(vector<HtWt*> &array) {
        vector<vector<HtWt*> > solutions;
        longestIncreasingSubsequence(array, solutions, 0);
        vector<HtWt*> res;
        for (auto &a : solutions) {
            res = seqWithMaxLength(res, a);
        }
        return res;
    }
    void longestIncreasingSubsequence(vector<HtWt*> &array, vector<vector<HtWt*> > &solutions, int curIdx) {
        if (curIdx >= array.size() || curIdx < 0) return;
        HtWt *cur = array[curIdx];
        vector<HtWt*> res;
        for (int i = 0; i < curIdx; ++i) {
            if (array[i]->isBefore(cur)) {
                res = seqWithMaxLength(res, solutions[i]);
            }
        }
        vector<HtWt*> new_solution = res;
        new_solution.push_back(cur);
        solutions.push_back(new_solution);
        longestIncreasingSubsequence(array, solutions, curIdx + 1);
    }
    vector<HtWt*> seqWithMaxLength(vector<HtWt*> seq1, vector<HtWt*> seq2) {
        if (seq1.empty()) return seq2;
        if (seq2.empty()) return seq1;
        return seq1.size() > seq2.size() ? seq1 : seq2;
    }
};
```














