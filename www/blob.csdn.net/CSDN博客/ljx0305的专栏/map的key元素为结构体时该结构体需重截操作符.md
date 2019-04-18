# map的key元素为结构体时该结构体需重截操作符 - ljx0305的专栏 - CSDN博客
2013年01月29日 10:21:40[ljx0305](https://me.csdn.net/ljx0305)阅读数：1698
**map的key元素为结构体时该结构体需重截操作符<**
```cpp
1.1 
struct s {
    int a;
    int b;
    int c;
    bool operator<(const s&) const 
    {
				if ((a<other.a) || ((a==other.a)&&(b<other.b)) || ((a==other.a)&&(b==other.b)&&(c<other.c)))
				{
				   return true;
				}
				return false;
    }
};
map<s,string> m;
m.insert( make_pair(s(),"") );
1.2
struct s {
    int a;
    int b;
    int c;
};
bool operator<(const s&,const s&) 
{
	if ((a<other.a) || ((a==other.a)&&(b<other.b)) || ((a==other.a)&&(b==other.b)&&(c<other.c)))
	{
	     return true;
	}
	return false;
}
map<s,string> m;
m.insert( make_pair(s(),"") );
2.
struct s {
    int a;
    int b;
    int c;
};
struct cmp {
    bool operator()(const s&,const s&) const 
    {
			if ((a<other.a) || ((a==other.a)&&(b<other.b)) || ((a==other.a)&&(b==other.b)&&(c<other.c)))
			{
			     return true;
			}
			return false;
    }
};
map<s,string,cmp> m;
m.insert(make_pair(s(),"" ) );
```
