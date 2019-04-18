# set和map - Orisun - 博客园







# [set和map](https://www.cnblogs.com/zhangchaoyang/articles/2026096.html)





set存储已排序的无重复的元素,multiset是元素可以重复.为实现快速的集合运算set内部数据组织采用红黑树(一种严格意义上的平衡二叉树).

map存储key-value对,按key排序,map的内部数据结构也是红黑树,并且key值无重复.multimap允许key值重复.

STL中算法包含在<Algorithm>头文件中,集合的算法也包含在该头文件中.
- 集合的并:set_union
- 集合的交:set_intersection
- 集合的差:set_differnce

给一个set的例子:

#include<algorithm>
#include<iostream>
#include<set>
using namespace std;
int main(){
	set<int> A,B,C;
	set<int>::iterator iter;
	for(int i=0;i<10;i++){
		A.insert(i);			//A存储0--9
		B.insert(2*i+1);		//B存储可数1--19
	}
	//集合的并
	set_union(A.begin(),A.end(),B.begin(),B.end(),
			insert_iterator<set<int> >(C,C.begin()));
	for(iter=C.begin();iter!=C.end();iter++)
		cout<<*iter<<"  ";
	cout<<endl;
	C.clear();				//清空集合
	//集合的交
	set_intersection(A.begin(),A.end(),B.begin(),B.end(),
			insert_iterator<set<int> >(C,C.begin()));
	for(iter=C.begin();iter!=C.end();iter++)
		cout<<*iter<<"  ";
	cout<<endl;
	C.clear();				//清空集合
	//集合的差A-B
	set_difference(A.begin(),A.end(),B.begin(),B.end(),
			insert_iterator<set<int> >(C,C.begin()));
	for(iter=C.begin();iter!=C.end();iter++)
		cout<<*iter<<"  ";
	cout<<endl;
	C.clear();				//清空集合
	//集合的差B-A
	set_difference(B.begin(),B.end(),A.begin(),A.end(),
			insert_iterator<set<int> >(C,C.begin()));
	for(iter=C.begin();iter!=C.end();iter++)
		cout<<*iter<<"  ";
	cout<<endl;
}


pair结构在<utility>里面是这样定义的:

template <class T1,class T2>
struct pair{
	typedef T1 first_type;			//可以在一个struct里面使用typedef
	typedef T2 second_type;

	T1 first;
	T2 second;

	pair():first(T1()),second(T2()){}		//默认构造函数,T1()和T2()内嵌构造
	pair(const T1 &a,const T2 &b):first(a),second(b){}		//有值的构造函数
	template<class U,class T> pair(const pair<U,V> &p):first(p.first),second(p.second){}		//拷贝构造函数.template里面还可以再嵌套template
};
//比较函数,重载运算符==和<,其他!=,<=,>=,>类似
template<class T1,class T2>
bool operator ==(const pair<T1,T2>& p1,const pair<T1,T2>& p2){
	return p1.first==p2.first && p1.second==p2.second;
}
template<class T1,class T2>
bool operator < (const pair<T1,T2>&,const pair<T1,T2>&);

//pair创建函数,无需与出类型,就可以生成一个pair对象
template<class T1,class T2>
pair<T1,T2> make_pair(const T1&x,const T2&y){
	return pair<T1,T2>(x,y);
}


pair被定义为struct而不是class,所有的成员都是public.

map用例:


#include<map>
#include<iostream>
using namespace std;
int main(){									//main()不能返回void,否则会error: ‘::main’ must return ‘int’
	map<int,string> stu;		//定义map对象
	//向map中插入数据的4种方法
	stu.insert(pair<int,string>(1,"stu_one"));		//插入方法1
	stu.insert(map<int,string>::value_type(2,"stu_two"));		//插入方法2
	stu[3]="stu_three";					//插入方法3
	stu[7]="stu_seven";
	stu[8]="stu_eight";
	stu[9]="stu_nine";
	//插入方法4,可以判断插入是否成功
	pair<map<int,string>::iterator,bool> Insert_Pair;
	Insert_Pair=stu.insert(pair<int,string>(1,"stu_one"));
	if(Insert_Pair.second==true)
		cout<<"Insert Successgully"<<endl;
	else
		cout<<"Insert Failure"<<endl;		/*输出Insert Failure*/
		
	map<int,string>::iterator iter;			//定义map迭代器
		
	//数据的查找.使用count或find
	for(int k=0;k<10;k++){
		if(!stu.count(k))			//count返回关键字出现的次数,没有出现则返回0
			cout<<"Don't find key=="<<k<<endl;
		else
			cout<<stu[k]<<endl;		//stu[k]是key=k时对应的value值
	}
	iter=stu.find(1);
	if(iter!=stu.end())
		cout<<"Find,the value is "<<iter->second<<endl;
	else
		cout<<"Don't find key==1"<<endl;

	//数据的遍历,2种方法
	for(iter=stu.begin();iter!=stu.end();iter++){		//使用迭代器遍历
		cout<<iter->first<<"  "<<iter->second<<endl;
	}
	int nSize=stu.size();					//获取map集合元素的个数
	for(int i=0;i<nSize;i++)				//使用下标遍历.i从0到5.使用下标直接访问的是value值
		cout<<stu[i]<<endl;		/*注意这里的下标不同于数组的下标,这里的下标指的就是key值.所以只有当i为1,2,3时才有打印输出*/


	//数据的删除与清空
	//用clear()清空,用empty()判断是否为空
	iter=stu.find(2);		//用迭代器使用erase删除
	stu.erase(iter);

	stu.erase(3);			//使用关键字来删除
	//把以下的元素全部清除
	stu.erase(stu.begin(),stu.end());
	
	for(iter=stu.begin();iter!=stu.end();iter++){
		cout<<iter->first<<"  "<<iter->second<<endl;
	}
	
	return 0;
}














