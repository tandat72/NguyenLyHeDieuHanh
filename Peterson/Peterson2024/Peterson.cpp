#include <stdio.h>
#include <windows.h>

#define NUMELEMENT 200
#define NUMTHREAD 2

#define BUFFERSIZE 8
volatile int buffer[BUFFERSIZE];
volatile int in = 0, out = 0, count = 0;
//Dekker
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
		MemoryBarrier();
		while (count == BUFFERSIZE);

		turn = 1;
		flag[0] = true;
		MemoryBarrier();
		while (flag[1] && (turn == 1));
		
		produced = a[i++];
		buffer[in] = produced;
		count = count + 1;
		in = (in + 1) % (BUFFERSIZE);
		flag[0] = false;
		
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
		MemoryBarrier();
		while (count == 0);

		turn = 0;
		flag[1] = true;
		MemoryBarrier();
		while (flag[0] && (turn == 0));
		
		consumed = buffer[out];
		count = count - 1;
		out = (out + 1) % (BUFFERSIZE);
		b[i++] = consumed;
		flag[1] = false;
		
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
			return 0; // Tr? v? 0 n?u có ít nh?t m?t ph?n t? khác nhau
		}
	}
	return 1; // Tr? v? 1 n?u m?ng gi?ng nhau
}

int main() {
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
	h[1] = CreateThread(NULL, 0, Consumer, p[1], 0, &dwID[1]);
	WaitForMultipleObjects(NUMTHREAD, h, TRUE, INFINITE);
	XuatMang(b, n);
	// So sanh a va b
	if (SoSanhMang(a, b, n))
		printf("Mang a va b giong nhau\n");
	else
		printf("Mang a va b khac nhau \n");

	return 0;
}
