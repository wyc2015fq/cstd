
#ifndef __OCR_IDCARD_HPP__
#define __OCR_IDCARD_HPP__


#include "std/time_c.h"
#include "utime.h"
#include "parser/cjson.hpp"
#include "idcard.h"
#include "std/iconv_c.h"
#include "std/dir_c.h"
#include "wstd/string.hpp"
using namespace wstd;

#include "ocr_detect_back.hpp"
#include "ocr_detect_front.hpp"
#include "ocr_caffe.hpp"


struct OcrIdCard {
	IdCardFrontDetecter front_detecter;
	IdCardBackDetecter back_detecter;

	std::vector<vector<RotatedRect> > front_rect;
	std::vector<vector<RotatedRect> > back_rect;

	vector<idcard> vid;
	vector<idcard_back> vidb;
	OcrIdCard() { init(); }
	int init() {
		back_detecter.init(MODELPATH"idcard_back_cas_20190326.txt");
		front_detecter.init(MODELPATH"idcard_front_cas_20190330.txt");
		//ocr_caffe_init();
		return 0;
	}
	int run(const Mat& mat) {
		//buf_t bf[1] = { 0 };
		front_rect.clear();
		back_rect.clear();
		vid.clear();
		vidb.clear();
		Mat src = mastbegray(mat);
		Mat gray;
		//vector<Mat> vv;
		for (int i = 0; i < 4; ++i) {
			rotate90(src, gray, i);
			front_detecter.run(gray, front_rect, i);
			back_detecter.run(gray, back_rect, i);
		}

		{
			const std::vector<vector<RotatedRect> >& vvrr = front_rect;
			for (int i = 0; i < vvrr.size(); ++i) {
				const vector<RotatedRect>& vrr = vvrr[i];
				if (vrr.size() > 5) {
					idcard id = { 0 };
					idcard* out = &id;
					strcpy(out->side, "front");
					if (1) {
						RotatedRect r4 = vrr.back();
						r4 = scale_size(r4, 1.1, 1.8);
						Mat im4 = getSubImage(src, r4);
						//imshow("im4", im4); waitKey(0);
						string ss = run_ocrnum_caffe(im4);
						char idnumber[32] = { 0 };
						strcpy(idnumber, ss.c_str());
						if (!idcard_get_info(idnumber, out)) {
							printf("idcard_get_info fail! number:%s\n", idnumber);
						}
						strncpy(out->number, idnumber, 32);
					}
					if (1) {
						RotatedRect r4 = vrr[1];
						r4 = scale_size(r4, 1.2, 1.6);
						Mat im4 = getSubImage(src, r4);
						string ss = run_ocr_caffe(im4);
						//imshow("im4", im4); waitKey(0);
						strncpy(out->name, ss.c_str(), 16);
					}
					if (1) {
						RotatedRect r4 = vrr[2];
						r4 = scale_size(r4, 1.1, 1.6);
						Mat im4 = getSubImage(src, r4);
						string ss = run_ocr_caffe(im4);
						if (ss.length() == 0) {
							ss = "บบ";
						}
						strncpy(out->race, ss.c_str(), 32);
					}
					if (1) {
						string ss;
						for (int j = 4; j < vrr.size() - 1; ++j) {
							RotatedRect r4 = vrr[j];
							r4 = scale_size(r4, 1.1, 1.6);
							Mat im4 = getSubImage(src, r4);
							ss += run_ocr_caffe(im4);
						}
						strcat(out->address, ss.c_str());
					}
					vid.push_back(id);
				}
			}
		}
		{
			const std::vector<vector<RotatedRect> >& vvrr = back_rect;
			for (int i = 0; i < vvrr.size(); ++i) {
				const vector<RotatedRect>& vrr = vvrr[i];
				if (vrr.size() == 3) {
					RotatedRect r0 = vrr[1];
					RotatedRect r1 = vrr[2];
					r0 = scale_size(r0, 1.1, 1.6);
					r1 = scale_size(r1, 1.1, 1.6);
					Mat im0 = getSubImage(src, r0);
					Mat im1 = getSubImage(src, r1);
					string ss0 = run_ocr_caffe(im0);
					//string ss1 = run_ocrnum_caffe(im1);
					string ss1 = run_ocr_caffe(im1);
					idcard_back idb = { 0 };
					strcpy(idb.side, "back");
					strcpy(idb.issued_by, ss0.c_str());
					strcpy(idb.valid_date, ss1.c_str());
					//imshow("im0", im0); imshow("im1", im1); waitKey(0);
					int ret = valid_date_curr(idb.valid_date);
					printf("issued_by: %s  valid_date:%s\n", idb.issued_by, idb.valid_date);
					if (ret) {
						vidb.push_back(idb);
						if (0) {
							imshow("im0", im0);
							imshow("im1", im1);
							waitKey(0);
						}
					}
				}
			}
		}

		int ret = vid.size() + vidb.size();
		return ret;
	}


	string jsonstr;
	std::string getjson(const cv::Mat& src) {
		const char* nowstr = timenow();
		int ret = 0;
		jsonstr = "";
		utime_start(a);
		ret = run(src);
		int time_used = 1000 * utime_elapsed(a);
		switch (ret) {
		case 1:
		{
			cjson * root = cjson_CreateObject();
			cjson * cards = cjson_CreateArray();
			cjson_AddItemToObject(root, "cards", cards);
			for (int i = 0; i < vid.size(); ++i) {
				cjson * card = cjson_CreateObject();
				idcard id = vid[i];
				cjson_AddItemToObject(card, "name", cjson_CreateString(id.name));
				cjson_AddItemToObject(card, "gender", cjson_CreateString(id.gender));
				cjson_AddItemToObject(card, "id_card_number", cjson_CreateString(id.number));
				cjson_AddItemToObject(card, "birthday", cjson_CreateString(id.birthday));
				cjson_AddItemToObject(card, "race", cjson_CreateString(id.race));
				cjson_AddItemToObject(card, "address", cjson_CreateString(id.address));
				cjson_AddItemToObject(card, "type", cjson_CreateNumber(1));
				cjson_AddItemToObject(card, "side", cjson_CreateString(id.side));
				cjson_AddItemToArray(cards, card);
			}
			for (int i = 0; i < vidb.size(); ++i) {
				cjson * card = cjson_CreateObject();
				idcard_back idb = vidb[i];
				cjson_AddItemToObject(card, "issued_by", cjson_CreateString(idb.issued_by));
				cjson_AddItemToObject(card, "valid_date", cjson_CreateString(idb.valid_date));
				cjson_AddItemToObject(card, "type", cjson_CreateNumber(1));
				cjson_AddItemToObject(card, "side", cjson_CreateString(idb.side));
				cjson_AddItemToArray(cards, card);
			}
			cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
			cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
			jsonstr = cjson_Print(root);
			printf("%s\n", jsonstr.c_str());
			cjson_Delete(root);
		}
		break;
		case -1:
		{
			cjson * root = cjson_CreateObject();
			cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
			cjson_AddItemToObject(root, "error_message", cjson_CreateString("IMAGE_TOO_VAGUE"));
			cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
			jsonstr = cjson_Print(root);
			printf("%s\n", jsonstr.c_str());
			cjson_Delete(root);
		}
		break;
		default:
		{
			cjson * root = cjson_CreateObject();
			cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
			cjson_AddItemToObject(root, "error_message", cjson_CreateString("MISSING_ARGUMENTS: image_url, image_file, image_base64"));
			cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
			jsonstr = cjson_Print(root);
			printf("%s\n", jsonstr.c_str());
			cjson_Delete(root);
		}
		break;
		}
		return jsonstr;
	}

};

#endif // __OCR_IDCARD_HPP__
