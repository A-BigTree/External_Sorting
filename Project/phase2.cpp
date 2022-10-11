#include<iostream>
#include<set>
using namespace std;


//双端优先队列
template<typename T>
class DoubleEndedPriorityQueue {

private:
	//多元素集合
	multiset<T> m_set;
	//集合元素最大数量
	int maxCapacity;

public:
	//构造函数1
	DoubleEndedPriorityQueue() {
		maxCapacity = 0;
	}


	//构造函数2
	DoubleEndedPriorityQueue(int capacity) {
		maxCapacity = capacity;
	}

	//元素数量
	int getSize() {
		return (int)m_set.size();
	}

	//插入元素
	bool insert(T data) {
		m_set.insert(data);

		return this->isFull();
	}

	//集合是否为空
	bool isEmpty() {
		return this->getSize() == 0;
	}

	//集合是否已满
	bool isFull() {
		if (this->getSize() >= maxCapacity) {
			return true;
		}
		return false;
	}

	//得到最大值
	T getMax() {
		return *m_set.rbegin();
	}

	//得到最小值
	T getMin() {
		return *m_set.begin();
	}

	//删除最小值
	void delMin() {
		if (this->isEmpty()) {
			return;
		}

		m_set.erase(m_set.begin());
	}

	//删除最大值
	void delMax() {
		if (this->isEmpty()) {
			return;
		}

		m_set.erase(--m_set.end());
	}

	//得到最大容量
	int getMaxCapacity() {
		return maxCapacity;
	}

	//得到多重集合
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