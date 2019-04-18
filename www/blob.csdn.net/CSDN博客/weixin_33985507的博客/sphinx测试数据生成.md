# sphinx测试数据生成 - weixin_33985507的博客 - CSDN博客
2017年03月31日 14:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
import json
from random import sample, randint
from uuid import uuid4
def gen_random_words():
    with open("D:\\exp\\test_data\\dictionary.txt") as f:
        words = [word.strip() for word in f]
        f.close()
        # print "OK. words length:", len(words)
        return sample(words, 3000)
    return []
total_words = 0
def sample_words(search_words, random_words):
    global total_words
    sample_cnt = 1000
    for word in random_words:
        total_words += 1
        if len(search_words) < sample_cnt:
            search_words.append(word)
        else:
            if randint(1, total_words) <= sample_cnt:
                kick_off = randint(0, sample_cnt-1)
                search_words[kick_off] = word
def gen_an_event(words, search_words):
    query_words = sample(words, randint(1, 10))
    sample_words(search_words,query_words)
    title = " ".join(query_words)
    query_words = sample(words, randint(1, 100))
    sample_words(search_words,query_words)
    content = " ".join(query_words)
    event_data = {"title": title, "content": content}
    return event_data
if __name__ == "__main__":
    search_words = []
    for i in range(1):
        words = gen_random_words()
        lines_cnt = 500000
        es_out_put = [""]*lines_cnt
        for i in range(0, lines_cnt):
            event = gen_an_event(words, search_words)
            es_out_put[i] = "        (%d, 2, 9, NOW(), '%s', '%s'), \n" % (i+5, event["title"], event["content"])
        # print es_out_put
        # print splunk_out_put
        out_puts = [es_out_put]
        file_name = str(uuid4()) + ".txt"
        for i,dir_name in enumerate(["Sphinx"]):
            outfile = "D:\\test_data\\%s\\%s" % (dir_name, file_name)
            f = open(outfile, "w")
            for j in range(0, lines_cnt):
                f.write(out_puts[i][j])
            f.close()
            print outfile
    outfile = "D:\\test_data\\search_words2.txt"
    f = open(outfile, "w")
    f.write(json.dumps(search_words))
    f.close()
sql = '''
DROP TABLE IF EXISTS test.documents;
CREATE TABLE test.documents
(
        id                      INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
        group_id        INTEGER NOT NULL,
        group_id2       INTEGER NOT NULL,
        date_added      DATETIME NOT NULL,
        title           VARCHAR(255) NOT NULL,
        content         TEXT NOT NULL
);
REPLACE INTO test.documents ( id, group_id, group_id2, date_added, title, content ) VALUES
        ( 1, 1, 5, NOW(), 'test one', 'this is my test document number one. also checking search within phrases.' ),
        ( 2, 1, 6, NOW(), 'test two', 'this is my test document number two' ),
        ( 3, 2, 7, NOW(), 'another doc', 'this is another group' ),
        ( 4, 2, 8, NOW(), 'doc number four', 'this is to test groups' );
DROP TABLE IF EXISTS test.tags;
CREATE TABLE test.tags
(
        docid INTEGER NOT NULL,
        tagid INTEGER NOT NULL,
        UNIQUE(docid,tagid)
);
INSERT INTO test.tags VALUES
        (1,1), (1,3), (1,5), (1,7),
        (2,6), (2,4), (2,2),
        (3,15),
        (4,7), (4,40);
'''
```
