#include <windows.h>
#include <stdio.h>

#define NUMELEMENT 200

#define NUMTHREAD 2
typedef struct _param_t {
	int NumElement;
	int * pArray;
	int id;
} param_t, *pparam_t;

#define BUFFERSIZE 8
volatile int buffer[BUFFERSIZE];
volatile int in = 0, out = 0, count = 0;
//dekker
volatile bool flag[2] = { false,false };
volatile int turn = 0;
//Bakery
volatile int num[NUMTHREAD];
volatile bool choosing[NUMTHREAD];

void InitNumChoosing()
{
	for (int i = 0; i < NUMTHREAD; i++)
	{
		num[i] = 0;
		choosing[i] = false;
	}
}
int MaxNum()
{
	int maxnum = num[0];

	return maxnum;
}
void bakeryEnterCS(int i)
{
	choosing[i] = true;
	num[i] = MaxNum() + 1;
	choosing[i] = false;
	for (int j = 0; j < NUMTHREAD; j++)
	{
		MemoryBarrier();
		while (choosing[j]);
		MemoryBarrier();
		while ((num[j] != 0) && ((num[j] < num[i]) || ((num[j] == num[i]) && (j < i))));
	}
}
DWORD WINAPI Producer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int *a = p->pArray;
	int id = p->id;
	int i = 0;
	int produced;
	while (i < n)
	{
		MemoryBarrier();
		while (count == BUFFERSIZE);
		bakeryEnterCS(id);
		produced = a[i++];
		buffer[in] = produced;
		count = count + 1;



		in = (in + 1) % (BUFFERSIZE);
		num[id] = 0;

	}
	return 0;

}
DWORD WINAPI Consumer(LPVOID lpParameter)
{
	pparam_t p = (pparam_t)lpParameter;
	int n = p->NumElement;
	int * b = p->pArray;
	int id = p->id;
	int i = 0;
	int consumed;
	while (i < n)
	{
		MemoryBarrier();
		while (count == 0);
		bakeryEnterCS(id);

		consumed = buffer[out];
		count = count - 1;
		out = (out + 1) % (BUFFERSIZE);
		b[i++] = consumed;
		num[id] = 0;


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
		printf("%d ", a[i]);
	}
	printf("\n");
}

int SoSanhMang(int a[], int b[], int n) {
	for (int i = 0; i < n; ++i) {
		if (a[i] != b[i]) {
			return 1;
		}
	}
	return 0;
}


int main()
{
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
	XuatMang(a, n);

	if (SoSanhMang(a, b, n))
		printf("mang a va b giong nhau\n");
	else
		printf("mang a va b khac nhau\n");
	return 0;
}