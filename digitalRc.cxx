#include "MinistReader.h"

#define MNIST_DIR "E:\\Projects\\cpp"
#include <iostream>

using namespace std;

int main(int argc,char** argv){
	MinistReader labelReader(MNIST_DIR"/train-labels-idx1-ubyte");
	int i = 0;
	while (labelReader.hasNext())
	{
		char digital = labelReader.readLabel();
		i++;
	}
	cout << "the actual count:" << i << "the read count:"<<labelReader.getCount()<<endl;
	getchar();
	return 0;
}