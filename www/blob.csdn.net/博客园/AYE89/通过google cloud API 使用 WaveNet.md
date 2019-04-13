
# 通过google cloud API 使用 WaveNet - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[通过google cloud API 使用 WaveNet](https://www.cnblogs.com/eniac1946/p/9042412.html)
|Posted on|2018-05-15 18:24|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9042412)|[收藏](#)
|[Cloud Text-to-Speech](https://cloud.google.com/text-to-speech/)|中使用了WaveNet，用于TTS，页面上有Demo。目前是BETA版
|使用方法
|注册及认证参考：|[Quickstart: Text-to-Speech](https://cloud.google.com/text-to-speech/docs/quickstart)
|安装google clould 的python库
|安装 Google Cloud Text-to-Speech API Python 依赖（Dependencies），参见|[github说明](https://github.com/GoogleCloudPlatform/python-docs-samples/tree/master/texttospeech/cloud-client)
|----其中包括了，安装pip install google-cloud-texttospeech==0.1.0
|为了implicit调用，设置环境变量GOOGLE_APPLICATION_CREDENTIALS到你的API Key（json文件），完成后重启
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515181116722-2106589020.png)
|python脚本：text到mp3
|\# [START tts_synthesize_text]
def synthesize_text(text):
    """Synthesizes speech from the input string of text."""
    from google.cloud import texttospeech
    client = texttospeech.TextToSpeechClient()
    input_text = texttospeech.types.SynthesisInput(text=text)
    \# Note: the voice can also be specified by name.
    \# Names of voices can be retrieved with client.list_voices().
    voice = texttospeech.types.VoiceSelectionParams(
        language_code='en-US',
        ssml_gender=texttospeech.enums.SsmlVoiceGender.FEMALE)
    audio_config = texttospeech.types.AudioConfig(
        audio_encoding=texttospeech.enums.AudioEncoding.MP3)
    response = client.synthesize_speech(input_text, voice, audio_config)
    \# The response's audio_content is binary.
    with open('output.mp3', 'wb') as out:
        out.write(response.audio_content)
        print('Audio content written to file "output.mp3"')
\# [END tts_synthesize_text]
|WaveNet特性
|目前支持的6种voice type
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515181544455-1214438762.png)
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515181442839-2027684729.png)
|参数说明
|https://cloud.google.com/text-to-speech/docs/reference/rest/v1beta1/text/synthesize\#audioconfig
|input_text
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515181749399-951545628.png)
|voice
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515181845480-899736167.png)
|audio_config
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515182033189-1009291943.png)
![](https://images2018.cnblogs.com/blog/1181483/201805/1181483-20180515182321730-762195159.png)








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
