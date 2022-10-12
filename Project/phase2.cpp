#include<iostream>
#include<set>
#include<fstream>
#include<string>
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

//���ݳ���
const int INT_LENGTH = 8;


/*----------------���庯��---------------------*/

//���ɲ�������
void createTestData(int sum) {
	ofstream file, file_re;
	file.open(DATA_FILE, ios::out);
	file_re.open(RESULT_FILE, ios::out);

	if (!file.fail() && !file_re.fail()) {
		for (int i = 0; i < sum; i++) {
			file << rand() << endl;
			for (int i = 0; i < INT_LENGTH; i++) {
				file_re << 0;
			}
			file_re << endl;
		}
	}
	file.close();
	file_re.close();
}

//����תΪ8λ�ַ���
char* intToString(int num) {
	char* str = new char[INT_LENGTH + 1];
	fill(str, str + INT_LENGTH, '0');
	str[INT_LENGTH] = '\0';

	string intS = to_string(num);
	for (int i = 0; i < intS.length(); i++) {
		str[INT_LENGTH - intS.length() + i] = intS[i];
	}
	return str;
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
	file.open("data//phase2//test.txt", ios::out | ios::in);

	int index = 5;
	int temp;
	for (int i = 0; i < index; i++) {
		file >> temp;
	}

	file.seekp(file.tellg(), ios::beg);
	char* temp = intToString(56789);
	for (int i = 0; i < INT_LENGTH; i++) {
		file << to_string(temp);
	}
	
	return 0;
}