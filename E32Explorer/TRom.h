#pragma once
#include <stdint.h>
#include "E32Std.h"

const uint32_t KRomHeaderSize = 0x100;

typedef int64_t TTime;
typedef uint32_t TLinAddr;

struct TVersion {
	uint8_t Major;
	uint8_t Minor;
	uint16_t Build;
};

struct TRomHeader {
	uint8_t iJump[128];
	TVersion iVersion;
	int64_t iTime;
	TLinAddr iRomBase;
	uint32_t iRomSize;
	TLinAddr iRomRootDirectoryList;
	TLinAddr iKernDataAddress;
	TLinAddr iKernStackAddress;
	TLinAddr iPrimaryFile;
	TLinAddr iSecondaryFile;
	uint32_t iCheckSum;
	int64_t iLanguage;
	uint32_t iHardware;
	union {
		uint64_t iScreenSize;
		struct iScreenSize {
			uint32_t Width;
			uint32_t Height;
		};
	};
	int32_t iScreenBitsPerPixel;
	TLinAddr iRomSectionHeader;
	int32_t iTotalSvDataSize;
	TLinAddr iVariantFile;
	TLinAddr iExtensionFile;
	TLinAddr iRelocInfo;
	uint32_t  iTraceMask;  // The kernel tracemask
	TLinAddr iUserDataAddress;	// non-MMU stuff
	int32_t iTotalUserDataSize;	// non-MMU stuff
	uint32_t iPad[32 - 25]; // sizeof(TRomHeader)=256
};

struct TRootDirInfo {
	uint32_t iHardwareVariant;
	TLinAddr iAddressLin;
};

struct TRomRootDirectoryList
{
	int32_t iNumRootDirs;
	TRootDirInfo iRootDir[1];
};

struct TRomSectionHeader{
	TVersion iVersion;
	TTime iTime;
	int64_t iLanguage;
	uint32_t iCheckSum;
};

struct TRomEntry {
public:
	int32_t iSize;
	uint32_t iAddressLin;
	uint8_t iAtt;
	uint8_t iNameLength;
	uint8_t iName[2];
};
const int32_t KRomEntrySize = (sizeof(TRomEntry) - 2);

class TRomDir
{
public:
	int32_t iSize;
	TRomEntry iEntry;
};

//
// Header for extension ROMs
// Equivalent to TRomHeader
//
class TExtensionRomHeader
{
public:
	TVersion iVersion;
	int64_t iTime;
	TLinAddr iRomBase;
	uint32_t iRomSize;
	TLinAddr iRomRootDirectoryList;
	uint32_t iCheckSum;
	//
	TVersion iKernelVersion;
	int64_t iKernelTime;
	uint32_t iKernelCheckSum;
	uint32_t iPad[32 - 11]; // sizeof(TExtensionRomHeader)=128
};