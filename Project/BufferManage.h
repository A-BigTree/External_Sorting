template<typename T>
class BufferManage
{
private:
	//������
	T* buffer;
	//����������
	int capcity;
	//��ָ��ָ��
	int posR;
	//дָ��ָ��
	int posW;
	
public:
	//Ĭ�Ϲ��캯��
	BufferManage();

	//���캯��1 
	BufferManage(int n);

	//���캯��2
	BufferManage(T data[], int n);

	~BufferManage() {
		delete[]buffer;
	}

	//��ʼ���ռ�
	void initBuffer(int n);

	//����������->��дָ�븴ԭ
	void reset();

	//����������->�����������
	void reset(T data[], int n);

	//д�뻺����
	bool bufferInput(T data);

	//����������
	T bufferOutput();

	//�ж��Ƿ����
	bool isReadOut();

	//�жϻ������Ƿ�д��
	bool isWriteOut();

	//��ȡ�ض�����λ������
	T getIndexData(int index);

	//�޸��ض�����λ������
	bool setIndexData(T data, int index);

	//�õ�����������
	int getCapcity();

	//�õ�д��λ��
	int getPosW();

	//�õ�����λ��
	int getPosR();

	//���ػ�����
	T* getBuffer();

	//�ͷŻ�����
	void free();

};