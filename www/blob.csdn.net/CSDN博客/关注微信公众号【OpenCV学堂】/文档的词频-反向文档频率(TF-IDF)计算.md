# 文档的词频-反向文档频率(TF-IDF)计算 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年05月04日 14:50:45[gloomyfish](https://me.csdn.net/jia20003)阅读数：11215
个人分类：[数据挖据学习](https://blog.csdn.net/jia20003/article/category/1408383)









TF-IDF计算：

TF-IDF反映了在文档集合中一个单词对一个文档的重要性，经常在文本数据挖据与信息

提取中用来作为权重因子。在一份给定的文件里，**词频**(termfrequency-TF)指的是某一

个给定的词语在该文件中出现的频率。**逆向文件频率**（inversedocument frequency，

IDF）是一个词语普遍重要性的度量。某一特定词语的IDF，可以由总文件数目除以包含

该词语之文件的数目，再将得到的商取[对数](https://zh.wikipedia.org/wiki/%E5%B0%8D%E6%95%B8)得到。

相关代码：



```java
private static Pattern r = Pattern.compile("([ \\t{}()\",:;. \n])"); 
	private static List<String> documentCollection;

    //Calculates TF-IDF weight for each term t in document d
    private static float findTFIDF(String document, String term)
    {
        float tf = findTermFrequency(document, term);
        float idf = findInverseDocumentFrequency(term);
        return tf * idf;
    }

    private static float findTermFrequency(String document, String term)
    {
    	int count = getFrequencyInOneDoc(document, term);

        return (float)((float)count / (float)(r.split(document).length));
    }
    
    private static int getFrequencyInOneDoc(String document, String term)
    {
    	int count = 0;
        for(String s : r.split(document))
        {
        	if(s.toUpperCase().equals(term.toUpperCase())) {
        		count++;
        	}
        }
        return count;
    }


    private static float findInverseDocumentFrequency(String term)
    {
        //find the  no. of document that contains the term in whole document collection
        int count = 0;
        for(String doc : documentCollection)
        {
        	count += getFrequencyInOneDoc(doc, term);
        }
        /*
         * log of the ratio of  total no of document in the collection to the no. of document containing the term
         * we can also use Math.Log(count/(1+documentCollection.Count)) to deal with divide by zero case; 
         */
        return (float)Math.log((float)documentCollection.size() / (float)count);

    }
```
建立文档的向量空间模型Vector Space Model并计算余弦相似度。


相关代码：



```java
public static float findCosineSimilarity(float[] vecA, float[] vecB)
{
    float dotProduct = dotProduct(vecA, vecB);
    float magnitudeOfA = magnitude(vecA);
    float magnitudeOfB = magnitude(vecB);
    float result = dotProduct / (magnitudeOfA * magnitudeOfB);
    //when 0 is divided by 0 it shows result NaN so return 0 in such case.
    if (Float.isNaN(result))
        return 0;
    else
        return (float)result;
}

public static float dotProduct(float[] vecA, float[] vecB)
{

    float dotProduct = 0;
    for (int i = 0; i < vecA.length; i++)
    {
        dotProduct += (vecA[i] * vecB[i]);
    }

    return dotProduct;
}

// Magnitude of the vector is the square root of the dot product of the vector with itself.
public static float magnitude(float[] vector)
{
    return (float)Math.sqrt(dotProduct(vector, vector));
}
```
**注意点**：

零词过滤(stop-words filter)

**零词列表**

[ftp://ftp.cs.cornell.edu/pub/smart/english.stop](ftp://ftp.cs.cornell.edu/pub/smart/english.stop)

**关于TF-IDF参考这里：**

链接–> [http://en.wikipedia.org/wiki/Tf*idf](http://en.wikipedia.org/wiki/Tf*idf)




