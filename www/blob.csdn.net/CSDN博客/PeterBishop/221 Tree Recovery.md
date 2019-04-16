# 221 Tree Recovery - PeterBishop - CSDN博客





2018年10月10日 20:42:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：14








### ①  前序遍历：先访问根节点，然后以前序访问左子树，右子树。  中序遍历：左子树，当前节点，右子树。根据前序和中序遍历的特点，可以发现如下规律：前序遍历的每个节点，都是当前子树的根节点。同时，以对应的节点为边界，就会把中序遍历的结果分为左子树和右子树。例如：前序：a b d c e f 'a'是根节点中序：d b a e c f'a'是根节点，把字符串分成左右两个子树'a'是前序遍历节点的第一个元素，可以看出，它把中序遍历的结果分成'db'和'ecf'两部分。如图：这就是'a'的左子树和右子树的遍历结果。如果能够找到前序遍历中对应的左子树和右子树，就可以把'a'作为当前的根节点，然后依次递归下去，这样就能够依次恢复左子树和右子树的遍历结果。 ![](https://img-blog.csdn.net/20140831163454843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwNjgxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)②后序遍历二叉树的操作定义为：若二叉树为空，则空操作；否则（1）后序遍历左子树；（2）后序遍历右子树；（3）访问根结点。

```cpp
#include<iostream>
#include<string.h>
#include<stdlib.h>
using namespace std;

typedef struct Node {
	Node *left;
	Node *right;
	char val;
}Node;

void Rebuild(char* PreOrder, char* InOrder, int TreeLen, Node** root) {
	
	if (PreOrder == NULL || InOrder == NULL || root == NULL)//先判断字符串会不会为空
		return;
	Node* p = (Node*)malloc(sizeof(Node));
	p->left = p->right = NULL;
	p->val = *PreOrder;
	*root = p;
	if (TreeLen == 1)//一定要在赋值完最后一个结点之后再去return
		return;
	int i;
	for (i = 0; i<TreeLen; i++) {
		if (*PreOrder == *(InOrder + i)) {
			break;
		}
	}
	int leftlen = i, rightlen = TreeLen - i - 1;
	if (leftlen)//左右子树去建立
		Rebuild(PreOrder + 1, InOrder, leftlen, &(p->left));
	if (rightlen)
		Rebuild(PreOrder + leftlen + 1, InOrder + leftlen + 1, rightlen, &(p->right));

}
void PostOrder(Node *p)//二叉树建完后进行后序遍历。 
{
	if (p != NULL)
	{
		PostOrder(p->left);
		PostOrder(p->right);
		cout << p->val;
	}
}
int main()
{
	char a[110], b[110];
	Node *p;
	while (cin >> a >> b)
	{
		int len = strlen(a);
		Rebuild(a, b, len, &p);
		PostOrder(p);
		cout << endl;
	}
	return 0;
}
```





