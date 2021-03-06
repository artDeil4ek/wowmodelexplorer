#pragma once

#include "core.h"
#include "SColor.h"
#include "wow_m2_structs.h"
#include "wow_animation.h"

class ITexture;
class IFileM2;
struct SModelBone;
class wow_m2instance;

struct Particle
{
	LENTRY	Link;			//live particles link

	vector3df		birthpos;
	vector3df		pos;
	vector3df		speed;
	vector3df		down;
	vector3df		dir;
	vector3df		corners[4];

	vector2df		size;
	f32  life, maxlife;
	SColorf		color;
	u32	tile;
};

class ParticleSystem;

class ParticleEmitter
{	
public:
	struct EmitterParam
	{
		float areaLen;
		float areaWidth;
		float speed;
		float variation;
		float vertical;
		float horizontal;
		float maxlife;
	};

	ParticleEmitter(ParticleSystem* sys) : sys(sys) {}
public:
	
	virtual void emitParticle(const wow_m2instance* character, u32 anim, u32 time, const EmitterParam& param, Particle* p) = 0;

protected:
	ParticleSystem* sys;
};

class PlaneParticleEmitter : public ParticleEmitter
{
public:
	PlaneParticleEmitter(ParticleSystem* sys) : ParticleEmitter(sys) {}
	virtual void emitParticle(const wow_m2instance* character, u32 anim, u32 time, const EmitterParam& param, Particle* p);
};

class SphereParticleEmitter : public ParticleEmitter
{
public:
	SphereParticleEmitter(ParticleSystem* sys) : ParticleEmitter(sys) {}
	virtual void emitParticle(const wow_m2instance* character, u32 anim, u32 time, const EmitterParam& param, Particle* p);
};

class ParticleSystem
{
friend class PlaneParticleEmitter;
friend class SphereParticleEmitter;

public:
	ParticleSystem()
		: Mesh(NULL)
	{
		tileRotation = 0;
		emitter = NULL;
		blend = 0;
		ParticleType = 0;
		rows = cols = 0;

		boneIndex = -1;
		texture = NULL;
		billboard = false;

		slowdown = 0;
		rotation = 0;

		EmitterType = 0;
		flags = 0;
		NumTiles = 0;

		Tiles = NULL;
	}

	~ParticleSystem()
	{
		delete emitter;
		delete[] Tiles;
	}

	struct TexCoordSet 
	{
		vector2df tc[4];
	};

public:
	void init(const M2::ModelParticleEmitterDef& mta, const u8* fileData, s32* globalSeq, u32 numGlobalSeq);

	matrix4 calcSpreadMatrix(float spread1, float spread2, float w, float l);

	void initTile(vector2df* tc, int num);

public:
	SWowAnimation<float>		emissionSpeed;
	SWowAnimation<float>		speedVariation;
	SWowAnimation<float>		verticalRange;
	SWowAnimation<float>		horizontalRange;
	SWowAnimation<float>		gravity;
	SWowAnimation<float>		lifespan;
	SWowAnimation<float>		emissionRate;
	SWowAnimation<float>		emissionAreaWidth;				//x
	SWowAnimation<float>		emissionAreaLength;				//z
	SWowAnimation<float>		deacceleration;
	SWowAnimation<u16>		enabled;

	IFileM2*	Mesh;
	ITexture*		texture;
	TexCoordSet*		Tiles;

	SColorf		colors[3];
	vector2df		sizes[3];
	
	float		slowdown;
	float		rotation;
	vector3df		pos;

	int		blend;
	int		ParticleType;
	s32		flags;
	u32		NumTiles;
	s16		tileRotation;
	s16		rows, cols;
	s16		boneIndex;
	s16		EmitterType;
	bool		billboard;
	
public:
	ParticleEmitter*		emitter;
};


struct RibbonSegment
{
	LENTRY	Link;			//live segment link

	vector3df	pos;
	vector3df up;
	vector3df back;
	float len;
	float len0;
	s32		time;
};

class RibbonEmitter
{
public:
	RibbonEmitter()
	: Mesh(NULL), texture(NULL)
	{
		parent = 0;
		boneIndex = 0;
		texture = 0;
		seglen = 0;
		numsegs = 0;
	}

public:
	void init(const M2::ModelRibbonEmitterDef& mta, const u8* fileData, s32* globalSeq, u32 numGlobalSeq);

public:
	SWowAnimation<vector3df>	color;
	SWowAnimationShort	opacity;
	SWowAnimation<float>		above, below;

	IFileM2*	Mesh;
	SModelBone* parent;
	ITexture* texture;

	vector3df pos;
	float seglen;
	int numsegs;
	int		boneIndex;
};