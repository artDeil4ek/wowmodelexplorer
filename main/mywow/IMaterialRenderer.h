#pragma once

#include "base.h"
#include "SMaterial.h"

class IPixelShader;

//根据不同的material设置不同的渲染状态
class IMaterialRenderer
{
public:
	virtual ~IMaterialRenderer() {}

public:
	virtual void OnSetMaterial( E_VERTEX_TYPE vType, const SMaterial& material, bool resetAllRenderStates) = 0;

	//for fp
	virtual u32 getNumPasses() const { return 1; }

	virtual void OnRender(const SMaterial& material, u32 pass) {}

public:
	SRenderStateBlock		MaterialBlock;
};

