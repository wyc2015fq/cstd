# 带权并查集模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月04日 22:28:52[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36








用一个经典的种类并查集做例子，食物链那一题
![代码中一个公式的解释](https://img-blog.csdnimg.cn/20181104222818804.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9vazE5OTg=,size_16,color_FFFFFF,t_70)
```
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=50050;
int n,k,q,a,b;
int p[N];
//num数组 0表示和父节点同类 1表示被父节点吃，2表示吃父节点
//所有取模前提条件都是A吃B B吃C C吃A  长度为3的环
int num[N];
int find(int x){
    if(x==p[x]){
        return x;
    }
    //要先保存这个父节点，否则下面压缩路径后p[x]会变
    int fa=p[x];
    //路径压缩
    p[x]=find(p[x]);
    //关系更新
    num[x]=(num[x]+num[fa])%3;
    return p[x];
}
//真话返回true
bool join(int a,int b,int q){
    int fa = find(a);
    int fb = find(b);
    //同一个根节点
    if(fa == fb){
        //q是操作 1表示同类 2表示a吃b
        //q如果是1 那么a和b就是同类，num值不同即为假话
        //q如果是2 那么a吃b，那么为了使得满足三角关系，b必须吃父节点，也就是num[b]为2,num[a]为1,或者num[b]为0,num[a]为2,即a吃b，a吃fa/fb，b和父节点同类,所以加了个取模
        //这里q-1是因为我们定义0同类 1被吃 比题目输入少1
        if(num[b] != (num[a] + (q - 1)) % 3){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        //b合并到a，所以要更新num[b]
        p[fb] = fa;
        //这个公式可以通过向量的关系图推出（见上图）
        num[fb] = (3-num[b]+num[a]+(q-1))%3;
        //没有关系连通，所以肯定不会矛盾
        return true;
    }
}
int main(void){
    scanf("%d%d",&n,&k);
    for(int i=0;i<n;i++){
        p[i]=i;
    }
    int cnt=0;
    while(k--){
        scanf("%d%d%d",&q,&a,&b);
        //特殊情况
        if(a>n || b>n ||(q==2 && a==b)){
            cnt++;
        }
        else{
            if(!join(a,b,q)){
                cnt++;
            }
        }
    }
    printf("%d\n",cnt);
    return 0;
}
```




