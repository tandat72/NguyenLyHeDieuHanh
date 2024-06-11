#include <Windows.h>
#include <stdio.h>

#define NUMELEMENT 20
#define NUMTHEAD 2
#define BUFFERSIZE 8
volatile int buffer[BUFFERSIZE];
volatile int in = 0, out = 0, count = 0;
HANDLE hSemaphoreEmpty, hSemaphoreFull, hMutex;

typedef struct _param_t
{
	int NumElement;
	int* pArray;
} param_t, *pparam_t;

DWORD WINAPI Producer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int* a = p->pArray;
	int i = 0;
	int produced;
	while (i < n)
	{
		produced = a[i++];
		WaitForSingleObject(hSemaphoreEmpty, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);

		buffer[in] = produced;
		in = (in + 1) % BUFFERSIZE;
		count++;

		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphoreFull, 1, NULL);
	}
	return 0;
}

DWORD WINAPI Consumer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int* b = p->pArray;
	int i = 0;
	int consumed;
	while (i < n)
	{
		WaitForSingleObject(hSemaphoreFull, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);

		consumed = buffer[out];
		out = (out + 1) % BUFFERSIZE;
		count--;

		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphoreEmpty, 1, NULL);

		b[i++] = consumed;
	}
	return 0;
}

void KhoiTaoMang(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % 100; // Random initialization
	}
}

void XuatMang(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

int SoSanhMang(int a[], int b[], int n)
{
	for (int i = 0; i < n; i++)
	{
		if (a[i] != b[i])
		{
			return 0; // Not equal
		}
	}
	return 1; // Equal
}

int main()
{
	int a[NUMELEMENT], b[NUMELEMENT];
	int n = NUMELEMENT;
	HANDLE h[NUMTHEAD];
	DWORD dwID[NUMTHEAD];
	pparam_t p[NUMTHEAD];

	hSemaphoreEmpty = CreateSemaphore(NULL, BUFFERSIZE, BUFFERSIZE, NULL);
	hSemaphoreFull = CreateSemaphore(NULL, 0, BUFFERSIZE, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);

	KhoiTaoMang(a, n);

	p[0] = (pparam_t)malloc(sizeof(param_t));
	p[0]->NumElement = n;
	p[0]->pArray = a;
	p[1] = (pparam_t)malloc(sizeof(param_t));
	p[1]->NumElement = n;
	p[1]->pArray = b;

	h[0] = CreateThread(NULL, 0, Producer, p[0], 0, &dwID[0]);
	h[1] = CreateThread(NULL, 0, Consumer, p[1], 0, &dwID[1]);

	WaitForMultipleObjects(NUMTHEAD, h, TRUE, INFINITE);

	XuatMang(a, n);
	XuatMang(b, n);

	if (SoSanhMang(a, b, n))
	{
		printf("Mang a va b giong nhau\n");
	}
	else
	{
		printf("Mang a va b khac nhau\n");
	}

	CloseHandle(hSemaphoreEmpty);
	CloseHandle(hSemaphoreFull);
	CloseHandle(hMutex);

	return 0;
}