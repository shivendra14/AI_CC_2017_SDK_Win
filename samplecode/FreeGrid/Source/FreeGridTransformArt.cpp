//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridTransformArt.cpp $
//
//  $Revision: #2 $
//
//  Copyright 2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================
#include "IllustratorSDK.h"
#include "FreeGridTransformArt.h"

FreeGridTransformArt::FreeGridTransformArt()
{
}

FreeGridTransformArt::~FreeGridTransformArt(void)
{
}

ASErr FreeGridTransformArt::GetSelection(AIArtHandle ***matches, ai::int32 *numMatches)
{
    short types[]={kPathArt, kCompoundPathArt};
	int size = sizeof(types) / sizeof(*types);
	AIMatchingArtSpec *spec = new AIMatchingArtSpec[size];	
	for (int nIndex = 0; nIndex < size; nIndex++)
	{
		spec[nIndex].type = types[nIndex];
		spec[nIndex].whichAttr = kArtFullySelected;
		spec[nIndex].attr = kArtFullySelected;
	}

	ASErr result = sAIMatchingArt->GetMatchingArt(spec, size, matches, numMatches);
	delete []spec;
	if (result)
		return result;
	if (!*numMatches)
		return kCanceledErr;
	return kNoErr;
}

AIErr FreeGridTransformArt::Transform(FreeGridOperation freeGridOperation, AIReal tx, AIReal ty, AIReal theta, AIReal sx, GridPlaneType dstPlane)
{
	AIErr error = kNoErr;
	AIBoolean isPerspectiveOn = (sAIPerspectiveGrid && sAIPerspectiveGrid->IsGridVisible() );
	if(isPerspectiveOn)
	{
		ai::int32 numMatches = 0;
		AIArtHandle** artList = NULL;
		GetSelection(&artList, &numMatches);
		for(ASInt32 i = 0; i < numMatches; ++i)
		{
			AIArtHandle art = (*artList)[i];
			if(sAIPerspectiveTransform->IsArtInPerspective(art))
			{
				switch(freeGridOperation)
				{
					case FreeGrid_translate: 
					case FreeGrid_scale: 
					case FreeGrid_rotate: 
						TransformWithParameters(art, tx, ty, (AIReal) (theta * kAIRealPi / 180.0), sx, sx);
					break; 
					case FreeGrid_changePlane: 
						error = ChangePlane(art, dstPlane);						
					break; 
					default:
					break;
				}	
			}
		}
	}
	return error;
}

AIErr FreeGridTransformArt::ChangePlane(AIArtHandle art, GridPlaneType dstPlane)
{    
    if(sAIPerspectiveTransform->IsArtInPerspective(art))
    {
        GridPlaneType srcPlane;
		sAIPerspectiveTransform->GetObjectPlane(art, &srcPlane);
        AIReal planeOffset=0;
        sAIPerspectiveTransform->GetOriginalArt(art);
        bool success = sAIPerspectiveTransform->PutInPerspective(art, dstPlane, &planeOffset);
        if(!success)
		{
			sAIPerspectiveTransform->PutInPerspective(art, srcPlane, &planeOffset); 
		}
        
    }
	return kNoErr;
}

ASErr FreeGridTransformArt::TransformWithParameters(AIArtHandle art, AIReal tx, AIReal ty, AIReal theta, AIReal sx, AIReal sy) 
{ 
	AIRealMatrix artMatrix;	
	AIRealRect boundsOfArt;
	sAIPerspectiveTransform->GetOriginalArtBounds(art, kWStrokeDependsOnPreference, boundsOfArt);	
    //get center point
    AIRealPoint artCenter;
    artCenter.h = ( boundsOfArt.left + boundsOfArt.right ) / 2;
    artCenter.v = ( boundsOfArt.top + boundsOfArt.bottom ) / 2;
	// move object so that the centerpoint is at the origin
	sAIRealMath->AIRealMatrixSetTranslate(&artMatrix, -artCenter.h, -artCenter.v);
	// translate object by tx and ty
	sAIRealMath->AIRealMatrixConcatTranslate(&artMatrix, tx, ty);
	// rotate object by theta
	sAIRealMath->AIRealMatrixConcatRotate(&artMatrix, theta);
	// scale object by sx and sy
	sAIRealMath->AIRealMatrixConcatScale(&artMatrix, sx, sy);
	// move the object back to original position
	sAIRealMath->AIRealMatrixConcatTranslate(&artMatrix, artCenter.h, artCenter.v);
	return sAIPerspectiveTransform->ApplyTransformInPerspective(art, artMatrix);
}
