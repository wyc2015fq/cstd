
# Flask jieba分词的完整api，swaggerUi展示切词文档 - 追求卓越,做到专业 - CSDN博客


2018年06月19日 17:44:14[Waldenz](https://me.csdn.net/enter89)阅读数：378


需要安装的包  flask, flask-swagger, flask-swagger-ui, jieba
Flask+swagger ui  直接上源码
SegmentAPI.py源码。
```python
# pip install flask-swagger-ui
# pip install flask_swagger
from flask import Flask, jsonify, abort, request
from flask_swagger import swagger
import jiebahelper
from flask_swagger_ui import get_swaggerui_blueprint
app = Flask(__name__)
SWAGGER_URL = '/api/docs'  # URL for exposing Swagger UI (without trailing '/') 
API_URL = '/swagger'
# Call factory function to create our blueprint
swaggerui_blueprint = get_swaggerui_blueprint(
    # Swagger UI static files will be mapped to '{SWAGGER_URL}/dist/'
    SWAGGER_URL,
    API_URL,
    config={  # Swagger UI config overrides
        'app_name': "Jiebao Application"
    }
)
# Register blueprint at URL
# (URL must match the one given to factory function above)
app.register_blueprint(swaggerui_blueprint, url_prefix=SWAGGER_URL)
#  https://github.com/OAI/OpenAPI-Specification/blob/master/versions/2.0.md#parameter-object

@app.route("/swagger")
def spec():
    swag = swagger(app)
    swag['info']['version'] = "1.0"
    swag['info']['title'] = "Segment API"
    return jsonify(swag)

@app.route('/')
def index():
    return 'Jiebao Segment API by Python.'

from flask import make_response

@app.errorhandler(404)
def not_found(error):
    # 当我们请求  # 2 id的资源时，可以获取，但是当我们请求#3的资源时返回了404错误。并且返回了一段奇怪的HTML错误，而不是我们期望的JSON，这是因为Flask产生了默认的404响应。客户端需要收到的都是JSON的响应，因此我们需要改进404错误处理：
    return make_response(jsonify({'error': 'Not found'}), 404)

@app.errorhandler(400)
def para_error(error):
    # 数据错误
    return make_response(jsonify({'error': 'Parameter Error'}), 400)

@app.route('/segment', methods=['POST'])
def segment():
    '''
        切词。不带词性，去停词
        ---
        tags:
          - segment
        parameters:
          - in: body
            name: body
            description: 内容
            required: true
            schema:
                type: string
     '''
    a = request.data.strip()
    if a == '':
        abort(400)
    ret = jiebahelper.dosegment(a)
    return ret

@app.route('/segmentpos', methods=['POST'])
def segmentpos():
    '''
        切词。带词性，去停词
        ---
        tags:
          - segment
        parameters:
          - in: body
            name: body
            description: 内容
            required: true
            schema:
                type: string
     '''
    a = request.data.strip()
    if a == '':
        abort(400)
    ret = jiebahelper.dosegment_with_pos(a)
    return ret

@app.route('/segmentall', methods=['POST'])
def segmentall():
    '''
        切词。带词性，不去停词
        ---
        tags:
            - segment
        parameters:
          - in: body
            name: body
            description: 内容
            required: true
            schema:
                type: string    
    '''
    a = request.data.strip()
    if not a:
        abort(400)
    ret = jiebahelper.dosegment_all(a)
    return ret

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
```
命令直接执行 python SegmentAPI.py启动站点
![](https://img-blog.csdn.net/20180619173917302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后再浏览器中直接输入 127.0.0.1:5000直接浏览
![](https://img-blog.csdn.net/20180619175144321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
直接在swagger中测试接口，结果如下图
![](https://img-blog.csdn.net/20180619175255524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
分词使用jieba，jiebahelp.py源码
```python
# jiebahelper 结巴分词封装模块
import jieba
import jieba.analyse
import jieba.posseg
import re
import datetime
#加载自定义词典
jieba.load_userdict('userdict.txt')
# 至少包含一个汉字的正则表达式
contains_hanzi_pattern = re.compile(r'.*[\u4e00-\u9fa5]+.*')
# 创建停用词list
def stopwordslist(filepath):
    stopwords = [line.strip() for line in open(filepath, 'r', encoding='utf-8').readlines()]
    return stopwords
stopwords = stopwordslist('./stopwordshit.txt')  # 这里加载停用词的路径
emptyList = ["\t", "\r\n", "\r", "\n"]
# 对句子进行分词
def dosegment(sentence, must_contains_hanzi=False):
    '''
    分词
    :param sentence:输入字符
    :param must_contains_hanzi:是否必须包含汉字，默认False,即全部切词。Ture,即不返回词中没有汉字的词语
    :return:
    '''
    start = datetime.datetime.now()
    sentence_seged = jieba.cut(sentence.strip())
    step1 = datetime.datetime.now()
    # outstr = ''
    # for word in sentence_seged:
    #     if word not in stopwords and word not in emptyList:
    #         # 带数字或者只返回不是数字的字符
    #         if not must_contains_hanzi or contains_hanzi_pattern.match(word):
    #             outstr += word+" "
    outstr=" ".join(list(filter(lambda x:(x not in stopwords and x not in emptyList and (not must_contains_hanzi or contains_hanzi_pattern.match(x))),sentence_seged)))
    #outstr=" ".join(sentence_seged)
    step2 = datetime.datetime.now()
    print("cut:{}us微秒 filter:{}us".format((step1-start).microseconds,(step2-step1).microseconds))
    return outstr
# 带词性标注，对句子进行分词
def dosegment_with_pos(sentence, must_contains_hanzi=False):
    '''
    分词
    :param sentence:输入字符
    :param must_contains_hanzi:是否必须包含汉字，默认False,即全部切词。Ture,即不返回词中没有汉字的词语
    :return:
    '''
    start = datetime.datetime.now()
    sentence_seged = jieba.posseg.cut(sentence.strip())
    step1 = datetime.datetime.now()
    outstr = ''
    for x in sentence_seged:
        # 是否必须包含汉字
        if x.word not in stopwords and x.word not in emptyList and (not must_contains_hanzi or
                                                                    contains_hanzi_pattern.match(x.word)):
            outstr+="{}/{},".format(x.word,x.flag)
    step2 = datetime.datetime.now()
    print("poscut:{}us微秒 filter:{}us".format((step1-start).microseconds,(step2-step1).microseconds))
    return outstr
def dosegment_all(sentence):
    '''
    带词性标注，对句子进行分词，不排除停词等
    :param sentence:输入字符
    :return:
    '''
    sentence_seged = jieba.posseg.cut(sentence.strip())
    outstr = ''
    for x in sentence_seged:
        outstr+="{}/{},".format(x.word,x.flag)
    return outstr
# 提取关键词
def extract_tags(content,topk):
    content = content.strip()
    tags=jieba.analyse.extract_tags(content, topK=topk)
    return ','.join(tags)
```
完整源码下载页：[下载](https://download.csdn.net/download/enter89/10486662)

