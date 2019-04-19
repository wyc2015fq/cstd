# c++ map遍历_用户5706034127_新浪博客
|||
map<<span style="color: rgb(0, 0, 255); font-family: 'Courier New' !important;">string,int> m;
map<<span style="color: rgb(0, 0, 255); font-family: 'Courier New' !important;">string,int>::iterator it;
it = m.begin();
while(it != m.end())
{
    //it->first;
    //it->second;
    it ++;         
}
**一。定义如：map < int, CString > 或者 **map < int, 结构体名>的元素遍历
**map < int, CString >  maps;  或者**map < int, 结构体名>  maps;
** for（int  i=0；i < maps. size() ;  i++） **
** {**
** CString s=maps[ i ];**
** }**
**二。定义如：map < CString **, 结构体名> **的元素遍历**
map <  CString **, 结构体名**>  maps; 
**map <  CString , 结构体名> : : iterator  iter;**
**for(  iter=maps.begin();  iter!=maps.end();  iter++){**CString a= iter - > first;
** 结构体名 p = iter - > second;**
** }**
