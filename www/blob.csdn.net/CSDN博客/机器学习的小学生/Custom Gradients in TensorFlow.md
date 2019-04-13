
# Custom Gradients in TensorFlow - 机器学习的小学生 - CSDN博客


2017年10月23日 20:46:54[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：269个人分类：[tensorflow																](https://blog.csdn.net/xuluhui123/article/category/6934245)


Custom Gradients in TensorFlow
TensorFlow defines deep learning models as computational graphs, where nodes are called**ops**, short for operations, and the data that flows between these ops are called**tensors**. Given a graph of ops, TensorFlow uses automatic differentiation to compute gradients. The theory behind automatic differentiation is that all numeric computations are composed of a finite set of elementary operations for which the gradient is well defined. In TensorFlow, each op must then have a well defined gradient for automatic differentiation to work properly.
When adding new ops in TensorFlow, you must use`tf.RegisterGradient`to register a gradient function which computes gradients with respect to the ops’ input tensors given gradients with respect to the ops’ output tensors. For example, let’s say we have an operation`Square`which computes the square of the input. Its forward activity and backward activity are defined as follows:
```python
Forward
```
```python
:
```
```python
$y
```
```python
= x^
```
```python
2
```
```python
$
```
```python
backward:
```
```python
$y
```
```python
=
```
```python
2
```
```python
x$
```
……
网址：
[https://uoguelph-mlrg.github.io/tensorflow_gradients/](https://uoguelph-mlrg.github.io/tensorflow_gradients/)

