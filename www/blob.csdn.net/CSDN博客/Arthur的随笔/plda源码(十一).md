# plda源码(十一) - Arthur的随笔 - CSDN博客
2019年01月30日 10:42:11[largetalk](https://me.csdn.net/largetalk)阅读数：69
## plda源码(十一)
Alias Table
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019013010080433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhcmdldGFsaw==,size_16,color_FFFFFF,t_70)
sample alias
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130100854500.png)
```
class VoseAlias {
    unsigned short n;				//Dimension
    double wsum;		//Sum of proportions
    std::vector<std::pair<double, unsigned short>> table; //Alias probabilities and indices
    std::vector<double> w;			//Contains proportions and alias probabilities
    unsigned noSamples;		// number of samples between replenishment
    inline unsigned short sample(unsigned short fair_die , double u) const
    {
        bool res = u < table[fair_die].first; 
        return res ? fair_die : table[fair_die].second; 
    }
};
#endif
```
```
double *p = new double[n];
	std::queue<unsigned short> Small, Large;
	for (unsigned short i = 0; i < n; ++i)
	{
		p[i] = (w[i] * n) / wsum;// lp
	}
	for (unsigned short i = 0; i < n; ++i)
	{
		if (p[i] < 1)// $p_i < l^{-1}$
			Small.push(i);//(i, p_i)，因为p_i可以通过i取到，所以只push i就可以了
		else
			Large.push(i);
	}
	while (!(Small.empty() || Large.empty()))
	{
		unsigned short l = Small.front(); Small.pop();
		unsigned short g = Large.front(); Large.pop();
		table[l].first = p[l]; 
		table[l].second = g; //（i, h, p_i), first是p_i, second是h， l就是i
		p[g] = (p[g] + p[l]) - 1;//对比p_h - p_i, This is a more numerically stable option
		if (p[g] < 1)
			Small.push(g);
		else
			Large.push(g);
	}
	while (!Large.empty())
	{
		unsigned short g = Large.front(); Large.pop();
		table[g].first = 1;	
	}
	while (!Small.empty())
	{
		unsigned short l = Small.front(); Small.pop();
		table[l].first = 1;	
	}
	noSamples = 0;
	delete[] p;
```
