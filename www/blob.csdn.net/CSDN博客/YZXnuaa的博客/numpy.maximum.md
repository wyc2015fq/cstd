# numpy.maximum - YZXnuaa的博客 - CSDN博客
2018年05月08日 15:14:32[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：269
https://docs.scipy.org/doc/numpy/reference/generated/numpy.maximum.html
# numpy.maximum
- `numpy.``maximum`(*x1*, *x2*, */*, *out=None*, ***, *where=True*, *casting='same_kind'*, *order='K'*, *dtype=None*, *subok=True*[, *signature*, *extobj*])* = <ufunc 'maximum'>*
Element-wise maximum of array elements.
Compare two arrays and returns a new array containing the element-wisemaxima. If one of the elements being compared is a NaN, then thatelement is returned. If both elements are NaNs then the first isreturned. The latter distinction is important for complex NaNs, whichare defined as at least one of the real or imaginary parts being a NaN.The net effect is that NaNs are propagated.
Returns:
**y** : ndarray or scalar
> 
The maximum of *x1* and *x2*, element-wise.  Returns scalar ifboth  *x1* and *x2* are scalars.
第三个参数是ｏｕｔ，不用写＝了
