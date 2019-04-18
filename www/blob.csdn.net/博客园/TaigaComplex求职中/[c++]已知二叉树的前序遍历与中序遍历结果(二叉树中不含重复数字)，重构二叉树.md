# [c++]已知二叉树的前序遍历与中序遍历结果(二叉树中不含重复数字)，重构二叉树 - TaigaComplex求职中 - 博客园







# [[c++]已知二叉树的前序遍历与中序遍历结果(二叉树中不含重复数字)，重构二叉树](https://www.cnblogs.com/TaigaCon/archive/2013/01/03/2843315.html)





比如说某二叉树

前序遍历结果：
|1|2|4|7|3|5|6|8|
|----|----|----|----|----|----|----|----|

中序遍历结果：
|4|7|2|1|5|3|8|6|
|----|----|----|----|----|----|----|----|

前序遍历就是先输出根节点，然后左节点，然后右节点

中序遍历就是先输出左节点，然后根节点，最后右节点



对于上面的二叉树我们可以这样分析：
|1|2|4|7|3|5|6|8|
|----|----|----|----|----|----|----|----|


|4|7|2|1|5|3|8|6|
|----|----|----|----|----|----|----|----|



其中绿色的为根结点，也就是**1**，

然后通过**中序遍历**结果，得到 **1** 左边的（粉红）为左子树，右边的（浅蓝）为右子树，

左子树跟右子树在**前序遍历**的结果中，我们能根据左子树跟右子树的**大小**划分开来，

然后就得到了一颗根节点为 **2 **，另一颗跟节点为 **3 **的二叉树了，

如此递归下去就能得到一颗完整的二叉树。



以下为c++实现的代码：

*基于gcc version 3.4.4 (cygming special, gdc 0.12, using dmd 0.125)*

```
1 /*已知二叉树的前序、中序遍历，重建二叉树*/
  2 
  3 #include <iostream>
  4 #include <list>
  5 #include <sstream>
  6 using namespace std;
  7 struct tree{
  8     int m_nValue;
  9     tree *l_tree;
 10     tree *r_tree;
 11 }*root;
 12 
 13 /*重建二叉树的主函数，通过递归重建
 14  *f_begin：前序遍历头结点
 15  *f_end :  前序遍历末尾节点
 16  *m_begin：中序遍历头结点
 17  *m_end：  中序遍历末尾节点
 18  */
 19 tree *rebuild_tree(list<int>::iterator f_begin,list<int>::iterator f_end,list<int>::iterator m_begin,list<int>::iterator m_end);
 20     
 21 /*显示重建后的二叉树*/    
 22 void show_tree(tree *root);
 23 
 24 /*实现对list迭代器的整型加法，itr为迭代器，i为所加的整数*/
 25 list<int>::iterator pluss(list<int>::iterator itr,int i);
 26     
 27 /*计算出迭代器find_相对于begin的距离，范围是begin到end，并且把所找到的迭代器赋给res*/    
 28 int dist(list<int>::iterator begin,list<int>::iterator end,list<int>::iterator find_,list<int>::iterator &res);
 29 
 30 /*convert是把string转换到list<int>容器内的转换函数*/
 31 void convert(string &str,list<int> &i_list);
 32 
 33 void free_tree(void);
 34 
 35 /*前序l_front,中序l_mid*/
 36     static list<int> l_front;
 37     static list<int> l_mid;
 38     
 39     /*list_head用于记录所创建的tree 
 40      *index用于记录tree指针所存的位置
 41      */
 42     static tree *list_head[100];
 43     static int tree_index;
 44 
 45 int main()
 46 {
 47     string front;
 48     string mid;
 49     
 50     tree_index=0;
 51 
 52 
 53     cout<<"请输入树的前序遍历序列（空格隔开）："<<endl;
 54     getline(cin,front);
 55 
 56     cout<<"请输入树的中序遍历序列（空格隔开）："<<endl;
 57     getline(cin,mid);
 58 
 59     
 60     convert(front,l_front);
 61     convert(mid,l_mid);
 62     
 63     /*这里不考虑.end()因为后面迭代的时候分段，list段内是没有.end()的，而，rebuild_tree是需要递归的函数，这样是为了保持一致性*/
 64     root=rebuild_tree(l_front.begin(),--l_front.end(),l_mid.begin(),--l_mid.end());
 65     
 66     cout<<"Show Tree"<<endl;
 67     show_tree(root);
 68     cout<<endl;
 69     free_tree();
 70 }
 71 
 72 tree *rebuild_tree(list<int>::iterator f_begin,list<int>::iterator f_end,list<int>::iterator m_begin,list<int>::iterator m_end)
 73 {
 74     list<int>::iterator tmp,res;
 75         list<int>::iterator tmp1,tmp2,tmp3;
 76         int dis;
 77         tree *root=(tree*)malloc(sizeof(tree));
 78         
 79         list_head[tree_index]=root;
 80         tree_index++;
 81         
 82         dis=dist(m_begin,m_end,f_begin,res);
 83         root->m_nValue=*f_begin;        
 84 
 85         cout<<"dis:"<<dis<<endl;        
 86         
 87         /*如果list内的数据已经被加入二叉树节点，那么把该list的节点赋值为-1，便于后面判断边界*/
 88         *f_begin=-1;
 89             
 90                 
 91 /*---------------------如果有左子树，往左遍历-----------------------------*/        
 92         tmp=res;
 93 /*---------------------如果dis为0，则说明没左子树-------------------------------*/    
 94         if(tmp==l_mid.begin()||dis==0)
 95             {
 96                 root->l_tree=NULL;
 97             }
 98             else if(*(--tmp)==-1)
 99                     {
100                         root->l_tree=NULL;
101                     }
102         else
103             {
104         tmp1=pluss(f_begin,1);
105         tmp2=pluss(f_begin,dis);
106         tmp3=pluss(m_begin,dis-1);
107         
108         root->l_tree=rebuild_tree(tmp1,tmp2,m_begin,tmp3);
109             }
110 /*-----------------------如果有右子树，往右遍历---------------------------*/    
111     
112         tmp=res;
113         /*如果搜索所得的结果为该中序遍历list段尾部，那么他的右子树为NULL*/
114     if(tmp==m_end)
115         {    
116             root->r_tree=NULL;
117         }
118         else{
119             tmp++;
120      if(*tmp==-1|| tmp==l_mid.end())
121         {
122             root->r_tree=NULL;
123         }
124     else
125         {
126             tmp1=pluss(f_begin,dis+1);
127             tmp2=pluss(res,1);
128             
129             root->r_tree=rebuild_tree(tmp1,f_end,tmp2,m_end);
130         }
131 }
132 /*------------------------------------------------------------------*/    
133     cout<<"add node:"<<root->m_nValue<<endl;
134         return root;
135 }
136 
137 list<int>::iterator pluss(list<int>::iterator itr,int i)
138     {
139         for(;i>0;i--)
140         {
141         itr++;
142     }
143         return itr;
144     }
145 
146 int dist(list<int>::iterator begin,list<int>::iterator end,list<int>::iterator find_,list<int>::iterator &res)
147 {
148     res=find_;
149     int i=0;
150     end++;
151 do
152 {
153     if(*begin==*res)
154         {
155             res=begin;
156             return i;
157         }
158         i++;
159         begin++;
160 }while(begin!=end);
161 
162     if(res!=l_mid.end())
163     *res=-1;
164 
165         return -1;
166 }
167 
168 void show_tree(tree *root)
169 {
170     cout<<root->m_nValue<<",";
171     if(root->l_tree)
172         {
173             show_tree(root->l_tree);
174         }
175     if(root->r_tree)
176     {
177         show_tree(root->r_tree);
178     }
179 }
180 
181 void convert(string &str,list<int> &i_list)
182 {
183     int i;
184 
185     stringstream strOutput(str);
186     for(;strOutput>>i;)
187     {        
188         i_list.push_back(i);
189     }
190     
191 }
192 
193 void free_tree(void)
194 {
195     for(;tree_index>=0;tree_index--)
196     free(list_head[tree_index]);
197 }
```














