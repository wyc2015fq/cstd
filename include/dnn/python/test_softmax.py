
import os
import numpy
import struct
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.python.platform import gfile



a = tf.constant(0.1, shape=[2, 3, 5])
ss=tf.nn.softmax(a)
sess = tf.Session()
print(sess.run(ss))
