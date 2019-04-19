# Maven自定义镜像Nexus的setting配置(.m2目录下面) - 零度的博客专栏 - CSDN博客
2017年07月07日 14:27:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：1007
```
<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0" 
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">
  
  
  <pluginGroups>
  </pluginGroups>
  <proxies>
  </proxies>
  <servers>
    
     	<server>
		<id>mytomcat7</id>
		<username>admin</username>
		<password>admin</password>
        </server>
        <server>
               <id>nexus-snapshots</id>
               <username>deployment</username>
               <password>deployment123</password>
        </server>
	<server>
              <id>nexus-release</id>
              <username>deployment</username>
              <password>deployment123</password>
        </server>
  </servers>
 
  <mirrors>
      <mirror>
         <id>nexusMirror</id>
         <mirrorOf>*</mirrorOf>
         <name>Human Readable Name for this Mirror.</name>
         <url>http://192.168.14.19:8081/nexus/content/groups/public/</url>
      </mirror>
  </mirrors>
  
 
  <profiles>
  
       <profile>
           <id>nexusProfile</id>
           <repositories>
               <repository>
                     <id>nexus</id>
                     <name>Repository for nexus</name>
                     <url>http://192.168.14.19:8081/nexus/content/groups/public/</url>
                     <releases>
		           <enabled>true</enabled>
		      </releases>
		      <snapshots>
		           <enabled>true</enabled>
		      </snapshots>
               </repository>
           </repositories>
       </profile>
  </profiles>
  <activeProfiles>
        <activeProfile>nexusProfile</activeProfile>
  </activeProfiles>
</settings>
```
