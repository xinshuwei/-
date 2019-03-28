#include "ringbuffer.h"
#include<algorithm>
#include<iostream>
#include<thread>
#include <iostream>//多线程编程
#include <stdio.h>    
#include <time.h>      //添加头文件
#include <sys/timeb.h>
#pragma warning(disable:4996)
# define BUFFER_SIZE  4096
# define  length       60*1024


int64_t state = 0;
BYTE* bufdata;


//ofstream filetest1("ringbuff.txt");
//ofstream file("udp.txt");
fstream NetDataReader::filetest;
RingBuffer* NetDataReader::databuffer = new RingBuffer();
NetDataReader::fCallBack    NetDataReader:: udpcallback = NULL;
SOCKET NetDataReader::sock_Client = NULL;

/**
* @brief RingBuffer::RingBuffer
* @param buffersize Byte
*/
RingBuffer::RingBuffer(int size)
{
	if (rbBuf != NULL)
		free(rbBuf);
	rbBuf = (byte*)malloc(size);
	ZeroMemory(rbBuf,size);

	bufferSize = size;
	rbCapacity = size;
	rbBuff = rbBuf;
	rbHead = rbBuff;
	rbTail = rbBuff;
	
	
}

RingBuffer::~RingBuffer()
{
	rbBuff = nullptr;
	rbHead = nullptr;
	rbTail = nullptr;
	rbCapacity = 0;
	delete[]rbBuf; //释放缓冲区
}


/**
* @brief RingBuffer::rbCanRead
* @return 缓冲区可读字节数
*/
int RingBuffer::canRead()
{
	//ring buufer is null, return -1 nullptr 不像NULL会被编译器优化为0
	if ((nullptr == rbBuff) || (nullptr == rbHead) || (nullptr == rbTail))
	{
		return -1;
	}

	if (rbHead == rbTail)
	{
		return 0;
	}

	if (rbHead < rbTail)
	{
		return rbTail - rbHead;
	}
	return rbCapacity - (rbHead - rbTail);
}

/**
* @brief RingBuffer::rbCanWrite  缓冲区剩余可写字节数
* @return  可写字节数
*/
int RingBuffer::canWrite()
{
	if ((nullptr == rbBuff) || (nullptr == rbHead) || (nullptr == rbTail))
	{
		return -1;
	}

	return rbCapacity - canRead();
}

/**
* @brief RingBuffer::read 从缓冲区读数据
* @param 目标数组地址
* @param 读的字节数
* @return
*/
int RingBuffer::read(void *data, int count)
{
	int copySz = 0;

	if ((nullptr == rbBuff) || (nullptr == rbHead) || (nullptr == rbTail))
	{
		return -1;
	}
	if (nullptr == data)
	{
		return -1;
	}

	if (rbHead < rbTail)
	{
		copySz = myMIN(count, canRead());
		memcpy(data, rbHead, copySz);
		rbHead += copySz;
		return copySz;
	}
	else
	{
		if (count < rbCapacity - (rbHead - rbBuff))
		{
			copySz = count;
			memcpy(data, rbHead, copySz);
			rbHead += copySz;
			return copySz;
		}
		else
		{
			copySz = rbCapacity - (rbHead - rbBuff);
			memcpy(data, rbHead, copySz);
			rbHead = rbBuff;
			copySz += read((unsigned char *)data + copySz, count - copySz);
			return copySz;
		}
	}
}

/**
* @brief RingBuffer::write
* @param 数据地址
* @param 要写的字节数
* @return 写入的字节数
*/
int RingBuffer::write(const void *data, int count)
{
	int tailAvailSz = 0;

	if ((nullptr == rbBuff) || (nullptr == rbHead) || (nullptr == rbTail))
	{
		return -1;
	}

	if (nullptr == data)
	{
		return -1;
	}

	if (count >= canWrite())
	{
		return -1;
	}

	if (rbHead <= rbTail)
	{
		tailAvailSz = rbCapacity - (rbTail - rbBuff);
		if (count <= tailAvailSz)
		{
			memcpy(rbTail, data, count);
			rbTail += count;
			if (rbTail == rbBuff + rbCapacity)
			{
				rbTail = rbBuff;
			}
			return count;
		}
		else
		{
			memcpy(rbTail, data, tailAvailSz);
			rbTail = rbBuff;

			return tailAvailSz + write((char*)data + tailAvailSz, count - tailAvailSz);
		}
	}
	else
	{
		memcpy(rbTail, data, count);
		rbTail += count;

		return count;
	}
}

/**
* @brief RingBuffer::size
* @return 缓冲区大小
*/
int RingBuffer::size()
{
	return bufferSize;
}




HRESULT NetDataReader::LoadNetData(void* DataInput, NET_DATARET* RetData, BufferData* Data)
{

	//初始化
	if (RetData == NULL)
	{
		RetData = (NET_DATARET*)malloc(1472);//帧格式大小
		ZeroMemory(RetData, 1472);
	}
	if (Data == NULL)
	{
		Data = (BufferData*)malloc(1320);
		ZeroMemory(Data, 1320);
	}


	//拷贝数据
	try
	{
		memcpy(RetData, DataInput, 1472);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	////异或和校验
	//if (NetDataReader::GetMsgXor((byte *)DataInput, 1408) != RetData->crc)
	//{
	//	printf("校验错啦\n");


	//}
	


	//file.write((char*)RetData, 1472);
	//file.close();



	//帧头 帧尾 长度校验
	if ((RetData->DataDr != (INT16)0xA5A5) && (RetData->DataLen != (INT16)0xBA05) && (RetData->OverLogo != (INT16)0xF5F5))
	{
		return S_FALSE;
	}




	//结构体复制
	try
	{
		memcpy(Data->Data1, RetData->ADCData1, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}

	try
	{
		memcpy(Data->Data2, RetData->ADCData2, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data3, RetData->ADCData3, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data4, RetData->ADCData4, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data5, RetData->ADCData5, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data6, RetData->ADCData6, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data7, RetData->ADCData7, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data8, RetData->ADCData8, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data9, RetData->ADCData9, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data10, RetData->ADCData10, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data11, RetData->ADCData11, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}
	try
	{
		memcpy(Data->Data12, RetData->ADCData12, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}


	try
	{
		memcpy(Data->Data13, RetData->ADCData13, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}



	try
	{
		memcpy(Data->Data14, RetData->ADCData14, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}


	try
	{
		memcpy(Data->Data15, RetData->ADCData15, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;
	}



	try
	{
		memcpy(Data->Data16, RetData->ADCData16, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data17, RetData->ADCData17, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data18, RetData->ADCData18, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data19, RetData->ADCData19, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data20, RetData->ADCData20, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data21, RetData->ADCData21, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}
	try
	{
		memcpy(Data->Data22, RetData->ADCData22, 60);

	}
	catch (const std::exception&)
	{
		return S_FALSE;

	}



	if (databuffer->write(Data, 1320) == -1)
	{
		return S_FALSE;
	}
	//if (state == 1000)
	//{
	//	filetest1.write((char*)Data, 1320);
	//	
	//	filetest1.close();
	//	//printf()

	//}
	return S_OK;
}

NetDataReader::~NetDataReader()
{
	//free()
}
NetDataReader::NetDataReader()
{

}


HRESULT NetDataReader::OpenSocket()
{
	
	SOCKET sock_Client; //客户端用于通信的Socket绑定发送
	WSADATA WSAData;

	char*  sendBuf; //接受数据的缓冲区

	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0) {
		printf("初始化失败!");
		//cout << "socket函数错误:" << WSAGetLastError << endl;
		return 0;
	}

	if (LOBYTE(WSAData.wVersion) != 1 || HIBYTE(WSAData.wVersion) != 1) {
		WSACleanup();
		printf("版本不对!");
		return 0;
	}

	SOCKADDR_IN addr_server;   //服务器的地址数据结构
	SOCKADDR_IN addrClient;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(15533);//下位机端口号
	addr_server.sin_addr.S_un.S_addr = inet_addr("192.168.0.2");   //下位机IP地址
	sock_Client = socket(AF_INET, SOCK_DGRAM, 0);



	addrClient.sin_family = AF_INET;
	addrClient.sin_addr.s_addr = inet_addr("192.168.0.3");
	//client.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrClient.sin_port = htons(15534);
	long  nert = ::bind(sock_Client, (SOCKADDR*)&addrClient, sizeof(addrClient));
	if (SOCKET_ERROR == nert)
	{
		printf("bind failed !/n");
		closesocket(sock_Client);
		WSACleanup();
		return -1;
	}

	if (sock_Client == INVALID_SOCKET)
	{
		printf("socket()failed:%d\n", WSAGetLastError());
		return -1;
	}
	int iLen = sizeof(addr_server);

	sendBuf = "ON";
	int last = sendto(sock_Client, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&addr_server, iLen);
	if (last == SOCKET_ERROR)
	{

		printf("socket()failed:%d\n", WSAGetLastError());
		return -1;
	}
	else if (last == 0)
	{
		return -1;
	}
	else
	{
		printf("sendto(ON)succeeded.\n");
	}
	//_beginthread(NetDataReader::RecvMain((void*)sock_Client), 0, (void*)sock_Client);//多线程
	NetDataReader::RecvMain((void*)sock_Client);
	printf("recvmain 结束\n");
	closesocket(sock_Client);
	return S_OK;

}


HRESULT NetDataReader::CloseSocket()
{
	SOCKET sock_Client1; //客户端用于通信的Socket绑定发送
	WSADATA WSAData1;

	char*  sendBuf1; //接受数据的缓冲区

	if (WSAStartup(MAKEWORD(1, 1), &WSAData1) != 0) {
		printf("初始化失败!");
		//cout << "socket函数错误:" << WSAGetLastError << endl;
		return 0;
	}

	if (LOBYTE(WSAData1.wVersion) != 1 || HIBYTE(WSAData1.wVersion) != 1) {
		WSACleanup();
		printf("版本不对!");
		return 0;
	}

	SOCKADDR_IN addr_server1;   //服务器的地址数据结构
	addr_server1.sin_family = AF_INET;
	addr_server1.sin_port = htons(15533);//下位机端口号
	addr_server1.sin_addr.S_un.S_addr = inet_addr("192.168.0.2");   //下位机IP地址
	sock_Client1 = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_Client1 == INVALID_SOCKET)
	{
		printf("socket()failed:%d\n", WSAGetLastError());
		return -1;
	}
	int iLen = sizeof(addr_server1);
	sendBuf1 = "OFF";
	int last = sendto(sock_Client1, sendBuf1, strlen(sendBuf1), 0, (SOCKADDR*)&addr_server1, iLen);
	if (last == SOCKET_ERROR || last == 0)
	{

		printf("socket(OFF)failed:%d\n", WSAGetLastError());
		return -1;
	}
	else
	{
		printf("sendto(OFF)succeeded.\n");
		return S_OK;
	}

}

void NetDataReader::RecvMain(void * p)//UDP接收线程
{

	//RingBuffer* recvdata = new RingBuffer(368640);
	sockaddr fromAddr;
	int addrLen = sizeof(fromAddr);
	byte* receBuf = (byte *)malloc(length);
	memset(receBuf, 0, length);
	static int cc = 0;
	cc++;
	int64_t getdata = 0;
	SOCKET sock = (SOCKET)p;


	struct timeb tb;
	struct tm *now;
	char sec1[16];
	char mss1[4];
	char sec2[16];
	char mss2[4];
	int min1, s1, ms1, min2, s2, ms2, timespan;



	ftime(&tb);
	now = localtime(&tb.time);
	sprintf(sec1, "%02d-%02d", now->tm_min, now->tm_sec);
	sprintf(mss1, "%03d", tb.millitm);
	min1 = now->tm_min;
	s1 = now->tm_sec;
	ms1 = tb.millitm;
	printf("%s.%s \n", sec1, mss1);



	while (1)
	{

		while (1)

		{
		rungo:
			//QueryPerformanceFrequency(&nFreq);
			//QueryPerformanceCounter(&t1);
			////printf("helloworld2");
			//clock_t start = clock();
			int ret = recvfrom(sock, (char*)receBuf, 1472, 0, &fromAddr, &addrLen);
			if (ret == -1 || ret == 0) {
				//free(receBuf);
				printf("not listen udpdata  \n");
				goto rungo;
			}
			else
			{
				state += 1;
				//printf("%I64d次 \n", state);
				if (state == 1024)
				{
					ftime(&tb);
					now = localtime(&tb.time);
					sprintf(sec2, "%02d-%02d", now->tm_min, now->tm_sec);
					sprintf(mss2, "%03d", tb.millitm);
					printf("%s.%s \n", sec2, mss2);
					min2 = now->tm_min;
					s2 = now->tm_sec;
					ms2 = tb.millitm;
					//int timespan = (min2 * 60 + s2 - min1 * 60 - s1) * 1000 + ms2 - ms1;
					timespan = ((min2 - min1) * 60 + s2 - s1) * 1000 + ms2 - ms1;
					printf("1024次用时%d ms\n", timespan);
					//system("pause");
				}//endif
				break;
			}//endelse
		}//endwhile
		NET_DATARET retdata;
		BufferData bufdata;
		if (NetDataReader::LoadNetData(receBuf, &retdata, &bufdata) == S_OK)
		{
			//printf("UDP存储%d次完成\n", state);
			//if (state % 64 == 0)
			if(databuffer->canRead()>length)
			{
				//BYTE* bufdata = new BYTE[length];
				BYTE* bufdata = (BYTE *)malloc(length);
				databuffer->read(bufdata, length);
				//printf("60K数据前十个........................................................\n");
				//for (int i = 0; i < 10; i++)
				//{
				//	printf("%d    ", (bufdata[i * 2 + 1] * 256 + bufdata[2 * i]) - 32768);
				//}
				//printf("\n60K数据最后十个........................................................\n");
				//for (int i = 30710; i <30720; i++)
				//{
				//	printf("%d    ", (bufdata[i * 2 + 1] * 256 + bufdata[2 * i]) - 32768);
				//}
				//
				//printf("\n剩余数据........................................................\n");
				//int num = databuffer->canRead();
				//BYTE* list = (BYTE *)malloc(num);
				//databuffer->read(list, num);
				//for (int i = 0; i < num/2; i++)
				//{
				//	printf(" %d   " , (list[i*2+1]*256+list[2*i])-32768);
				//	if ((i + 1) % 10 == 0)
				//	{
				//		printf("\n");
				//	}

				//}
				//databuffer->write(list, num);
				udpcallback((PVOID)bufdata, (DWORD)length);
				//free(list);
				free(bufdata);
				//delete[] bufdata;
			}

			goto rungo;

		}
		else{
			printf("帧格式错误或复制错误\n");
			goto rungo;

		}
		

	}//endwhile1
	//return;
}
void NetDataReader::setUdpCallBack(fCallBack Newudpcallback)
{
	udpcallback = Newudpcallback;
	//return S_OK;
	return;
}


byte NetDataReader::GetMsgXor(byte* message, int32_t size)
{
	int i;
	byte check;
	check = 0;
	for (i = 0; i < size; i++)
	{
		check ^= message[i+4];
	}
	return check;
}


