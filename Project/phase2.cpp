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
string CACHE_SMALL = "data//phase2//cache//small.txt";
string CACHE_LARGE = "data//phase2//cache//large.txt";
string CACHE_MIDDLE = "data//phase2//cache//middle.txt";

//��¼I/O����
int IO_TIME = 0;

//���ݳ���
const int INT_LENGTH = 8;


/*----------------���庯��---------------------*/

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
bool readInput(string& filePath, int index, int left) {

	IO_TIME++;

	//����INPIUT��дָ��
	INPUT.reset();

	ifstream file;
	file.open(filePath, ios::in);

	int i = 0;

	if (!file.fail()) {
		int temp;
		//�ҵ���Ӧλ��
		for (i; i < index; i++) {
			file >> temp;
		}
		//����INPUT
		while (!INPUT.isWriteOut() && !file.eof() && i < left) {
			file >> temp;
			INPUT.bufferInput(temp);
			i++;
		}
	}
	file.close();

	return file.eof() || i >= left;
}

//����MIDDLE
bool readMiddle(string& filePath, int right, int left) {

	IO_TIME++;

	//���set
	MIDDLE.clear();

	ifstream file;
	file.open(filePath, ios::in);

	int i = 0;

	if (!file.fail()) {
		int temp;

		
		for (i; i < right; i++) {
			file >> temp;
		}

		while (!MIDDLE.isFull() && !file.eof() && i < left) {
			i++;
			file >> temp;
			MIDDLE.insert(temp);
		}
	}

	return file.eof() || i >= left;
}

//дBuffer
void writerBuffer(string& filePath, BufferManage<int>& buffer) {

	IO_TIME++;

	ofstream file;
	file.open(filePath, ios::app);

	if (!file.fail()) {
		while (!buffer.isReadOut()) {
			file << buffer.bufferOutput() << endl;
		}
	}

	file.close();
	buffer.reset();
}

//��ָ��λ��дMIDDLE
void writeMiddle(ofstream &file, int index) {

	IO_TIME++;

	file.seekp(index * (INT_LENGTH + 1), ios::beg);

	int size = MIDDLE.getSize();

	for (int i = 0; i < size; i++) {
		file << intToString(MIDDLE.popMin()) << " ";
	}

}

//�ļ�����
void copyFile(string inputFile, string outputFile, int num, ios_base::openmode _Mode = ios::out) {

	IO_TIME++;

	ifstream ifile;
	ofstream ofile;

	ifile.open(inputFile, ios::in);
	ofile.open(outputFile, _Mode);

	if (ifile.fail() || ofile.fail()) {
		return;
	}

	int temp;
	for (int i = 0; i < num; i++) {
		ifile >> temp;
		ofile << temp << endl;
	}

	ifile.close();
	ofile.close();
}

//middle����д�����ļ�
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
	ofstream middleFile;
	//��¼����������Ŀ
	int sum;

public:
	ExternalQuickSort() {
		middleFile.open(CACHE_MIDDLE, ios::out);
		sum = MAX_SUM;
	}


	//ʵ�ֺ���
	void quickSort(int left, int right, int startIndex) {
		
		//���small.txt��large.txt
		ofstream file;
		file.open(CACHE_SMALL, ios::out);
		file.close();
		file.open(CACHE_LARGE, ios::out);
		file.close();

		cout << "left:right " << left << ":" << right << endl;
		cout << "Data sum:" << sum << endl;
		cout << "Cache start index:" << startIndex << endl <<endl;

		//��¼��Զ�ȡ����λ��
		int endIndex = startIndex + right - left;

		//���middle
		if (readMiddle(CACHE_TEMP, startIndex, endIndex)) {
			writeMiddle(middleFile, left);
			return;
		}

		//��¼small��large��������
		int rs = 0, rl = 0;

		//��¼Input��ȡλ��
		int indexIn = startIndex + MIDDLE.getSize();

		while (true) {
			//����INPUT����
			bool flag = readInput(CACHE_TEMP, indexIn, endIndex);
			//������һ�ζ�ȡλ��
			indexIn += INPUT.getPosW();

			while (!INPUT.isReadOut()) {
				//��ȡINPUT
				int temp = INPUT.bufferOutput();

				if (temp <= MIDDLE.getMin()) {//С�ڵ���middle��Сֵ
					SMALL.bufferInput(temp);
					rs++;

				}else if (temp >= MIDDLE.getMax()) {//���ڵ���middle���ֵ
					LARGE.bufferInput(temp);
					rl++;

				}else {//�����м�ֵ

					if (SMALL.getPosW() <= LARGE.getPosW()) {//SAMLL��������Ŀ����
						SMALL.bufferInput(MIDDLE.popMin());
						MIDDLE.insert(temp);
						rs++;

					}else {//LARGR��������Ŀ��
						LARGE.bufferInput(MIDDLE.popMax());
						MIDDLE.insert(temp);
						rl++;
					}

				}

				//�ж�SMALL��LARGR�Ƿ���Ҫд��
				if (SMALL.isWriteOut()) {
					writerBuffer(CACHE_SMALL, SMALL);
				}
				if (LARGE.isWriteOut()) {
					writerBuffer(CACHE_LARGE, LARGE);
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
		writerBuffer(CACHE_SMALL, SMALL);
		writerBuffer(CACHE_LARGE, LARGE);

		//дmiddle
		writeMiddle(middleFile, left + rs);

		//small��large�ļ����Ƶ������ļ���
		copyFile(CACHE_SMALL, CACHE_TEMP, rs, ios::app);
		copyFile(CACHE_LARGE, CACHE_TEMP, rl, ios::app);

		int dataSum = sum;
		sum += (rs + rl);

		quickSort(left, left + rs, dataSum);
		quickSort(right - rl, right, dataSum + rs);

	}

	//���к���
	void run() {
		//copy���ݵ������ļ�
		copyFile(DATA_FILE, CACHE_TEMP, MAX_SUM, ios::out);
		//�����ⲿ����
		this->quickSort(0, MAX_SUM, 0);
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



int main() {
	//���ɲ�������
	//createTestData();

	ExternalQuickSort qs;
	qs.run();

	cout << "Data Size: " << MAX_SUM << endl;
	cout << "I/O times: " << IO_TIME << endl;

	return 0;
}