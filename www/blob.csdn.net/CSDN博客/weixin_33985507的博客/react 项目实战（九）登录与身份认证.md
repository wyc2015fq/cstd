# react 项目实战（九）登录与身份认证 - weixin_33985507的博客 - CSDN博客
2018年03月12日 17:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：68
SPA的鉴权方式和传统的web应用不同：由于页面的渲染不再依赖服务端，与服务端的交互都通过接口来完成，而REASTful风格的接口提倡无状态（state less），通常不使用cookie和session来进行身份认证。
比较流行的一种方式是使用web token，所谓的token可以看作是一个标识身份的令牌。客户端在登录成功后可以获得服务端加密后的token，然后在后续需要身份认证的接口请求中在header中带上这个token，服务端就可以通过判断token的有效性来验证该请求是否合法。
我们先来改造一下服务端，实现一个简单的基于token的身份认证（可直接复制代码，无需关心具体实现）。
# 改造服务端
先在根目录下执行`npm i json-server -D`，虽然一开始以全局的方式安装过json-server这个工具，但本次要在代码中使用json-server的api，需要将其安装为项目依赖。
然后新建`/server/auth.js`文件，写入以下代码：
```java
/**
 * 到期时间
 */
const expireTime = 1000 * 60;
 
module.exports = function (req, res, next) {
  res.header('Access-Control-Expose-Headers', 'access-token');
  const now = Date.now();
 
  let unauthorized = true; // 未授权
  const token = req.headers['access-token'];
  if (token) {
    const expired = now - token > expireTime;
    if (!expired) {
      unauthorized = false;
      res.header('access-token', now);
    }
  }
 
  if (unauthorized) {
    res.sendStatus(401);
  } else {
    next();
  }
};
```
新建`/server/index.js`文件，写入以下代码：
```java
const path = require('path');
const jsonServer = require('json-server');
const server = jsonServer.create();
const router = jsonServer.router(path.join(__dirname, 'db.json'));
const middlewares = jsonServer.defaults();
server.use(jsonServer.bodyParser);
server.use(middlewares);
server.post('/login', function (req, res, next) {
  res.header('Access-Control-Expose-Headers', 'access-token');
  const {account, password} = req.body;
  if (account === 'admin' && password === '123456') {
    res.header('access-token', Date.now());
    res.json(true);
  } else {
    res.json(false);
  }
});
server.use(require('./auth'));
server.use(router);
server.listen(8000, function () {
  console.log('JSON Server is running in http://localhost:8000');
});
```
修改`/package.json`文件中的`scripts.server`：
```java
{
  ...
  "scripts": {
    "server": "node server/index.js",
    ...
  },
  ...
}
```
然后使用`npm run server`重启服务器。
现在我们的服务器就拥有了身份认证的功能，访问除了’/login’外的其它接口时，服务端会根据请求的header中access-token来判断请求是否有效，如果无效则会返回401状态码。
当客户端收到401的状态码时，需要跳转到登录页面进行登录，有效的管理员账号为admin，密码为123456。
以POST方法提交下面的参数到’[http://localhost:8000/login](http://localhost:8000/login)‘接口，就能够完成登录。
```java
{
  "account": "admin",
  "password": "123456"
}
```
登录成功后，接口返回`true`，并且在返回的headers中包含了一个有效的access-token，用于在后面的请求中使用；登录失败则返回`false`。
access-token的有效期为1分钟，每次有效的接口请求都会获得新的access-token；若1分钟内没有做操作，则会过期需要重新登录。
*我们的access-token只是一个简单的timestamp，且没有做任何加密措施。*
# 封装fetch
由于我们每个接口的请求都需要加上一个名为access-token的header，在每次需要调用接口的时候都写一遍就非常的不明智了，所以我们需要封装fetch方法。
新建`/src/utils/request.js`，写入以下代码：
```java
/**
 * 封装 fetch
 */
import { hashHistory } from 'react-router';
 
export default function request (method, url, body) {
  method = method.toUpperCase();
  if (method === 'GET') {
    // fetch的GET不允许有body，参数只能放在url中
    body = undefined;
  } else {
    body = body && JSON.stringify(body);
  }
 
  return fetch(url, {
    method,
    headers: {
      'Content-Type': 'application/json',
      'Accept': 'application/json',
      'Access-Token': sessionStorage.getItem('access_token') || '' // 从sessionStorage中获取access token
    },
    body
  })
  .then((res) => {
    if (res.status === 401) {
      hashHistory.push('/login');
      return Promise.reject('Unauthorized.');
    } else {
      const token = res.headers.get('access-token');
      if (token) {
        sessionStorage.setItem('access_token', token);
      }
      return res.json();
    }
  });
}
// GET 请求
export const get = url => request('GET', url);
// POST 请求
export const post = (url, body) => request('POST', url, body);
// PUT 上传
export const put = (url, body) => request('PUT', url, body);
// DELETE 删除
export const del = (url, body) => request('DELETE', url, body);
```
request方法封装了添加access-token头等逻辑，然后就可以在需要调用接口的时候使用request或get、post等方法了，比如`/src/components/BookEditor.js`：
```java
...
import request, {get} from '../utils/request';
class BookEditor extends React.Component {
  ...
  handleSubmit (e) {
    ...
    let editType = '添加';
    let apiUrl = 'http://localhost:8000/book';
    let method = 'post';
    if (editTarget) {
      ...
    }
    request(method, apiUrl, {
      name: name.value,
      price: price.value,
      owner_id: owner_id.value
    })
      .then((res) => {
        if (res.id) {
          ...
        } else {
          ...
        }
      })
      .catch((err) => console.error(err));
  }
  getRecommendUsers (partialUserId) {
    get('http://localhost:8000/user?id_like=' + partialUserId)
      .then((res) => {
        if (res.length === 1 && res[0].id === partialUserId) {
          return;
        }
        ...
      });
  }
  ...
}
...
```
其它还有`/src/components/UserEditor.js`、`/src/pages/BookEdit.js`、`/src/pages/BookList.js`、`/src/pages/UserEdit.js`和`/src/pages/UserList.js`文件需要进行相应的修改。
`/src/components/UserEditor.js`
```java
/**
 * 用户编辑器组件
 */
import React from 'react';
import FormItem from '../components/FormItem'; // 或写成 ./FormItem
// 高阶组件 formProvider表单验证
import formProvider from '../utils/formProvider';
// 引入 prop-types
import PropTypes from 'prop-types';
// 引入 封装fetch工具类
import request from '../utils/request'; 
class UserEditor extends React.Component {
  // 按钮提交事件
  handleSubmit(e){
    // 阻止表单submit事件自动跳转页面的动作
    e.preventDefault();
    // 定义常量
    const { form: { name, age, gender }, formValid, editTarget} = this.props; // 组件传值
    // 验证
    if(!formValid){
      alert('请填写正确的信息后重试');
      return;
    }
    // 默认值
    let editType = '添加';
    let apiUrl = 'http://localhost:8000/user';
    let method = 'post';
    // 判断类型
    if(editTarget){
      editType = '编辑';
      apiUrl += '/' + editTarget.id;
      method = 'put';
    }
    // 发送请求
    request(method,apiUrl, {
      name: name.value,
      age: age.value,
      gender: gender.value
    })
    // 成功的回调
    .then((res) => {
      // 当添加成功时,返回的json对象中应包含一个有效的id字段
      // 所以可以使用res.id来判断添加是否成功
      if(res.id){
        alert(editType + '添加用户成功!');
        this.context.router.push('/user/list'); // 跳转到用户列表页面
        return;
      }else{
        alert(editType + '添加用户失败!');
      }
    })
    // 失败的回调
    .catch((err) => console.error(err));
  }
  // 生命周期--组件加载中
  componentWillMount(){
    const {editTarget, setFormValues} = this.props;
    if(editTarget){
      setFormValues(editTarget);
    }
  }
  
  render() {
    // 定义常量
    const {form: {name, age, gender}, onFormChange} = this.props;
    return (
      <form onSubmit={(e) => this.handleSubmit(e)}>
        <FormItem label="用户名:" valid={name.valid} error={name.error}>
          <input
            type="text"
            value={name.value}
            onChange={(e) => onFormChange('name', e.target.value)}/>
        </FormItem>
        <FormItem label="年龄:" valid={age.valid} error={age.error}>
          <input
            type="number"
            value={age.value || ''}
            onChange={(e) => onFormChange('age', e.target.value)}/>
        </FormItem>
        <FormItem label="性别:" valid={gender.valid} error={gender.error}>
          <select
            value={gender.value}
            onChange={(e) => onFormChange('gender', e.target.value)}>
            <option value="">请选择</option>
            <option value="male">男</option>
            <option value="female">女</option>
          </select>
        </FormItem>
        <br />
        <input type="submit" value="提交" />
      </form>
    );
  }
}
// 必须给UserEditor定义一个包含router属性的contextTypes
// 使得组件中可以通过this.context.router来使用React Router提供的方法
UserEditor.contextTypes = {
  router: PropTypes.object.isRequired
};
// 实例化
UserEditor = formProvider({ // field 对象
  // 姓名
  name: {
    defaultValue: '',
    rules: [
      {
        pattern: function (value) {
          return value.length > 0;
        },
        error: '请输入用户名'
      },
      {
        pattern: /^.{1,4}$/,
        error: '用户名最多4个字符'
      }
    ]
  },
  // 年龄
  age: {
    defaultValue: 0,
    rules: [
      {
        pattern: function(value){
          return value >= 1 && value <= 100;
        },
        error: '请输入1~100的年龄'
      }
    ]
  },
  // 性别
  gender: {
    defaultValue: '',
    rules: [
      {
        pattern: function(value) {
          return !!value;
        },
        error: '请选择性别'
      }
    ]
  }
})(UserEditor);
export default UserEditor;
```
`/src/pages/BookEdit.js`
```java
/**
 * 编辑图书页面
 */
import React from 'react';
// 布局组件
import HomeLayout from '../layouts/HomeLayout';
// 引入 prop-types
import PropTypes from 'prop-types';
// 图书编辑器组件
import BookEditor from '../components/BookEditor';
// 引入 封装fetch工具类
import { get } from '../utils/request'; 
class BookEdit extends React.Component {
  // 构造器
  constructor(props) {
    super(props);
    // 定义初始化状态
    this.state = {
      book: null
    };
  }
  // 生命周期--组件加载中
  componentWillMount(){
    // 定义常量
    const bookId = this.context.router.params.id;
    /**
     * 发送请求
     * 获取用户数据
     */
    get('http://localhost:8000/book/' + bookId)
    .then((res) => {
      console.log(res);
      // 设置状态
      this.setState({
        book: res
      });
    })
  }
  render() {
    const {book} = this.state;
    return (
      <HomeLayout title="编辑图书">
        {
          book ? <BookEditor editTarget={book} /> : '加载中...'
        }
      </HomeLayout>
    );
  }
}
BookEdit.contextTypes = {
  router: PropTypes.object.isRequired
};
export default BookEdit;
```
`/src/pages/BookList.js`
```java
/**
 * 图书列表页面
 */
import React from 'react';
// 布局组件
import HomeLayout from '../layouts/HomeLayout';
// 引入 prop-types
import PropTypes from 'prop-types';
// 引入 封装fetch工具类
import { get, del } from '../utils/request'; 
class BookList extends React.Component {
  // 构造器
  constructor(props) {
    super(props);
    // 定义初始化状态
    this.state = {
      bookList: []
    };
  }
  /**
   * 生命周期
   * componentWillMount
   * 组件初始化时只调用，以后组件更新不调用，整个生命周期只调用一次
   */
  componentWillMount(){
    // 请求数据
    get('http://localhost:8000/book')
      .then((res) => {
        /**
         * 成功的回调
         * 数据赋值
         */
        this.setState({
          bookList: res
        });
      });
  }
  /**
   * 编辑
   */
  handleEdit(book){
    // 跳转编辑页面
    this.context.router.push('/book/edit/' + book.id);
  }
  /**
   * 删除
   */
  handleDel(book){
    // 确认框
    const confirmed = window.confirm(`确认要删除书名 ${book.name} 吗?`);
    // 判断
    if(confirmed){
      // 执行删除数据操作
      del('http://localhost:8000/book/' + book.id, {
      })
      .then(res => {
        /**
         * 设置状态
         * array.filter
         * 把Array的某些元素过滤掉，然后返回剩下的元素
         */
        this.setState({
          bookList: this.state.bookList.filter(item => item.id !== book.id)
        });
        alert('删除用户成功');
      })
      .catch(err => {
        console.log(err);
        alert('删除用户失败');
      });
    }
  }
  render() {
    // 定义变量
    const { bookList } = this.state;
    return (
      <HomeLayout title="图书列表">
        <table>
          <thead>
            <tr>
              <th>图书ID</th>
              <th>图书名称</th>
              <th>价格</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            {
              bookList.map((book) => {
                return (
                  <tr key={book.id}>
                    <td>{book.id}</td>
                    <td>{book.name}</td>
                    <td>{book.price}</td>
                    <td>
                      <a onClick={() => this.handleEdit(book)}>编辑</a>
                       
                      <a onClick={() => this.handleDel(book)}>删除</a>
                    </td>
                  </tr>
                );
              })
            }
          </tbody>
        </table>
      </HomeLayout>
    );
  }
}
/**
 * 任何使用this.context.xxx的地方，必须在组件的contextTypes里定义对应的PropTypes
 */
BookList.contextTypes = {
  router: PropTypes.object.isRequired
};
export default BookList;
```
`/src/pages/UserEdit.js`
```java
/**
 * 编辑用户页面
 */
import React from 'react';
// 布局组件
import HomeLayout from '../layouts/HomeLayout';
// 引入 prop-types
import PropTypes from 'prop-types';
// 用户编辑器组件
import UserEditor from '../components/UserEditor';
// 引入 封装fetch工具类
import { get } from '../utils/request'; 
class UserEdit extends React.Component {
  // 构造器
  constructor(props) {
    super(props);
    // 定义初始化状态
    this.state = {
      user: null
    };
  }
  // 生命周期--组件加载中
  componentWillMount(){
    // 定义常量
    const userId = this.context.router.params.id;
    /**
     * 发送请求
     * 获取用户数据
     */
    get('http://localhost:8000/user/' + userId)
    .then((res) => {
      // 设置状态
      this.setState({
        user: res
      });
    })
  }
  render() {
    const {user} = this.state;
    return (
      <HomeLayout title="编辑用户">
        {
          user ? <UserEditor editTarget={user} /> : '加载中...'
        }
      </HomeLayout>
    );
  }
}
UserEdit.contextTypes = {
  router: PropTypes.object.isRequired
};
export default UserEdit;
```
`/src/pages/UserList.js`
```java
/**
 * 用户列表页面
 */
import React from 'react';
// 布局组件
import HomeLayout from '../layouts/HomeLayout';
// 引入 prop-types
import PropTypes from 'prop-types';
// 引入 封装后的fetch工具类
import { get, del } from '../utils/request';
class UserList extends React.Component {
  // 构造器
  constructor(props) {
    super(props);
    // 定义初始化状态
    this.state = {
      userList: []
    };
  }
  /**
   * 生命周期
   * componentWillMount
   * 组件初始化时只调用，以后组件更新不调用，整个生命周期只调用一次
   */
  componentWillMount(){
    // 请求数据
    get('http://localhost:8000/user')
      .then((res) => {
        /**
         * 成功的回调
         * 数据赋值
         */
        this.setState({
          userList: res
        });
      });
  }
  /**
   * 编辑
   */
  handleEdit(user){
    // 跳转编辑页面
    this.context.router.push('/user/edit/' + user.id);
  }
  /**
   * 删除
   */
  handleDel(user){
    // 确认框
    const confirmed = window.confirm(`确认要删除用户 ${user.name} 吗?`);
    // 判断
    if(confirmed){
      // 执行删除数据操作
      del('http://localhost:8000/user/' + user.id, {
      })
      .then((res) => {
        /**
         * 设置状态
         * array.filter
         * 把Array的某些元素过滤掉，然后返回剩下的元素
         */
        this.setState({
          userList: this.state.userList.filter(item => item.id !== user.id)
        });
        alert('删除用户成功');
      })
      .catch(err => {
        console.log(err);
        alert('删除用户失败');
      });
    }
  }
  render() {
    // 定义变量
    const { userList } = this.state;
    return (
      <HomeLayout title="用户列表">
        <table>
          <thead>
            <tr>
              <th>用户ID</th>
              <th>用户名</th>
              <th>性别</th>
              <th>年龄</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            {
              userList.map((user) => {
                return (
                  <tr key={user.id}>
                    <td>{user.id}</td>
                    <td>{user.name}</td>
                    <td>{user.gender}</td>
                    <td>{user.age}</td>
                    <td>
                      <a onClick={() => this.handleEdit(user)}>编辑</a>
                       
                      <a onClick={() => this.handleDel(user)}>删除</a>
                    </td>
                  </tr>
                );
              })
            }
          </tbody>
        </table>
      </HomeLayout>
    );
  }
}
/**
 * 任何使用this.context.xxx的地方，必须在组件的contextTypes里定义对应的PropTypes
 */
UserList.contextTypes = {
  router: PropTypes.object.isRequired
};
export default UserList;
```
# 实现登录页面
现在尝试访问一下用户列表页，发现表格里面并没有数据，因为没有登录接口访问被拒绝了并且尝试跳转到路由’/login’。
现在来实现一个登录页面组件，在`/src/pages`下新建Login.js文件：
```java
/**
 * 登录页
 */
import React from 'react';
// 页面布局组件
import HomeLayout from '../layouts/HomeLayout';
import FormItem from '../components/FormItem';
// 引入 封装后的fetch工具类
import { post } from '../utils/request';
// 表单验证组件
import formProvider from '../utils/formProvider';
// 引入 prop-types
import PropTypes from 'prop-types';
 
class Login extends React.Component {
  // 构造器
  constructor () {
    super();
    this.handleSubmit = this.handleSubmit.bind(this);
  }
  
  handleSubmit (e) {
    e.preventDefault();
 
    const {formValid, form: {account, password}} = this.props;
    if (!formValid) {
      alert('请输入账号或密码');
      return;
    }
 
    post('http://localhost:8000/login', {
      account: account.value,
      password: password.value
    })
      .then((res) => {
        if (res) {
          this.context.router.push('/');
        } else {
          alert('登录失败，账号或密码错误');
        }
      })
  }
 
  render () {
    const {form: {account, password}, onFormChange} = this.props;
    return (
      <HomeLayout title="请登录">
        <form onSubmit={this.handleSubmit}>
          <FormItem label="账号：" valid={account.valid} error={account.error}>
            <input type="text" value={account.value} onChange={e => onFormChange('account', e.target.value)}/>
          </FormItem>
          <FormItem label="密码：" valid={password.valid} error={password.error}>
            <input type="password" value={password.value} onChange={e => onFormChange('password', e.target.value)}/>
          </FormItem>
          <br/>
          <input type="submit" value="登录"/>
        </form>
      </HomeLayout>
    );
  }
}
 
Login.contextTypes = {
  router: PropTypes.object.isRequired
};
 
Login = formProvider({
  account: {
    defaultValue: '',
    rules: [
      {
        pattern (value) {
          return value.length > 0;
        },
        error: '请输入账号'
      }
    ]
  },
  password: {
    defaultValue: '',
    rules: [
      {
        pattern (value) {
          return value.length > 0;
        },
        error: '请输入密码'
      }
    ]
  }
})(Login);
 
export default Login;
```
登录页面组件和UserEditor或者BookEditor类似，都是一个表单。
在这里提交表单成功后跳转到首页。
最后，别忘了加上登录页面的路由。
# 最终效果
![](https://images2018.cnblogs.com/blog/1090399/201803/1090399-20180315225133987-109159133.gif)
项目结构：
![](https://images2018.cnblogs.com/blog/1090399/201803/1090399-20180316154414683-859311152.png)
