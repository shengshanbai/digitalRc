#include "MinistReader.h"

#define MNIST_DIR "E:\\Projects\\cpp"
#include <iostream>
#include <opencv2/ml.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int trainSvmModel(enum ml::SVM::KernelTypes kernelType,int iterCount,float c) {
	MinistReader labelReader(MNIST_DIR"/train-labels-idx1-ubyte");
	MinistReader imageReader(MNIST_DIR"/train-images.idx3-ubyte");
	Mat lablesMat(labelReader.getCount(),1, CV_32SC1);
	int imageVectorLength = imageReader.getCols()*imageReader.getRows();
	Mat trainDataMat(imageReader.getCount(),imageVectorLength,CV_32FC1);
	int i = 0;
	while (labelReader.hasNext())
	{
		int digital = labelReader.readLabel();
		int32_t* dataInt =(int32_t*)(lablesMat.data + i * lablesMat.elemSize());
		*dataInt = digital;
		Mat image = imageReader.readImage();
		memcpy(trainDataMat.data + i * imageVectorLength * sizeof(float),image.data,imageVectorLength*sizeof(float));
		i++;
	}
	Ptr<ml::SVM> svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(kernelType);
	svm->setC(c);
	svm->setGamma(0.01);
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, iterCount, FLT_EPSILON));
	const Ptr<ml::TrainData> trainData = ml::TrainData::create(trainDataMat, ml::ROW_SAMPLE,lablesMat);
	bool result = svm->train(trainData);
	if (result) {
		svm->save("model.xml");
		return 0;
	}
	return -1;
}

int testSvmModel(string path) {
	Ptr<ml::SVM> svm = ml::SVM::load(path);
	int sum = 0;
	//对每一个测试图像进行SVM分类预测
	MinistReader labelReader(MNIST_DIR"/t10k-labels.idx1-ubyte");
	MinistReader imageReader(MNIST_DIR"/t10k-images.idx3-ubyte");
	while (labelReader.hasNext())
	{
		int digital = labelReader.readLabel();
		Mat image = imageReader.readImage();
		int imgVectorLen = imageReader.getCols()*imageReader.getRows();
		Mat predict_mat = Mat::zeros(1, imgVectorLen, CV_32FC1);
		memcpy(predict_mat.data, image.data, imgVectorLen * sizeof(float));
		int predict_label = svm->predict(predict_mat);
		if (predict_label == digital)
		{
			sum++;
		}
	}
	cout << "预测准确率为：" << (double)sum / (double)labelReader.getCount() << endl;
	return 0;
}

int detectImage(string modelPath,string imagePath) {
	Mat image = imread(imagePath,CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty()) {
		cout << "read image failed!" << endl;
		return -1;
	}
	//进行边缘检测
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y; 
	Sobel(image, grad_x, CV_8U, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(image, grad_y, CV_8U, 0, 1, 3);
	convertScaleAbs(grad_y, abs_grad_y);
	Mat grad;
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	//二值化
	threshold(grad,grad,25, 255, CV_THRESH_BINARY);
	//两次膨胀和腐蚀
	Mat dilateK = getStructuringElement(MORPH_RECT,Size(20,8));
	Mat erodeK = getStructuringElement(MORPH_RECT, Size(20, 6));
	dilate(grad,grad,dilateK);
	erode(grad,grad,erodeK);
	dilate(grad, grad, dilateK);
	erode(grad, grad, erodeK);
	return 0;
}

#define PROJECT_PATH "C:\\projects\\digitalRc"

int main(int argc,char** argv){
	int out = detectImage(PROJECT_PATH"/model_brf.xml", PROJECT_PATH"/image1.jpg");
	getchar();
	return 0;
}