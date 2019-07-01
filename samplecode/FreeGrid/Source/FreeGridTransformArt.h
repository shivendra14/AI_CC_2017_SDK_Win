//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridTransformArt.h $
//
//  $Revision: #1 $
//
//  Copyright 2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#pragma once

#ifndef __FREEGRIDTRANSFORMART_H__
#define __FREEGRIDTRANSFORMART_H__

#include "FreeGridSuites.h"

enum FreeGridOperation 
{
	FreeGrid_translate =0,
	FreeGrid_scale,
	FreeGrid_rotate,
	FreeGrid_changePlane = 3
};

class FreeGridTransformArt
{
public:
	FreeGridTransformArt();
	
	~FreeGridTransformArt(void);

	AIErr Transform(FreeGridOperation freeGridOperation, AIReal tx, AIReal ty, AIReal theta, AIReal sx, GridPlaneType dstPlane);

private:
	FreeGridTransformArt& operator=(const FreeGridTransformArt& rhs);
	
	FreeGridTransformArt(const FreeGridTransformArt& rhs);

	ASErr GetSelection(AIArtHandle ***matches, ai::int32 *numMatches);

	AIErr ChangePlane(AIArtHandle art, GridPlaneType dstPlane);

	ASErr TransformWithParameters(AIArtHandle art, AIReal tx, AIReal ty, AIReal theta, AIReal sx, AIReal sy) ;

};

#endif
