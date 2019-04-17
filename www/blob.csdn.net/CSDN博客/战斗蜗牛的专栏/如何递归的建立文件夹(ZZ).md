# 如何递归的建立文件夹(ZZ) - 战斗蜗牛的专栏 - CSDN博客





2009年12月12日 13:33:00[vbskj](https://me.csdn.net/vbskj)阅读数：797标签：[string																[access																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)





 int   create_dir_recursive(const   string&   d)   
  {   
 string   dir;   
  dir.assign(d);   
  if(dir[dir.size()-1]   !=   '//')   
  dir.append(1,'//');   
  string::size_type   b,e;   
  b   =   e   =   0;   
  int   ret=0;
  //ACE_stat   st;   
  while(b   <   dir.size())   
  {   
  e   =   dir.find_first_of('//',b);   
  if(e   ==   string::npos)   
  break;   
  if(e   ==   2)   
  e++;   //   hack   for   root   directory   
  ret   =   _access(dir.substr(0,e).c_str(),0);   
  if(ret   ==   -1)   
  if(!CreateDirectory(dir.substr(0,e).c_str(),NULL))     
  return   -1;   
  b   =   e   +   1;   
  }   
  return   ret;   

  }



返回值：

   0： 文件夹存在，创建失败

   -1：创建成功](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)




