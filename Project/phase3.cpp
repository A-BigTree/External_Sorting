//External Sort : Merge Sort

#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include <algorithm>
#include"BufferManage.cpp"

using namespace std;

/*----------------------全局变量--------------------*/

//数据最大容量
const int MAX_DATA_NUM = 500;

//数据长度
const int DATA_LENGTH = 8;

//INPUT缓存区容量
const int INPUT_BUFFER = 20;
//Output缓存区容量
const int OUTPUT_BUFFER = 40;

//缓存区初始化
BufferManage<int> INPUT0(INPUT_BUFFER);
BufferManage<int> INPUT1(INPUT_BUFFER);
BufferManage<int> OUTPUT(OUTPUT_BUFFER);

//文件定义
const string DATA = "data//phase3//data.txt";
const string RESULT = "data//phase3//result.txt";
const string CACHE = "data//phase3//temp.txt";
const string CACHE_MIDDLE = "data//phase3//middle.txt";

//记录I/O次数
int TIME_IO = 0;

/*------------------------实现函数----------------------*/

//生成测试数据
void createData() {
	ofstream file;
	file.open(DATA, ios::out);

	if (!file.fail()) {
		for (int i = 0; i < MAX_DATA_NUM; i++) {
			file << rand() << endl;
		}
	}
	file.close();
}

//整数转为固定长度字符串
string int2String(int num) {
	string str = "";
	for (int i = 0; i < DATA_LENGTH; i++) {
		str.append("0");
	}

	string intS = to_string(num);
	for (int i = 0; i < intS.length(); i++) {
		str[DATA_LENGTH - intS.length() + i] = intS[i];
	}
	return str;
}

//指定位置读Input
bool readInput(BufferManage<int>& buffer, int index, int right) {
	TIME_IO++;

	//清空buffer
	buffer.reset();

	ifstream file;
	file.open(CACHE, ios::in);

	int i = index;

	char temp[DATA_LENGTH + 1];

	while (!buffer.isWriteOut() && i < right) {
		file.seekg((i++) * (DATA_LENGTH + 1), ios::beg);
		file.get(temp, DATA_LENGTH + 1);
		buffer.bufferInput(stoi(temp));
	}

	file.close();

	return i >= right;
}

//指定位置写output
void writeOutput(fstream& file, int index) {
	TIME_IO++;

	int i = index;

	while (!OUTPUT.isReadOut()) {
		file.seekp((i++) * (DATA_LENGTH + 1), ios::beg);
		file.write(int2String(OUTPUT.bufferOutput()).append(" ").c_str(), DATA_LENGTH + 1);
	}

	OUTPUT.reset();
}

//结果复制到Cache
void copyCache(fstream& file) {
	TIME_IO++;

	ofstream ofile;

	ofile.open(CACHE, ios::out);

	char temp[DATA_LENGTH + 1];
	for (int i = 0; i < MAX_DATA_NUM; i++) {
		file.seekg(i * (DATA_LENGTH + 1), ios::beg);
		file.get(temp, DATA_LENGTH + 1);
		ofile.seekp(i * (DATA_LENGTH + 1), ios::beg);
		ofile << int2String(stoi(temp)) << " ";
	}
	ofile.close();
}

//数据写入Cache
void dataToCache(fstream& file) {
	TIME_IO++;

	ifstream ifile;
	ofstream ofile;

	ifile.open(DATA, ios::in);
	ofile.open(CACHE, ios::out);

	if (ifile.fail() || ofile.fail()) {
		return;
	}

	int temp;
	for (int i = 0; i < MAX_DATA_NUM; i++) {
		ifile >> temp;
		ofile << int2String(temp) << " ";
		file << int2String(temp) << " ";
	}

	ifile.close();
	ofile.close();
}

//缓存写入结果文件
void cacheToResult() {
	TIME_IO++;

	ifstream ifile;
	ofstream ofile;

	ifile.open(CACHE_MIDDLE, ios::in);
	ofile.open(RESULT, ios::out);

	if (ifile.fail() || ofile.fail()) {
		return;
	}

	char temp[DATA_LENGTH + 1];
	for (int i = 0; i < MAX_DATA_NUM; i++) {
		ifile.seekg(i * (DATA_LENGTH + 1), ios::beg);

		ifile.get(temp, DATA_LENGTH + 1);

		ofile << stoi(temp) << endl;
	}

	ifile.close();
	ofile.close();
}

/*------------------解决方法------------------*/

class ExternalMergeSort {
private:
	//中间文件流
	fstream file;

public:
	ExternalMergeSort() {
		file.open(CACHE_MIDDLE, ios::out | ios::in);
	}

	//外部Merge排序
	void mergeSort(int left, int right) {
		//可在你内存中进行排序
		if (right + 1 - left <= OUTPUT_BUFFER) {

			readInput(OUTPUT, left, right + 1);
			sort(OUTPUT.getBuffer(), OUTPUT.getBuffer() + OUTPUT.getPosW());
			writeOutput(file, left);
			//copy缓存文件
			copyCache(file);
			return;
		}

		int mid = (right + left) / 2;

		mergeSort(left, mid);

		mergeSort(mid + 1, right);

		//cout << "Run->left: mid: right -> " << left << ": " << mid + 1 << ": " << right + 1 << endl << endl;

		//写指针
		int current_w = left;
		//两个输入读指针
		int current_r0 = left, current_r1 = mid + 1;
		//判断输入是否读完
		bool flag0 = false, flag1 = false;

		int i = 0, j = 0;

		while (true) {
			//读Input
			if (INPUT0.isReadOut()) {
				flag0 = readInput(INPUT0, current_r0, mid + 1);
				current_r0 += INPUT0.getPosW();
				i = 0;
			}
			if (INPUT1.isReadOut()) {
				flag1 = readInput(INPUT1, current_r1, right + 1);
				current_r1 += INPUT1.getPosW();
				j = 0;
			}
			//归并过程
			while (!INPUT0.isReadOut() && !INPUT1.isReadOut()) {
				int d0 = INPUT0.getIndexData(i, false);
				int d1 = INPUT1.getIndexData(j, false);

				//找到较小的数
				int data;
				if (d0 <= d1) {//INPUT0小
					data = INPUT0.bufferOutput();
					i++;
				}
				else {//INPUT1小
					data = INPUT1.bufferOutput();
					j++;
				}

				//放入OUTPUT缓存
				OUTPUT.bufferInput(data);
				//判断写OUTPUT
				if (OUTPUT.isWriteOut()) {
					int size = OUTPUT.getPosW();
					writeOutput(file, current_w);
					current_w += size;
				}
			}

			//跳出循环
			if ((flag0 && INPUT0.isReadOut()) || (flag1 && INPUT1.isReadOut())) {
				break;
			}
		}

		//处理INPUT0剩余元素
		if (!flag0 || !INPUT0.isReadOut()) {
			while (true) {
				if (INPUT0.isReadOut()) {
					flag0 = readInput(INPUT0, current_r0, mid + 1);
					current_r0 += INPUT0.getPosW();
				}

				while (!INPUT0.isReadOut()) {
					OUTPUT.bufferInput(INPUT0.bufferOutput());

					if (OUTPUT.isWriteOut()) {
						int size = OUTPUT.getPosW();
						writeOutput(file, current_w);
						current_w += size;
					}
				}

				if (flag0) {
					break;
				}
			}
		}

		//处理INPUT1剩余元素
		if (!flag1 || !INPUT1.isReadOut()) {
			while (true) {
				if (INPUT1.isReadOut()) {
					flag1 = readInput(INPUT1, current_r1, right + 1);
					current_r1 += INPUT1.getPosW();
				}

				while (!INPUT1.isReadOut()) {
					OUTPUT.bufferInput(INPUT1.bufferOutput());

					if (OUTPUT.isWriteOut()) {
						int size = OUTPUT.getPosW();
						writeOutput(file, current_w);
						current_w += size;
					}
				}

				if (flag1) {
					break;
				}
			}
		}

		//处理OUTPUT剩余元素
		writeOutput(file, current_w);

		//copy缓存文件
		copyCache(file);

		return;
	}

	//运行方法
	void run() {
		//data→cache
		dataToCache(file);
		mergeSort(0, MAX_DATA_NUM - 1);
		//关闭中间文件
		file.close();
		//middle→result
		cacheToResult();
		//清空缓存文件
		file.open(CACHE, ios::out);
		file.close();
		file.open(CACHE_MIDDLE, ios::out);
		file.close();
	}
};

/*-----------------------------主函数------------------------*/

int main() {
	//生成数据
	createData();

	clock_t start = clock();

	ExternalMergeSort ms;
	ms.run();

	clock_t end = clock();

	double time = (double)(end - start) / CLOCKS_PER_SEC;

	printf("Data size: %d.\n", MAX_DATA_NUM);
	printf("Cost time: %.4fs.\n", time);
	printf("I/O times: %d.\n", TIME_IO);

	return 0;
}