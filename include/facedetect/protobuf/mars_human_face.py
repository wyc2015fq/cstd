#!/usr/bin/env python

import os
import json
import urllib
import universal_report_pb2
import human_face_report_pb2

uni_log = universal_report_pb2.UniversalReportRequest()
uni_log.id = 'uni-log-id-2'
uni_log.timestamp = 1516158077000
uni_log.message_name = 'rongyi.face.log.UserFaceInfo'

faces = human_face_report_pb2.UserFaceInfo()
faces.id = 'face-log-id-1'
faces.timestamp = 1516158077000
faces.mall_id = 'mall-id-1'
faces.terminal_id = 'RYW-201311144'
face = faces.face.add()
face.profile.begin_age=23
face.profile.end_age=23
face.profile.gender=human_face_report_pb2.GENDER_MALE
face.enter_time = 1516158047
face.leave_time = 1516158070
faces_str = faces.SerializeToString()
uni_log.message_data.append(faces_str)
uni_log_str = uni_log.SerializeToString()
#request = urllib2.Request('http://192.168.10.192:28080/ad_content?json=1')
headers = {'Content-Type':'application/octet-stream'}
request = urllib2.Request('http://192.168.10.194:28080/screen_detector_log?',headers=headers)
res = urllib2.urlopen(request,uni_log_str)
print(resJ)
data = res.read()
print(data)
