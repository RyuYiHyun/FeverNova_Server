#pragma once
#pragma warning(disable:4996)
#pragma comment(lib, "libmysql.lib")

//#pragma comment(lib, "ws2_32")
//#include <winsock2.h>
#include <iostream>
#include <list>
#include <ctime>
#include <tchar.h>
#include "Util.h"
#include "LockGuard.h"
#include "MyStream.h"
using namespace std;
#define SERVERPORT	9000		
#define BUFSIZE		4096
#define INFOSIZE	128
#define MAXPACKETNUM 210000000



