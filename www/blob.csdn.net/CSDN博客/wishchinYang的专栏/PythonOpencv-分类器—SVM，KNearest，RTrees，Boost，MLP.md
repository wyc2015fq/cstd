# PythonOpencv-分类器—SVM，KNearest，RTrees，Boost，MLP - wishchinYang的专栏 - CSDN博客
2014年08月11日 15:54:20[wishchin](https://me.csdn.net/wishchin)阅读数：1055
个人分类：[MLandPy																[判别分析																[资源整理](https://blog.csdn.net/wishchin/article/category/6044747)](https://blog.csdn.net/wishchin/article/category/3126941)](https://blog.csdn.net/wishchin/article/category/1508471)
原文链接：[http://blog.csdn.net/gjy095/article/details/9243153](http://blog.csdn.net/gjy095/article/details/9243153)
上一篇文章，不是很详细，这一篇解释的清晰些，请访问原始链接。
Rtrees介绍！参考链接：[http://docs.opencv.org/modules/ml/doc/random_trees.html]()
Opencv提供了几种分类器，例程里通过字符识别来进行说明的
1、支持向量机（SVM）：给定训练样本，支持向量机建立一个超平面作为决策平面，使得正例和反例之间的隔离边缘被最大化。
函数原型：训练原型 cv2.SVM.train(trainData, responses[, varIdx[, sampleIdx[, params]]])
                   其中 trainData 为训练数据，responses为对应数据的标识，
2、K近邻（Knearest）：K近邻是移动惰性学习法，当给定大量数据集时，该算法是计算密集的。最近邻方法是基于类比学习，即通过将给定的检验元组与和它相似的训练元组进行比较来学习。训练元组用n个属性来表示。当给定位置元组时，K近邻找出最接近未知元组的k个训练元组，未知元组被分配到k个最近邻中最多的类。
函数原型：cv2.KNearest.train(trainData, responses[, sampleIdx[, isRegression[, maxK[, updateBase]]]])
                   其中，trainData为训练数据，responses为对应的数据标识，isRegression表示回归运算还是训练，maxK为最大邻居数
3、随机树（RTrees）：个体决策树的每个节点使用随机选择属性决定划分，每一棵树依赖于独立的抽样，并与森林中所有的树具有相同的分布的随即向量的值。分类时，每棵树都投票并且返回得票最多的类。
函数原型：cv2.RTrees.train(trainData, tflag, responses[, varIdx[, sampleIdx[, varType[,
 missingDataMask[, params]]]]])
                   其中trainData为训练数据，responses为对应的数据标识，tflag表示特征向量是行还是列表示，responses为表示对应数据标识
4、提升（Boost）：权重赋予每个训练元组。迭代的学习k个分类器，学习到分类器Mi后，更新权重，使得其后的分类器Mi+1更关注误分类的训练元组。Adaboost是一种流行的提升算法。给定数据集D，它包含d个类标记的元组。开始对每个训练元组赋予相等的权重1/d。为组合分类器产生k个基分类器。在第i轮，从D中元组进行抽样，形成大小为d的训练集Di。使用有放回抽样--同一个元组可能被选中多次。每个元组被选中的机会由它的权重决定。从训练集Di导出分类器Mi。然后使用Di作为检验集计算Mi的误差。如果元组不正确的分类，则它的权重增加。如果元组正确的分类，则它的权重减少。权重越高越可能错误地分类。使用这些权重为下一轮分类器产生训练样本。
函数原型：cv2.Boost.train(trainData, tflag, responses[, varIdx[, sampleIdx[, varType[, missingDataMask[,
 params[, update]]]]]])
5、多层感知（MLP）：多层感知器用于解决单层神经网络不能解决非线性分类问题而提出的，训练多层感知器的流行方法是反向传播，通过多层感知能够通过多个输入产生单一的输出达到分类的结果。
函数原型：cv2.ANN_MLP.train(inputs, outputs, sampleWeights[, sampleIdx[, params[, flags]]])
代码函数原型：
```python
#decoding:utf-8  
import numpy as np  
import cv2  
  
def load_base(fn):  
    a = np.loadtxt(fn, np.float32, delimiter=',', converters={ 0 : lambda ch : ord(ch)-ord('A') })#导入的字母特征数据，并将字母转化为数字类别  
    samples, responses = a[:,1:], a[:,0]#将类别给responses，特征给samples  
    return samples, responses  
  
class LetterStatModel(object):  
    class_n = 26  
    train_ratio = 0.5  
  
    def load(self, fn):  
        self.model.load(fn)  
    def save(self, fn):  
        self.model.save(fn)  
      
    def unroll_samples(self, samples):  
        sample_n, var_n = samples.shape#获取特征维数和特征个数  
        new_samples = np.zeros((sample_n * self.class_n, var_n+1), np.float32)  
        new_samples[:,:-1] = np.repeat(samples, self.class_n, axis=0)  
        new_samples[:,-1] = np.tile(np.arange(self.class_n), sample_n)  
        return new_samples  
      
    def unroll_responses(self, responses):  
        sample_n = len(responses)  
        new_responses = np.zeros(sample_n*self.class_n, np.int32)  
        resp_idx = np.int32( responses + np.arange(sample_n)*self.class_n )  
        new_responses[resp_idx] = 1  
        return new_responses  
  
class RTrees(LetterStatModel):  
    def __init__(self):  
        self.model = cv2.RTrees()  
  
    def train(self, samples, responses):  
        sample_n, var_n = samples.shape  
        var_types = np.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL], np.uint8)  
        #CvRTParams(10,10,0,false,15,0,true,4,100,0.01f,CV_TERMCRIT_ITER));  
        params = dict(max_depth=10 )  
        self.model.train(samples, cv2.CV_ROW_SAMPLE, responses, varType = var_types, params = params)  
  
    def predict(self, samples):  
        return np.float32( [self.model.predict(s) for s in samples] )  
          
  
class KNearest(LetterStatModel):  
    def __init__(self):  
        self.model = cv2.KNearest()  
  
    def train(self, samples, responses):  
        self.model.train(samples, responses)  
  
    def predict(self, samples):  
        retval, results, neigh_resp, dists = self.model.find_nearest(samples, k = 10)  
        return results.ravel()  
  
  
class Boost(LetterStatModel):  
    def __init__(self):  
        self.model = cv2.Boost()  
      
    def train(self, samples, responses):  
        sample_n, var_n = samples.shape  
        new_samples = self.unroll_samples(samples)  
        new_responses = self.unroll_responses(responses)  
        var_types = np.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL, cv2.CV_VAR_CATEGORICAL], np.uint8)  
        #CvBoostParams(CvBoost::REAL, 100, 0.95, 5, false, 0 )  
        params = dict(max_depth=5) #, use_surrogates=False)  
        self.model.train(new_samples, cv2.CV_ROW_SAMPLE, new_responses, varType = var_types, params=params)  
  
    def predict(self, samples):  
        new_samples = self.unroll_samples(samples)  
        pred = np.array( [self.model.predict(s, returnSum = True) for s in new_samples] )  
        pred = pred.reshape(-1, self.class_n).argmax(1)  
        return pred  
  
  
class SVM(LetterStatModel):  
    train_ratio = 0.1  
    def __init__(self):  
        self.model = cv2.SVM()  
  
    def train(self, samples, responses):  
        params = dict( kernel_type = cv2.SVM_LINEAR,   
                       svm_type = cv2.SVM_C_SVC,  
                       C = 1 )  
        self.model.train(samples, responses, params = params)  
  
    def predict(self, samples):  
        return np.float32( [self.model.predict(s) for s in samples] )  
  
  
class MLP(LetterStatModel):  
    def __init__(self):  
        self.model = cv2.ANN_MLP()  
  
    def train(self, samples, responses):  
        sample_n, var_n = samples.shape  
        new_responses = self.unroll_responses(responses).reshape(-1, self.class_n)  
  
        layer_sizes = np.int32([var_n, 100, 100, self.class_n])  
        self.model.create(layer_sizes)  
          
        # CvANN_MLP_TrainParams::BACKPROP,0.001  
        params = dict( term_crit = (cv2.TERM_CRITERIA_COUNT, 300, 0.01),  
                       train_method = cv2.ANN_MLP_TRAIN_PARAMS_BACKPROP,   
                       bp_dw_scale = 0.001,  
                       bp_moment_scale = 0.0 )  
        self.model.train(samples, np.float32(new_responses), None, params = params)  
  
    def predict(self, samples):  
        ret, resp = self.model.predict(samples)  
        return resp.argmax(-1)  
  
  
if __name__ == '__main__':  
    import getopt  
    import sys  
  
    models = [RTrees, KNearest, Boost, SVM, MLP] # NBayes  
    models = dict( [(cls.__name__.lower(), cls) for cls in models] )#将名字之母字母转为小写  
  
    print 'USAGE: letter_recog.py [--model <model>] [--data <data fn>] [--load <model fn>] [--save <model fn>]'  
    print 'Models: ', ', '.join(models)  
    print  
      
    args, dummy = getopt.getopt(sys.argv[1:], '', ['model=', 'data=', 'load=', 'save='])  
    args = dict(args)  
    args.setdefault('--model', 'boost')  
    args.setdefault('--data', '../letter-recognition.data')  
  
    print 'loading data %s ...' % args['--data']  
    samples, responses = load_base(args['--data'])  
    Model = models[args['--model']]  
    model = Model()  
  
    train_n = int(len(samples)*model.train_ratio)#获取训练数据的数目  
    if '--load' in args:  
        fn = args['--load']  
        print 'loading model from %s ...' % fn  
        model.load(fn)  
    else:  
        print 'training %s ...' % Model.__name__  
        model.train(samples[:train_n], responses[:train_n])  
  
    print 'testing...'  
    train_rate = np.mean(model.predict(samples[:train_n]) == responses[:train_n])#前一半进行训练，并得到训练准确率  
    test_rate  = np.mean(model.predict(samples[train_n:]) == responses[train_n:])#后一半进行测试，并得到测试准确率  
  
    print 'train rate: %f  test rate: %f' % (train_rate*100, test_rate*100)  
  
    if '--save' in args:  
        fn = args['--save']  
        print 'saving model to %s ...' % fn  
        model.save(fn)  
    cv2.destroyAllWindows()
```
