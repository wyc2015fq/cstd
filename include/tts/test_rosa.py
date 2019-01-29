import librosa
import matplotlib.pyplot as plt
import numpy as np

path = "E:\BaiduNetdiskDownload\重生六零空间纪事\重生六零空间纪事\wav/001_1.wav"
y,sr = librosa.load(path)
# 通过改变采样率来改变音速，相当于播放速度X2
librosa.output.write_wav("resample.wav",y,sr//2)

b = librosa.effects.pitch_shift(y, sr, n_steps=14)
librosa.output.write_wav("pitch_shift.wav",b,sr)
# stft 短时傅立叶变换
a = librosa.stft(y)
length = len(a)
 
# 改变或去除某些值，可以改变声音
r_a = a[10:length-10]
 
# istft 逆短时傅立叶变换，变回去
b = librosa.istft(r_a)
 
librosa.output.write_wav("stft.wav",b,sr)
 
# 以下是显示频谱图
fig = plt.figure()
s1 = fig.add_subplot(3,1,1)
s2 = fig.add_subplot(3,1,2)
s3 = fig.add_subplot(3,1,3)
 
s1.plot(y)
s2.plot(a)
s3.plot(b)
 
plt.show()
