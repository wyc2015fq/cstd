# Writing your first Django app--Django 第一步 - wishchinYang的专栏 - CSDN博客
2014年07月06日 17:47:52[wishchin](https://me.csdn.net/wishchin)阅读数：716
个人分类：[Django](https://blog.csdn.net/wishchin/article/category/2379183)
主页链接：[https://docs.djangoproject.com/en/1.6/intro/tutorial01/](https://docs.djangoproject.com/en/1.6/intro/tutorial01/)
     Let’s learn by example.使用例子进行学习.
    Throughout this tutorial, we’ll walk you through the creation of a basic poll application. 这个例子教会我们写基本的poll程序---投票程序
It’ll consist of two parts:
- A public site that lets people view polls and vote in them.      一个可以让人们看到投票和投票的公共站点。
- An admin site that lets you add, change and delete polls.     一个可以增加、删除、改变投票结果的管理。
     We’ll assume you have [*Django installed*](https://docs.djangoproject.com/en/1.6/intro/install/) already. You can tell Django is installed and which version by running the following command :
     确认Django已经安装：确认版本和是否安装。
python -c "import django; print(django.get_version())"
     If Django is installed, you should see the version of your installation. If itisn’t, you’ll get an error telling “No module named django”.
     如果没有安装会显示：“No module named django”
      This tutorial is written for Django 1.6 and Python 2.x. If the Djangoversion doesn’t match, you can refer to the tutorial for your version of Django by using the version switcher at the bottom right corner of this page,or update Django to the newest
 version. If you are using Python 3.x, be awarethat your code may need to differ from what is in the tutorial and you shouldcontinue using the tutorial only if you know what you are doing with Python3.x.
      此例程是使用Django 1.6 and Python 2.x。注意版本问题。
       See [*How to install Django*](https://docs.djangoproject.com/en/1.6/topics/install/) for advice on how to removeolder versions of Django and install a newer one.
Where to get help:  寻求帮助
      If you’re having trouble going through this tutorial, please post a messageto[*django-users*](https://docs.djangoproject.com/en/1.6/internals/mailing-lists/#django-users-mailing-list)
 or drop by [#django on irc.freenode.net]() to chat with other Django users who mightbe able to help.
## Creating a project  创建一个工程
      If this is your first time using Django, you’ll have to take care of someinitial setup. Namely, you’ll need to auto-generate some code that establishes aDjango[*project*](https://docs.djangoproject.com/en/1.6/glossary/#term-project)
 – a collection of settings for an instance of Django,including database configuration, Django-specific options andapplication-specific settings.
From the command line, cd into a directory where you’d like to store yourcode, then run the following command:
django-admin.py startproject mysite
This will create a mysite directory in your current directory. If it didn’twork, see[*Problems running django-admin.py*](https://docs.djangoproject.com/en/1.6/faq/troubleshooting/#troubleshooting-django-admin-py).
Note
     You’ll need to avoid naming projects after built-in Python or Djangocomponents. In particular, this means you should avoid using names likedjango (which will conflict with Django
 itself) or test (whichconflicts with a built-in Python package).
Where should this code live?
If your background is in plain old PHP (with no use of modern frameworks),you’re probably used to putting code under the Web server’s document root(in a place such as/var/www). With Django, you don’t
 do that. It’snot a good idea to put any of this Python code within your Web server’sdocument root, because it risks the possibility that people may be ableto view your code over the Web. That’s not good for security.
Put your code in some directory **outside** of the document root, such as/home/mycode.
Let’s look at what [startproject](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-startproject) created:
mysite/
    manage.py
    mysite/
        __init__.py
        settings.py
        urls.py
        wsgi.py
Doesn’t match what you see?
The default project layout recently changed. If you’re seeing a “flat”layout (with no innermysite/ directory), you’re probably usinga version of Django that doesn’t match this tutorial
 version. You’llwant to either switch to the older tutorial or the newer Django version.
These files are:
- The outer mysite/ root directory is just a container for yourproject. Its name doesn’t matter to Django; you can rename it to anythingyou like.
- manage.py: A command-line utility that lets you interact with thisDjango project in various ways. You can read all the details aboutmanage.py
 in [*django-admin.py and manage.py*](https://docs.djangoproject.com/en/1.6/ref/django-admin/).
- The inner mysite/ directory is the actual Python package for yourproject. Its name is the Python package name you’ll need to use to importanything inside it (e.g.mysite.urls).
- mysite/__init__.py: An empty file that tells Python that thisdirectory should be considered a Python package. (Read[more
 aboutpackages](http://docs.python.org/tutorial/modules.html#packages) in the official Python docs if you’re a Python beginner.)
- mysite/settings.py: Settings/configuration for this Djangoproject.[*Django settings*](https://docs.djangoproject.com/en/1.6/topics/settings/) will tell you all about how settingswork.
- mysite/urls.py: The URL declarations for this Django project; a“table of contents” of your Django-powered site. You can read more aboutURLs in[*URL dispatcher*](https://docs.djangoproject.com/en/1.6/topics/http/urls/).
- mysite/wsgi.py: An entry-point for WSGI-compatible web servers toserve your project. See[*How to deploy with WSGI*](https://docs.djangoproject.com/en/1.6/howto/deployment/wsgi/) for more details.
### The development server
Let’s verify this worked. Change into the outer mysite directory, ifyou haven’t already, and run the commandpythonmanage.pyrunserver. You’llsee the following output on the command line:
Validating models...
0 errors found
July 06, 2014 - 15:50:53
Django version 1.6, using settings 'mysite.settings'
Starting development server at [http://127.0.0.1:8000/](http://127.0.0.1:8000/)
Quit the server with CONTROL-C.
You’ve started the Django development server, a lightweight Web server writtenpurely in Python. We’ve included this with Django so you can develop thingsrapidly, without having to deal with configuring a production server – such asApache – until you’re ready
 for production.
Now’s a good time to note: **Don’t** use this server in anything resembling aproduction environment. It’s intended only for use while developing. (We’re inthe business of making Web frameworks, not Web servers.)
Now that the server’s running, visit [http://127.0.0.1:8000/](http://127.0.0.1:8000/) with your Webbrowser. You’ll see a “Welcome to Django” page, in pleasant, light-blue pastel.It worked!
Changing the port
By default, the [runserver](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-runserver) command starts the development serveron the internal IP at port 8000.
If you want to change the server’s port, passit as a command-line argument. For instance, this command starts the serveron port 8080:
$ python manage.py runserver 8080
If you want to change the server’s IP, pass it along with the port. So tolisten on all public IPs (useful if you want to show off your work on othercomputers), use:
$ python manage.py runserver 0.0.0.0:8000
Full docs for the development server can be found in the[runserver](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-runserver)
 reference.
Automatic reloading of [runserver](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-runserver)
The development server automatically reloads Python code for each requestas needed. You don’t need to restart the server for code changes to takeeffect. However, some actions like adding files or compiling translationfiles don’t trigger a restart,
 so you’ll have to restart the server inthese cases.
### Database setup
Now, edit mysite/settings.py. It’s a normal Python module withmodule-level variables representing Django settings.
By default, the configuration uses SQLite. If you’re new to databases, oryou’re just interested in trying Django, this is the easiest choice. SQLite isincluded in Python, so you won’t need to install anything else to support yourdatabase.
If you wish to use another database, install the appropriate [*databasebindings*](https://docs.djangoproject.com/en/1.6/topics/install/#database-installation), and change the following keys in the[DATABASES](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-DATABASES)'default'
 item to match your database connectionsettings:
- [ENGINE](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-DATABASE-ENGINE) – Either'django.db.backends.sqlite3','django.db.backends.postgresql_psycopg2','django.db.backends.mysql',
 or'django.db.backends.oracle'. Other backends are[alsoavailable](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-DATABASE-ENGINE).
- [NAME](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-NAME) – The name of your database.
 If you’re using SQLite, thedatabase will be a file on your computer; in that case,
[NAME](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-NAME)should be the full absolute path, including filename, of that file. Thedefault value,os.path.join(BASE_DIR,'db.sqlite3'), will store the filein your project directory.
If you are not using SQLite as your database, additional settings such as [USER](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-USER),[PASSWORD](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-PASSWORD),[HOST](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-HOST)
 must be added.For more details, see the reference documentation for[DATABASES](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-DATABASES).
Note
If you’re using PostgreSQL or MySQL, make sure you’ve created a database bythis point. Do that with “CREATEDATABASEdatabase_name;” within yourdatabase’s interactive prompt.
If you’re using SQLite, you don’t need to create anything beforehand - thedatabase file will be created automatically when it is needed.
While you’re editing mysite/settings.py, set[TIME_ZONE](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-TIME_ZONE)
 toyour time zone.
Also, note the [INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS) setting at the top of the file. Thatholds the names of all Django applications that are activated in this Djangoinstance. Apps can be used in multiple projects,
 and you can package anddistribute them for use by others in their projects.
By default, [INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS) contains the following apps, all of whichcome with Django:
- [django.contrib.admin](https://docs.djangoproject.com/en/1.6/ref/contrib/admin/#module-django.contrib.admin)
 – The admin site. You’ll use it in [*part 2of this tutorial*](https://docs.djangoproject.com/en/1.6/intro/tutorial02/).
- [django.contrib.auth](https://docs.djangoproject.com/en/1.6/topics/auth/#module-django.contrib.auth)
 – An authentication system.
- [django.contrib.contenttypes](https://docs.djangoproject.com/en/1.6/ref/contrib/contenttypes/#module-django.contrib.contenttypes)
 – A framework for content types.
- [django.contrib.sessions](https://docs.djangoproject.com/en/1.6/topics/http/sessions/#module-django.contrib.sessions)
 – A session framework.
- [django.contrib.messages](https://docs.djangoproject.com/en/1.6/ref/contrib/messages/#module-django.contrib.messages)
 – A messaging framework.
- [django.contrib.staticfiles](https://docs.djangoproject.com/en/1.6/ref/contrib/staticfiles/#module-django.contrib.staticfiles)
 – A framework for managingstatic files.
These applications are included by default as a convenience for the common case.
Some of these applications makes use of at least one database table, though,so we need to create the tables in the database before we can use them. To dothat, run the following command:
$ python manage.py syncdb
The [syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb) command looks at the[INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS)
 settingand creates any necessary database tables according to the database settingsin yourmysite/settings.py file. You’ll see a message for eachdatabase table it creates, and you’ll get a prompt
 asking you if you’d like tocreate a superuser account for the authentication system. Go ahead and dothat.
If you’re interested, run the command-line client for your database and type\dt (PostgreSQL),SHOWTABLES; (MySQL), or.schema (SQLite) todisplay the tables Django created.
For the minimalists
Like we said above, the default applications are included for the commoncase, but not everybody needs them. If you don’t need any or all of them,feel free to comment-out or delete the appropriate line(s) from[INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS)
 before running [syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb). The[syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb)
 command will only create tables for apps in[INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS).
## Creating models
Now that your environment – a “project” – is set up, you’re set to startdoing work.
Each application you write in Django consists of a Python package that followsa certain convention. Django comes with a utility that automatically generatesthe basic directory structure of an app, so you can focus on writing coderather than creating directories.
Projects vs. apps
What’s the difference between a project and an app? An app is a Webapplication that does something – e.g., a Weblog system, a database ofpublic records or a simple poll app. A project is a collection ofconfiguration and apps for a particular
 Web site. A project can containmultiple apps. An app can be in multiple projects.
Your apps can live anywhere on your [Python path](http://docs.python.org/tutorial/modules.html#the-module-search-path). In this tutorial, we’llcreate our poll app right next to your manage.py file so that it can beimported as its own top-level module, rather than a submodule ofmysite.
To create your app, make sure you’re in the same directory as manage.pyand type this command:
$ python manage.py startapp polls
That’ll create a directory polls, which is laid out like this:
polls/
    __init__.py
    admin.py
    models.py
    tests.py
    views.py
This directory structure will house the poll application.
The first step in writing a database Web app in Django is to define your models– essentially, your database layout, with additional metadata.
Philosophy
A model is the single, definitive source of data about your data. It containsthe essential fields and behaviors of the data you’re storing. Django followsthe[*DRY Principle*](https://docs.djangoproject.com/en/1.6/misc/design-philosophies/#dry). The goal is to define your data model in oneplace and automatically derive things from it.
In our simple poll app, we’ll create two models: Poll and Choice.APoll has a question and a publication date. AChoice
 has two fields:the text of the choice and a vote tally. EachChoice is associated with aPoll.
These concepts are represented by simple Python classes. Edit thepolls/models.py file so it looks like this:
fromdjango.dbimportmodelsclassPoll(models.Model):question=models.CharField(max_length=)pub_date=models.DateTimeField('date published')classChoice(models.Model):poll=models.ForeignKey(Poll)choice_text=models.CharField(max_length=)votes=models.IntegerField(default=)
The code is straightforward. Each model is represented by a class thatsubclasses[django.db.models.Model](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model).
 Each model has a number of classvariables, each of which represents a database field in the model.
Each field is represented by an instance of a [Field](https://docs.djangoproject.com/en/1.6/howto/custom-model-fields/#django.db.models.Field)class – e.g.,[CharField](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.CharField)
 for character fields and[DateTimeField](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.DateTimeField)
 for datetimes. This tells Django whattype of data each field holds.
The name of each [Field](https://docs.djangoproject.com/en/1.6/howto/custom-model-fields/#django.db.models.Field) instance (e.g.question orpub_date) is the field’s name,
 in machine-friendly format. You’ll use thisvalue in your Python code, and your database will use it as the column name.
You can use an optional first positional argument to a[Field](https://docs.djangoproject.com/en/1.6/howto/custom-model-fields/#django.db.models.Field)
 to designate a human-readable name. That’s usedin a couple of introspective parts of Django, and it doubles as documentation.If this field isn’t provided, Django will use the machine-readable name. In thisexample, we’ve only defined a human-readable name forPoll.pub_date.
 For allother fields in this model, the field’s machine-readable name will suffice asits human-readable name.
Some [Field](https://docs.djangoproject.com/en/1.6/howto/custom-model-fields/#django.db.models.Field) classes have required arguments.[CharField](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.CharField),
 for example, requires that you give it a[max_length](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.CharField.max_length).
 That’s used not only in thedatabase schema, but in validation, as we’ll soon see.
A [Field](https://docs.djangoproject.com/en/1.6/howto/custom-model-fields/#django.db.models.Field) can also have various optional arguments; inthis case, we’ve set the[default](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.Field.default)
 value ofvotes to 0.
Finally, note a relationship is defined, using[ForeignKey](https://docs.djangoproject.com/en/1.6/ref/models/fields/#django.db.models.ForeignKey).
 That tells Django each Choice is relatedto a singlePoll. Django supports all the common database relationships:many-to-ones, many-to-manys
 and one-to-ones.
## Activating models
That small bit of model code gives Django a lot of information. With it, Djangois able to:
- Create a database schema (CREATETABLE statements) for this app.
- Create a Python database-access API for accessing Poll and Choice objects.
But first we need to tell our project that the polls app is installed.
Philosophy
Django apps are “pluggable”: You can use an app in multiple projects, andyou can distribute apps, because they don’t have to be tied to a givenDjango installation.
Edit the mysite/settings.py file again, and change the[INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS)
 setting to include the string 'polls'. So it’lllook like this:
INSTALLED_APPS=('django.contrib.admin','django.contrib.auth','django.contrib.contenttypes','django.contrib.sessions','django.contrib.messages','django.contrib.staticfiles','polls',)
Now Django knows to include the polls app. Let’s run another command:
$ python manage.py sql polls
You should see something similar to the following (the CREATETABLE SQLstatements for the polls app):
BEGIN;CREATETABLE"polls_poll"("id"integerNOTNULLPRIMARYKEY,"question"varchar()NOTNULL,"pub_date"datetimeNOTNULL);CREATETABLE"polls_choice"("id"integerNOTNULLPRIMARYKEY,"poll_id"integerNOTNULLREFERENCES"polls_poll"("id"),"choice_text"varchar()NOTNULL,"votes"integerNOTNULL);COMMIT;
Note the following:
- The exact output will vary depending on the database you are using. Theexample above is generated for SQLite.
- Table names are automatically generated by combining the name of the app(polls) and the lowercase name of the model –poll andchoice.
 (You can override this behavior.)
- Primary keys (IDs) are added automatically. (You can override this, too.)
- By convention, Django appends "_id" to the foreign key field name.(Yes, you can override this, as well.)
- The foreign key relationship is made explicit by a REFERENCESstatement.
- It’s tailored to the database you’re using, so database-specific fieldtypes such asauto_increment (MySQL),serial (PostgreSQL), orintegerprimarykey (SQLite) are handled for you automatically. Samegoes for quoting of field names – e.g., using double quotes or singlequotes.
- The [sql](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sql) command doesn’t actually run the SQL in your database -it just prints it to the screen so that you can see what SQL Django thinksis required. If you wanted to, you could
 copy and paste this SQL into yourdatabase prompt. However, as we will see shortly, Django provides aneasier way of committing the SQL to the database.
If you’re interested, also run the following commands:
- [pythonmanage.pyvalidate](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-validate) – Checks for any errorsin the construction of your models.
- [pythonmanage.pysqlcustompolls](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlcustom) – Outputs any[*custom SQL statements*](https://docs.djangoproject.com/en/1.6/howto/initial-data/#initial-sql)
 (such as table modifications orconstraints) that are defined for the application.
- [pythonmanage.pysqlclearpolls](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlclear) – Outputs thenecessary
DROPTABLE statements for this app, according to whichtables already exist in your database (if any).
- [pythonmanage.pysqlindexespolls](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlindexes) – Outputs theCREATEINDEX statements for this app.
- [pythonmanage.pysqlallpolls](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlall) – A combination of allthe SQL from the
[sql](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sql),[sqlcustom](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlcustom),
 and[sqlindexes](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlindexes) commands.
Looking at the output of those commands can help you understand what’s actuallyhappening under the hood.
Now, run [syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb) again to create those model tables in your database:
$ python manage.py syncdb
The [syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb) command runs the SQL from[sqlall](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-sqlall)
 on yourdatabase for all apps in[INSTALLED_APPS](https://docs.djangoproject.com/en/1.6/ref/settings/#std:setting-INSTALLED_APPS)
 that don’t already exist inyour database. This creates all the tables, initial data and indexes for anyapps you’ve added to your project since the last time you ran syncdb.[syncdb](https://docs.djangoproject.com/en/1.6/ref/django-admin/#django-admin-syncdb)
 can be called as often as you like, and it will only evercreate the tables that don’t exist.
Read the [*django-admin.py documentation*](https://docs.djangoproject.com/en/1.6/ref/django-admin/) for fullinformation on what the manage.py utility can do.
## Playing with the API
Now, let’s hop into the interactive Python shell and play around with the freeAPI Django gives you. To invoke the Python shell, use this command:
$ python manage.py shell
We’re using this instead of simply typing “python”, because manage.pysets the DJANGO_SETTINGS_MODULE environment variable, which gives Djangothe Python import path to yourmysite/settings.py
 file.
Bypassing manage.py
If you’d rather not use manage.py, no problem. Just set theDJANGO_SETTINGS_MODULE environment variable tomysite.settings
 andrunpython from the same directorymanage.py is in (or ensurethat directory is on the Python path, so thatimportmysite works).
For more information on all of this, see the [*django-admin.pydocumentation*](https://docs.djangoproject.com/en/1.6/ref/django-admin/).
Once you’re in the shell, explore the [*database API*](https://docs.djangoproject.com/en/1.6/topics/db/queries/):
>>> frompolls.modelsimportPoll,Choice# Import the model classes we just wrote.# No polls are in the system yet.>>> Poll.objects.all()[]# Create a new Poll.# Support for time zones is enabled in the default settings file, so# Django expects a datetime with tzinfo for pub_date. Use timezone.now()# instead of datetime.datetime.now() and it will do the right thing.>>> fromdjango.utilsimporttimezone>>> p=Poll(question="What's new?",pub_date=timezone.now())# Save the object into the database. You have to call save() explicitly.>>> p.save()# Now it has an ID. Note that this might say "1L" instead of "1", depending# on which database you're using. That's no biggie; it just means your# database backend prefers to return integers as Python long integer# objects.>>> p.id1# Access database columns via Python attributes.>>> p.question"What's new?">>> p.pub_datedatetime.datetime(2012, 2, 26, 13, 0, 0, 775217, tzinfo=<UTC>)# Change values by changing the attributes, then calling save().>>> p.question="What's up?">>> p.save()# objects.all() displays all the polls in the database.>>> Poll.objects.all()[<Poll: Poll object>]
Wait a minute. <Poll:
Pollobject> is, utterly, an unhelpful representationof this object. Let’s fix that by editing the polls model (in thepolls/models.py file) and adding a[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__)
 method to both Poll andChoice. On Python 3, simply replace__unicode__ by
__str__ in thefollowing example:
fromdjango.dbimportmodelsclassPoll(models.Model):# ...def__unicode__(self):# Python 3: def __str__(self):returnself.questionclassChoice(models.Model):# ...def__unicode__(self):# Python 3: def __str__(self):returnself.choice_text
It’s important to add [__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__) methods (or[__str__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__str__)
 on Python 3) to your models, not onlyfor your own sanity when dealing with the interactive prompt, but also becauseobjects’ representations are used throughout Django’s automatically-generatedadmin.
__unicode__ or __str__?
On Python 3, things are simpler, just use[__str__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__str__)
 and forget about[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__).
If you’re familiar with Python 2, you might be in the habit of adding[__str__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__str__)
 methods to your classes, not[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__)
 methods. We use[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__)
 here because Django models dealwith Unicode by default. All data stored in your database is converted toUnicode when it’s returned.
Django models have a default [__str__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__str__) methodthat calls[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__)
 and converts theresult to a UTF-8 bytestring. This means thatunicode(p) will return aUnicode string, andstr(p) will return a normal string,
 with charactersencoded as UTF-8.
If all of this is gibberish to you, just remember to add[__unicode__()](https://docs.djangoproject.com/en/1.6/ref/models/instances/#django.db.models.Model.__unicode__)
 methods to your models. With anyluck, things should Just Work for you.
Note these are normal Python methods. Let’s add a custom method, just fordemonstration:
importdatetimefromdjango.utilsimporttimezone# ...classPoll(models.Model):# ...defwas_published_recently(self):returnself.pub_date>=timezone.now()-datetime.timedelta(days=)
Note the addition of importdatetime and
fromdjango.utilsimporttimezone, to reference Python’s standard[datetime](http://docs.python.org/2.7/library/datetime.html#module-datetime)
 module and Django’stime-zone-related utilities in[django.utils.timezone](https://docs.djangoproject.com/en/1.6/ref/utils/#module-django.utils.timezone),
 respectively. Ifyou aren’t familiar with time zone handling in Python, you can learn more inthe[*time zone support docs*](https://docs.djangoproject.com/en/1.6/topics/i18n/timezones/).
Save these changes and start a new Python interactive shell by runningpythonmanage.pyshell again:
>>> frompolls.modelsimportPoll,Choice# Make sure our __unicode__() addition worked.>>> Poll.objects.all()[<Poll: What's up?>]# Django provides a rich database lookup API that's entirely driven by# keyword arguments.>>> Poll.objects.filter(id=)[<Poll: What's up?>]>>> Poll.objects.filter(question__startswith='What')[<Poll: What's up?>]# Get the poll that was published this year.>>> fromdjango.utilsimporttimezone>>> current_year=timezone.now().year>>> Poll.objects.get(pub_date__year=current_year)<Poll: What's up?># Request an ID that doesn't exist, this will raise an exception.>>> Poll.objects.get(id=)Traceback (most recent call last):...DoesNotExist: Poll matching query does not exist.# Lookup by a primary key is the most common case, so Django provides a# shortcut for primary-key exact lookups.# The following is identical to Poll.objects.get(id=1).>>> Poll.objects.get(pk=)<Poll: What's up?># Make sure our custom method worked.>>> p=Poll.objects.get(pk=)>>> p.was_published_recently()True# Give the Poll a couple of Choices. The create call constructs a new# Choice object, does the INSERT statement, adds the choice to the set# of available choices and returns the new Choice object. Django creates# a set to hold the "other side" of a ForeignKey relation# (e.g. a poll's choices) which can be accessed via the API.>>> p=Poll.objects.get(pk=)# Display any choices from the related object set -- none so far.>>> p.choice_set.all()[]# Create three choices.>>> p.choice_set.create(choice_text='Not much',votes=)<Choice: Not much>>>> p.choice_set.create(choice_text='The sky',votes=)<Choice: The sky>>>> c=p.choice_set.create(choice_text='Just hacking again',votes=)# Choice objects have API access to their related Poll objects.>>> c.poll<Poll: What's up?># And vice versa: Poll objects get access to Choice objects.>>> p.choice_set.all()[<Choice: Not much>, <Choice: The sky>, <Choice: Just hacking again>]>>> p.choice_set.count()3# The API automatically follows relationships as far as you need.# Use double underscores to separate relationships.# This works as many levels deep as you want; there's no limit.# Find all Choices for any poll whose pub_date is in this year# (reusing the 'current_year' variable we created above).>>> Choice.objects.filter(poll__pub_date__year=current_year)[<Choice: Not much>, <Choice: The sky>, <Choice: Just hacking again>]# Let's delete one of the choices. Use delete() for that.>>> c=p.choice_set.filter(choice_text__startswith='Just hacking')>>> c.delete()
For more information on model relations, see [*Accessing related objects*](https://docs.djangoproject.com/en/1.6/ref/models/relations/). For more on how to use double underscores to performfield lookups via the API, see[*Field lookups*](https://docs.djangoproject.com/en/1.6/topics/db/queries/#field-lookups-intro). Forfull details on the database API, see our [*Database API reference*](https://docs.djangoproject.com/en/1.6/topics/db/queries/).
When you’re comfortable with the API, read [*part 2 of this tutorial*](https://docs.djangoproject.com/en/1.6/intro/tutorial02/) to get Django’s automatic admin working.
慢慢来....................
