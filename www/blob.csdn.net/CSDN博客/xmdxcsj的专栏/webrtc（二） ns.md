# webrtc（二） ns - xmdxcsj的专栏 - CSDN博客





2015年12月03日 21:46:51[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3232








版本对应的commit

```
commit 99f8566524b3764bf84b7f4cbd8232bdf6c142a5
Author: kjellander <kjellander@webrtc.org>
Date:   Sun Nov 29 20:50:44 2015 -0800
```

由于缺少说明文档和参考文献，这里简单罗列一下使用的例子

```cpp
int nsTest(char *filename, char *outfilename, int mode)
{
    //alloc
    FILE *infp=fopen(filename,"r");
    int nBands = 1;
    int frameSize = 160;//10ms对应于160个short
    short *temp = (short*)malloc(frameSize*sizeof(short));
    float **pData = (float**)malloc(nBands*sizeof(float*));
    pData[0] = (float*)malloc(frameSize*sizeof(float));

    float **pOutData = (float**)malloc(nBands*sizeof(float*));
    pOutData[0] = (float*)malloc(frameSize*sizeof(float));
    int offset = 0;

    //init
    NsHandle *handle = 0;
    int ret = 0;
    handle=WebRtcNs_Create();
    ret= WebRtcNs_Init(handle,16000);
    if(ret==-1){
        printf("%s","init failure!");
        return -1;
    }
    ret = WebRtcNs_set_policy(handle,2);

    //process
    FILE *outfp = fopen(outfilename,"w");
    int len = frameSize;
    while(len > 0)
    {
        memset(temp, 0, frameSize*sizeof(short));
        len = fread(temp, sizeof(short), frameSize, infp);
        int i;
        for(i = 0; i < frameSize; i++)
        {
            pData[0][i] = (float)temp[i];
            //fprintf(stdout, "ns:%d\n", temp[i]);
        }
        WebRtcNs_AnalyzeCore((NoiseSuppressionC*)handle,pData[0]);
        WebRtcNs_Process(handle,pData,nBands,pOutData);

        for(i = 0; i < frameSize; i++)
        {
            temp[i] = (short)pOutData[0][i];
            //fprintf(stderr, "ns:%d\n", temp[i]);
        }
        //write file
        len = fwrite(temp,sizeof(short),len,outfp);
    }
    fclose(infp);
    fclose(outfp);

    WebRtcNs_Free(handle);
    free(temp);
    free(pData[0]);
    free(pData);
    free(pOutData[0]);
    free(pOutData);
}
```

代码使用流程参考webrtc/modules/audio_processing/noise_suppression_impl.cc



