//External Sort: Quick Sort

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

	//最小值出栈
	T popMin() {
		T temp = this->getMin();
		this->delMin();
		return temp;
	}

	//最大值出栈
	T popMax() {
		T temp = this->getMax();
		this->delMax();
		return temp;
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

//排序数据个数
const int MAX_SUM = 1000;

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

//中间缓存文件路径
string CACHE_TEMP = "data//phase2//cache//temp.txt";
string CACHE_MIDDLE = "data//phase2//cache//middle.txt";

//记录I/O次数
int IO_TIME = 0;

//数据长度
const int INT_LENGTH = 8;

//生成测试数据
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

//整数转为8位字符串
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

//读入INPUT
bool readInput(int index, int left) {
	IO_TIME++;

	//重置INPIUT读写指针
	INPUT.reset();

	ifstream file;
	file.open(CACHE_TEMP, ios::in);

	int i = index;

	if (!file.fail()) {
		char temp[INT_LENGTH + 1];
		//读入INPUT
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

//读入MIDDLE
bool readMiddle(int right, int left) {
	IO_TIME++;

	//清空set
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

//在指定位置写MIDDLE
void writeMiddle(fstream& file, int index) {
	IO_TIME++;

	file.seekp(index * (INT_LENGTH + 1), ios::beg);

	int size = MIDDLE.getSize();

	for (int i = 0; i < size; i++) {
		file.write(intToString(MIDDLE.popMin()).append(" ").c_str(), INT_LENGTH + 1);
	}
}

//写Buffer
void writerBuffer(fstream& file, BufferManage<int>& buffer, int index) {
	IO_TIME++;

	int i = index;

	while (!buffer.isReadOut()) {
		file.seekp((i++) * (INT_LENGTH + 1), ios::beg);
		file.write(intToString(buffer.bufferOutput()).append(" ").c_str(), INT_LENGTH + 1);
	}

	buffer.reset();
}

//结果复制到Cache
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

//数据写入Cache
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

//缓存写入结果文件
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

/*-----------------------外部快排解决方法--------------------------*/

//外部快速排序
class ExternalQuickSort {
private:
	//middle写
	fstream middleFile;

public:
	ExternalQuickSort() {
		middleFile.open(CACHE_MIDDLE, ios::out | ios::in);
	}

	//实现函数
	void quickSort(int left, int right) {
		cout << "left:right " << left << ":" << right << endl << endl;

		//填充middle
		if (readMiddle(left, right)) {
			writeMiddle(middleFile, left);
			return;
		}

		//记录small、large缓存位置
		int rs = left, rl = right;

		//记录Input读取位置
		int indexIn = left + MIDDLE.getSize();

		while (true) {
			//读入INPUT缓存
			bool flag = readInput(indexIn, right);
			//更新下一次读取位置
			indexIn += INPUT.getPosW();

			while (!INPUT.isReadOut()) {
				//读取INPUT
				int temp = INPUT.bufferOutput();

				if (temp <= MIDDLE.getMin()) {//小于等于middle最小值
					SMALL.bufferInput(temp);
				}
				else if (temp >= MIDDLE.getMax()) {//大于等于middle最大值
					LARGE.bufferInput(temp);
				}
				else {//处于中间值
					if (SMALL.getPosW() <= LARGE.getPosW()) {//SAMLL缓存区数目较少
						SMALL.bufferInput(MIDDLE.popMin());
						MIDDLE.insert(temp);
					}
					else {//LARGR缓存区数目少
						LARGE.bufferInput(MIDDLE.popMax());
						MIDDLE.insert(temp);
					}
				}

				//判断SMALL、LARGR是否需要写回
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

			//文件读取完成
			if (flag) {
				break;
			}
		}

		cout << "left:right " << left << ":" << right << endl;
		cout << "small:" << rs << " large: " << rl << " sum: " << rs + rl << endl << endl;

		//写剩余的small、large
		int size = SMALL.getPosW();
		writerBuffer(middleFile, SMALL, rs);
		rs += size;
		size = LARGE.getPosW();
		writerBuffer(middleFile, LARGE, rl - size);
		rl -= size;

		//写middle
		writeMiddle(middleFile, rs);

		//复制文件
		copyCache(middleFile);

		quickSort(left, rs);
		quickSort(rl, right);
	}

	//运行函数
	void run() {
		//copy数据到缓存文件
		dataToCache();
		//进行外部排序
		this->quickSort(0, MAX_SUM);
		this->middleFile.close();
		//cache->Result
		cacheToResult();
		//清空缓存文件
		ofstream file;
		file.open(CACHE_TEMP, ios::out);
		file.close();
		file.open(CACHE_MIDDLE, ios::out);
		file.close();
	}
};

/*------------------------主函数-------------------------*/

int main() {
	//生成测试数据
	//createTestData();

	ExternalQuickSort qs;
	qs.run();

	cout << "Data Size: " << MAX_SUM << endl;
	cout << "I/O times: " << IO_TIME << endl;

	return 0;
}