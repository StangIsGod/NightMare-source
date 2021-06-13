#pragma once
#include <netex/net.h>
#include <cell/sysmodule.h>
#include <cell/http.h>
#include "in.h"
#include <sys/timer.h>
#include "webFunctions.h"

extern "C" /*liblv2 functions*/ {
	void* _sys_malloc(size_t);
	void _sys_free(void*);
	int _sys_sprintf(char*, const char*, ...);
	int _sys_vsprintf(char*, const char*, va_list);
	int _sys_printf(const char*, ...);
}
//#define RECV_BUFFER_SIZE 

//toggle Debug Printf by __DEBUG
#ifdef __DEBUG 

#define printf _sys_printf
#else
#define printf
#define SERVER_PORT htons(80)

#endif

int Socket;
struct hostent *Host;
struct sockaddr_in SocketAddress;

int CONNECTIONERROR[16] = { 188, 176, 177, 177, 186, 188, 171, 182, 176, 177, 223, 186, 173, 173, 176, 173 };
int GET[5] = { 184, 186, 171, 223, 208 };
int HTTPHOST[17] = { 223, 183, 171, 171, 175, 208, 206, 209, 207, 242,245, 183, 176, 172, 171, 197, 223 };
int DOUBLEBREAK[4] = { 242, 245, 242, 245 };
char* XOR(int* Input, int Length)
{
	char buffer[1024] = { 0 };
	memFree(buffer, Length + 2);
	for (int i = 0; i < Length; i++)
	{
		buffer[i] = (char)(Input[i] ^ 0xFF);
	}
	return buffer;
}
int* XOR(char* Input, int Length)
{
	int buffer[1024];
	for (int i = 0; i < Length; i++)
	{
		buffer[i] = (int)Input[i] ^ 0xFF;
	}
	return buffer;
}
int Numbersss(float number)
{
	float acc = 0;
	for (float i = number; i > 0; i--)
		acc += i;
	return (int)acc;
}

int getstrLen(const char* str) { int i = 0; for (i = 0; i < 0x600; i++) { if (*(str + i) == 0x00) break; }return i; }
char* SocketRequest(char* URL, char* Path = "")
{
	char bufferReturn[10000];
	char RequestBuffer[2000];
	Host = gethostbyname(URL);
	SocketAddress.sin_addr.s_addr = *((unsigned long*)Host->h_addr);
	SocketAddress.sin_family = AF_INET;
	int Val = Numbersss(35.053);
	SocketAddress.sin_port = SERVER_PORT;
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	int* te = XOR("pot", 3);
	if (connect(Socket, (struct sockaddr *)&SocketAddress, sizeof(SocketAddress)) != 0) {
		return XOR(CONNECTIONERROR, 16);
	}
	strcpy(RequestBuffer, XOR(GET, 5));
	if (strlen(Path) > 0) {
		strcat(RequestBuffer, Path);
	}
	strcat(RequestBuffer, XOR(HTTPHOST, 17));
	strcat(RequestBuffer, URL);
	strcat(RequestBuffer, XOR(DOUBLEBREAK, 4));
	send(Socket, RequestBuffer, strlen(RequestBuffer), 0);
	while (recv(Socket, bufferReturn, 10000, 0) > 0)
	{
		return bufferReturn;
		sys_timer_usleep(1000);
	}
	socketclose(Socket);
}
namespace network
{

#define RECV_BUFFER_SIZE 4000
	//int RECV_BUFFER_SIZE = 0;
	int sendRequest_main(const char*);
	int sendRequest(const char* url)
	{
		int ret, ret2;

		ret = cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP);
		if (ret != CELL_SYSMODULE_LOADED) {
			printf("cellSysmoduleLoadModule() failed (0x%x)\n");
			return ret;
		}

		ret = sys_net_initialize_network();
		if (ret != CELL_OK) {
			printf("sys_net_initialize_network() failed (0x%x)\n", ret);
			return ret;
		}

		ret2 = sendRequest_main(url);

		ret = sys_net_finalize_network();
		if (ret != CELL_OK) {
			printf("sys_net_finalize_network() failed (0x%x)\n", ret);
			return ret;
		}

		ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTP);
		if (ret != CELL_OK) {
			printf("cellSysmoduleUnloadModule() failed (0x%x)\n", ret);
			return ret;
		}

		return ret2;
	}
	/*RECV_BUFFER_SIZE 256 phpÇÃÉfÅ[É^ÇéÛÇØéÊÇÈó */
//#define RECV_BUFFER_SIZE 256
#define HTTP_POOL_SIZE 8192
	
	char responseBuf[RECV_BUFFER_SIZE];
	int sendRequest_main(const char* url)
	{
		
		int ret;
		void* httpPool, *uriPool;
		CellHttpClientId clientId;
		CellHttpUri uri;
		CellHttpTransId transId;
		size_t poolSize, localRecv;
		uint64_t length, recvd;
		bool has_cl = true;

		httpPool = _sys_malloc(HTTP_POOL_SIZE);
		if (httpPool == NULL) {
			printf("failed to malloc http pool\n");
			ret = -1;
			goto end;
		}

		ret = cellHttpInit(httpPool, HTTP_POOL_SIZE);
		if (ret != CELL_OK) {
			printf("cellHttpInit() failed (0x%x)\n", ret);
			goto end;
		}

		ret = cellHttpCreateClient(&clientId);

		if (ret != CELL_OK) {
			printf("cellHttpCreateClient() failed (0x%x)\n", ret);
			goto end;
		}

		ret = cellHttpUtilParseUri(NULL, url, NULL, 0, &poolSize);
		if (ret != CELL_OK) {
			printf("cellHttpUtilParseUri() failed (0x%x)\n", ret);
			goto end;
		}

		uriPool = _sys_malloc(poolSize);
		if (uriPool == NULL) {
			printf("failed to malloc uri pool\n");
			goto end;
		}

		ret = cellHttpUtilParseUri(&uri, url, uriPool, poolSize, NULL);
		if (ret != CELL_OK) {
			_sys_free(uriPool);
			uriPool = NULL;
			printf("cellHttpUtilParseUri() failed (0x%x)\n", ret);
			goto end;
		}

		ret = cellHttpCreateTransaction(&transId, clientId, CELL_HTTP_METHOD_GET, &uri);
		if (ret != CELL_OK) {
			printf("cellHttpCreateTransaction() failed (0x%x)\n", ret);
			goto end;
		}

		ret = cellHttpSendRequest(transId, NULL, 0, NULL);
		if (ret != CELL_OK) {
			printf("cellHttpSendRequest() failed (0x%x)\n", ret);
			goto end;
		}

		int code;
		ret = cellHttpResponseGetStatusCode(transId, &code);
		if (ret != CELL_OK) {
			printf("cellHttpResponseGetStatusCode() failed (0x%x)\n", ret);
			goto end;
		}
		printf("Status code is %d\n", code);

		ret = cellHttpResponseGetContentLength(transId, &length);
		if (ret != CELL_OK) {
			if (ret == CELL_HTTP_ERROR_NO_CONTENT_LENGTH) {
				has_cl = false;
			}
			else {
				printf("cellHttpResponseGetContentLength() failed (0x%x)\n", ret);
				goto end;
			}
		}

		recvd = 0;
		while (!has_cl || recvd < length) {
			ret = cellHttpRecvResponse(transId, responseBuf, RECV_BUFFER_SIZE - 1, &localRecv);
			if (ret != CELL_OK) {
				printf("cellHttpRecvResponse() failed (0x%x)\n", ret);
				goto end;
			}
			else if (localRecv == 0) break;
			recvd += localRecv;
			responseBuf[localRecv] = '\0';
		}

		printf(responseBuf);
		printf("\n");

		ret = 0;

	end:
		if (transId) {
			cellHttpDestroyTransaction(transId);
			transId = 0;
		}
		if (clientId) {
			cellHttpDestroyClient(clientId);
			clientId = 0;
		}
		cellHttpEnd();
		_sys_free(httpPool);

		return ret;
	}

	char* getResponse(void)
	{
		return responseBuf;
	}
}