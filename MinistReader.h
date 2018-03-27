#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include <opencv2/core.hpp>

class MinistReader
{
private:
	std::ifstream inputStream;
	int32_t itemCount;
	int32_t rows;
	int32_t columns;
public:
	enum Type
	{
		Lable = 2049,
		Image = 2051
	}type;
	MinistReader(std::string filePath);
	Type getType();
	char readLabel();
	cv::Mat readImage();
	bool hasNext();
	int getCount();
	int32_t getRows();
	int32_t getCols();
	~MinistReader();
};

