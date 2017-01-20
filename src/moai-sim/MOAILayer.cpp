// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clear
	@text	Remove all props from the layer's partition.
	
	@in		MOAILayer self
	@out	nil
*/
int MOAILayer::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	if ( self->mPartition ) {
		self->mPartition->Clear ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->Render ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getCamera
	@text	Get the camera associated with the layer.
	
	@in		MOAILayer self
	@out	MOAICamera camera
*/
int MOAILayer::_getCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	state.Push (( MOAILuaObject* )self->mCamera );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFitting
	@text	Computes a camera fitting for a given world rect along with
			an optional screen space padding. To do a fitting, compute
			the world rect based on whatever you are fitting to, use
			this method to get the fitting, then animate the camera
			to match.
	
	@in		MOAILayer self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number xPad
	@opt	number yPad
	@out	number x		X center of fitting (use for camera location).
	@out	number y		Y center of fitting (use for camera location).
	@out	number s		Scale of fitting (use for camera scale).
*/
int MOAILayer::_getFitting ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNNNN" )

	ZLRect worldRect;
	worldRect.mXMin = state.GetValue < float >( 2, 0.0f );
	worldRect.mYMin = state.GetValue < float >( 3, 0.0f );
	worldRect.mXMax = state.GetValue < float >( 4, 0.0f );
	worldRect.mYMax = state.GetValue < float >( 5, 0.0f );

	worldRect.Bless ();

	float hPad = state.GetValue < float >( 6, 0.0f );
	float vPad = state.GetValue < float >( 7, 0.0f );

	float x = worldRect.mXMin + (( worldRect.mXMax - worldRect.mXMin ) * 0.5f );
	float y = worldRect.mYMin + (( worldRect.mYMax - worldRect.mYMin ) * 0.5f );

	lua_pushnumber ( state, x );
	lua_pushnumber ( state, y );

	float fitting = self->GetFitting ( worldRect, hPad, vPad );
	lua_pushnumber ( state, fitting );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getFitting3D
	@text	Find a position for the camera where all given locations or
			props will be visible without changing the camera's orientation
			(i.e. orient the camera first, then call this to derive the
			correct position).
	
	@in		MOAILayer self
	@in		table targets		A table of either props or locations. Locations are tables containing {x, y, z, r}.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILayer::_getFitting3D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UT" )

	if (( !self->mViewport ) || ( !self->mCamera ) || ( self->mCamera->GetType () != MOAICamera::CAMERA_TYPE_3D )) return 0;
	
	ZLRect fitRect = state.GetValue < ZLRect >( 3, *self->mViewport );
	
	self->mCamera->ForceUpdate ();
	
	ZLFrustumFitter fitter;
	
	fitter.Init (
		*self->mViewport,
		fitRect,
		self->mCamera->GetFieldOfView (),
		self->mCamera->GetLocalToWorldMtx ()
	);

	u32 itr = state.PushTableItr ( 2 );
	while ( state.TableItrNext ( itr )) {
	
		int type = lua_type ( state, -1 );
		
		switch ( type ) {
		
			case LUA_TTABLE: {
			
				ZLVec3D loc;
				
				loc.mX = state.GetFieldValue < float >( -1, "x", 0.0f );
				loc.mY = state.GetFieldValue < float >( -1, "y", 0.0f );
				loc.mZ = state.GetFieldValue < float >( -1, "z", 0.0f );
				
				float r = state.GetFieldValue < float >( -1, "r", 0.0f );
				
				fitter.FitPoint( loc, r );
				
				break;
			}
			
			case LUA_TUSERDATA: {
			
				MOAIPartitionHull* hull = state.GetLuaObject < MOAIPartitionHull >( -1, true );
		
				if ( hull ) {
					ZLBox bounds = hull->GetBounds ();
					
					ZLVec3D center;
					bounds.GetCenter ( center );
					fitter.FitBox ( bounds, 0.0f );
				}
				break;
			}
		}
	}
	
	ZLVec3D position = fitter.GetPosition ();
	
	state.Push ( position.mX );
	state.Push ( position.mY );
	state.Push ( position.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getPartition
	@text	Returns the partition currently attached to this layer.
	
	@in		MOAILayer self
	@out	MOAIPartition partition
*/
int	MOAILayer::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->AffirmPartition ();
	self->mPartition->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getPropViewList
	@text	Return a list of props gathered and sorted by layer.
	
	@in		MOAILayer self
	@opt	number sortMode					Default is layer's current value.
	@opt	boolean sortInViewSpace			Default is layer's current value.
	@opt	number xSortScale				Default is layer's current value.
	@opt	number ySortScale				Default is layer's current value.
	@opt	number zSortScale				Default is layer's current value.
	@opt	number pSortScale				Priority sort scale. Default is layer's current value.
	@out	...								Gathered props.
*/
int	MOAILayer::_getPropViewList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	if ( self->mPartition && self->mViewport ) {
		
		u32 interfaceMask = self->mPartition->GetInterfaceMask < MOAIGraphicsProp >();
		if ( !interfaceMask ) return 0;
		
		float sortScale [ 4 ];
		
		u32 sortMode			= state.GetValue < u32 >( 2, self->mSortMode );
		bool sortInViewSpace	= state.GetValue < bool >( 3, self->mSortInViewSpace );
		
		sortScale [ 0 ]			= state.GetValue < float >( 4, self->mSortScale [ 0 ]);
		sortScale [ 1 ]			= state.GetValue < float >( 5, self->mSortScale [ 1 ]);
		sortScale [ 2 ]			= state.GetValue < float >( 6, self->mSortScale [ 2 ]);
		sortScale [ 3 ]			= state.GetValue < float >( 7, self->mSortScale [ 3 ]);
		
		ZLMatrix4x4 viewMtx = MOAIViewProj::GetViewMtx ( self->mCamera, self->mParallax );
		ZLMatrix4x4 invViewProjMtx = viewMtx;
		invViewProjMtx.Append ( MOAIViewProj::GetProjectionMtx ( self->mViewport, self->mCamera ));
		invViewProjMtx.Inverse ();
	
		ZLFrustum viewVolume;
		viewVolume.Init ( invViewProjMtx );
		
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		
		u32 totalResults = 0;
		
		if ( self->mPartitionCull2D ) {
			totalResults = self->mPartition->GatherHulls ( buffer, 0, viewVolume.mAABB, interfaceMask );
		}
		else {
			totalResults = self->mPartition->GatherHulls ( buffer, 0, viewVolume, interfaceMask );
		}
		
		if ( !totalResults ) return 0;
		
		if ( sortInViewSpace ) {
			buffer.Transform ( viewMtx, false );
		}
		
		buffer.GenerateKeys (
			sortMode,
			sortScale [ 0 ],
			sortScale [ 1 ],
			sortScale [ 2 ],
			sortScale [ 3 ]
		);
		
		buffer.Sort ( self->mSortMode );
	
		buffer.PushHulls ( L );
		return totalResults;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getSortMode
	@text	Get the sort mode for rendering.
	
	@in		MOAILayer self
	@out	number sortMode
*/
int MOAILayer::_getSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	lua_pushnumber ( state, self->mSortMode );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getSortScale
	@text	Return the scalar applied to axis sorts.
	
	@in		MOAILayer self
	@out	number x
	@out	number y
	@out	number priority
*/
int	MOAILayer::_getSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	lua_pushnumber ( state, self->mSortScale [ 0 ]);
	lua_pushnumber ( state, self->mSortScale [ 1 ]);
	lua_pushnumber ( state, self->mSortScale [ 3 ]);

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getViewport
	@text	Return the viewport currently associated with the layer.
	
	@in		MOAILayer self
	@out	MOAILuaObject viewport
*/
int MOAILayer::_getViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	state.Push (( MOAILuaObject* )self->mViewport );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	insertProp
	@text	Adds a hull to the layer's partition.
	
	@in		MOAILayer self
	@in		MOAIPartitionHull hull
	@out	nil
*/
int	MOAILayer::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	MOAIPartitionHull* hull = state.GetLuaObject < MOAIPartitionHull >( 2, true );
	if ( !hull ) return 0;
	if ( hull == self ) return 0;

	self->AffirmPartition ();
	self->mPartition->InsertHull ( *hull );
	hull->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	removeProp
	@text	Removes a hull from the layer's partition.
	
	@in		MOAILayer self
	@in		MOAIPartitionHull hull
	@out	nil
*/
int	MOAILayer::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	MOAIPartitionHull* hull = state.GetLuaObject < MOAIPartitionHull >( 2, true );
	if ( !hull ) return 0;
	if ( hull == self ) return 0;

	if ( self->mPartition ) {
		self->mPartition->RemoveHull ( *hull );
		hull->ScheduleUpdate ();
	}

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setDebugCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mDebugCamera.Set ( *self, state.GetLuaObject < MOAICamera >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCamera
	@text	Sets a camera for the layer. If no camera is supplied,
			layer will render using the identity matrix as view/proj.
	
	@overload
	
		@in		MOAILayer self
		@opt	MOAICamera camera		Default value is nil.
		@out	nil
	
	@overload
	
		@in		MOAILayer self
		@opt	MOAICamera2D camera		Default value is nil.
		@out	nil
*/
int MOAILayer::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mCamera.Set ( *self, state.GetLuaObject < MOAICamera >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setOverlayTable
	@text	Set or clear the table of renderables to be drawn on
			top of the layer.
	
	@in		MOAILayer self
	@opt	table overlayTable
	@out	nil
*/
int MOAILayer::_setOverlayTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mOverlayTable.SetRef ( *self, state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setParallax
	@text	Sets the parallax scale for this layer. This is simply a
			scalar applied to the view transform before rendering.
	
	@in		MOAILayer self
	@opt	number xParallax	Default value is 1.
	@opt	number yParallax	Default value is 1.
	@opt	number zParallax	Default value is 1.
	@out	nil
*/
int MOAILayer::_setParallax ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mParallax.mX = state.GetValue < float >( 2, 1.0f );
	self->mParallax.mY = state.GetValue < float >( 3, 1.0f );
	self->mParallax.mZ = state.GetValue < float >( 4, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPartition
	@text	Sets a partition for the layer to use. The layer will automatically
			create a partition when the first hull is added if no partition
			has been set.
	
	@in		MOAILayer self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAILayer::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPartitionCull2D
	@text	Enables 2D partition cull (projection of frustum AABB will
			be used instead of AABB or frustum).
	
	@in		MOAILayer self
	@in		boolean partitionCull2D		Default value is false.
	@out	nil
*/
int	MOAILayer::_setPartitionCull2D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mPartitionCull2D = state.GetValue < bool >( 2, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSortMode
	@text	Set the sort mode for rendering.
	
	@in		MOAILayer self
	@in		number sortMode				One of MOAILayer.SORT_NONE, MOAILayer.SORT_PRIORITY_ASCENDING,
										MOAILayer.SORT_PRIORITY_DESCENDING, MOAILayer.SORT_X_ASCENDING,
										MOAILayer.SORT_X_DESCENDING, MOAILayer.SORT_Y_ASCENDING,
										MOAILayer.SORT_Y_DESCENDING, MOAILayer.SORT_Z_ASCENDING,
										MOAILayer.SORT_Z_DESCENDING
	@in		boolean sortInViewSpace		Default value is 'false'.
	@out	nil
*/
int MOAILayer::_setSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mSortMode			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	self->mSortInViewSpace	= state.GetValue < bool >( 3, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSortScale
	@text	Set the scalar applied to axis sorts.
	
	@in		MOAILayer self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@opt	number priority		Default value is 1.
	@out	nil
*/
int	MOAILayer::_setSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mSortScale [ 0 ] = state.GetValue < float >( 2, 0.0f );
	self->mSortScale [ 1 ] = state.GetValue < float >( 3, 0.0f );
	self->mSortScale [ 2 ] = state.GetValue < float >( 4, 0.0f );
	self->mSortScale [ 3 ] = state.GetValue < float >( 5, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setOverlayTable
	@text	Set or clear the table of renderables to be drawn behind
			the layer.
	
	@in		MOAILayer self
	@opt	table underlayTable
	@out	nil
*/
int MOAILayer::_setUnderlayTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mUnderlayTable.SetRef ( *self, state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setViewport
	@text	Set the layer's viewport.
	
	@in		MOAILayer self
	@in		MOAIViewport viewport
	@out	nil
*/
int MOAILayer::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	self->mViewport.Set ( *self, state.GetLuaObject < MOAIViewport >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showDebugLines
	@text	Display debug lines for props in this layer.
	
	@in		MOAILayer self
	@opt	boolean showDebugLines		Default value is 'true'.
	@out	nil
*/
int	MOAILayer::_showDebugLines ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mShowDebugLines = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	wndToWorld
	@text	Project a point from window space into world space.
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILayer::_wndToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

	ZLMatrix4x4 worldToWnd = self->GetWorldToWndMtx ();

	ZLMatrix4x4 wndToWorld = worldToWnd;
	wndToWorld.Inverse ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = worldToWnd.m [ ZLMatrix4x4::C3_R2 ] / worldToWnd.m [ ZLMatrix4x4::C3_R3 ];
	loc.mW = 1.0f;

	if ( self->mCamera && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		wndToWorld.Project ( loc );
	}
	else {
		wndToWorld.Transform ( loc );
	}

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	wndToWorldRay
	@text	Project a point from window space into world space and return
			a normal vector representing a ray cast from the point into
			the world away from the camera (suitable for 3D picking).
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@in		number d	If non-zero, scale normal by dist to plane d units away from camera. Default is zero.
	@out	number x
	@out	number y
	@out	number z
	@out	number xn
	@out	number yn
	@out	number zn
*/
int MOAILayer::_wndToWorldRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

	if ( self->mCamera ) {
		self->mCamera->ForceUpdate ();
	}

	ZLMatrix4x4 wndToWorld = self->GetWndToWorldMtx ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = 0.0f;
	loc.mW = 1.0f;

	float d = state.GetValue < float >( 4, 0.0f );

	ZLVec4D origin;

	if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		const ZLAffine3D& localToWorldMtx = self->mCamera->GetLocalToWorldMtx ();
		ZLVec3D cameraLoc = localToWorldMtx.GetTranslation ();
		origin.mX = cameraLoc.mX;
		origin.mY = cameraLoc.mY;
		origin.mZ = cameraLoc.mZ;
	}
	else {
		origin = loc;
		wndToWorld.Project ( origin );
	}
	
	lua_pushnumber ( state, origin.mX );
	lua_pushnumber ( state, origin.mY );
	lua_pushnumber ( state, origin.mZ );

	ZLVec3D norm;

	if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
	
		wndToWorld.Project ( loc );
	
		norm.mX = loc.mX - origin.mX;
		norm.mY = loc.mY - origin.mY;
		norm.mZ = loc.mZ - origin.mZ;
		norm.Norm ();
	}
	else {
		
		norm.mX = 0.0f;
		norm.mY = 0.0f;
		norm.mZ = -1.0f;
	}

	float ns = 1.0f;
	
	if ( d != 0.0f ) {
	
		if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
			const ZLAffine3D& localToWorldMtx = self->mCamera->GetLocalToWorldMtx ();
			ZLVec3D zAxis = localToWorldMtx.GetZAxis ();
			ns = -( d * zAxis.Dot ( norm ));
		}
		else {
			ns = d;
		}
	}
	
	lua_pushnumber ( state, norm.mX * ns );
	lua_pushnumber ( state, norm.mY * ns );
	lua_pushnumber ( state, norm.mZ * ns );

	return 6;
}

//----------------------------------------------------------------//
/**	@lua	worldToWnd
	@text	Transform a point from world space to window space.
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@in		number Z
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILayer::_worldToWnd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );
	loc.mW = 1.0f;

	ZLMatrix4x4 worldToWnd = self->GetWorldToWndMtx ();
	worldToWnd.Project ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//================================================================//
// MOAILayer
//================================================================//

//----------------------------------------------------------------//
void MOAILayer::AffirmPartition () {

	if ( !this->mPartition ) {
		this->mPartition.Set ( *this, new MOAIPartition ());
	}
}

//----------------------------------------------------------------//
void MOAILayer::DrawPartition ( MOAIPartition& partition ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 interfaceMask = partition.GetInterfaceMask < MOAIDrawable >();
	if ( !interfaceMask ) return;
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	const ZLFrustum& viewVolume = gfxMgr.mGfxState.GetViewVolume ();
	
	u32 totalResults = 0;
	
	if ( this->mPartitionCull2D ) {
		totalResults = partition.GatherHulls ( buffer, 0, viewVolume.mAABB, interfaceMask );
	}
	else {
		totalResults = partition.GatherHulls ( buffer, 0, viewVolume, interfaceMask );
	}
	
	if ( !totalResults ) return;
	
	if ( this->mSortInViewSpace ) {
		buffer.Transform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_VIEW_MTX ), false );
	}
	
	buffer.GenerateKeys (
		this->mSortMode,
		this->mSortScale [ 0 ],
		this->mSortScale [ 1 ],
		this->mSortScale [ 2 ],
		this->mSortScale [ 3 ]
	);
	
	buffer.Sort ( this->mSortMode );
	
	MOAIMaterialStackMgr& materialStack = MOAIMaterialStackMgr::Get ();
	materialStack.Push ( this->GetMaterial ());
	
	this->DrawProps ( buffer );
	
	materialStack.Pop ();
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		
		// clear the ambient color and bind vector drawing
		//gfxMgr.mGfxState.SetAmbientColor ( 1.0f, 1.0f, 1.0f, 1.0f );
		//MOAIDraw::Get ().Bind ();
		this->DrawPropsDebug ( buffer );
		if ( this->mCamera ) {
			this->mCamera->DrawDebug ();
		}
	}
}

//----------------------------------------------------------------//
void MOAILayer::DrawProps ( MOAIPartitionResultBuffer& buffer ) {

	u32 totalResults = buffer.GetTotalResults ();

	for ( u32 i = 0; i < totalResults; ++i ) {
		MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
		MOAIDrawable* drawable = result->AsType < MOAIDrawable >();
		drawable->Draw ( result->mSubPrimID );
	}
}

//----------------------------------------------------------------//
void MOAILayer::DrawPropsDebug ( MOAIPartitionResultBuffer& buffer ) {

	u32 totalResults = buffer.GetTotalResults ();

	for ( u32 i = 0; i < totalResults; ++i ) {
		MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
		MOAIDrawable* drawable = result->AsType < MOAIDrawable >();
		drawable->DrawDebug ( result->mSubPrimID );
	}
}

//----------------------------------------------------------------//
float MOAILayer::GetFitting ( ZLRect& worldRect, float hPad, float vPad ) {

	if ( !( this->mCamera && this->mViewport )) return 1.0f;

	ZLRect viewRect = this->mViewport->GetRect ();
	
	float hFit = ( viewRect.Width () - ( hPad * 2 )) / worldRect.Width ();
	float vFit = ( viewRect.Height () - ( vPad * 2 )) / worldRect.Height ();
	
	return ( hFit < vFit ) ? hFit : vFit;
}

//----------------------------------------------------------------//
MOAIPartition* MOAILayer::GetPartition () {

	this->AffirmPartition ();
	return this->mPartition;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetWndToWorldMtx () const {

	return MOAIViewProj::GetWndToWorldMtx ( this->mViewport, this->mCamera, this->mLocalToWorldMtx, this->mParallax );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetWorldToWndMtx () const {

	return MOAIViewProj::GetWorldToWndMtx ( this->mViewport, this->mCamera, this->mLocalToWorldMtx, this->mParallax );
}

//----------------------------------------------------------------//
MOAILayer::MOAILayer () :
	mParallax ( 1.0f, 1.0f, 1.0f ),
	mShowDebugLines ( true ),
	mSortMode ( MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING ),
	mSortInViewSpace ( false ),
	mPartitionCull2D ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIRenderPassBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILayer::~MOAILayer () {

	this->mCamera.Set ( *this, 0 );
	this->mDebugCamera.Set ( *this, 0 );
	this->mViewport.Set ( *this, 0 );
	this->mPartition.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaClass ( state );
	MOAIRenderPassBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "SORT_NONE",						( u32 )MOAIPartitionResultBuffer::SORT_NONE );
	state.SetField ( -1, "SORT_ISO",						( u32 )MOAIPartitionResultBuffer::SORT_ISO );
	state.SetField ( -1, "SORT_PRIORITY_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	state.SetField ( -1, "SORT_PRIORITY_DESCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_DESCENDING );
	state.SetField ( -1, "SORT_X_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_X_ASCENDING );
	state.SetField ( -1, "SORT_X_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_X_DESCENDING );
	state.SetField ( -1, "SORT_Y_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Y_ASCENDING );
	state.SetField ( -1, "SORT_Y_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Y_DESCENDING );
	state.SetField ( -1, "SORT_Z_ASCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Z_ASCENDING );
	state.SetField ( -1, "SORT_Z_DESCENDING",				( u32 )MOAIPartitionResultBuffer::SORT_Z_DESCENDING );
	state.SetField ( -1, "SORT_VECTOR_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_ASCENDING );
	state.SetField ( -1, "SORT_VECTOR_DESCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_DESCENDING );
	state.SetField ( -1, "SORT_DIST_SQUARED_ASCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_DIST_SQUARED_ASCENDING );
	state.SetField ( -1, "SORT_DIST_SQUARED_DESCENDING",	( u32 )MOAIPartitionResultBuffer::SORT_DIST_SQUARED_DESCENDING );
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIGraphicsProp::RegisterLuaFuncs ( state );
	MOAIRenderPassBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "draw",					_draw },
		{ "getCamera",				_getCamera },
		{ "getFitting",				_getFitting },
		{ "getFitting3D",			_getFitting3D },
		{ "getPartition",			_getPartition },
		{ "getPropViewList",		_getPropViewList },
		{ "getSortMode",			_getSortMode },
		{ "getSortScale",			_getSortScale },
		{ "getViewport",			_getViewport },
		{ "insertProp",				_insertProp },
		{ "removeProp",				_removeProp },
		{ "setDebugCamera",			_setDebugCamera },
		{ "setCamera",				_setCamera },
		{ "setOverlayTable",		_setOverlayTable },
		{ "setParallax",			_setParallax },
		{ "setPartition",			_setPartition },
		{ "setPartitionCull2D",		_setPartitionCull2D },
		{ "setSortMode",			_setSortMode },
		{ "setSortScale",			_setSortScale },
		{ "setUnderlayTable",		_setUnderlayTable },
		{ "setViewport",			_setViewport },
		{ "showDebugLines",			_showDebugLines },
		{ "wndToWorld",				_wndToWorld },
		{ "wndToWorldRay",			_wndToWorldRay },
		{ "worldToWnd",				_worldToWnd },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILayer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIGraphicsProp::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAILayer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGraphicsProp::SerializeOut ( state, serializer );
}

//================================================================//
// MOAILayer virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILayer::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
    
   	if ( !this->IsVisible ()) return;
	if ( !this->mViewport ) return;
	if ( this->IsClear ()) return;
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	gfxMgr.mGfxState.SetFrameBuffer ( this->GetFrameBuffer ());
	
	MOAIViewport& viewport = *this->mViewport;
	ZLRect viewportRect = viewport;

	ZLMatrix4x4 mtx ( this->mLocalToWorldMtx );
	mtx.Transform ( viewportRect );

	gfxMgr.mGfxState.SetViewRect ( viewportRect );
	gfxMgr.mGfxState.SetScissorRect ( viewportRect );
	
	this->ClearSurface ();
	
	gfxMgr.mGfxState.SetViewProj ( this->mViewport, this->mCamera, this->mParallax );
	gfxMgr.mGfxState.SetDebug ( this->mViewport, this->mDebugCamera );
	gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );
	
	// set up the ambient color
	gfxMgr.mGfxState.SetAmbientColor ( this->mColor );
	
	MOAIDrawable::Draw ( this->mUnderlayTable );
	
	if ( this->mPartition ) {
		this->DrawPartition ( *this->mPartition );
	}
	
	MOAIDrawable::Draw ( this->mOverlayTable );
	
	gfxMgr.mGfxState.SetFrameBuffer ( this->GetFrameBuffer ());
}

//----------------------------------------------------------------//
u32 MOAILayer::MOAIPartitionHull_GetModelBounds ( ZLBox& bounds ) {
	
	if ( this->mViewport ) {
		ZLRect frame = this->mViewport->GetRect ();
		bounds.Init ( frame.mXMin, frame.mYMax, frame.mXMax, frame.mYMin, 0.0f, 0.0f );
		return MOAIPartitionHull::BOUNDS_OK;
	}
	return MOAIPartitionHull::BOUNDS_EMPTY;
}
