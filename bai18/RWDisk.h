#pragma once
#include <windows.h>
#include <winioctl.h>
#define READ 0
#define WRITE 1

#define FAT__NUMBER_OF_BYTE      (9*512)
#define FAT__NUMBER_OF_ELEMENT   (9*512 *2/3)
// vi moi phan tu chiem 1.5 byte

#define ENTRY__NUMBER_OF_ELEMENT 224
#define __EOF 0xFF8
#define __SECTOR_SIZE 512
#define __DATA_OFFSET 31

 //33(1 + 9 + 9 + 14) - 2(availble Fat index)


/////////////////////////////////////////
//Cau truc cua bootsector

#pragma pack (1)

struct __BootSector
{
   BYTE  Ma[3];
   BYTE  Version[8];
   WORD  BytePerSector;
   BYTE  SectorPerCluster;
   WORD  ReservedSector;
   BYTE  FatNum;
   WORD  RootEntry;
   WORD  Sum_Sector;
   BYTE  MediaDescripttor;
   WORD  SectorPerFat;
   WORD  SectorPerTrack;
   WORD  HeadPerDisk;
   DWORD HiddenSector;
   DWORD SumSector;
   BYTE  PhysicDisk;
   BYTE  Danhrieng;
   BYTE  KiHieuNhanDien;
   BYTE  SerialNumber[4];
   BYTE  VolumeLabel[11];
   BYTE  FATID[8];
   BYTE  Boot[448];
   BYTE  Rest[2];   
};
typedef __BootSector BOOTSECTOR;

#pragma pack ()

////////////////////////////////////////////////////////////////////////
//Cau truc cua Entry

#pragma pack (1)

struct __EntryRDET
{
   BYTE FileName[8];
   BYTE FileExt[3];
   BYTE Attrib;
   BYTE Reserved[10];
   WORD Time;
   WORD Date;
   WORD FirstCluster;
   ULONG FileSize;
};
typedef struct __EntryRDET ENTRY;

#pragma pack ()

class CRWDisk
{
public:
   CRWDisk(void);
   ~CRWDisk(void);
public:
   //Phuong thuc lay thong tin ve o dia 
   BOOL GetDriveGeometry(int m_Drive, DISK_GEOMETRY *pdg);
   //Phuong thuc doc 1 day cac sector
   BOOL AbsRead (int m_Drive, int nSectors, long lSectors, void *Buffer);
   //Phuong thuc doc 1 byte cua 1 sector
   BOOL AbsRead (int m_Drive, int lSectors, int nOffset, char &ch);
   //Phuong thuc doc 1 day byte cua 1 sector
   BOOL AbsRead (int m_Drive, int lSectors, int nOffset, int nBytesRead, char *Buffer);
    //Phuong thuc ghi 1 day cac sector
   BOOL AbsWrite (int m_Drive, int nSectors, long lSectors, void *Buffer);
   BOOL AbsWrite (int m_Drive, int lSectors, int nOffset, char ch);
   //Phuong thuc ghi 1 day byte cua 1 sector
   BOOL AbsWrite (int m_Drive, int lSectors, int nOffset, int nBytesWrite, char *Buffer);
   //Phuong thuc biosdisk
   BOOL BiosDisk (int cmd, int m_Drive, int head, int track, int sector, int nSectors, void *Buffer);

};

bool ReadFAT(WORD FAT[FAT__NUMBER_OF_ELEMENT], int m_Drive);
bool WriteFAT(WORD FAT[FAT__NUMBER_OF_ELEMENT], int m_Drive);
bool ReadENTRY(ENTRY Entry[ENTRY__NUMBER_OF_ELEMENT], int m_Drive);
bool WriteENTRY(ENTRY Entry[ENTRY__NUMBER_OF_ELEMENT], int m_Drive);


