# Nebula3学习笔记(8): 脚本系统 - 逍遥剑客 - CSDN博客
2008年07月25日 20:51:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1967
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
可能是还在开发当中的缘故, 我感觉Nebula3中的lua脚本系统不是很完善. 所有的调用都是封装成Command来执行的, 并不像LuaBind那样直接绑定到C++类对象; 而且, 对于C++调用脚本的接口也不是很方便, 只有一个Eval()来执行一个字符串. 如果要实际进行应用的话, 我想最好是自己扩展一下, 这里有一篇不错的文章: Integrating Lua into C++. 当然, 对于需求更高的用户来说, 可以选择使用LuaBind等第三方库来整合脚本系统.
**Command(命令)**
可以这么说, 脚本中调用的, 都是一个个的Command. 一个新的Command定义了一个脚本语言独立的新的脚本命令, 你可以通过派生一个Command的子类并注册到脚本服务器来实现. 也就是说, 新的命令不依赖于你具体使用的脚本系统, 可以是lua, 也可以是python等等.
class Print : public Scripting::Command
{
    DeclareClass(Print);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& str);
};
**ScriptServer(脚本服务器)**ScriptServer是语言无双的, 也就是说你可以自己派生一个相应语言的子来来支持一种脚本言. Nebula3里已经实现了一个LuaServer, 不过个感觉没有LuaBind方便. 所有的脚本执行都是通过LuaServer::Eval(const String& str)来完成的. 脚本要调用C++代码的话, 需要封装一个Command, 然后用LuaServer::RegisterCommand()来注册就可以用了. 具体可以参考Command命名空间里的相关代码.
   scriptServer->RegisterCommand("print", Print::Create());
**应用实例**
其实App::ConsoleApplication里就有LuaServer, 并且已经注册了一些IO命名. 我们派生一个从命令行读取脚本命令执行的来做测试:
class ScripTestApp : public App::ConsoleApplication
{
public:
 ScripTestApp(void);
 /// open the application
 virtual bool Open();
 /// run the application, return when user wants to exit
 virtual void Run();
};
 ScripTestApp::ScripTestApp(void)
{
}
bool ScripTestApp::Open()
{
 if (ConsoleApplication::Open())
 {
  return true;
 }
 return false;
}
void ScripTestApp::Run()
{
 Util::String input;
 while (true)
 {
  input = IO::Console::Instance()->GetInput();
  if (!input.IsEmpty())
  {
   this->scriptServer->Eval(input);
  }
 }
}
运行结果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20080725/Nebula3_LuaScript_Test633526159504193264.JPG)
