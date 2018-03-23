#pragma once
#include <string>
#include <fstream>
#include <cstdint>

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
	bool hasNext();
	int getCount();
	~MinistReader();
};

