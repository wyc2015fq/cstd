# maven自定义插件 jenkis打包 jar包版本校验dependency-check-plugin:check - z69183787的专栏 - CSDN博客
2016年10月31日 13:05:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2122
**更新校验方式，使用 maven 自带 version range api**
**[http://blog.csdn.net/z69183787/article/details/54342057](http://blog.csdn.net/z69183787/article/details/54342057)**
**替换了version的compareTo方法，关于Version range api详见[http://maven.apache.org/ref/3.3.9/maven-artifact/apidocs/org/apache/maven/artifact/versioning/VersionRange.html](http://maven.apache.org/ref/3.3.9/maven-artifact/apidocs/org/apache/maven/artifact/versioning/VersionRange.html)。 **
在maven 打包时 根据设定的指定jar包版本 对项目中的版本进行校验，若低于设定版本，则抛出异常：
如何在自定义插件mojo中 获取项目下的所有 依赖
How to get all dependecy in a mojo ?
[http://maven.40175.n5.nabble.com/How-to-get-all-dependecy-in-a-mojo-td111541.html](http://maven.40175.n5.nabble.com/How-to-get-all-dependecy-in-a-mojo-td111541.html)
maven 插件使用annotation 的形式自动注入参数 或 设置 插件goal
${basedir} 项目根目录
${project.build.directory} 构建目录，缺省为target
${project.build.outputDirectory} 构建过程输出目录，缺省为target/classes
${project.build.finalName} 产出物名称，缺省为${project.artifactId}-${project.version}
${project.packaging} 打包类型，缺省为jar
${project.xxx} 当前pom文件的任意节点的内容
maven生命周期及goal等之间的关系：[http://blog.csdn.net/z69183787/article/details/52984602](http://blog.csdn.net/z69183787/article/details/52984602)
maven-mojo 标注及参数：[http://blog.csdn.net/z69183787/article/details/52984622](http://blog.csdn.net/z69183787/article/details/52984622)
Mojo类：goal为check，执行阶段为  process-resources
/**
 * Created by Administrator on 2016/10/31.
 * @goal check
 * @phase process-resources
 * @requiresDependencyResolution compile
 */
/**
     * 插件参数
     * @parameter expression="${checklist}"
     * @required
     */
pom.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.zs</groupId>
    <artifactId>zs-dependency-check</artifactId>
    <packaging>maven-plugin</packaging>
    <version>1.0-SNAPSHOT</version>
    <dependencies>
        <dependency>
            <groupId>org.apache.maven</groupId>
            <artifactId>maven-plugin-api</artifactId>
            <version>2.0</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>3.8.1</version>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-dependency-plugin</artifactId>
            <version>2.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>4.3</version>
        </dependency>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>1.1.43</version>
        </dependency>
    </dependencies>
</project>
```
ArtifactPojo：
```java
package plugin;
import org.apache.maven.artifact.Artifact;
import org.apache.maven.artifact.versioning.ArtifactVersion;
import org.apache.maven.artifact.versioning.DefaultArtifactVersion;
/**
 * Created by Administrator on 2016/10/31.
 */
public class ArtifactPojo implements Comparable<Artifact>
{
    private String groupId;
    private String artifactId;
    private String version;
    public ArtifactPojo()
    {
    }
    public ArtifactPojo(String groupId, String artifactId, String version)
    {
        this.groupId = groupId;
        this.artifactId = artifactId;
        this.version = version;
    }
    public int compareTo(Artifact artifact)
    {
        if ((artifact != null) && (getGroupId().equals(artifact.getGroupId())) &&
                (getArtifactId().equals(artifact.getArtifactId())))
        {
            return new DefaultArtifactVersion(getVersion()).compareTo(new DefaultArtifactVersion(artifact.getVersion()));
        }
        return 0;
    }
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ArtifactPojo that = (ArtifactPojo) o;
        if (!artifactId.equals(that.artifactId)) return false;
        if (!groupId.equals(that.groupId)) return false;
        if (!version.equals(that.version)) return false;
        return true;
    }
    @Override
    public int hashCode() {
        int result = groupId.hashCode();
        result = 31 * result + artifactId.hashCode();
        result = 31 * result + version.hashCode();
        return result;
    }
    public String getGroupId() {
        return this.groupId;
    }
    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }
    public String getArtifactId() {
        return this.artifactId;
    }
    public void setArtifactId(String artifactId) {
        this.artifactId = artifactId;
    }
    public String getVersion() {
        return this.version;
    }
    public void setVersion(String version) {
        this.version = version;
    }
    public String toString()
    {
        return this.groupId + ":" + this.artifactId + ":" + this.version;
    }
}
```
CheckVersionMojo：
```java
package plugin;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.maven.artifact.Artifact;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;
import org.apache.maven.shared.artifact.filter.collection.ArtifactFilterException;
import org.apache.maven.shared.artifact.filter.collection.FilterArtifacts;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.*;
/**
 * Created by Administrator on 2016/10/31.
 * @goal check
 * @phase process-resources
 * @requiresDependencyResolution compile
 */
public class CheckVersionMojo extends AbstractMojo {
    /**
     * @parameter expression="${project}"
     * @required
     * @readonly
     */
    private MavenProject project;
    //http://123.123.123
    /**
     * 插件参数
     * @parameter expression="${checklist}"
     * @required
     */
    private String checklist;
    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        System.out.println("checkList := " + checklist);
        if (this.project == null) {
            throw new MojoExecutionException("Can not find a Maven project.");
        }
        if (("".equals(this.checklist)) || (null == this.checklist)) {
            getLog().info("Checklist is empty, bye.");
            return;
        }
        getLog().debug("checklist:" + this.checklist);
        if (this.checklist.startsWith("http://")) {
            this.checklist = getChecklistFromUrl(this.checklist);
            getLog().debug("get checklist from url:" + this.checklist);
        }
        Map<ArtifactPojo,Artifact> brokenArtifacts = getBrokenArtifacts();
        Artifact artifact = null;
        getLog().debug("The count of invalid artifacts: " + brokenArtifacts.size());
        if (!brokenArtifacts.isEmpty()) {
            getLog().info("The version of following files are invalid.");
            Set<ArtifactPojo> brokenArtifactsSet = brokenArtifacts.keySet();
            for (ArtifactPojo sa : brokenArtifactsSet) {
                artifact = (Artifact)brokenArtifacts.get(sa);
                getLog().error(sa.getGroupId() + ":" + sa.getArtifactId() + " required version[" + sa.getVersion() + "]," +
                        " actual version[" + artifact.getVersion() + "]");
            }
            throw new MojoFailureException("please upgrade to the specified version.");
        }
    }
    private String getChecklistFromUrl(String checklistUrl) {
        CloseableHttpClient httpclient = HttpClients.createDefault();
        HttpGet httpGet = new HttpGet(checklistUrl);
        String list = "com.alibaba:fastjson:1.2.0";
        return list;
//        try {
//            CloseableHttpResponse response = httpclient.execute(httpGet);
//            InputStream is = response.getEntity().getContent();
//            BufferedReader br = new BufferedReader(new InputStreamReader(is));
//
//            list = br.readLine();
//            list = list.replace("\"", "");
//            response.close();
//            br.close();
//        } catch (IOException e) {
//            throw new RuntimeException("Can not get checklist from `" + checklistUrl + "`", e);
//        }
//        return list;
    }
    private Set<ArtifactPojo> fromChecklist(String checklist) {
        Set simpleArtifacts = new HashSet();
        String[] toBeChecked = checklist.split(",");
        String[] splitedArray = null;
        for (int i = 0; i < toBeChecked.length; i++) {
            splitedArray = toBeChecked[i].split(":");
            if (splitedArray.length < 3)
                getLog().warn("Invalid artifact: " + toBeChecked[i] + ", available format is groupId:artifactId:verion");
            else {
                simpleArtifacts.add(new ArtifactPojo(splitedArray[0], splitedArray[1], splitedArray[2]));
            }
        }
        return simpleArtifacts;
    }
    private Map<ArtifactPojo, Artifact> getBrokenArtifacts() throws MojoExecutionException
    {
        Set simpleArtifacts = fromChecklist(this.checklist);
        Set artifacts = this.project.getArtifacts();
        System.out.println(artifacts);
        FilterArtifacts filter = new FilterArtifacts();
        filter.clearFilters();
        Set unMarkedArtifacts = null;
        try {
            unMarkedArtifacts = filter.filter(artifacts);
        } catch (ArtifactFilterException e) {
            throw new MojoExecutionException(e.getMessage(), e);
        }
        ArtifactPojo ArtifactPojo = null;
        Artifact artifact = null;
        Iterator it;
        Map brokenArtifacts = new HashMap();
        for (Iterator sait = simpleArtifacts.iterator(); sait.hasNext(); ) {
            ArtifactPojo = (ArtifactPojo)sait.next();
            for (it = unMarkedArtifacts.iterator(); it.hasNext(); ) {
                artifact = (Artifact)it.next();
                if (ArtifactPojo.compareTo(artifact) > 0)
                    brokenArtifacts.put(ArtifactPojo, artifact);
            }
        }
        return brokenArtifacts;
    }
    public MavenProject getProject() {
        return project;
    }
    public void setProject(MavenProject project) {
        this.project = project;
    }
    public String getChecklist() {
        return checklist;
    }
    public void setChecklist(String checklist) {
        this.checklist = checklist;
    }
}
```
运行：
mvn clean install 将插件install入本地maven仓库
校验：
mvn com.zs:zs-dependency-check:check -Dchecklist=http://dasdt=http
checklist可设置为http请求，返回
"com.xxxx.a:a-a:1.1.4,com.xxxx.b:b-b:1.6.4,groupId:artifactId:version"
该格式分割的指定版本jar包
```
E:\zone\practice\zs-dependency-check>mvn com.zs:zs-dependency-check:check -Dchecklist=http://123.123
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------------------------------------------------------------
[INFO] Building zs-dependency-check 1.0-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO]
[INFO] --- zs-dependency-check:1.0-SNAPSHOT:check (default-cli) @ zs-dependency-check ---
checkList := http://123.123
[org.apache.maven:maven-plugin-api:jar:2.0:compile, org.apache.maven.plugins:maven-dependency-plugin:jar:2.1:compile, org.apache.maven:maven-artifact:jar:2.0.9:compile, org.apache.maven:maven-project:jar:2.0.9:compile, org.apache.maven:maven-settings:j
ar:2.0.9:compile, org.apache.maven:maven-profile:jar:2.0.9:compile, org.apache.maven:maven-artifact-manager:jar:2.0.9:compile, org.apache.maven:maven-plugin-registry:jar:2.0.9:compile, org.apache.maven:maven-model:jar:2.0.9:compile, org.apache.maven:ma
ven-core:jar:2.0.9:compile, org.apache.maven:maven-plugin-parameter-documenter:jar:2.0.9:compile, xml-apis:xml-apis:jar:1.0.b2:compile, org.apache.maven.wagon:wagon-provider-api:jar:1.0-beta-2:compile, org.apache.maven:maven-repository-metadata:jar:2.0
.9:compile, org.apache.maven:maven-error-diagnostics:jar:2.0.9:compile, commons-cli:commons-cli:jar:1.0:compile, org.apache.maven:maven-plugin-descriptor:jar:2.0.9:compile, org.codehaus.plexus:plexus-interactivity-api:jar:1.0-alpha-4:compile, org.apach
e.maven:maven-monitor:jar:2.0.9:compile, classworlds:classworlds:jar:1.1:compile, org.codehaus.plexus:plexus-archiver:jar:1.0-alpha-9:compile, org.codehaus.plexus:plexus-utils:jar:1.4.6:compile, org.apache.maven.shared:file-management:jar:1.1:compile,
org.apache.maven.shared:maven-shared-io:jar:1.0:compile, org.codehaus.plexus:plexus-container-default:jar:1.0-alpha-9-stable-1:compile, org.apache.maven.shared:maven-dependency-analyzer:jar:1.1:compile, asm:asm:jar:3.0:compile, org.apache.maven.shared:
maven-dependency-tree:jar:1.2:compile, org.apache.maven.shared:maven-common-artifact-filters:jar:1.0:compile, org.apache.maven.shared:maven-plugin-testing-harness:jar:1.1:compile, org.codehaus.plexus:plexus-io:jar:1.0-alpha-1:compile, org.apache.maven.
reporting:maven-reporting-api:jar:2.0.6:compile, org.apache.maven.doxia:doxia-sink-api:jar:1.0-alpha-7:compile, org.apache.maven.reporting:maven-reporting-impl:jar:2.0.4:compile, commons-validator:commons-validator:jar:1.2.0:compile, commons-beanutils:
commons-beanutils:jar:1.7.0:compile, commons-digester:commons-digester:jar:1.6:compile, commons-collections:commons-collections:jar:2.1:compile, org.apache.maven.doxia:doxia-core:jar:1.0-alpha-7:compile, oro:oro:jar:2.0.7:compile, org.apache.maven.doxi
a:doxia-site-renderer:jar:1.0-alpha-7:compile, org.codehaus.plexus:plexus-i18n:jar:1.0-beta-6:compile, org.codehaus.plexus:plexus-velocity:jar:1.1.2:compile, plexus:plexus-utils:jar:1.0.2:compile, commons-logging:commons-logging-api:jar:1.0.4:compile,
velocity:velocity:jar:1.4:compile, org.apache.maven.doxia:doxia-decoration-model:jar:1.0-alpha-7:compile, org.apache.httpcomponents:httpclient:jar:4.3:compile, org.apache.httpcomponents:httpcore:jar:4.3:compile, commons-logging:commons-logging:jar:1.1.
3:compile, commons-codec:commons-codec:jar:1.6:compile, com.alibaba:fastjson:jar:1.1.43:compile]
[INFO] The version of following files are invalid.
<span style="color:#ff0000;"><strong>[ERROR] com.alibaba:fastjson required version[1.2.0], actual version[1.1.43]</strong></span>
[INFO] ------------------------------------------------------------------------
[INFO] BUILD FAILURE
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 1.034s
[INFO] Finished at: Mon Oct 31 17:58:12 CST 2016
[INFO] Final Memory: 11M/219M
[INFO] ------------------------------------------------------------------------
[ERROR] Failed to execute goal com.zs:zs-dependency-check:1.0-SNAPSHOT:check (default-cli) on project zs-dependency-check: please upgrade to the specified version. -> [Help 1]
[ERROR]
[ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR]
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoFailureException
```
git：
http://git.oschina.net/zhou2404/practice
