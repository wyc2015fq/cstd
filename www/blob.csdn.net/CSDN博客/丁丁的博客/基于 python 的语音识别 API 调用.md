
# 基于 python 的语音识别 API 调用 - 丁丁的博客 - CSDN博客


2018年04月19日 23:31:35[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2433


[腾讯 AI 开放平台](https://ai.qq.com/)开放了语音、图像等多种 AI 功能接口。本文尝试基于 python 完成对语音识别接口的本地调用。
# 1. 准备工作
API 调用需要身份认证。我们首先需要注册并获得 AppID 和 AppKey。
在[官网](https://ai.qq.com/)注册后，进入控制台，创建一个新应用，并在**接口选择**栏，为应用勾选“语音识别”。应用创建成功后，记下 AppID 和 AppKey。
# 2. 接口鉴权
接口鉴权的要求见[官方说明](https://ai.qq.com/doc/auth.shtml)。为完成鉴权，我们需要定义一些辅助函数，代码见[这里](https://github.com/DingKe/yoyo/blob/master/utils.py)。
## 2.1 哈希
认证需要用到 MD5 哈希算法，这里我们调用 hashlib 库的实现。
```python
import
```
```python
hashlib
```
```python
def
```
```python
md5
```
```python
(string)
```
```python
:
```
```python
md = hashlib.md5()
    md.update(string)
    md5 = md.hexdigest().upper()
```
```python
return
```
```python
md5
```
## 2.2 签名
认证要求使用包括 AppKey 在内的参数拼接字符串的 MD5 值做为签名，其中 AppKey 要位于字条串最后：
```python
def
```
```python
signify
```
```python
(args, app_key)
```
```python
:
```
```python
query_str = urlencode(args)
    query_str = query_str +
```
```python
'&app_key='
```
```python
+ app_key
    signiture = md5(query_str)
```
```python
return
```
```python
signiture
```
参数需要按键值排序，实现如下：
```python
import
```
```python
urllib
```
```python
def
```
```python
urlencode
```
```python
(args)
```
```python
:
```
```python
tuples = [(k, args[k])
```
```python
for
```
```python
k
```
```python
in
```
```python
sorted(args.keys())
```
```python
if
```
```python
args[k]]
    query_str = urllib.urlencode(tuples)
```
```python
return
```
```python
query_str
```
# 3. 语音识别
语音识别接口的具体调用信息见[官网说明](https://ai.qq.com/doc/aaiasr.shtml)，提供了整段语音和流式两种调用方式。这里我们识别整段语音识别及 AI Lab 流式语音识别两个接口的调用。完成代码见[这里](https://github.com/DingKe/yoyo/blob/master/asr.py)。
调用使用的是 post  方式，简单封装如下：
```python
import
```
```python
requests
```
```python
def
```
```python
http_post
```
```python
(api_url, args)
```
```python
:
```
```python
resp = requests.post(url=api_url, data=args)
    resp = json.loads(resp.text)
```
```python
return
```
```python
resp
```
## 3.1 echo 版
echo 版对整段音频进行语音识别，并返回语音的文字内容。
```python
class
```
```python
BaseASR
```
```python
(object)
```
```python
:
```
```python
ext2idx = {
```
```python
'pcm'
```
```python
:
```
```python
'1'
```
```python
,
```
```python
'wav'
```
```python
:
```
```python
'2'
```
```python
,
```
```python
'amr'
```
```python
:
```
```python
'3'
```
```python
,
```
```python
'slk'
```
```python
:
```
```python
'4'
```
```python
}
```
```python
def
```
```python
__init__
```
```python
(self, api_url, app_id, app_key)
```
```python
:
```
```python
self.api_url = api_url
        self.app_id = app_id
        self.app_key = app_key
```
```python
def
```
```python
stt
```
```python
(self, audio_file, ext, rate)
```
```python
:
```
```python
raise
```
```python
Exceptin(
```
```python
"Unimplemented!"
```
```python
)
```
```python
class
```
```python
BasicASR
```
```python
(BaseASR)
```
```python
:
```
```python
""" Online ASR from Tencent
    https://ai.qq.com/doc/aaiasr.shtml
    """
```
```python
def
```
```python
__init__
```
```python
(self, api_url=
```
```python
'https://api.ai.qq.com/fcgi-bin/aai/aai_asr'
```
```python
,
                 app_id=
```
```python
'???'
```
```python
, app_key=
```
```python
'???'
```
```python
)
```
```python
:
```
```python
super(BasicASR, self).__init__(api_url, app_id, app_key)
```
```python
def
```
```python
stt
```
```python
(self, audio_file, ext=
```
```python
'wav'
```
```python
, rate=
```
```python
16000
```
```python
)
```
```python
:
```
```python
if
```
```python
ext ==
```
```python
'wav'
```
```python
:
            wf = wave.open(audio_file)
            nf = wf.getnframes()
            audio_data = wf.readframes(nf)
            wf.close()
```
```python
else
```
```python
:
```
```python
raise
```
```python
Exception(
```
```python
"Unsupport audio file format!"
```
```python
)
        args = {
```
```python
'app_id'
```
```python
: self.app_id,
```
```python
'time_stamp'
```
```python
: str(int(time.time())),
```
```python
'nonce_str'
```
```python
:
```
```python
'%.x'
```
```python
% random.randint(
```
```python
1048576
```
```python
,
```
```python
104857600
```
```python
),
```
```python
'format'
```
```python
: self.ext2idx[ext],
```
```python
'rate'
```
```python
: str(rate),
```
```python
'speech'
```
```python
: base64.b64encode(audio_data),
        }
        signiture = signify(args, self.app_key)
        args[
```
```python
'sign'
```
```python
] = signiture
        resp = http_post(self.api_url, args)
        text = resp[
```
```python
'data'
```
```python
][
```
```python
'text'
```
```python
].encode(
```
```python
'utf-8'
```
```python
)
```
```python
if
```
```python
DEBUG:
            print(
```
```python
'msg: %s, ret: %s, format: %s'
```
```python
%
                  (resp[
```
```python
'msg'
```
```python
], resp[
```
```python
'ret'
```
```python
], resp[
```
```python
'data'
```
```python
][
```
```python
'format'
```
```python
]))
```
```python
return
```
```python
text
```
## 3.2 流式版（AI Lab）
```python
class
```
```python
BasicStreamASR
```
```python
(BaseASR)
```
```python
:
```
```python
""" Online ASR from Tencent AI Lab
    https://ai.qq.com/doc/aaiasr.shtml
    """
```
```python
def
```
```python
__init__
```
```python
(self, api_url=
```
```python
'https://api.ai.qq.com/fcgi-bin/aai/aai_asrs'
```
```python
,
                 app_id=
```
```python
'???'
```
```python
, app_key=
```
```python
'???'
```
```python
)
```
```python
:
```
```python
super(BasicStreamASR, self).__init__(api_url, app_id, app_key)
```
```python
def
```
```python
stt
```
```python
(self, audio_file, ext=
```
```python
'wav'
```
```python
, rate=
```
```python
16000
```
```python
, chunk=
```
```python
4800
```
```python
)
```
```python
:
```
```python
if
```
```python
ext ==
```
```python
'wav'
```
```python
:
            wf = wave.open(audio_file)
```
```python
else
```
```python
:
```
```python
raise
```
```python
Exception(
```
```python
"Unsupport audio file format!"
```
```python
)
        total_len = wf.getnframes() * wf.getsampwidth()
        seq, end =
```
```python
0
```
```python
,
```
```python
'0'
```
```python
while
```
```python
end !=
```
```python
'1'
```
```python
:
            data = wf.readframes(chunk)
            length = len(data)
            end =
```
```python
'0'
```
```python
if
```
```python
length + seq < total_len
```
```python
else
```
```python
'1'
```
```python
args = {
```
```python
'app_id'
```
```python
: self.app_id,
```
```python
'time_stamp'
```
```python
: str(int(time.time())),
```
```python
'nonce_str'
```
```python
:
```
```python
'%.x'
```
```python
% random.randint(
```
```python
1048576
```
```python
,
```
```python
104857600
```
```python
),
```
```python
'format'
```
```python
: self.ext2idx[ext],
```
```python
'rate'
```
```python
: str(rate),
```
```python
'seq'
```
```python
: str(seq),
```
```python
'len'
```
```python
: str(length),
```
```python
'end'
```
```python
: end,
```
```python
'speech_id'
```
```python
:
```
```python
'0'
```
```python
,
```
```python
'speech_chunk'
```
```python
: base64.b64encode(data),
            }
            signiture = signify(args, self.app_key)
            args[
```
```python
'sign'
```
```python
] = signiture
            resp = http_post(self.api_url, args)
            seq += length
```
```python
if
```
```python
DEBUG:
                print(
```
```python
'seq: '
```
```python
, seq)
                print(resp[
```
```python
'data'
```
```python
][
```
```python
'speech_text'
```
```python
])
```
```python
return
```
```python
resp[
```
```python
'data'
```
```python
][
```
```python
'speech_text'
```
```python
].encode(
```
```python
'utf-8'
```
```python
)
```
# 4. 测试
语音必须符合 16k 或 8K 采样率、16bit 采样位数、单声道。支持 PCM、WAV、AMR、SILK 四种编码格式。这里我们只示例了无压缩的 WAV 格式。流式识别对于 AMR、SILK 的分片方式有其他要求，具体可以参见接口说明。
```python
def
```
```python
test_basic_asr
```
```python
()
```
```python
:
```
```python
audio_files = [
```
```python
'0.wav'
```
```python
,
```
```python
'1.wav'
```
```python
,
```
```python
'2.wav'
```
```python
]
    asr_engine = BasicASR()
```
```python
for
```
```python
audio_file
```
```python
in
```
```python
audio_files:
        text = asr_engine.stt(audio_file)
        print(text)
```
```python
def
```
```python
test_stream_asr
```
```python
()
```
```python
:
```
```python
audio_files = [
```
```python
'0.wav'
```
```python
,
```
```python
'1.wav'
```
```python
,
```
```python
'2.wav'
```
```python
]
    asr_engine = BasicStreamASR()
```
```python
for
```
```python
audio_file
```
```python
in
```
```python
audio_files:
        text = asr_engine.stt(audio_file)
        print(text)
```
# 小结
本文基于本地音频文件，识别了基于 python 的语音识别接口调用。基于流式掊，接合 ALSA 、PortAudio 等系统录音工具，我们实现实时的语音采样和识别。
语音识别接合语音唤醒（e.g[snowboy](https://github.com/Kitt-AI/snowboy)）、静音检测（e.g.[werbrtc VAD](https://github.com/wangshub/python-vad)）等技术，可以实现一个简单但完整的语音交互前端（一个简单的 demo 见[这里](https://github.com/DingKe/yoyo/blob/master/main_loop.py)）。

