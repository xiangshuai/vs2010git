/**
*	CPUռ������������
*	�ο���http://noalgo.info/444.html
*/
#include <windows.h>
#include <math.h>

const double PI = acos(-1.0);	// PI
const int COUNT = 200;			// ÿ���ڲ�������Ŀ
const int INTERVAL = 100;		// CPUռ����ˢ��ʱ�䣨���룩

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
		while((GetTickCount() - startTime) <= busySpan[i]); //æµbusySpan[i]
		Sleep(idleSpan[i]);									//����idleSpan[i]
	}
}

int main()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	int CPUNum = info.dwNumberOfProcessors; 
	HANDLE *handle = new HANDLE[CPUNum];  //�߳̾��
	DWORD  *threadID = new DWORD[CPUNum]; //�߳�ID

	for (int i = 0; i < CPUNum; i++)
	{
		if ((handle[i] = CreateThread(NULL, 0, 
			(LPTHREAD_START_ROUTINE)core, 0, 0, threadID+i)) != NULL)
			SetThreadAffinityMask(handle[i], 0x01 << i); //ָ��CPU
	}
	WaitForSingleObject(handle[0], INFINITE);
}

