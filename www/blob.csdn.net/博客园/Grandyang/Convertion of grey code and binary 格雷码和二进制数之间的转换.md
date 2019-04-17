# Convertion of grey code and binary 格雷码和二进制数之间的转换 - Grandyang - 博客园







# [Convertion of grey code and binary 格雷码和二进制数之间的转换](https://www.cnblogs.com/grandyang/p/4315607.html)







以下转换代码摘自维基百科 Wikipedia：



```
/*
        The purpose of this function is to convert an unsigned
        binary number to reflected binary Gray code.
 
        The operator >> is shift right. The operator ^ is exclusive or.
*/
unsigned int binaryToGray(unsigned int num)
{
        return (num >> 1) ^ num;
}
 
/*
        The purpose of this function is to convert a reflected binary
        Gray code number to a binary number.
*/
unsigned int grayToBinary(unsigned int num)
{
    unsigned int mask;
    for (mask = num >> 1; mask != 0; mask = mask >> 1)
    {
        num = num ^ mask;
    }
    return num;
}
```














