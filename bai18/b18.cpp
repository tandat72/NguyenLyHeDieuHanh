#include "RWDisk.h"
#include <stdio.h>

#define SECTORSIZE 512
int FindEntry(ENTRY e[], int nr, char*filename)
{
	for (int i = 0; i < nr; i++)
		if ((e[i].FileName[0] != 0xE5) && (e[i].FileName[0] != 0) && (e[i].Attrib != 0x0F) && (strncmp((char*)(e[i].FileName), filename, 11) == 0))
			return i;
	return -1;
}
void ShowBuffer(char* buffer, int size)
{
	for (int i = 0; i < size; i++)
	{
		putchar(buffer[i]);
	}

}
BOOL DocHienThiTapTin(int drive, char*filename)
{
	ENTRY e[ENTRY__NUMBER_OF_ELEMENT];
	if (!ReadENTRY(e, drive))
		printf("ko the doc entryo dia %c\n", drive + 'A'), exit(0);
	int index = FindEntry(e, ENTRY__NUMBER_OF_ELEMENT, filename);
	if (index<0)
		printf("ko tim thay tap tin %c\n", filename), exit(0);
	int fc = e[index].FirstCluster;
	int fs = e[index].FileSize;

	WORD FAT[FAT__NUMBER_OF_ELEMENT];
	if (!ReadFAT(FAT, drive))
		printf("ko the doc FAT o dia chi %c\n", drive + 'A'), exit(0);
	char buffer[SECTORSIZE];
	CRWDisk disk;
	int i = fc;
	int size;
	while (i != 0XFFF)
	{
		int lsector = i + 31;
		if (!disk.AbsRead(drive, 1, lsector, (void*)buffer))
			printf("ko the doc o dia chi %c\n", drive + 'A'), exit(0);
		size = min(SECTORSIZE, fs);
		ShowBuffer(buffer, size);
		fs -= size;
		i = FAT[i];
	}
	return TRUE;
}
void main()
{
	int drine = 0;
	char*filename = "ABCD1234TXT";
	DocHienThiTapTin(drine, filename);
}