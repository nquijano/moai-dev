// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICPARBITER_H
#define	MOAICPARBITER_H
#if USE_CHIPMUNK

struct cpArbiter;
class MOAICpSpace;

//================================================================//
// MOAICpArbiter
//================================================================//
/**	@brief Chipmunk Arbiter.
*/
class MOAICpArbiter :
	public virtual USLuaData {
private:

	cpArbiter*	mArbiter;

	//----------------------------------------------------------------//
	static int		_countContacts					( lua_State* L );
	static int		_getContactDepth				( lua_State* L );
	static int		_getContactNormal				( lua_State* L );
	static int		_getContactPoint				( lua_State* L );
	static int		_getTotalImpulse				( lua_State* L );
	static int		_getTotalImpulseWithFriction	( lua_State* L );
	static int		_isFirstContact					( lua_State* L );
	static int		_new							( lua_State* L );
	
	//----------------------------------------------------------------//

public:
	
	DECL_LUA_DATA ( MOAICpArbiter )
	
	GET_SET ( cpArbiter*, Arbiter, mArbiter )
	
	//----------------------------------------------------------------//
					MOAICpArbiter			();
					~MOAICpArbiter			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
#endif