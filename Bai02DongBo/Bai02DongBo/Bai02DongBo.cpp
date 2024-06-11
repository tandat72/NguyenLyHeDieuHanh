#include <Windows.h>
#include <stdio.h>
#define NUMTHREAD 5
#define NUMSEM 3
HANDLE hSem[NUMSEM];
volatile int x1 = 11, x2 = 12, x3 = 13, x4 = 14, x5 = 15, t, v, y, z, ans;
DWORD WINAPI TinhV(LPVOID lpParam)
{
	v = x2 * x3;
	ReleaseSemaphore(hSem[0], 2, NULL);
	return 0;
}
DWORD WINAPI TinhY(LPVOID lpParam)
{
	WaitForSingleObject(hSem[0], INFINITE);
	y = x1 + v;
	ReleaseSemaphore(hSem[2], 1, NULL);
	return 0;
}
DWORD WINAPI TinhZ(LPVOID lpParam)
{
	z = x4 * x4;
	ReleaseSemaphore(hSem[1], 1, NULL);
	return 0;
}
DWORD WINAPI TinhT(LPVOID lpParam)
{
	WaitForSingleObject(hSem[0], INFINITE); WaitForSingleObject(hSem[1], INFINITE);
	t = v + z;
	ReleaseSemaphore(hSem[2], 1, NULL);
	return 0;
}
DWORD WINAPI TinhAns(LPVOID lpParam)
{
	WaitForSingleObject(hSem[2], INFINITE); WaitForSingleObject(hSem[2], INFINITE);
	ans = y *x4 + x1 * z + v * t;
	return 0;
}
void main() {
	HANDLE h[NUMTHREAD];
	DWORD dwID[NUMTHREAD];
	LPTHREAD_START_ROUTINE pfuct[] = { TinhV ,TinhZ, TinhT, TinhY, TinhAns };
	int i;
	for (i = 0; i < NUMSEM; i++)
		hSem[i] = CreateSemaphore(NULL, 0, 2, NULL);
	for (i = 0; i < NUMTHREAD; i++)
		h[i] = CreateThread(NULL, 0, pfuct[i], NULL, 0, &dwID[i]);
	WaitForMultipleObjects(NUMTHREAD, h, TRUE, INFINITE);
	printf("ans = %d\n", ans);
}