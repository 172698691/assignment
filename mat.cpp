// mat.cpp: 定义应用程序的入口点。
//

#include "mat.h"
#include <iostream>
#include <chrono>
#include<stdlib.h>
#include <future>
using namespace std;

float dot(float* v1, float* v2, unsigned len);

mat::mat() 
{
	row = 0;
	column = 0;
	data = NULL;
}
mat::mat(unsigned r, unsigned c)
{
	row = r;
	column = c;
	data = NULL;
}

mat::mat(const mat& m)
{
	cout << "using copy constructor!" << endl;
	row = m.row;
	column = m.column;
	data = m.data;
	data[row][0]++;
}

mat::~mat()
{
	if (data[row][0] == 1)
	{
		for (int i = 0; i <= row; i++)
			delete[] data[i];
		delete[] data;
	}
	else
		data[row][0]--;
}

void mat::set()
{
	data = new float* [row + 1];
	cout << "Creating a matrix with " << row << " rows and " << column << " columns." << endl;
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[column] { 0 };
		cout << "Please the each element of the " << i + 1 << " row, using Blank to seperate each element and using Enter to finish this row :" << endl;
		unsigned j = 0, error = 0;
		float temp;
		do
		{
			cin >> temp;
			if (cin.fail())//异常处理
			{
				cout << "Error : please enter integers!" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				error = 1;
				exit(0);
			}
			else if (j < column)
				data[i][j] = temp;//将输入写入数组
			j++;//计数器
		} while (cin.get() != '\n');
		if (j != column && error == 0)//判断向量的长度是否正确
		{
			cout << "Error : the length of your vector is not right!" << endl;
			error = 1;
			exit(0);
		}
	}
	cout << '\n';
	data[row] = new float[1];//令计数器为1
	data[row][0] = 1;
}

mat mat::operator *(const mat& m)
{
	if (data == NULL || m.data == NULL)
	{
		cout << "Error : the data of matrix has not define!\n" << endl;
		exit(0);
	}
	if (column != m.row)
	{
		cout << "Error : the size of two matrix is not right!\n" << endl;
		exit(0);
	}
	mat n(row, m.column);
	n.data = new float* [n.row + 1];
	for (int i = 0; i < n.row; i++)
	{
		n.data[i] = new float[n.column]{ 0 };
		for (int j = 0; j < n.column; j++)
		{
			float* temp = new float[m.row];
			for (int k = 0; k < m.row; k++)
				temp[k] = m.data[k][j];
			n.data[i][j] = dot(data[i], temp, column);
		}
	}
	n.data[n.row] = new float[1];
	n.data[n.row][0] = 1;
	return n;
}

mat mat::operator *(float f)
{
	
	if (data == NULL)
	{
		cout << "Error : the data of matrix has not define!\n" << endl;
		exit(0);
	}
	mat m(row, column);
	m.data = new float* [row + 1];
	for (int i = 0; i < row; i++)
	{
		m.data[i] = new float[column] { 0 };
		for (int j = 0; j < column; j++)
			m.data[i][j] = f * data[i][j];
	}
	m.data[m.row] = new float[1];
	m.data[m.row][0] = 1;
	return m;
}

void mat::operator =(const mat& m)
{
	cout << "using assign constructor!" << endl;
	row = m.row;
	column = m.column;
	data = m.data;
	data[row][0]++;
}

mat operator *(float f, const mat& m)
{
	if (m.data == NULL)
	{
		cout << "Error: the data of matrix has not define!\n" << endl;
		exit(0);
	}
	mat n(m.row, m.column);
	n.data = new float* [m.row + 1];
	for (int i = 0; i < m.row; i++)
	{
		n.data[i] = new float[m.column] { 0 };
		for (int j = 0; j < m.column; j++)
			n.data[i][j] = f * m.data[i][j];
	}
	n.data[n.row] = new float[1];
	n.data[n.row][0] = 1;
	return n;
}

std::ostream& operator<< (std::ostream& os, const mat& m)
{
	if (m.data == NULL)
	{
		cout << "Error : the data of matrix has not define!\n" << endl;
		exit(0);
	}
	os << "The matrix has " << m.row << " rows and " << m.column << " columns." << endl;
	os.precision(3);
	for (int i = 0; i < m.row; i++)
	{
		for (int j = 0; j < m.column; j++)
			os << m.data[i][j] << " ";
		os << '\n';
	}
	os << '\n';
	return os;
}

float dot(float* v1, float* v2, unsigned len)//通过多线程并行运算的方法
{
	if (len < 100000)//当len太小时直接运算
	{
		if (len % 8 == 0)
		{
			float sum[8] = { 0 };
			__m256 a, b;
			__m256 c = _mm256_setzero_ps();
			for (unsigned i = 0; i < len; i += 8)
			{
				a = _mm256_load_ps(v1 + i);
				b = _mm256_load_ps(v2 + i);
				c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
			}
			_mm256_store_ps(sum, c);
			return (sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7]);
		}
		else
		{
			float sum = 0;
			for (unsigned i = 0; i < len; i++)
				sum += (v1[i] * v2[i]);
			return sum;
		}
	}
	else//当len达到一定大小时使用多线程
	{
		unsigned l1 = len / 10, l2 = 2 * l1, l3 = 3 * l1, l4 = 4 * l1, l5 = 5 * l1, l6 = 6 * l1, l7 = 7 * l1, l8 = 8 * l1, l9 = 9 * l1;//将len平均分为10个部分
		future<float> fut1 = async(std::launch::async, [&] {//第一个子部分
			float sum = 0;
			for (unsigned i = 0; i != l1; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut2 = async(std::launch::async, [&] {//第二个子部分
			float sum = 0;
			for (unsigned i = l1; i != l2; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut3 = async(std::launch::async, [&] {//第三个子部分
			float sum = 0;
			for (unsigned i = l2; i != l3; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut4 = async(std::launch::async, [&] {//......
			float sum = 0;
			for (unsigned i = l3; i != l4; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut5 = async(std::launch::async, [&] {
			float sum = 0;
			for (unsigned i = l4; i != l5; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut6 = async(std::launch::async, [&] {
			float sum = 0;
			for (unsigned i = l5; i != l6; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut7 = async(std::launch::async, [&] {
			float sum = 0;
			for (unsigned i = l6; i != l7; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut8 = async(std::launch::async, [&] {
			float sum = 0;
			for (unsigned i = l7; i != l8; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut9 = async(std::launch::async, [&] {
			float sum = 0;
			for (unsigned i = l8; i != l9; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		future<float> fut10 = async(std::launch::async, [&] {//最后一个子部分
			float sum = 0;
			for (unsigned i = l9; i != len; i++)
				sum += (v1[i] * v2[i]);
			return sum;
			});
		return fut1.get() + fut2.get() + fut3.get() + fut4.get() + fut5.get() + fut6.get() + fut7.get() + fut8.get() + fut9.get() + fut10.get();//对子部分的部分点积求和
	}
}