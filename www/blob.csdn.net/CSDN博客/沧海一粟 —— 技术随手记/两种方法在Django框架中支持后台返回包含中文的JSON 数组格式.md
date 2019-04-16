# 两种方法在Django框架中支持后台返回包含中文的JSON/数组格式 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月21日 21:14:00[慢游](https://me.csdn.net/eengel)阅读数：24110








    去年因缘际会，和Django接触了有半年时间。Django有开发快速、语言简洁的特点，对于web前端开发人员来说，其官方的模板系统也是尽量做到逻辑与显示分离的典型例子。

    在开发的期间，也积累了一些这方面的经验。一个印象比较深的例子就是如何支持后台返回包含中文的JSON/数组格式。和想象中不一样，在后台用Python定义的JSON或数组，直接当作response传回前台后，JavaScript是无法识别的（以dojo方式试过）。这儿就列举两种支持方式，希望能够对其他Django使用者有所帮助。

**前台：用DOJO发送AJAX请求，设定返回值为json格式。**

var deferred = dojo.xhrPost({

        url: "/test_json/",

        handleAs: "json",

        load: function(response, ioArgs){

            return response;

        },

        error: function(response, ioArgs){

            return response;

        }

    });


    deferred.addCallback(function(response){

        if(response){

            if(!response.length){//json object            

                alert("id:" + response.id + ",name:" + response.name);

            }else if(response.length > 1){//array object

                alert(response);

            }

        }else{

            console.error("failed");

        }

        return response;

    });
**后台：方法一 将JSON或数组拼装成string类型**

def test_json( request ):

    result = "{'id': 1, 'name': '虫子'}"

    return HttpResponse( result )

    这个方法的优点是简单明了，不需要依赖额外的包。缺点是当json或数组很长很复杂时，拼装起来比较麻烦，而且容易出错。

**后台：方法二 利用django的simplejson工具**

from django.utils import simplejson

from django.core import serializers


class QuerySetEncoder( simplejson.JSONEncoder ):

    """

    Encoding QuerySet into JSON format.

    """

    def default( self, object ):

        try:

            return serializers.serialize( "python", object,

                                          ensure_ascii = False )

        except:

            return simplejson.JSONEncoder.default( self, object )


def test_json( request ):

    result = {'id': 1, 'name': "虫子"}

    result = simplejson.dumps( result, cls = QuerySetEncoder )

    return HttpResponse( result )

    若经常需要从后台返回JSON或数组格式的数据，则这种方法比较好。可以事先将QuerySetEncoder封装在独立的函数中，用的时候比平时多一句simplejson.dumps就可以了。



