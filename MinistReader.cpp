#include "MinistReader.h"
#include <iostream>

extern "C" {
#include <Winsock2.h>
}

using namespace std;

MinistReader::MinistReader(std::string filePath):inputStream(filePath,ios::binary)
{
	int32_t fileType = 0;
	inputStream.read((char*)&fileType,sizeof(fileType));
	printf("the file type:%ld \n", ntohl(fileType));
	type = (Type)ntohl(fileType);
	inputStream.read((char*)&itemCount,sizeof(itemCount));
	itemCount = ntohl(itemCount);
	cout << "the item count:" << itemCount << endl;
	if (type == Image) {
		inputStream.read((char*)&rows, sizeof(rows));
		rows = ntohl(rows);
		inputStream.read((char*)&columns, sizeof(columns));
		columns = ntohl(columns);
		cout << "the image rows:" << rows << " columns:" << columns << endl;
	}
}

MinistReader::Type MinistReader::getType() {
	return type;
}

char MinistReader::readLabel() {
	if (type != Lable)
	{
		return 30;
	}
	char digital = 0;
	inputStream.read(&digital,sizeof(digital));
	return digital;
}

cv::Mat MinistReader::readImage() {
	cv::Mat m(columns,rows,CV_32FC1);
	if (type != Image)
		return m;
	for (int i = 0; i < rows; i++) {
		float* line = m.ptr<float>(i);
		for (int j = 0; j < columns; j++) {
			unsigned char pixel;
			inputStream.read((char*)&pixel,sizeof(pixel));
			line[j] = (pixel+0.0)/255.0;
		}
	}
	return m;
}

bool MinistReader::hasNext() {
	return inputStream.peek() != EOF;
}

int MinistReader::getCount() {
	return itemCount;
}

int32_t MinistReader::getRows() {
	return rows;
}

int32_t MinistReader::getCols() {
	return columns;
}

MinistReader::~MinistReader()
{
	if (inputStream.is_open()) {
		inputStream.close();
	}
}

