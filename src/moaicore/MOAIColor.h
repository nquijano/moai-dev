// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLOR_H
#define	MOAICOLOR_H

#include <moaicore/MOAIShader.h>

class MOAIContentLibrary2D;
class MOAILayer2D;

//================================================================//
// MOAIColor
//================================================================//
/**	@brief Color vector.
*/
class MOAIColor :
	public virtual MOAIShader,
	public USColorVec {
protected:
	
	//----------------------------------------------------------------//
	static int		_moveColor			( lua_State* L );
	static int		_seekColor			( lua_State* L );
	static int		_setColor			( lua_State* L );

public:
	
	enum {
		ATTR_R_COL,
		ATTR_G_COL,
		ATTR_B_COL,
		ATTR_A_COL,
		TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIColor )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	void			Bind				();
	u32				CountAttributes		();
					MOAIColor			();
					~MOAIColor			();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
