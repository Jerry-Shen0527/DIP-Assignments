#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
	using namespace cv;

	if (argc != 4)
	{
		std::cout << "usage: img_src_name img_ref_name img_save_name"<< std::endl;
	}

	auto src = argv[1];

	src = "1.jpg";
	auto ref = argv[2];
	auto save = argv[3];

	auto img = imread(src);
	std::cout<<img.at<cv::Vec4b>(0,0);
	//std::cout << img.size[0];
}