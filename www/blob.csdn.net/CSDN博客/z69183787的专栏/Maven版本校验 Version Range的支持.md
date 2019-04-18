# Maven版本校验 Version Range的支持 - z69183787的专栏 - CSDN博客
2017年01月11日 11:53:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2775
Version Range规范：[http://maven.apache.org/enforcer/enforcer-rules/versionRanges.html](http://maven.apache.org/enforcer/enforcer-rules/versionRanges.html)
![](https://img-blog.csdn.net/20170111113710504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
private static boolean checkArtifact(SimpleArtifact simpleArtifact, Artifact artifact) {
        if (simpleArtifact.getGroupId().equals(artifact.getGroupId()) && simpleArtifact.getArtifactId().equals(artifact.getArtifactId())) {
            return checkVersion(artifact.getVersion(), simpleArtifact.getVersion());
        }
        return true;
    }
    //use version range to check version like [3.1,3.2)
    private static boolean checkVersion(String sourceVersion, String targetVersion) {
        if (!targetVersion.contains(")") && !targetVersion.contains("]")) {
            return targetVersion.equals(sourceVersion);
        } else {
            VersionRange targetVersionRange = null;
            try {
                targetVersionRange = VersionRange.createFromVersionSpec(targetVersion);
            } catch (InvalidVersionSpecificationException e) {
                log.error("Invalid target version: " + targetVersion);
            }
            return targetVersionRange.containsVersion(new DefaultArtifactVersion(sourceVersion));
        }
    }
```
CheckVersionNewMojo：
```java
package plugin;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.maven.artifact.Artifact;
import org.apache.maven.artifact.versioning.DefaultArtifactVersion;
import org.apache.maven.artifact.versioning.InvalidVersionSpecificationException;
import org.apache.maven.artifact.versioning.VersionRange;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;
import org.apache.maven.shared.artifact.filter.collection.ArtifactFilterException;
import org.apache.maven.shared.artifact.filter.collection.FilterArtifacts;
import java.util.*;
/**
 * Created by Administrator on 2016/10/31.
 * @goal checkNew
 * @phase process-resources
 * @requiresDependencyResolution compile
 */
public class CheckVersionNewMojo extends AbstractMojo {
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
            getLog().info("The version of following files are invalid. checkNew");
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
                if (!checkArtifact(ArtifactPojo, artifact))
                    brokenArtifacts.put(ArtifactPojo, artifact);
            }
        }
        return brokenArtifacts;
    }
    private boolean checkArtifact(ArtifactPojo artifactPojo, Artifact artifact) {
        if (artifactPojo.getGroupId().equals(artifact.getGroupId()) && artifactPojo.getArtifactId().equals(artifact.getArtifactId())) {
            return checkVersion(artifact.getVersion(), artifactPojo.getVersion());
        }
        return true;
    }
    //use version range to check version like [3.1,3.2)
    private boolean checkVersion(String sourceVersion, String targetVersion) {
        if (!targetVersion.contains(")") && !targetVersion.contains("]")) {
            return targetVersion.equals(sourceVersion);
        } else {
            VersionRange targetVersionRange = null;
            try {
                targetVersionRange = VersionRange.createFromVersionSpec(targetVersion);
            } catch (InvalidVersionSpecificationException e) {
                getLog().error("Invalid target version: " + targetVersion);
            }
            return targetVersionRange.containsVersion(new DefaultArtifactVersion(sourceVersion));
        }
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
mvn com.zs:zs-dependency-check:checkNew -Dchecklist=http://dasdt=http
