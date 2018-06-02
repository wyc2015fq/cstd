
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#include <iostream>
#include <fstream>
#include <sstream>

#endif
#include "FaceSqlite.h"
//#include "faceRecognizer.hpp"
//#include "socket/CommonFunction.h"


//using namespace std;

CFaceSqlite::CFaceSqlite()
{
	//CFileManager::creatdir(DIR_FACEDB);
		facedb.open("./facedb/face.db");
		const char* sqlstr = "create table faceinfo("  \
			"faceid	INT default -1," \
			"ftime	INT default -1," \
			"stime	INT default -1," \
			"imgid	INT default -1," \
			"age	INT default -1," \
			"gender	INT default -1," \
			"glass	INT default -1," \
			"smile	INT default -1," \
			"send   INT default 0," \
			"imgupload   INT default 0," \
			"sideface  INT default -1);";
		facedb.execDML(sqlstr);
		sqlstr = "create table faceimg(img	CHAR(200));";
		facedb.execDML(sqlstr);
}


CFaceSqlite::~CFaceSqlite()
{
	facedb.close();
}

CFaceSqlite faceSqlite;

int32_t DbSQLite_AddFaceData(const faceobj_t* face, const char *imgpath)
{
	int32_t faceid = face->id;
	time_t firsttime = face->enter_time;
	time_t endtime = face->left_time;
	int age = face->age;
	int gender = face->gender;
	char strbuf[200];
	CppSQLite3DB& facedb = faceSqlite.facedb;

	if (0) {
		FILE* pf = fopen("E://log.txt", "a");
		fprintf(pf, "age=%d\n", age);
    fprintf(pf, "gender=%d\n", gender);
    fclose(pf);
  }
  _snprintf(strbuf, 200, "insert into faceimg values ('%s');", imgpath);
  facedb.execDML(strbuf);
  _snprintf(strbuf, 200, "insert into faceinfo (id, ftime, stime, imgid, sideface, age, gender, glass, smile, send, imgupload) values (%d, %d, %d, %lld, %d, %d, %d, -1, -1, 0, 0);",
    faceid, (int32_t)firsttime, (int32_t)endtime, facedb.lastRowId(), face->sideface, age, gender);
  facedb.execDML(strbuf);

	return 0;
}
