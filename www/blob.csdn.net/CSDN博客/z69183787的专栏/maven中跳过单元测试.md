# maven中跳过单元测试 - z69183787的专栏 - CSDN博客
2015年06月29日 22:12:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2555
你可能想要配置 Maven 使其完全跳过单元测试。 可能你有一个很大的系统，单元测试需要花好多分钟来完成，而你不想在生成最终输出前等单元测试完成。 你可能正工作在一个遗留系统上面，这个系统有一系列的失败的单元测试，你可能仅仅想要生成一个 JAR 而不是去修复所有的单元测试。 Maven 提供了跳过单元测试的能力，只需要使用 Surefire 插件的 `skip` 参数。 在命令行，只要简单的给任何目标添加 `maven.test.skip` 属性就能跳过测试：
$ mvn install -Dmaven.test.skip=true
...
[INFO] [compiler:testCompile]
[INFO] Not compiling test sources
[INFO] [surefire:test]
[INFO] Tests are skipped.
...
当 Surefire 插件到达 `test` 目标的时候，如果 `maven.test.skip` 设置为 `true` ，它就会跳过单元测试。 另一种配置 Maven 跳过单元测试的方法是给你项目的 `pom.xml` 添加这个配置。 你需要为你的 `build` 添加 `plugin` 元素。
<project>
  [...]
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-surefire-plugin</artifactId>
        <configuration>
          <skip>true</skip>
        </configuration>
      </plugin>
    </plugins>
  </build>
  [...]
</project>
