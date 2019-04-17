# Trie树 - PeterBishop - CSDN博客





2018年08月16日 19:19:42[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[数据结构与算法](https://blog.csdn.net/qq_40061421/article/category/7898328)









1、基本概念

　　字典树，又称为单词查找树，Tire数，是一种树形结构，它是一种哈希树的变种。

![](https://images2015.cnblogs.com/blog/713721/201606/713721-20160624160557531-261335182.png)

　　2、基本性质
- 根节点不包含字符，除根节点外的每一个子节点都包含一个字符
- 从根节点到某一节点。路径上经过的字符连接起来，就是该节点对应的字符串
- 每个节点的所有子节点包含的字符都不相同

　　3、应用场景

　　典型应用是用于统计，排序和保存大量的字符串(不仅限于字符串)，经常被搜索引擎系统用于文本词频统计。

　　4、优点

　　利用字符串的公共前缀来减少查询时间，最大限度的减少无谓的字符串比较，查询效率比哈希树高。



```cpp
/*
功能描述:实现trie树的创建、插入、查询
说明:
对统计对象,要求符合正则"[a-z]*"的格式的单词
若考虑大写,标点和空白字符(空格.TAB.回车换行符),
可修改next数组大小,最多255可包含所有字符
*/
#include<stdio.h>
#include <stdlib.h>
#include<string.h>


#define MAX_CHILD 26 //此函数只考虑26个英文字母的情况




typedef struct Tree
{
	int count;         //用来标记该节点是个可以形成一个单词，如果count！=0，则从根节点到该节点的路径可以形成一个单词
	struct Tree *child[MAX_CHILD];
}Node, *Trie_node;




Node* CreateTrie()                             //创建trie节点树
{
	Node *node = (Node*)malloc(sizeof(Node));
	memset(node, 0, sizeof(Node));
	return node;
}


void insert_node(Trie_node root, char *str)      //trie树插入结点
{
	if (root == NULL || *str == '\0')
		return;
	Node *t = root;
	char *p = str;
	while (*p != '\0')
	{
		if (t->child[*p - 'a'] == NULL)
		{
			Node *tmp = CreateTrie();
			t->child[*p - 'a'] = tmp;
		}
		t = t->child[*p - 'a'];
		p++;
	}
	t->count++;
}




void search_str(Trie_node root, char *str)             //查找串是否在该trie树中
{
	if (NULL == root || *str == '\0')
	{
		printf("trie is empty or str is null\n");
		return;
	}


	char *p = str;
	Node *t = root;
	while (*p != '\0')
	{
		if (t->child[*p - 'a'] != NULL)
		{
			t = t->child[*p - 'a'];
			p++;
		}
		else
			break;
	}
	if (*p == '\0')
	{
		if (t->count == 0)
			printf("该字符串不在trie树中，但该串是某个单词的前缀\n");
		else
			printf("该字符串在该trie树中\n");
	}
	else
		printf("该字符串不在trie树中\n");
}


void del(Trie_node root)      //释放整个字典树占的堆空间
{
	int i;
	for (i = 0; i<MAX_CHILD; i++)
	{
		if (root->child[i] != NULL)
			del(root->child[i]);
	}
	free(root);
}




int main()
{
	int i, n;
	char str[20];
	printf("请输入要创建的trie树的大小：");
	scanf("%d", &n);
	Trie_node root = NULL;
	root = CreateTrie();
	if (root == NULL)
		printf("创建trie树失败\n");
	for (i = 0; i<n; i++)
	{
		scanf("%s", str);
		insert_node(root, str);
	}
	printf("trie树已建立完成\n");
	printf("请输入要查询的字符串:\n");
	while (scanf("%s", str) != NULL)
	{
		search_str(root, str);

	}
	return 0;
}
```





