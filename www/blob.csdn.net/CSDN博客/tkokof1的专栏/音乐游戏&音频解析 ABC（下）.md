# 音乐游戏&音频解析 ABC（下） - tkokof1的专栏 - CSDN博客

2013年03月02日 15:14:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：3247




  四. 路途

  接着上回所述，我们现在要解决的问题便是：

  “2. 通过所得数据通过一定方法解析出其中的歌曲信息”

  首先我们应该先确定一下我们到底需要解析歌曲的哪些信息？歌名？作者？呵呵，都不是，我们要解析的应该是歌曲的“信号”属性（譬如歌曲的[BPM](http://en.wikipedia.org/wiki/Beats_Per_Minute)）。然而歌曲的这些属性非常繁杂，有些解析起来比较简单，有些提取起来则比较困难，出于简单考虑，我们这次的目标就定于歌曲的[Onset](http://en.wikipedia.org/wiki/Onset_(audio))属性上，一方面是因为Onset是歌曲的重要特征之一，通过它我们可以了解不少歌曲信息；另一方面则是由于提取Onset的方法也相对容易，算法过程不会显的艰辛苦涩。（当然的，歌曲或者说音频信号的另一些特性属性就不那么“可爱”了，譬如[MFCC](http://en.wikipedia.org/wiki/Mel-frequency_cepstrum)，解析过程就相对繁琐艰涩，对此有兴趣的朋友可以仔细看看：））

  那么到底什么是歌曲的Onset呢？非专科出身的我们在此便简单理解：所谓Onset，就是歌曲中音符的起始点，或者更简单的说，就是你在歌曲中听到的“重音”点（我本人的“简陋”理解：）），通过歌曲的Onset，我们能够大体感觉出一首歌的节奏变化，而通过歌曲的节奏变化，我们便能简单的辅以一些规则元素（譬如目前音乐游戏中“泛滥”的节奏点击），使之成为一个游戏 ：）

  OK，我们确认了我们所需的音乐信息，那么接下来的问题便是：通过什么方法来获取呢？前面我们已经顺利获取了歌曲的PCM数据，内容大概便是歌曲在各个时间点的波形幅值，或是一个整数，或是一个小数（规范化后），仅仅通过这些“原生”数据，我们确实也可以得到不少信息（譬如[ZCR](http://en.wikipedia.org/wiki/ZCR)），但是这还远远不够，如果我们想要得到相对精确的Onset信息，还要借助另外的方法，而这方法便是在DSP领域“无人不知”的——[傅里叶变换](http://en.wikipedia.org/wiki/Fourier_transform) ：）

  关于傅里叶变化，本人仅是初通，网上有不少优秀的教程（譬如[这里](http://blog.csdn.net/v_JULY_v/article/details/6196862)），有兴趣的朋友可以仔细看看，而就傅里叶变化在信号处理中的作用来讲，我们可以简单的这么理解：前面我们提到过声波，当时简单的就以一个正弦波来替代了，而实际上的声波自然比这要复杂，但是我们仍然可以将其认为为多种正弦波的叠加，即实际上的声波不是单一的正弦波，而是多种正弦波（的叠加），而傅里叶变化就是帮助我们将信号（声波）分解为多种正弦波的一种方法。

  有了这些歌曲的频率数据，我们便能获取歌曲的更多信息，譬如判断当前的歌曲是“清亮”的还是“低沉”的，我们只要简单的查询歌曲频率的幅值，如果高频部分“能量”大，即可判断其相对“清亮”，相反，如果低频部分“能量”大，则可认为其相对“低沉”。不过可惜的是，傅里叶变换虽然作用巨大，但是其一直有一个致命的弱点：计算复杂度较高，对于一些实时程序（譬如游戏）基本不能适用。那怎么办呢？这时我们便需要仰仗[快速傅里叶变换](http://en.wikipedia.org/wiki/Fast_Fourier_transform)了 ：）

  所谓快速傅里叶变换，顾名思义，其实便是一种快速计算傅里叶变换的算法，当然了，这种的说法有些欠妥（傅里叶变换本身还有细分，譬如[离散傅里叶变换](http://en.wikipedia.org/wiki/Discrete_Fourier_transform)，快速傅里叶变换实际上是针对离散傅里叶变换而提出的），但是好在我们目前还不需要去理会，大概知道快速傅里叶变换可以帮助我们快速计算出歌曲（音频）信号的频率信息即可。

  OK，至此我们已经可以计算歌曲的频率信息了，那么接下来的问题就是：我们如何通过这些频率信息获取我们想要的歌曲信息（即Onset）呢？在此我们又要介绍一个新的名词：[Spectral Flux](http://en.wikipedia.org/wiki/Spectral_flux)，有兴趣的朋友可以仔细了解，其大概意思便是频率能量差异。我们之前提到Onset即是一些我们在歌曲中听到的一些“重音”，而这些音之所以被称为“重音”，其实便是因为其“前音”较轻，以频率信息来说的话，即其“前音”的频率能量较弱，而当前“音”的频率能量较强，我们通过检测这些频率能量差异，便可以找出我们所需要的Onset ：）（当然，检测Onset的方法远不止这些，有个小巧的[OnsetsDS](onsetsds.sourceforge.net)库提供了不少相关方面的参考，有兴趣的朋友可以看看）

  在此我们暂停脚步，稍稍总结一下我们先前所讲的各项步骤：

  1. 首先我们通过一定方法（譬如[之前](http://blog.csdn.net/tkokof1/article/details/8581938)提到的[libsndfile](http://www.mega-nerd.com/libsndfile/)）从歌曲文件中获取歌曲的PCM数据（或者说音频信号）。

  2. 应用快速傅里叶变换对这些PCM数据（或者说音频信号）进行变换，以获取音频信号中的频率信息。

  3. 再使用一定方法（譬如Spectral Flux）从这些频率信息中获取我们需要的歌曲信息（譬如Onset）

  4. 通过这些歌曲信息（譬如Onset），结合我们的游戏规则设计（譬如节奏点击），一个简单的音乐游戏原型就完成了 ：）

  五. 终点

  最后，让我们简单的辅以一些代码细节来分别讲一讲上面所述的四个步骤，处于简单考虑，此处我们省略了音频数据获取（有兴趣的朋友可以以libsndfile为出发点进行了解）和快速傅里叶变换的部分（关于FFT（即快速傅里叶变换）的实现可以参考[这里](http://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857)，当然网上也有非常优秀的FFT程序库，譬如[FFTW](http://www.fftw.org/)，有兴趣的朋友可以仔细了解一下），而直接选用[BASS](http://www.un4seen.com/bass.html)库来承担这些工作，但就基本原理而言，并没有什么差别：

  首先是 DSPConfig.h 文件，非常简单：

#ifndef__DSP_CONFIG_H__

#define__DSP_CONFIG_H__

//! FFT block frames count

constsize_tDSP_AUDIO_FRAME_PER_BLOCK = 4096;

#endif// __DSP_CONFIG_H__

  内容目前只是定义一个常量而已，其意义便是FFT运算时的每一段的帧数。前面我们有提到过PCM中的取样操作，而每一次取样操作所取得的数值便称为帧，FFT运算时需要确定所操作的帧数数量，而DSP_AUDIO_FRAME_PER_BLOCK便是用于定义这个数量。

  接着是IDSPEventDelegate.h，该文件中定义了不少解析中的结构：

//! dsp stream event

enumEDSPEvent

{

EDE_SpectrumBegin = 0, //< dsp spectrum begin event

EDE_Spectrum, //< dsp spectrum event

EDE_SpectrumEnd//< dsp spectrum event end

};

  EDSPEvent定义了解析过程中的所产生的事件（信号）。

//! audio info struct

structAudioInfo

{

intsamplerate; //< audio samplerate

intchannel; //< audio channel

AudioInfo():samplerate(0),channel(0) {};

AudioInfo(constAudioInfo& audioInfo):samplerate(audioInfo.samplerate),channel(audioInfo.channel) {};

AudioInfo& operator = (constAudioInfo& audioInfo)

{

samplerate = audioInfo.samplerate;

channel = audioInfo.channel;

return *this;

}

};

  简单的AudioInfo结构用以存储歌曲的各项属性（譬如采样率、声道等等）。

//! dsp event struct

structSDSPEventData

{

EDSPEventevent; //< event type

AudioInfoaudioInfo; //< audio info

size_tcurFrame; //< current audio frame

floatcurTime; //< current audio frame time

size_tspectrumCount; //< spectrum count

constfloat* spectrumData; //< event extra data

// 省略的构造赋值函数

};

  这个结构定义了解析过程中的事件参数，包括事件类型，当前帧数等等，细节可以参考源码 ：）

//! dsp event listener

classIDSPEventDelegate

{

public:

virtual ~IDSPEventDelegate() {};

//! dsp event handler

virtualvoidOnEvent(constSDSPEventData& event) = 0;

};

  最后的便是IDSPEventDelegate，用于处理DSP事件的代理，有过iOS开发经验的朋友对于Delegate应该是再熟悉不过了，大抵很类似于[Observer](http://en.wikipedia.org/wiki/Observer_pattern)，有兴趣的朋友可以仔细看看 ：）

  好了，接下了便是DSPParserBASS.h，功能其实也很直白，就是使用BASS库解析出相应文件的频率信息，头文件内容基本如下:

classDSPParserBASS

{

public:

DSPParserBASS():m_BASSStream(0),m_curFrameCount(0),m_dspDelegate(NULL) {};

//! do DSP stream operation by the given filename

boolDoDSPParse(constchar* filename);

//! dsp event delegate

voidSetDSPEventDelegate(IDSPEventDelegate* delegate_) { m_dspDelegate = delegate_; }

IDSPEventDelegate* GetDSPEventDelegate() { returnm_dspDelegate; }

private:

// dispatch helper function

voiddispatchSpectrumEvent(EDSPEventevent, size_tspectrumCount, constfloat* spectrum);

// get bass FFT flag

DWORDgetBassFFTFlag(size_twindowSize) const;

private:

HSTREAMm_BASSStream; //< BASS stream handler

size_tm_curFrameCount; //< current frame count

AudioInfom_audioInfo; //< audio info

private:

IDSPEventDelegate* m_dspDelegate; //< dsp event delegate

};

  SetDSPEventDelegate与GetDSPEventDelegate不用详述，设置和获取Delegate而已，DoDSPParse则是核心的用以解析的函数，在此我们稍稍细看一下他：

boolDSPParserBASS::DoDSPParse(constchar* filename)

{

// first load the file

m_BASSStream = BASS_StreamCreateFile(false, filename, 0, 0, BASS_STREAM_DECODE|BASS_STREAM_PRESCAN);

if (!m_BASSStream)

{

printf("Error to load audio file : %s", filename);

returnfalse;

}

// get the channel info

BASS_CHANNELINFOinfo;

BOOLret = BASS_ChannelGetInfo(m_BASSStream, &info);

if (!ret)

{

printf("Error to load audio info : %s", filename);

returnfalse;

}

// get audio info

m_audioInfo.channel = info.chans;

m_audioInfo.samplerate = info.freq;

m_curFrameCount = 0;

// dispatch begin event

dispatchSpectrumEvent(EDE_SpectrumBegin, 0, NULL);

// allocate buffer

constsize_tsizeCount = DSP_AUDIO_FRAME_PER_BLOCK / 2;

float* spectrum = newfloat[sizeCount];

// use BASS get spectrum data, more info see BASS doc

DWORDfftFlag = getBassFFTFlag(DSP_AUDIO_FRAME_PER_BLOCK);

DWORDspectrumRet = BASS_ChannelGetData(m_BASSStream, spectrum, fftFlag);

while (spectrumRet != -1)

{

intspectrumCount = spectrumRet / sizeof(float);

m_curFrameCount += spectrumCount;

// dispatch parsing event here

dispatchSpectrumEvent(EDE_Spectrum, spectrumCount, spectrum);

spectrumRet = BASS_ChannelGetData(m_BASSStream, spectrum, fftFlag);

}

// dispatch end event

dispatchSpectrumEvent(EDE_SpectrumEnd, 0, NULL);

// release the bass stream here

BASS_StreamFree(m_BASSStream);

m_BASSStream = NULL;

// release buffer

delete[] spectrum;

returntrue;

}

  通过注释其实流程非常明了：首先是创建Stream，然后获取Stream的信息，接着按照DSP_AUDIO_FRAME_PER_BLOCK大小一段一段的解析Stream的频率信息，并发送相应消息，最后自然是做一些清理 ：）至于相关函数细节，有兴趣的朋友可以参考一下BASS库的文档，在此就不细述了。

  最后，便是真正执行Onset检测的AudioAnalysisOnsetSpectralFlux：

classAudioAnalysisOnsetSpectralFlux: publicIDSPEventDelegate

{

public:

AudioAnalysisOnsetSpectralFlux():m_thresholdTime(0.16f),m_multiplier(1.5f) {};

//! reset method

voidReset();

//! dsp event handler

virtualvoidOnEvent(constSDSPEventData& event);

//! get the onset count

size_tGetOnsetCount() const { returnm_onset.size(); }

//! get the onset time which is detected

floatGetOnsetByIndex(size_tindex) const

{ 

if (index >= m_onset.size()) return -1;

returnm_onset[index];

}

private:

// get spectrum data

voidgetSpectrumData(constSDSPEventData& event);

// do spectrum analysis

voiddoSpectrumAnalysis();

private:

std::vector<float> m_lastSpectrum; //< last spectrum buffer

std::vector<float> m_spectralFlux; //< spectral flux TODO: more detail, like bin spectral flux

std::vector<float> m_threshold; //< threshold flux buffer

std::vector<float> m_prunnedSpectralFlux; //< prunne spectral flux

std::vector<float> m_onset; //< onset storage

private:

floatm_thresholdTime; //< threshold time

floatm_multiplier; //< onset detect multiplier

private:

AudioInfom_audioInfo; //< audio info buffer

};

  内容可能稍显凌乱，但就使用上来说其实就是：GetOnsetCount和GetOnsetByIndex，用途便是获取Onset个数和Onset信息（时间点），非常简单。

  在此我们仔细看一下doSpectrumAnalysis这个函数，因为其便是用于解析Onset的核心函数：

voidAudioAnalysisOnsetSpectralFlux::doSpectrumAnalysis()

{

//////////////////////////////////////////////////////////////////////////

size_tthresholdWindowSize = 10;

//////////////////////////////////////////////////////////////////////////

// do threshold

for (size_ti = 0; i < m_spectralFlux.size(); ++i)

{

intstart = std::max(0, int(i - thresholdWindowSize));

intend = std::min((int)(m_spectralFlux.size() - 1), (int)(i + thresholdWindowSize));

floatmean = 0;

for (intj = start; j <= end; ++j)

{

mean += m_spectralFlux[j];

}

mean /= (end - start);

m_threshold.push_back(mean * m_multiplier);

}

// do prune

for (size_ti = 0; i < m_threshold.size(); ++i)

{

if (m_threshold[i] <= m_spectralFlux[i])

{

m_prunnedSpectralFlux.push_back(m_spectralFlux[i] - m_threshold[i]);

}

else

{

m_prunnedSpectralFlux.push_back(0);

}

}

// at last, we analysis onset

for (inti = 0; i < int(m_prunnedSpectralFlux.size() - 1); ++i)

{

if (m_prunnedSpectralFlux[i] > m_prunnedSpectralFlux[i+1])

{

m_onset.push_back(i * DSP_AUDIO_FRAME_PER_BLOCK * 1.0f / m_audioInfo.samplerate);

}

}

}

  此处相交简单的前后帧数频率对比，做了更多的工作：首先是确定了一个阈值窗口，即thresholdWindowSize，用以进一步过滤太短的差异变化，方法是取一定范围内的平均值，接着又做了进一步的“裁剪”，用以记录平均值与原值的差异，最后通过差异的大小，确定Onset位置（差异大则说明频率能量改变大，也就相当于Onset的位置）。

  OK，细节到此为止，有兴趣的朋友可以自取一份[源码](https://github.com/tkokof/GitTest/tree/master/Examples/DSP/AudioOnset)来看看。

六. 尾声

  前面的过程貌似简单，但实际上仍然有不少问题存在，其中一个便是效率，虽然FFT的运算速度已经较普通的傅里叶变换有了长足的进步，但是有时仍然难以满足实时程序需求，尤其像游戏这类对效率要求极高的程序更是难以适从；另一个与游戏相关的问题便是歌曲信息的进一步运用，虽然上述的Onset信息很大程度上表现出了歌曲的节奏变化，但是这与优秀的音乐游戏关卡还有不少距离，优秀的音乐游戏关卡除了能表现出这些节奏变化，还要能让我们区分出其中的主次，辨别出歌曲的层次，以及其节奏位置排布也要附有变化性和适用性等等，而这些单靠单薄的一个歌曲Onset信息还远远不够……

  好了，关于音乐游戏和音频解析的一些内容算是粗糙的讲了一些基础，算作总结，也希望能给有兴趣的朋友做些参看，网上有很多优秀的参考：譬如有很多程序库已经帮我们解决了上面的问题（譬如[Aubio](http://aubio.org/)，[libxtract](libxtract.sourceforge.net)，[Marsyas](marsyasweb.appspot.com)等等），当然这些库比较偏向[MIR](http://en.wikipedia.org/wiki/Music_information_retrieval)，稍稍学术化一些，而[QueenMary](http://www.elec.qmul.ac.uk/digitalmusic/index.html)则是研究MIR领域中不错的一个机构，感觉很权威，他们有个工具[SonicVisualiser](http://www.sonicvisualiser.org/)很不错 ：）当然也有不少偏向运用的程序库，譬如[Maximilian](http://maximilian.strangeloop.co.uk/) 、[ALF](http://music.ece.drexel.edu/ALF)等等，都是不错的相关参考，网上自然还有不少教程，譬如[这里](http://www.badlogicgames.com/wordpress/?p=122)，强力推荐，上面文章中的很多内容都是借鉴于此：）另外的书籍也有不少，其中一本之前提到过，这里再推荐一次（[这里](http://www.dspguide.com/)） ：）

  OK，闲话结束，下次再见吧 ：）

