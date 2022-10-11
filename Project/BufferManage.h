template<typename T>
class BufferManage
{
private:
	//缓冲区
	T* buffer;
	//缓冲区容量
	int capcity;
	//读指针指向
	int posR;
	//写指针指向
	int posW;
	
public:
	//默认构造函数
	BufferManage();

	//构造函数1 
	BufferManage(int n);

	//构造函数2
	BufferManage(T data[], int n);

	~BufferManage() {
		delete[]buffer;
	}

	//初始化空间
	void initBuffer(int n);

	//缓冲区重置->读写指针复原
	void reset();

	//缓冲区重置->内容重新填充
	void reset(T data[], int n);

	//写入缓冲区
	bool bufferInput(T data);

	//读出缓冲区
	T bufferOutput();

	//判断是否读完
	bool isReadOut();

	//判断缓冲区是否写满
	bool isWriteOut();

	//获取特定索引位置数据
	T getIndexData(int index);

	//修改特定索引位置数据
	bool setIndexData(T data, int index);

	//得到缓冲区容量
	int getCapcity();

	//得到写入位置
	int getPosW();

	//得到读入位置
	int getPosR();

	//返回缓存区
	T* getBuffer();

	//释放缓冲区
	void free();

};