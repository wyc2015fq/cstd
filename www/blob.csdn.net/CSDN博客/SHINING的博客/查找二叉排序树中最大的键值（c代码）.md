# 查找二叉排序树中最大的键值（c代码） - SHINING的博客 - CSDN博客
2005年09月03日 10:09:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：2072

#include<stdio.h>
#include<conio.h>
struct  tree{
 char key;
 struct tree *Lchild,*Rchild;
};
struct tree *create_btree(struct tree *t,struct tree *r,char key)
{  
 if (r ==0 )
 {   
  r=new (struct tree);
  if ( r == 0)
  { 
   printf("Out of memory/n");   return 0 ; 
  }
  r->Lchild= 0;  r->Rchild=0;   r->key=key;
  if (t)
  { 
   if(key<t->key)   t->Lchild=r;
   else                    
    t->Rchild=r;
  }
  else
  {  
   r->Rchild=0;   r->Lchild = 0; 
  }
  return  r;
 }    
 if (key < r->key)
  create_btree(r,r->Lchild,key);
 if(key>=r->key)
  create_btree(r,r->Rchild,key);
 return t;
};
struct tree *search_btree(struct tree *t,char key1)
{   struct tree *p;
    p=t;
 if (!p)
 {  printf("Empty btree/n");   return p;  }    
 while(p->Rchild) {p=p->Rchild;}; 
 printf("Successful search/n key1=%c/n",p->key);
 return p;
};
 void main()
{   
 char s[100], c,  e,q;                     
 struct tree *t=0, *p;       
 printf("Input a letter for Creating the Binary_Tree ( Directly press <Enter> to stop ):/n"); 
    while (*s){
  printf("/nInput a letter: "); 
  e=getch();     /*#include<conio.h>*/
  putch(e);       /*#include<conio.h>*/
  if(e==13) break;
  if (!t)
   t=create_btree(t,t,e);
  else
   create_btree(t,t,e);
 };
     search_btree(t,0); 
  printf("/n"); 
  printf("结束请按q！");
   if(getchar()=='q') printf("再见");
   else  {while(1);};
 }
