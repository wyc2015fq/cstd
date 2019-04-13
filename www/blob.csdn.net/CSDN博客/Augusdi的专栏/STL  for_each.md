
# STL: for_each - Augusdi的专栏 - CSDN博客


2016年10月11日 12:58:23[Augusdi](https://me.csdn.net/Augusdi)阅读数：738


```python
#include <algorithm>  
#include <iterator>  
#include <iostream>  
#include <vector>  
using namespace std;  
  
class GenByTwo {  
    public:  
        GenByTwo (int _seed = 0):seed(_seed){}  
        int operator() () {return seed += 2;}  
  
    private:  
        int seed;  
};  
  
void operate(int &rhs)  
{  
    cout << rhs << " ";  
    rhs += 1;  
}  
int main(int argc, char* argv[])  
{  
    vector<int> v(10);  
    generate(v.begin(), v.end(), GenByTwo(2));  
    for_each(v.begin(), v.end(), operate);  
  
    std::cout << std::endl;
    //print  
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));  
  
    return 0;  
}
```
4 6 8 10 12 14 16 18 20 22
5 7 9 11 13 15 17 19 21 23

```python
#include <iostream>     // std::cin, std::cout
#include <iterator>     // std::istream_iterator
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

map<string , int> m;
void print( const pair<string, int> &r)
{
		cout << r.first << "   " << r.second << endl;
}
void record( const string &s)
{
		m[ s ] ++;
}
int main()
{
	double beginTime = clock();
	fstream in("test.txt");
	istream_iterator<string> ii(in);
	istream_iterator<string> eos;
	for_each(ii,eos,record);
	for_each( m.begin(), m.end(), print);
	double endTime = clock();
	cout << " time: " << endTime - beginTime << " ms" << endl;
	return 0;
}
```
time: 66 ms


﻿﻿

