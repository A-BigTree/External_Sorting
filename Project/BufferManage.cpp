#include "BufferManage.h"
#include<iostream>
using namespace std;

template<typename T>
BufferManage<T>::BufferManage()
{
	buffer = NULL;
	capcity = 0;
	posR = 0;
	posW = 0;
}

template<typename T>
BufferManage<T>::BufferManage(int n) {
	capcity = n;
	buffer = new T[n];
	posR = 0;
	posW = 0;
}

template<typename T>
BufferManage<T>::BufferManage(T data[], int n) {
	capcity = n;
	buffer = new T[n];
	for (int i = 0; i < n; i++) {
		buffer[i] = data[i];
	}
	posR = 0;
	posW = n;
}

template<typename T>
void BufferManage<T>::initBuffer(int n)
{
	if (buffer != NULL) {
		this->free();
	}
	capcity = n;
	buffer = new T[n];
}

template<typename T>
void BufferManage<T>::reset() {
	posW = 0;
	posR = 0;
}

template<typename T>
void BufferManage<T>::reset(T data[], int n) {
	int num = n;
	if (n > capcity) {
		num = capcity;
	}
	posW = num;
	for (int i = 0; i < num; i++) {
		buffer[i] = data[i];
	}
	posR = 0;
}

template<typename T>
bool BufferManage<T>::bufferInput(T data) {
	if (posW >= capcity) {
		return false;
	}
	buffer[posW++] = data;
	return true;
}

template<typename T>
T BufferManage<T>::bufferOutput() {
	if (posR >= posW) {
		posR = 0;
	}
	return buffer[posR++];
}

template<typename T>
bool BufferManage<T>::isReadOut() {
	if (posR >= posW) {
		return true;
	}
	return false;
}

template<typename T>
bool BufferManage<T>::isWriteOut(){
	if (posW >= capcity) {
		return true;
	}
	return false;
}

template<typename T>
T BufferManage<T>::getIndexData(int index){
	if (index >= capcity) {
		printf("Index(%d) is out of index(%d)\n", index, capcity - 1);
		posR = capcity;
		return buffer[capcity - 1];
	}
	posR = index + 1;
	return buffer[index];
}

template<typename T>
bool BufferManage<T>::setIndexData(T data, int index){
	if (index >= capcity) {
		printf("Index(%d) is out of index(%d)\n", index, capcity - 1);
		return false;
	}
	if (index > posW) {
		posW = index + 1;
	}  
	buffer[index] = data;
	return true;
}

template<typename T>
int BufferManage<T>::getCapcity()
{
	return capcity;
}

template<typename T>
int BufferManage<T>::getPosW()
{
	return posW;
}

template<typename T>
int BufferManage<T>::getPosR()
{
	return posR;
}

template<typename T>
T* BufferManage<T>::getBuffer()
{
	return buffer;
}

template<typename T>
void BufferManage<T>::free()
{
	if (buffer != NULL) {
		delete[]buffer;
		buffer = NULL;
	}
	capcity = 0;
	posR = 0;
	posW = 0;
}


