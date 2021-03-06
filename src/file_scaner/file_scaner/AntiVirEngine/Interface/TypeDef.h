#pragma once

/**
* 基本宏定义：
* 平台宏，用于区分当前编译的目标平台，定义下列宏表明编译的目标操作系统为对应的操作系统
*	PLT_WIN32 32位 Windows 平台
*   PLT_WIN64 64位 Windows 平台
*   PLT_LINUX32 32 位 Linux 平台
*   PLT_LINUX64 64 位 Linux 平台
* 传参方式:
*   AV_CALLTYPE
*/

#include <stdint.h>

#ifndef PURE
#define PURE =0
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


#ifdef _MSC_VER
#ifdef _WIN64
#define  PLT_WIN64
#define  PLT_WIN
#else
#define  PLT_WIN32
#define  PLT_WIN
#endif 
#endif // VC

#ifdef __GNUC__
#ifdef _X86_
#define  PLT_LINUX32
#define  PLT_LINUX
#else
#define  PLT_LINUX64
#define  PLT_LINUX
#endif // _X86_
#endif // GCC


#ifdef PLT_WIN

#define CRUN_CALLTYPE __cdecl // C运行库的传参方式
#define  AV_CALLTYPE __stdcall 
#define PATH_SPLIT_CHAR  '\\'
#define PATH_SPLIT_STRING L"\\"

#ifdef AVUTIL_EXPORTS
#define AVUTIL_API __declspec(dllexport)
#else
#define AVUTIL_API __declspec(dllimport)
#endif

#endif // PLT_WIN

#ifdef PLT_LINUX

#define CRUN_CALLTYPE __attribute__((__cdecl__)) // C运行库的传参方式
#define AV_CALLTYPE __attribute__((__stdcall__))
#define PATH_SPLIT_CHAR  '/'
#define PATH_SPLIT_STRING L"/"

#define AVUTIL_API 

#define __cdecl 	 __attribute__((__cdecl__))


#include <stdio.h>
#include <stdlib.h>


#endif // PLT_LINUX


#ifdef __cplusplus
#define  EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#define EXTERN_C  extern "C"

#ifdef PLT_WIN

#ifdef _WIN64
    typedef unsigned __int64 size_t;	// 8 Bytes
#else
    typedef unsigned int     size_t;	// 4 Bytes
#endif // WIN64

__if_not_exists(FILETIME) {
#include  "PshPack4.h"
	typedef struct _FILETIME {
		uint32_t dwLowDateTime;
		uint32_t dwHighDateTime;
	} FILETIME, *PFILETIME;
#include "PopPack.h"
}

#endif // WIN32

#ifdef PLT_LINUX

/*
#ifdef PLT_LINUX64
    typedef unsigned long long  size_t;		// 8 bytes
#else
    typedef unsigned int     size_t;		// 4 bytes
#endif // PLT_LINUX64
*/

typedef long long __int64;		// 8 bytes


#define _IN_
#define _OUT_
#define IN
#define OUT

#define _In_
#define _Out_

#define _In_z_
#define _Inout_z_
#define _Out_z_

#define _In_opt_
#define _Out_opt_

#define _Ret_maybenull_
#define _Post_writable_byte_size_(n)
#define _Inout_updates_(n)
#define _Inout_updates_to_(n,n2)
#define _Out_writes_all_(n)
#define _In_reads_(n)
#define _Out_writes_all_(n)
#define _Null_terminated_

#define nullptr 0

#define FALSE 0

#define _ATL_PACKING	8
#define AtlThrow(x)

#include  "PshPack4.h"
	typedef struct _FILETIME {
		uint32_t dwLowDateTime;
		uint32_t dwHighDateTime;
	} FILETIME, *PFILETIME;
#include "PopPack.h"
#endif // PLT_LINUX

/*
*
*/
class IFileEx {
protected:
	virtual ~IFileEx() {
	}
public:
	/**
	@brief 文件定位方式
	*/
	enum ESeekFrom
	{
		seekFromBegin = 0, ///< 从文件开始
		seekFromCurrent,   ///< 从当前位置
		seekFromEnd,       ///< 从文件结尾
	};

	/**
	* @brief 文件准备清除的结果
	*/
	enum EPrepareForCureResult
	{
		forCureUnknown = 0,
		forCureSuccess = 0x1,
		forCureReboot = 0x2,
		forCureFailed = 0x3,
		forCureIsSafe = 0x4, // 该文件是安全的，不应该做报毒
		forCureSubstitute = 0x5,
		forCureCanDelete = 0x6, // 无法以写方式打开以清除，但可以删除，如果用户设置清除失败删除的话，可以调用删除进行删除
		forCureDelete = 0x7, // 应该直接进行删除
	};

	enum EDeleteResult
	{
		deleteSuccess = 0x1,
		deleteReboot = 0x2,
		deleteFailed = 0x3,
		deleteIsSafe = 0x4,
	};
	static const uint64_t INVALID_SIZE = UINT64_MAX;

	virtual void AV_CALLTYPE Dispose() PURE;

	/**
	@brief 读取文件

	@param buffer 读取到的内存缓冲区
	@param ulByteCount 缓冲区的大小

	@retval 返回 读取的字节数，如果失败返回 0
	*/
	virtual uint32_t AV_CALLTYPE Read(OUT void* buffer, IN uint32_t ulByteCount) PURE;


	/**
	@brief 写文件

	@param buffer 要写的内存缓冲区
	@param ulByteCount 缓冲区大小

	@retval 返回写入的字节数
	*/
	virtual uint32_t AV_CALLTYPE Write(IN void const* buffer, IN uint32_t ulByteCount) PURE;


	/**
	@brief 调整文件访访问偏移

	如果文件已经删除或者改名，调用该方法时不需要做任何处理，直接返回　INVALID_SIZE 即可

	@param qwOffset 调整的文件偏移量
	@param nFrom 调整的起始位置

	@retval 返回 当前的文件偏移，如果失败返回 INVALID_SIZE

	@see ESeekFrom
	*/
	virtual uint64_t AV_CALLTYPE Seek(IN int64_t qwOffset, IN ESeekFrom nFrom) PURE;


	/**
	@brief 取当前文件偏移位置

	如果文件已经删除或者改名，调用该方法是直接返回 INVALID_SIZE

	@retval 返回 当前文件偏移，如果失败返回 INVALID_SIZE
	*/
	virtual uint64_t AV_CALLTYPE GetPosition() PURE;


	/**
	@brief 设置文件的大小

	@param qwNewSize 将该文件大小调整为的大小

	@retval 如果成功返回 TRUE，否则返回 FALSE
	*/
	virtual bool AV_CALLTYPE SetSize(IN uint64_t qwNewSize) PURE;


	/**
	@brief 读取文件的大小

	@retval 返回文件大小，如果失败返回 UINT64_MAX
	*/
	virtual uint64_t AV_CALLTYPE GetSize() PURE;

	/**
	* @brief 读取文件的完整路径
	*
	*/
	virtual const wchar_t* AV_CALLTYPE GetPath() PURE;

	/**
	@brief 读取文件对象名称

	该方法返回的是名称，不包含其他路径，由于返回的是名称的指针，文件对象负责
	保证在文件对象有效性，该指针指向的内容有效

	@retval 返回具体名称
	*/
	virtual const wchar_t* AV_CALLTYPE GetName() PURE;


	/**
	@brief 读取文件的扩展包，包含开头的 .

	*/
	virtual const char* AV_CALLTYPE GetExtA() PURE;

	/**
	@brief 保证该文件对象是可写的，以便于清除处理

	在扫描过程中，PrepareForCure 会调用多次，因此实现时应该进行相应的处理，
	支持对该函数的重复调用
	该函数返回时，应该保证文件指针与调用函数时的值一致。

	@retval 返回具体结果，可能值见 EPrepareForCureResult
	*/
	virtual EPrepareForCureResult AV_CALLTYPE PrepareForCure() PURE;

	/**
	@brief 本方法通知对应的文件对象需要删除

	具体实现时可以在文件关闭时再作删除，同时应该注意调用该函数后不应该再调用其他相关的文件
	操作，否则可能导致未知结果。
	对于删除后的文件，需要支持　Seek 和 GetPosition 函数，这时执行空操作即可，并返回
	INVALID_SIZE 。

	@retval 返回枚举 EDeleteResult 中的值
	*/
	virtual EDeleteResult AV_CALLTYPE Delete() PURE;

	/**
	@breif 取修改时间

	主要用于指纹中的相关计算
	*/
	virtual bool AV_CALLTYPE GetModifyTime(FILETIME* tm) PURE;
};

#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#define EXTERN_C 

#endif

#define MAX_VIRNAME_LEN		64
