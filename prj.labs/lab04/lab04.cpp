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
	return res;
}


std::vector<cv::Mat> colorConversation(std::vector<cv::Mat> f, std::string name) {
	std::vector<cv::Mat> vec;
	for (int i = 0; i < f.size(); ++i) {
		cv::Mat tmp;
		cv::cvtColor(f[i], tmp, cv::COLOR_BGR2GRAY);
		cv::imwrite("grey_" + name + "_" + std::to_string(i) + ".png", tmp);
		vec.push_back(tmp);
	}
	return vec;
}

std::vector<cv::Mat> tresholdForEveryone(std::vector<cv::Mat> f, std::string name) {
	int threshold_value = 46;
	int threshold_type = 0;
	int const max_value = 255;
	int const max_binary_value = 255;
	std::vector<cv::Mat> vec;
	for (int i = 0; i < f.size(); ++i) {
		cv::Mat tmp;
		cv::threshold(f[i], tmp, 50, max_binary_value, 0);
		vec.push_back(tmp);
		cv::imwrite("treshold_" + name + "_" + std::to_string(i) + ".png", tmp);
	}
	return vec;
}


std::vector<cv::Mat> morph(std::vector<cv::Mat> f, std::string name) {
	std::vector<cv::Mat> vec;


	for (int i = 0; i < f.size(); ++i) {
		cv::Mat tmp;
		cv::morphologyEx(f[i], tmp, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));
		vec.push_back(tmp);
		cv::imwrite("morph_" + name + "_" + std::to_string(i) + ".png", tmp);
	}

	return vec;
}

int main() {
	
	std::vector<std::vector<cv::Mat> > frames(5), gray_frames(5), bin_frames(5), morph_frames(5);

///////////////////////////////////////////  1  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	frames[0] = selectFramesFromVideo("../data/rub100(2).mp4", "rub100_2");
	std::cout << "/////////////////////////////////////////////////////" << frames.size() << "\n\n\n\n\n\n\n\n";
	gray_frames[0] = colorConversation(frames[0], "rub100_2");
	bin_frames[0] = tresholdForEveryone(gray_frames[0], "rub100_2");
	morph_frames[0] = morph(bin_frames[0], "rub100_2");

///////////////////////////////////////////  2  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	frames[1] = selectFramesFromVideo("../data/rub100.mp4", "rub100");
	gray_frames[1] = colorConversation(frames[1], "rub100");
	bin_frames[1] = tresholdForEveryone(gray_frames[1], "rub100");
	morph_frames[1] = morph(bin_frames[1], "rub100");

///////////////////////////////////////////  3  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	frames[2] = selectFramesFromVideo("../data/rub100(3).mp4", "rub100_3");
	gray_frames[2] = colorConversation(frames[2], "rub100_3");
	bin_frames[2] = tresholdForEveryone(gray_frames[2], "rub100_3");
	morph_frames[2] = morph(bin_frames[2], "rub100_3");

///////////////////////////////////////////  4  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	frames[3] = selectFramesFromVideo("../data/rub500.mp4", "rub500");
	gray_frames[3] = colorConversation(frames[3], "rub500");
	bin_frames[3] = tresholdForEveryone(gray_frames[3], "rub500");
	morph_frames[3] = morph(bin_frames[3], "rub500");

///////////////////////////////////////////  5  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	frames[4] = selectFramesFromVideo("../data/rub500(2).mp4", "rub500_2");
	gray_frames[4] = colorConversation(frames[4], "rub500_2");
	bin_frames[4] = tresholdForEveryone(gray_frames[4], "rub500_2");
	morph_frames[4] = morph(bin_frames[4], "rub500_2");

	cv::waitKey(0);
	return 0;
}





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