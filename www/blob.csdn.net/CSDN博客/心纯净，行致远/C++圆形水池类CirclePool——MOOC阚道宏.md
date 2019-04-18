# C++圆形水池类CirclePool——MOOC阚道宏 - 心纯净，行致远 - CSDN博客





2018年07月03日 14:51:39[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：116








```cpp
#include <iostream> 
using namespace std;
class CirclePool	// 定义一个圆形水池类CirclePool 
{
	private:
		double price;
		double r;
	public:
		CirclePool(double p1=0,double p2=0){
			price=p1;
			r=p2;
		}
		void SetPrice(double x){
			if(x<=0){
				price=0;
				cout<<""<<endl;
			}
			else price=x;
		}
		double GetPrice(){
			return price;
		}
		void SetRadius(double x){	//设置半径 
			if(x<=0){
				r=0;cout<<""<<endl;
			}
			else r=x;
		}
		double GetRadius(){
			return r;
		}
		double GetCost(){
			return (3.14*r*r*price);
		}
};

int main(){
	double totalCost=0;
	
	double r1,r2;
	cin>>r1>>r2;
	CirclePool pool1,pool2;
	
	pool1.SetPrice(10);
	pool1.SetRadius(r1);
	totalCost+=pool1.GetCost() ;
	pool2.SetPrice(10);
	pool2.SetRadius(r2);
	totalCost+=pool2.GetCost() ;
	
	cout<<totalCost<<endl;
	return 0;    
}
```




