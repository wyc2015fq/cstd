# STL:map的使用:删除map中指定值为value的所有元素 - ljx0305的专栏 - CSDN博客
2008年04月14日 21:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3122标签：[string																[iterator																[reference																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
//第一个解决方案
//erase.h
#include <algorithm>
using std::find_if; 
template<class _map>
class Proxy{
typename _map::mapped_type   const _value;
public:
    Proxy(typename _map::mapped_type const &value):_value(value){}
booloperator()(typename _map::value_type & value){
return value.second==_value;
    }
};
template<class _map>
size_t Erase(_map& m,typename _map::mapped_type   const &value)
{   
    size_t count=0;
typename _map::iterator iter;
typename _map::iterator begin=m.begin();
typename _map::iterator end=m.end();
    Proxy<_map> proxy(value);
while((iter=find_if(begin,end,proxy))!=end)
    {
       ++count;
       m.erase(iter);
       begin=m.begin();
       end=m.end();
    }   
return count;
}
//示例代码
#include <string>
#include <map>
#include "erase.h"
using std::map;
using std::string;
typedef map<int ,string> Int2Str;
int main(){   
    Int2Str sample;   
    sample[1]=string("abc");
    sample[2]=string("abc");
    sample[3]=string("1234");
    sample[4]=string("abc");
    Erase(sample,string("abc"));    
return 0;
}
//******************************************************************//
//第二种方法
#include <map>
#include <string>
#include <algorithm>
#include<iostream>
using std::map;
using std::string;
using std::find_if;
using std::endl;
using std::cout;
template <class _MAP>
class DeleteAllValueProxy{
typedeftypename _MAP::mapped_type const map_type;
typedeftypename _MAP::iterator map_iter;
typedeftypename _MAP::const_reference map_value;
    map_type _val;
public:
    DeleteAllValueProxy(_MAP &rmap,map_type &val)
       :_val(val)
    {
       map_iter begin=rmap.begin();
       map_iter end=rmap.end();
       map_iter iter=find_if(begin,end,*this);
while (iter!=end)
       {
           cout<<iter->first<<"   "<<iter->second<<endl;
           begin=rmap.erase(iter);
           end=rmap.end();
           iter=find_if(begin,end,*this);
       }
    }
booloperator()(map_value item){
return item.second==_val;
    }
};
template <class _MAP>
inlinevoid DeleteAllValue(_MAP&rmap,typename _MAP::mapped_type const&val)
{
    (DeleteAllValueProxy<_MAP>(rmap,val));
}
int main()
{
    map<int,string> Int2Str;
    Int2Str[5]="abc";
    Int2Str[12]="abc";
    Int2Str[43]="abc";
    Int2Str[27]="abc";
    Int2Str[1]="def";
    Int2Str[8]="ghi";
    Int2Str[24]="jkl";
    Int2Str[56]="mn";
    Int2Str[0]="opq";
    DeleteAllValue(Int2Str,string("abc"));
return 0;
}
//*******************************************************************/
//第三种方法
#include <iostream>
#include<map>
#include <string>
#include <algorithm>
using std::endl;
using std::cout;
using std::find_if;
using std::string;
using std::map;
template<class _map>
void Erase(_map& m,typename _map::mapped_type   const &value)
{
typename _map::iterator iter=m.begin();
typename _map::iterator end=m.end();
typename _map::iterator temp;
while (iter!=end)
    {
if (iter->second==value)
       {
           temp=iter;
           ++iter;
           cout<<temp->first<<" "<<temp->second<<endl;
           m.erase(temp);
       }
else
           ++iter;
       end=m.end();
    }
}
typedef map<int ,string> Int2Str;
int main(){   
    Int2Str sample;   
    sample[11]=string("abc");
    sample[6]=string("bcd");
    sample[3]=string("afd");
    sample[4]=string("abc");
    sample[15]=string("gif");
    sample[44]=string("abc");
    sample[12]=string("gif");
    sample[47]=string("abc");
    sample[356]=string("364");
    sample[42]=string("abc");
    Erase(sample,string("abc"));    
return 0;
}
 转自[http://blog.csdn.net/SammyLan/archive/2006/09/10/1203753.aspx](http://blog.csdn.net/SammyLan/archive/2006/09/10/1203753.aspx)
