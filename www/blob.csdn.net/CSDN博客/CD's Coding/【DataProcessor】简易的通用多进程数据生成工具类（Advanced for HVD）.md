# 【DataProcessor】简易的通用多进程数据生成工具类（Advanced for HVD） - CD's Coding - CSDN博客





2018年10月03日 17:46:52[糖果天王](https://me.csdn.net/okcd00)阅读数：110








## 0x00 前言

基于先前实现的Python多进程共享内存队列实现的SMQueue（[原文地址](https://blog.csdn.net/okcd00/article/details/82901347)），

认真思考了一下，虽说prototype类的东西写起来很酷，但无法产生太大的使用价值，

为了方便调用和扩展性，还是需要一个工具类来封装一下的，

让其得以轻松运用在各种需要多进程数据生成与传输的任务中。
What’s more:

但感觉单纯的多进程数据生成实现用作一篇文章的point太少了，

于是决定加上和HVD（Horovod）相搭配的功能。
## 0x01 实现思路

[horovod](https://github.com/uber/horovod)是Uber开源的一个Tensorflow并行计算框架，可以让一台机器或是多台机器上的多张GPU协同计算，每个batch会共享一次optimizer的梯度信息，对于数据并行需求的模型训练，推荐可以尝试一下这个框架。

该框架的使用比较简单（参照github项目中的教程），但需要为其实现配套的数据提供模块，本篇主要介绍与其相适配的数据提供的工具类`hvd_processor`

思路说来也不麻烦，大概是这么个思路：

1）首先按文件切分准备好原始数据，glob其文件名列表作为输入，

2）使用前，使用`set_one_epoch`分配数据（对于hvd需要根据`hvd.rank()`进行对应进程的数据分配）

3）在`define_data_info`中定义数据类型与shape，原先的做法需要每个batch的数据shape相同，现在增加了batch_size使得允许在第一维度（batch）上的动态长度，如果不需要或是需要更多维度上的动态长度可自行修改

4）分配数据时，`warp_data`将字典数据wrap成定长的数组并拼接

5）使用时，调用`batches`获取数据，`unwarp_data`将数组拆分成字典数据以供使用，特别的，如果需要进一步的预处理（如作为训练数据时可能还需要mask，cell_len之类的），可以自定义并调用`batch_data_generate`

6）数据为空时（通过runtime超时判断），`reset_data_processor`进行重置进入下一个epoch的循环
```python
```python
data_processor.set_one_epoch(
    data=path_list, data_format='path', mode='train')
for idx, data in enumerate(data_processor.batches()):
    if not data:
        continue
    else:
        # do something
```
```

## 0x02 Source Code

```python
```python
# coding: utf-8
# ==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : hvd_processor.py
#   author   : okcd00 / okcd00@qq.com
#   origin   : lihongwei / lhw446@qq.com
#   date     : 2018-09-20
#   desc     : data processor for hvd usage.
# ==========================================================================
from __future__ import print_function
import os
import glob
import time
import random
import numpy as np
import multiprocessing
import cPickle as pickle
from copy import deepcopy
import horovod.tensorflow as hvd
from collections import OrderedDict

# see shared_memory_queue in my last blog:
# https://blog.csdn.net/okcd00/article/details/82901347
from shared_memory_queue import SMQueue

def random_sort(paths):
    state = random.getstate()
    random.seed(123)
    random.shuffle(paths)
    random.setstate(state)
    return paths


def get_idx_from_path(path):
    idx = os.path.splitext(os.path.basename(path))[0].split('-')
    return tuple([int(_) for _ in idx])


def glob_file_names(dir_name, file_type='.pkl'):
    return glob.glob('{}/*{}'.format(dir_name, file_type))


def batch_data_generate(sentences, merge=False, is_instance=None, need_split=True):
    raise ("need to complete this function for generating")


example_options = {
    "gpus": "0123",  # using which GPUs

    "file_size": 200,  # 每个文件有多少个句子
    "batch_size": 400,  # 每个batch有多少个句子，需要是file_size的整数倍
    "max_seg_len": 48,  # word counts in single segment
    "max_can_len": 127,  # candidate counts for single word

    "emb_dim": 128,  # word_embedding dimension is 128
    "n_words": 87780,  # use leading x words in dictionary
    "lstm_dim": 128,  # lstm_hidden dimension is 128
    'use_hvd': True,  # using hvd or not
    'multi_processing': True,  # using multi_processing or not
    'shared_memory_size': 8,  # size for shared memory queue.
    'data_processor_number': 8,  # number of processors
    'max_batch_size': 1000,  # maximum batch_size for wraping
}


class DataProcessor(object):
    ''' basic data processor for training model by using shared memory'''

    def __init__(self, options, generate_y=False, debug=False):
        hvd.init()
        self.generate_y = generate_y
        self.idx = None
        self.mode = None
        self.data = None
        self.debug = debug
        self.data_format = None
        self.options = options
        self.data_info = self.define_data_info()
        self.data_producers = []
        self.batch_file = options['batch_size'] / options['file_size']

        self.f_data_size = sum([np.prod(data_info['shape']) for data_info in self.data_info.values()
                                if data_info['dtype'] == np.float32])
        self.i_data_size = sum([np.prod(data_info['shape']) for data_info in self.data_info.values()
                                if data_info['dtype'] == np.int32])
        if self.options.get('multi_processing'):
            self.multi_processing = True
            self.smque = SMQueue(
                self.options['shared_memory_size'],
                self.f_data_size, self.i_data_size)

    def help(self):
        print ("""
        data_processor.set_one_epoch(
            data=path_list, data_format='path', mode='train')
        for idx, data in enumerate(data_processor.batches()):
            if not data:
                continue
            else:
                # do something
        """)

    def debug_info(self, text):
        if self.debug:
            print("[D_{}] {}, {}".format(
                os.getpid(), text, time.ctime()))

    def get_data_for_hvd(self, data):
        if hvd.size() == 1:
            return data
        else:
            data = random_sort(deepcopy(data))
            # file_size / file_per_batch / hvd_cores
            batch_num = len(data) / self.batch_file / hvd.size()
            start = hvd.rank() * batch_num * self.batch_file
            end = (hvd.rank() + 1) * batch_num * self.batch_file
            return data[start: end]

    def set_one_epoch(self, data, data_format='path', mode='infer', multi_processing=None):
        '''
            set one epoch by path or data
            data_format = path/data
            mode = train/valid/infer
        '''
        if not mode.startswith('infer'):
            assert self.generate_y

        self.mode = mode
        self.data = self.get_data_for_hvd(data)
        self.data_len = self.data.__len__()
        self.data_format = data_format
        np.random.shuffle(self.data)

        if multi_processing is None:
            multi_processing = self.multi_processing
        if multi_processing:
            self.run_one_epoch()

    def run_one_epoch(self):
        ''' create multi data producer and run '''
        self.data_producers = []
        for idx in range(self.options['data_processor_number']):
            producer = multiprocessing.Process(target=self.data_producer, args=(idx,))
            self.data_producers.append(producer)
            producer.start()

    def data_producer(self, idx):
        ''' produce data and put into shared memory queue '''
        self.debug_info("Data producer <{}> is working now".format(idx))
        for c_idx in range(idx, len(self.data) / self.batch_file,
                           self.options['data_processor_number']):
            # d_idx = c_idx * self.options['batch_size']
            batch_data = self.data[c_idx: c_idx + self.batch_file]  # one file per batch
            prepared_data = self.prepare_data(batch_data)
            f_data, i_data = self.wrap_data(prepared_data)
            self.smque.put(f_data, i_data)

    def reset_data_producers(self):
        ''' kill all data_producers and reset shared memory queue '''
        for producer in self.data_producers:
            producer.terminate()
            producer.join()

        self.debug_info("Reset shared memory queue finished.")
        self.smque.reset()

    def batches(self):
        ''' yield each batch data on one epoch '''
        if self.multi_processing:
            for _ in range(len(self.data) / self.batch_file):
                f_data, i_data = self.smque.get()
                if (f_data is None) and (i_data is None):
                    self.reset_data_producers()
                    print('[{}] shared memory queue is empty now.'.format(os.getpid()))
                    return
                yield self.unwrap_data(f_data, i_data)
        else:
            for c_idx in range(len(self.data) / self.batch_file):
                d_idx = c_idx * self.batch_file
                batch_data = self.data[d_idx: d_idx + self.batch_file]
                data = self.prepare_data(batch_data)
                yield data

        last_batch_size = len(self.data) % self.batch_file
        if last_batch_size != 0:
            last_batch = self.data[-last_batch_size:]
            last_batch = self.prepare_data(last_batch)
            # dtype transfer
            for name, data_info in self.data_info.items():
                dtype = data_info['dtype']
                last_batch[name] = np.asarray(last_batch[name], dtype=dtype)
            yield last_batch

    @staticmethod
    def np_padding(arr, pad_size, padding=0):
        shape = arr.shape

        def get_pad_size(i):
            if pad_size.__len__() <= i:
                return 0
            else:
                return pad_size[i] - shape[i]

        return np.lib.pad(
            arr,
            [(0, get_pad_size(i)) for i in range(shape.__len__())],
            mode='constant',
            constant_values=padding)

    def wrap_data(self, data):
        ''' wrap data for putting to smque '''
        f_flatten_data, i_flatten_data = [], []
        max_batch_size = self.options.get('max_batch_size')
        for name, data_info in self.data_info.items():
            if name == 'batch_size':
                i_flatten_data.append(
                    np.ones(1, dtype=np.int32) *\
                    data.values()[-1].__len__())
                continue
            dtype = data_info['dtype']
            item = np.asarray(data[name], dtype=dtype)
            if dtype == np.float32:
                f_flatten_data.append(
                    self.np_padding(item, [max_batch_size]).flatten())
            elif dtype == np.int32:
                i_flatten_data.append(
                    self.np_padding(item, [max_batch_size]).flatten())
        if f_flatten_data:
            f_flatten_data = np.concatenate(f_flatten_data)
        if i_flatten_data:
            i_flatten_data = np.concatenate(i_flatten_data)
        return f_flatten_data, i_flatten_data

    def unwrap_data(self, f_data, i_data):
        ''' unwrap smque data '''
        new_data = OrderedDict()
        batch_size = 0
        f_acc_size, i_acc_size = 0, 0
        for name, data_info in self.data_info.items():
            shape = data_info['shape']
            dtype = data_info['dtype']
            if dtype == np.float32:
                new_data[name] = np.reshape(
                    f_data[f_acc_size: f_acc_size + np.prod(shape)], shape)
                f_acc_size += np.prod(shape)
            elif dtype == np.int32:
                new_data[name] = np.reshape(
                    i_data[i_acc_size: i_acc_size + np.prod(shape)], shape)
                i_acc_size += np.prod(shape)
            # assignment for batch_size
            if name == 'batch_size':
                batch_size = new_data[name][0]
            else:
                new_data[name] = new_data[name][:batch_size, ...]
        return new_data

    def load_data_from_path(self, batch_data):
        data_case = []
        self.debug_info("Now loading data from {} file_path.".format(
            batch_data.__len__()))
        if isinstance(batch_data, list):
            for path in batch_data:
                with open(path, 'rb') as opened_file:
                    data_case.extend(pickle.load(opened_file))
        else:
            data_case.extend(pickle.load(open(batch_data, 'r')))
        return data_case

    def prepare_data(self, batch_data):
        ''' prepare data for train or infer '''

        if self.data_format == 'path':
            sentences = self.load_data_from_path(batch_data)
        else:  # data_format is 'data'
            sentences = batch_data
        self.debug_info("Data prepared, with {} sentences.".format(
            sentences.__len__()))
        return batch_data_generate(
            sentences, merge=True, is_instance=None, need_split=True)

    def define_data_info(self):
        # change this to your own data form.
        data_info = OrderedDict()
        data_info['batch_size'] = {
            'shape': (1),
            'dtype': np.int32}
        data_info['input_data'] = {
            'shape': (self.options.get('max_batch_size'),
                      self.options.get('max_seg_len') + 2),
            'dtype': np.int32}
        data_info['cell_lens'] = {
            'shape': (self.options.get('max_batch_size')),
            'dtype': np.int32}
        data_info['candidates'] = {
            'shape': (self.options.get('max_batch_size'),
                      self.options.get('max_seg_len'),
                      self.options.get('max_can_len')),
            'dtype': np.int32}
        if self.generate_y:
            data_info['target'] = {
                'shape': (self.options.get('max_batch_size'),
                          self.options.get('max_seg_len')),
                'dtype': np.int32}
        return data_info
```
```






