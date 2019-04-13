
# python三维生命游戏（一）-------二维生命游戏 - leofionn的博客 - CSDN博客


2018年05月10日 15:52:59[leofionn](https://me.csdn.net/qq_36142114)阅读数：444


```python
import numpy as np
import matplotlib.pyplot as plt
 
class GameOfLife(object):
 
  def __init__(self, cells_shape):
    # 矩阵的四周不参与运算
    self.cells = np.zeros(cells_shape)
    real_width = cells_shape[0] - 2
    real_height = cells_shape[1] - 2
    self.cells[1:-1, 1:-1] = np.random.randint(2, size=(real_width, real_height))
    b = self.cells[1:-1, 1:-1]
    print (b)
    self.timer = 0
    self.mask = np.ones(9)
    self.mask[4] = 0
   
  def update_state(self):
    """更新一次状态"""
    buf = np.zeros(self.cells.shape)
    cells = self.cells
    for i in range(1, cells.shape[0] - 1):
      for j in range(1, cells.shape[0] - 1):
        # 计算该细胞周围的存活细胞数
        neighbor = cells[i-1:i+2,j-1:j+2].reshape((-1, ))
        print(neighbor)
        neighbor_num = np.convolve(self.mask, neighbor, 'valid')[0]
        print(neighbor_num)
        if neighbor_num == 3:
          buf[i, j] = 1
        elif neighbor_num == 2:
          buf[i, j] = cells[i, j]
        else:
          buf[i, j] = 0
    self.cells = buf
    self.timer += 1
   
  def plot_state(self):
    """画出当前的状态"""
    plt.title('Iter :{}'.format(self.timer))
    plt.imshow(self.cells)
    #plt.show
    
  def update_and_plot(self, n_iter):
    """更新状态并画图
    Parameters
    ----------
    n_iter : 更新的轮数
    """
    plt.ion()
    for _ in range(n_iter):
      plt.title('Iter :{}'.format(self.timer))
      plt.imshow(self.cells)
      self.update_state()
      plt.pause(0.2)
    plt.ioff()
```


