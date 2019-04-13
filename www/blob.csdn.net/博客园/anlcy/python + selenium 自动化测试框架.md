
# python + selenium 自动化测试框架 - anlcy - 博客园






# [python + selenium 自动化测试框架](https://www.cnblogs.com/camilla/p/7804175.html)
分享一个网站自动化测试框架
结构如下：
test_project
|--logs
|---pages
|---register_page.py
|      |---base_page.py
|---test_case
|---web_test_case
|---test_register.py
|      |---base.py
|---utilities
|---conn_db.py
|---conn_redis.py
|---operation.py
|---settings.py
|---settings_local.py
|---requirements.txt
一、requirements.txt
这个文件里面是需要安装的包，包名+版本号
1 nose==1.3.72 redis==2.10.53 selenium==3.4.34 PyMySQL==0.7.115 requests==2.18.1
在cmd 中直接运行 pip install -r requirements.txt ，自动安装所有需要的包
二、settings.py
项目配置文件，用于记录测试项目域名、全局等待时间、数据库地址、redis地址、log的存放目录等
\# env config
ENV = 'test'
\# test url test config
WEB_TEST_BASE_URL = "http://www.abc.com"
\# global waiting time config
WAIT_TIME = 10
\# redis config
REDIS_HOST = ''
REDIS_PORT = ''
\# drive config
GECKODRIVER_PATH = 'geckodriver'
\# mysql config
DB_HOST = ''
DB_PORT = ''
DB_USER = ''
DB_PASSWORD = ''
\# local debugging config
import os
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
SNAPSHOT_DIRECTORY = os.path.join(BASE_DIR, 'logs')
SETTING_LOCAL_DIR = os.path.join(BASE_DIR, "settings_local.py")
if os.path.exists(SETTING_LOCAL_DIR):
execfile(SETTING_LOCAL_DIR)
三、base,py
test_case目录下所有的case需要继承BaseSeleniumTestCase()这个类，此处需要说明一下，通过设置settings中ENV的值修改启动的浏览器，当ENV='dev'的时候，启动的是Firefox，否则启动PhantomJS。
import logging
from unittest import TestCase
from selenium import webdriver
import settings
logger = logging.getLogger(__name__)

class BaseSeleniumTestCase(TestCase):
def get_web_driver(self):
driver = webdriver.Firefox(executable_path=settings.GECKODRIVER_PATH) if settings.ENV == "dev" else webdriver.PhantomJS()
driver.maximize_window()
return driver
def setUp(self):
self.selenium = self.get_web_driver()
def tearDown(self):
self.selenium.quit()
四、base_page.py
页面基类，主要是定位、判断元素等，所有pages目录下的各个页面的page文件需要继承BasePage()类。
fail_on_screenshot() 修饰器，当定位元素或者判断元素的方法报出 TimeoutException、NoSuchElementException、InvalidElementStateException 错误时，对当时的页面进行截图，并且保存在项目logs目录中，名称为当前时间。
\# coding=UTF-8
import logging,os,settings
from datetime import datetime
from selenium.common.exceptions import NoSuchElementException, TimeoutException, InvalidElementStateException
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as expected
from selenium.webdriver.support.wait import WebDriverWait
from settings import WAIT_TIME
logger = logging.getLogger(__name__)

def fail_on_screenshot(function):
def get_snapshot_directory():
if not os.path.exists(settings.SNAPSHOT_DIRECTORY):
os.mkdir(settings.SNAPSHOT_DIRECTORY)
return settings.SNAPSHOT_DIRECTORY
def get_current_time_str():
return datetime.strftime(datetime.now(), "%Y%m%d%H%M%S%f")
def wrapper(*args, **kwargs):
instance, selector = args[0], args[1]
try:
return function(*args, **kwargs)
except (TimeoutException, NoSuchElementException, InvalidElementStateException) as ex:
logger.error("Could not find the selector: [{}].".format(selector))
filename = "{}.png".format(get_current_time_str())
screenshot_path = os.path.join(get_snapshot_directory(), filename)
logger.debug(instance.selenium.page_source)
instance.selenium.save_screenshot(screenshot_path)
raise ex
return wrapper

class BasePage(object):
url = ""
base_url = settings.WEB_TEST_BASE_URL
def __init__(self, selenium, url_params=None):
if not url_params:
url_params = []
self.selenium = selenium
self.url_params = url_params
self.go_to()
def go_to(self):
logger.debug("Goto page: [{}]".format(self.get_page_url()))
return self._selenium_get_url(self.get_page_url())
def refresh(self):
self.selenium.refresh()
def navigate_back(self):
self.selenium.back()
def _selenium_get_url(self, url):
try:
self.selenium.get('about:blank')
self.selenium.get(str(url))
except Exception as ex:
logger.error("Can not open the url:[{}]".format(url))
raise ex
return self
def get_page_url(self):
if not self.url:
raise RuntimeError("no url been set")
return self._get_url(self.url)
def _get_url(self, url):
format_url = url.format(*self.url_params)
return "{0}{1}".format(self.base_url, format_url)
def get_current_page_url(self):
return self.selenium.current_url
def get_page_title(self):
return self.selenium.title
def get_cookie_value(self):
return self.selenium.get_cookie('client_identity')['value']
\# ---------------------------------------------------------------------------------------------------------------
'''判断某个元素是否被添加到了dom里并且可见，可见代表元素可显示且宽和高都大于0'''
@fail_on_screenshot
def find_element_by_css(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.CSS_SELECTOR, selector)))
@fail_on_screenshot
def find_element_by_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.LINK_TEXT, selector)))
@fail_on_screenshot
def find_element_by_partial_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.PARTIAL_LINK_TEXT, selector)))
@fail_on_screenshot
def find_element_by_id(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.ID, selector)))
@fail_on_screenshot
def find_element_by_xpath(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.XPATH, selector)))
@fail_on_screenshot
def find_element_by_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.NAME, selector)))
@fail_on_screenshot
def find_element_by_class_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.CLASS_NAME, selector)))
@fail_on_screenshot
def find_element_by_tag_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.TAG_NAME, selector)))
\# ----------------------------------------------------------------------------------------------------------------
'''判断是否至少有1个元素存在于dom树中，如果定位到就返回列表'''
@fail_on_screenshot
def find_elements_by_css(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.presence_of_all_elements_located((By.CSS_SELECTOR, selector)))
@fail_on_screenshot
def find_elements_by_class_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.presence_of_all_elements_located((By.CLASS_NAME, selector)))
@fail_on_screenshot
def find_elements_by_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.presence_of_all_elements_located((By.LINK_TEXT, selector)))
@fail_on_screenshot
def find_elements_by_xpath(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.presence_of_all_elements_located((By.XPATH, selector)))
\# -------------------------------------------------------------------------------------------------------------
'''判断某个元素在是否存在于dom或不可见,如果可见返回False,不可见返回这个元素'''
@fail_on_screenshot
def invisible_element_by_id(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.ID, selector)))
@fail_on_screenshot
def invisible_element_by_xpath(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.XPATH, selector)))
@fail_on_screenshot
def invisible_element_by_css(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.CSS_SELECTOR, selector)))
@fail_on_screenshot
def invisible_element_by_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.LINK_TEXT, selector)))
@fail_on_screenshot
def invisible_element_by_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.NAME, selector)))
@fail_on_screenshot
def invisible_element_by_class_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.CLASS_NAME, selector)))
@fail_on_screenshot
def invisible_element_by_tag_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.TAG_NAME, selector)))
@fail_on_screenshot
def invisible_element_by_partial_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.invisibility_of_element_located((By.PARTIAL_LINK_TEXT, selector)))
\# -----------------------------------------------------------------------------------------------------------------
'''判断指定的元素中是否包含了预期的字符串，返回布尔值'''
@fail_on_screenshot
def text_to_be_present_in_element_by_id(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.ID, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_by_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.NAME, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_by_class_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.CLASS_NAME, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_by_xpath(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.XPATH, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_by_tag_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.TAG_NAME, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_by_css(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element((By.CSS_SELECTOR, selector),text))
\# -----------------------------------------------------------------------------------------------------------------
'''判断指定元素的属性值中是否包含了预期的字符串，返回布尔值'''
@fail_on_screenshot
def text_to_be_present_in_element_value_by_css(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.CSS_SELECTOR, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_value_by_id(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.ID, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_value_by_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.NAME, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_value_by_css_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.CLASS_NAME, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_value_by_xpath(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.XPATH, selector),text))
@fail_on_screenshot
def text_to_be_present_in_element_value_by_tag_name(self, selector, wait_time=WAIT_TIME,text=None):
return WebDriverWait(self.selenium, wait_time).until(
expected.text_to_be_present_in_element_value((By.TAG_NAME, selector),text))

\# -----------------------------------------------------------------------------------------------------------------
'''判断title,返回布尔值'''
@fail_on_screenshot
def page_title_is(self, title, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(expected.title_is(title))
@fail_on_screenshot
def page_title_contains(self, title, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(expected.title_contains(title))
\# -----------------------------------------------------------------------------------------------------------------
'''判断某个元素中是否可见并且是enable的，代表可点击'''
@fail_on_screenshot
def element_to_be_click_able_by_id(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.ID, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.NAME, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_class_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.CLASS_NAME, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_css(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.CSS_SELECTOR, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_tag_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.TAG_NAME, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_xpath(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.XPATH, selector)))
@fail_on_screenshot
def element_to_be_click_able_by_link_text(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.element_to_be_clickable((By.LINK_TEXT, selector)))
\# -----------------------------------------------------------------------------------------------------------------
'''判断元素是否可见，如果可见就返回这个元素，不可见返回False'''
@fail_on_screenshot
def visibility_of_element_by_id(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.ID, selector)))
@fail_on_screenshot
def visibility_of_element_by_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.NAME, selector)))
@fail_on_screenshot
def visibility_of_element_by_class_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.CLASS_NAME, selector)))
@fail_on_screenshot
def visibility_of_element_by_css(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.CSS_SELECTOR, selector)))
@fail_on_screenshot
def visibility_of_element_by_xpath(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.XPATH, selector)))
@fail_on_screenshot
def visibility_of_element_by_tag_name(self, selector, wait_time=WAIT_TIME):
return WebDriverWait(self.selenium, wait_time).until(
expected.visibility_of_element_located((By.TAG_NAME, selector)))
def get_cookie_by_name(self, name):
cookie = self.selenium.get_cookie(name)
return cookie['value']
def get_session_id(self):
return self.get_cookie_by_name("TSID")
五、utilities目录下主要存放数据库、redis等其他相关的操作
conn_db.py
连接数据库
import pymysql.cursors,settings

def execute(sql, params=None, db='', is_fetchone=True):
\# Connect to the database
connection = pymysql.connect(host=settings.DB_HOST,
port=settings.DB_PORT,
user=settings.DB_USER,
password=settings.DB_PASSWORD,
db=db,
autocommit=True,
charset='utf8mb4',
cursorclass=pymysql.cursors.DictCursor)
try:
with connection.cursor() as cursor:
cursor.execute(sql, params)
if is_fetchone:
return cursor.fetchone()
else:
return cursor.fetchall()
finally:
connection.close()
conn_redis.py
连接redis（在redis中通过session获取图形验证码、清除缓存等）
import redis, settings

def redis_connect(host=settings.CACHED_REDIS_HOST, port=settings.CACHED_REDIS_PORT):
\# Connect to the redis
redis_client = redis.StrictRedis(host=host, port=port)
return redis_client

下面是对注册功能的自动化，供参考：
在pages下创建register_page.py，封装注册页面中所需要的元素，内容如下：
\# coding=UTF-8
from pages.base_page import BasePage
from utilities import operation

class RegisterPage(BasePage):
url = '/register'

def input_new_mobile(self,mobile):
\# 输入新注册手机号
element = self.find_element_by_id('phonenumber')
element.clear()
element.send_keys(mobile)
return self
def click_send_sms_captcha_bt(self):
\# 点击获取短信验证码
if self.element_to_be_click_able_by_id('get_code_btn'):
self.find_element_by_id('get_code_btn').click()
return self
def judgement_sms_captcha_frame(self):
\# 校验发送验证码成功后弹框
self.visibility_of_element_by_css('.except-live-pop')
return self
def click_send_sms_captcha_affirm(self):
\# 点击发送短信验证码弹框上的确定按钮
self.find_element_by_css('.except-live-pop-btn').click()
return self
def input_sms_captcha(self,mobile):
\# 输入短信验证码
sms_captcha = operation.get_sms_captcha(mobile)
element = self.find_element_by_id('r_valiedatecode')
element.clear()
element.send_keys(sms_captcha)
return self
def input_new_password(self,password):
\# 输入注册时密码
element = self.find_element_by_id('r_pwd')
element.clear()
element.send_keys(password)
return self
def click_submit_register(self):
\# 点击注册按钮
if self.element_to_be_click_able_by_id('btn_reg'):
self.find_element_by_id('btn_reg').click()
return self
def judgement_input_nickname_frame(self):
\# 校验弹出输入用户昵称框
self.visibility_of_element_by_css('.c-name')
return self
def input_nickname(self,nickname):
\# 输入用户昵称
element = self.find_element_by_id('c_name_ipt')
element.clear()
element.send_keys(nickname)
return self
def click_nickname_bt(self):
\# 点击输入昵称框确定按钮
if self.element_to_be_click_able_by_css('.c-n-btn'):
self.find_element_by_css('.c-n-btn').click()
return self
def register(self,mobile,password,nickname):
\# 注册流程
self.input_new_mobile(mobile).click_send_sms_captcha_bt()
if self.judgement_sms_captcha_frame():
self.click_send_sms_captcha_affirm()
self.input_sms_captcha(mobile)
self.input_new_password(password).click_submit_register()
if self.judgement_input_nickname_frame():
self.input_nickname(nickname).click_nickname_bt()
return self
index_page.py
封装首页所需要的元素，主要是进行注册成功后用户昵称的校验\# -*- coding:utf-8 -*-
from page.base_page import BasePage

class IndexPage(BasePage):
url = '/index'
def get_user_name(self):
\# 获取用户昵称
name = self.find_element_by_css('.yk-name').text
return name
operation.py文件内容：
在数据库中获取短信验证码和删除新注册用户的sql
\# -*- coding:utf-8 -*-
import db
def get_sms_captcha(mobile):
\# 获取短信验证码
sms_captcha = db.execute('select code from send_sms_code where mobile=%s order by id desc',params=(mobile))
return sms_captcha['code']
def delete_user(mobile):
\# 删除用户
db.execute('delete from user where mobile=%s',params=(mobile))
在test_case下新建test_register.py
下面是一个简单的测试注册成功的测试用例
\# -*- coding:utf-8 -*-
from pages.register_page import RegisterPage
from pages.index_page import IndexPage
from test_case.base import BaseSeleniumTestCase
from utilities import operation
import settings,time

class TestRegister(BaseSeleniumTestCase):
mobile = '13100000001'
password = '123abc'
nickname = 'autotester'

def test_register_success(self):
\# 校验注册流程及注册成功后自动跳转到首页
after_register_url = RegisterPage(self.selenium).register(self.mobile, self.password,self.nickname).get_current_page_url()
time.sleep(1.5)
self.assertEqual(after_register_url, settings.WEB_TEST_BASE_URL + '/index')
\# 校验注册成功后页面title
after_register_title = RegisterPage(self.selenium).get_page_title()
self.assertEqual(after_register_title,u'Test-Title')
\# 校验注册成功后首页用户昵称是否与填写的一致
nickname = IndexPage(self.selenium).get_user_name()
self.assertEqual(nickname,u'autotest...')
def tearDown(self):
super(TestRegister,self).tearDown()
operation.delete_user(self.mobile)

用例执行结束后，通过teardown()方法将该用户在数据库中删除，下次执行的时候避免提示该手机号已注册。





