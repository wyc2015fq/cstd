# Pytorch/Numpy之squeeze - qq_37385726的博客 - CSDN博客





2018年08月18日 23:17:00[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：146
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[对于Pytorch而言](#%E5%AF%B9%E4%BA%8EPytorch%E8%80%8C%E8%A8%80)

[对于Numpy而言](#%E5%AF%B9%E4%BA%8ENumpy%E8%80%8C%E8%A8%80)

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

> 
### **对于Pytorch而言**

**squeeze(torch.tensor,axis)  返回值与原tensor共享内存，修改返回值中元素值对原tensor也有影响**
- **未指定axis，将张量size()中所有为1的维度去除**
- **指定axis，如果该axis为1则去除，反之保留原来的size不变**

### **对于Numpy而言**

**squeeze(np.ndarray,axis)  返回值与原ndarray共享内存，修改返回值中元素值对原ndarray也有影响**
- **未指定axis，将ndarray shape中所有为1的维度去除**
- **指定axis，如果该axis为1则去除，反之则报错**




# 代码

```python
t = torch.rand(1,2,1,3)
print(t.size(),'\n')

t1 = torch.squeeze(t)
print(t1.size(),'\n')

t2 = torch.squeeze(t,0)
print(t2.size(),'\n')

t3 = torch.squeeze(t,1)
print(t3.size(),'\n')
```

```python
n = np.zeros((1,2,1,3))
print(n.shape,'\n')

n1 = np.squeeze(n)
print(n1.shape,'\n')

n2 = np.squeeze(n,0)
print(n2.shape,'\n')

n3 = np.squeeze(n,1)
print(n3.shape,'\n')
```



# 输出

torch.Size([1, 2, 1, 3]) 

torch.Size([2, 3]) 

torch.Size([2, 1, 3]) 

torch.Size([1, 2, 1, 3]) 



(1, 2, 1, 3) 

(2, 3) 

(2, 1, 3) 

    return squeeze(axis=axis)

ValueError: cannot select an axis to squeeze out which has size not equal to one





