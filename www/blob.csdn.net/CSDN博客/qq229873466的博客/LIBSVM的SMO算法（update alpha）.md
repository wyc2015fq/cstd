# LIBSVM的SMO算法（update alpha） - qq229873466的博客 - CSDN博客

2017年03月17日 16:14:13[qq229873466](https://me.csdn.net/qq229873466)阅读数：581


![](https://img-blog.csdn.net/20170317161316807)

![](https://img-blog.csdn.net/20170317161737958)

![](https://img-blog.csdn.net/20170317161335800)![](https://img-blog.csdn.net/20170317161342566)


```cpp
// update alpha[i] and alpha[j], handle bounds carefully
		
		const Qfloat *Q_i = Q.get_Q(i,active_size);
		const Qfloat *Q_j = Q.get_Q(j,active_size);

		double C_i = get_C(i);
		double C_j = get_C(j);

		double old_alpha_i = alpha[i];
		double old_alpha_j = alpha[j];

		if(y[i]!=y[j])
		{
			double quad_coef = QD[i]+QD[j]+2*Q_i[j];
			if (quad_coef <= 0)
				quad_coef = TAU;
			double delta = (-G[i]-G[j])/quad_coef;
			double diff = alpha[i] - alpha[j];
			alpha[i] += delta;
			alpha[j] += delta;
			
			if(diff > 0)
			{
				if(alpha[j] < 0)
				{
					alpha[j] = 0;
					alpha[i] = diff;
				}
			}
			else
			{
				if(alpha[i] < 0)
				{
					alpha[i] = 0;
					alpha[j] = -diff;
				}
			}
			if(diff > C_i - C_j)
			{
				if(alpha[i] > C_i)
				{
					alpha[i] = C_i;
					alpha[j] = C_i - diff;
				}
			}
			else
			{
				if(alpha[j] > C_j)
				{
					alpha[j] = C_j;
					alpha[i] = C_j + diff;
				}
			}
		}
		else
		{
			double quad_coef = QD[i]+QD[j]-2*Q_i[j];
			if (quad_coef <= 0)
				quad_coef = TAU;
			double delta = (G[i]-G[j])/quad_coef;
			double sum = alpha[i] + alpha[j];
			alpha[i] -= delta;
			alpha[j] += delta;

			if(sum > C_i)
			{
				if(alpha[i] > C_i)
				{
					alpha[i] = C_i;
					alpha[j] = sum - C_i;
				}
			}
			else
			{
				if(alpha[j] < 0)
				{
					alpha[j] = 0;
					alpha[i] = sum;
				}
			}
			if(sum > C_j)
			{
				if(alpha[j] > C_j)
				{
					alpha[j] = C_j;
					alpha[i] = sum - C_j;
				}
			}
			else
			{
				if(alpha[i] < 0)
				{
					alpha[i] = 0;
					alpha[j] = sum;
				}
			}
		}
```

![](https://img-blog.csdn.net/20170317165304727)

```cpp
// update G

		double delta_alpha_i = alpha[i] - old_alpha_i;
		double delta_alpha_j = alpha[j] - old_alpha_j;
		
		for(int k=0;k<active_size;k++)
		{
			G[k] += Q_i[k]*delta_alpha_i + Q_j[k]*delta_alpha_j;
		}
```

