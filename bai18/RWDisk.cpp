#include "rwdisk.h"

/////////////////////////////////////////////////////////////////////
//Phuong thuc khoi tao mac dinh
/////////////////////////////////////////////////////////////////////

CRWDisk::CRWDisk(void)
{

}


/////////////////////////////////////////////////////////////////////
//Phuong thuc huy
/////////////////////////////////////////////////////////////////////

CRWDisk::~CRWDisk(void)
{
   

}

//////////////////////////////////////////////////////////////////////
//Phuong thuc lay thong tin ve o dia
///////////////////////////////////////////////////////////////////////////////////////

BOOL CRWDisk::GetDriveGeometry(int m_Drive, DISK_GEOMETRY *pdg)
{
   HANDLE hDevice;              
   BOOL bResult;                 
   DWORD junk;                  
   char m_DeviceName[]="\\\\.\\A:";
   m_DeviceName[4]+=m_Drive;
   hDevice = CreateFile(m_DeviceName, 
      0,                
      FILE_SHARE_READ | FILE_SHARE_WRITE, 
      NULL,            
      OPEN_EXISTING,   
      0,               
      NULL);         
   if (hDevice == INVALID_HANDLE_VALUE)
   {
      return (FALSE);
   }

   bResult = DeviceIoControl(hDevice, 
      IOCTL_DISK_GET_DRIVE_GEOMETRY,  
      NULL, 0,
      pdg, sizeof(*pdg),    
      &junk,               
      (LPOVERLAPPED) NULL);  
   CloseHandle(hDevice);
   return (bResult);
}

//////////////////////////////////////////////////////////////////
//Phuong thuc mo phong absread
/////////////////////////////////////////////////////////////////////////////////

BOOL CRWDisk::AbsRead(int m_Drive, int nSectors, long lSectors, void *Buffer)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      //LONG m_Seek;
      DWORD m_ByteRead;
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4]+=m_Drive;
      m_HandelDevice = CreateFile(m_DeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {         
            SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
            if (!ReadFile (m_HandelDevice, Buffer, pdg.BytesPerSector*nSectors, &m_ByteRead, NULL) )
            {
               CloseHandle(m_HandelDevice);
               return FALSE;         
            }
      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;
}


/////////////////////////////////////////////////////////////////
//Phuong thuc doc 1 byte cua 1 sector
//////////////////////////////////////////////////////////////////

BOOL CRWDisk::AbsRead (int m_Drive, int lSectors, int nOffset, char &ch)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4]+=m_Drive;
      m_HandelDevice = CreateFile(m_DeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {
//            LONG m_Seek;
            DWORD m_ByteRead;
            char *Buffer;
            Buffer=new char[pdg.BytesPerSector];
            SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
            if (!ReadFile (m_HandelDevice, Buffer, pdg.BytesPerSector, &m_ByteRead, NULL) )
            {
                    CloseHandle(m_HandelDevice);
               return FALSE;         
            }
            ch=Buffer[nOffset];
            delete []Buffer;

      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;

}

BOOL CRWDisk::AbsRead (int m_Drive, int lSectors, int nOffset, int nBytesRead, char *Buffer)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4] += m_Drive;
      m_HandelDevice = CreateFile(m_DeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {         
//            LONG m_Seek;
            DWORD m_ByteRead;
            char *BufferTemp;
            BufferTemp=new char[pdg.BytesPerSector];
            SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
            if (!ReadFile (m_HandelDevice, BufferTemp, pdg.BytesPerSector, &m_ByteRead, NULL) )
            {
                    CloseHandle(m_HandelDevice);
               return FALSE;         
            }
            int i;
            for (i=0; i<nBytesRead; i++)
               Buffer[i]= BufferTemp[i+nOffset];
            delete []BufferTemp;
      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;   

}
////////////////////////////////////////////////////////////////////
//Phuong thuc ghi 1 day cac sector
////////////////////////////////////////////////////////////////////

BOOL CRWDisk::AbsWrite (int m_Drive, int nSectors, long lSectors, void *Buffer)
{

   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4]+=m_Drive;

      //m_HandelDevice = CreateFile(m_DeviceName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 

	  m_HandelDevice = CreateFile(m_DeviceName
		, GENERIC_WRITE
		, 0
		, NULL
		, OPEN_EXISTING
		, FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING
		, NULL);
 

      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {      
            DWORD m_ByteWrite;
            SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
            if (!WriteFile (m_HandelDevice, Buffer, pdg.BytesPerSector*nSectors, &m_ByteWrite, NULL))
            {
               CloseHandle(m_HandelDevice);
               return FALSE;         
            }
      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;
}

/////////////////////////////////////////////////////////////////
//Phuong thuc ghi 1 byte cua 1 sector
//////////////////////////////////////////////////////////////////

BOOL CRWDisk::AbsWrite (int m_Drive, int lSectors, int nOffset, char ch)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4]+=m_Drive;
      m_HandelDevice = CreateFile(m_DeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {         
//         LONG m_Seek;
         DWORD m_ByteRead;
         char *Buffer;
         Buffer=new char[pdg.BytesPerSector];
         SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
         if (!ReadFile (m_HandelDevice, Buffer, pdg.BytesPerSector, &m_ByteRead, NULL) )
         {
            CloseHandle(m_HandelDevice);
            return FALSE;         
         }
         Buffer[nOffset]=ch;
         SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
         if (!WriteFile (m_HandelDevice, Buffer, pdg.BytesPerSector, &m_ByteRead, NULL) )
         {
            CloseHandle(m_HandelDevice);
            return FALSE;         
         }
         delete []Buffer;

      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//Phuong thuc ghi 1 day byte cua 1 sector
///////////////////////////////////////////////////////////////////////

BOOL CRWDisk::AbsWrite(int m_Drive, int lSectors, int nOffset, int nBytesWrite, char *Buffer)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      HANDLE m_HandelDevice;
      char m_DeviceName[]="\\\\.\\A:";
      m_DeviceName[4]+=m_Drive;
      m_HandelDevice = CreateFile(m_DeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
      if (m_HandelDevice == INVALID_HANDLE_VALUE) 
         return FALSE;
      else
      {         
//         LONG m_Seek;
         DWORD m_ByteRead;
         char *BufferTemp;
         BufferTemp=new char[pdg.BytesPerSector];
         SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
         if (!ReadFile (m_HandelDevice, BufferTemp, pdg.BytesPerSector, &m_ByteRead, NULL) )
         {
            CloseHandle(m_HandelDevice);
            return FALSE;         
         }
         int i;
         for (i=0; i<nBytesWrite; i++)
            BufferTemp[i+nOffset]=Buffer[i];
         SetFilePointer (m_HandelDevice, (lSectors*pdg.BytesPerSector), NULL, FILE_BEGIN); 
         if (!WriteFile (m_HandelDevice, BufferTemp, pdg.BytesPerSector, &m_ByteRead, NULL) )
         {
            CloseHandle(m_HandelDevice);
            return FALSE;         
         }
         delete []BufferTemp;
      }   
      CloseHandle (m_HandelDevice);
      return TRUE;
   }
   else
      return FALSE;   

}

//////////////////////////////////////////////////////////////////
//Phuong thuc biosdisk
////////////////////////////////////////////////////////////////////

BOOL CRWDisk::BiosDisk (int cmd, int m_Drive, int head, int track, int sector, int nSectors, void *Buffer)
{
   DISK_GEOMETRY pdg;           
   BOOL bResult;    
   bResult = this->GetDriveGeometry (m_Drive, &pdg);
   if (bResult)
   {
      ULONG lSectors;
      lSectors= head*pdg.SectorsPerTrack+track*pdg.SectorsPerTrack*pdg.TracksPerCylinder+sector-1;
      if (cmd==READ)
         return this->AbsRead (m_Drive, nSectors, lSectors, Buffer);
      else if (cmd==WRITE)
         return this->AbsWrite (m_Drive, nSectors, lSectors, Buffer);
      else
         return FALSE;
   }
   else
      return FALSE;    

}



//int m_Drive = 0 <=> O A, 1 <=> O B, lay ra bang FAT nam trong FAT////////////////////////////////
bool ReadFAT(WORD FAT[FAT__NUMBER_OF_ELEMENT], int m_Drive){   
   CRWDisk disk; // Khai bao bien mang kieu CRWDisk
   unsigned char FatBuff[FAT__NUMBER_OF_BYTE];
   if(!disk.AbsRead ( m_Drive, 9, 1, FatBuff))
      return false;   
   for (int k = 0; k < FAT__NUMBER_OF_ELEMENT; ++k)
   {
      int i = (k*3)/2;
      int value16 = 0x100 * FatBuff[i+1] + FatBuff[i];
      if (k%2 != 0)
         FAT[k] = (value16 >> 4);
      else
         FAT[k] = value16 & 0x0FFF;      
   }
   return true;
}

//int m_Drive = 0 <=> O A, 1 <=> O B
bool WriteFAT(WORD FAT[FAT__NUMBER_OF_ELEMENT], int m_Drive){
   CRWDisk disk; // Khai bao bien mang kieu CRWDisk
   unsigned char buf[FAT__NUMBER_OF_BYTE]; // bang FAT cua dia mem
   for ( int clus=0, off=0 ; clus < FAT__NUMBER_OF_ELEMENT; clus += 2 )
   {
      int offset = (clus)*3/2;
      buf[offset] = FAT[clus] & 0x00FF;
      buf[offset + 2] = FAT[clus + 1] >> 4;
      buf[offset + 1] = ((FAT[clus + 1] & 0x00F) << 4) | (FAT[clus] >> 8);
   }
   if (!disk.AbsWrite (m_Drive, 9, 1, &buf))
      return false;
	
   if (!disk.AbsWrite (m_Drive, 9, 10, &buf))
      return false;

   return true;
}

bool ReadENTRY(ENTRY Entry[ENTRY__NUMBER_OF_ELEMENT], int m_Drive){
   CRWDisk disk; // Khai bao bien mang kieu CRWDisk (dinh nghia trong RWDisk.h)      
   int nSectors = 14; // Doc 14 sector
   int lSectors = 19; // Bat dau tu sector thu 19
   if(!disk.AbsRead(m_Drive, nSectors, lSectors, Entry)){ // Doc toan bo 224 entry trong RDET
      return false;
   }
   return true;
}
bool WriteENTRY(ENTRY Entry[ENTRY__NUMBER_OF_ELEMENT], int m_Drive){
   CRWDisk disk; // Khai bao bien mang kieu CRWDisk (dinh nghia trong RWDisk.h)      
   int nSectors = 14; // Ghi 14 sector
   int lSectors = 19; // Bat dau tu sector thu 19
   if(!disk.AbsWrite(m_Drive, nSectors, lSectors, Entry)){ // Ghi toan bo 224 entry trong RDET
      return false;
   }
   return true;
}

