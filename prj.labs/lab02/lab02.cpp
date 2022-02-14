#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>

int main() {
	//1
	std::string img_path = "C:/Users/balae/Documents/balaev_a_a/data/cross_0256x0256.png";
	cv::Mat img = cv::imread(img_path);
	if (img.empty()) {
		std::cout << "Could not read the image: " << img_path << std::endl;
		return 1;
	}
	//2
	cv::imwrite("cross_0256x0256_025.jpeg", img, { cv::IMWRITE_JPEG_QUALITY , 25 });
	//3








	cv::waitKey(0);

	return 0;
}