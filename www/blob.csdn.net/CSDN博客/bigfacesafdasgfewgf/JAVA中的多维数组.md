# JAVA中的多维数组 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:46:00[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：484标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)









- 声明一个二维数组：int [] [] matrix = new int [5] [5]; 或者使用：int [] [] matrix = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} }; 这是一个直接初始化的格式；
- 获取二维数组的长度。int [] [] martix = new int [3] [4]; 那么martix.length=3; 即行数；matrix[0].length=4; 即列数；
- 创建一个锯齿数组：int [] [] triangleArray = new int [5] [];  注意：这里第一个维数一定要确定，因为锯齿数组的行数都是一定的，只有每行的元素个数不一定；
- 多维数组的定义方式和前面二维数组类似。double [] [] [] scores = new double [10] [24] [2]; 求数组长度的方式也和前面类似；




