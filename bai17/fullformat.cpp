#include "RWDisk.h"
#include <stdio.h>

#define SECTORSIZE 512
int tinhSf(int Sv, int Sc, int nr, int Sb = 1, int nf = 2)
{
	//int Sf = 9;
	int Sf = (Sv - 1 - 2 * Sb - (nr * 32) / SECTORSIZE) / (Sc + 2 * nf);
	return Sf;
}
BOOL QuickFormat(int drive, int nr, int Sc)
{
	//doc boot sector => Sc,nr,Sc
	//tinh Sf
	BOOTSECTOR bs;
	CRWDisk disk;
	if (!disk.AbsRead(drive, 1, 0, (void*)&bs))
		printf("ko the doc o dia %c\n", drive + 'A'), exit(0);
	int Sv = bs.Sum_Sector;
	if (!Sv)
		Sv = bs.SumSector;

	int Sf = tinhSf(Sv, Sc, nr);

	bs.RootEntry = nr;
	bs.SectorPerCluster = Sc;
	bs.SectorPerFat = Sf;
	if (!disk.AbsWrite(drive, 1, 0, (void*)&bs))
		printf("ko the ghi bootsector o dia %c\n", drive + 'A'), exit(0);
	char * FATbuffer = (char*)malloc(Sf * 512);
	memset(FATbuffer, 0, Sf * 512);
	FATbuffer[0] = (char)0xF0;
	FATbuffer[1] = (char)0xFF;
	FATbuffer[2] = (char)0xFF;
	if (!disk.AbsWrite(drive, Sf, 1, (void*)FATbuffer))
		printf("ko the ghi FAT o dia %c\n", drive + 'A'), exit(0);
	if(!disk.AbsWrite(drive, Sf, 1+Sf, (void*)FATbuffer))
		printf("ko the ghi FAT o dia %c\n",drive+'A'), exit(0);
	ENTRY *e = (ENTRY*)malloc(nr * 32);
	memset(e, 0, nr * 32);
	int Sr = (nr * 32) / 512;
	if(!disk.AbsWrite(drive, Sr, 1+2* Sf, (void*)e))
		printf("ko the ghi RDET o dia %c\n", drive + 'A'), exit(0);
	return TRUE;
}
BOOL FullFormat(int drive, int nr, int Sc)
{
	BOOTSECTOR bs;
	CRWDisk disk;
	if (!disk.AbsRead(drive, 1, 0, (void*)&bs))
		printf("ko the doc o dia %c\n", drive + 'A'), exit(0);
	int Sv = bs.Sum_Sector;
	if (!Sv)
		Sv = bs.SumSector;

	int Sf = tinhSf(Sv, Sc, nr);

	bs.RootEntry = nr;
	bs.SectorPerCluster = Sc;
	bs.SectorPerFat = Sf;
	if (!disk.AbsWrite(drive, 1, 0, (void*)&bs))
		printf("ko the ghi bootsector o dia %c\n", drive + 'A'), exit(0);
	char * FATbuffer = (char*)malloc(Sf * 512);
	memset(FATbuffer, 0, Sf * 512);
	FATbuffer[0] = (char)0xF0;
	FATbuffer[1] = (char)0xFF;
	FATbuffer[2] = (char)0xFF;
	if (!disk.AbsWrite(drive, Sf, 1, (void*)FATbuffer))
		printf("ko the ghi FAT o dia %c\n", drive + 'A'), exit(0);
	if (!disk.AbsWrite(drive, Sf, 1 + Sf, (void*)FATbuffer))
		printf("ko the ghi FAT o dia %c\n", drive + 'A'), exit(0);
	ENTRY *e = (ENTRY*)malloc(nr * 32);
	memset(e, 0, nr * 32);
	int Sr = (nr * 32) / 512;
	if (!disk.AbsWrite(drive, Sr, 1 + 2 * Sf, (void*)e))
		printf("ko the ghi RDET o dia %c\n", drive + 'A'), exit(0);
	//xoa vung data
	char* buffer[SECTORSIZE];
	memset(buffer, 0, SECTORSIZE);
	for (int i = 1 + 2 * Sf + Sr; i < Sv; i++)
		if (!disk.AbsWrite(drive, 1, i, (void*)buffer))
			printf("ko the ghi data o dia %c\n", drive + 'A'), exit(0);
	return TRUE;
}
void main()
{
	int drive = 0;
	int Sc = 1;
	int nr = 224;
	QuickFormat(drive, nr, Sc);
	FullFormat(drive, nr, Sc);
}

