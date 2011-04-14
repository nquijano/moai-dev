// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USDrawBuffer.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USTexture.h>

//================================================================//
// USDrawBuffer
//================================================================//

//----------------------------------------------------------------//
void USDrawBuffer::BeginPrim () {

	if ( this->mPrimSize ) {

		u32 primBytes = this->mPrimSize * this->mVertexFormat.GetVertexSize ();

		this->mMaxPrims = ( u32 )( this->mSize / primBytes );
		this->mPrimTop = this->mTop + primBytes;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::BeginPrim ( u32 primType ) {

	this->SetPrimType ( primType );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
bool USDrawBuffer::BindTexture ( USTexture* texture ) {
	
	if ( this->mTexture == texture ) {
		return texture ? true : false;
	}
	
	this->Flush ();
	
	if ( texture ) {
		if ( !this->mTexture ) {
			glEnable ( GL_TEXTURE_2D );
		}
		this->mTexture = texture;
		return texture->Bind ();
	}

	if ( this->mTexture ) {
		glDisable ( GL_TEXTURE_2D );
		this->mTexture = 0;
	}
	return false;
}

//----------------------------------------------------------------//
void USDrawBuffer::BindVertexFormat ( const USVertexFormat& format ) {

	if ( !this->mVertexFormat.IsMatch ( format )) {

		this->Flush ();
		this->mVertexPreset = USVertexFormatMgr::CUSTOM_FORMAT;

		this->mVertexFormat = format;

		this->mVertexFormat.Bind ( this->mBuffer );
		this->mVertexColorType = format.GetColorType ();
	}
	
}

//----------------------------------------------------------------//
void USDrawBuffer::BindVertexPreset ( u32 presetID ) {

	if ( this->mVertexPreset != presetID ) {

		this->Flush ();
		this->mVertexPreset = presetID;
		
		const USVertexFormat& preset = USVertexFormatMgr::Get ().GetPreset ( presetID );
		this->mVertexFormat = preset;

		this->mVertexFormat.Bind ( this->mBuffer );
		this->mVertexColorType = preset.GetColorType ();
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::Clear () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
		this->mSize = 0;
		this->mTop = 0;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::DrawPrims () {

	u32 vertexSize = this->mVertexFormat.GetVertexSize ();

	if ( this->mPrimCount && vertexSize ) {
	
		u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / vertexSize );
		glDrawArrays ( this->mPrimType, 0, count );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::EndPrim () {

	if ( this->mPrimSize ) {
		this->mTop = this->mPrimTop;
	}
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
		this->Flush ();
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::Flush () {

	this->DrawPrims ();

	this->mTop = 0;
	this->mPrimTop = 0;
	this->mPrimCount = 0;
}

//----------------------------------------------------------------//
const USColorVec& USDrawBuffer::GetPenColor () {

	return this->mPenColor;
}

//----------------------------------------------------------------//
const USAffine2D& USDrawBuffer::GetUVTransform () {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
const USAffine2D& USDrawBuffer::GetVtxTransform () {

	return this->mVtxTransform;
}

//----------------------------------------------------------------//
void USDrawBuffer::Rebind () {

	this->Flush ();
}

//----------------------------------------------------------------//
void USDrawBuffer::Reserve ( u32 size ) {

	this->mSize = size;
	this->mTop = 0;
	this->mBuffer = malloc ( size );
}

//----------------------------------------------------------------//
void USDrawBuffer::Reset () {

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	glDisable ( GL_TEXTURE_2D );
	this->mTexture = 0;
	
	// turn off blending
	glDisable ( GL_BLEND );
	this->mBlendEnabled = false;
	
	// disable vertex arrays
	this->mVertexFormat.Bind ( this->mBuffer );

	// load identity matrix
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	// ident the cpu transforms
	this->mUVTransform.Ident ();
	this->mVtxTransform.Ident ();
	
	// disable backface culling
	glDisable ( GL_CULL_FACE );
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	glLineWidth (( GLfloat )this->mPenWidth );
	
	// reset the point size
	this->mPointSize = 1.0f;
	glPointSize (( GLfloat )this->mPointSize );
	
	// reset the scissor rect
	USGfxDevice& device = USGfxDevice::Get ();
	USRect scissorRect = device.GetRect ();
	glScissor (( int )scissorRect.mXMin, ( int )scissorRect.mYMin, ( int )scissorRect.Width (), ( int )scissorRect.Height ());
	this->mScissorRect = scissorRect;
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode () {

	if ( this->mBlendEnabled ) {
		glDisable ( GL_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode ( const USBlendMode& blendMode ) {

	if ( !( this->mBlendEnabled && this->mBlendMode.IsSame ( blendMode ))) {
		glBlendFunc ( blendMode.mSourceFactor, blendMode.mDestFactor );
		glEnable ( GL_BLEND );
		this->mBlendEnabled = true;
		this->mBlendMode = blendMode;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode ( int srcFactor, int dstFactor ) {

	USBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->mPackedColor = color;
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( const USColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->mPenWidth = penWidth;
		glLineWidth (( GLfloat )penWidth );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPointSize ( float pointSize ) {

	if ( this->mPointSize != pointSize ) {
		this->mPointSize = pointSize;
		glPointSize (( GLfloat )pointSize );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPrimType ( u32 primType ) {

	if ( this->mPrimType != primType ) {

		this->Flush ();
		this->mPrimType = primType;

		switch ( primType ) {
		
			case GL_POINTS:
				this->mPrimSize = 1;
				break;
			
			case GL_LINES:
				this->mPrimSize = 2;
				break;
			
			case GL_TRIANGLES:
				this->mPrimSize = 3;
				break;
			
			case GL_LINE_LOOP:
			case GL_LINE_STRIP:
			case GL_TRIANGLE_FAN:
			case GL_TRIANGLE_STRIP:
			default:
				this->mPrimSize = 0;
				break;
		}
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetScissorRect () {

	USGfxDevice& device = USGfxDevice::Get ();
	this->SetScissorRect ( device.GetRect ());
}

//----------------------------------------------------------------//
void USDrawBuffer::SetScissorRect ( const USRect& rect ) {
	
	USRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
	
		this->Flush ();
		glScissor (( int )rect.mXMin, ( int )rect.mYMin, ( int )rect.Width (), ( int )rect.Height ());
		this->mScissorRect = rect;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetUVTransform () {

	this->mUVTransform.Ident ();	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetUVTransform ( const USAffine2D& uvTransform ) {

	this->mUVTransform.Init ( uvTransform );	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVtxTransform () {

	this->mVtxTransform.Ident ();	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVtxTransform ( const USAffine2D& vtxTransform ) {

	this->mVtxTransform.Init ( vtxTransform );
}

//----------------------------------------------------------------//
USDrawBuffer::USDrawBuffer () :
	mVertexPreset ( USVertexFormatMgr::CUSTOM_FORMAT ),
	mVertexColorType ( 0 ),
	mBuffer ( 0 ),
	mSize ( 0 ),
	mTop ( 0 ),
	mPrimTop ( 0 ),
	mPrimType ( GL_POINTS ),
	mPrimSize ( 0 ),
	mPrimCount ( 0 ),
	mMaxPrims ( 0 ),
	mTexture ( 0 ),
	mPackedColor ( 0xffffffff ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mBlendEnabled ( 0 ) {
	
	RTTI_SINGLE ( USDrawBuffer )
	
	this->Reserve ( DEFAULT_BUFFER_SIZE );
	
	this->mVtxTransform.Ident ();
	this->mUVTransform.Ident ();
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
USDrawBuffer::~USDrawBuffer () {

	this->Clear ();
}