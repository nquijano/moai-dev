// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/ZLFrameBuffer.h>

//================================================================//
// ZLFrameBuffer
//================================================================//

//----------------------------------------------------------------//
void ZLFrameBuffer::AffirmBuffers () {

	this->ZLFrameBuffer_AffirmBuffers ();
}

//----------------------------------------------------------------//
void ZLFrameBuffer::DetectGLFrameBufferID () {

	this->SetGLFrameBuffer ( MOAIGfxMgr::GetDrawingAPI ().GetCurrentFramebuffer ());
}

//----------------------------------------------------------------//
ZLRect ZLFrameBuffer::GetBufferRect () const {

	ZLRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mBufferWidth;
	rect.mYMax = ( float )this->mBufferHeight;
	
	return rect;
}

//----------------------------------------------------------------//
bool ZLFrameBuffer::NeedsClear () const {

	return this->mNeedsClear;
}

//----------------------------------------------------------------//
void ZLFrameBuffer::NeedsClear ( bool needsClear ) {

	this->mNeedsClear = needsClear;
}

//----------------------------------------------------------------//
//void ZLFrameBuffer::Render () {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	//this->mLastDrawCount = gfxMgr.GetDrawCount ();
//
//	gfxState.SetFrameBuffer ( this );
//	
//	//disable scissor rect for clear
//	gfxState.SetScissorRect ();
//	this->ClearSurface ();
//	
//	if ( this->mRenderTable ) {
//		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//		state.Push ( this->mRenderTable );
//		this->RenderTable ( state, -1 );
//		state.Pop ( 1 );
//	}
//
//	gfxState.FlushVertexCache (); // do we need to do this if we aren't reading pixels?
//
//	// since we're doing this on the render thread, set it every time until we get a callback
//	if ( this->mGrabNextFrame ) {
//
//		// remember, if we're not in immediate mode, this schedules a pixel read for later
//		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
//		gfx.ReadPixels ( 0, 0, this->mBufferWidth, this->mBufferHeight, ZGL_PIXEL_FORMAT_RGBA, ZGL_PIXEL_TYPE_UNSIGNED_BYTE, 4, this, 0 );
//	}
//	
//	this->mRenderCounter++;
//	//this->mLastDrawCount = gfxMgr.GetDrawCount () - this->mLastDrawCount;
//}

//----------------------------------------------------------------//
void ZLFrameBuffer::SetBufferSize ( u32 width, u32 height ) {

	this->mBufferWidth = width;
	this->mBufferHeight = height;
}

//----------------------------------------------------------------//
void ZLFrameBuffer::SetGLFrameBuffer ( const ZLGfxHandle& frameBuffer ){

	ZLGfxResourceClerk::DeleteOrDiscard ( this->mGLFrameBuffer, true );
	this->mGLFrameBuffer = frameBuffer;
}

//----------------------------------------------------------------//
ZLRect ZLFrameBuffer::WndRectToDevice ( ZLRect rect ) const {

	rect.Bless ();

	if ( this->mLandscape ) {
	
		float width = ( float )this->mBufferWidth;
		
		float xMin = rect.mYMin;
		float yMin = width - rect.mXMax;
		float xMax = rect.mYMax;
		float yMax = width - rect.mXMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}
	else {
	
		float height = ( float )this->mBufferHeight;
		
		float xMin = rect.mXMin;
		float yMin = height - rect.mYMax;
		float xMax = rect.mXMax;
		float yMax = height - rect.mYMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}

	rect.Scale ( this->mBufferScale, this->mBufferScale );
	return rect;
}

//----------------------------------------------------------------//
ZLFrameBuffer::ZLFrameBuffer () :
	mBufferWidth ( 0 ),
	mBufferHeight ( 0 ),
	mBufferScale ( 1.0f ),
	mLandscape ( false ),
	mNeedsClear ( true ) {
}

//----------------------------------------------------------------//
ZLFrameBuffer::~ZLFrameBuffer () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void ZLFrameBuffer::ZLFrameBuffer_AffirmBuffers () {
}