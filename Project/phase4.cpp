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

	void adjust(int index) {
		int t = (index + LENGTH) / 2;
		while (t > 0) {
			if (buffer[index] > buffer[ls[t]]) {
				ls[t] ^= index;
				index ^= ls[t];
				ls[t] ^= index;
			}
			t /= 2;
		}
		ls[0] = index;
	}

	void build() {
		buffer[LENGTH] = MIN;
		fill(ls, ls + LENGTH + 1, LENGTH);
		for (int i = 0; i < LENGTH; ++i) {
			adjust(i);
		}
	}
};