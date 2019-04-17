# 2016 CCF-CSP 计算机职业资格认证考试 解题报告 - CD's Coding - CSDN博客





2017年01月03日 19:38:49[糖果天王](https://me.csdn.net/okcd00)阅读数：9915标签：[CCF																[CCF-CSP																[2016																[职业资格认证考试](https://so.csdn.net/so/search/s.do?q=职业资格认证考试&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





# 前言

嘛，参加这次考试属于一连串比较机缘巧合的事情——

首先是当上了2016年度CCF优秀大学生的荣誉，这个奖颁发的同时也赠予了为期一年的CCF会员资格

而会员……是具有一次免费参加CCF计算机职业资格认证考试的机会（就算续费或者再次获得会员资格也只有这一次免费的机会）

即便是我已经两年没有敲过C++了，经常会敲成Python的代码习惯……

嘛，我就抱着不考白不考的精神——


去试试吧 ^_^

![](https://img-blog.csdn.net/20170103185912763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


（毕竟期末复习周，也没太多时间可以整理代码，如下代码均为考场中敲出，各种没有注意的小细节请原谅QwQ，哦对了第三题的答案并不是全对的需要注意一下）







# A. 中间数




## 题面






问题描述

|试题编号：|1|
|----|----|
|试题名称：|中间数|
|时间限制：|1.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　在一个整数序列*a*1, *a*2, …, *an*中，如果存在某个数，大于它的整数数量等于小于它的整数数量，则称其为中间数。在一个序列中，可能存在多个下标不相同的中间数，这些中间数的值是相同的。　　给定一个整数序列，请找出这个整数序列的中间数的值。输入格式　　输入的第一行包含了一个整数*n*，表示整数序列中数的个数。　　第二行包含*n*个正整数，依次表示*a*1, *a*2, …, *an*。输出格式　　如果约定序列的中间数存在，则输出中间数的值，否则输出-1表示不存在中间数。样例输入62 6 5 6 3 5样例输出5样例说明　　比5小的数有2个，比5大的数也有2个。样例输入43 4 6 7样例输出-1样例说明　　在序列中的4个数都不满足中间数的定义。样例输入53 4 6 6 7样例输出-1样例说明　　在序列中的5个数都不满足中间数的定义。评测用例规模与约定　　对于所有评测用例，1 ≤ *n* ≤ 1000，1 ≤ *ai* ≤ 1000。|





## 解题报告

题意是说，要找一个数，序列中比这个数大的数的个数与序列中比这个数小的数的个数相同，该数称作中间数

这个问题可以等效看作一个更简单的问题模型：对于排序后数列最中间的数，向左向右移动两个pivot至第一个非该数的位置，倘若这两个pivot两侧的数字个数相同，则该数为中间数，反之为不是（简单的四则运算23333）。



```cpp
#include <map>
#include <cmath>
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 1024

int a[MAXN];

int main()
{
	//freopen("A.txt","r",stdin);
	int n; scanf("%d",&n);
	map<int,int> cnt;
	for(int i=1;i<=n;i++)
	{
		cin>>a[i-1];
		cnt[a[i-1]]++;
	}
	sort(a,a+n);
	int l = n>>1, r = n>>1, mid = a[n>>1];
	while(l>=0 && a[l]==mid)l--;
	while(r< n && a[r]==mid)r++;
	//cout<<l<<":"<<r<<endl;
	if(l+r == n-1) cout<<mid<<endl;
	else cout<<"-1"<<endl;
	return 0;
}
```





# B. 工资计算




## 题面




问题描述

|试题编号：|2|
|----|----|
|试题名称：|工资计算|
|时间限制：|1.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　小明的公司每个月给小明发工资，而小明拿到的工资为交完个人所得税之后的工资。假设他一个月的税前工资（扣除五险一金后、未扣税前的工资）为*S*元，则他应交的个人所得税按如下公式计算：　　1） 个人所得税起征点为3500元，若*S*不超过3500，则不交税，3500元以上的部分才计算个人所得税，令*A*=*S*-3500元；　　2） *A*中不超过1500元的部分，税率3%；　　3） *A*中超过1500元未超过4500元的部分，税率10%；　　4） *A*中超过4500元未超过9000元的部分，税率20%；　　5） *A*中超过9000元未超过35000元的部分，税率25%；　　6） *A*中超过35000元未超过55000元的部分，税率30%；　　7） *A*中超过55000元未超过80000元的部分，税率35%；　　8） *A*中超过80000元的部分，税率45%；　　例如，如果小明的税前工资为10000元，则*A*=10000-3500=6500元，其中不超过1500元部分应缴税1500×3%=45元，超过1500元不超过4500元部分应缴税(4500-1500)×10%=300元，超过4500元部分应缴税(6500-4500)×20%=400元。总共缴税745元，税后所得为9255元。　　已知小明这个月税后所得为*T*元，请问他的税前工资*S*是多少元。输入格式　　输入的第一行包含一个整数*T*，表示小明的税后所得。所有评测数据保证小明的税前工资为一个整百的数。输出格式　　输出一个整数*S*，表示小明的税前工资。样例输入9255样例输出10000评测用例规模与约定　　对于所有评测用例，1 ≤ *T* ≤ 100000。|





## 解题报告



可以先用正向计算的方法得出（注释里的代码）各个收入水平的税后价格进行一一对应

对于每个税后价格就可以直接知道该税后价格是在哪个税前价格的区间里，大大减少了尝试所花费的时间和空间

需要注意的一个要点是：A是除去无税收入的，结果别忘了加回来



```cpp
#include <map>
#include <cmath>
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 100024

int rate[8]  = {0,    3,   10,   20,    25,    30,    35,     45};
int rank[8]  = {0, 1500, 4500, 9000, 35000, 55000, 80000, 154400};
int limit[8] = {0, 1455, 4155, 7755, 27255, 41255, 57505, 100000};

//	limit[i] = limit[i-1]  \
//	+( rank[i]-rank[i-1] ) \
//	-( rank[i]-rank[i-1] ) * rate[i]/100;

int main()
{
	int t; cin>>t;
	int a = t - 3500;
	
	if(a<=0)
	{
		cout<<t<<endl; 
		return 0;
	}
	
	for(int i=1; i<8; i++)
	{
		if(a <= limit[i])
		{
			int rest = a - limit[i-1];
			// Using integer calculation to avoid float_point error
			int ret = rank[i-1] + rest*100/(100-rate[i]);
			cout<< ret + 3500 <<endl;
			return 0;
		}
	}
	
	return 0;
}
```




# C. 权限查询






## 题面






问题描述

|试题编号：|3|
|----|----|
|试题名称：|权限查询|
|时间限制：|1.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　授权 (authorization) 是各类业务系统不可缺少的组成部分，系统用户通过授权机制获得系统中各个模块的操作权限。　　本题中的授权机制是这样设计的：每位用户具有若干角色，每种角色具有若干权限。例如，用户 david 具有 manager 角色，manager 角色有 crm:2 权限，则用户 david 具有 crm:2 权限，也就是 crm 类权限的第 2 等级的权限。　　具体地，用户名和角色名称都是由小写字母组成的字符串，长度不超过 32。权限分为分等级权限和不分等级权限两大类。分等级权限由权限类名和权限等级构成，中间用冒号“:”分隔。其中权限类名也是由小写字母组成的字符串，长度不超过 32。权限等级是一位数字，从 0 到 9，数字越大表示权限等级越高。系统规定如果用户具有某类某一等级的权限，那么他也将自动具有该类更低等级的权限。例如在上面的例子中，除 crm:2 外，用户 david 也具有 crm:1 和 crm:0 权限。不分等级权限在描述权限时只有权限类名，没有权限等级（也没有用于分隔的冒号）。　　给出系统中用户、角色和权限的描述信息，你的程序需要回答多个关于用户和权限的查询。查询可分为以下几类：　　* 不分等级权限的查询：如果权限本身是不分等级的，则查询时不指定等级，返回是否具有该权限；　　* 分等级权限的带等级查询：如果权限本身分等级，查询也带等级，则返回是否具有该类的该等级权限；　　* 分等级权限的不带等级查询：如果权限本身分等级，查询不带等级，则返回具有该类权限的等级；如果不具有该类的任何等级权限，则返回“否”。输入格式　　输入第一行是一个正整数 *p*，表示不同的权限类别的数量。紧接着的 *p* 行被称为 P 段，每行一个字符串，描述各个权限。对于分等级权限，格式为 <category>-<level>，其中 <category> 是权限类名，<level> 是该类权限的最高等级。对于不分等级权限，字符串只包含权限类名。　　接下来一行是一个正整数 *r*，表示不同的角色数量。紧接着的 *r* 行被称为 R 段，每行描述一种角色，格式为　　<role> <s> <privilege 1> <privilege 2> ... <privilege s>　　其中 <role> 是角色名称，<s> 表示该角色具有多少种权限。后面 <s> 个字符串描述该角色具有的权限，格式同 P 段。　　接下来一行是一个正整数 *u*，表示用户数量。紧接着的 *u* 行被称为 U 段，每行描述一个用户，格式为　　<user> <t> <role 1> <role 2> ... <role t>　　其中 <user> 是用户名，<t> 表示该用户具有多少种角色。后面 <t> 个字符串描述该用户具有的角色。　　接下来一行是一个正整数 *q*，表示权限查询的数量。紧接着的 *q* 行被称为 Q 段，每行描述一个授权查询，格式为 <user> <privilege>，表示查询用户 <user> 是否具有 <privilege> 权限。如果查询的权限是分等级权限，则查询中的 <privilege> 可指定等级，表示查询该用户是否具有该等级的权限；也可以不指定等级，表示查询该用户具有该权限的等级。对于不分等级权限，只能查询该用户是否具有该权限，查询中不能指定等级。输出格式　　输出共 *q* 行，每行为 false、true，或者一个数字。false 表示相应的用户不具有相应的权限，true 表示相应的用户具有相应的权限。对于分等级权限的不带等级查询，如果具有权限，则结果是一个数字，表示该用户具有该权限的（最高）等级。如果用户不存在，或者查询的权限没有定义，则应该返回 false。样例输入3crm:2git:3game4hr 1 crm:2it 3 crm:1 git:1 gamedev 2 git:3 gameqa 1 git:23alice 1 hrbob 2 it qacharlie 1 dev9alice gamealice crm:2alice git:0bob gitbob poweroffcharlie gamecharlie crmcharlie git:3malice game样例输出falsetruefalse2falsetruefalsetruefalse样例说明　　样例输入描述的场景中，各个用户实际的权限如下：　　* 用户 alice 具有 crm:2 权限　　* 用户 bob 具有 crm:1、git:2 和 game 权限　　* 用户 charlie 具有 git:3 和 game 权限　　* 用户 malice 未描述，因此不具有任何权限评测用例规模与约定　　评测用例规模：　　* 1 ≤ *p*, *r*, *u* ≤ 100　　* 1 ≤ *q* ≤ 10 000　　* 每个用户具有的角色数不超过 10，每种角色具有的权限种类不超过 10　　约定：　　* 输入保证合法性，包括：　　1) 角色对应的权限列表（R 段）中的权限都是之前（P 段）出现过的，权限可以重复出现，如果带等级的权限重复出现，以等级最高的为准　　2) 用户对应的角色列表（U 段）中的角色都是之前（R 段）出现过的，如果多个角色都具有某一分等级权限，以等级最高的为准　　3) 查询（Q 段）中的用户名和权限类名不保证在之前（U 段和 P 段）出现过　　* 前 20% 的评测用例只有一种角色　　* 前 50% 的评测用例权限都是不分等级的，查询也都不带等级|





## 解题报告

唔，恕我直言这是不是大模拟？

实现难度不大，主要是敲之前需要事先想好合适的存储数据结构，我就是原先的结构感觉太罗嗦删了重写来着QvQ

将(user, category)作为主key，权限等级作为value

关于作为value的权限等级，此处我做了如下改动：

· 该category有等级时，value为等级+1，即0-9级的value为1-10

· 该category无等级时，具有权限的value为-1

· 该category不具有权限时，value为0，因为default的默认值是0

使用两个map进行存储，分别为Role-Authority与User-Authority，先对RA进行更新，然后对于每个User，遍历其拥有的Role更新该User的每项权限，取权限较高的值





```cpp
#include <map>
#include <cmath>
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

// Define Un-leveled is -1; level +1; Ignored 0;
typedef pair<string, string> pss;
typedef map<string, int> psi;
typedef map<pss, int> mpi;

mpi ra; //roleAuthor;
mpi ua; //userAuthor;

#define MAXN 233
#define MP(a,b) make_pair(a,b)

void update(string user, string role, string cate)
{
	int original_level = ua[make_pair(user, cate)];
	int current_level = ra[make_pair(role, cate)];
	ua[make_pair(user, cate)] = (
		abs(current_level) >= abs(original_level)?
		current_level : original_level
	);
	// cout<<"Update "<<user<<":"<<role<<":"<<cate<<endl;
	// cout<<original_level<<" "<<current_level<<endl;
}

int split(string s, char delim = ' ')
{
	int ret = s.find(delim);
	return ret;
}

int main()
{
	//freopen("C.txt","r",stdin);
	string str;
	int i,j,k;
	// pss testPair = make_pair("chendian", "CCFTest");
	// cout << author[testPair]; // Initial is 0
	
	int p; cin>>p;
	string anames[MAXN];
	for(i=0;i<p;i++)
	{
		cin>>str;
		int sp = split(str,':');
		//cout<<str<<" -> "<<split(str,':')<<endl;
		if (sp != -1) anames[i] = str.substr(0,sp);
		else anames[i] = str;
	}
	
	// R-Para
	int r; cin>>r;
	string rname; //roleName
	for(i=0;i<r;i++)
	{
		int s;
		cin>>rname>>s;
		for(j=0;j<s;j++)
		{
			cin>>str;
			int level = 0;
			int sp = split(str,':');			
			string category = "";
			if (sp != -1)
			{
				category = str.substr(0,sp);
				level = str.substr(sp+1)[0] - '0';
				// cout << category <<":"<< level << endl;
				ra[make_pair(rname, category)] = level+1;
			}
			else
			{
				category = str; // cate
				ra[make_pair(rname, category)] = -1;
			}
		}
	}
	
	int u; cin>>u;
	string uname; //userName
	for(i=0;i<u;i++)
	{
		int s;
		cin>>uname>>s;
		for(j=0;j<s;j++)
		{
			cin>>str; // role
			for(k=0;k<p;k++)
			{
				update(uname, str, anames[k]);
				// cout<<uname<<":"<<anames[k]<<":"<<ua[make_pair(uname, anames[k])]<<endl;
			}
		}
	}
	
	// Q-Para
	int q; cin>>q;
	string qname; //queryName
	for(i=0;i<q;i++)
	{
		cin>>qname>>str;
		int sp = split(str,':');
		string cate = "";
		cate = (sp != -1)?str.substr(0,sp):str;
		// cout<<qname<<":"<<cate<<endl;
		if(sp == -1)
		{
			int lev = ua[make_pair(qname,cate)];
			if(lev==0) cout<<"false"<<endl;
			else if(lev==-1) cout<<"true"<<endl;
			else cout<<lev-1<<endl;
		}
		else
		{
			int lev = ua[make_pair(qname,cate)]-1;
			int qlevel = str.substr(sp+1)[0]-'0';
			//cout<<lev<<":"<<qlevel<<endl;
			if(lev>=qlevel) cout<<"true"<<endl;
			else cout<<"false"<<endl;
		}
	}
	
	return 0;
}
```








# D. 压缩编码






## 题面






问题描述

|试题编号：|4|
|----|----|
|试题名称：|压缩编码|
|时间限制：|3.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　给定一段文字，已知单词*a*1, *a*2, …, *an*出现的频率分别*t*1, *t*2, …, *tn*。可以用01串给这些单词编码，即将每个单词与一个01串对应，使得任何一个单词的编码（对应的01串）不是另一个单词编码的前缀，这种编码称为前缀码。　　使用前缀码编码一段文字是指将这段文字中的每个单词依次对应到其编码。一段文字经过前缀编码后的长度为：*L*=*a*1的编码长度×*t*1+*a*2的编码长度×*t*2+…+ *an*的编码长度×*tn*。　　定义一个前缀编码为字典序编码，指对于1 ≤ *i *< *n*，*ai*的编码（对应的01串）的字典序在*ai*+1编码之前，即*a*1, *a*2, …, *an*的编码是按字典序升序排列的。　　例如，文字E A E C D E B C C E C B D B E中， 5个单词A、B、C、D、E出现的频率分别为1, 3, 4, 2, 5，则一种可行的编码方案是A:000, B:001, C:01, D:10, E:11，对应的编码后的01串为1100011011011001010111010011000111，对应的长度*L*为3×1+3×3+2×4+2×2+2×5=34。　　在这个例子中，如果使用哈夫曼(Huffman)编码，对应的编码方案是A:000, B:01, C:10, D:001, E:11，虽然最终文字编码后的总长度只有33，但是这个编码不满足字典序编码的性质，比如C的编码的字典序不在D的编码之前。　　在这个例子中，有些人可能会想的另一个字典序编码是A:000, B:001, C:010, D:011, E:1，编码后的文字长度为35。　　请找出一个字典序编码，使得文字经过编码后的长度*L*最小。在输出时，你只需要输出最小的长度*L*，而不需要输出具体的方案。在上面的例子中，最小的长度*L*为34。输入格式　　输入的第一行包含一个整数*n*，表示单词的数量。　　第二行包含*n*个整数，用空格分隔，分别表示*a*1, *a*2, …, *an*出现的频率，即*t*1, *t*2, …, *tn*。请注意*a*1, *a*2, …, *an*具体是什么单词并不影响本题的解，所以没有输入*a*1, *a*2, …, *an*。输出格式　　输出一个整数，表示文字经过编码后的长度*L*的最小值。样例输入51 3 4 2 5样例输出34样例说明　　这个样例就是问题描述中的例子。如果你得到了35，说明你算得有问题，请自行检查自己的算法而不要怀疑是样例输出写错了。评测用例规模与约定　　对于30%的评测用例，1 ≤ *n* ≤ 10，1 ≤ *ti* ≤ 20；　　对于60%的评测用例，1 ≤ *n* ≤ 100，1 ≤ *ti* ≤ 100；　　对于100%的评测用例，1 ≤ *n* ≤ 1000，1 ≤ *ti* ≤ 10000。|





## 解题报告

个人当时的想法先是用树状数组维护初始值已全部设为1的叶子节点，

使用DP维护OPT[lef,rig]，初始化为OPT[i,i]=1，

自底向上的更新中间节点的最小值，对于OPT[lef,rig]，遍历[lef,rig]的分割点，左右两个子问题的最优解加上合并消耗，和当前OPT比较更新

更新完整张图即可得到答案。

不过尚未实现，不知道对不对就是了(*^__^*) 


# 结语

这次只能对陪伴我前三年大学生活的C++说声真的很抱歉了……QAQ

第五题的级数收敛由于本人数学的问题看不懂题意……QwQ

第四题没能来得及写完……OvO

前三题的第三题在交卷前五分钟才想起来注释掉freopen，即便如此也有一组测试数据没能通过……=v=

成绩如下，待MachineLearning学得小有所成，再回来继续玩CF和TC……^_^

C++等着我~~

![](https://img-blog.csdn.net/20170103190416977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=2016&t=blog)](https://so.csdn.net/so/search/s.do?q=CCF-CSP&t=blog)](https://so.csdn.net/so/search/s.do?q=CCF&t=blog)




