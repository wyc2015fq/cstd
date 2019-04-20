# 剑指offer--从一段字符串中提取数字 - 风雪夜归人 - CSDN博客
2019年04月10日 15:57:19[cdjccio](https://me.csdn.net/qq_34624951)阅读数：8
问题：从一段字符串中获取数字
example：
input:1ab2cd3ef45g
output:1 2 3 45
思路：解析字符串
**局部整型变量、整型数组的初始化：**
**       如果部分赋值，则其余部分初始化为0；比如int a[5] = {1, 2}; 那么编译器会帮你把后面的三个元素置为0。**
**       如果未赋值，则都是随机值。比如int a[5]; 则编译器将数组元素赋值为随机值。**
**全局整型变量、静态[局部整形变量](https://www.baidu.com/s?wd=%E5%B1%80%E9%83%A8%E5%8F%98%E9%87%8F&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的初始值是0。**
解法一：
```cpp
/*
    只能提取出数字  可以分割出数字
    input:1ab2cd3ef45g
    output:1 2 3 45
*/
void getNumber(const string str){
    int Number[100] = {0}; // 如果未赋值，则为随机值，将导致后面计算错误
    int index = 0;   // 数组下标
    int len = str.length();
    for (int i = 0; i<len; i++){
        while (!(str[i] >= '0' && str[i] <= '9')){
            i ++;
        }
        while ((str[i] >= '0' && str[i] <= '9')){
            Number[index] = Number[index] * 10 + (str[i] - '0');
            i ++;
        }
        index ++;
    }
    for (int i = 0; i<index; i++){
        cout << Number[i] << " ";
    }
    cout << endl;
}
int main()
{
    string str = "1ab2cd3ef45g";
    getNumber(str);
    return 0;
}
```
