# 开源库OpenNMT-py使用记录 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年12月21日 09:00:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：374








一、准备

官网：[http://opennmt.net/](http://opennmt.net/)

python+torch版Github：[https://github.com/OpenNMT/OpenNMT-py](https://github.com/OpenNMT/OpenNMT-py)

python+torch版说明：[http://zh.opennmt.net/OpenNMT-py/main.html](http://zh.opennmt.net/OpenNMT-py/main.html)

环境：ubuntu+python3+torch

需求：将图片公式转化为latex格式的半结构化字符串

案例：[http://zh.opennmt.net/OpenNMT-py/im2text.html](http://zh.opennmt.net/OpenNMT-py/im2text.html)

二、过程

1.Download the data.

```python
wget -O data/im2text.tgz http://lstm.seas.harvard.edu/latex/im2text_small.tgz; tar zxf data/im2text.tgz -C data/
```

2.Preprocess the data.

```python
python3 preprocess.py -data_type img -src_dir data/im2text/images/ -train_src data/im2text/src-train.txt -train_tgt data/im2text/tgt-train.txt -valid_src data/im2text/src-val.txt -valid_tgt data/im2text/tgt-val.txt -save_data data/im2text/demo -tgt_seq_length 150 -tgt_words_min_frequency 2 -shard_size 500 -image_channel_size 1
```

3.Train the model.

```python
nohup python3 train.py -model_type img -data data/im2text/demo -save_model demo-model -gpu_ranks 0 -batch_size 20 -max_grad_norm 20 -learning_rate 0.1 -word_vec_size 80 -encoder_type brnn -image_channel_size 1 &
```

4.Translate the images.
`python3 translate.py -data_type img -model demo-model_step_*.pt -src_dir data/im2text/images -src data/im2text/src-test.txt -output pred.txt -max_length 150 -beam_size 5 -gpu 0 -verbose -image_channel_size 1`
注意：-image_channel_size要设置，和前面两个步骤一致，否则会默认为3。

三、问题

问题1：

```
fjs@gpu1:~/OpenNMT-py$ python3 translate.py -data_type img -model demo-model_step_*.pt -src_dir data/im2text/images \
>                     -src data/im2text/src-test.txt -output pred.txt -max_length 150 -beam_size 5 -gpu 0 -verbose
Traceback (most recent call last):
  File "translate.py", line 33, in <module>
    main(opt)
  File "translate.py", line 14, in main
    translator = build_translator(opt, report_score=True)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 31, in build_translator
    fields, model, model_opt = load_test_model(opt, dummy_opt.__dict__)
  File "/home/fjs/OpenNMT-py/onmt/decoders/ensemble.py", line 135, in load_test_model
    'Ensemble models must use the same preprocessed data'
AssertionError: Ensemble models must use the same preprocessed data
```

处理：在onmt/decoders/ensemble.py中注释点assert，这个不知有何影响，暂无法明确。

![](https://img-blog.csdnimg.cn/20181221085656502.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

问题2：

```
fjs@gpu1:~/OpenNMT-py$ python3 translate.py -data_type img -model demo-model_step_*.pt -src_dir data/im2text/images                     -src data/im2text/src-test.txt -output pred.txt -max_length 150 -beam_size 5 -gpu 0 -verbose -image_channel_size 1
/usr/local/lib/python3.5/dist-packages/torchtext/data/field.py:359: UserWarning: To copy construct from a tensor, it is recommended to use sourceTensor.clone().detach() or sourceTensor.clone().detach().requires_grad_(True), rather than torch.tensor(sourceTensor).
  var = torch.tensor(arr, dtype=self.dtype, device=device)
Traceback (most recent call last):
  File "translate.py", line 33, in <module>
    main(opt)
  File "translate.py", line 19, in main
    attn_debug=opt.attn_debug)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 205, in translate
    batch, data, attn_debug, fast=self.fast
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 309, in translate_batch
    return self._translate_batch(batch, data)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 621, in _translate_batch
    batch, data_type)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 322, in _run_encoder
    'Ensemble decoding only supported for text data'
AssertionError: Ensemble decoding only supported for text data
```

 是onmt/translate/translator.py的提示，增加代码如下：

![](https://img-blog.csdnimg.cn/20181221112248264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

问题3：

```
fjs@gpu1:~/OpenNMT-py$ python3 translate.py -data_type img -model demo-model_step_*.pt -src_dir data/im2text/images  -src data/im2text/src-test.txt -output pred.txt -max_length 150 -beam_size 5 -gpu 0 -verbose -image_channel_size 1
/usr/local/lib/python3.5/dist-packages/torchtext/data/field.py:359: UserWarning: To copy construct from a tensor, it is recommended to use sourceTensor.clone().detach() or sourceTensor.clone().detach().requires_grad_(True), rather than torch.tensor(sourceTensor).
  var = torch.tensor(arr, dtype=self.dtype, device=device)
Traceback (most recent call last):
  File "translate.py", line 33, in <module>
    main(opt)
  File "translate.py", line 19, in main
    attn_debug=opt.attn_debug)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 205, in translate
    batch, data, attn_debug, fast=self.fast
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 309, in translate_batch
    return self._translate_batch(batch, data)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 646, in _translate_batch
    memory_bank = tuple(tile(x, beam_size, dim=1) for x in memory_bank)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 646, in <genexpr>
    memory_bank = tuple(tile(x, beam_size, dim=1) for x in memory_bank)
  File "/home/fjs/OpenNMT-py/onmt/utils/misc.py", line 42, in tile
    .transpose(0, 1) \
RuntimeError: CUDA out of memory. Tried to allocate 435.00 MiB (GPU 0; 10.92 GiB total capacity; 9.64 GiB already allocated; 355.50 MiB free; 376.90 MiB cached)
```

 调整执行参数：-beam_size 3

问题4：

```
fjs@gpu1:~/OpenNMT-py$ python3 translate.py -data_type img -model demo-model_step_*.pt -src_dir data/im2text/images  -src data/im2text/src-test.txt -output pred.txt -max_length 150 -beam_size 3 -gpu 0 -verbose -image_channel_size 1
/usr/local/lib/python3.5/dist-packages/torchtext/data/field.py:359: UserWarning: To copy construct from a tensor, it is recommended to use sourceTensor.clone().detach() or sourceTensor.clone().detach().requires_grad_(True), rather than torch.tensor(sourceTensor).
  var = torch.tensor(arr, dtype=self.dtype, device=device)
Traceback (most recent call last):
  File "translate.py", line 33, in <module>
    main(opt)
  File "translate.py", line 19, in main
    attn_debug=opt.attn_debug)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 205, in translate
    batch, data, attn_debug, fast=self.fast
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 309, in translate_batch
    return self._translate_batch(batch, data)
  File "/home/fjs/OpenNMT-py/onmt/translate/translator.py", line 649, in _translate_batch
    memory_lengths = tile(src_lengths, beam_size)
  File "/home/fjs/OpenNMT-py/onmt/utils/misc.py", line 32, in tile
    perm = list(range(len(x.size())))
AttributeError: 'int' object has no attribute 'size'
```

未解决？



