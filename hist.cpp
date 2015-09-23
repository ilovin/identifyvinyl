// hist.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"

using namespace std;
using namespace cv;

void getCanny(Mat gray, Mat &canny) ;
void hitmiss(cv::Mat& src, cv::Mat& dst);
int histgram(Mat &src);

int _tmain(int argc, _TCHAR* argv[])
{
	string str;
	str = argv[1];
	Mat src = imread(str, 0);
	resize(src, src, Size(cvRound(src.cols*0.3), cvRound(src.rows*0.3)));
	if (!src.data)
	{
		return -1;
	}

	imshow("src", src);
	//inverse the color
	for (int i = 0; i< src.rows; i++)
	{
		for (int j = 0; j< src.cols; j++)
		{
			src.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);   // 每一个像素反转
		}
	}


	//open or tophat
	Mat elementv = getStructuringElement(MORPH_ELLIPSE, Size(12,12),Point(-1,-1));
	//Mat elementh = getStructuringElement(MORPH_RECT, Size(36, 10), Point(18, 5));
	//Mat elementh = getStructuringElement(MORPH_ELLIPSE, Size(10, 10), Point(5, 5));
	//Mat elementh =
	Mat dstv,dsth;
	//morphologyEx(src, dst, MORPH_OPEN, element);
	morphologyEx(src, dstv, MORPH_CLOSE, elementv );
	//morphologyEx(dstv, dstv, MORPH_OPEN, elementv );

	imshow("dstv", dstv);

	//morphologyEx(src, dsth, MORPH_CLOSE, elementh);
	//morphologyEx(dsth, dsth, MORPH_OPEN, elementh);
	//imshow("dsth", dsth);
	int tmp = histgram(dstv);
	//设定阈值

	Mat her, vec;
	threshold(dstv, dstv, tmp, 255, THRESH_TOZERO);
	morphologyEx(dstv, dstv, MORPH_CLOSE, elementv,Point(-1,-1),2);
	imshow("thresh", dstv);
	//threshold(dsth, dsth, 190, 255, THRESH_TOZERO);
	//dilate(dst, dst, element, Point(0, 0));

	//击中击不中

	//morphologyEx(dstv, vec, MORPH_OPEN, elementv);
	//imshow("vec", vec);
	//Mat htm;
	//hitmiss(vec, htm);
	//imshow("htm", htm);
	//Rect ccomp;
	//int tmp = 0;
	//for (int i = 0; i < vec.rows; i++)
	//{
	//	for (int j = 0; j < vec.cols; j++)
	//	{
	//		if (htm.at<uchar>(i,j)> 0 )
	//		{
	//			tmp++;
	//			if (tmp>3)
	//			{
	//				floodFill(vec, Point(i, j), Scalar(0));
	//				/*			floodFill(dst, seed, newVal, &ccomp, Scalar(lo, lo, lo),
	//				Scalar(up, up, up), flags);*/
	//				floodFill(vec, Point(i, j), Scalar(0), &ccomp, Scalar(7, 7, 7), Scalar(20, 20, 20), 4);
	//				break;
	//			}
	//		}
	//	}
	//}
	//morphologyEx(vec, vec, MORPH_ERODE, elementh);
	//imshow("repair_vec", vec);

	//morphologyEx(dsth, her, MORPH_OPEN, elementh);
	//imshow("her", her);
	//imshow("dst", dst);
	//Mat dst;
	//dst = Mat::zeros(src.rows, src.cols, CV_8U);
	//for (int i = 0; i < src.rows; i++)
	//{
	//	for (int j = 0; j < src.cols; j++)
	//	{
	//		if (vec.at<uchar>(i, j)>0 || her.at<uchar>(i, j)>0)
	//		{
	//			dst.at<uchar>(i, j) = 255;
	//		}
	//	}
	//}
	//imshow("dst", dst);
	//Mat canny;
	//getCanny(dstv, canny);
	//imshow("canny", canny);
	//Mat result;
	//calcBackProject(&imgsrc, 1, 0, r_hist, result, &histRange, 1,true);
	//imshow("result", result);
	////calcBackProject(&ImgSrc, 1, channels, hist, result, ranges, 255);

	//for (int i = 0; i < histSize; i++)
	//{
	//	cout << i << " : " << r_hist.at<float>(i) << endl;
	//}
	waitKey(0);


	return 0;
}

int histgram(Mat &src)
{
	//histgram
	vector<Mat> rgb_planes;

	const int channels[3] = { 0, 1, 2 };
	int histSize = 255;
		/// 设定取值范围 ( R,G,B) )
	float range[] = { 1, 255 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;
//最后创建储存直方图的矩阵:

	Mat r_hist, g_hist, b_hist;
	//下面使用OpenCV函数 calcHist 计算直方图 :

	/// 计算直方图:
	calcHist(&src, 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	//normalize(r_hist, r_hist, 0, 255, NORM_MINMAX, -1, Mat());

	//cout << r_hist;
	//显示直方图
	int hist_w = 400; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//draw the gram
	circle(histImage, Point(0, histImage.rows / 2), 5, Scalar(255, 0, 0), -1);
	circle(histImage, Point(0, histImage.rows / 4), 3, Scalar(255, 0, 0), -1);
	circle(histImage, Point(0, histImage.rows * 3 / 4), 3, Scalar(255, 0, 0), -1);

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
/*
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);

		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	*/
	}

	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
	float k[5] = { 1, 1, 2, 1, 1 };
	Mat km = Mat(5, 1, CV_32FC1, k);
	Mat dst;
	filter2D(r_hist, dst, -1, km);
	//cout << dst;
	for (int i = 128; i < 255; i++)
	{
		cout << i<<":" << dst.at<float>(i) << endl;
	}
	int max = 0, tmp;	
	for (int i = 0; i < 128; i++)
	{
		if (dst.at<float>(255-i-1)>100)
		{
			tmp = i - 1;
			break;
		}
	}
	//find the max
	int count = 0;
	int min = 2000;
	for (int i = tmp; i < 128; i++)
	{
		if (dst.at<float>(255-i)>max)
		{
			max = int(dst.at<float>(255 - i));
			count = 0;
		}
		else
		{
			count++;
			if (count>10)
			{
				tmp = i-10;
				min = int(dst.at<float>(255 - i+10));
				break;
			}
		}
	}
	cout << max << endl;
	count = 0;
	for (int i = tmp; i < 128; i++)
	{
		if (dst.at<float>(255 - i) <= min)
		{
			min = int(dst.at<float>(255 - i));
			count = 0;
		}
		else
		{
			count++;
			if (count>4)
			{
				cout << 255 - i;
				return 255 - i;
				break;
			}
		}
	}
/*	int min, min_thresh = 255;
	min = histImage.rows;
	int count = 0;
	cout << r_hist << endl;
	for (int i = 0; i < 128; i++)
	{
		if (cvRound(r_hist.at<float>(255 - i - 1)) < min)
		{
			if (cvRound(r_hist.at<float>(255 - i - 1) != 0))
			{
				min = cvRound(r_hist.at<float>(255 - i - 1));
				min_thresh = 255 - i - 1;
				count = 0;
			}
		}
		else
		{
			count++;
			if (count>19)
			{
				break;
			}
		}
	}
	cout << "the thresh is:" << min_thresh << endl*/;
	return 0;

}

/**
* Hit-or-miss transform function
*
* Parameters:
*   src     The source image, 8 bit single-channel matrix
*   dst     The destination image
*   kernel  The kernel matrix. 1=foreground, -1=background, 0=don't care
*/
void hitmiss(cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_8U && src.channels() == 1);

	Mat k1, k2;
	k1 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2, 2));
	k2 = getStructuringElement(MORPH_ELLIPSE, Size(25, 25), Point(7, 7));
	//cv::normalize(src, src, 0, 1, cv::NORM_MINMAX);
	//imshow("test", src);

	cv::Mat e1, e2;
	cv::erode(src, e1, k1, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, cv::Scalar(0));
	//erode(src, e1, k1);
	//imshow("e1", e1);
	cv::erode(255 - src, e2, k2, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, cv::Scalar(0));
	//imshow("e2", e2);
	dst = e1 & e2;
}

void getCanny(Mat gray, Mat &canny) {
	Mat thres;
	double high_thres = threshold(gray, thres, 0, 255, THRESH_BINARY | THRESH_OTSU), low_thres = high_thres * 0.5;
	Canny(gray, canny, low_thres, high_thres);
}

