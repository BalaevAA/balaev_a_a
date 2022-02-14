#include <opencv2/opencv.hpp>

int main() {

	const double Gamma1 = 2.2;
	const double Gamma2 = 2.4;
	
	cv::Mat img(180, 768, CV_8UC1);
	// draw dummy image
	img = 0;
	
	cv::Rect2d rc = { 0, 0, 768, 60 };

	for (ptrdiff_t y = 0; y < 180; ++y) {
		for (ptrdiff_t x = 0; x < 768; ++x) {
				img.at<uchar>(y, x) = x / 3;
		}
	}
	
	cv::rectangle(img, rc, { 150 }, 1);
	rc.y += rc.height;

	cv::Mat new_image;
	img.copyTo(new_image);
	

	new_image.convertTo(new_image, CV_64FC1, 1.0f / 255.0f);
	cv::pow(new_image, Gamma1, new_image);
	new_image.convertTo(new_image, CV_8UC1, 255.0f);
	new_image(rc).copyTo(img(rc));

	
	cv::rectangle(img, rc, { 150 }, 1);
	rc.y += rc.height;
	

	for (ptrdiff_t y = 120; y < 180; ++y) {
		for (ptrdiff_t x = 0; x < 768; ++x) {
			img.at<uchar>(y,x) = cv::saturate_cast<uchar>(cv::pow((img.at<uchar>(y,x) / 255.0f), Gamma2) * 255.0f);
		}
	}

	cv::rectangle(img, rc, { 150 }, 1);
	// save result
	cv::imwrite("lab01.png", img);
	
	cv::imshow("Output", img);

	cv::waitKey(0);
	return 0;
}