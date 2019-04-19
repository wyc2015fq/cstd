# C++算法之-小球问题 - fanyun的博客 - CSDN博客
2018年07月29日 21:33:01[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：364
1、问题：
        一条直线上有n个小球，初始坐标及速度已知，且球的初始位置两两不相同。若任意两个球相遇后则两球均消失，找出最后还留在直线上的球。
2、算法思路：
        任意两个小球之间无非两种情况（一起消失、永不相遇）。那么最先相遇的必然是两个相邻的小球，排除掉这俩球后就又回到了最初的状态。所以把所有球按坐标从小到大排序，用带有排序功能的容器保存相邻小球相遇的时间，每次去掉最先相遇的小球直到剩下的球永不相遇或全部消失。时间复杂度为：O(n log n)
         其实想起来不是很难，但是实际实现你会发现，还是有很多细节需要考虑的。举个例子，若当前剩下球a、b、c、d，b和c最先相遇，去掉（b，c）后如何快速定位并删除（a，b）和（c，d）并构建（a，d）。
 3、代码实现
```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define urp(i,a,b) for(int i=(a),__tzg_##i=(b); i>=__tzg_##i; --i)
#define rp(i,b) for(int i=(0), __tzg_##i=(b);i<__tzg_##i;++i)
#define rep(i,a,b) for(int i=(a), __tzg_##i=(b);i<__tzg_##i;++i)
#define repd(i,a,b) for(int i=(a), __tzg_##i=(b);i<=__tzg_##i;++i)
#define mst(a,b) memset(a,b,sizeof(a))
#define px first
#define py second
#define __0(x) (!(x))
#define __1(x) (x)
typedef pair<int,int> pii;
const ll mod = 1000000007;
const int MAXM = 15;
const double eps = 1e-8;
const int inf = 0x3f3f3f3f;
#define mp(a,b) make_pair(a,b)
typedef vector<int> VI;
typedef vector<ll> VL;
typedef vector<pii> VPII;
typedef vector<string> VS;
typedef vector<VI> VVI;
typedef vector<VL> VVL;
const int N = 20;
const double INF = 1e100;
inline int cmp(double a, double b) {
    return (a-b>eps) - (a-b<-eps);
}
inline int cmp(double a) {
    return cmp(a, 0.0);
}
class Node;
class QueueNode;
class Calc;
typedef list<Node>::iterator LT;
typedef set<QueueNode>::iterator ST;
class Node {
public:
    double pos_;
    double speed_;
    int removed_;
    int idx_;
    Node(double pos, double speed, int idx) {
        this->pos_ = pos;
        this->speed_ = speed;
        this->idx_ = idx;
        removed_ = 0;
    }
    int neverMeet(const Node & nd) const {
        int b = cmp(pos_, nd.pos_), c = cmp(speed_, nd.speed_);
        if (b == 0)
            return 0;
        if (c == 0)
            return 1;
        if (b > 0 && c>=0)
            return 1;
        if (b < 0 && c<=0)
            return 1;
        return 0;
    }
    double getTime(const Node & nd) const {
        if (neverMeet(nd))
            return INF;
        return fabs(pos_ - nd.pos_)/fabs(speed_ - nd.speed_);
    }
    bool operator < (const Node & a) const {
        return cmp(pos_, a.pos_) < 0;
    }
};
 
class QueueNode {
public:
    LT t1_, t2_;
    double time_;
    int id_;
    QueueNode(LT t1, LT t2, int id) {
        this->t1_ = t1;
        this->t2_ = t2;
        this->id_ = id;
        time_ = t1_->getTime(*t2_);
    }
    bool operator < (const QueueNode & nd) const {
        int k = cmp(time_, nd.time_);
        return k==0?id_<nd.id_:k<0;
    }
};
class Calc {
public:
    VI solve(vector<double> speed, vector<double> pos) {
        map<pair<Node*,Node*>, ST> mmp;
        set<QueueNode> sq;
        vector<Node> nl;
        int n = speed.size();
        rp(i, n) {
            nl.push_back(Node(pos[i], speed[i], i));
        }
        sort(nl.begin(), nl.end());
        list<Node> vn;
        rp(i, n) {
            vn.push_back(nl[i]);
        }
        LT it = vn.begin();
        int id = 1;
        for (++it; it != vn.end(); ++it) {
            LT yt = it;
            --yt;
            QueueNode nd(yt, it, id++);
            sq.insert(nd);
            ST t = sq.find(nd);
            mmp[make_pair(&*yt, &*it)] = t;
        }
        while (!sq.empty()) {
            ST t = sq.begin();
            QueueNode qd = *t;
            if (qd.time_ >= INF)
                break;
            LT t1 = qd.t1_, t2 = qd.t2_, p = vn.end(), q = vn.end();
            if (t1 != vn.begin()) {
                p = t1;
                --p;
                sq.erase(mmp[mp(&*p, &*t1)]);
            }
            sq.erase(mmp[mp(&*t1, &*t2)]);
            t1->removed_ = 1;
            t2->removed_ = 1;
            q = t2;
            ++q;
            if (q != vn.end() ) {
                sq.erase(mmp[mp(&*t2, &*q)]);
            }
            if (p != vn.end() && q != vn.end()) {
                QueueNode tp(p, q, qd.id_);
                sq.insert(tp);
                mmp[mp(&*p, &*q)] = sq.find(tp);
            }
        }
        VI res;
        for(LT i = vn.begin(); i != vn.end(); ++i) {
            if (!i->removed_)
                res.push_back(i->idx_);
        }
        return res;
    }
};
int main() {
    Calc c;
    VI res = c.solve({-1,0,1,2}, {1,2,-3,-1});
    cout<<res.size()<<endl;
    rp(i, res.size()) {
        cout<<res[i]<<endl;
    }
    return 0;
}
```
