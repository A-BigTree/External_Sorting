#include<iostream>
#include<set>
using namespace std;


//˫�����ȶ���
template<typename T>
class DoubleEndedPriorityQueue {

private:
	//��Ԫ�ؼ���
	multiset<T> m_set;
	//����Ԫ���������
	int maxCapacity;

public:
	//���캯��1
	DoubleEndedPriorityQueue() {
		maxCapacity = 0;
	}


	//���캯��2
	DoubleEndedPriorityQueue(int capacity) {
		maxCapacity = capacity;
	}

	//Ԫ������
	int getSize() {
		return (int)m_set.size();
	}

	//����Ԫ��
	bool insert(T data) {
		m_set.insert(data);

		return this->isFull();
	}

	//�����Ƿ�Ϊ��
	bool isEmpty() {
		return this->getSize() == 0;
	}

	//�����Ƿ�����
	bool isFull() {
		if (this->getSize() >= maxCapacity) {
			return true;
		}
		return false;
	}

	//�õ����ֵ
	T getMax() {
		return *m_set.rbegin();
	}

	//�õ���Сֵ
	T getMin() {
		return *m_set.begin();
	}

	//ɾ����Сֵ
	void delMin() {
		if (this->isEmpty()) {
			return;
		}

		m_set.erase(m_set.begin());
	}

	//ɾ�����ֵ
	void delMax() {
		if (this->isEmpty()) {
			return;
		}

		m_set.erase(--m_set.end());
	}

	//�õ��������
	int getMaxCapacity() {
		return maxCapacity;
	}

	//�õ����ؼ���
	multiset<T>& getM_Set() {
		return &m_set;
	}

};




int main() {

	DoubleEndedPriorityQueue<int> temp(10);
	temp.insert(3);
	temp.insert(2);
	temp.insert(3);
	temp.insert(4);
	temp.insert(1);

	cout << "size:" << temp.getSize() << endl;
	cout << "max:" << temp.getMax() << endl;
	cout << "min:" << temp.getMin() << endl << endl;

	temp.delMin();

	cout << "size:" << temp.getSize() << endl;
	cout << "max:" << temp.getMax() << endl;
	cout << "min:" << temp.getMin() << endl << endl;


	return 0;
}