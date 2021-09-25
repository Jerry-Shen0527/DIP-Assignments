#include <opencv2/opencv.hpp>

class Converter
{
public:
	virtual ~Converter() = default;

	Converter(const cv::Mat& mat) { src = mat.clone(); }

	virtual cv::Mat convert() = 0;

protected:
	cv::Mat src;
};

class RefConverter : public Converter
{
public:
	RefConverter(const cv::Mat& mat_src, const cv::Mat& mat_ref) :Converter(mat_src) { ref = mat_ref.clone(); }
protected:
	cv::Mat ref;
};

cv::Vec3d mul(cv::Vec3d vec_a, cv::Vec3d vec_b)
{
	return cv::Vec3d(vec_a[0] * vec_b[0], vec_a[1] * vec_b[1], vec_a[2] * vec_b[2]);
}

class ColorRefConverter : public RefConverter
{
public:
	using  RefConverter::RefConverter;
	cv::Mat convert() override;

	static cv::Vec3d average(const cv::Mat& mat)
	{
		using namespace cv;

		cv::Vec3d rst = 0;
		int count = 0;
		for (auto pixel : cv::Mat_<cv::Vec3b>(mat))
		{
			rst += pixel;
			count++;
		}
		rst /= float(count);
		return rst;
	}

	static cv::Vec3d variance(const cv::Mat& mat, cv::Vec3b& average)
	{
		cv::Vec3d rst = 0, ave = average;
		int count = 0;
		for (auto pixel : cv::Mat_<cv::Vec3b>(mat))
		{
			rst += mul(cv::Vec3d(pixel - average), cv::Vec3d(pixel - average));
			count++;
		}
		rst /= float(count);
		return rst;
	}
};

cv::Mat ColorRefConverter::convert()
{
	using namespace cv;
	Mat src_lab;
	cv::cvtColor(src, src_lab, COLOR_BGR2Lab);
	Mat ref_lab;
	cv::cvtColor(ref, ref_lab, COLOR_BGR2Lab);

	auto src_ave = average(src_lab);
	auto src_var = variance(src_lab, Vec3b(src_ave));

	auto ref_ave = average(ref_lab);
	auto ref_var = variance(ref_lab, Vec3b(ref_ave));

	auto diff = ref_ave - src_ave;
	Vec3d scale(ref_var[0] / src_var[0], ref_var[1] / src_var[1], ref_var[2] / src_var[2]);

	Mat_<Vec3b> rst_lab = src_lab.clone();

	for (auto& pixel : rst_lab)
	{
		Vec3d t_pixel = pixel;

		t_pixel = ref_ave + mul(Vec3d(t_pixel - src_ave), scale);

		pixel = t_pixel;
	}
	cv::Mat rst_bgr;

	cv::cvtColor(rst_lab, rst_bgr, COLOR_Lab2BGR);

	return rst_bgr;
}

int main(int argc, char** argv)
{
	using namespace cv;

	if (argc != 4)
	{
		std::cout << "usage: img_src_name img_ref_name img_save_name" << std::endl;
	}

	auto src = argv[1];

	src = "1.jpg";
	auto ref = "2.jpg";
	auto save = argv[3];

	auto src_img = imread(src);
	auto ref_img = imread(src);

	ref_img = imread(ref);

	ColorRefConverter converter(src_img, ref_img);
	std::cout << converter.average(ref_img);

	auto rst = converter.convert();

	cv::imwrite("3.jpg", rst);

	//std::cout << img.size[0];
}