#pragma once

#include "base.h"

class IVertexBuffer
{
private:
	DISALLOW_COPY_AND_ASSIGN(IVertexBuffer);

public:
	IVertexBuffer()
	{
		HWLink = NULL;
		Clear = true;
		Vertices = NULL;
		Type = EST_P;
		Size = 0;
		Mapping = EMM_STATIC;

		InitializeListHead(&Link);
	}
	IVertexBuffer(bool clear)
		: Clear(clear)
	{
		HWLink = NULL;
		Vertices = NULL;
		Type = EST_P;
		Size = 0;
		Mapping = EMM_STATIC;
	}

	virtual ~IVertexBuffer() 
	{
		if (Clear)
			delete[] Vertices;
	}

public:
	void set(void* vertices, E_STREAM_TYPE type, u32 size, E_MESHBUFFER_MAPPING mapping);

	void setClear(bool c) { Clear = c; }

public:
	LENTRY		Link;		//

	void* Vertices;
	void*	HWLink;
	u32  Size;
	E_STREAM_TYPE		Type;
	E_MESHBUFFER_MAPPING		Mapping;
	
private:
	bool Clear;
};

inline void IVertexBuffer::set( void* vertices, E_STREAM_TYPE type, u32 size, E_MESHBUFFER_MAPPING mapping )
{
	Vertices = vertices;
	Type = type;
	Size = size;

	Mapping = mapping;
}

class IIndexBuffer
{
private:
	DISALLOW_COPY_AND_ASSIGN(IIndexBuffer);

public:
	IIndexBuffer() 
	{
		HWLink = NULL;
		Clear = true;
		Indices = NULL;
		Type = EIT_16BIT;
		Size = 0;
		Mapping = EMM_STATIC;

		InitializeListHead(&Link);
	}
	IIndexBuffer(bool clear) 
		: Clear(clear) 
	{
		HWLink = NULL;
		Indices = NULL;
		Type = EIT_16BIT;
		Size = 0;
		Mapping = EMM_STATIC;
	}

	virtual ~IIndexBuffer() 
	{
		if (Clear)
			delete[] Indices;
	}

public:
	void set(void* indices, E_INDEX_TYPE type, u32 size, E_MESHBUFFER_MAPPING mapping);

	void setClear(bool c) { Clear = c; }
public:
	LENTRY		Link;		//
public:
	void*	Indices;
	void*	HWLink;
	u32  Size;
	E_INDEX_TYPE		Type;
	E_MESHBUFFER_MAPPING		Mapping;
		
private:
	bool Clear;
};

inline void IIndexBuffer::set( void* indices, E_INDEX_TYPE type, u32 size, E_MESHBUFFER_MAPPING mapping)
{
	Indices = indices;
	Type = type;
	Size = size;

	Mapping = mapping;
}

struct SBufferParam
{
	IVertexBuffer*		vbuffer0;				//1 stream
	IVertexBuffer*		vbuffer1;			//2 stream
	E_VERTEX_TYPE		vType;
	IIndexBuffer*		ibuffer;

	void clear()
	{
		vbuffer0 = vbuffer1 = NULL;
		ibuffer = NULL;
	}

	void destroy()
	{
		delete ibuffer; ibuffer = NULL;
		delete vbuffer1; vbuffer1 = NULL;
		delete vbuffer0; vbuffer0 = NULL;
	}

	IVertexBuffer* getVBuffer(u32 index) const
	{
		switch(index)
		{
		case 0:
			return vbuffer0;
		case 1:
			return vbuffer1;
		default:
			return NULL;
		}
	}

};