// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESYSTEM_H
#define	MOAIPARTICLESYSTEM_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIGfxPrim2D.h>

class MOAIContentLibrary2D;
class MOAIParticleEngine;
class MOAIParticleScript;
class MOAIParticleState;
class MOAITexture;

//================================================================//
// MOAIParticle
//================================================================//
class MOAIParticle {
private:
	friend class MOAIParticleSystem;
	friend class MOAIParticleEngine;

	float					mAge;
	float					mDuration;
	MOAIParticleState*		mState;
	u32						mID;

	MOAIParticle*	mNext;
};

//================================================================//
// MOAIParticleSprite
//================================================================//
class MOAIParticleSprite {
private:
	friend class MOAIParticleSystem;
	friend class MOAIParticleEngine;

	USVec2D		mLoc;
	float		mRot;
	USVec2D		mScale;
	u32			mColor;
	float		mGlow;
	
	u32			mGfxID;
};

//================================================================//
// MOAIParticleSystem
//================================================================//
/**	@brief Particle system.
*/
class MOAIParticleSystem :
	public MOAIGfxPrim2D,
	public MOAIAction {
private:

	USLeanArray < MOAIParticleState* >	mStates;

	USLeanArray < MOAIParticle >		mParticles;
	USLeanArray < float >				mParticleData;
	u32									mParticleSize;
	
	MOAIParticle*						mParticle;
	bool								mCapParticles;
	bool								mCapSprites;
	bool								mKill;
	
	MOAIParticle*						mHead;
	MOAIParticle*						mTail;
	MOAIParticle*						mFree;
	
	USLeanArray < float >				mConsts;
	USLeanArray < MOAIParticleSprite >	mSprites;
	u32									mSpriteTop;
	
	//----------------------------------------------------------------//
	static int		_capParticles		( lua_State* L );
	static int		_capSprites			( lua_State* L );
	static int		_clearSprites		( lua_State* L );
	static int		_getState			( lua_State* L );
	static int		_pushParticle		( lua_State* L );
	static int		_pushSprite			( lua_State* L );
	static int		_reserveConstants	( lua_State* L );
	static int		_reserveParticles	( lua_State* L );
	static int		_reserveRects		( lua_State* L );
	static int		_reserveSprites		( lua_State* L );
	static int		_reserveStates		( lua_State* L );
	static int		_setConstant		( lua_State* L );
	static int		_setSpriteColor		( lua_State* L );
	static int		_setSpriteGfxID		( lua_State* L );
	static int		_setSpriteGlow		( lua_State* L );
	static int		_setState			( lua_State* L );
	static int		_surge				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ClearStates				();
	void					ClearQueue				();
	void					EnqueueParticle			( MOAIParticle& particle );
	MOAIParticleSprite*		GetTopSprite			();
	MOAIParticleState*		GetState				( u32 id );
	void					LoadParticle			( u32 particleID, float registers []);
	void					OnUpdate				( float step );
	u32						PackRegisterIDs			( u8 r0, u8 r1, u8 r2, u8 r3 );
	bool					PushSprite				( const MOAIParticleSprite& sprite );
	void					StoreParticle			( u32 particleID, float registers []);

public:
	
	friend class MOAIParticleEngine;
	
	DECL_LUA_DATA ( MOAIParticleSystem )

	//----------------------------------------------------------------//
	void			Draw					();
	u32				GetLocalFrame			( USRect& frame );
	bool			IsBusy					();
					MOAIParticleSystem		();
					~MOAIParticleSystem		();
	bool			PushParticle			( float x, float y );
	bool			PushParticle			( float x, float y, float dx, float dy );
	void			ReserveConstants		( u32 total );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReserveParticles		( u32 maxParticles, u32 particleSize );
	void			ReserveRects			( u32 total );
	void			ReserveSprites			( u32 maxSprites );
	void			ReserveStates			( u32 total );
	void			SetConstant				( u32 idx, float value );
	void			SetRect					( u32 idx, USRect& rect );
	STLString		ToString				();
};

#endif
