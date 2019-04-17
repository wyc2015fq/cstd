# rbgirshick/py-faster-rcnn训练过程中xml读取的过程 - Snoopy_Dream - CSDN博客





2018年04月17日 22:14:01[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：200








imdb表示image database

ind表示index




在tools\train_net.py中

```python
from datasets.factory import get_imdb #与下文相关的头文件
```

```python
def combined_roidb(imdb_names):
    def get_roidb(imdb_name):
        imdb = get_imdb(imdb_name)# 调用该函数，返回imdb  
        print 'Loaded dataset `{:s}` for training'.format(imdb.name)
        imdb.set_proposal_method(cfg.TRAIN.PROPOSAL_METHOD)#设置方法一般默认gt
        print 'Set proposal method: {:s}'.format(cfg.TRAIN.PROPOSAL_METHOD)
        roidb = get_training_roidb(imdb)#利用imdb，产生roi_db  

        return roidb

    roidbs = [get_roidb(s) for s in imdb_names.split('+')]
```

```python

```
  roidb = roidbs[0]
  if len(roidbs) > 1:
      for r in roidbs[1:]:
          roidb.extend(r)
      imdb = datasets.imdb.imdb(imdb_names)
  else:
      imdb = get_imdb(imdb_names)
  return imdb, roidb
```python

```
if __name__ == '__main__':...          roidb, imdb = get_roidb(imdb_name) # 调用函数，返回训练数据  
```python
model_paths = train_net(solver, roidb, output_dir, #传入数据roidb，供训练  
                          pretrained_model=init_model,  
                          max_iters=max_iters)
```
所以，进入get_roidb函数：
```python
def get_roidb(imdb_name, rpn_file=None):  
    imdb = get_imdb(imdb_name) # 调用该函数，返回imdb  
    print 'Loaded dataset `{:s}` for training'.format(imdb.name)  
    imdb.set_proposal_method(cfg.TRAIN.PROPOSAL_METHOD)  
    print 'Set proposal method: {:s}'.format(cfg.TRAIN.PROPOSAL_METHOD)  
    if rpn_file is not None:  
        imdb.config['rpn_file'] = rpn_file  
    roidb = get_training_roidb(imdb) 
    return roidb, imdb
```




