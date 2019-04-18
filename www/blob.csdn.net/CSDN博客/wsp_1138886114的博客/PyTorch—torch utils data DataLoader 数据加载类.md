# PyTorch—torch.utils.data.DataLoader 数据加载类 - wsp_1138886114的博客 - CSDN博客





2019年01月05日 22:33:12[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：562











### 文章目录
- [DataLoader(object)类：](#DataLoaderobject_6)
- [_DataLoaderIter(object)类](#_DataLoaderIterobject_109)
- [__next__函数](#__next___127)
- [pin_memory_batch()](#pin_memory_batch_142)
- [_get_batch函数](#_get_batch_145)
- [_process_next_batch函数](#_process_next_batch_148)
- [_put_indices函数](#_put_indices_151)



PyTorch中数据读取的一个重要接口是**torch.utils.data.DataLoader**。

该接口主要用来将自定义的数据读取接口的输出或者PyTorch已有的数据读取接口的输入按照batch size封装成Tensor，后续只需要再包装成Variable即可作为模型的输入，因此该接口有承上启下的作用。


> 
dataloader.py的github地址：[https://github.com/pytorch/pytorch/blob/master/torch/utils/data/dataloader.py](https://github.com/pytorch/pytorch/blob/master/torch/utils/data/dataloader.py)

主要包含 **DataLoader** 和 **DataLoaderIter** 两个类。


### DataLoader(object)类：

**DataLoader** 下有`__init__`，`__setattr__`，`__iter__`，`__len__`等四个函数，详情如下。
- **`__init__` 函数**

1.`dataset(Dataset)`，数据读取接口（比如torchvision.datasets.ImageFolder）或者自定义的数据接口的输出，该输出是torch.utils.data.Dataset类的对象(或者继承自该类的自定义类的对象)。

2.`batch_size (int, optional)`，批训练数据量的大小，根据具体情况设置即可。（默认：1）

3.`shuffle (bool, optional)`，打乱数据，一般在训练数据中会采用。（默认：False）

4.`sampler (Sampler, optional)`，从数据集中提取样本的策略。如果指定，“shuffle”必须为false。一般默认即可。

5.`batch_sampler (Sampler, optional)`，和batch_size、shuffle等参数互斥，一般用默认。

6.`num_workers`，这个参数必须大于等于0，其他大于0的数表示通过多个进程来导入数据，可以加快数据导入速度。（默认：0）

7.`collate_fn (callable, optional)`，合并样本清单以形成小批量。用来处理不同情况下的输入dataset的封装，一般采用默认即可，除非你自定义的数据读取输出非常少见。

8.`pin_memory (bool, optional)`：数据加载器将把张量复制到CUDA内存中，然后返回它们。也就是一个数据拷贝的问题。

9.`drop_last (bool, optional)`: 如果数据集大小不能被批大小整除，则设置为“true”以除去最后一个未完成的批。如果“false”那么最后一批将更小。（默认：false）

10.`timeout(numeric, optional)`：设置数据读取超时，但超过这个时间还没读取到数据的话就会报错。（默认：0）

11.`worker_init_fn (callable, optional)`如果不是：`none`，则在种子设定之后和数据加载之前，将在每个工作进程上调用它，并输入工作进程ID（[0，num_workers-1）（默认：None)
```python
import torch
import torch.multiprocessing as multiprocessing
from . import SequentialSampler, RandomSampler, BatchSampler
from . import _utils
import threading
from torch._six import queue


default_collate = _utils.collate.default_collate

class DataLoader(object):

    __initialized = False

    def __init__(self, dataset, batch_size=1, shuffle=False, sampler=None,
                 batch_sampler=None, num_workers=0, collate_fn=default_collate,
                 pin_memory=False, drop_last=False, timeout=0,
                 worker_init_fn=None):
        self.dataset = dataset
        self.batch_size = batch_size
        self.num_workers = num_workers
        self.collate_fn = collate_fn
        self.pin_memory = pin_memory
        self.drop_last = drop_last
        self.timeout = timeout
        self.worker_init_fn = worker_init_fn

        if timeout < 0:
            raise ValueError('timeout option should be non-negative')

        if batch_sampler is not None:
            if batch_size > 1 or shuffle or sampler is not None or drop_last:
                raise ValueError('batch_sampler option is mutually exclusive '
                                 'with batch_size, shuffle, sampler, and drop_last')
            self.batch_size = None
            self.drop_last = None

        if sampler is not None and shuffle:
            raise ValueError('sampler option is mutually exclusive with '
                             'shuffle')

        if self.num_workers < 0:
            raise ValueError('num_workers option cannot be negative; '
                             'use num_workers=0 to disable multiprocessing.')

        if batch_sampler is None:
            if sampler is None:
                if shuffle:
                    sampler = RandomSampler(dataset)
                else:
                    sampler = SequentialSampler(dataset)
            batch_sampler = BatchSampler(sampler, batch_size, drop_last)

        self.sampler = sampler
        self.batch_sampler = batch_sampler
        self.__initialized = True

    def __setattr__(self, attr, val):
        if self.__initialized and attr in ('batch_size', 'sampler', 'drop_last'):
            raise ValueError('{} attribute should not be set after {} is '
                             'initialized'.format(attr, self.__class__.__name__))

        super(DataLoader, self).__setattr__(attr, val)

    def __iter__(self):
        return _DataLoaderIter(self)

    def __len__(self):
        return len(self.batch_sampler)
```

在__init__初始化函数中：

我们可以看到第1个`dataset(Dataset)`，调用`Dataset`类。

第4/(5一样继承)个`sampler (Sampler, optional)`调用`Sampler`类函数中：

RandomSampler类表示随机采样且不重复，所以起到的就是shuffle的作用。

BatchSampler类则是把batch size个RandomSampler类对象封装成一个，这样就实现了随机选取一个batch的目的。
> 
这两个采样类都是定义在sampler.py脚本中，

地址：[https://github.com/pytorch/pytorch/blob/master/torch/utils/data/sampler.py。](https://github.com/pytorch/pytorch/blob/master/torch/utils/data/sampler.py%E3%80%82)


当代码运行到要从torch.utils.data.DataLoader类生成的对象中取数据的时候，比如：

train_data=torch.utils.data.DataLoader(…)

for i, (input, target) in enumerate(train_data):

…

就会调用taLoader类的__iter__方法，__iter__方法返回 DataLoaderIter(self)类，接下来介绍 **_DataLoaderIter**。
### _DataLoaderIter(object)类

关于初始化__init__我们只看：
`self.sample_iter = iter(self.batch_sampler)`，得到的self.sample_iter可以通过next(self.sample_iter)来获取batch size个数据的index。
`self.rcvd_idx`表示读取到的一个batch数据的index，初始化为0，该值在迭代读取数据的时候会用到。
**self.worker_result_queue = multiprocessing.Queue()**

其中multiprocessing是Python中的多进程管理包，

而 threading则是Python中的多线程管理包。- `if self.num_workers > 0:` 是否设置为多进程读取数据，若无，则无需初始化操作

在此if语句中通过multiprocessing.SimpleQueue()类创建了一个简单的队列对象。multiprocessing.Process类就是构造进程的类，这里根据设定的进程数来启动，然后赋值给self.workers。

接下来一个 `for 循环`就通过调用start方法依次启动self.workers中的进程。

接下来关于`self.pin_memory`的判断语句，该判断语句内部主要是实现了多线程操作。当self.pin_memory为True的时候，就会把数据拷到CUDA中。

self.data_queue = queue.Queue() 是通过Python的queue模块初始化得到一个先进先出的队列（queue模块用queue.LifoQueue()初始化，得到先进后出的队列），queue模块主要应用在多线程读取数据中。在threading.Thread的args参数中，第一个参数in_data就是一个进程的数据，一个进程中不同线程的数据也是通过队列来维护的，这里采用的是Python的queue模块来初始化得到一个队列：queue.Queue()。总的来说，如果设置为多进程读取数据，那么就会采用队列的方式来读，如果不是采用多进程来读取数据，那就采用普通方式来读。

初始化结束后，就会调用__next__方法，接下来介绍。
##### __next__函数

**第一个if语句 self.num_workers=0:**，不采用多进程进行数据读取。

首先通过`indices = next(self.sample_iter)`获取长度为batch size的列表：indices，这个列表的每个值表示一个batch中每个数据的index，每执行一次next操作都会读取一批长度为batch size的indices列表。

然后通过`batch = self.collate_fn()`函数将batch size个tuple（每个tuple长度为2，其中第一个值是数据，Tensor类型，第二个值是标签，int类型）封装成一个list，这个list长度为2，两个值都是Tensor，一个是batch size个数据组成的FloatTensor，另一个是batch size个标签组成的LongTensor。所以简单讲self.collate_fn函数就是将batch size个分散的Tensor封装成一个Tensor。
`if self.pin_memory:`,`batch = pin_memory_batch(batch)`中pin_memory_batch函数将输入batch的每个Tensor都拷贝到CUDA中，该函数后面会详细介绍。
**第二个if语句 if self.rcvd_idx in self.reorder_dict:** 当前batch的index是否之前已经读出来过。已读出来的index和batch数据保存在self.reorder_dict字典中，可以结合最后的while语句一起看，因为self.reorder_dict字典的更新是在最后的while语句中），如果之前已经读取过了，就根据这个index从reorder_dict字典中弹出对应的数据。最后返回batch数据的时候是 return self._process_next_batch(batch)，该方法后面会详细介绍。主要做是获取下一个batch的数据index信息。（整个判断语句用来检查是否已生成下一批样本。）

**第三个if语句 if self.batches_outstanding == 0**：

self.batches_outstanding的值在前面初始中调用self._put_indices()方法时修改了，所以假设你的进程数self.num_workers设置为3，那么这里self.batches_outstanding就是3*2=6，可具体看self._put_indices()方法。

最后的while循环就是真正用来从队列中读取数据的操作：
`idx, batch = self._get_batch()`，通过调用_get_batch()方法来读取（后面有介绍），简单讲就是调用了队列的get方法得到下一个batch的数据，得到的batch一般是长度为2的列表，列表的两个值都是Tensor，分别表示数据（是一个batch的）和标签。_get_batch()方法除了返回batch数据外，还得到另一个输出：idx。
`if idx != self.rcvd_idx` 表示如果你读取到的batch的index不等于当前想要的index:selg,rcvd_idx，那么就执行 self.reorder_dict[idx] = batch 将读取到的数据保存在字典self.reorder_dict中，直到读取到的数据的index等于self.rcvd_idx。
##### pin_memory_batch()

pin_memory_batch函数不是定义在DataLoader类或DataLoaderIter类中。该函数主要是对batch中的Tensor执行batch.pin_memory()操作，这里的很多条件语句只是用来判断batch的类型，假如batch是一个列表，列表中的每个值是Tensor，那么就会执行 elif isinstance(batch, collections.Sequence):这个条件，从而遍历该列表中的每个Tensor，然后执行第一个条件语句的内容： return batch.pin_memory()

##### _get_batch函数

主要根据是否设置了超时时间来操作，如果超过指定的超时时间后没有从队列中读到数据就报错

##### _process_next_batch函数

更新下下一个要读取的batch数据的index。然后调用_put_indices()方法获取下一个batch的每个数据的index。

##### _put_indices函数

该函数主要实现从self.sample_iter中读取下一个batch数据中每个数据的index：indices = next(self.sample_iter, None)，注意这里的index和前面idx是不一样的，这里的index是一个batch中每个数据的index，idx是一个batch的index；然后将读取到的index通过调用queue对象的put方法压到队列self.index_queue中：self.index_queue.put((self.send_idx, indices))

```python
def pin_memory_batch(batch):
    if torch.is_tensor(batch):
        return batch.pin_memory()
    elif isinstance(batch, string_classes):
        return batch
    elif isinstance(batch, collections.Mapping):
        return {k: pin_memory_batch(sample) for k, sample in batch.items()}
    elif isinstance(batch, collections.Sequence):
        return [pin_memory_batch(sample) for sample in batch]
    else:
        return batch
```

```python
class _DataLoaderIter(object):
    """Iterates once over the DataLoader's dataset, as specified by the sampler"""
    # NOTE [ Data Loader Multiprocessing Shutdown Logic ]
    # Our data model looks like this (queues are indicated with curly brackets):
    #
    #                main process                              ||
    #                     |                                    ||
    #               {index_queue}                              ||
    #                     |                                    ||
    #              worker processes                            ||     DATA
    #                     |                                    ||
    #            {worker_result_queue}                         ||     FLOW
    #                     |                                    ||
    #      pin_memory_thread of main process                   ||   DIRECTION
    #                     |                                    ||
    #               {data_queue}                               ||
    #                     |                                    ||
    #                data output                               \/
    #

    def __init__(self, loader):
        self.dataset = loader.dataset
        self.collate_fn = loader.collate_fn
        self.batch_sampler = loader.batch_sampler
        self.num_workers = loader.num_workers
        self.pin_memory = loader.pin_memory and torch.cuda.is_available()
        self.timeout = loader.timeout

        self.sample_iter = iter(self.batch_sampler)

        base_seed = torch.LongTensor(1).random_().item()

        if self.num_workers > 0:
            self.worker_init_fn = loader.worker_init_fn
            self.worker_queue_idx = 0
            self.worker_result_queue = multiprocessing.Queue()
            self.batches_outstanding = 0
            self.worker_pids_set = False
            self.shutdown = False
            self.send_idx = 0
            self.rcvd_idx = 0
            self.reorder_dict = {}
            self.done_event = multiprocessing.Event()

            self.index_queues = []
            self.workers = []
            for i in range(self.num_workers):
                index_queue = multiprocessing.Queue()
                index_queue.cancel_join_thread()
                w = multiprocessing.Process(
                    target=_utils.worker._worker_loop,
                    args=(self.dataset, index_queue,
                          self.worker_result_queue, self.done_event,
                          self.collate_fn, base_seed + i,
                          self.worker_init_fn, i))
                w.daemon = True
                # NB: Process.start() 
                w.start()
                self.index_queues.append(index_queue)
                self.workers.append(w)

            if self.pin_memory:
                self.data_queue = queue.Queue()
                pin_memory_thread = threading.Thread(
                    target=_utils.pin_memory._pin_memory_loop,
                    args=(self.worker_result_queue, self.data_queue,
                          torch.cuda.current_device(), self.done_event))
                pin_memory_thread.daemon = True
                pin_memory_thread.start()
                # Similar to workers (see comment above), we only register pin_memory_thread once it is started.
                self.pin_memory_thread = pin_memory_thread
            else:
                self.data_queue = self.worker_result_queue

            _utils.signal_handling._set_worker_pids(id(self), tuple(w.pid for w in self.workers))
            _utils.signal_handling._set_SIGCHLD_handler()
            self.worker_pids_set = True

            # prime the prefetch loop
            for _ in range(2 * self.num_workers):
                self._put_indices()

    def __len__(self):
        return len(self.batch_sampler)

    def _get_batch(self):
        if self.timeout > 0:
            try:
                return self.data_queue.get(timeout=self.timeout)
            except queue.Empty:
                raise RuntimeError('DataLoader timed out after {} seconds'.format(self.timeout))
        elif self.pin_memory:
            while self.pin_memory_thread.is_alive():
                try:
                    return self.data_queue.get(timeout=_utils.MP_STATUS_CHECK_INTERVAL)
                except queue.Empty:
                    continue
            else:
                # while condition is false, i.e., pin_memory_thread died.
                raise RuntimeError('Pin memory thread exited unexpectedly')
        else:
            return self.data_queue.get()

    def __next__(self):
        if self.num_workers == 0:  # same-process loading
            indices = next(self.sample_iter)  # may raise StopIteration
            batch = self.collate_fn([self.dataset[i] for i in indices])
            if self.pin_memory:
                batch = _utils.pin_memory.pin_memory_batch(batch)
            return batch

        # check if the next sample has already been generated
        if self.rcvd_idx in self.reorder_dict:
            batch = self.reorder_dict.pop(self.rcvd_idx)
            return self._process_next_batch(batch)

        if self.batches_outstanding == 0:
            self._shutdown_workers()
            raise StopIteration

        while True:
            assert (not self.shutdown and self.batches_outstanding > 0)
            idx, batch = self._get_batch()
            self.batches_outstanding -= 1
            if idx != self.rcvd_idx:
                # store out-of-order samples
                self.reorder_dict[idx] = batch
                continue
            return self._process_next_batch(batch)

    next = __next__  # Python 2 compatibility

    def __iter__(self):
        return self

    def _put_indices(self):
        assert self.batches_outstanding < 2 * self.num_workers
        indices = next(self.sample_iter, None)
        if indices is None:
            return
        self.index_queues[self.worker_queue_idx].put((self.send_idx, indices))
        self.worker_queue_idx = (self.worker_queue_idx + 1) % self.num_workers
        self.batches_outstanding += 1
        self.send_idx += 1

    def _process_next_batch(self, batch):
        self.rcvd_idx += 1
        self._put_indices()
        if isinstance(batch, _utils.ExceptionWrapper):
            raise batch.exc_type(batch.exc_msg)
        return batch

    def __getstate__(self):
		"""
		TODO:为HogWild添加有限的picking支持，以便跨多个线程共享迭代器。
			 最好的方法可能是将示例推送到单独的线程，然后只共享数据队列，
			 但如果没有非阻塞的API，则发送结束信号是很困难的。
		"""
        raise NotImplementedError("_DataLoaderIter cannot be pickled")

    def _shutdown_workers(self):
        # See NOTE [ Data Loader Multiprocessing Shutdown Logic ] for details on the logic of this function.
        python_exit_status = _utils.python_exit_status
        if python_exit_status is True or python_exit_status is None:
            # See (2) of the note. If Python is shutting down, do no-op.
            return
        # Normal exit when last reference is gone / iterator is depleted. See (1) and the second half of the note.
        if not self.shutdown:
            self.shutdown = True
            # Removes pids from the C side data structure first so worker termination afterwards won't trigger false positive error report.
            if self.worker_pids_set:
                _utils.signal_handling._remove_worker_pids(id(self))
                self.worker_pids_set = False

            self.done_event.set()

            # Exit `pin_memory_thread` first because exiting workers may leave
            # corrupted data in `worker_result_queue` which `pin_memory_thread` reads from.
            if hasattr(self, 'pin_memory_thread'):
                self.worker_result_queue.cancel_join_thread()
                self.worker_result_queue.put(None)
                self.pin_memory_thread.join()
                self.worker_result_queue.close()

            # Exit workers now.
            for q in self.index_queues:
                q.put(None)
                # Indicate that no more data will be put on this queue by the current process.
                q.close()
            for w in self.workers:
                w.join()

    def __del__(self):
        if self.num_workers > 0:
            self._shutdown_workers()
```











