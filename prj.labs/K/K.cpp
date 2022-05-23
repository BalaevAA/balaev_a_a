#include <opencv2/opencv.hpp>
#include <vector>


int main() {
	cv::Mat res(450, 300, CV_8UC1);
	cv::Mat rect_black(150, 150, CV_8UC1, cv::Scalar(0, 0, 0));
	cv::Mat rect_white(150, 150, CV_8UC1, cv::Scalar(255, 255, 255));
	cv::Mat rect_gray(150, 150, CV_8UC1, cv::Scalar(127, 127, 127));
	std::vector<cv::Mat> vec = { rect_black, rect_white, rect_gray };

	cv::Mat_<float> kernel1(3, 3);
	kernel1 << 1, -2, 1, 0, 0, 0, 1, -2, 1;
	cv::Mat_<float> kernel2(3, 3);
	kernel2 << 1, 0, 1, -2, 0, 2,	1, 0, 1;

	std::vector<cv::Scalar> color = { cv::Scalar(255,255,255),
																		cv::Scalar(0, 0, 0),
																		cv::Scalar(0, 0, 0),
																		cv::Scalar(127, 127, 127),
																		cv::Scalar(127, 127, 127),
																		cv::Scalar(255,255,255)};

	int k = -1, t = 0;
	for (int i = 0; i < 450; i += 150) {
		for (int j = 0; j < 300; j += 150) {
			k = (k + 1) % 3;
			vec[k].copyTo(res(cv::Rect(j, i, 150, 150)));
			cv::circle(res, cv::Point(75 + j, 75 + i), 60, color[t], -1);
			t++;
		}
	}

	cv::Mat filter1;
	cv::filter2D(res, filter1, -1, kernel1, cv::Point(-1, -1));
	cv::Mat filter2;
	cv::filter2D(res, filter2, -1, kernel2, cv::Point(-1, -1));
	
	cv::Mat filter3(450, 300, CV_8UC1);
	for (int i = 0; i < res.rows; i++) {
		for (int j = 0; j < res.cols; j++) {
			filter3.at<uchar>(i, j) = std::sqrt(filter1.at<uchar>(i, j) * filter2.at<uchar>(i, j));
		}
	}

	cv::imwrite("img.png", res);
	cv::imwrite("filter_1.png", filter1);
	cv::imwrite("filter_2.png", filter2);
	cv::imwrite("filter_3.png", filter3);
	cv::waitKey(0);
	return 0;
}