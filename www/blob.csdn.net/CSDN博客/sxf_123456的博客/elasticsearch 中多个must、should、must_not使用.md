# elasticsearch 中多个must、should、must_not使用 - sxf_123456的博客 - CSDN博客
2017年12月27日 14:51:35[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：22329
elasticsearch 中多个must、should、must_not使用
在A成立的情况下，查询B成立或者C成立
GET gather-034-20171225/_search
{
  "query": {
    "bool": {
      "must": [
        {"range": {
          "recive_time": {
            "gte": "2017-12-25T01:00:00.000Z",
            "lte": "2017-12-25T02:10:00.000Z"
          }
        }},
        {
          "bool": {
            "should": [
              {"range": {
                "live_delay": {
                  "gte": 1500
                }
              }},
              {
                "range": {
                  "stream_break_count.keyword": {
                    "gte": 1
                  }
                }
              }
            ]
          }
        }
      ]
    }
  }
}
最终
GET m-stb-2017.12.12/_search
{
  "query": {
    "bool": {
      "must": [
        {"exists":{"field":"iptv_ua"}},
        {"term": {
          "2fg_company_name.keyword": {
            "value": "双流分公司"
          }
        }}
      ]
    }
  },
  "_source": ["olt_ip","2fg_company_name","iptv_ua"]
}
GET gather-014-20171225/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "alarm_time": {
              "gte": "2017-12-25T01:00:00.000Z",
              "lte": "2017-12-25T02:10:00.000Z"
            }
          }
        },
        {
          "bool": {
            "should": [
              {
                "term": {
                  "alarm_name": {
                    "value": "3"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "7"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "8"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "9"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "10"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "11"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "12"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "13"
                  }
                }
              },
              {
                "term": {
                  "alarm_name": {
                    "value": "14"
                  }
                }
              }
            ]
          }
        }
      ]
    }
  },
  "_source": ["alarm_name","alarm_time","alarm_type","info"]
}
GET gather-038/_search
{
  "query": {
   "bool": {
     "must": [
       {"term": {
     "alarm_name": {
       "value": "16"}}},
       {
       "range": {
      "alarm_time": {
        "gte": "2017-12-25T01:00:00.000Z",
        "lte": "2017-12-25T02:10:00.000Z"
       }
       }
      }
     ]
   }
  }
}
GET gather-034-20171225/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "recive_time": {
              "gte": "2017-12-25T01:00:00.000Z",
              "lte": "2017-12-25T02:10:00.000Z"
            }
          }
        },
        {
          "bool": {
            "should": [
              {
                "range": {
                  "live_delay": {
                    "gt": 1500
                  }
                }
              },
              {
                "bool": {
                  "should": [
                    {
                      "range": {
                        "flr.keyword": {
                          "gt": 0.5
                        }
                      }
                    },
                    {
                      "range": {
                        "stream_break_count.keyword": {
                          "gt": 1
                        }
                      }
                    },
                    {
                      "range": {
                        "frozen_count.keyword": {
                          "gt": 1
                        }
                      }
                    },
                    {
                      "bool": {
                        "must": [
                          {
                            "range": {
                              "mlr_avg.keyword": {
                                "gte": 6
                              }
                            }
                          },
                          {
                            "range": {
                              "mlr_count.keyword": {
                                "lte": 3
                              }
                            }
                          }
                        ]
                      }
                    }
                  ]
                }
              }
            ]
          }
        }
      ]
    }
  },
  "_source": ["recive_time","itv_account"]
}
1.   查看gather-034在9:00到10：10内用户性能指标的告警情况
符合直播告警要求的用户数量时间分布
直播告警条件，满足以下一种即为直播告警：
a     直播时延值大于1500
b    live_video_type存在且至少存在以下一种情况：
a)      二层丢帧：二层丢帧值大于0.5
b)     断流：断流次数大于1
c)      卡顿：卡顿次数大于1
d)     媒体丢包：媒体丢包率平均值>=6且丢包次数<=3
GET gather-034-20171225/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "recive_time": {
              "gte": "2017-12-25T01:00:00.000Z",
              "lte": "2017-12-25T02:10:00.000Z"
            }
          }
        },
        {
          "bool": {
            "should": [
              {
                "range": {
                  "live_delay": {
                    "gt": 1500
                  }
                }
              },
              {
                "bool": {
                  "should": [
                    {
                      "range": {
                        "flr.keyword": {
                          "gt": 0.5
                        }
                      }
                    },
                    {
                      "range": {
                        "stream_break_count.keyword": {
                          "gt": 1
                        }
                      }
                    },
                    {
                      "range": {
                        "frozen_count.keyword": {
                          "gt": 1
                        }
                      }
                    },
                    {
                      "bool": {
                        "must": [
                          {
                            "range": {
                              "mlr_avg.keyword": {
                                "gte": 6
                              }
                            }
                          },
                          {
                            "range": {
                              "mlr_count.keyword": {
                                "lte": 3
                              }
                            }
                          }
                        ]
                      }
                    }
                  ]
                }
              }
            ]
          }
        }
      ]
    }
  },
  "_source": ["recive_time","itv_account"]
}
