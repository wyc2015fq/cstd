# 解决The truth value of an array with more than one element is ambiguous. Use a.any() or a.all()问题 - YZXnuaa的博客 - CSDN博客
2018年04月08日 15:42:29[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2463
def bottom_data_is(self, x, s_prev=None, h_prev=None):
    # if this is the first lstm node in the network
if [s_prev == None]:
        s_prev = np.zeros_like(self.state.s)
    if [h_prev == None]:
        h_prev = np.zeros_like(self.state.h)
    # save data for use in backprop
self.s_prev = s_prev
    self.h_prev = h_prev
打上括号就行
