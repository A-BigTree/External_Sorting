#include<iostream>
#include<set>
#include<fstream>
#include"BufferManage.cpp"

using namespace std;


/*------------------˫�����ȶ���-----------------*/

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

	void clear() {
		m_set.clear();
	}

};

/*----------------ȫ�ֱ���-----------------------*/

//�ڴ��������
const int MAX_MERMARY = 80;
// �������������
const int MAX_BUFFER = 10;
//�м����������
const int MAX_MIDDLE = MAX_MERMARY - 3 * MAX_BUFFER;

//����������
BufferManage<int> INPUT(MAX_BUFFER);
BufferManage<int> LARGE(MAX_BUFFER);
BufferManage<int> SMALL(MAX_BUFFER);

//�м���˫�����ȶ���
DoubleEndedPriorityQueue<int> MIDDLE(MAX_MIDDLE);

//�����ļ�·��
string DATA_FILE = "data//phase2//QuckSort.txt";
string RESULT_FILE = "data//phase2//QuckSortResult.txt";

//��¼I/O����
int IO_TIME = 0;


/*----------------���庯��---------------------*/

//���ɲ�������
void createTestData(int sum) {
	ofstream file;
	file.open(DATA_FILE, ios::out);

	if (!file.fail()) {
		for (int i = 0; i < sum; i++) {
			file << rand() % 1000 << endl;
		}
	}
	file.close();
}

//����INPUT
void readInput(string &filePath, int index) {
	//����INPIUT��дָ��
	INPUT.reset();

	ifstream file;
	file.open(filePath, ios::in);

	IO_TIME++;

	if (!file.fail()) {
		int temp;
		//�ҵ���Ӧλ��
		for (int i = 0; i < index; i++) {
			file >> temp;
		}
		//����INPUT
		while (!INPUT.isWriteOut() && !file.eof()) {
			file >> temp;
			INPUT.bufferInput(temp);
		}
	}
	file.close();
}


//����MIDDLE
bool readMiddle(string &filePath){
	//���set
	MIDDLE.clear();

	ifstream file;
	file.open(filePath, ios::in);

	if (!file.fail()) {
		int temp;
		while (!MIDDLE.isFull() && !file.eof()) {
			file >> temp;
			MIDDLE.insert(temp);
		}
	}

	return file.eof();
}

//дBuffer
void writerBuffer(string& filePath, BufferManage<int>& buffer) {
	ofstream file;
	file.open(filePath, ios::app);

	if (!file.fail()) {
		file << buffer.bufferOutput() << endl;
	}

	file.close();
	buffer.reset();
}

//дMIDDLE
void writeMiddle(string& filePath) {
	ofstream file;
	file.open(filePath, ios::out);

	if (!file.fail()) {
		file << MIDDLE.getMin() << endl;
		MIDDLE.delMin();
	}
	file.close();
}


int main() {

	//createTestData(1000);
	fstream file;
	file.open("data//phase2//test.txt", ios::in | ios::out);

	int temp;
	file >> temp;
	cout << temp << endl;
	file.write("0\n", 3);
	file.write("0\n", 3);
	file.close();

	return 0;
}