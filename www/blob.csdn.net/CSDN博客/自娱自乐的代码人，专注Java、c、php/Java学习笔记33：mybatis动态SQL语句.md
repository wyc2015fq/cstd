# Java学习笔记33：mybatis动态SQL语句 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年03月13日 13:06:18[initphp](https://me.csdn.net/initphp)阅读数：1144
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









**一 if标签**

<select id=" getStudentListLikeName " parameterType="StudentEntity" resultMap="studentResultMap">

 SELECT * from STUDENT_TBL ST

 <**if **test="studentName!=null and studentName!='' ">

  WHERE ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName}),'%')

 </if>

</select>





**二 where标签**

<select id="getStudentListWhere" parameterType="StudentEntity" resultMap="studentResultMap">

 SELECT * from STUDENT_TBL ST
**<where>**  <if test="studentName!=null and studentName!='' ">

   ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName}),'%')

  </if>

  <if test="studentSex!= null and studentSex!= '' ">

   AND ST.STUDENT_SEX = #{studentSex}

  </if>

 </where>

</select>

如果它包含的标签中有返回值的话就插入一个where。此外如果标签返回的内容是以AND或OR开头的，则它会剔除掉。




**三 set 标签**

使用set+if标签修改后，如果某项为null则不进行更新，而是保持数据库原值

<update id="updateStudent" parameterType="StudentEntity">

 UPDATE STUDENT_TBL
** <set>**  <if test="studentName!=null and studentName!='' ">

   STUDENT_TBL.STUDENT_NAME = #{studentName},

  </if>

  <if test="studentSex!=null and studentSex!='' ">

   STUDENT_TBL.STUDENT_SEX = #{studentSex},

  </if>

  <if test="studentBirthday!=null ">

   STUDENT_TBL.STUDENT_BIRTHDAY = #{studentBirthday},

  </if>

  <if test="classEntity!=null and classEntity.classID!=null and classEntity.classID!='' ">

   STUDENT_TBL.CLASS_ID = #{classEntity.classID}

  </if>

 </set>

 WHERE STUDENT_TBL.STUDENT_ID = #{studentID};

</update>




**四 trim标签**



trim是更灵活的去处多余关键字的标签，他可以实践where和set的效果。


where例子的等效trim语句

<select id="getStudentListWhere" parameterType="StudentEntity" resultMap="studentResultMap">

 SELECT * from STUDENT_TBL ST
**<trim prefix="WHERE" prefixOverrides="AND|OR">**  <if test="studentName!=null and studentName!='' ">

   ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName}),'%')

  </if>

  <if test="studentSex!= null and studentSex!= '' ">

   AND ST.STUDENT_SEX = #{studentSex}

  </if>

 </trim>

</select>



set例子的等效trim语句

<update id="updateStudent" parameterType="StudentEntity">

 UPDATE STUDENT_TBL
**<trim prefix="SET" suffixOverrides=",">**  <if test="studentName!=null and studentName!='' ">

   STUDENT_TBL.STUDENT_NAME = #{studentName},

  </if>

  <if test="studentSex!=null and studentSex!='' ">

   STUDENT_TBL.STUDENT_SEX = #{studentSex},

  </if>

  <if test="studentBirthday!=null ">

   STUDENT_TBL.STUDENT_BIRTHDAY = #{studentBirthday},

  </if>

  <if test="classEntity!=null and classEntity.classID!=null and classEntity.classID!='' ">

   STUDENT_TBL.CLASS_ID = #{classEntity.classID}

  </if>

 </trim>

 WHERE STUDENT_TBL.STUDENT_ID = #{studentID};

</update>





**五 choose (when, otherwise)**

有时候并不想应用所有的条件，而只是想从多个选项中选择一个。MyBatis提供了choose 元素，按顺序判断when中的条件出否成立，**如果有一个成立，则choose结束**。当choose中所有when的条件都不满则时，则执行otherwise中的sql。类似于Java 的switch语句，choose为switch，when为case，otherwise则为default。



<select id="getStudentListChooseEntity" parameterType="StudentEntity" resultMap="studentResultMap">

 SELECT * from STUDENT_TBL ST

 <where>

  <choose>

   <when test="studentName!=null and studentName!='' ">

    ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName}),'%')

   </when>

   <when test="studentSex!= null and studentSex!= '' ">

    AND ST.STUDENT_SEX = #{studentSex}

   </when>

   <when test="studentBirthday!=null">

    AND ST.STUDENT_BIRTHDAY = #{studentBirthday}

   </when>

   <when test="classEntity!=null and classEntity.classID !=null and classEntity.classID!='' ">

    AND ST.CLASS_ID = #{classEntity.classID}

   </when>

   <otherwise></otherwise>

  </choose>

 </where>

</select>





**六 foreach**

对于动态SQL 非常必须的，主是要迭代一个集合，通常是用于IN 条件。List实例将使用“list”做为键，数组实例以“array”做为键。


1 参数为list实例的写法

SqlMapper.xml

<select id="getStudentListByClassIDs" resultMap="studentResultMap">

 SELECT * FROM STUDENT_TBL ST

  WHERE ST.CLASS_ID IN 

  <foreach collection="list" item="classList"  open="(" separator="," close=")">

   #{classList}

  </foreach> 

</select>


Java

List<String> classList = new ArrayList<String>();

classList.add("20000002");

classList.add("20000003");

List<StudentEntity> studentList = studentMapper.getStudentListByClassIDs(classList);

for(StudentEntity entityTemp : studentList){

 System.out.println(entityTemp.toString());

}



2 参数为Array实例的写法


SqlMapper.xml

<select id="getStudentListByClassIDs" resultMap="studentResultMap">

 SELECT * FROM STUDENT_TBL ST

  WHERE ST.CLASS_ID IN 

  <foreach collection="array" item="ids"  open="(" separator="," close=")">

   #{ids}

  </foreach>

</select>


Java

String[] ids = new String[2];

ids[0] = "20000002";

ids[1] = "20000003";

List<StudentEntity> studentList = studentMapper.getStudentListByClassIDs(ids);

for(StudentEntity entityTemp : studentList){

 System.out.println(entityTemp.toString());

}


