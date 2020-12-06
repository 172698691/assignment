// mat.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#ifndef MAT_H
#define MAT_H
#include <iostream>


class mat {
private:
	unsigned row;
	unsigned column;
	float** data;
public:
	mat();
	mat(unsigned r, unsigned c);
	mat(const mat& m);
	~mat();
	void set();
	mat operator *(const mat& m);
	mat operator *(float f);
    void operator =(const mat& m);
	friend mat operator *(float f, const mat& m);
	friend std::ostream& operator<< (std::ostream& os, const mat& m);
};

#endif