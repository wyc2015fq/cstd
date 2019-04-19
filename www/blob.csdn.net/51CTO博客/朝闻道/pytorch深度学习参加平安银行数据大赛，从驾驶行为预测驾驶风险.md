# pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险-朝闻道-51CTO博客
比赛链接http://www.datafountain.cn/#/competitions/284/intro
本赛题提供部分客户1分钟级驾驶行为数据及对应客户的赔付率作为训练集，包括经纬度定位及驾驶状态等（已脱敏），参赛队伍需要对其进行数据挖掘和必要的机器学习训练。另外，我们会提供同期其他部分客户的驾驶行为数据来做评测，检测您的算法是否能准确的识别出当时客户的驾驶风险。
与以往比赛不同的是, 由于数据安全的问题本次比赛数据训练集跟评测集不对外公开, 参赛选手提交的不在是数据评测结果而是平台可执行的Python代码. 具体提交要求请参见[作品要求]. 为方便选手更好的理解数据,企业将会提供部分样例数据.
比赛内容我不再重复，简单来说就是从司机的驾驶行为数据预测保险赔付率。
数据示例
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/78c806b6dd0c244495bf8135cde20a44.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
其中第一列为用户的id 下载数据集合后该示例数据有一百个用户 
trip_id为用户行程id
后面两列为经纬度 当时行驶的海拔 速度 电话是否能接通 时间
Y值为赔付比例
读取数据
采用pandas读取数据
清洗数据，对于该数据表中 速度方向都不正常的值予以清除
特征工程
可以猜想一下什么样开车的人容易出事故，哪些路段容易出事故
对此我们做了一个前端显示效果对比 Y =0 和 Y>0的行驶轨迹
先将数据放入数据库，数据库表结构如下，该数据库主要为了取数据方便用，不必详细考虑数据类型：
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/6d49e706a66778afcd784ace7d9debf1.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
最后一列是为了将表格中的时间转化为常用的时间，原来的时间为自从1970年1月1号开始的时间
我们从数据库中调出不同的行驶轨迹：
这是Y=0的行驶轨迹：
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/69374fc2acd82a81d653d3df4d18c87d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
这是Y>0的行驶轨迹：
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/b0f24d03a420478b4714fb17b7f3ab3a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
如果将这两个图在百度地图上详细对比发现路段和赔付比例没有必然联系，暂时我们就不把经纬度作为一个特征。
驾驶数据的时间分布：
我们可以假设一个人喜欢晚上开车会比较容易出事故。
所以抽取他的定位数据在24个小时的分布作为特征。
假设有人喜欢急刹，擅长大转弯也容易出事
所以将速度的一阶差分，和速度均值，方向的一阶差分作为特征
抽取特征的程序如下：
```
data = pd.read_csv(path_train)
train1 = []
label1 = []#分类标签
label2 = []#回归标签
alluser = data['TERMINALNO'].nunique()
# Feature Engineer, 对每一个用户生成特征:
#计算特征：时间24小时分布 速度方差 速度均值 电话状态分布
for item in data['TERMINALNO'].unique():
    temp = data.loc[data['TERMINALNO'] == item,:]
    temp.index = range(len(temp))
    # trip 特征
    num_of_trips = temp['TRIP_ID'].nunique()
    # record 特征
    num_of_records = temp.shape[0]
    num_of_state = temp[['TERMINALNO','CALLSTATE']]
    nsh = num_of_state.shape[0]
    num_of_state_0 = num_of_state.loc[num_of_state['CALLSTATE']==0].shape[0]/float(nsh)
    num_of_state_1 = num_of_state.loc[num_of_state['CALLSTATE']==1].shape[0]/float(nsh)
    num_of_state_2 = num_of_state.loc[num_of_state['CALLSTATE']==2].shape[0]/float(nsh)
    num_of_state_3 = num_of_state.loc[num_of_state['CALLSTATE']==3].shape[0]/float(nsh)
    num_of_state_4 = num_of_state.loc[num_of_state['CALLSTATE']==4].shape[0]/float(nsh)
    del num_of_state
    # 时间特征
    # temp['weekday'] = temp['TIME'].apply(lambda x:datetime.datetime.fromtimestamp(x).weekday())
    temp['hour'] = temp['TIME'].apply(lambda x:datetime.datetime.fromtimestamp(x).hour)
    hour_state = np.zeros([24,1])
    for i in range(24):
        hour_state[i] = temp.loc[temp['hour']==i].shape[0]/float(nsh)
    # 驾驶行为特征
    mean_speed = temp['SPEED'].mean()#速度均值
    std_speed = temp['SPEED'].std()#速度标准差
    mean_height = temp['HEIGHT'].mean()#海拔均值
    std_height = temp['HEIGHT'].std()#海拔标准差
    diffmean_direction = temp['DIRECTION'].diff().mean()#方向一阶差分均值
    # 添加label
    target = temp.loc[0, 'Y']
    # 所有特征
    feature = [num_of_trips,num_of_records,num_of_state_0,num_of_state_1,num_of_state_2,num_of_state_3,num_of_state_4,\
               mean_speed,std_speed,mean_height,std_height,diffmean_direction\
            ,float(hour_state[0]),float(hour_state[1]),float(hour_state[2]),float(hour_state[3]),float(hour_state[4]),float(hour_state[5])
            ,float(hour_state[6]),float(hour_state[7]),float(hour_state[8]),float(hour_state[9]),float(hour_state[10]),float(hour_state[11])
            ,float(hour_state[12]),float(hour_state[13]),float(hour_state[14]),float(hour_state[15]),float(hour_state[16]),float(hour_state[17])
            ,float(hour_state[18]),float(hour_state[19]),float(hour_state[20]),float(hour_state[21]),float(hour_state[22]),float(hour_state[23])]
    train1.append(feature)
    label2.append([target])
```
读取了数据之后做一个分类预测，将Y值分为三类，分别是=0 小于均值 和 其他
打标签程序如下：
```
mean_y = np.mean(np.array(label2))
for label in label2:
    if label[0] == 0:
        label1.append([0])
    elif label[0] <= mean_y:
        label1.append([1])
    else:
        label1.append([2])
```
然后构建神经网络：
通过我对参数的调整，以下神经网络可以较好拟合数据，所以不再加深和加宽（增加神经元个数）：
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/f5cfbf77d41cf9d218465a3e0fa188c7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
训练代码如下：
```
import torch
from torch.autograd import Variable
import torch.nn.functional as F
#数据类型转化为tensor
x = torch.Tensor(train1)
y = torch.Tensor(label1)
y = torch.cat((y), ).type(torch.LongTensor)    # shape (100,) LongTensor = 64-bit integer
x, y = Variable(x), Variable(y)
#构造分类模型
net1 = torch.nn.Sequential(
    torch.nn.Linear(36, 13),
    torch.nn.ReLU(),
    torch.nn.Linear(13, 13),
    torch.nn.ReLU(),
    torch.nn.Linear(13, 3)
)
print(net1)  # 输出网络模型
optimizer = torch.optim.SGD(net1.parameters(), lr=0.002)
loss_func = torch.nn.CrossEntropyLoss()
#训练分类模型
len_data = len(label1)
for t in range(100000):
    out = net1(x)                 # input x and predict based on x
    loss = loss_func(out, y)     # must be (1. nn output, 2. target), the target label is NOT one-hotted
    optimizer.zero_grad()   # clear gradients for next train
    loss.backward()         # backpropagation, compute gradients
    optimizer.step()        # apply gradients
    if t % 100 == 0:
        print("第" + str(t) + "次训练")
        prediction = torch.max(out, 1)[1]
        pred_y = prediction.data.numpy().squeeze()
        target_y = y.data.numpy()
        accuracy = sum(pred_y == target_y)/(len_data+0.0)
        print('Accuracy=%.2f' % accuracy)
        if accuracy>0.95:
            # 保存模型
            torch.save(net1, 'model/net1.pkl')
            print("保存分类模型")
            break
    if t ==99999:
        torch.save(net1, 'model/net1.pkl')
        print("保存分类模型")
```
然后对所有Y>0的数据做回归，回归模型如下：
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/f2633f79738ef7b764d30c10fdff97b6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
训练代码如下：
```
#读取数据
data = pd.read_csv(path_train)
train1 = []#回归训练数据集
label2 = []#回归标签
alluser = data['TERMINALNO'].nunique()
# Feature Engineer, 对每一个用户生成特征:
#计算特征：时间24小时分布 速度方差 速度均值 电话状态分布
for item in data['TERMINALNO'].unique():
    temp = data.loc[data['TERMINALNO'] == item,:]
    temp.index = range(len(temp))
    # trip 特征
    num_of_trips = temp['TRIP_ID'].nunique()
    # record 特征
    num_of_records = temp.shape[0]
    num_of_state = temp[['TERMINALNO','CALLSTATE']]
    nsh = num_of_state.shape[0]
    num_of_state_0 = num_of_state.loc[num_of_state['CALLSTATE']==0].shape[0]/float(nsh)
    num_of_state_1 = num_of_state.loc[num_of_state['CALLSTATE']==1].shape[0]/float(nsh)
    num_of_state_2 = num_of_state.loc[num_of_state['CALLSTATE']==2].shape[0]/float(nsh)
    num_of_state_3 = num_of_state.loc[num_of_state['CALLSTATE']==3].shape[0]/float(nsh)
    num_of_state_4 = num_of_state.loc[num_of_state['CALLSTATE']==4].shape[0]/float(nsh)
    del num_of_state
    # 时间特征
    # temp['weekday'] = temp['TIME'].apply(lambda x:datetime.datetime.fromtimestamp(x).weekday())
    temp['hour'] = temp['TIME'].apply(lambda x:datetime.datetime.fromtimestamp(x).hour)
    hour_state = np.zeros([24,1])
    for i in range(24):
        hour_state[i] = temp.loc[temp['hour']==i].shape[0]/float(nsh)
    # 驾驶行为特征
    mean_speed = temp['SPEED'].mean()#速度均值
    std_speed = temp['SPEED'].std()#速度标准差
    mean_height = temp['HEIGHT'].mean()#海拔均值
    std_height = temp['HEIGHT'].std()#海拔标准差
    diffmean_direction = temp['DIRECTION'].diff().mean()#方向一阶差分均值
    # 添加label
    target = temp.loc[0, 'Y']
    # 所有特征
    feature = [num_of_trips,num_of_records,num_of_state_0,num_of_state_1,num_of_state_2,num_of_state_3,num_of_state_4,\
               mean_speed,std_speed,mean_height,std_height,diffmean_direction\
            ,float(hour_state[0]),float(hour_state[1]),float(hour_state[2]),float(hour_state[3]),float(hour_state[4]),float(hour_state[5])
            ,float(hour_state[6]),float(hour_state[7]),float(hour_state[8]),float(hour_state[9]),float(hour_state[10]),float(hour_state[11])
            ,float(hour_state[12]),float(hour_state[13]),float(hour_state[14]),float(hour_state[15]),float(hour_state[16]),float(hour_state[17])
            ,float(hour_state[18]),float(hour_state[19]),float(hour_state[20]),float(hour_state[21]),float(hour_state[22]),float(hour_state[23])]
    if target>0:#只用大于零的做回归
        train1.append(feature)
        label2.append([target])
#归一化
train1 = AutoNorm(train1)
import torch
from torch.autograd import Variable
#构造回归模型
net2 = torch.nn.Sequential(
    torch.nn.Linear(36, 4),
    torch.nn.ReLU(),
    torch.nn.Linear(4, 4),
    torch.nn.ReLU(),
    torch.nn.Linear(4,1)
    )
print(net2)  # 输出网络模型
#训练回归模型
x = torch.Tensor(train1)
y = torch.Tensor(label2)
y = torch.cat((y), ).type(torch.FloatTensor)    # shape (100,) LongTensor = 64-bit integer
y = torch.unsqueeze(y, 1)
x, y = Variable(x), Variable(y)
optimizer = torch.optim.SGD(net2.parameters(),lr=0.02)
loss_func = torch.nn.MSELoss()
for t in range(100000):
    prediction = net2(x)
    loss = loss_func(prediction,y)
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()
    if t % 10 ==0:
        print("第" + str(t) + "次训练")
        print(loss.data[0])
        #如果误差小于%5即可停止训练保存模型
        if loss.data[0]/mean_y<0.05:
            torch.save(net2, 'model/net2.pkl')
            print("保存分类模型")
            break
    if t ==99999:
        torch.save(net2, 'model/net2.pkl')
        print("保存分类模型")
```
这里有两个要注意的地方，在数据输入神经网络之前，为了避免不同的数据类型即特征之间量纲不一样导致系统失灵的问题，要对所有的特征数据做归一化：
归一化代码如下：
```
#归一化
def width(lst):
    i = 0
    for j in lst[0]:
        i = i + 1
    return i
def AutoNorm(mat):
    n = len(mat)
    m = width(mat)
    MinNum = [9999999999] * m
    MaxNum = [0] * m
    for i in mat:
        for j in range(0, m):
            if i[j] > MaxNum[j]:
                MaxNum[j] = i[j]
    for p in mat:
        for q in range(0, m):
            if p[q] <= MinNum[q]:
                MinNum[q] = p[q]
    section = list(map(lambda x: x[0] - x[1], zip(MaxNum, MinNum)))
    print (section)
    NormMat = []
    for k in mat:
        distance = list(map(lambda x: x[0] - x[1], zip(k, MinNum)))
        value = list(map(lambda x: x[0] / x[1], zip(distance, section)))
        NormMat.append(value)
    return NormMat
```
另外需要说明在训练模型的时候如何解决过拟合的问题：
学习曲线是什么？
学习曲线就是通过画出不同训练集大小时训练集和交叉验证的准确率，可以看到模型在新数据上的表现，进而来判断模型是否方差偏高或偏差过高，以及增大训练集是否可以减小过拟合。
![pytorch深度学习参加平安银行数据大赛，从驾驶行为预测驾驶风险](https://s1.51cto.com/images/blog/201805/21/d2032b344a182daa4cc8f6251c2e1145.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
当训练集和测试集的误差收敛但却很高时，为高偏差。 
左上角的偏差很高，训练集和验证集的准确率都很低，很可能是欠拟合。 
我们可以增加模型参数，比如，构建更多的特征，减小正则项。 
此时通过增加数据量是不起作用的。
当训练集和测试集的误差之间有大的差距时，为高方差。 
当训练集的准确率比其他独立数据集上的测试结果的准确率要高时，一般都是过拟合。 
右上角方差很高，训练集和验证集的准确率相差太多，应该是过拟合。 
我们可以增大训练集，降低模型复杂度，增大正则项，或者通过特征选择减少特征数。
理想情况是是找到偏差和方差都很小的情况，即收敛且误差较小。
最后即可载入已经训练好的神经网络预测结果：
```
#载入网络做分类预测
import torch
x = torch.Tensor(test1)
from torch.autograd import Variable
x = Variable(x)
print("载入分类网络")
net1 = torch.load('model/net1.pkl')
out = net1(x)
prediction = torch.max(out, 1)[1]
pred_y = prediction.data.numpy().tolist()
```
```
x = torch.Tensor(test2)
x = Variable(x)
#载入网络模型
net2 = torch.load('model/net2.pkl')
prediction = net2(x)
y =prediction.data.numpy().tolist()
print(y)
```
当然此处要完成这个比赛还是很不够的，还需要综合多个模型集成才能准确预测。
