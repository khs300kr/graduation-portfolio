#pragma once
#include "stdafx.h"

class CStream
{
private:
	BYTE*			m_BufferPointer;
	DWORD			m_Length;
public:
	CStream();
	~CStream();
public:
	BOOL SetBuffer(BYTE *buf);

	BOOL ReadInt32(INT* data);
	BOOL ReadDWORD(DWORD* data);
	BOOL READDWORD_PTR(DWORD_PTR *data);
	BOOL READByte(BYTE* data);
	BOOL ReadBytes(BYTE* data, DWORD length);
	BOOL ReadFloat(float* data);
	BOOL ReadInt64(INT64 * data);
	BOOL READUSHORT(USHORT *data);
	BOOL ReadSHORT(SHORT *data);
	BOOL ReadBOOL(BOOL *data);

	BOOL WriteInt32(INT* data);
	BOOL WriteDWORD(DWORD* data);
	BOOL WriteDWORD_PTR(DWORD_PTR *data);
	BOOL WriteByte(BYTE* data);
	BOOL WriteBytes(BYTE* data, DWORD length);
	BOOL WriteFloat(float* data);
	BOOL WriteInt64(INT64 * data);
	BOOL WriteUSHORT(USHORT *data);
	BOOL WriteSHORT(SHORT *data);
	BOOL WriteBOOL(BOOL *data);

	DWORD GetLength();
};