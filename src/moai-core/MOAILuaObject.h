// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moai-core/MOAILuaRef.h>

#define DECL_LUA_FACTORY(type)																\
	IMPLEMENT_FINALIZABLE ( type )															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); }			\
	static void RegisterLuaType () { MOAILuaFactoryClass < type >::Get ().Register (); }	\
	cc8* TypeName () const { return #type; }

#define DECL_LUA_ABSTRACT(type)																\
IMPLEMENT_ABSTRACT_FINALIZABLE ( type )														\
MOAILuaClass* GetLuaClass () { return 0; }													\
cc8* TypeName () const { return #type; }

#define DECL_LUA_OPAQUE(type)																\
	IMPLEMENT_FINALIZABLE ( type )															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); }			\
	cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type)															\
	IMPLEMENT_FINALIZABLE ( type )															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaSingletonClass < type >::Get (); }		\
	static void RegisterLuaType () { MOAILuaSingletonClass < type >::Get ().Register (); }	\
	cc8* TypeName () const { return #type; }

class MOAIDeserializer;
class MOAILuaClass;
class MOAILuaObject;
class MOAIComposer;
class MOAILuaState;
class MOAIScopedLuaState;
class MOAISerializer;

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject,
	public virtual ZLAbstractFinalizable {
private:

	u32						mActiveUserdataCount;
	MOAILuaWeakRef			mUserdata;		// ref to userdata (weak)
	MOAILuaStrongRef		mFinalizer;		// ref to finalizer (strong)

protected:

	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	static int				_getMemberTable			( lua_State* L );
	static int				_getRefTable			( lua_State* L );
	static int				_pin					( lua_State* L );
	static int				_serializeIn			( lua_State* L );
	static int				_serializeOut			( lua_State* L );
	static int				_setFinalizer			( lua_State* L );
	static int				_setInterface			( lua_State* L );
	static int				_setMembers				( lua_State* L );
	static int				_tostring				( lua_State* L );
	static int				_unpin					( lua_State* L );

	//----------------------------------------------------------------//
	static int				InjectAndCall			( lua_CFunction func, MOAILuaObject* self, lua_State* L );
	void					MakeLuaBinding			( MOAILuaState& state );
	bool					PushRefTable			( MOAILuaState& state );
	void					SetInterfaceTable		( MOAILuaState& state, int idx );
	void					SetMemberTable			( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	virtual void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	virtual void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	virtual	void			MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	void					ZLRefCountedObjectBase_OnRelease	( u32 refCount );

public:

	friend class MOAILuaCanary;
	friend class MOAILuaClass;
	friend class MOAILuaMemberRef;
	friend class MOAILuaRuntime;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	static int				_alertNewIsUnsupported		( lua_State* L );

	//----------------------------------------------------------------//
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	MOAIScopedLuaState		GetSelf						();
	void					GetRef						( MOAILuaRef& ref );
	bool					IsBound						();
	static bool				IsMoaiUserdata				( MOAILuaState& state, int idx );
	bool					IsSingleton					();
	void					LuaRelease					();
	void					LuaRelease					( MOAILuaObject* object );
	void					LuaRetain					();
	void					LuaRetain					( MOAILuaObject* object );
							MOAILuaObject				();
	virtual					~MOAILuaObject				();
	void					PrintTracking				();
	void					PushLuaClassTable			( MOAILuaState& state );
	bool					PushLuaUserdata				( MOAILuaState& state );
	bool					PushMemberTable				( MOAILuaState& state );
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	
	//----------------------------------------------------------------//
	template < typename TYPE, lua_CFunction FUNC >
	static int WrapInstanceFuncAsGlobal ( lua_State* L ) {
	
		TYPE* type = ZLContextMgr::Get ()->GetGlobal < TYPE >();
		assert ( type );
		return InjectAndCall ( FUNC, type, L );
	}
};

#endif
