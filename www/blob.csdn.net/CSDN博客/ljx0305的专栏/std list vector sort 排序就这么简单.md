# std list/vector sort 排序就这么简单 - ljx0305的专栏 - CSDN博客
2012年08月15日 10:29:05[ljx0305](https://me.csdn.net/ljx0305)阅读数：897标签：[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)

网上江湖郎中和蒙古大夫很多，因此，此类帖子也很多。关于排序，我还真没研究过，看了江湖郎中和蒙古大夫的帖子，搞了半天不行，所以，自己研究了一下，如下：三种方式都可以，如重写<，（）和写比较函数compare_index。但是要注意对象和对象指针的排序区别。
容器中是对象时，用<排序。
容器中是对象指针时，用（）和比较函数排序都可以。
list用成员方法sort
vector用sort函数
**[cpp]**[view plain](http://blog.csdn.net/marising/article/details/4567531#)[copy](http://blog.csdn.net/marising/article/details/4567531#)[print](http://blog.csdn.net/marising/article/details/4567531#)[?](http://blog.csdn.net/marising/article/details/4567531#)
- class TestIndex{  
- public:  
- int index;  
-     TestIndex(){  
-     }  
-     TestIndex(int _index):index(_index){  
-     }  
- bool operator()(const TestIndex* t1,const TestIndex* t2){  
-         printf("Operator():%d,%d/n",t1->index,t2->index);  
- return t1->index < t2->index;  
-     }  
- bool operator < (const TestIndex& ti) const {  
-         printf("Operator<:%d/n",ti.index);  
- return index < ti.index;  
-     }  
- };  
- bool compare_index(const TestIndex* t1,const TestIndex* t2){  
-     printf("CompareIndex:%d,%d/n",t1->index,t2->index);  
- return t1->index < t2->index;  
- }  
- int main(int argc, char** argv) {  
-     list<TestIndex*> tiList1;  
-     list<TestIndex> tiList2;  
-     vector<TestIndex*> tiVec1;  
-     vector<TestIndex> tiVec2;  
-     TestIndex* t1 = new TestIndex(2);  
-     TestIndex* t2 = new TestIndex(1);  
-     TestIndex* t3 = new TestIndex(3);  
-     tiList1.push_back(t1);  
-     tiList1.push_back(t2);  
-     tiList1.push_back(t3);  
-     tiList2.push_back(*t1);  
-     tiList2.push_back(*t2);  
-     tiList2.push_back(*t3);  
-     tiVec1.push_back(t1);  
-     tiVec1.push_back(t2);  
-     tiVec1.push_back(t3);  
-     tiVec2.push_back(*t1);  
-     tiVec2.push_back(*t2);  
-     tiVec2.push_back(*t3);  
-     printf("tiList1.sort()/n");  
-     tiList1.sort();//无法正确排序 
-     printf("tiList2.sort()/n");  
-     tiList2.sort();//用<比较 
-     printf("tiList1.sort(TestIndex())/n");  
-     tiList1.sort(TestIndex());//用()比较 
-     printf("sort(tiVec1.begin(),tiVec1.end())/n");  
-     sort(tiVec1.begin(),tiVec1.end());//无法正确排序
-     printf("sort(tiVec2.begin(),tiVec2.end())/n");  
-     sort(tiVec2.begin(),tiVec2.end());//用<比较
-     printf("sort(tiVec1.begin(),tiVec1.end(),TestIndex())/n");  
-     sort(tiVec1.begin(),tiVec1.end(),TestIndex());//用()比较
-     printf("sort(tiVec1.begin(),tiVec1.end(),compare_index)/n");  
-     sort(tiVec1.begin(),tiVec1.end(),compare_index);//用compare_index比较
- return 0;  
- ｝  
引用:[http://blog.csdn.net/marising/article/details/4567531](http://blog.csdn.net/marising/article/details/4567531)
