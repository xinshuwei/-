
#include "stdafx.h"
#include <limits> 
#ifndef RB_MAX_LEN
#define RB_MAX_LEN 2*3600*1024
#endif // !RB_MAX_LEN

#define myMIN(a,b) (a)<(b)?(a):(b)//求最小


using std::string;
using std::wstring;
using namespace std;

class RingBuffer
{
public:
	RingBuffer(int size = RB_MAX_LEN);
	~RingBuffer();
	int canRead();//how much can read 
	int canWrite();//how much can write
	int read(void *data, int count);  //read data frome ringbuffer
	int write(const void *data, int count);
	int size();
private:
	int bufferSize;       //buffer size
	unsigned char *rbBuf = new unsigned char[bufferSize];
	/*环形缓冲区变量*/
	int rbCapacity; //容量
	unsigned char  *rbHead;//buffer帧头  用于写数据
	unsigned char  *rbTail;//buffer帧尾 用于读数据
	unsigned char  *rbBuff;

};

struct NET_DATARET
{
	INT16 DataDr;//帧头标志
	INT16 DataLen;//数据长度
	byte ADCData1[60];
	byte ADCData_b1[4];

	byte ADCData2[60];
	byte ADCData_b2[4];

	byte ADCData3[60];
	byte ADCData_b3[4];

	byte ADCData4[60];
	byte ADCData_b4[4];

	byte ADCData5[60];
	byte ADCData_b5[4];

	byte ADCData6[60];
	byte ADCData_b6[4];

	byte ADCData7[60];
	byte ADCData_b7[4];


	byte ADCData8[60];
	byte ADCData_b8[4];

	byte ADCData9[60];
	byte ADCData_b9[4];

	byte ADCData10[60];
	byte ADCData_b10[4];

	byte ADCData11[60];
	byte ADCData_b11[4];


	byte ADCData12[60];
	byte ADCData_b12[4];


	byte ADCData13[60];
	byte ADCData_b13[4];


	byte ADCData14[60];
	byte ADCData_b14[4];


	byte ADCData15[60];
	byte ADCData_b15[4];


	byte ADCData16[60];
	byte ADCData_b16[4];

	byte ADCData17[60];
	byte ADCData_b17[4];

	byte ADCData18[60];
	byte ADCData_b18[4];

	byte ADCData19[60];
	byte ADCData_b19[4];

	byte ADCData20[60];
	byte ADCData_b20[4];

	byte ADCData21[60];
	byte ADCData_b21[4];

	byte ADCData22[60];
	byte ADCData_b22[4];

	//byte ADCData[1408];//ADC数据
	byte timedata[57];//预留及时间戳
	byte crc;
	INT16 OverLogo;//结束标志
};


struct BufferData
{
	byte Data1[60];
	byte Data2[60];
	byte Data3[60];
	byte Data4[60];
	byte Data5[60];
	byte Data6[60];
	byte Data7[60];
	byte Data8[60];
	byte Data9[60];
	byte Data10[60];
	byte Data11[60];
	byte Data12[60];
	byte Data13[60];
	byte Data14[60];
	byte Data15[60];
	byte Data16[60];
	byte Data17[60];
	byte Data18[60];
	byte Data19[60];
	byte Data20[60];
	byte Data21[60];
	byte Data22[60];
};





class NetDataReader
{
public:
	 NetDataReader();
	~NetDataReader();
	static HRESULT LoadNetData(void* DataInput, NET_DATARET*RetData, BufferData* Data);
	HRESULT OpenSocket();
	HRESULT CloseSocket();
	void RecvMain(void *p);
	typedef void (*fCallBack)(PVOID, DWORD);
	// 接收回调函数地址并保存
	void setUdpCallBack(fCallBack udpcallback);
	static byte GetMsgXor(byte* message, int32_t size);
	//std::thread NetDataReader::arrayDataThread();
	
private:
	static fstream filetest;
	
	static SOCKET sock_Client;
	static SOCKET sock_Client1;
	
	static SOCKADDR_IN addr_server;
	static SOCKADDR_IN addr_server1;
	static fCallBack udpcallback;
	static RingBuffer* databuffer;

	//static void 

};








