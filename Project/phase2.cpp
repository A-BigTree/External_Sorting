#include<iostream>
#include<set>
#include<fstream>
#include<string>
#include"BufferManage.cpp"

using namespace std;


/*------------------双端优先队列-----------------*/

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

	void clear() {
		m_set.clear();
	}

};

/*----------------全局变量-----------------------*/

//内存最大容量
const int MAX_MERMARY = 80;
// 缓冲区最大容量
const int MAX_BUFFER = 10;
//中间区最大容量
const int MAX_MIDDLE = MAX_MERMARY - 3 * MAX_BUFFER;

//三个缓冲区
BufferManage<int> INPUT(MAX_BUFFER);
BufferManage<int> LARGE(MAX_BUFFER);
BufferManage<int> SMALL(MAX_BUFFER);

//中间区双端优先队列
DoubleEndedPriorityQueue<int> MIDDLE(MAX_MIDDLE);

//数据文件路径
string DATA_FILE = "data//phase2//QuckSort.txt";
string RESULT_FILE = "data//phase2//QuckSortResult.txt";

//记录I/O次数
int IO_TIME = 0;

//数据长度
const int INT_LENGTH = 8;


/*----------------具体函数---------------------*/

//生成测试数据
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

//整数转为8位字符串
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



//读入INPUT
void readInput(string &filePath, int index) {
	//重置INPIUT读写指针
	INPUT.reset();

	ifstream file;
	file.open(filePath, ios::in);

	IO_TIME++;

	if (!file.fail()) {
		int temp;
		//找到对应位置
		for (int i = 0; i < index; i++) {
			file >> temp;
		}
		//读入INPUT
		while (!INPUT.isWriteOut() && !file.eof()) {
			file >> temp;
			INPUT.bufferInput(temp);
		}
	}
	file.close();
}


//读入MIDDLE
bool readMiddle(string &filePath){
	//清空set
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

//写Buffer
void writerBuffer(string& filePath, BufferManage<int>& buffer) {
	ofstream file;
	file.open(filePath, ios::app);

	if (!file.fail()) {
		file << buffer.bufferOutput() << endl;
	}

	file.close();
	buffer.reset();
}

//写MIDDLE
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