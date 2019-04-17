# 【Multiprocessing】采用多进程计算处理数据 - CD's Coding - CSDN博客





2018年06月28日 21:23:32[糖果天王](https://me.csdn.net/okcd00)阅读数：113








## 0x00 前言

在数据处理方面，通常会将较为简单的源数据存于本地磁盘中， 

每次使用时读取，经过生成函数生成模型可以直接使用的训练数据， 

而这种数据通常为矩阵的形式，考虑到padding位的情况下通常较大， 

那么，在源数据的量较大，而生成的数据的空间占用增量巨大无法存于本地， 

就需要采用生成一个，传输一个，消耗一个，之后再生成的形式。
线性计算终归是缓慢的，此时就需要多进程进行处理， 

如下所示，基本框架用以记录多进程计算的基本方法： 

后续有空的时候再继续记录一下基于此的、时空复杂度均更优的 “生产者—消费者” 模型。
## 0x01 Source Code

```python
ori_data = range(65536)

def get_args(i):
    batch_size = 5 * 200
    return ori_data[i * batch_size: (i+1) * batch_size], i, 'container_{}'.format(i)

def batch_data_generate_fn(inp, i, name):
    print('Run child process %s (%s)' % (name, os.getpid()))
    # do some calculation etc.
    return inp

def generate_with_multiprocessing(method='default', fn=None):
    import multiprocessing
    targets = {  # pre-defined functions
        # 'segments': generate_original_segments_single_task,
        # 'single': batch_data_generate_single_block,
        # 'batch': batch_data_generate_single_task,
        'default': batch_data_generate_fn,
    }
    print('Run the main process (%s).' % (os.getpid()))

    i = 0
    n_cores = 9
    for i in range(n_cores):
        p = multiprocessing.Process(
            target=targets.get(method),
            args=fn(i) if fn else (i,))
        p.start()
    print('Waiting for all subprocesses done ...')

generate_with_multiprocessing(method='default', fn=get_args)
```

## 0x02 实际场景运用

```python
def generate_correction_input_from_path_case(path_case, name_prefix=0):
    ret, file_idx = [], 0
    print('Run the child process<{}> {}.'.format(name_prefix, os.getpid()))
    for path in path_case:
        if os.path.exists(path):
            data = load301(0, path)
            sents = sentence_dict_to_correction_input(data.get('Sentence', []), False)
            ret.extend(sents)
        name = path.split('/')[-1] if path.split('/')[-1] != 'output.o301' else path.split('/')[-2]
        print("Now solving {:>6s} | cache {:>05} sentences for file-{:>02}{:>04}".format(
            name.replace('.o301', ''), ret.__len__(), name_prefix, file_idx))
        while ret.__len__() > 10000:
            pickle.dump(
                ret[:10000],
                open('/data/chend/sentence_words_180901/{:>02}{:>04}.pkl'.format(
                    name_prefix, file_idx), 'w'))
            file_idx += 1
            del ret[:10000]
    else:
        pickle.dump(
            ret,
            open('/data/chend/sentence_words_180901/{:>02}{:>04}.pkl'.format(
                name_prefix, file_idx), 'w'))
        del ret[:]



def generate_correction_input_from_o301():
    import multiprocessing
    print('Run the main process {}.'.format(os.getpid()))

    n_cores = 25
    bad_docs = [line for line in open('/data/bad_quality.txt', 'rb')]
    dir_name = '/data/pdf_bond_disclosure_nafmii/'
    online_doc_case, total_doc_case = [], []

    for idx in range(364, 4467):
        doc = str(idx)
        if doc in bad_docs:
            continue
        online_doc_case.append("/data/fake_path/{}/output.o301".format(doc))

    for _, _, files in os.walk(dir_name):
        for file_name in files:
            if file_name.endswith('o301'):
                total_doc_case.append(dir_name + file_name)

    path_case = sorted(online_doc_case + total_doc_case)
    block_size = int(path_case.__len__() / n_cores)
    for core in range(n_cores):
        args = (path_case[block_size*core: block_size*core+block_size], core)
        p = multiprocessing.Process(
            target=generate_correction_input_from_path_case,
            args=args
        )
        p.start()

    print('Waiting for all subprocesses done ...')
```





