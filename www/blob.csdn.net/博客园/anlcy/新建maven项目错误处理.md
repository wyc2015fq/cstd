
# 新建maven项目错误处理 - anlcy - 博客园






# [新建maven项目错误处理](https://www.cnblogs.com/camilla/p/8260127.html)
1.如果新建maven 项目确省maven dependencies,则在存在问题工程的.classpath末尾 加上
<classpathentry kind="con" path="org.eclipse.m2e.MAVEN2_CLASSPATH_CONTAINER">
<attributes>
<attribute name="maven.pomderived" value="true"/>
<attribute name="org.eclipse.jst.component.dependency" value="/WEB-INF/lib"/>
</attributes>
</classpathentry>
2.MAVEN项目报错：Project configuration is not up-to-date with pom.xml. Run Maven->Update Project
一般是[pom](https://www.baidu.com/s?wd=pom&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YYPyu-uj0YuHfsPA7-uW990ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHm1rjRYn10zPjmznjD3PHD3n0).xml被修改了，但是工程还没有同步过来导致的，按照提示说的，在工程上右击执行“Maven->Update Project“项进行同步更新下配置就正常了。





