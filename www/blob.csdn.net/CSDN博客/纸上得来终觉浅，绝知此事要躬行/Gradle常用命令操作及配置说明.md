# Gradle常用命令操作及配置说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月27日 18:01:29[boonya](https://me.csdn.net/boonya)阅读数：10222标签：[gradle](https://so.csdn.net/so/search/s.do?q=gradle&t=blog)
个人分类：[Groovy/Gradle](https://blog.csdn.net/boonya/article/category/3152311)









Gradle强大的编译能力和构建能力足以胜任日常中的开发，很多人将其视为maven的替代方案，至于用maven还是用Gradle这个根据各人喜爱或团队要求来决定。

### Gradle快速查看指南

官网提供：[https://docs.gradle.org/current/dsl/index.html](https://docs.gradle.org/current/dsl/index.html)



- Home
- [Introduction](https://docs.gradle.org/current/dsl/index.html#N1000C)
- [Some basics](https://docs.gradle.org/current/dsl/index.html#N10011)
- [Build script structure](https://docs.gradle.org/current/dsl/index.html#N10060)
- [Core types](https://docs.gradle.org/current/dsl/index.html#N100CA)
- [Container types](https://docs.gradle.org/current/dsl/index.html#N10292)
- [Help Task types](https://docs.gradle.org/current/dsl/index.html#N102D8)
- [Task types](https://docs.gradle.org/current/dsl/index.html#N10335)
- [Reporting types](https://docs.gradle.org/current/dsl/index.html#N10502)
- [Eclipse/IDEA model types](https://docs.gradle.org/current/dsl/index.html#N1055E)
- [Eclipse/IDEA task types](https://docs.gradle.org/current/dsl/index.html#N105E7)
- [Native software model types](https://docs.gradle.org/current/dsl/index.html#N10644)
- [Native binary task types](https://docs.gradle.org/current/dsl/index.html#N107C3)

- Build script blocks
- [`allprojects { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:allprojects%28groovy.lang.Closure%29)
- [`artifacts { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:artifacts%28groovy.lang.Closure%29)
- [`buildscript { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:buildscript%28groovy.lang.Closure%29)
- [`configurations { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:configurations%28groovy.lang.Closure%29)
- [`dependencies { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:dependencies%28groovy.lang.Closure%29)
- [`repositories { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:repositories%28groovy.lang.Closure%29)
- [`sourceSets { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:sourceSets%28groovy.lang.Closure%29)
- [`subprojects { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:subprojects%28groovy.lang.Closure%29)
- [`publishing { }`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html#org.gradle.api.Project:publishing%28groovy.lang.Closure%29)
- Core types
- [`Project`](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html)
- [`Task`](https://docs.gradle.org/current/dsl/org.gradle.api.Task.html)
- [`Gradle`](https://docs.gradle.org/current/dsl/org.gradle.api.invocation.Gradle.html)
- [`Settings`](https://docs.gradle.org/current/dsl/org.gradle.api.initialization.Settings.html)
- [`Script`](https://docs.gradle.org/current/dsl/org.gradle.api.Script.html)
- [`JavaToolChain`](https://docs.gradle.org/current/dsl/org.gradle.jvm.toolchain.JavaToolChain.html)
- [`SourceSet`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.SourceSet.html)
- [`SourceSetOutput`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.SourceSetOutput.html)
- [`IncrementalTaskInputs`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.incremental.IncrementalTaskInputs.html)
- [`Configuration`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.Configuration.html)
- [`ResolutionStrategy`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.ResolutionStrategy.html)
- [`ArtifactResolutionQuery`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.query.ArtifactResolutionQuery.html)
- [`ComponentSelection`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.ComponentSelection.html)
- [`ComponentSelectionRules`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.ComponentSelectionRules.html)
- [`ConventionProperty`](https://docs.gradle.org/current/dsl/org.gradle.api.dsl.ConventionProperty.html)
- [`GradlePluginPortal`](https://docs.gradle.org/current/dsl/org.gradle.plugin.repository.GradlePluginPortal.html)
- [`ExtensionAware`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.ExtensionAware.html)
- [`ExtraPropertiesExtension`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.ExtraPropertiesExtension.html)
- [`PublishingExtension`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.PublishingExtension.html)
- [`IvyPublication`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.IvyPublication.html)
- [`IvyArtifact`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.IvyArtifact.html)
- [`IvyArtifactSet`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.IvyArtifactSet.html)
- [`IvyModuleDescriptorSpec`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.IvyModuleDescriptorSpec.html)
- [`IvyPluginRepository`](https://docs.gradle.org/current/dsl/org.gradle.plugin.repository.IvyPluginRepository.html)
- [`MavenPublication`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.MavenPublication.html)
- [`MavenArtifact`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.MavenArtifact.html)
- [`MavenArtifactSet`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.MavenArtifactSet.html)
- [`MavenPom`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.MavenPom.html)
- [`MavenPluginRepository`](https://docs.gradle.org/current/dsl/org.gradle.plugin.repository.MavenPluginRepository.html)
- [`PluginDependenciesSpec`](https://docs.gradle.org/current/dsl/org.gradle.plugin.use.PluginDependenciesSpec.html)
- [`PluginDependencySpec`](https://docs.gradle.org/current/dsl/org.gradle.plugin.use.PluginDependencySpec.html)
- [`PluginRepositoriesSpec`](https://docs.gradle.org/current/dsl/org.gradle.plugin.repository.PluginRepositoriesSpec.html)
- [`PluginRepository`](https://docs.gradle.org/current/dsl/org.gradle.plugin.repository.PluginRepository.html)
- [`ResourceHandler`](https://docs.gradle.org/current/dsl/org.gradle.api.resources.ResourceHandler.html)
- [`TextResourceFactory`](https://docs.gradle.org/current/dsl/org.gradle.api.resources.TextResourceFactory.html)
- Container types
- [`ConfigurationContainer`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.ConfigurationContainer.html)
- [`RepositoryHandler`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.dsl.RepositoryHandler.html)
- [`DependencyHandler`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.dsl.DependencyHandler.html)
- [`ArtifactHandler`](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.dsl.ArtifactHandler.html)
- Help Task types
- [`TaskReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.TaskReportTask.html)
- [`ProjectReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.ProjectReportTask.html)
- [`DependencyReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.DependencyReportTask.html)
- [`DependencyInsightReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.DependencyInsightReportTask.html)
- [`PropertyReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.PropertyReportTask.html)
- [`ComponentReport`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.components.ComponentReport.html)
- [`ModelReport`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.model.ModelReport.html)
- Task types
- [`AntlrTask`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.antlr.AntlrTask.html)
- [`BuildEnvironmentReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.diagnostics.BuildEnvironmentReportTask.html)
- [`Checkstyle`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.Checkstyle.html)
- [`CodeNarc`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.CodeNarc.html)
- [`CompareGradleBuilds`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.buildcomparison.gradle.CompareGradleBuilds.html)
- [`Copy`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.Copy.html)
- [`CreateStartScripts`](https://docs.gradle.org/current/dsl/org.gradle.jvm.application.tasks.CreateStartScripts.html)
- [`Delete`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.Delete.html)
- [`Ear`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ear.Ear.html)
- [`Exec`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.Exec.html)
- [`FindBugs`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.FindBugs.html)
- [`GenerateIvyDescriptor`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.tasks.GenerateIvyDescriptor.html)
- [`GenerateMavenPom`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.tasks.GenerateMavenPom.html)
- [`GenerateBuildDashboard`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.GenerateBuildDashboard.html)
- [`GradleBuild`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.GradleBuild.html)
- [`GroovyCompile`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.compile.GroovyCompile.html)
- [`Groovydoc`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.javadoc.Groovydoc.html)
- [`HtmlDependencyReportTask`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.dependencies.HtmlDependencyReportTask.html)
- [`JacocoReport`](https://docs.gradle.org/current/dsl/org.gradle.testing.jacoco.tasks.JacocoReport.html)
- [`JacocoMerge`](https://docs.gradle.org/current/dsl/org.gradle.testing.jacoco.tasks.JacocoMerge.html)
- [`Jar`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.bundling.Jar.html)
- [`JavaCompile`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.compile.JavaCompile.html)
- [`Javadoc`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.javadoc.Javadoc.html)
- [`JavaExec`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.JavaExec.html)
- [`JDepend`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.JDepend.html)
- [`JettyRun`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.jetty.JettyRun.html)
- [`JettyRunWar`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.jetty.JettyRunWar.html)
- [`JettyStop`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.jetty.JettyStop.html)
- [`Pmd`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.Pmd.html)
- [`PublishToIvyRepository`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.ivy.tasks.PublishToIvyRepository.html)
- [`PublishToMavenRepository`](https://docs.gradle.org/current/dsl/org.gradle.api.publish.maven.tasks.PublishToMavenRepository.html)
- [`ScalaCompile`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.scala.ScalaCompile.html)
- [`ScalaDoc`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.scala.ScalaDoc.html)
- [`InitBuild`](https://docs.gradle.org/current/dsl/org.gradle.buildinit.tasks.InitBuild.html)
- [`Sign`](https://docs.gradle.org/current/dsl/org.gradle.plugins.signing.Sign.html)
- [`SonarAnalyze`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.sonar.SonarAnalyze.html)
- [`SonarRunner`](https://docs.gradle.org/current/dsl/org.gradle.sonar.runner.tasks.SonarRunner.html)
- [`Sync`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.Sync.html)
- [`Tar`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.bundling.Tar.html)
- [`Test`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.testing.Test.html)
- [`TestReport`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.testing.TestReport.html)
- [`Upload`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.Upload.html)
- [`War`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.bundling.War.html)
- [`Wrapper`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.wrapper.Wrapper.html)
- [`Zip`](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.bundling.Zip.html)
- Reporting types
- [`CustomizableHtmlReport`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.CustomizableHtmlReport.html)
- [`SingleFileReport`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.SingleFileReport.html)
- [`DirectoryReport`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.DirectoryReport.html)
- [`FindBugsXmlReport`](https://docs.gradle.org/current/dsl/org.gradle.api.plugins.quality.FindBugsXmlReport.html)
- [`Report`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.Report.html)
- [`Reporting`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.Reporting.html)
- [`ReportContainer`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.ReportContainer.html)
- [`ReportingExtension`](https://docs.gradle.org/current/dsl/org.gradle.api.reporting.ReportingExtension.html)
- Eclipse/IDEA model types
- [`EclipseModel`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseModel.html)
- [`EclipseProject`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseProject.html)
- [`EclipseClasspath`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseClasspath.html)
- [`EclipseJdt`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseJdt.html)
- [`EclipseWtp`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseWtp.html)
- [`EclipseWtpComponent`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseWtpComponent.html)
- [`EclipseWtpFacet`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.model.EclipseWtpFacet.html)
- [`IdeaModel`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.model.IdeaModel.html)
- [`IdeaProject`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.model.IdeaProject.html)
- [`IdeaModule`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.model.IdeaModule.html)
- [`IdeaWorkspace`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.model.IdeaWorkspace.html)
- [`XmlFileContentMerger`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.api.XmlFileContentMerger.html)
- [`FileContentMerger`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.api.FileContentMerger.html)
- Eclipse/IDEA task types
- [`GenerateEclipseProject`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.GenerateEclipseProject.html)
- [`GenerateEclipseClasspath`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.GenerateEclipseClasspath.html)
- [`GenerateEclipseJdt`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.GenerateEclipseJdt.html)
- [`GenerateEclipseWtpComponent`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.GenerateEclipseWtpComponent.html)
- [`GenerateEclipseWtpFacet`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.eclipse.GenerateEclipseWtpFacet.html)
- [`GenerateIdeaModule`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.GenerateIdeaModule.html)
- [`GenerateIdeaProject`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.GenerateIdeaProject.html)
- [`GenerateIdeaWorkspace`](https://docs.gradle.org/current/dsl/org.gradle.plugins.ide.idea.GenerateIdeaWorkspace.html)
- Native software types
- [`PrebuiltLibrary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.PrebuiltLibrary.html)
- [`PrebuiltSharedLibraryBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.PrebuiltSharedLibraryBinary.html)
- [`PrebuiltStaticLibraryBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.PrebuiltStaticLibraryBinary.html)
- [`NativeComponentSpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeComponentSpec.html)
- [`NativeExecutableSpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeExecutableSpec.html)
- [`NativeLibrarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeLibrarySpec.html)
- [`NativeBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeBinarySpec.html)
- [`NativeExecutableBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeExecutableBinarySpec.html)
- [`NativeLibraryBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeLibraryBinarySpec.html)
- [`SharedLibraryBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.SharedLibraryBinarySpec.html)
- [`StaticLibraryBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.StaticLibraryBinarySpec.html)
- [`NativePlatform`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.platform.NativePlatform.html)
- [`BuildType`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.BuildType.html)
- [`Flavor`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.Flavor.html)
- [`Gcc`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.toolchain.Gcc.html)
- [`Clang`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.toolchain.Clang.html)
- [`VisualCpp`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.toolchain.VisualCpp.html)
- [`NativeTestSuiteSpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.test.NativeTestSuiteSpec.html)
- [`NativeTestSuiteBinarySpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.test.NativeTestSuiteBinarySpec.html)
- [`CUnitTestSuiteSpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.test.cunit.CUnitTestSuiteSpec.html)
- [`GoogleTestTestSuiteSpec`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.test.googletest.GoogleTestTestSuiteSpec.html)
- [`AssemblerSourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.assembler.AssemblerSourceSet.html)
- [`CSourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.c.CSourceSet.html)
- [`CppSourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.cpp.CppSourceSet.html)
- [`ObjectiveCSourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.objectivec.ObjectiveCSourceSet.html)
- [`ObjectiveCppSourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.objectivecpp.ObjectiveCppSourceSet.html)
- [`WindowsResourceSet`](https://docs.gradle.org/current/dsl/org.gradle.language.rc.WindowsResourceSet.html)
- [`VisualStudioProject`](https://docs.gradle.org/current/dsl/org.gradle.ide.visualstudio.VisualStudioProject.html)
- [`VisualStudioSolution`](https://docs.gradle.org/current/dsl/org.gradle.ide.visualstudio.VisualStudioSolution.html)
- [`NativeExecutable`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeExecutable.html)
- [`NativeLibrary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeLibrary.html)
- [`NativeBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeBinary.html)
- [`NativeExecutableBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.NativeExecutableBinary.html)
- [`SharedLibraryBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.SharedLibraryBinary.html)
- [`StaticLibraryBinary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.StaticLibraryBinary.html)
- Native component task types
- [`CppCompile`](https://docs.gradle.org/current/dsl/org.gradle.language.cpp.tasks.CppCompile.html)
- [`CCompile`](https://docs.gradle.org/current/dsl/org.gradle.language.c.tasks.CCompile.html)
- [`Assemble`](https://docs.gradle.org/current/dsl/org.gradle.language.assembler.tasks.Assemble.html)
- [`ObjectiveCCompile`](https://docs.gradle.org/current/dsl/org.gradle.language.objectivec.tasks.ObjectiveCCompile.html)
- [`ObjectiveCppCompile`](https://docs.gradle.org/current/dsl/org.gradle.language.objectivecpp.tasks.ObjectiveCppCompile.html)
- [`WindowsResourceCompile`](https://docs.gradle.org/current/dsl/org.gradle.language.rc.tasks.WindowsResourceCompile.html)
- [`LinkExecutable`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.tasks.LinkExecutable.html)
- [`LinkSharedLibrary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.tasks.LinkSharedLibrary.html)
- [`CreateStaticLibrary`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.tasks.CreateStaticLibrary.html)
- [`InstallExecutable`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.tasks.InstallExecutable.html)
- [`RunTestExecutable`](https://docs.gradle.org/current/dsl/org.gradle.nativeplatform.test.tasks.RunTestExecutable.html)


### Gradle常用命令列表



```java
gradle build
gradle test
gradle compilejava
gradle jar
gradle clean
gradle jar
gradle init
//生成wrapper包
gradle wrapper
gradle compilejava
gradle javadoc
gradle dependencies
gradle help
gradle check
gradle test
gradle -v
gradle --info
gradle --debug
gradle --help
//列出可执行的所有任务(即查看可以执行的命令)
gradle tasks
gradle -q tasks
```

### 包的依赖



```java
dependencies {
    compile group: 'commons-collections', name: 'commons-collections', version: '3.2'
    testCompile group: 'junit', name: 'junit', version: '4.+'
}
```



### 依赖仓库



```java
repositories {
    mavenLocal()
    mavenCentral()
}
```



# Gradle任务操作

====以下内容转载自：[http://blog.csdn.net/jjwwmlp456/article/details/41350413](http://blog.csdn.net/jjwwmlp456/article/details/41350413)



新建一个build.gradle文件，以下示例都编辑在build.gradle中。

说明：> 后 跟gradle命令。 -q参数会关闭一些命令中的log信息，如 gradle -q hello

### 定义一个任务，并执行



```java
task hello {
    doLast {
        println 'Hello world!\nadmin'
    }
}
```



> gradle hello

### 定义一个快捷任务



```java
task hello2 << {
        println 'admin\nHello world!'
    
}
```



> gradle hello2

<< 代表 doLast{}

### 使用Groovy 语言



```java
task upper << {
    String someString = 'mY_nAmE'
    println "Original: " + someString 
    println "Upper case: " + someString.toUpperCase()
}
```
> gradle upper



### 循环遍历



```java
task foreach << {
    10.times { println "the times is $it." }
}
```



> gradle foreach

10.times，循环条件<10；$it 表示循环变量

### 依赖其他任务



```java
task intro(dependsOn: hello) << {
    println "I'm Gradle---intro"
}
```
> gradle intro



依赖任务hello，会先调用hello，再执行当前task

### 依赖的任务定义在当前任务之后



```java
task taskX(dependsOn: 'taskY') << {
    println 'taskX'
}
task taskY << {
    println 'taskY'
}
```


> gradle taskX



依赖定义在后面的任务，需要在任务名上加单引号。 而上一个例子，加不加单引号都可以

### 动态任务



```java
4.times { counter ->
    task "task$counter" << {
        println "I'm task number $counter"
    }
}
```
> gradle task[0-4]



循环四次，生成4个task， 任务名分别为task0,task1,task2,task3。




### 动态任务依赖动态任务



```java
task0.dependsOn task2, task3
```
> gradle task0



当然动态依赖静态，静态依赖动态都是可以的

### 符号<< 和 doFirst、doLast



```java
task hello3 << {
    println 'Hello Earth'
}
hello3.doFirst {
    println 'Hello Venus'
}
hello3.doLast {
    println 'Hello Mars'
}
hello3 << {
    println 'Hello Jupiter'
}
```
> gradle hello3



可以有多个doFirst和doLast(或<<)。doFirtst先执行，doLast在其后。这里顺序就应为：Venus、Earth、Mars、Jupiter

### 使用外部任务的属性



```java
task myTask {
    ext.myProperty = "myValue"
}
task extraProps << {
    println myTask.myProperty
}
```
> gradle extraProps



### 使用ant任务

```java
task loadfile1 << {
    def files = file('../builds').listFiles().sort()
    files.each { File file ->
        if (file.isFile()) {
            ant.loadfile(srcFile: file, property: file.name)
            println " *** $file.name ***"
            println "${ant.properties[file.name]}"
        }
    }
}
```
> gradle loadfile1
调用任务ant.properties。这里输出了文件的内容

### 使用ant任务，并调用了Method

```java
task checksum << {
    fileList('../builds').each {File file ->
        ant.checksum(file: file, property: "cs_$file.name")
        println "$file.name Checksum: ${ant.properties["cs_$file.name"]}"
    }
}
task loadfile2 << {
    fileList('../builds/').each {File file ->
        ant.loadfile(srcFile: file, property: file.name)
        println "I'm fond of $file.name"
    }
}
File[] fileList(String dir) {
    file(dir).listFiles({file -> file.isFile() } as FileFilter).sort()
}

> gradle loadfile2

> gradle checksum
```
声明了method ---- fileList()
### 默认任务 

```java
defaultTasks 'clean', 'run'

task clean << {
    println 'Default Cleaning!'
}
task run << {
    println 'Default Running!'
}
task other << {
    println "I'm not a default task!"
}

> gradle
```
后不跟任务名即 执行默认任务
### Configure by DAG



```java
task distribution << {
    println "We build the zip with version=$version"
}
task release(dependsOn: 'distribution') << {
    println 'We release now'
}
gradle.taskGraph.whenReady {taskGraph ->
    if (taskGraph.hasTask(release)) {
        version = '1.0'
    } else {
        version = '1.0-SNAPSHOT'
    }
}
```



> gradle distribution

运行任务图中没有release-task，输出 We build the zip with version=1.0-SNAPSHOT

> release

运行任务图中有release-task，输出 We build the zip with version=1.0


===以上内容转载自：[http://blog.csdn.net/jjwwmlp456/article/details/41350413](http://blog.csdn.net/jjwwmlp456/article/details/41350413)








