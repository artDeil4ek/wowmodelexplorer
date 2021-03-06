#pragma once

#include "IParticleSystemSceneNode.h"
#include "wow_particle.h"
#include "SMaterial.h"

class ISceneManager;
class IParticleSystemServices;

class CParticleSystemSceneNode : public IParticleSystemSceneNode
{
public:
	CParticleSystemSceneNode(ParticleSystem* ps, IM2SceneNode* parent);
	virtual ~CParticleSystemSceneNode();

public:
	//ISceneNode
	virtual void registerSceneNode(bool frustumcheck = true);
	virtual bool isNodeEligible() const;
	virtual aabbox3df getBoundingBox() const { return aabbox3df::Zero(); }
	virtual void tick(u32 timeSinceStart, u32 timeSinceLastFrame, bool visible);
	virtual void render();
	virtual void onPreRender();
	virtual void onUpdated();

	static const u32 MAX_PARTICLES = 400;

	virtual void setEmitting(bool on) { Emitting = on; }
	virtual void setAnimationFrame(u32 anim, u32 frame) { CurrentAnim = anim; CurrentFrame = frame; }
	virtual void setSpeed(f32 speed) { Speed = speed; }
	virtual void setOwnView(matrix4* view) { CurrentView = view; }
	virtual void setOwnProjection(matrix4* projection) { CurrentProjection = projection; }

	virtual void setWholeAlpha(bool enable, f32 val);
	virtual void setWholeColor(bool enable, SColor color);

	virtual void setEmitDensity(f32 density) { Density = density; }

protected:
	void setMaterial(SMaterial& material);

private:
	struct SHint
	{
		s32 grav;
		s32 deaccel;
		s32 frate;
		s32 flife;
		s32 len;
		s32 width;
		s32 speed;
		s32 variation;
		s32 vert;
		s32 horz;
		s32 en;
	};

private:
	IParticleSystemServices*	ParticleSystemServices;
	IM2SceneNode*		ParentM2Node;
	ParticleSystem*		Ps;
	matrix4*		CurrentProjection;
	matrix4*		CurrentView;

	LENTRY		LiveParticleList;
	u32		LiveParticleCount;
	u32		CurrentAnim;
	u32		CurrentFrame;
	u32		LastAnim;
	u32		LastTime;

	f32		Density;
	f32		Rem;
	f32		Speed;
	f32		Scale;
	f32			ModelAlpha;

	SMaterial		Material;
	SHint	Hint;

	SColor	ModelColor;
	bool		EnableModelAlpha;
	bool		EnableModelColor;
	bool		Emitting;

	
};