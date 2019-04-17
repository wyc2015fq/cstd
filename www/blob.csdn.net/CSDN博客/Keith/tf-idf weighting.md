# tf-idf weighting - Keith - CSDN博客





2016年12月01日 13:37:33[ke1th](https://me.csdn.net/u012436149)阅读数：957








水平有限，如有错误请指正！

# tf-idf weighting

## tf(term frequency)

`a document or zone that mentions a query term more often has more to do with that query and therefore should receive a higher score`
`query term`: a set of words 
$N$:document中term的总数 
$t$:document中term出现的次数 


$tf_t=\frac{t}{N}$
## idf(inverse document frequency)

$N$: document的数量 
$df_t$:有多少documents 包含$t$


$idf_t=log\frac{N}{df_t}$

从公式可以看出：- $df_t$ 越小，$idf_t$越大,表明$t$对文档的区分性更大
- 反之

## tf-idf weighting



$\text{tf-idf}_{t,d}=tf_{t,d}* idf_t$

参考： 
[tf](http://nlp.stanford.edu/IR-book/html/htmledition/term-frequency-and-weighting-1.html)
[idf](http://nlp.stanford.edu/IR-book/html/htmledition/inverse-document-frequency-1.html)
[tf-idf](http://nlp.stanford.edu/IR-book/html/htmledition/tf-idf-weighting-1.html)





