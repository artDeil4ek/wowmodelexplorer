#pragma once

#include "core.h"

class MPQArchive
{
public:
	explicit MPQArchive(const c8* filename);
	~MPQArchive();
public:
	bool applyPatch(const c8* patchname);
	void applyPatchs(const c8* patchnames[], u32 num);
	void close();

public:
	LENTRY	Link;

	HANDLE	mpq_a;
	string256	archivename;
};

class MPQFile
{
public:
	MPQFile( u8* buf, u32 size, const c8* fname, bool tmp) : buffer(buf), size(size), pointer(0), eof(false), temp(tmp)
	{
		strcpy_s(filename, MAX_PATH, fname);
	}
	~MPQFile()
	{
		close();
	}

	u32		read(void* dest, u32 bytes);
	u32		getSize() const { return size; }
	u32		getPos() const { return pointer; }
	u8*		getBuffer() { return buffer; }
	u8*		getPointer() { return buffer + pointer; }
	bool		isEof() const { return eof; }
	bool		seek(s32 offset, bool relative=false);
	void		close();
	void		save(const c8* filename);
	const c8*	getFileName() const { return filename; }			

private:
	bool eof;
	u8* buffer;
	u32 pointer, size;
	c8	filename[MAX_PATH];
	bool temp;
};

inline void flipcc(char *fcc)
{
	char t;
	t=fcc[0];
	fcc[0]=fcc[3];
	fcc[3]=t;
	t=fcc[1];
	fcc[1]=fcc[2];
	fcc[2]=t;
}
