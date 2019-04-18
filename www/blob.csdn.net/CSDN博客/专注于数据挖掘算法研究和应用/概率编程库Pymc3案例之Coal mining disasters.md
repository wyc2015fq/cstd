# 概率编程库Pymc3案例之Coal mining disasters - 专注于数据挖掘算法研究和应用 - CSDN博客
2019年03月21日 14:54:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：36
github地址：[https://github.com/pymc-devs/pymc3](https://github.com/pymc-devs/pymc3)
案例说明：[https://docs.pymc.io/notebooks/getting_started](https://docs.pymc.io/notebooks/getting_started)
API参考：[https://docs.pymc.io/api/inference.html#module-pymc3.sampling](https://docs.pymc.io/api/inference.html#module-pymc3.sampling)
代码：
```python
# -*- coding: utf-8 -*-
'''
Created on 2019年3月20日
@author: cvter
'''
from matplotlib import pyplot as plt
import numpy as np
import pymc3 as pm
def CoalMiningDisasters():
    disaster_data = np.ma.masked_values([4, 5, 4, 0, 1, 4, 3, 4, 0, 6, 3, 3, 4, 0, 2, 6,
                            3, 3, 5, 4, 5, 3, 1, 4, 4, 1, 5, 5, 3, 4, 2, 5,
                            2, 2, 3, 4, 2, 1, 3, -999, 2, 1, 1, 1, 1, 3, 0, 0,
                            1, 0, 1, 1, 0, 0, 3, 1, 0, 3, 2, 2, 0, 1, 1, 1,
                            0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 0, 0, 1, 1, 0, 2,
                            3, 3, 1, -999, 2, 1, 1, 1, 1, 2, 4, 2, 0, 0, 1, 4,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1], value=-999)
    years = np.arange(1851, 1962)
    '''
    plt.plot(years, disaster_data, 'o', markersize=8);
    plt.ylabel("Disaster count")
    plt.xlabel("Year")
    plt.show()
    '''
    with pm.Model() as disaster_model:
        switchpoint = pm.DiscreteUniform('switchpoint', lower=years.min(), upper=years.max(), testval=1900)
    
        # Priors for pre- and post-switch rates number of disasters
        early_rate = pm.Exponential('early_rate', .1)
        late_rate = pm.Exponential('late_rate', .1)
    
        # Allocate appropriate Poisson rates to years before and after current
        rate = pm.math.switch(switchpoint >= years, early_rate, late_rate)
    
        disasters = pm.Poisson('disasters', rate, observed=disaster_data)
        
    with disaster_model:
        trace = pm.sample(10000)
        
    print (pm.traceplot(trace))
    
    plt.figure(figsize=(10, 8))
    plt.plot(years, disaster_data, '.')
    plt.ylabel("Number of accidents", fontsize=16)
    plt.xlabel("Year", fontsize=16)
    
    plt.vlines(trace['switchpoint'].mean(), disaster_data.min(), disaster_data.max(), color='C1')
    average_disasters = np.zeros_like(disaster_data, dtype='float')
    for i, year in enumerate(years):
        idx = year < trace['switchpoint']
        average_disasters[i] = (trace['early_rate'][idx].sum() + trace['late_rate'][~idx].sum()) / (len(trace) * trace.nchains)
    
    sp_hpd = pm.hpd(trace['switchpoint'])
    plt.fill_betweenx(y=[disaster_data.min(), disaster_data.max()],
                      x1=sp_hpd[0], x2=sp_hpd[1], alpha=0.5, color='C1');
    plt.plot(years, average_disasters,  'k--', lw=2);
    plt.show()
def main():
    CoalMiningDisasters()
    
if __name__ == "__main__":
    main()
```
