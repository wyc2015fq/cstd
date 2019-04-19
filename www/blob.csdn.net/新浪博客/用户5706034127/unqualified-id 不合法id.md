# unqualified-id 不合法id_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
#include
void delete(){}
int main()
{
 std::max(1,123);
 delete();
 return 0;
}
void delete(){}
--->  error: expected unqualified-id before
'delete'
std::max(1,123)--->  error: expected
unqualified-id before '(' token
不合法的变量名，
max和delete和系统中存在的变量名冲突了。
