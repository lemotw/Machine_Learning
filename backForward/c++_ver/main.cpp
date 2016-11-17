#include "matrixOP.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;
using namespace Math;

MatrixOP<double> lossBackward(MatrixOP<double> y, MatrixOP<double> t)
{return y-t;}

double ReLu(double in_Val)
{return in_Val>0?in_Val:0.0;}

class Layer
{

public:

	Layer(int input, int output, double lr) : input_size{input}, output_size{output},
	learnRate{lr}, bias{0}, grad_b{0}, weight{input, output}, grad_w{0,0},
	topVal{}, downVal{}
	{
		weight.rndSet((double)0.0, (double)1.0);
	}

	MatrixOP<double> forward(MatrixOP<double> in_data);
	MatrixOP<double> backward(MatrixOP<double> in_data);
	MatrixOP<double> sigmoid(MatrixOP<double> in_data);

	MatrixOP<double> weight;
	MatrixOP<double> grad_w;

	MatrixOP<double> topVal;
	MatrixOP<double> downVal;

	int input_size;
	int output_size;

	double bias;
	double grad_b;
	double learnRate;
};

class Net
{

public:
	Net(int inNum = 2, int hidNum = 4, int outNum = 1, double lr = 0.1):fc1{inNum, hidNum, lr},fc2{hidNum, outNum, lr}
	{}

	void train(MatrixOP<double> in_data, MatrixOP<double> target);

	Layer fc1;
	Layer fc2;
};

int main(void)
{
	
	MatrixOP<double> X1{{0,0},{0,1},{1,0},{1,1}};
	MatrixOP<double> Y1{{0},{1},{0},{1}};
	Net net{2,4,1,0.1};
	X1 = X1.transpose();
	Y1 = Y1.transpose();
	net.train(X1,Y1);

	
	return 0;
}


MatrixOP<double> Layer::forward(MatrixOP<double> in_data)
{
	topVal  = weight.transpose() * in_data;
	downVal = in_data;

	topVal  = topVal + bias;

	for(int i=0; i<topVal.rowCount ;++i)
		for(int j=0; j<topVal.colCount ;++j)
			topVal.set(i, j, 1/(1 + exp(0-topVal(i,j) )) );

	return topVal;
}

MatrixOP<double> Layer::backward(MatrixOP<double> in_data)
{
	for(int i=0; i<in_data.rowCount ;++i)
		for(int j=0; j<in_data.colCount ;++j)
			in_data.set(i, j, in_data(i, j)*topVal(i, j)*(1 - topVal(i, j)));

	grad_w = downVal * in_data.transpose();
	grad_b = in_data.sum();

	grad_w = learnRate*grad_w;
	
	weight = weight - grad_w;
	bias  -= grad_b*learnRate;

	return weight * in_data;
}


void Net::train(MatrixOP<double> X, MatrixOP<double> Y)
{

	MatrixOP<double> layer1out{};
	MatrixOP<double> layer2out{};

	MatrixOP<double> layerloss{};
	
	for(int i=0; i<10000 ;++i)
	{

		layer1out  = fc1.forward(X);
		layer2out  = fc2.forward(layer1out);

		layerloss = fc2.backward(lossBackward(layer2out, Y));

		fc1.backward(layerloss);
	}

	layer1out  = fc1.forward(X);
	layer2out  = fc2.forward(layer1out);

	cout << layer1out << endl << layer2out;


}
