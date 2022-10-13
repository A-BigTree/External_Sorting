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
string CACHE_SMALL = "data//phase2//cache//small.txt";
string CACHE_LARGE = "data//phase2//cache//large.txt";
string CACHE_MIDDLE = "data//phase2//cache//middle.txt";

//记录I/O次数
int IO_TIME = 0;

//数据长度
const int INT_LENGTH = 8;


/*----------------具体函数---------------------*/

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
bool readInput(string& filePath, int index, int left) {

	IO_TIME++;

	//重置INPIUT读写指针
	INPUT.reset();

	ifstream file;
	file.open(filePath, ios::in);

	int i = 0;

	if (!file.fail()) {
		int temp;
		//找到对应位置
		for (i; i < index; i++) {
			file >> temp;
		}
		//读入INPUT
		while (!INPUT.isWriteOut() && !file.eof() && i < left) {
			file >> temp;
			INPUT.bufferInput(temp);
			i++;
		}
	}
	file.close();

	return file.eof() || i >= left;
}

//读入MIDDLE
bool readMiddle(string& filePath, int right, int left) {

	IO_TIME++;

	//清空set
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

//写Buffer
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

//在指定位置写MIDDLE
void writeMiddle(ofstream &file, int index) {

	IO_TIME++;

	file.seekp(index * (INT_LENGTH + 1), ios::beg);

	int size = MIDDLE.getSize();

	for (int i = 0; i < size; i++) {
		file << intToString(MIDDLE.popMin()) << " ";
	}

}

//文件复制
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

//middle缓存写入结果文件
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
	ofstream middleFile;
	//记录缓存数据数目
	int sum;

public:
	ExternalQuickSort() {
		middleFile.open(CACHE_MIDDLE, ios::out);
		sum = MAX_SUM;
	}


	//实现函数
	void quickSort(int left, int right, int startIndex) {
		
		//清空small.txt、large.txt
		ofstream file;
		file.open(CACHE_SMALL, ios::out);
		file.close();
		file.open(CACHE_LARGE, ios::out);
		file.close();

		cout << "left:right " << left << ":" << right << endl;
		cout << "Data sum:" << sum << endl;
		cout << "Cache start index:" << startIndex << endl <<endl;

		//记录相对读取结束位置
		int endIndex = startIndex + right - left;

		//填充middle
		if (readMiddle(CACHE_TEMP, startIndex, endIndex)) {
			writeMiddle(middleFile, left);
			return;
		}

		//记录small、large缓存数量
		int rs = 0, rl = 0;

		//记录Input读取位置
		int indexIn = startIndex + MIDDLE.getSize();

		while (true) {
			//读入INPUT缓存
			bool flag = readInput(CACHE_TEMP, indexIn, endIndex);
			//更新下一次读取位置
			indexIn += INPUT.getPosW();

			while (!INPUT.isReadOut()) {
				//读取INPUT
				int temp = INPUT.bufferOutput();

				if (temp <= MIDDLE.getMin()) {//小于等于middle最小值
					SMALL.bufferInput(temp);
					rs++;

				}else if (temp >= MIDDLE.getMax()) {//大于等于middle最大值
					LARGE.bufferInput(temp);
					rl++;

				}else {//处于中间值

					if (SMALL.getPosW() <= LARGE.getPosW()) {//SAMLL缓存区数目较少
						SMALL.bufferInput(MIDDLE.popMin());
						MIDDLE.insert(temp);
						rs++;

					}else {//LARGR缓存区数目少
						LARGE.bufferInput(MIDDLE.popMax());
						MIDDLE.insert(temp);
						rl++;
					}

				}

				//判断SMALL、LARGR是否需要写回
				if (SMALL.isWriteOut()) {
					writerBuffer(CACHE_SMALL, SMALL);
				}
				if (LARGE.isWriteOut()) {
					writerBuffer(CACHE_LARGE, LARGE);
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
		writerBuffer(CACHE_SMALL, SMALL);
		writerBuffer(CACHE_LARGE, LARGE);

		//写middle
		writeMiddle(middleFile, left + rs);

		//small、large文件复制到缓存文件中
		copyFile(CACHE_SMALL, CACHE_TEMP, rs, ios::app);
		copyFile(CACHE_LARGE, CACHE_TEMP, rl, ios::app);

		int dataSum = sum;
		sum += (rs + rl);

		quickSort(left, left + rs, dataSum);
		quickSort(right - rl, right, dataSum + rs);

	}

	//运行函数
	void run() {
		//copy数据到缓存文件
		copyFile(DATA_FILE, CACHE_TEMP, MAX_SUM, ios::out);
		//进行外部排序
		this->quickSort(0, MAX_SUM, 0);
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



int main() {
	//生成测试数据
	//createTestData();

	ExternalQuickSort qs;
	qs.run();

	cout << "Data Size: " << MAX_SUM << endl;
	cout << "I/O times: " << IO_TIME << endl;

	return 0;
}