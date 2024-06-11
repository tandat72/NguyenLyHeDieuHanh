#include <stdio.h>
#include <windows.h>

#define NUMELEMENT 20
#define NUMTHREAD 2

#define BUFFERSIZE 8
volatile int buffer[BUFFERSIZE];
volatile int in = 0, out = 0, count = 0;
volatile bool flag[2] = { false, false };
volatile int turn = 0;

typedef struct _param_t
{
	int NumElement;
	int * pArray;
} param_t, *pparam_t;

DWORD WINAPI Producer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int * a = p->pArray;
	int i = 0;
	int produced;
	while (i < n)
	{
		while (count == BUFFERSIZE);

		flag[0] = true;
		MemoryBarrier();
		while (flag[1])
		{
			if (turn != 0)
			{
				flag[0] = false;
				MemoryBarrier();
				while (turn != 0);
				flag[0] = true;
			}
		}
		produced = a[i++];
		buffer[in] = produced;
		count = count + 1;
		in = (in + 1) % (BUFFERSIZE);
		flag[0] = false;
		turn = 1;
	}
	return 0;
}

DWORD WINAPI Consumer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int * b = p->pArray;
	int i = 0;
	int consumed;
	while (i < n)
	{
		while (count == 0);
		flag[1] = true;
		while (flag[0])
		{
			if (turn != 1)
			{
				flag[1] = false;
				while (turn != 1);
				flag[1] = true;
			}
		}
		consumed = buffer[out];
		count = count - 1;
		out = (out + 1) % (BUFFERSIZE);
		b[i++] = consumed;
		flag[1] = false;
		turn = 0;
	}
	return 0;
}

void KhoiTaoMang(int a[], int n) {
	for (int i = 0; i < n; ++i) {
		a[i] = i + 1; // Kh?i t?o m?ng v?i các giá tr? t? 1 ??n 200
	}
}


void XuatMang(int a[], int n) {
	printf("Mang: ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", a[i]); // In ra m?ng
	}
	printf("\n");
}

int SoSanhMang(int a[], int b[], int n) {
	for (int i = 0; i < n; ++i) {
		if (a[i] != b[i]) {
			return 0; // M?ng không gi?ng nhau
		}
	}
	return 1; // M?ng gi?ng nhau
}

void main() {
	int a[NUMELEMENT], b[NUMELEMENT];
	int n = sizeof(a) / sizeof(a[0]);
	HANDLE h[NUMTHREAD];
	DWORD dwID[NUMTHREAD];
	pparam_t p[NUMTHREAD];
	KhoiTaoMang(a, n);
	XuatMang(a, n);

	p[0] = (pparam_t)malloc(sizeof(param_t));
	p[0]->NumElement = n;
	p[0]->pArray = a;

	p[1] = (pparam_t)malloc(sizeof(param_t));
	p[1]->NumElement = n;
	p[1]->pArray = b;

	h[0] = CreateThread(NULL, 0, Producer, p[0], 0, &dwID[0]);
	h[1] = CreateThread(NULL, 0, Consumer, p[1], 0, &dwID[0]);
	WaitForMultipleObjects(NUMTHREAD, h, TRUE, INFINITE);
	XuatMang(b, n);
	// So sanh a va b
	if (SoSanhMang(a, b, n))
		printf("Mang a va b giong nhau\n");
	else
		printf("Mang a va b khac nhau \n");

	
}
