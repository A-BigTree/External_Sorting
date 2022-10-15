#include<iostream>

using namespace std;


/*----------------Min loser Tree-----------------*/

template<typename T>
class MinLoserTree {

private:
	//������
	int* ls;
	//��������
	T* buffer;
	//�������ݳ���
	int LENGTH;
	//���ò�����Сֵ
	T MIN = -1;

public:
	//Ĭ�Ϲ��캯��
	MinLoserTree() {
		ls = NULL;
		buffer = NULL;
		LENGTH = 0;
	}

	//���캯��
	MinLoserTree(int length) {
		ls = new int[length + 1];
		buffer = new T[length + 1];
		LENGTH = length;
	}
	
	~MinLoserTree() {
		delete[]ls;
		delete[]buffer;
	}

	//������Сֵ
	void setMin(T& _MIN) {
		MIN = _MIN;
	}




};