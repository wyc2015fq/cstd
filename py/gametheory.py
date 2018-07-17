import random

aa = [[3, -2], [-2, 1]]

print(random.random())
from mpl_toolkits.mplot3d import Axes3D  
import numpy as np  
from matplotlib import pyplot as plt  
  
fig = plt.figure()  
ax = Axes3D(fig)  
x=np.arange(0,1,0.01)
y=np.arange(0,1,0.01)
X, Y = np.meshgrid(x, y)#网格的创建，这个是关键  
Z=3*X*Y-2*X*(1-Y)-2*(1-X)*Y+(1-X)*(1-Y)
plt.xlabel('x')  
plt.ylabel('y')  
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='rainbow')  
plt.show()  