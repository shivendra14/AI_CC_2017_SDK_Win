//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridDialogController.h $
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

#ifndef __FREEGRIDDIALOGCONTROLLER_H__
#define __FREEGRIDDIALOGCONTROLLER_H__

#include "FreeGridTransformArt.h"
#include "FreeGridPlugin.h"
#include "SDKPlugPlug.h"
#include "FlashUIController.h"

extern FreeGridPlugin *gPlugin;

class FreeGridParams
{
public:
	FreeGridParams():
		tx(0.0),
		ty(0.0),
		theta(0.0),
		sx(1.0),
		sy(1.0),
		dstPlane(kInvalidGridPlane)
	{
	
	}
	float tx;
	float ty;
	float theta;
	float sx;
	float sy;
	GridPlaneType dstPlane;
};

class FreeGridDialogController : public FlashUIController
{
public:

	FreeGridDialogController(void);

	void Transform(FreeGridOperation freeGridOperation, FreeGridParams fgParams);
	
	AIErr Initialise(AIFilterMessage* pb);
	
	/** Registers the events this plug-in will listen for with PlugPlug.
	 @return error code if error found, or success otherwise.
	 */
	csxs::event::EventErrorCode RegisterCSXSEventListeners();
	
	/** Removes the previously added event listeners from PlugPlug.
	 @return error code if error found, or success otherwise.
	 */
	csxs::event::EventErrorCode RemoveEventListeners();
	
	ASErr SendData();
	
	void SendError();

	void ParseData(const char* eventData);

	inline FreeGridOperation GetFreeGridOperation()
	{
		return fFreeGridOperation;
	}


	inline FreeGridParams GetFreeGridParams()
	{
		return fFreeGridParams;
	}
	
	bool GetPanelReady()
	{
		return fIsPanelReady;
	}
	void SetPanelReady(bool isPanelReady)
	{
		fIsPanelReady = isPanelReady;
	} 
private:

	FreeGridDialogController& operator=(const FreeGridDialogController &rhs);
	
	FreeGridDialogController(const FreeGridDialogController &rhs);
	
	ai::UnicodeString GetElement(ai::UnicodeString srcData, const char* value);
	
	void GetTranslate(ai::UnicodeString srcData, float& xOffset, float& yOffset);
	
	void GetRotate(ai::UnicodeString srcData, float& angle);
	
	void GetScale(ai::UnicodeString srcData, float& size);
	
	void GetMoveTo(ai::UnicodeString srcData, GridPlaneType& plane);

	void InitData();

private:

	FreeGridTransformArt freeGridTransformArt;	

	FreeGridParams fFreeGridParams;

	FreeGridOperation fFreeGridOperation; 

	bool fIsPanelReady;

};

#endif
