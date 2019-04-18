# Tutorial_2：Models and the admin site - weixin_33985507的博客 - CSDN博客
2017年04月23日 16:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
### 一、数据库
Django默认使用的是SQLite，如果想使用别的，可以在项目的`settings`下修改：
> 
ENGINE : 'django.db.backends.sqlite3', 'django.db.backends.postgresql', 'django.db.backends.mysql', or 'django.db.backends.oracle'等等。
如果不使用SQLite的话， USER, PASSWORD和 HOST的信息必须被添加。
### 二、Models
#### 2.1 创建
在app的`models.py`下创建models
写入
```
from django.db import models
class Question(models.Model):
    question_text = models.CharField(max_length=200)
    pub_date = models.DateTimeField('date published')
class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE)
    choice_text = models.CharField(max_length=200)
    votes = models.IntegerField(default=0)
```
使用的是ORM对象关系映射。
#### 2.2 激活
建立了polls这个app后，在`polls/apps`下面有PollsConfig这个类。将其添加到project的settings下面的INSTALLED_APPS。
```
INSTALLED_APPS = [
    'polls.apps.PollsConfig',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
]
```
#### 2.3 迁移
然后迁移migrations。
```
python manage.py makemigrations polls
```
会看见类似：
```
Migrations for 'polls':
  polls/migrations/0001_initial.py:
    - Create model Choice
    - Create model Question
    - Add field question to choice
```
则会在app的migrations下面产生0001_initial.py文件，适用于记录产生的models变化。可以使用`sqlmigrate`查看。
```
python manage.py sqlmigrate polls 0001
```
会看到类似：
```
BEGIN;
--
-- Create model Choice
--
CREATE TABLE "polls_choice" (
    "id" serial NOT NULL PRIMARY KEY,
    "choice_text" varchar(200) NOT NULL,
    "votes" integer NOT NULL
);
--
-- Create model Question
--
CREATE TABLE "polls_question" (
    "id" serial NOT NULL PRIMARY KEY,
    "question_text" varchar(200) NOT NULL,
    "pub_date" timestamp with time zone NOT NULL
);
--
-- Add field question to choice
--
ALTER TABLE "polls_choice" ADD COLUMN "question_id" integer NOT NULL;
ALTER TABLE "polls_choice" ALTER COLUMN "question_id" DROP DEFAULT;
CREATE INDEX "polls_choice_7aa0f6ee" ON "polls_choice" ("question_id");
ALTER TABLE "polls_choice"
  ADD CONSTRAINT "polls_choice_question_id_246c99a640fbbd72_fk_polls_question_id"
    FOREIGN KEY ("question_id")
    REFERENCES "polls_question" ("id")
    DEFERRABLE INITIALLY DEFERRED;
COMMIT;
```
注意：
1、这个sql是依赖于所用的数据库
2、表名被自动生成（查看是怎么样的）
#### 2.4 生成
再次使用`migrate`
```
python manage.py migrate
```
### 三、Shell
进入shell中
```
python manage.py shell
```
开始调试
```
from polls.models import Question, Choice   # Import the model classes we just wrote.
Question.objects.all()
from django.utils import timezone
q = Question(question_text="What's new?", pub_date=timezone.now())
q.save()
q.id
q.question_text
q.pub_date
q.question_text = "What's up?"
q.save()
Question.objects.all()
```
在使用`Question.objects.all()`得到的`<QuerySet [<Question: Question object>]>`结果是没有什么直接用处的。我们可以修改app下的model.py文件。
```
import datetime
from django.db import models
from django.utils.encoding import python_2_unicode_compatible
from django.utils import timezone
@python_2_unicode_compatible
class Question(models.Model):
    question_text = models.CharField(max_length=200)
    pub_date = models.DateTimeField('date published')
    def __str__(self):
        return self.question_text
    def was_published_recently(self):
        return self.pub_date >= timezone.now() - datetime.timedelta(days=1)
@python_2_unicode_compatible
class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE)
    choice_text = models.CharField(max_length=200)
    votes = models.IntegerField(default=0)
    def __str__(self):
        return self.choice_text
```
在原有代码的情况下，添加`__str__`的方法。
上面还添加了一个`was_published_recently`的方法。
```
Question.objects.filter(id=1)
Question.objects.filter(question_text__startswith='What')
from django.utils import timezone
current_year = timezone.now().year
Question.objects.get(pub_date__year=current_year)
Question.objects.get(pk=1)
q = Question.objects.get(pk=1)
q.was_published_recently()
q.choice_set.create(choice_text='Not much', votes=0)
q.choice_set.create(choice_text='The sky', votes=0)
c = q.choice_set.create(choice_text='Just hacking again', votes=0)
q.choice_set.count()
Choice.objects.filter(question__pub_date__year=current_year)
c = q.choice_set.filter(choice_text__startswith='Just hacking')
c.delete()
```
### 四、Admin
创建superuser
```
python manage.py createsuperuser
```
在 [http://127.0.0.1:8000/admin/](https://link.jianshu.com?t=http://127.0.0.1:8000/admin/) 中可以登录。
我们需要告诉admin有polls中的models的接口，在app的admin.py文件下：
```
from django.contrib import admin
from .models import Question, Choice
admin.site.register(Question)
admin.site.register(Choice)
```
