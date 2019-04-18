# Oracle10g - 正则表达式  - ljx0305的专栏 - CSDN博客
2009年07月05日 20:58:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：643
-- REGEXP_LIKE --
function REGEXP_LIKE (srcstr   VARCHAR2 CHARACTER SET ANY_CS,
                      pattern  VARCHAR2 CHARACTER SET srcstr%CHARSET,
                      modifier VARCHAR2 DEFAULT NULL)
  return BOOLEAN;
  pragma FIPSFLAG('REGEXP_LIKE', 1452);
function REGEXP_LIKE (srcstr   CLOB CHARACTER SET ANY_CS,
                      pattern  VARCHAR2 CHARACTER SET srcstr%CHARSET,
                      modifier VARCHAR2 DEFAULT NULL)
  return BOOLEAN;
  pragma FIPSFLAG('REGEXP_LIKE', 1452);
-- REGEXP_INSTR --
function REGEXP_INSTR(srcstr      VARCHAR2 CHARACTER SET ANY_CS,
                      pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                      position    PLS_INTEGER := 1,
                      occurrence  PLS_INTEGER := 1,
                      returnparam PLS_INTEGER := 0,
                      modifier    VARCHAR2 DEFAULT NULL)
  return PLS_INTEGER;
  pragma FIPSFLAG('REGEXP_INSTR', 1452);
function REGEXP_INSTR(srcstr      CLOB CHARACTER SET ANY_CS,
                      pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                      position    INTEGER := 1,
                      occurrence  INTEGER := 1,
                      returnparam PLS_INTEGER := 0,
                      modifier    VARCHAR2 DEFAULT NULL)
  return INTEGER;
  pragma FIPSFLAG('REGEXP_INSTR', 1452);
-- REGEXP_SUBSTR --
function REGEXP_SUBSTR(srcstr      VARCHAR2 CHARACTER SET ANY_CS,
                       pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                       position    PLS_INTEGER := 1,
                       occurrence  PLS_INTEGER := 1,
                       modifier    VARCHAR2 DEFAULT NULL)
  return VARCHAR2 CHARACTER SET srcstr%CHARSET;
  pragma FIPSFLAG('REGEXP_SUBSTR', 1452);
function REGEXP_SUBSTR(srcstr      CLOB CHARACTER SET ANY_CS,
                       pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                       position    INTEGER := 1,
                       occurrence  INTEGER := 1,
                       modifier    VARCHAR2 DEFAULT NULL)
  return CLOB CHARACTER SET srcstr%CHARSET;
  pragma FIPSFLAG('REGEXP_SUBSTR', 1452);
-- REGEXP_REPLACE --
function REGEXP_REPLACE(srcstr      VARCHAR2 CHARACTER SET ANY_CS,
                        pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                        replacestr  VARCHAR2 CHARACTER SET srcstr%CHARSET
                                      DEFAULT NULL,
                        position    PLS_INTEGER := 1,
                        occurrence  PLS_INTEGER := 0,
                        modifier    VARCHAR2 DEFAULT NULL)
  return VARCHAR2 CHARACTER SET srcstr%CHARSET;
  pragma FIPSFLAG('REGEXP_REPLACE', 1452);
function REGEXP_REPLACE(srcstr      CLOB CHARACTER SET ANY_CS,
                        pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                        replacestr  CLOB CHARACTER SET srcstr%CHARSET
                                      DEFAULT NULL,
                        position    INTEGER := 1,
                        occurrence  INTEGER := 0,
                        modifier    VARCHAR2 DEFAULT NULL)
  return CLOB CHARACTER SET srcstr%CHARSET;
  pragma FIPSFLAG('REGEXP_REPLACE', 1452);
function REGEXP_REPLACE(srcstr      CLOB CHARACTER SET ANY_CS,
                        pattern     VARCHAR2 CHARACTER SET srcstr%CHARSET,
                        replacestr  VARCHAR2 CHARACTER SET srcstr%CHARSET
                                      DEFAULT NULL,
                        position    INTEGER := 1,
                        occurrence  INTEGER := 0,
                        modifier    VARCHAR2 DEFAULT NULL)
  return CLOB CHARACTER SET srcstr%CHARSET;
  pragma FIPSFLAG('REGEXP_REPLACE', 1452);
-- End REGEXP Support --
引用:http://blog.chinaunix.net/u/30637/showart_1981359.html
