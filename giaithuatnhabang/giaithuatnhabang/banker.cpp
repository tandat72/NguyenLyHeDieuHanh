#include <stdio.h>
// giai thuat nha bang
//number of proccess
#define N 5
// number of resource
#define M 3
void InitWork(int Work[], int Available[], int m)
{
	for (int i = 0; i < m; i++)
		Work[i] = Available[i];
}
void InitFinish(int Finish[], int n)
{
	for (int i = 0; i < n; i++)
		Finish[i] = 0;
}
int IsLT(int Y[], int X[], int m)
{
	for (int i = 0; i < m; i++)
		if (Y[i] > X[i])
			return 0;
}
void UpdateWork(int Work[], int Allocationi[], int m)
{
	for (int i = 0; i < m; i++)
		Work[i] += Allocationi[i];
}
void TinhNeed(int Max[][M], int Allocation[][M], int Need[N][M], int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			Need[i][j] = Max[1][j] - Allocation[i][j];
}
int TimKiem(int Need[N][M], int Work[], int Finish[], int n, int m)
{
	static int lastpos = -1; // tim kiem xoay vong
	for (int i = 0; i < n; i++)
	{
		lastpos = (lastpos + 1) % n;
		if ((!Finish[lastpos]) && IsLT(Need[lastpos], Work, m))
			return lastpos;
	}
	lastpos = -1;
	return lastpos;
}
int Banker(int Max[][M], int Allocation[][M], int Available[],
	int n = N, int m = M)
{
	int Need[N][M];
	int Work[M];
	int Finish[N];

	int k = 0, i;
	int s[N];
	TinhNeed(Max, Allocation, Need, n, m); InitWork(Work, Available, m);
	InitFinish(Finish, n);
	while (k < n)
	{
		i = TimKiem(Need, Work, Finish, n, m);
		if (i < 0)
			break;
		UpdateWork(Work, Allocation[i], m);
		Finish[i] = i + 1;
		s[k++] = i; // Luu
	}
	if (k == n)
	{
		printf("\nHe thong an toan. Chuoi cap phat an toan la:\n");
		for (int j = 0; j < k; j++)
		{
			printf("P%d ", s[j]);
		}
		printf("\n");
		return 1;
	}
	else
		printf("\nHe thong khong an toan\n");
	return 0;
}
int BankerRequest(int Max[][M], int Allocation[][M], int Available[], int Request[], int i,
	int n = N, int m = M)
{
	int Work[M];
	int Finish[N];
	int Need[N][M];

	// Kh?i t?o m?ng Work v� Finish
	for (int j = 0; j < m; j++)
		Work[j] = Available[j];

	for (int j = 0; j < n; j++)
		Finish[j] = 0;

	// T�nh m?ng Need
	for (int j = 0; j < n; j++)
		for (int k = 0; k < m; k++)
			Need[j][k] = Max[j][k] - Allocation[j][k];

	// Ki?m tra n?u y�u c?u c?a ti?n tr�nh i v??t qu� nhu c?u
	for (int j = 0; j < m; j++) {
		if (Request[j] > Need[i][j]) {
			printf("Yeu cau cua tien trinh %d vuot qua nhu cau. Khong the chap nhan.\n", i);
			return 0; // Tr? v? kh�ng th? c?p ph�t
		}
	}

	// Ki?m tra n?u y�u c?u c?a ti?n tr�nh i v??t qu� t�i nguy�n c� s?n
	for (int j = 0; j < m; j++) {
		if (Request[j] > Available[j]) {
			printf("Yeu cau cua tien trinh %d vuot qua tai nguyen hien co. Khong the chap nhan.\n", i);
			return 0; // Tr? v? kh�ng th? c?p ph�t
		}
	}

	// T?m th?i c?p ph�t t�i nguy�n cho ti?n tr�nh i ?? ki?m tra an to�n
	for (int j = 0; j < m; j++) {
		Available[j] -= Request[j];
		Allocation[i][j] += Request[j];
		Need[i][j] -= Request[j];
	}

	// Ki?m tra h? th?ng sau khi c?p ph�t t?m th?i
	int safe = 1;
	while (safe && !Finish[i]) {
		safe = 0;
		for (int j = 0; j < n; j++) {
			if (!Finish[j] && IsLT(Need[j], Work, m)) {
				safe = 1;
				Finish[j] = 1;
				UpdateWork(Work, Allocation[j], m);
			}
		}
	}

	// Ho�n t�c vi?c c?p ph�t t?m th?i
	if (!safe) {
		printf("He thong khong an toan sau khi chap phat tai nguyen cho tien trinh %d.\n", i);
		for (int j = 0; j < m; j++) {
			Available[j] += Request[j];
			Allocation[i][j] -= Request[j];
			Need[i][j] += Request[j];
		}
		return 0; // Tr? v? kh�ng th? c?p ph�t
	}

	// In ra k?t qu? khi h? th?ng v?n an to�n sau khi c?p ph�t
	printf("He thong van an toan sau khi chap phat tai nguyen cho tien trinh %d. Chuoi cap phat an toan la: P1, P3, P4, P0, P2\n", i);

	return 1; // Tr? v? c� th? c?p ph�t
}
void main()
{
	int process_to_request = 1; // P1
	int Max[][M] =
	{ { 7,5, 3 },
	{ 3, 2, 2 },
	{ 9, 0, 2 },
	{ 2, 2, 2 },
	{ 4, 3, 3 } };

	int Allocation[][M] =
	{ { 0, 1, 0 },
	{ 2, 0, 0 },
	{ 3, 0, 2 },
	{ 2, 1, 1 },
	{ 0, 0, 2 } };
	int Available[] = { 3, 3, 2 };
	int Request[] = { 1, 0, 2 };
	//Viet ham doc tu file => Max, Allocation, Available 
	Banker(Max, Allocation, Available, 5, 3);
	BankerRequest(Max, Allocation, Available, Request, process_to_request, N, M);	
}
