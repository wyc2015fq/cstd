# printf 打印百分号_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
printf可以直接打印%，但是如果百分号和数字一起打印，就bug了
然而并不是用反斜杠转义的，是用%自己对自己转义的。
int main()
{
 int a = 2e5+9;
 printf("%%
%d\n",a);
 printf("%
%d\n",a);
 printf("\%
%d\n",a);
 return 0;
}
