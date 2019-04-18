# 使用waveOut接口在Windows中播放声音 - 深之JohnChen的专栏 - CSDN博客

2012年03月04日 00:01:08[byxdaz](https://me.csdn.net/byxdaz)阅读数：12275


**                                                                                                                      Windows waveOut教程**
本教程将帮助您了解如何使用Windows waveOut接口播放数字音频。根据经验，这些接口函数掌握起来有些困难。在本教程中，我们将会建立一个Windows命令行程序来原始数字音频。注意：本教程假设您熟悉C程序及Windows API的使用。理解数字音频的相关知识也是有益的，但不是必须的。 

**教程内容：**

·                             获取文档 

·                             什么是数字音频 

·                             打开声音设备 

·                             播放声音 

·                             播放流式音频到设备 

·                             缓存机制 

·                             运行程序 

·                             接下来该做什么？ 

**获取文档**
首先，您需要有关waveOut接口的相关文档。如果您有Microsoft Platform SDK或者VisualC++，那么它们已经提供了相关的信息。如果您还没有这些，您可以通过MSDN在线查看。(http://msdn.microsoft.com) 

**什么是数字音频**
这一部分是为那些对数字音频如何存储完全没有概念的人准备的。如果您理解有关数字音频的信息，同时了解“样本(sample)”、“采样频率(samplerate)”、“样本大小(samplesize)”及“声道(channels)”的概念，您可以跳过此节。 
只要把字节码发送到声卡就可以播放声音了，但是这些字节都是什么意思呢？音频只是简单的一系列运动的压力波。在现实世界中，它们是一些相似的波形，但是在数字世界中，我们必须从这波形中采集一系列的样本并存储起来。“样本”是表现某一时间点上波形振幅的一个值——它仅仅是一个数字。 
“采样频率”表明我们采集波形样本的频率。它的计量单位是赫兹(Hz)或每秒样本数。显然，采样频率越高，采样的波形就越接近真实的波形，所以声音的音质也就越好。 
另外一个有助于改善音质的参数是每个样本的大小。当然，样本越大音质也就越好。样本大小用字节位数(bits)来计量。为何样本越大音质越好?假设一个8bits的样本，它有256（2的八次方）个可能的值，这意味着您不能精确地再现波形的振幅。而如果是一个16bits的样本，它的可能值有65536（2的16次方）个，这样它就拥有256倍于8bits的样本的更精确表现波形的能力。 
最后是关于声道。在多数机器上有两个喇叭（左、右），那是两个声道。您需要同时把样本数据存入左声道和右声道。 
幸运的是，操作两个声道是很容易的（您将在本教程中看到）。样本总是交错存储的，它们将按左、右、左、右……的顺序存储。 
CD品质的音频采样频率是44100Hz，样本大小是16bits,意味着1M的音频数据只能持续约6秒的时间。 

**打开声音设备**
打开声音设备需要使用waveOutOpen函数（可以在您的文档中查到）。象其它许多Windows对象一样，您可以简单地使用一个句柄（Handle)调用该设备。如存储Windows窗口句柄使用HWND类似，我们可以用HWAVEOUT句柄来调用声音设备。 
下面的代码段说明了如何打开一个CD标准音质的波形设备，然后关闭它。 

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
int main(int argc, char* argv[])
{

HWAVEOUT hWaveOut; /* device handle */
WAVEFORMATEX wfx; /* look this up in your documentation */
MMRESULT result;/* for waveOut return values */
/*
* first we need to set up the WAVEFORMATEX structure. 
* the structure describes the format of the audio.
*/
wfx.nSamplesPerSec = 44100; /* sample rate */
wfx.wBitsPerSample = 16; /* sample size */
wfx.nChannels = 2; /* channels*/
/*
* WAVEFORMATEX also has other fields which need filling.
* as long as the three fields above are filled this should
* work for any PCM (pulse code modulation) format.
*/
wfx.cbSize = 0; /* size of _extra_ info */
wfx.wFormatTag = WAVE_FORMAT_PCM;
wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
/*
* try to open the default wave device. WAVE_MAPPER is
* a constant defined in mmsystem.h, it always points to the
* default wave device on the system (some people have 2 or
* more sound cards).
*/
if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) !=MMSYSERR_NOERROR) {

fprintf(stderr, "unable to openWAVE_MAPPER device\n");
ExitProcess(1);

}
/*
* device is now open so print the success message
* and then close the device again.
*/
printf("The Wave Mapper device was opened successfully!\n");
waveOutClose(hWaveOut);
return 0;

}

注意：要编译本程序，您需要添加winmm.lib到您的工程，否则将会链接失败。 
好了，我们已经做好了第一步，现在声音设备已经准备好，我们可以写音频数据进去了。 

**播放声音**
打开和关闭声音设备挺有意思的，但是上面的代码并没有真的做什么事情。我们想要的是能从设备听到声音。在这之前，我们有两件事要做。 

·                             获得一个正确格式的原始音频 

·                             解决如何将数据写入设备 

问题一很好解决，您可以使用Winamp的DiskWriter插件来转换一个音乐文件为原始音频。比如您可以转换\Windows\Media下的Windows声音文件(比如Ding.wav)为原始音频文件。如果您不能转换这些文件，那么直接播放未经转换的文件也是件很有意思的事。直接播放的话，听起来会很快，因为这些文件大部分是用22kHz的采样频率存储的。 
问题二就稍微复杂一些了。音频是以块（Block）的形式写入设备的，每个块都有它自己的头(Header)。写入一个块(Block)是很容易的，但是大部分时候，我们需要建立一个队列机制并写入很多的块（Blocks)。之所以用一个小的文件开始学习，是因为下面的例子我们将载入整个文件到一个块中并写入设备。 
首先，我们要写个函数发送一块数据到音频设备中。函数命名为writeAudioBlock。要写入音频数据，我们需要三个接口函数：**waveOutPrepareHeader**,**waveOutWrite**和**waveOutUnprepareHeader**，并按这个顺序调用它们。您可以在相关文档中查找到并熟悉这些函数。 
下面的代码是函数writeAudioBlock的初期版本。 

void writeAudioBlock(HWAVEOUT hWaveOut, LPSTR block, DWORD size)
{

WAVEHDR header;
/*
* initialize the block header with the size
* and pointer.
*/
ZeroMemory(&header, sizeof(WAVEHDR));
header.dwBufferLength = size;
header.lpData = block;
/*
* prepare the block for playback
*/
waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
/*
* write the block to the device. waveOutWrite returns immediately
* unless a synchronous driver is used (not often).
*/
waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
/*
* wait a while for the block to play then start trying
* to unprepare the header. this will fail until the block has
* played.
*/
Sleep(500);
while(waveOutUnprepareHeader(hWaveOut,&header,sizeof(WAVEHDR)) ==WAVERR_STILLPLAYING)

Sleep(100);

}

现在我们有了一个写入块数据的函数。我们还需要一个函数来获得音频数据块。这就是函数loadAudioBlock的任务了。函数loadAudioBlock读取文件到内存并把指针返回。下面就是loadAudioBlock的代码: 

LPSTR loadAudioBlock(const char*filename, DWORD* blockSize)
{

HANDLE hFile= INVALID_HANDLE_VALUE;
DWORD size = 0;
DWORD readBytes = 0;
void* block = NULL;
/*
* open the file
*/
if((hFile = CreateFile(

filename,
GENERIC_READ,
FILE_SHARE_READ,
NULL,
OPEN_EXISTING,
0,
NULL
)) == INVALID_HANDLE_VALUE)

return NULL;

/*
* get it's size, allocate memory and read the file
* into memory. don't use this on large files!
*/
do {

if((size = GetFileSize(hFile, NULL)) ==0) 

break;

if((block = HeapAlloc(GetProcessHeap(),0, size)) == NULL)

break;

ReadFile(hFile, block, size,&readBytes, NULL);

} while(0);
CloseHandle(hFile);
*blockSize = size;
return (LPSTR)block;

}

这部分的最后，是整个程序调用和main函数。 

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
LPSTR loadAudioBlock(const char* filename, DWORD* blockSize);
void writeAudioBlock(HWAVEOUT hWaveOut, LPSTR block, DWORD size);
int main(int argc, char* argv[])
{

HWAVEOUT hWaveOut; 
WAVEFORMATEX wfx; 
LPSTR block;/* pointer to the block */
DWORD blockSize;/* holds the size of the block */

.
. (leave middle section as it was) 
.
printf("The Wave Mapper device was opened successfully!\n");
/*
* load and play the block of audio
*/
if((block = loadAudioBlock("c:\\temp\\ding.raw", &blockSize)) ==NULL) {

fprintf(stderr, "Unable to loadfile\n");
ExitProcess(1);

}
writeAudioBlock(hWaveOut, block, blockSize); 
waveOutClose(hWaveOut);
return 0;

}

将上面的代码放到一个工程里进行编译就可以播放小的声音文件了。我们实现了类似PlaySound函数的功能。请试着做一些小的试验：改变播放的采样频率（在main函数中）或者改变样本大小（注意：一定要是8的倍数）看看会发生什么，甚至可以改变一下声道的数量。我们会发现改变采样频率或者声道数会加快或减慢播放的速度，而改变样本大小可能会有毁灭性的影响! 

**播放流式音频到设备**
您可能会注意到上面的代码有几个重要的缺陷（注意这些都是故意的:)），明显的几个缺陷如下： 

·                             限于载入数据的方式，我们不能播放太大的文件。现在的方法缓存整个文件并一次播放完毕。而音频本质上是很大的，所以我们需要找一种方法把音频数据转换为流式数据并一个块（Block)接一个块的写入到设备中。 

·                             现有的writeAudioBlock函数是同步执行的，所以一个位（bit)一个位地写入多个块会在两个块输出之间有个间隔（即不能足够快速地重新填充缓存(buffer)）。微软建议至少需要两个buffer的体制，这样我们可以在播放一个块（block)的同时填充另一个块，然后交换播放和填充的块。实际上这样也不完全能解决问题。即使交换数据块播放也会引起一个非常小（但是很烦人）的间隔。 

幸运的是数据块的读取很简单，所以暂时不用管它。现在让我们集中于如何建立一种缓存机制以避免出现音频设备的声音间隔吧。 
这个块切换的问题并不象它听起来那么严重。我们不能无间隔地切换两个数据块，但是接口有某种机制可以让我们避开这个问题。接口管理着一个块的队列，我们用waveOutPrepareHeader传送的每个数据块都可以通过调用waveOutWrite插入到这个队列中。这意味着我们可以写2个（或者更多）的数据块到设备中，当第一个数据块播放时填充第三个数据块，然后当第二个播放的时候再进行切换。这样我们可以得到无间隔的音频输出了。 
在说明这个方法之还有最后一个问题，我们如何知道一个数据块播放完了？前面writeAudioBlock的第一个例子中直到块完成再调用waveOutUnprepareHeader的方式是非常不好的。我们在实际应用中不能这么做，因为我们还要继续填充新的数据块到设备中以继续播放，关于这些，在waveOut接口中提供了更好的方法来实现。 
waveOut接口提供了4种回调机制来通知我们数据块已经播放完成了。它们是： 

·                             事件(Event)——数据块播放完成的时候会触发一个事件 

·                             回调函数(CallbackFunction)——数据块播放完成时会调用一个函数 

·                             线程消息(Thread)——数据块播放完成时会发送一个线程消息 

·                             窗口消息（Window)——数据块播放完成时会发送一个窗口消息 

要指定使用哪种方式只需要在调用waveOutOpen函数时指定参数dwCallback的值就可以了。在我们下面的例子中将使用回调函数的方式。 
所以我们需要一个新的函数：waveOutProc。这个函数如何定义可以在相关文档中查到。您可以看到，这个函数将在以下三种情况下被调用： 

·                             设备打开时（Opened） 

·                             设备关闭时（Closed） 

·                             数据块播放完成时 

我们感兴趣的只是数据块播放完成时这种情况。 

**缓存机制**
我们将要实现的缓存机制如上面我们提到的一样运行。它需要一个变量来随时保存空闲缓存（buffer)的数量（你可能想到了使用信号量（Semaphore)来控制，但是我们不能使用它，后面将解释原因）。这个变量初始化为缓存的数量，当数据块写入的时候减小并在数据块完成时增加。如果没有缓存可用，我们将等待直到该变量计数器为1以上然后再继续写入。这将可以让我们有效地循环向任何数量的数据块队列中写入数据。我们例子中没有使用3个数据块队列，而是更多，比如20个，这样每次可以处理大概8KB的数据。 
有些事情你可能猜中了：waveOutProc是在不同的线程中被调用的。Windows建立了一个特殊的线程来管理音频播放。在此回调函数中你可以做的事情有很多限制。让我们看一下微软的文档上是怎么说的吧： 

**"Applications should not call anysystem-defined functions from inside a callback function, except forEnterCriticalSection, LeaveCriticalSection, midiOutLongMsg, midiOutShortMsg,OutputDebugString, PostMessage, PostThreadMessage, SetEvent, timeGetSystemTime,timeGetTime, timeKillEvent, and timeSetEvent. Calling other wave functions will cause deadlock." 应用程序不能在该回调函数中调用除下列以外的系统函数：EnterCriticalSection,LeaveCriticalSection, midiOutLongMsg, midiOutShortMsg, OutputDebugString,PostMessage, PostThreadMessage, SetEvent, timeGetSystemTime, timeGetTime,timeKillEvent, and timeSetEvent。 调用其它的wave函数可能会引起死锁。 **

这解释了为什么我们不能使用信号量（Semaphore）——这将需要调用ReleaseSemaphore系统函数，而这是我们不能去做的。在实际应用中可能会灵活一些——我见过在回调函数中使用信号量的代码，不过那样的程序可能在某些版本的Windows上可以执行而不能执行在其它版本的机器上。同样的，在回调函数中调用waveOut函数也将导致死锁。实际上我们也将在回调函数中调用waveOutUnprepareHeader，不过我们不能那么做。（如果你不调用waveOutReset将不会发生死锁）。 
您可能注意到waveOutOpen提供了一个传递实例数据到回调函数的方法（一个用户定义的指针），我们将使用这个方法传递我们的计数器变量指针。 
另外需要注意的是，既然waveOutProc被在另外的线程中调用，所以会有两个以上的线程操作此计数器变量。为了避免线程冲突，我们需要使用Critical Section对象（我们将使用一个静态变量并命名为waveCriticalSection）。 
下面是waveOutProc函数的代码： 

static void CALLBACK waveOutProc(

HWAVEOUT hWaveOut, 
UINT uMsg, 
DWORD dwInstance, 
DWORD dwParam1,
DWORD dwParam2 
)

{

/*
* pointer to free block counter
*/
int* freeBlockCounter = (int*)dwInstance;
/*
* ignore calls that occur due to openining and closing the
* device.
*/
if(uMsg != WOM_DONE)

return;

EnterCriticalSection(&waveCriticalSection);
(*freeBlockCounter)++;
LeaveCriticalSection(&waveCriticalSection);

}

然后我们需要两个函数分配和释放数据块的内存以及一个命名为writeAudio的新的writeAudioBlock的实现。下面的两个函数allocateBlocks和freeBlocks实现了数据块的分配和释放。allocateBlocks分配了一组数据块（Block），每个数据块的头（Header）是固定长度的。freeBlocks则释放了数据块的内存。如果allocateBlocks失败将导致程序退出。这意味着我们不需要在main函数中检查它的返回值。 

WAVEHDR* allocateBlocks(int size, intcount)
{

unsigned char* buffer;
int i;
WAVEHDR* blocks;
DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
/*
* allocate memory for the entire set in one go
*/
if((buffer = HeapAlloc(

GetProcessHeap(), 
HEAP_ZERO_MEMORY, 
totalBufferSize
)) == NULL) 

{

fprintf(stderr, "Memory allocationerror\n");
ExitProcess(1);

}
/*
* and set up the pointers to each bit
*/
blocks = (WAVEHDR*)buffer;
buffer += sizeof(WAVEHDR) * count;
for(i = 0; i < count; i++) {

blocks[i].dwBufferLength = size;
blocks[i].lpData = buffer;
buffer += size;

}
return blocks;

}
void freeBlocks(WAVEHDR* blockArray)
{

/* 
* and this is why allocateBlocks works the way it does
*/ 
HeapFree(GetProcessHeap(), 0, blockArray);

}

新的writeAudio函数需要能够把那些必须的数据块（Block）写入队列中。基本的逻辑如下： 

While there's data available

If the current free block is prepared

Unprepare it

End If
If there's space in the current free block

Write all the data to the block
Exit the function

Else

Write as much data as is possible tofill the block
Prepare the block
Write it
Decrement the free blocks counter
Subtract however many bytes were written from the data available
Wait for at least one block to become free
Update the current block pointer

End If

End While

这就产生了一个问题：我们如何知道什么时候一个数据块（Block）准备好了而什么时候没有准备好？ 
实际上这是一个相当简单的事情。Windows使用结构体WAVEHDR的dwFlags成员变量来解决这个问题。waveOutPrepareHeader函数的功能中有一项就是设置dwFlags为WHDR_PREPARED。所以我们需要做的就是检查dwFlags中的这个标识位。 
我们将使用结构体WAVEHDR中的dwUser成员变量来管理数据块的计数器。下面是writeAudio函数的代码： 

void writeAudio(HWAVEOUT hWaveOut,LPSTR data, int size)
{

WAVEHDR* current;
int remain;
current = &waveBlocks[waveCurrentBlock];
while(size > 0) {

/* 
* first make sure the header we're going to use is unprepared
*/
if(current->dwFlags & WHDR_PREPARED) 

waveOutUnprepareHeader(hWaveOut,current, sizeof(WAVEHDR));

if(size < (int)(BLOCK_SIZE -current->dwUser)) {

memcpy(current->lpData +current->dwUser, data, size);
current->dwUser += size;
break;

}
remain = BLOCK_SIZE - current->dwUser;
memcpy(current->lpData + current->dwUser, data, remain);
size -= remain;
data += remain;
current->dwBufferLength = BLOCK_SIZE;
waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
EnterCriticalSection(&waveCriticalSection);
waveFreeBlockCount--;
LeaveCriticalSection(&waveCriticalSection);
/*
* wait for a block to become free
*/
while(!waveFreeBlockCount)

Sleep(10);

/*
* point to the next block
*/
waveCurrentBlock++;
waveCurrentBlock %= BLOCK_COUNT;
current = &waveBlocks[waveCurrentBlock];
current->dwUser = 0;

}

}

现在我们有了写音频的新的函数，因为不会再被用到所以你可以扔掉writeAudioBlock函数了。你也可以扔掉loadAudioBlock函数，因为下一部分我们将在main函数中实现新的方法不再需要loadAudioBlock函数了。 

**运行程序**
如果您按照本教程进行到这里，现在应该拥有了一个C文件包含下面的函数： 

·                             **main**

·                             **waveOutProc**

·                             **allocateBlocks**

·                             **freeBlocks**

·                             **writeAudio**

下面让我们完成main函数的新版本以实现把硬盘上的文件流式播放到waveOut设备上吧。下面的代码当然也包括了程序运行所需要的模块变量的声明以及我们已经写出的函数的原型。 

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
/*
* some good values for block size and count
*/
#define BLOCK_SIZE 8192
#define BLOCK_COUNT 20
/*
* function prototypes
*/ 
static void CALLBACK waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
static WAVEHDR* allocateBlocks(int size, int count);
static void freeBlocks(WAVEHDR* blockArray);
static void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size);
/*
* module level variables
*/
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR* waveBlocks;
static volatile int waveFreeBlockCount;
static int waveCurrentBlock;
int main(int argc, char* argv[])
{

HWAVEOUT hWaveOut; /* device handle */
HANDLEhFile;/* file handle */
WAVEFORMATEX wfx; /* look this up in your documentation */
char buffer[1024]; /* intermediate buffer for reading */
int i;
/*
* quick argument check
*/
if(argc != 2) {

fprintf(stderr, "usage: %s\n", argv[0]);
ExitProcess(1);

}
/*
* initialise the module variables
*/ 
waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
waveFreeBlockCount = BLOCK_COUNT;
waveCurrentBlock= 0;
InitializeCriticalSection(&waveCriticalSection);
/*
* try and open the file
*/ 
if((hFile = CreateFile(

argv[1],
GENERIC_READ,
FILE_SHARE_READ,
NULL,
OPEN_EXISTING,
0,
NULL
)) == INVALID_HANDLE_VALUE) 

{

fprintf(stderr, "%s: unable toopen file '%s'\n", argv[0], argv[1]);
ExitProcess(1);

}
/*
* set up the WAVEFORMATEX structure.
*/
wfx.nSamplesPerSec = 44100; /* sample rate */
wfx.wBitsPerSample = 16; /* sample size */
wfx.nChannels= 2; /* channels*/
wfx.cbSize = 0; /* size of _extra_ info */
wfx.wFormatTag = WAVE_FORMAT_PCM;
wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
/*
* try to open the default wave device. WAVE_MAPPER is
* a constant defined in mmsystem.h, it always points to the
* default wave device on the system (some people have 2 or
* more sound cards).
*/
if(waveOutOpen(

&hWaveOut, 
WAVE_MAPPER, 
&wfx, 
(DWORD_PTR)waveOutProc, 
(DWORD_PTR)&waveFreeBlockCount, 
CALLBACK_FUNCTION
) != MMSYSERR_NOERROR) 

{

fprintf(stderr, "%s: unable toopen wave mapper device\n", argv[0]);
ExitProcess(1);

}
/*
* playback loop
*/
while(1) {

DWORD readBytes;
if(!ReadFile(hFile, buffer, sizeof(buffer), &readBytes, NULL))

break;

if(readBytes == 0)

break;

if(readBytes < sizeof(buffer)) {

printf("at end of buffer\n");
memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);
printf("after memcpy\n");

}
writeAudio(hWaveOut, buffer, sizeof(buffer));

}
/*
* wait for all blocks to complete
*/
while(waveFreeBlockCount < BLOCK_COUNT)

Sleep(10);

/*
* unprepare any blocks that are still prepared
*/
for(i = 0; i < waveFreeBlockCount; i++) 

if(waveBlocks[i].dwFlags &WHDR_PREPARED)

waveOutUnprepareHeader(hWaveOut,&waveBlocks[i], sizeof(WAVEHDR));

DeleteCriticalSection(&waveCriticalSection);
freeBlocks(waveBlocks);
waveOutClose(hWaveOut);
CloseHandle(hFile);
return 0;

}

**接下来该做什么？**
接下来要做的事就取决于你自己了。我有几个您可能会有兴趣的建议： 

·                             试着修改原始音频程序让它可以从标准输入读取。您可以直接从命令行控制管道输出声音文件。 

·                             重写读取部分让它直接读取Wave文件（*.wav)而不是RAW文件（原始音频文件）。您会发现这令人吃惊的简单。Wave文件使用结构体WAVEFORMATEX来定义数据格式，您可以在打开声音设备后使用它。关于文件格式的有关信息可以参考WOTSIT's Format网站(http://www.wotsit.org)。 

·                             看看自己能不能创建新的或更好的缓存机制 

·                             试着把这些代码加入某个开源解码器中，如Vorbis解码器或MP3解码器。这样您就可以得到一个属于您自己的媒体播放器了：） 

