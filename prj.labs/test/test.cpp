#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>

void doMagic(std::string path, std::string name, std::vector<std::vector<std::pair<int, int> > > coord);
void makeMosaic(std::vector<cv::Mat> src, std::string name, int type);
double sq(const std::vector<std::pair<int, int> >& fig);


void doMagic(std::string path, std::string name, std::vector<std::vector<std::pair<int, int> > > coord) {
	cv::VideoCapture cp(path);
	std::vector<cv::Mat> original_images(3);

	int amount_of_frames = cp.get(cv::CAP_PROP_FRAME_COUNT);
	int f1 = (amount_of_frames / 5) * 2,
		f2 = (amount_of_frames / 5) * 3,
		f3 = (amount_of_frames / 5) * 4;
	cv::Mat tmp1, tmp2, tmp3;
	cp.set(cv::CAP_PROP_POS_FRAMES, f1);
	cp >> original_images[0];
	cv::resize(original_images[0], original_images[0], cv::Size(640, 360), cv::INTER_LINEAR);
	cp.set(cv::CAP_PROP_POS_FRAMES, f2);
	cp >> original_images[1];
	cv::resize(original_images[1], original_images[1], cv::Size(640, 360), cv::INTER_LINEAR);
	cp.set(cv::CAP_PROP_POS_FRAMES, f3);
	cp >> original_images[2];
	cv::resize(original_images[2], original_images[2], cv::Size(640, 360), cv::INTER_LINEAR);

	cv::imwrite(name + "_img1.png", original_images[0]);
	cv::imwrite(name + "_img2.png", original_images[1]);
	cv::imwrite(name + "_img3.png", original_images[2]);
	makeMosaic(original_images, name, CV_8UC3);

	std::vector<cv::Mat> gray_images;
	for (int i = 0; i < original_images.size(); ++i) {
		cv::Mat tmp(640, 360, CV_8UC3);
		cv::cvtColor(original_images[i], tmp, cv::COLOR_BGR2GRAY);
		cv::imwrite(name + "binary_" + std::to_string(i) + ".png", tmp);
		gray_images.push_back(tmp);
	}
	makeMosaic(gray_images, "gray_" + name, CV_32S);

	int const max_binary_value = 255;
	std::vector<cv::Mat> bin_images;
	for (int i = 0; i < gray_images.size(); ++i) {
		cv::Mat tmp(640, 360, CV_8UC3);
		cv::threshold(gray_images[i], tmp, 0, max_binary_value, cv::THRESH_BINARY | cv::THRESH_OTSU);
		cv::imwrite(name + "treshold_" + std::to_string(i) + ".png", tmp);
		bin_images.push_back(tmp);
	}
	makeMosaic(bin_images, "bin_" + name, CV_32S);

	std::vector<cv::Mat> morph_images;
	for (int i = 0; i < bin_images.size(); ++i) {
		cv::Mat tmp(640, 360, CV_8UC3);
		cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
		cv::morphologyEx(bin_images[i], tmp, cv::MORPH_OPEN, k);
		//cv::morphologyEx(f[i], tmp, cv::MORPH_CLOSE, k);
		morph_images.push_back(tmp);
		cv::imwrite(name + "_morph_" + std::to_string(i) + ".png", tmp);
	}
	makeMosaic(morph_images, "morph_" + name, CV_32S);

	std::vector<cv::Mat> conn_comp;
	for (int i = 0; i < morph_images.size(); ++i) {
		int max_area = -1, x_max = -1, y_max = -1, w_max = -1, h_max = -1;
		cv::Mat labels;
		cv::Mat stats, centroids;
		int num_labels = cv::connectedComponentsWithStats(morph_images[i], labels, stats, centroids, 8, 4);

		std::vector<cv::Vec3b> colors(num_labels);

		for (int i = 0; i < num_labels; ++i) {
			colors[i] = cv::Vec3b(255, 255, 255);
		}

		cv::Mat dst = cv::Mat::zeros(morph_images[i].size(), CV_8UC3);
		int w = morph_images[i].cols;
		int h = morph_images[i].rows;

		for (int row = 0; row < h; ++row) {
			for (int col = 0; col < w; ++col) {
				int label = labels.at<int>(row, col);
				if (label == 0) continue;
				dst.at<cv::Vec3b>(row, col) = colors[label];
			}
		}

		for (int j = 1; j < num_labels; ++j) {
			cv::Vec2d pt = centroids.at<cv::Vec2d>(j, 0);
			int x = stats.at<int>(j, cv::CC_STAT_LEFT);
			int y = stats.at<int>(j, cv::CC_STAT_TOP);
			int width = stats.at<int>(j, cv::CC_STAT_WIDTH);
			int heigth = stats.at<int>(j, cv::CC_STAT_HEIGHT);
			int area = stats.at<int>(j, cv::CC_STAT_AREA);
			if (area >= max_area) {
				max_area = area;
				x_max = x;
				y_max = y;
				w_max = width;
				h_max = heigth;
			}
		}
		std::cout << "(x,y) = (" << x_max << ", " << y_max << ") and (width, height) = (" << w_max << ", " << h_max << ")\n";
		std::cout << "area: " << max_area << "\n";
		std::cout << "accuracy(square_orig / square_conn_comp): " << sq(coord[i]) / max_area << "\n\n\n";
		cv::rectangle(dst, cv::Rect(x_max, y_max, w_max, h_max), cv::Scalar(255, 0, 255), 1, 8, 0);
		std::cout << "\n";
		conn_comp.push_back(dst);
		cv::imwrite(name + "con" + std::to_string(i) + ".png", dst);
	}
	makeMosaic(conn_comp, "comp_" + name, CV_8UC3);
}


void makeMosaic(std::vector<cv::Mat> src, std::string name, int type) {
	cv::Mat res_img(720, 1280, type);
	src[0].copyTo(res_img(cv::Rect(0, 0, 640, 360)));
	src[1].copyTo(res_img(cv::Rect(640, 360, 640, 360)));
	src[2].copyTo(res_img(cv::Rect(0, 360, 640, 360)));

	cv::imwrite("lab04_mosaic_" + name + ".png", res_img);

	//return res_img;
}

double sq(const std::vector<std::pair<int, int> >& fig) {
	double res = 0;
	for (unsigned i = 0; i < fig.size(); i++) {
		std::pair<int, int> p1 = i ? fig[i - 1] : fig.back(),
			p2 = fig[i];
		res += (p1.first - p2.first) * (p1.second + p2.second);
	}
	return fabs(res) / 2;
}

int main() {
	
	std::vector<std::vector<std::pair<int, int> > > p1 = { { {135, 71}, {588, 88}, {586, 283}, {131, 277} },
																												 { {153, 56}, {589, 74}, {587, 260}, {147, 256} },
																												 { {99, 58}, {553, 71}, {554, 264}, {96, 265}   } };
	/*
	std::vector<std::vector<std::pair<int, int> > > p2 = { { },
																												 { },
																												 { } };

	std::vector<std::vector<std::pair<int, int> > > p3 = { { },
																												 { },
																												 { } };

	std::vector<std::vector<std::pair<int, int> > > p4 = { { },
																											   { },
																											   { } };

	std::vector<std::vector<std::pair<int, int> > > p5 = { { },
																												{ },
																												 { } };
*/



	doMagic("../data/rub100(2).mp4", "lab04_rub100_2", p1);

	//doMagic("../data/rub200(3).mp4", "lab04_rub200_3", p2);

	//doMagic("../data/rub200(4).mp4", "lab04_rub200_4", p3);

	//doMagic("../data/rub500.mp4", "lab04_rub500", p4);

	//doMagic("../data/rub500(2).mp4", "lab04_rub500_2", p5);

	cv::waitKey(0);
	return 0;
}
