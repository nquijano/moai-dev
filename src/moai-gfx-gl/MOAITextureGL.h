// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREGL_H
#define	MOAITEXTUREGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class ZLImage;

//================================================================//
// MOAITextureGL
//================================================================//
/**	@lua	MOAITextureGL
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureGL :
	public virtual MOAITexture,
	public virtual MOAIGfxResourceGL {
protected:

	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class ZLTextureFormat;

	// GL texture
	ZLGfxHandle			mGLTexture;
	
	int					mGLInternalFormat;
	int					mGLPixelType;

	//----------------------------------------------------------------//
	static int			_getSize					( lua_State* L );
	static int			_release					( lua_State* L );
	static int			_setDebugName				( lua_State* L );
	static int			_setFilter					( lua_State* L );
	static int			_setWrap					( lua_State* L );

	//----------------------------------------------------------------//
	void				CleanupOnError				();
	bool				CreateTextureFromImage		( ZLImage& srcImage );
	void				SetGLTexture				( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );
	bool				ShouldGenerateMipmaps		();
	bool				UpdateTextureFromImage		( ZLImage& image, ZLIntRect rect );
	
	//----------------------------------------------------------------//
	bool				MOAIGfxResource_OnCPUCreate				();
	void				MOAIGfxResource_OnCPUPurgeRecoverable	();
	void				MOAIGfxResourceGL_OnGPUBind				();
	void				MOAIGfxResourceGL_OnGPUDeleteOrDiscard	( bool shouldDelete );
	void				MOAIGfxResourceGL_OnGPUUnbind			();
	bool				MOAIGfxResourceGL_OnGPUUpdate			();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void				ZLGfxListener_OnGfxEvent				( u32 event, void* userdata );
	
public:
	
	GET_CONST ( ZLGfxHandle&, GLTexture, mGLTexture );

	//----------------------------------------------------------------//
	static void			CheckFilterModes			( int min, int mag );
						MOAITextureGL				();
						~MOAITextureGL				();
	
};

#endif