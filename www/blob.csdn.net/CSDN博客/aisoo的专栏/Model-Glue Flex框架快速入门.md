# Model-Glue:Flex框架快速入门 - aisoo的专栏 - CSDN博客
2010年05月28日 09:11:00[aisoo](https://me.csdn.net/aisoo)阅读数：1642标签：[flex																[框架																[coldfusion																[flexbuilder																[application																[button](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=flexbuilder&t=blog)](https://so.csdn.net/so/search/s.do?q=coldfusion&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=flex&t=blog)
对于做Coldfusion开发的朋友来说，[Model-Glue](http://www.model-glue.com/)这个轻量级的Coldfusion框架可谓鼎鼎大名哦，最近该项目分解成了两个项目：[Model-Glue:Coldfusion](http://www.model-glue.com/coldfusion.cfm)和[Model-Glue:Flex](http://www.model-glue.com/flex.cfm)，Model-Glue:Coldfusion就是原来的Model-Glue框架，而Model-Glue:Flex就是我们今天要说的主角。
秉承Model-Glue:Coldfusion框架一贯的特性，Model-Glue:Flex是一个轻量级的Flex框架，其设计思想与Model-Glue:Coldfusion一致，都是基于事件驱动，类似XML的事件处理配置的一个MVC框架，熟悉Model-Glue:Coldfusion的朋友完全可以很平滑无障碍的使用Model-Glue:Flex框架。其核心的设计很简单易懂：UI组件触发事件，控制器监听事件处理事件。
大家可以到[这里](http://www.model-glue.com/downloads/modelglueflex_0.0.zip)下载Model-Glue:Flex框架。解压后的文件夹结构如下：
|-------applicationtemplate/
|-------docs/
|       |-------howto/
|       |-------quickstart/
|-------examples/
|-------LICENSE-2.0
|-------modelglue/
|       |-------bin/
|       |       |-------ModelGlueFlex.swc
|       |-------com/
|-------NOTICE
|-------README
其中applicationtemplate目录是Model-Glue:Flex应用程序模板，如果您要基于Model-Glue:Flex进行Flex开发，可以从这个模板开始，后面我们会详细说明；docs目录是文档目录，当前Model-Glue:Flex还处于Alpha 1阶段，文档还没有整理出来，还只有一个quickstart，并且该quickstart也还不是很完整；examples是两个基于Model-Glue:Flex的Flex应用示例，仔细研究两个示例将能帮组您对Model-Glue:Flex有一个深刻的了解；最后modelglue目录是Model-Glue:Flex的源代码目录，其中的bin目录是Model-Glue:Flex二进制SWC组件包，要在Flex中使用Model-Glue:Flex框架，需要将该SWC文件添加到编译路径中。
下面我们来动手进行基于Model-Glue:Flex框架的Flex应用开发。首先在FlexBuilder中创建一个Flex工程：ModelGlueQuickstart。然后在我们新创建的ModelGlueQuickstart工程中，右键选择>属性>Flex Build Path>Library path>Add SWC将前面我们提到的bin目录下的Model-Glue:Flex二进制SWC组件包添加到Flex工程的编译路径中。未使用FlexBuilder的朋友，注意在mxmlc编译的时候将该SWC添加到编译LIB路径中。
前面我们已经提到了applicationtemplate目录，这是一个基于Model-Glue:Flex开发的Flex应用模板，现在我们要把applicationtemplate目录下的assets、config和control目录复制到ModelGlueQuickstart工程的源代码目录。然后将applicationtemplate/ApplicationTemplate.mxml文件的内容复制/粘贴到ModelGlueQuickstart工程的ModelGlueQuickstart.mxml文件中。
上面所有的操作完成后，正常的话，您就可以成功编译ModelGlueQuickstart.mxml文件，并在bin目录生成ModelGlueQuickstart.swf文件，[运行该文件](http://labs.eshangrao.com/flex/2/modelglue_flex/ModelGlueQuickstart.swf)，显示如下：
![](http://www.47school.com/uploads/allimg/c091011/12551a0M20130-95632.jpg)
点击"sayHi"按钮，画面如下：
![](http://www.47school.com/uploads/allimg/c091011/12551a0N1940-10Q54.jpg)
现在让我来看看ModelGlueQuickstart.mxml文件：
<?xml version="1.0" encoding="utf-8"?>
<mx:Application 
	xmlns:mx="http://www.adobe.com/2006/mxml"
	xmlns:config="config.*"
>
 
	<mx:Script>
		<![CDATA[		
			import com.firemoss.modelglue.event.ModelGlueEvent;		
		]]>
	</mx:Script>
	
	<!-- Styles / Skinning -->
	<mx:Style source="/assets/css/stylesheet.css" />
	
	<!-- ModelGlue Configuration -->
	<config:ModelGlueConfiguration />
	
	<mx:Panel width="95%" height="95%" title="Model-Glue Application Template" verticalAlign="middle" horizontalAlign="center">
		<mx:Button label="Say Hi!" click="new ModelGlueEvent('greeting').dispatch();" />
	</mx:Panel>
 
</mx:Application>
其中当我们点击"Say Hi"按钮时，我们直接触发一个“greeting”ModelGleEvent：
<mx:Button label="Say Hi!" click="new ModelGlueEvent('greeting').dispatch();" />
现在我们触发了一个ModelGlueEvent，前面我们说过Model-Glue:Flex就是UI组件触发事件，控制器监听事件处理事件。在Model-Glue:Flex中，事件的如何处理是通过ModelGlueConfiguration来配置的：
<config:ModelGlueConfiguration />
以下是config/ModelGlueConfiguration.mxml文件：
<ModelGlue xmlns="com.firemoss.modelglue.tags.*" xmlns:event="com.firemoss.modelglue.tags.event.*" xmlns:control="control.*" xmlns:mx="http://www.adobe.com/2006/mxml"
>
 
	<controllers>
	
		<!-- A controller "listens" for messages by name. -->
		<control:Controller id="controller">
			<control:messageListeners>
				<!-- A message listener states that a given controller method should run whenever its message is broadcast. -->
				<MessageListener message="greetingRequested" method="{controller.sayHello}" />
			</control:messageListeners>
		</control:Controller>
	
	</controllers>
	
	<eventHandlers>
	
		<!-- An event handler is an architectural event, such as a user logging in. -->
		<event:Handler name="greeting">
			<event:broadcasts>
				<!-- A message is a statement that something has happened or that an action is needed. -->
				<event:Message name="greetingRequested" />
			</event:broadcasts>
		</event:Handler>
	
	</eventHandlers>
 
</ModelGlue>
以上，在<eventHandlers/>我们定义了“greeting”事件的处理器，当该事件触发的时候广播一个greetingRequested消息：
<event:Handler name="greeting">
	<event:broadcasts>
		<!-- A message is a statement that something has happened or that an action is needed. -->
		<event:Message name="greetingRequested" />
	</event:broadcasts>
</event:Handler>
接着在<controllers/>中我们定义对该“greetingRequested”消息进行处理的控制器，当监测到该消息被广播的时候，执行Controller.sayHello：
<control:Controller id="controller">
	<control:messageListeners>
		<!-- A message listener states that a given controller method should run whenever its message is broadcast. -->
		<MessageListener message="greetingRequested" method="{controller.sayHello}" />
	</control:messageListeners>
</control:Controller>
最后我们来看看control/Controller.as文件：
package control
{
	import com.firemoss.modelglue.controller.ModelGlueController;
	import com.firemoss.modelglue.event.ModelGlueEvent;
	import mx.controls.Alert;
 
	public class Controller extends ModelGlueController
	{
		
		public function sayHello(e:ModelGlueEvent):void {
			Alert.show("Model-Glue is up and running!", "Ok!");
		}
		
	}
}
以上代码很简单直接显示一个警告窗口。
总结一下，首先我们在Button组件中触发一个ModelGlueEvent事件，然后在ModelGlueConfiguration中定义如何处理该事件、广播一个消息，然后再配置一个要处理该消息的控制器，进而来处理该事件，最终我们实现了MVC的分离，而且UI和控制器是松耦合的。
