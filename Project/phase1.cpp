//Warm up

#include"BufferManage.cpp"
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

//�����Сn
const int DATA_VOLUME = 10;

//����������w
const int BUFFER_LINE = 5;

//����A��B��C��Ӧ������
BufferManage<int> MATRIX_A_BUFFER(BUFFER_LINE);
BufferManage<int> MATRIX_B_BUFFER(BUFFER_LINE);
BufferManage<int> MATRIX_C_BUFFER(BUFFER_LINE);

//cache miss����
int CACHE_MISS_A = 0;
int CACHE_MISS_B = 0;

//�����ļ���
string MATRIX_A_FILE, MATRIX_B_FILE, MATRIX_C_FILE;

//����н���
struct Bounds {
	//�к�
	int row;
	//��ʼ�к�
	int colS;
	//�����к�
	int colE;
};

//������ɾ���A��B
void writeMatrix(string filePath){
	ofstream file;
	file.open(filePath, ios::out);
	cout << "Writing matrix into " << filePath << "...\n";
	//д������������
	file << DATA_VOLUME << endl;
	file << DATA_VOLUME << endl;
	//���ɾ���д���ļ�
	for (int i = 0; i < DATA_VOLUME; i++) {
		for (int j = 0; j < DATA_VOLUME; j++) {
			file << rand() % 100 << " ";
		}
		file << endl;
	}
	file.close();
	cout << "Write matrix into " << filePath << " completed.\n";
}

//�����������������д���ļ�
void writeInitMatrix(string filePath, int row, int col) {
	ofstream file;
	file.open(filePath, ios::out);
	file << row << endl;
	file << col << endl;
	file.close();
}
 
//��ȡ��д�ļ���
void initFileName() {
	char temp[1024];
	sprintf_s(temp, sizeof(temp), "matrix_A_%d.txt", DATA_VOLUME);
	MATRIX_A_FILE = temp;
	sprintf_s(temp, sizeof(temp), "matrix_B_%d.txt", DATA_VOLUME);
	MATRIX_B_FILE = temp;
	sprintf_s(temp, sizeof(temp), "matrix_C_%d.txt", DATA_VOLUME);
	MATRIX_C_FILE = temp;
}

//д��buffer
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
	//��ȡǰ���о����С��ʶ
	int x, y;
	file >> x >> y;
	//������ָ����
	int t;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < y; j++) {
			file >> t;
		}
	}
	//������ָ��λ��
	for (int i = 0; i < col_s; i++) {
		file >> t;
	}
	//����Buffer
	for (int i = 0; i < col_e - col_s; i++) {
		file >> t;
		bufferManage.bufferInput(t);
	}
	file.close();
}

//����A��������ȡ
void readBufferA(Bounds bound){
	CACHE_MISS_A++;
	readBuffer(MATRIX_A_FILE, bound, MATRIX_A_BUFFER);
}

//����B��������ȡ
void readBufferB(Bounds bound){
	CACHE_MISS_B++;
	readBuffer(MATRIX_B_FILE, bound, MATRIX_B_BUFFER);
}

//����C������д��
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


//ijk˳��
void ijkOrder() {
	printf("ijk˳����㣺\n");
	//д��C����������
	writeInitMatrix(MATRIX_C_FILE, DATA_VOLUME, DATA_VOLUME);

	for (int i = 0; i < DATA_VOLUME; i++) {
		for (int j = 0; j < DATA_VOLUME; j++) {
			int result = 0;
			for (int k = 0; k < DATA_VOLUME; k++) {
				//��BufferA
				if (MATRIX_A_BUFFER.isReadOut()) {
					Bounds boundA{ i, k, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
					readBufferA(boundA);
				}
				//��BufferB
				Bounds boundB{ k, (j / BUFFER_LINE) * BUFFER_LINE, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
				readBufferB(boundB);
				
				result += MATRIX_A_BUFFER.getIndexData(k % BUFFER_LINE) * MATRIX_B_BUFFER.getIndexData(j % BUFFER_LINE);
			}
			//����������Index(i, j)���
			//printf("Index(%d, %d): %d.\n", i, j, result);
			//дBufferC
			if (MATRIX_C_BUFFER.isWriteOut()) {
				writeBufferC();
			}
			MATRIX_C_BUFFER.bufferInput(result);
		}
		//дBufferC
		writeBufferC(true);
	}
}

//ikj˳��
void ikjOrder() {
	printf("ikj˳����㣺\n");
	//д��C����������
	writeInitMatrix(MATRIX_C_FILE, DATA_VOLUME, DATA_VOLUME);

	int* a = new int[DATA_VOLUME];

	for (int i = 0; i < DATA_VOLUME; i++) {
		
		fill(a, a + DATA_VOLUME, 0);
		for (int k = 0; k < DATA_VOLUME; k++) {
			//��BufferA
			if (MATRIX_A_BUFFER.isReadOut()) {
				Bounds boundA{ i, k, min(MATRIX_A_BUFFER.getCapcity(), DATA_VOLUME - k) };
				readBufferA(boundA);
			}
			for (int j = 0; j < DATA_VOLUME; j++) {
				//��BufferB
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
	
	
	printf("�����С��%d �� %d.\nCache��С��%d.\n\n", DATA_VOLUME, DATA_VOLUME, BUFFER_LINE);
	ijkOrder();
	//ikjOrder();
	cout << "Cache A miss��" << CACHE_MISS_A << endl;
	cout << "Cache B miss��" << CACHE_MISS_B << endl;
	

	return 0;
}*/