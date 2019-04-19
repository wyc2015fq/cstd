# ES支持汉字、拼音、简拼搜索的自定义分词器 - 零度的博客专栏 - CSDN博客
2018年06月29日 12:06:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：723
                {
  "settings": {
    "refresh_interval": "2s",
    "number_of_shards": 5,
    "number_of_replicas": 1,
    "analysis": {
      "filter": {
        "edge_ngram_filter": {
          "type": "edge_ngram",
          "min_gram": 2,
          "max_gram": 20
        },
        "pinyin_jianpin": {
          "type": "pinyin",
          "first_letter": "none",
          "padding_char": ""
        },
        "pinyin_simple_filter": {
          "type": "pinyin",
          "keep_first_letter": true,
          "keep_separate_first_letter": true,
          "keep_full_pinyin": false,
          "keep_original": false,
          "limit_first_letter_length": 20,
          "lowercase": true
        },
        "pinyin_full_filter": {
          "type": "pinyin",
          "keep_first_letter": false,
          "keep_separate_first_letter": false,
          "keep_full_pinyin": true,
          "none_chinese_pinyin_tokenize": true,
          "keep_original": false,
          "limit_first_letter_length": 20,
          "lowercase": true
        }
      },
      "tokenizer": {
        "ik_smart": {
          "type": "ik",
          "use_smart": true
        }
      },
      "analyzer": {
        "ngramIndexAnalyzer": {
          "type": "custom",
          "tokenizer": "ik_max_word",
          "filter": [
            "edge_ngram_filter",
            "lowercase"
          ]
        },
        "ngramSearchAnalyzer": {
          "type": "custom",
          "tokenizer": "whitespace",
          "filter": [
            "lowercase"
          ]
        },
        "ikIndexAnalyzer": {
          "type": "custom",
          "tokenizer": "ik"
        },
        "ikSearchAnalyzer": {
          "type": "custom",
          "tokenizer": "ik"
        },
        "pinyinSimpleIndexAnalyzer": {
          "tokenizer": "ik_max_word",
          "filter": [
            "pinyin_simple_filter",
            "edge_ngram_filter",
            "lowercase"
          ]
        },
        "pinyinSimpleSearchAnalyzer": {
          "tokenizer": "whitespace",
          "filter": [
            "pinyin_simple_filter",
            "lowercase"
          ]
        },
        "jianpinIndexAnalyzer": {
          "type": "custom",
          "tokenizer": "keyword",
          "filter": [
            "pinyin_first_letter",
            "edge_ngram_filter",
            "lowercase"
          ]
        },
        "jianpinSearchAnalyzer": {
          "type": "custom",
          "tokenizer": "whitespace",
          "filter": [
            "pinyin_first_letter",
            "lowercase"
          ]
        },
        "pinyinFullIndexAnalyzer": {
          "tokenizer": "keyword",
          "filter": [
            "pinyin_full_filter",
            "lowercase"
          ]
        },
        "pinyinFullSearchAnalyzer": {
          "tokenizer": "whitespace",
          "filter": [
            "pinyin_full_filter",
            "lowercase"
          ]
        }
      }
    }
  }
}     
