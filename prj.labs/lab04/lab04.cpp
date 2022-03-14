#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>





std::vector<cv::Mat> selectFramesFromVideo(std::string path, std::string name) {
	cv::VideoCapture cp(path);
	std::vector<cv::Mat> res;
	cv::Mat tmp;
	for (int i = 0; i < cv::CAP_PROP_FRAME_COUNT; ++i) {
		cp >> tmp;
		res.push_back(tmp);
	}
	cv::Mat frame1 = res[res.size() * 2 / 5];
	cv::Mat frame2 = res[res.size() * 3 / 5];
	cv::Mat frame3 = res[res.size() * 4 / 5];
	for (auto i :{ frame1, frame2, frame3 }) {
		res.push_back(i);
	}
	cv::imwrite(name + "_img1.png", frame1);
	cv::imwrite(name + "_img2.png", frame2);
	cv::imwrite(name + "_img3.png", frame3);
}


std::vector<cv::Mat> colorConversation(std::vector<cv::Mat> f) {
	std::vector<cv::Mat> vec;
	for (int i = 0; i < f.size(); ++i) {
		cv::Mat tmp;
		cv::cvtColor(f[i], tmp, cv::COLOR_BGR2GRAY);
		cv::imwrite("grey" + str(i)+(".png"), tmp);
	}
}

void tresholdForEveryone(std::vector<cv::Mat> f) {
	int threshold_value = 46;
	int threshold_type = 0;
	int const max_value = 255;
	int const max_binary_value = 255;


}


/*
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";
static void Threshold_Demo(int, void*)
{
	/* 0: Binary
	 1: Binary Inverted
	 2: Threshold Truncated
	 3: Threshold to Zero
	 4: Threshold to Zero Inverted
	
	threshold(frame_gray, dst, threshold_value, max_binary_value, threshold_type);
	imshow(window_name, dst);
}
*/
int main() {
	
	cv::Mat frame, frame_gray, dst, dst_new;

	//Select the necessary frames from the video
	selectFramesFromVideo("../data/rub100.mp4", "rub100");
	selectFramesFromVideo("../data/rub100(2).mp4", "rub100_2");
	selectFramesFromVideo("../data/rub100(3).mp4", "rub100_3");
	selectFramesFromVideo("../data/rub500.mp4", "rub500");
	selectFramesFromVideo("../data/rub500(2).mp4", "rub500_2");
/*
	cv::imshow("first frame", frame);

	//convert to GrayScale
	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	cv::imshow("grey", frame_gray);
	// threshold
	
	//cv::threshold(frame_gray, dst, 46,max_binary_value, 0);
	cv::threshold(frame_gray, dst, 50,max_binary_value, 0);
	cv::imshow("", dst);
	

	cv::morphologyEx(dst, dst_new, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT,cv::Size(18, 16)));
	cv::imshow("", dst_new);

	/*cv::createTrackbar(
		trackbar_type,
		window_name, 
		&threshold_type,
		max_type, 
		Threshold_Demo
	); // Create a Trackbar to choose type of Threshold
	
	cv::createTrackbar(
		trackbar_value,
		window_name, 
		&threshold_value,
		max_value, 
		Threshold_Demo
	); // Create a Trackbar to choose Threshold value*/
	


	cv::waitKey(0);
	return 0;
}