# 几种分割loss - 别说话写代码的博客 - CSDN博客





2018年12月14日 17:08:17[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：302标签：[分割loss																[分割损失总结](https://so.csdn.net/so/search/s.do?q=分割损失总结&t=blog)](https://so.csdn.net/so/search/s.do?q=分割loss&t=blog)
个人分类：[计算机视觉](https://blog.csdn.net/qq_21997625/article/category/7782165)








转自：[https://blog.csdn.net/qq_16949707/article/details/79929951](https://blog.csdn.net/qq_16949707/article/details/79929951)

### 1 2d交叉熵（mutil class 分割）

定义

```python
def cross_entropy2d(input, target, weight=None, size_average=True):
    # input: (n, c, h, w), target: (n, h, w)
    n, c, h, w = input.size()
    # log_p: (n, c, h, w)
    if LooseVersion(torch.__version__) < LooseVersion('0.3'):
        # ==0.2.X
        log_p = F.log_softmax(input)
    else:
        # >=0.3
        log_p = F.log_softmax(input, dim=1)
    # log_p: (n*h*w, c)
    log_p = log_p.transpose(1, 2).transpose(2, 3).contiguous()
    log_p = log_p[target.view(n, h, w, 1).repeat(1, 1, 1, c) >= 0]
    log_p = log_p.view(-1, c)
    # target: (n*h*w,)
    mask = target >= 0
    target = target[mask]
    loss = F.nll_loss(log_p, target, weight=weight, size_average=False)
    if size_average:
        loss /= mask.data.sum()
    return loss
```

使用

```python
data, target = Variable(data, volatile=True), Variable(target)
            score = self.model(data)

            loss = cross_entropy2d(score, target,
                                   size_average=self.size_average)
            if np.isnan(float(loss.data[0])):
                raise ValueError('loss is nan while validating')
            val_loss += float(loss.data[0]) / len(data)

            imgs = data.data.cpu()
            lbl_pred = score.data.max(1)[1].cpu().numpy()[:, :, :]
            lbl_true = target.data.cpu()
```

其他metrics

```python
metrics = label_accuracy_score(label_trues, label_preds, n_class)

import numpy as np


def _fast_hist(label_true, label_pred, n_class):
    mask = (label_true >= 0) & (label_true < n_class)
    hist = np.bincount(
        n_class * label_true[mask].astype(int) +
        label_pred[mask], minlength=n_class ** 2).reshape(n_class, n_class)
    return hist


def label_accuracy_score(label_trues, label_preds, n_class):
    """Returns accuracy score evaluation result.
      - overall accuracy
      - mean accuracy
      - mean IU
      - fwavacc
    """
    hist = np.zeros((n_class, n_class))
    for lt, lp in zip(label_trues, label_preds):
        hist += _fast_hist(lt.flatten(), lp.flatten(), n_class)
    acc = np.diag(hist).sum() / hist.sum()
    acc_cls = np.diag(hist) / hist.sum(axis=1)
    acc_cls = np.nanmean(acc_cls)
    iu = np.diag(hist) / (hist.sum(axis=1) + hist.sum(axis=0) - np.diag(hist))
    mean_iu = np.nanmean(iu)
    freq = hist.sum(axis=1) / hist.sum()
    fwavacc = (freq[freq > 0] * iu[freq > 0]).sum()
    return acc, acc_cls, mean_iu, fwavacc
```

[https://github.com/wkentaro/pytorch-fcn](https://github.com/wkentaro/pytorch-fcn)

### 2 bce loss

```python
criterion = nn.BCELoss()
            y_pred = net(X)
            probs = F.sigmoid(y_pred)
            probs_flat = probs.view(-1)

            y_flat = y.view(-1)

            loss = criterion(probs_flat, y_flat.float())
            epoch_loss += loss.data[0]

            print('{0:.4f} --- loss: {1:.6f}'.format(i * batch_size / N_train,
                                                     loss.data[0]))

            optimizer.zero_grad()

            loss.backward()

            optimizer.step()
```

评估的时候又选择dice loss 这是啥原因？

```python
import torch
from torch.autograd import Function, Variable


class DiceCoeff(Function):
    """Dice coeff for individual examples"""

    def forward(self, input, target):
        self.save_for_backward(input, target)
        self.inter = torch.dot(input, target) + 0.0001
        self.union = torch.sum(input) + torch.sum(target) + 0.0001

        t = 2 * self.inter.float() / self.union.float()
        return t

    # This function has only a single output, so it gets only one gradient
    def backward(self, grad_output):

        input, target = self.saved_variables
        grad_input = grad_target = None

        if self.needs_input_grad[0]:
            grad_input = grad_output * 2 * (target * self.union + self.inter) \
                         / self.union * self.union
        if self.needs_input_grad[1]:
            grad_target = None

        return grad_input, grad_target


def eval_net():
        ...
        y_pred = net(X)

        y_pred = (F.sigmoid(y_pred) > 0.6).float()
        # y_pred = F.sigmoid(y_pred).float()

        dice = dice_coeff(y_pred, y.float()).data[0]
        ...
```

预测

```python
X = torch.FloatTensor(X).unsqueeze(0)
        y = torch.ByteTensor(y).unsqueeze(0)

        if gpu:
            X = Variable(X, volatile=True).cuda()
            y = Variable(y, volatile=True).cuda()
        else:
            X = Variable(X, volatile=True)
            y = Variable(y, volatile=True)

        y_pred = net(X)

        y_pred = (F.sigmoid(y_pred) > 0.6).float()
        # y_pred = F.sigmoid(y_pred).float()

        dice = dice_coeff(y_pred, y.float()).data[0]
        tot += dice

        if 0:
            X = X.data.squeeze(0).cpu().numpy()
            X = np.transpose(X, axes=[1, 2, 0])
            y = y.data.squeeze(0).cpu().numpy()
            y_pred = y_pred.data.squeeze(0).squeeze(0).cpu().numpy()
            print(y_pred.shape)

            fig = plt.figure()
            ax1 = fig.add_subplot(1, 4, 1)
            ax1.imshow(X)
            ax2 = fig.add_subplot(1, 4, 2)
            ax2.imshow(y)
            ax3 = fig.add_subplot(1, 4, 3)
            ax3.imshow((y_pred > 0.5))
```

[https://github.com/milesial/Pytorch-UNet/blob/master/eval.py](https://github.com/milesial/Pytorch-UNet/blob/master/eval.py)

### 3 loss与bce叠加

```python
outputs = model(images)
        loss, bce_loss, soft_dice_loss = criterion(outputs, labels)
        loss_val = loss.data[0]
        sum_epoch_loss += loss_val
        if i == 0:
            optimizer.zero_grad()
        loss.backward()
```

loss.py

```python
import torch
import torch.utils.data
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
import numpy as np


class BCELoss2d(nn.Module):
    """
    Binary Cross Entropy loss function
    """
    def __init__(self):
        super(BCELoss2d, self).__init__()
        self.bce_loss = nn.BCEWithLogitsLoss()

    def forward(self, logits, labels):
        logits_flat = logits.view(-1)
        labels_flat = labels.view(-1)
        return self.bce_loss(logits_flat, labels_flat)


class WeightedBCELoss2d(nn.Module):
    def __init__(self):
        super(WeightedBCELoss2d, self).__init__()

    def forward(self, logits, labels, weights):
        w = weights.view(-1)
        logits = logits.view(-1)
        gt = labels.view(-1)
        # http://geek.csdn.net/news/detail/126833
        loss = logits.clamp(min=0) - logits * gt + torch.log(1 + torch.exp(-logits.abs()))
        loss = loss * w
        loss = loss.sum() / w.sum()
        return loss


class WeightedSoftDiceLoss(nn.Module):
    def __init__(self):
        super(WeightedSoftDiceLoss, self).__init__()

    def forward(self, logits, labels, weights):
        probs = F.sigmoid(logits)
        num = labels.size(0)
        w = weights.view(num, -1)
        w2 = w * w
        m1 = probs.view(num, -1)
        m2 = labels.view(num, -1)
        intersection = (m1 * m2)
        score = 2. * ((w2 * intersection).sum(1) + 1) / (
            (w2 * m1).sum(1) + (w2 * m2).sum(1) + 1)
        score = 1 - score.sum() / num
        return score


class SoftDiceLoss(nn.Module):
    def __init__(self):
        super(SoftDiceLoss, self).__init__()

    def forward(self, logits, labels):
        probs = F.sigmoid(logits)
        num = labels.size(0)
        m1 = probs.view(num, -1)
        m2 = labels.view(num, -1)
        intersection = (m1 * m2)
        score = 2. * (intersection.sum(1) + 1) / (m1.sum(1) + m2.sum(1) + 1)
        score = 1 - score.sum() / num
        return score


class DiceScore(nn.Module):
    def __init__(self, threshold=0.5):
        super(DiceScore, self).__init__()
        self.threshold = threshold

    def forward(self, logits, labels):
        probs = F.sigmoid(logits)
        num = labels.size(0)
        predicts = (probs.view(num, -1) > self.threshold).float()
        labels = labels.view(num, -1)
        intersection = (predicts * labels)
        score = 2. * (intersection.sum(1)) / (predicts.sum(1) + labels.sum(1))
        return score.mean()


def dice_score_np(predicted_masks, labels):
    assert len(predicted_masks.shape) >= 3
    num = predicted_masks.shape[0]

    predicted_masks = predicted_masks.reshape(num, -1).astype(float)
    assert predicted_masks.min() == 0 and predicted_masks.max() == 1.0
    labels = labels.reshape(num, -1)
    assert labels.min() == 0 and labels.max() == 1.0
    intersection = (predicted_masks * labels)
    assert not np.any((predicted_masks.sum(1) + labels.sum(1)) == 0)
    score = 2. * (intersection.sum(1)) / (predicted_masks.sum(1) + labels.sum(1))
    assert len(score.shape) == 1
    return score.mean()


class CombinedLoss(nn.Module):
    def __init__(self, is_weight=True, is_log_dice=False):
        super(CombinedLoss, self).__init__()
        self.is_weight = is_weight
        self.is_log_dice = is_log_dice
        if self.is_weight:
            self.weighted_bce = WeightedBCELoss2d()
            self.soft_weighted_dice = WeightedSoftDiceLoss()
        else:
            self.bce = BCELoss2d()
            self.soft_dice = SoftDiceLoss()

    def forward(self, logits, labels):
        size = logits.size()
        assert size[1] == 1, size
        logits = logits.view(size[0], size[2], size[3])
        labels = labels.view(size[0], size[2], size[3])
        if self.is_weight:
            batch_size, H, W = labels.size()
            if H == 128:
                kernel_size = 11
            elif H == 256:
                kernel_size = 21
            elif H == 512:
                kernel_size = 21
            elif H == 1024:
                kernel_size = 41
            elif H == 1280:
                kernel_size = 51
            else:
                raise ValueError('Unknown height')

            a = F.avg_pool2d(labels, kernel_size=kernel_size, padding=kernel_size // 2,
                             stride=1)
            ind = a.ge(0.01) * a.le(0.99)
            ind = ind.float()
            weights = Variable(torch.tensor.torch.ones(a.size())).cuda()

            w0 = weights.sum()
            weights += ind * 2
            w1 = weights.sum()
            weights = weights / w1 * w0

            bce_loss = self.weighted_bce(logits, labels, weights)
            dice_loss = self.soft_weighted_dice(logits, labels, weights)
        else:
            bce_loss = self.bce(logits, labels)
            dice_loss = self.soft_dice(logits, labels)

        if self.is_log_dice:
            l = bce_loss - (1 - dice_loss).log()
        else:
            l = bce_loss + dice_loss
        return l, bce_loss, dice_loss


def combined_loss(logits, labels, is_weight=True, is_log_dice=False):
    size = logits.size()
    assert size[1] == 1, size
    logits = logits.view(size[0], size[2], size[3])
    labels = labels.view(size[0], size[2], size[3])
    if is_weight:
        batch_size, H, W = labels.size()
        if H == 128:
            kernel_size = 11
        elif H == 256:
            kernel_size = 21
        elif H == 512:
            kernel_size = 21
        elif H == 1024:
            kernel_size = 41
        elif H == 1280:
            kernel_size = 51
        else:
            raise ValueError('Unknown height')

        a = F.avg_pool2d(labels, kernel_size=kernel_size, padding=kernel_size // 2, stride=1)
        ind = a.ge(0.01) * a.le(0.99)
        ind = ind.float()
        weights = Variable(torch.tensor.torch.ones(a.size())).cuda()

        w0 = weights.sum()
        weights += ind * 2
        w1 = weights.sum()
        weights = weights / w1 * w0

        bce_loss = WeightedBCELoss2d().cuda()(logits, labels, weights)
        dice_loss = WeightedSoftDiceLoss().cuda()(logits, labels, weights)
    else:
        bce_loss = BCELoss2d().cuda()(logits, labels)
        dice_loss = SoftDiceLoss().cuda()(logits, labels)

    if is_log_dice:
        l = bce_loss - (1 - dice_loss).log()
    else:
        l = bce_loss + dice_loss
    return l, bce_loss, dice_loss
```

预测mask

```python
images = Variable(images.cuda(), volatile=True)

        outputs = model(images)
        outputs = F.upsample(outputs, size=original_shape, mode='bilinear')
        output_probs = F.sigmoid(outputs)
        if save_probs:
            probs_np = np.squeeze(output_probs.data.cpu().numpy())
            if len(probs_np.shape) == 2:
                probs_np = probs_np[np.newaxis, ...]
            assert len(probs_np.shape) == 3, probs_np.shape
            prob_images = np.asarray(np.round(probs_np * 255), dtype=np.uint8)
            for probs_img, sample_name in izip(prob_images, names):
                cv2.imwrite(str(output_dir.joinpath(sample_name + '.png')), probs_img)
```

### 4 bce和dice loss综合

albu的方法也是综合bce 和 diceloss

```python
def dice_loss(preds, trues, weight=None, is_average=True):
    num = preds.size(0)
    preds = preds.view(num, -1)
    trues = trues.view(num, -1)
    if weight is not None:
        w = torch.autograd.Variable(weight).view(num, -1)
        preds = preds * w
        trues = trues * w
    intersection = (preds * trues).sum(1)
    scores = 2. * (intersection + 1) / (preds.sum(1) + trues.sum(1) + 1)

    if is_average:
        score = scores.sum()/num
        return torch.clamp(score, 0., 1.)
    else:
        return scores

def dice_clamp(preds, trues, is_average=True):
    preds = torch.round(preds)
    return dice_loss(preds, trues, is_average=is_average)

class DiceLoss(nn.Module):
    def __init__(self, size_average=True):
        super().__init__()
        self.size_average = size_average

    def forward(self, input, target, weight=None):
        return 1-dice_loss(F.sigmoid(input), target, weight=weight, is_average=self.size_average)

class BCEDiceLoss(nn.Module):
    def __init__(self, size_average=True):
        super().__init__()
        self.size_average = size_average
        self.dice = DiceLoss(size_average=size_average)

    def forward(self, input, target, weight=None):
        return nn.modules.loss.BCEWithLogitsLoss(size_average=self.size_average, weight=weight)(input, target) + self.dice(input, target, weight=weight)

        ypreds = self.model(images)
        loss = self.criterion(ypreds, ytrues)

        ret['loss'] = loss.data
        for name, func in self.metrics:
            acc = func(F.sigmoid(ypreds)[:, :, 6:-6, 6:-6].contiguous(), ytrues[:, :, 6:-6, 6:-6].contiguous())
            ret[name] = acc.data
        if training:
            loss.backward()
            torch.nn.utils.clip_grad_norm(self.model.parameters(), 1.)
```

[https://github.com/asanakoy/kaggle_carvana_segmentation/tree/master/albu/src](https://github.com/asanakoy/kaggle_carvana_segmentation/tree/master/albu/src)




