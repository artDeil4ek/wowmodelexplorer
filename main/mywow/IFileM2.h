#pragma once

#include "core.h"
#include "IResourceCache.h"
#include "wow_enums.h"
#include "wow_animation.h"
#include "wow_particle.h"
#include "wow_m2_structs.h"
#include "S3DVertex.h"
#include <list>
#include <vector>
#include <map>
#include <set>

class MPQFile;
class ITexture;
class wow_m2Action;

inline vector3df fixCoordinate(const vector3df& v)
{
	return vector3df(v.X, v.Z, v.Y);
}

inline quaternion fixQuat(const quaternion& q)
{
	return quaternion(q.X, q.Z, q.Y, q.W);
}

#define	ANIMATION_HANDSCLOSED	15

struct SModelAnimation
{
	u32	animID;
	u32  animSubID;
	u32	timeLength;

	s16	NextAnimation;
	s16	Index;
};

#define	RENDERFLAGS_UNLIT		1
#define	RENDERFLAGS_UNFOGGED	2
#define	RENDERFLAGS_TWOSIDED	4
#define	RENDERFLAGS_BILLBOARD	8
#define	RENDERFLAGS_UNZWRITE		16
#define RENDERFLAGS_ADDCOLOR		256

enum E_BONE_TYPE
{
	EBT_NONE = 0,
	EBT_LEFTHAND,
	EBT_RIGHTHAND
};

struct SModelBone
{
	SWowAnimationVec3		trans;
	SWowAnimationQuat		rot;
	SWowAnimationVec3		scale;

	vector3df	pivot;
	s32		parent;
	u32		flags;
	E_BONE_TYPE  bonetype;			//from bone lookup table
	bool		billboard;

	void		init(const u8* filedata, s32* globalSeq, u32 numGlobalSeq, const M2::bone& b, SAnimFile* animFiles)
	{
		parent = b._ParentBone;
		billboard = (b._Flags & 8) != 0;
		flags = b._Flags;

		pivot = fixCoordinate(b._PivotPoint);
		trans.init(&b._Translation, filedata, animFiles, globalSeq, numGlobalSeq);
		rot.init(&b._Rotation, filedata, animFiles, globalSeq, numGlobalSeq);
		scale.init(&b._Scaling, filedata, animFiles, globalSeq, numGlobalSeq);
	}
};

struct SModelColor
{
	SWowAnimation<vector3df>	colorAnim;
	SWowAnimationShort		opacityAnim;

	void init(const u8* filedata, s32* globalSeq, u32 numGlobalSeq, const M2::colorDef& cd)
	{
		colorAnim.init(&cd.color, filedata, globalSeq, numGlobalSeq);
		opacityAnim.init(&cd.opacity, filedata, globalSeq, numGlobalSeq);
	}
};

struct SModelTransparency
{
	SWowAnimation<u16>	tranAnim;

	void init(const u8* filedata, s32* globalSeq, u32 numGlobalSeq, const M2::transDef& td)
	{
		tranAnim.init(&td.trans, filedata, globalSeq, numGlobalSeq);
	}
};

struct SModelTextureAnim
{
	SWowAnimation<vector3df>	trans, rot, scale;

	void init(const u8* filedata, s32* globalSeq, u32 numGlobalSeq, const M2::texanimDef& t)
	{
		trans.init(&t.trans, filedata, globalSeq, numGlobalSeq);
		rot.init(&t.rot, filedata, globalSeq, numGlobalSeq);
		scale.init(&t.scale, filedata, globalSeq, numGlobalSeq);
	}
};

struct SModelAttachment			//????
{
	SModelAttachment() : id(0), boneIndex(-1) {}
	s32 id;
	s32 boneIndex;
	vector3df position;
};

struct SModelCamera
{
	vector3df	position, target;
	f32	nearclip, farclip;
 //	SWowAnimationVec3		positionAnim, targetAnim;
 //	SWowAnimation<f32>		rotationAnim;
//	SWowAnimation<f32>		fovAnim;
	void init(const M2::ModelCameraDefV10& mcd, const u8* filedata, s32* globalSeq)
	{
		position = fixCoordinate(mcd.pos);
		target = fixCoordinate(mcd.target);
		nearclip = mcd.nearclip;
		farclip = mcd.farclip;
//  		positionAnim.init(&mcd.transPos, filedata, globalSeq);
//  		targetAnim.init(&mcd.transTarget, filedata, globalSeq);
//  		rotationAnim.init(&mcd.rot, filedata, globalSeq);
//		fovAnim.init(&mcd.fov, filedata, globalSeq);
	}
};

class IIndexBuffer;

class CBoneUnit
{
public:
	CBoneUnit() : BoneCount(0), PrimCount(0), Index(-1), BoneVStart(0), StartIndex(0) { }

	u32		BoneVStart;				//??skin bone????????????
	u32		PrimCount;
	u16		StartIndex;				//??skin ??index????
	u8	BoneCount;
	s8		Index;

	typedef std::set<u8, std::less<u8>, qzone_allocator<u8>> T_BoneIndices;
	T_BoneIndices			BoneIndices;

	typedef std::map<u8, u8, std::less<u8>, qzone_allocator<std::pair<u8,u8>>>	T_bone2boneMap;	 
	T_bone2boneMap		local2globalMap;					//local -> global
};

struct STexUnit
{
	s16	TexID;				//????????????
	s16	rfIndex;				//render flag????
	s16	ColorIndex;
	s16  TransIndex;
	s16	TexAnimIndex;
	u16	Mode;
	u16	Shading;		//shader?
	bool	WrapX;
	bool	WrapY;
	u32  TexFlags;
};

class	CGeoset 
{	
private:
	DISALLOW_COPY_AND_ASSIGN(CGeoset);

public:
	CGeoset() : VStart(0), VCount(0), IStart(0), ICount(0), GeoID(0), MaxWeights(4) {}

	u16		VStart;
	u16		VCount;
	u16		IStart;
	u16		ICount;
	u16		MaxWeights;
	u32		GeoID;	
	
	//tex unit
	typedef std::vector<STexUnit> T_TexUnits;
	T_TexUnits		TexUnits;

	//bone unit
	typedef std::vector<CBoneUnit> T_BoneUnits;
	T_BoneUnits		BoneUnits;

	u32 getTexUnitCount() const { return TexUnits.size(); }
	const STexUnit* getTexUnit(u8 index) const 
	{
		if (index >= TexUnits.size())
			return NULL;
		return &TexUnits[index];
	}
};

class CFileSkin;

#define	MAX_M2_TEXTURES	64

//????m2????????????????????m2????????????????skin??anim????
class IFileM2 : public IReferenceCounted<IFileM2>
{
private:
	virtual void onRemove() 
	{
		releaseVideoResources(); 
	}

public:
	IFileM2()
		: VideoBuilt(false), ContainsBillboardBone(false)
	{
		m2Header = NULL;
		GVertices = NULL;
		AVertices = NULL;
		Bounds = NULL;
		BoundTris = NULL;
		TextureFlags = NULL;
		TextureTypes = NULL;
		Textures = NULL;
		TexLookup = NULL;
		Attachments = NULL;
		AttachLookup = NULL;
		GlobalSequences = NULL;
		Colors = NULL;
		Transparencies = NULL;
		TransparencyLookup = NULL;
		TextureAnim = NULL;
		Bones = NULL;
		BoneLookup = NULL;
		Animations = NULL;
		AnimationLookup = NULL;
		RenderFlags = NULL;
		Skin = NULL;
		ParticleSystems = NULL;
		RibbonEmitters = NULL;
		ModelCameras = NULL;

		NumVertices =
		NumBoundingTriangles =
		NumBoundingVerts =
		NumTextures =
		NumTexLookup =
		NumAttachments =
		NumAttachLookup =
		NumBones = 
		NumBoneLookup =
		NumAnimations =
		NumAnimationLookup =
		NumGlobalSequences =
		NumColors =
		NumTransparencies =
		NumTranparencyLookukp =
		NumTexAnim = 
		NumRenderFlags = 
		NumParticleSystems =
		NumRibbonEmitters =
		NumModelCameras = 0;

		CollisionRadius = BoundingRadius = 0;

		::memset(Name, 0, sizeof(Name));
		::memset(Dir, 0, sizeof(Dir));
	}

	virtual ~IFileM2()  { }

	//render flags
	struct SRenderFlag
	{
		bool lighting;
		bool zwrite;
		bool frontCulling;
		bool invisible;				

		//raw
		u16	flags;
		u16	blend;
	};

public:
	virtual bool loadFile(MPQFile* file) = 0;
	virtual M2Type getType() const = 0;
	virtual const aabbox3df& getBoundingBox() const = 0;
	virtual s16 getAnimationIndex(const c8* name, u32 subIdx = 0)= 0;
	virtual u32 getAnimationCount(const c8* name)= 0;

	virtual bool buildVideoResources() = 0;
	virtual void releaseVideoResources() = 0;

	virtual void clearAllActions() = 0;
	virtual bool addAction(wow_m2Action* action) = 0;
	virtual wow_m2Action* getAction(const c8* name) const = 0;

public:
	c8		Name[DEFAULT_SIZE];
	c8		Dir[MAX_PATH];

	u32		NumVertices;
	u32		NumBoundingVerts;
	u32		NumBoundingTriangles;
	u32		NumTextures;
	u32		NumTexLookup;
	u32		NumAttachments;
	u32		NumAttachLookup;
	u32		NumGlobalSequences;
	u32		NumColors;
	u32		NumTransparencies;
	u32		NumTranparencyLookukp;
	u32		NumTexAnim;
	u32		NumAnimations;
	u32		NumAnimationLookup;
	u32		NumBones;
	u32		NumBoneLookup;
	u32		NumRenderFlags;
	u32		NumParticleSystems;
	u32		NumRibbonEmitters;
	u32		NumModelCameras;

	M2::header*		m2Header;

	//verts
	SVertex_PNTW*		GVertices;
	SVertex_A*			AVertices;

	//bounds
	vector3df*		Bounds;
	u16*		BoundTris;
	aabbox3df		CollisionAABBox;			//????????????????????
	float		CollisionRadius;
	aabbox3df	BoundingAABBox;			//??????????
	float		BoundingRadius;

	//texture
	u32*		TextureFlags;
	ETextureTypes*		TextureTypes;							//??????????????????????????????????mesh??
	ITexture**		Textures;					//????????????mesh??????????????
	s16*		TexLookup;
	
	//attachment
	SModelAttachment*			Attachments;	
	s16*		AttachLookup;
	
	//anim
	s32*		GlobalSequences;
	SModelColor*		Colors;
	SModelTransparency*		Transparencies;
	s16*		TransparencyLookup;
	SModelTextureAnim*		TextureAnim;
	
	//actions
	SModelAnimation*		Animations;
	s16*		AnimationLookup;
	
	//bones
	SModelBone*		Bones;
	s16*		BoneLookup;
	
	SRenderFlag*		RenderFlags;

	//paritcle
	ParticleSystem*		ParticleSystems;

	//ribbon
	RibbonEmitter*			RibbonEmitters;

	//camera
	SModelCamera*			ModelCameras;
	
	//skin 0
	CFileSkin*		Skin;

	bool	VideoBuilt;
	bool	ContainsBillboardBone;
};
