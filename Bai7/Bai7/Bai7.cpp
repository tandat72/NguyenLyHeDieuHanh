#include <stdio.h>
#include <windows.h>

int x1 = 11, x2 = 12, x3 = 13, x4 = 14, x5 = 15, t, v, y, z, ans;
HANDLE semaphore;

DWORD WINAPI CalculateV(LPVOID lpParam) {
	WaitForSingleObject(semaphore, INFINITE);
	v = x2 * x3;
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}

DWORD WINAPI CalculateY(LPVOID lpParam) {
	WaitForSingleObject(semaphore, INFINITE);
	y = x1 + v;
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}

DWORD WINAPI CalculateZ(LPVOID lpParam) {
	WaitForSingleObject(semaphore, INFINITE);
	z = x4 * v - y;
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}

DWORD WINAPI CalculateT(LPVOID lpParam) {
	WaitForSingleObject(semaphore, INFINITE);
	t = v - x5;
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}

DWORD WINAPI CalculateAns(LPVOID lpParam) {
	WaitForSingleObject(semaphore, INFINITE);
	ans = y * x4 + x1 * z + v * t;
	printf("ans = %d\n", ans);
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}

int main() {
	HANDLE hThread[5];
	DWORD dwThreadIdArray[5];
	LPDWORD lpThreadIdArray = NULL;

	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	hThread[0] = CreateThread(NULL, 0, CalculateV, NULL, 0, &dwThreadIdArray[0]);
	hThread[1] = CreateThread(NULL, 0, CalculateY, NULL, 0, &dwThreadIdArray[1]);
	hThread[2] = CreateThread(NULL, 0, CalculateZ, NULL, 0, &dwThreadIdArray[2]);
	hThread[3] = CreateThread(NULL, 0, CalculateT, NULL, 0, &dwThreadIdArray[3]);
	hThread[4] = CreateThread(NULL, 0, CalculateAns, NULL, 0, &dwThreadIdArray[4]);

	WaitForMultipleObjects(5, hThread, TRUE, INFINITE);

	CloseHandle(semaphore);
	for (int i = 0; i < 5; ++i) {
		CloseHandle(hThread[i]);
	}

	return 0;
}
