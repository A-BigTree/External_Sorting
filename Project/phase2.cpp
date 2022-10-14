//External Sort: Quick Sort

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

	//��Сֵ��ջ
	T popMin() {
		T temp = this->getMin();
		this->delMin();
		return temp;
	}

	//���ֵ��ջ
	T popMax() {
		T temp = this->getMax();
		this->delMax();
		return temp;
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

//�������ݸ���
const int MAX_SUM = 1000;

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

//�м仺���ļ�·��
string CACHE_TEMP = "data//phase2//cache//temp.txt";
string CACHE_MIDDLE = "data//phase2//cache//middle.txt";

//��¼I/O����
int IO_TIME = 0;

//���ݳ���
const int INT_LENGTH = 8;

//���ɲ�������
void createTestData() {
	ofstream file, file_re;
	file.open(DATA_FILE, ios::out);
	file_re.open(RESULT_FILE, ios::out);

	if (!file.fail() && !file_re.fail()) {
		for (int i = 0; i < MAX_SUM; i++) {
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
string intToString(int num) {
	string str = "";
	for (int i = 0; i < INT_LENGTH; i++) {
		str.append("0");
	}

	string intS = to_string(num);
	for (int i = 0; i < intS.length(); i++) {
		str[INT_LENGTH - intS.length() + i] = intS[i];
	}
	return str;
}

//����INPUT
bool readInput(int index, int left) {
	IO_TIME++;

	//����INPIUT��дָ��
	INPUT.reset();

	ifstream file;
	file.open(CACHE_TEMP, ios::in);

	int i = index;

	if (!file.fail()) {
		char temp[INT_LENGTH + 1];
		//����INPUT
		while (!INPUT.isWriteOut() && i < MAX_SUM && i < left) {
			file.seekg(i * (INT_LENGTH + 1), ios::beg);
			file.get(temp, INT_LENGTH + 1);
			INPUT.bufferInput(stoi(temp));
			i++;
		}
	}
	file.close();

	return i >= MAX_SUM || i >= left;
}

//����MIDDLE
bool readMiddle(int right, int left) {
	IO_TIME++;

	//���set
	MIDDLE.clear();

	ifstream file;
	file.open(CACHE_TEMP, ios::in);

	int i = right;

	if (!file.fail()) {
		char temp[INT_LENGTH + 1];

		while (!MIDDLE.isFull() && i < MAX_SUM && i < left) {
			file.seekg(i * (INT_LENGTH + 1), ios::beg);
			file.get(temp, INT_LENGTH + 1);
			MIDDLE.insert(stoi(temp));
			i++;
		}
	}
	file.close();

	return file.eof() || i >= left;
}

//��ָ��λ��дMIDDLE
void writeMiddle(fstream& file, int index) {
	IO_TIME++;

	file.seekp(index * (INT_LENGTH + 1), ios::beg);

	int size = MIDDLE.getSize();

	for (int i = 0; i < size; i++) {
		file.write(intToString(MIDDLE.popMin()).append(" ").c_str(), INT_LENGTH + 1);
	}
}

//дBuffer
void writerBuffer(fstream& file, BufferManage<int>& buffer, int index) {
	IO_TIME++;

	int i = index;

	while (!buffer.isReadOut()) {
		file.seekp((i++) * (INT_LENGTH + 1), ios::beg);
		file.write(intToString(buffer.bufferOutput()).append(" ").c_str(), INT_LENGTH + 1);
	}

	buffer.reset();
}

//������Ƶ�Cache
void copyCache(fstream& file) {
	IO_TIME++;

	ofstream ofile;

	ofile.open(CACHE_TEMP, ios::out);

	char temp[INT_LENGTH + 1];
	for (int i = 0; i < MAX_SUM; i++) {
		file.seekg(i * (INT_LENGTH + 1), ios::beg);
		file.get(temp, INT_LENGTH + 1);
		ofile.seekp(i * (INT_LENGTH + 1), ios::beg);
		ofile << intToString(stoi(temp)) << " ";
	}
	ofile.close();
}

//����д��Cache
void dataToCache() {
	IO_TIME++;

	ifstream ifile;
	ofstream ofile;

	ifile.open(DATA_FILE, ios::in);
	ofile.open(CACHE_TEMP, ios::out);

	if (ifile.fail() || ofile.fail()) {
		return;
	}

	int temp;
	for (int i = 0; i < MAX_SUM; i++) {
		ifile >> temp;
		ofile << intToString(temp) << " ";
	}

	ifile.close();
	ofile.close();
}

//����д�����ļ�
void cacheToResult() {
	IO_TIME++;

	ifstream ifile;
	ofstream ofile;

	ifile.open(CACHE_MIDDLE, ios::in);
	ofile.open(RESULT_FILE, ios::out);

	if (ifile.fail() || ofile.fail()) {
		return;
	}

	char temp[INT_LENGTH + 1];
	for (int i = 0; i < MAX_SUM; i++) {
		ifile.seekg(i * (INT_LENGTH + 1), ios::beg);

		ifile.get(temp, INT_LENGTH + 1);

		ofile << stoi(temp) << endl;
	}

	ifile.close();
	ofile.close();
}

/*-----------------------�ⲿ���Ž������--------------------------*/

//�ⲿ��������
class ExternalQuickSort {
private:
	//middleд
	fstream middleFile;

public:
	ExternalQuickSort() {
		middleFile.open(CACHE_MIDDLE, ios::out | ios::in);
	}

	//ʵ�ֺ���
	void quickSort(int left, int right) {
		cout << "left:right " << left << ":" << right << endl << endl;

		//���middle
		if (readMiddle(left, right)) {
			writeMiddle(middleFile, left);
			return;
		}

		//��¼small��large����λ��
		int rs = left, rl = right;

		//��¼Input��ȡλ��
		int indexIn = left + MIDDLE.getSize();

		while (true) {
			//����INPUT����
			bool flag = readInput(indexIn, right);
			//������һ�ζ�ȡλ��
			indexIn += INPUT.getPosW();

			while (!INPUT.isReadOut()) {
				//��ȡINPUT
				int temp = INPUT.bufferOutput();

				if (temp <= MIDDLE.getMin()) {//С�ڵ���middle��Сֵ
					SMALL.bufferInput(temp);
				}
				else if (temp >= MIDDLE.getMax()) {//���ڵ���middle���ֵ
					LARGE.bufferInput(temp);
				}
				else {//�����м�ֵ
					if (SMALL.getPosW() <= LARGE.getPosW()) {//SAMLL��������Ŀ����
						SMALL.bufferInput(MIDDLE.popMin());
						MIDDLE.insert(temp);
					}
					else {//LARGR��������Ŀ��
						LARGE.bufferInput(MIDDLE.popMax());
						MIDDLE.insert(temp);
					}
				}

				//�ж�SMALL��LARGR�Ƿ���Ҫд��
				if (SMALL.isWriteOut()) {
					int size = SMALL.getPosW();
					writerBuffer(middleFile, SMALL, rs);
					rs += size;
				}
				if (LARGE.isWriteOut()) {
					int size = LARGE.getPosW();
					writerBuffer(middleFile, LARGE, rl - size);
					rl -= size;
				}
			}

			//�ļ���ȡ���
			if (flag) {
				break;
			}
		}

		cout << "left:right " << left << ":" << right << endl;
		cout << "small:" << rs << " large: " << rl << " sum: " << rs + rl << endl << endl;

		//дʣ���small��large
		int size = SMALL.getPosW();
		writerBuffer(middleFile, SMALL, rs);
		rs += size;
		size = LARGE.getPosW();
		writerBuffer(middleFile, LARGE, rl - size);
		rl -= size;

		//дmiddle
		writeMiddle(middleFile, rs);

		//�����ļ�
		copyCache(middleFile);

		quickSort(left, rs);
		quickSort(rl, right);
	}

	//���к���
	void run() {
		//copy���ݵ������ļ�
		dataToCache();
		//�����ⲿ����
		this->quickSort(0, MAX_SUM);
		this->middleFile.close();
		//cache->Result
		cacheToResult();
		//��ջ����ļ�
		ofstream file;
		file.open(CACHE_TEMP, ios::out);
		file.close();
		file.open(CACHE_MIDDLE, ios::out);
		file.close();
	}
};

/*------------------------������-------------------------*/

int main() {
	//���ɲ�������
	//createTestData();

	ExternalQuickSort qs;
	qs.run();

	cout << "Data Size: " << MAX_SUM << endl;
	cout << "I/O times: " << IO_TIME << endl;

	return 0;
}