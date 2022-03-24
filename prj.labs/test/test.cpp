#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>



void connected_component_stats_demo(cv::Mat& image);

int main() {
	cv::Mat src = cv::imread("C:/Users/balae/Documents/balaev_a_a/bin.dbg/lab04_rub200_2_img3.png");
	if (src.empty()) {
		std::cout << "couldn't load image";
	}
	cv::imshow("imshow", src);
	connected_component_stats_demo(src);
	cv::waitKey(0);
	return 0;
}

void connected_component_stats_demo(cv::Mat& image) {
	cv::Mat gray, binary;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::morphologyEx(binary, binary, cv::MORPH_OPEN, k);
	cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, k);
	cv::imshow("binary", binary);

	cv::Mat labels = cv::Mat::zeros(image.size(), CV_32S);
	cv::Mat stats, centroids;
	int num_labels = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8,4);
	
	std::cout << "\n\n\n\n\n\n" << num_labels - 1 << "\n\n\n\n\n\n";
	
	std::vector<cv::Vec3b> colors(num_labels);

	colors[0] = cv::Vec3b(0, 0, 0);


	for (int i = 1; i < num_labels; ++i) {
		colors[i] = cv::Vec3b(0, 256, 0);
	}

	cv::Mat dst = cv::Mat::zeros(image.size(), image.type());
	int w = image.cols;
	int h = image.rows;

	for (int row = 0; row < h; ++row) {
		for (int col = 0; col < w; ++col) {
			int label = labels.at<int>(row, col);
			if (label == 0) continue;
			dst.at<cv::Vec3b>(row, col) = colors[label];
		}
	}

	for (int i = 1; i < num_labels; ++i) {
		cv::Vec2d pt = centroids.at<cv::Vec2d>(i, 0);
		int x = stats.at<int>(i, cv::CC_STAT_LEFT);
		int y = stats.at<int>(i, cv::CC_STAT_TOP);
		int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
		int heigth = stats.at<int>(i, cv::CC_STAT_HEIGHT);
		int area = stats.at<int>(i, cv::CC_STAT_AREA);
		std::cout << "area: " << area << "center point " << pt[0] << " " << pt[1];
		cv::circle(dst, cv::Point(pt[0], pt[1]), 2, cv::Scalar(0, 0, 255), -1, 8, 0);
		cv::rectangle(dst, cv::Rect(x, y, width, heigth), cv::Scalar(255, 0, 255), 1, 8, 0);
	}
	cv::imshow("ccla-demo", dst);
}