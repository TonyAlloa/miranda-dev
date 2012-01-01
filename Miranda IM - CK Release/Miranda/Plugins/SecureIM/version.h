#ifndef __VERSION_H_INCLUDED
#define __VERSION_H_INCLUDED

#define __MAJOR_VERSION			1
#define __MINOR_VERSION			0
#define __RELEASE_NUM			12
#define __BUILD_NUM			4

#define __FILEVERSION_STRING		__MAJOR_VERSION,__MINOR_VERSION,__RELEASE_NUM,__BUILD_NUM
#define __FILEVERSION_STRING_DOTS	__MAJOR_VERSION.__MINOR_VERSION.__RELEASE_NUM.__BUILD_NUM
#define __STRINGIFY(x)			#x
#define __TOSTRING(x)			__STRINGIFY(x)
#define __VERSION_STRING		__TOSTRING(__FILEVERSION_STRING_DOTS)
#define __VERSION_DWORD             	((__MAJOR_VERSION<<24) | (__MINOR_VERSION<<16) | (__RELEASE_NUM<<8) | __BUILD_NUM)

#endif //__VERSION_H_INCLUDED
