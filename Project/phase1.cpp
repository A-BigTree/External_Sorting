//Warm up

#include"BufferManage.cpp"
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

//矩阵大小n
const int DATA_VOLUME = 10;

//缓冲区容量w
const int BUFFER_LINE = 5;

//矩阵A、B、C对应缓冲区
BufferManage<int> MATRIX_A_BUFFER(BUFFER_LINE);
BufferManage<int> MATRIX_B_BUFFER(BUFFER_LINE);
BufferManage<int> MATRIX_C_BUFFER(BUFFER_LINE);

//cache miss次数
int CACHE_MISS_A = 0;
int CACHE_MISS_B = 0;

//矩阵文件名
string MATRIX_A_FILE, MATRIX_B_FILE, MATRIX_C_FILE;

//标记行界限
struct Bounds {
	//行号
	int row;
	//起始列号
	int colS;
	//结束列号
	int colE;
};

//随机生成矩阵A，B
void writeMatrix(string filePath){
	ofstream file;
	file.open(filePath, ios::out);
	cout << "Writing matrix into " << filePath << "...\n";
	//写入行数，列数
	file << DATA_VOLUME << endl;
	file << DATA_VOLUME << endl;
	//生成矩阵并写入文件
	for (int i = 0; i < DATA_VOLUME; i++) {
		for (int j = 0; j < DATA_VOLUME; j++) {
			file << rand() % 100 << " ";
		}
		file << endl;
	}
	file.close();
	cout << "Write matrix into " << filePath << " completed.\n";
}

//将矩阵的行数与列数写入文件
void writeInitMatrix(string filePath, int row, int col) {
	ofstream file;
	file.open(filePath, ios::out);
	file << row << endl;
	file << col << endl;
	file.close();
}
 
//获取读写文件名
void initFileName() {
	char temp[1024];
	sprintf_s(temp, sizeof(temp), "matrix_A_%d.txt", DATA_VOLUME);
	MATRIX_A_FILE = temp;
	sprintf_s(temp, sizeof(temp), "matrix_B_%d.txt", DATA_VOLUME);
	MATRIX_B_FILE = temp;
	sprintf_s(temp, sizeof(temp), "matrix_C_%d.txt", DATA_VOLUME);
	MATRIX_C_FILE = temp;
}

//写入buffer
void readBuffer(string filePath, Bounds bound, BufferManage<int>& bufferManage) {
	bufferManage.reset();
	int row = bound.row;
	int col_s = bound.colS;
	int col_e = bound.colE;
	ifstream file;
	file.open(filePath, ios::in);
	if (file.fail()) {
		printf("File: %s opened filed.", filePath.c_str());
		return;
	}
	string temp;
	//读取前两行矩阵大小标识
	int x, y;
	file >> x >> y;
	//索引到指定行
	int t;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < y; j++) {
			file >> t;
		}
	}
	//索引到指定位置
	for (int i = 0; i < col_s; i++) {
		file >> t;
	}
	//读入Buffer
	for (int i = 0; i < col_e - col_s; i++) {
		file >> t;
		bufferManage.bufferInput(t);
	}
	file.close();
}

//矩阵A缓冲区读取
void readBufferA(Bounds bound){
	CACHE_MISS_A++;
	readBuffer(MATRIX_A_FILE, bound, MATRIX_A_BUFFER);
}

//矩阵B缓冲区读取
void readBufferB(Bounds bound){
	CACHE_MISS_B++;
	readBuffer(MATRIX_B_FILE, bound, MATRIX_B_BUFFER);
}

//矩阵C缓冲区写入
void writeBufferC(bool is_next_line = false){
	ofstream file;
	file.open(MATRIX_C_FILE, ios::app);
	if (file.fail()) {
		printf("File: %s opened filed.", MATRIX_C_FILE.c_str());
		return;
	}
	while (!MATRIX_C_BUFFER.isReadOut()) {
		file << MATRIX_C_BUFFER.bufferOutput() << " ";
	}
	if (is_next_line) {
		file << endl;
	}
	file.close();
	MATRIX_C_BUFFER.reset();
}


//ijk顺序
void ijkOrder() {
	printf("ijk顺序计算：\n");
	//写入C矩阵行列数
	writeInitMatrix(MATRIX_C_FILE, DATA_VOLUME, DATA_VOLUME);

	for (int i = 0; i < DATA_VOLUME; i++) {
		for (int j = 0; j < DATA_VOLUME; j++) {
			int result = 0;
			for (int k = 0; k < DATA_VOLUME; k++) {
				//读BufferA
				if (MATRIX_A_BUFFER.isReadOut()) {
					Bounds boundA{ i, k, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
					readBufferA(boundA);
				}
				//读BufferB
				Bounds boundB{ k, (j / BUFFER_LINE) * BUFFER_LINE, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
				readBufferB(boundB);
				
				result += MATRIX_A_BUFFER.getIndexData(k % BUFFER_LINE) * MATRIX_B_BUFFER.getIndexData(j % BUFFER_LINE);
			}
			//计算结果矩阵Index(i, j)结果
			//printf("Index(%d, %d): %d.\n", i, j, result);
			//写BufferC
			if (MATRIX_C_BUFFER.isWriteOut()) {
				writeBufferC();
			}
			MATRIX_C_BUFFER.bufferInput(result);
		}
		//写BufferC
		writeBufferC(true);
	}
}

//ikj顺序
void ikjOrder() {
	printf("ikj顺序计算：\n");
	//写入C矩阵行列数
	writeInitMatrix(MATRIX_C_FILE, DATA_VOLUME, DATA_VOLUME);

	int* a = new int[DATA_VOLUME];

	for (int i = 0; i < DATA_VOLUME; i++) {
		
		fill(a, a + DATA_VOLUME, 0);
		for (int k = 0; k < DATA_VOLUME; k++) {
			//读BufferA
			if (MATRIX_A_BUFFER.isReadOut()) {
				Bounds boundA{ i, k, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
				readBufferA(boundA);
			}
			for (int j = 0; j < DATA_VOLUME; j++) {
				//读BufferB
				if (MATRIX_B_BUFFER.isReadOut()) {
					Bounds boundB{ k, j, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - j) };
					readBufferB(boundB);
				}
				a[j] += MATRIX_A_BUFFER.getIndexData(k % BUFFER_LINE) * MATRIX_B_BUFFER.getIndexData(j % BUFFER_LINE);
			}
		}
		int m = 0;
		for (int r = 0; r <= DATA_VOLUME / BUFFER_LINE; r++) {
			while (!MATRIX_C_BUFFER.isWriteOut()) {
				MATRIX_C_BUFFER.bufferInput(a[m++]);
			}
			writeBufferC();
		}
		for (m; m < DATA_VOLUME; m++) {
			MATRIX_C_BUFFER.bufferInput(a[m]);
		}
		writeBufferC(true);
	}
	delete[]a;
}

/*
int main() {
	//writeMatrix("matrix_A_10.txt");
	//writeMatrix("matrix_B_10.txt");
	
	
	initFileName();
	
	
	printf("矩阵大小：%d × %d.\nCache大小：%d.\n\n", DATA_VOLUME, DATA_VOLUME, BUFFER_LINE);
	ijkOrder();
	//ikjOrder();
	cout << "Cache A miss：" << CACHE_MISS_A << endl;
	cout << "Cache B miss：" << CACHE_MISS_B << endl;
	

	return 0;
}*/