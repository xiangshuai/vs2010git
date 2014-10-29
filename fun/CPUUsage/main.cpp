/**
*	CPU占用率正弦曲线
*	参考：http://noalgo.info/444.html
*/
#include <windows.h>
#include <math.h>

const double PI = acos(-1.0);	// PI
const int COUNT = 200;			// 每周期采样点数目
const int INTERVAL = 100;		// CPU占用率刷新时间（毫秒）

void core()
{
	DWORD busySpan[COUNT];
	DWORD idleSpan[COUNT];

	for (int i=0; i<COUNT; i++)
	{
		busySpan[i] = (DWORD)(((1 + sin(2 * PI / COUNT * i)) * 0.5 * INTERVAL));
		idleSpan[i] = INTERVAL - busySpan[i];
	}

	for (int i = 0; ; i = (i + 1) % COUNT)
	{
		DWORD startTime = GetTickCount();
		while((GetTickCount() - startTime) <= busySpan[i]); //忙碌busySpan[i]
		Sleep(idleSpan[i]);									//空闲idleSpan[i]
	}
}

int main()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	int CPUNum = info.dwNumberOfProcessors; 
	HANDLE *handle = new HANDLE[CPUNum];  //线程句柄
	DWORD  *threadID = new DWORD[CPUNum]; //线程ID

	for (int i = 0; i < CPUNum; i++)
	{
		if ((handle[i] = CreateThread(NULL, 0, 
			(LPTHREAD_START_ROUTINE)core, 0, 0, threadID+i)) != NULL)
			SetThreadAffinityMask(handle[i], 0x01 << i); //指定CPU
	}
	WaitForSingleObject(handle[0], INFINITE);
}

