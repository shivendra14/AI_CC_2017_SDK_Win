//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridDialogController.cpp $
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
#include "AppContext.hpp"
#include "FreeGridDialogController.h"
#include "SDKErrors.h"
#include "FreeGridSuites.h"

#define FREEGRID_UI_EXTENSION	"com.adobe.illustrator.freegridui.panel"

#define EVENT_TYPE_OK_CLICKED		"com.adobe.csxs.events.OkClicked"
#define EVENT_TYPE_PANEL_READY		"com.adobe.csxs.events.PanelReady"

static void OkClickedFunc(const csxs::event::Event* const eventParam, void* const context)
{	
	FreeGridDialogController *freeGridController = (FreeGridDialogController *)context;
	if(NULL == freeGridController || eventParam == NULL)
		return;
	freeGridController->ParseData(eventParam->data);
	
	do {
		// Set up the application context, so that suite calls can work.
		AppContext appContext(gPlugin->GetPluginRef());
		ai::UnicodeString undoText("Undo Free Grid ");
		ai::UnicodeString redoText("Redo Free Grid ");
		switch(freeGridController->GetFreeGridOperation())
		{
			case FreeGrid_translate: 
				undoText.append(ai::UnicodeString("Translate"));
				redoText.append(ai::UnicodeString("Translate"));	
			break;
			case FreeGrid_scale:
				undoText.append(ai::UnicodeString("Scale"));
				redoText.append(ai::UnicodeString("Scale"));
			break;
			case FreeGrid_rotate: 
				undoText.append(ai::UnicodeString("Rotate"));
				redoText.append(ai::UnicodeString("Rotate"));
			break; 
			case FreeGrid_changePlane: 
				undoText.append(ai::UnicodeString("Change Plane"));
				redoText.append(ai::UnicodeString("Change Plane"));
			break; 
			default:
			break;
		}
		sAIUndo->SetUndoTextUS(undoText, redoText);
		freeGridController->Transform(freeGridController->GetFreeGridOperation(), freeGridController->GetFreeGridParams());
		// Clean up the application context and return.
	} while(false);
	
}

static void PanelReadyFunc(const csxs::event::Event* const eventParam, void* const context)
{

	FreeGridDialogController *freeGridController = (FreeGridDialogController *)context;
	do {
		AppContext appContext(gPlugin->GetPluginRef());
		freeGridController->SendData();
		freeGridController->SetPanelReady(true);
	}while(false);
	
}

FreeGridDialogController::FreeGridDialogController(void)
: FlashUIController(FREEGRID_UI_EXTENSION),
fFreeGridOperation(FreeGrid_changePlane),
fIsPanelReady(false)
{
}

/* Add event listeners
*/
csxs::event::EventErrorCode FreeGridDialogController::RegisterCSXSEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	do {
		result =  fPPLib.AddEventListener(EVENT_TYPE_OK_CLICKED, OkClickedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}
	
		result =  fPPLib.AddEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) {
			break;
		}
	}
	while(false);
	return result;
}

/* Remove event listeners
*/
csxs::event::EventErrorCode FreeGridDialogController::RemoveEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	do {
		result =  fPPLib.RemoveEventListener(EVENT_TYPE_OK_CLICKED, OkClickedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{		
			break;
		}
		result =  fPPLib.RemoveEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}
	}
	while(false);
	return result;
}

void FreeGridDialogController::InitData()
{
	fFreeGridOperation = FreeGrid_changePlane;
	fFreeGridParams.tx = 0.0;
	fFreeGridParams.ty = 0.0;
	fFreeGridParams.theta = 0.0;
	fFreeGridParams.sx = 1.0;
	fFreeGridParams.sy = 1.0;
	fFreeGridParams.dstPlane = kInvalidGridPlane;
}
/* Parse received data from flash panel.
*/
void FreeGridDialogController::ParseData(const char* eventData)
{
	InitData();
	ai::UnicodeString dataStr(eventData);
	//// Get index of start of payload value string
	ai::UnicodeString openStr("<operation>");
	ai::UnicodeString::size_type openIndex = dataStr.find(openStr);
	openIndex = openIndex + openStr.length();

	//// Get index of end of payload value string
	ai::UnicodeString closeStr("</operation>");
	ai::UnicodeString::size_type closeIndex = dataStr.find(closeStr);	
	// Get new art value
	ai::UnicodeString::size_type strLength = closeIndex - openIndex;

	if (strLength > 0) 
	{
		ai::UnicodeString operationStr = dataStr.substr(openIndex, strLength);
		ai::NumberFormat numFormat;
		ASInt32 nOperation = 0;
		sAIStringFormatUtils->StringToInteger(numFormat, operationStr, nOperation);
		fFreeGridOperation = (FreeGridOperation)nOperation;
	}
	switch(fFreeGridOperation)
	{ 
		case FreeGrid_translate: 
			GetTranslate(dataStr, fFreeGridParams.tx, fFreeGridParams.ty);
		break;
		case FreeGrid_scale:
			GetScale(dataStr, fFreeGridParams.sx);
		break;
		case FreeGrid_rotate: 
			GetRotate(dataStr, fFreeGridParams.theta);
		break;
		case FreeGrid_changePlane: 
			GetMoveTo(dataStr, fFreeGridParams.dstPlane);
		break;
		default:
		break;
	}		
}

void FreeGridDialogController::SendError()
{
	std::string xmlString = "<payload><error>OutOfBounds</error></payload>";
	csxs::event::Event event = {"com.adobe.csxs.events.Error",
								 csxs::event::kEventScope_Application,
								 "Error",
								 NULL,
								 xmlString.c_str()};
	fPPLib.DispatchEvent(&event);
}

ASErr FreeGridDialogController::SendData()
{
	ASErr error = kNoErr;
	PerspectiveGridType perspectiveGridType = kTwoPointGridType;
	error = sAIPerspectiveGrid->GetActiveGridType(perspectiveGridType);
	ai::NumberFormat numFormat;
	// Get operation value
	ai::UnicodeString valueUString;
	error = sAIStringFormatUtils->IntegerToString(numFormat, (ASInt32)perspectiveGridType, valueUString);

	std::string operationTypeCStr = valueUString.as_Platform();
	// Construct XML string
	std::string xmlString = "<payload><gridtype>" + operationTypeCStr + "</gridtype></payload>";
	
	// Create and dispatch event
	csxs::event::Event event = {"com.adobe.csxs.events.UpdatePanel",
								 csxs::event::kEventScope_Application,
								 "UpdatePanel",
								 NULL,
								 xmlString.c_str()};
	fPPLib.DispatchEvent(&event);

	return error;
}

void FreeGridDialogController::Transform(FreeGridOperation freeGridOperation, FreeGridParams fgParams)
{
	AIErr error = freeGridTransformArt.Transform(freeGridOperation, fgParams.tx, fgParams.ty, 
		fgParams.theta, fgParams.sx, fgParams.dstPlane);
	if(error){
		SendError();
	}
}

ai::UnicodeString FreeGridDialogController::GetElement(ai::UnicodeString srcData, const char* value)
{
	ai::UnicodeString openStr("<");
	ai::UnicodeString contentStr(value);
	ai::UnicodeString endStr(">");
	openStr.append(contentStr);
	openStr.append(endStr);
	ai::UnicodeString::size_type openIndex = srcData.find(openStr);
	openIndex = openIndex + openStr.length();
	ai::UnicodeString strValue("");

    ai::UnicodeString colseStr("</");
	colseStr.append(contentStr);
	colseStr.append(endStr);
	ai::UnicodeString::size_type closeIndex = srcData.find(colseStr);
	ai::UnicodeString::size_type strLength = closeIndex - openIndex;
	if (strLength > 0)
	{
		strValue = srcData.substr(openIndex, strLength);
	}
	
	return strValue;
	
}

void FreeGridDialogController::GetTranslate(ai::UnicodeString srcData, float& xOffset, float& yOffset)
{
	if(GetElement(srcData, "translate").empty())
		return;
	else
	{
		ai::NumberFormat numFormat;
		ai::UnicodeString contentV(GetElement(srcData, "v"));
		sAIStringFormatUtils->StringToReal(numFormat, contentV, xOffset);
		ai::UnicodeString contentH(GetElement(srcData, "h"));
		sAIStringFormatUtils->StringToReal(numFormat, contentH, yOffset);
	}
}

void FreeGridDialogController::GetRotate(ai::UnicodeString srcData, float& angle)
{
	if(GetElement(srcData, "rotate").empty())
		return;
	else
	{
		ai::NumberFormat numFormat;
		ai::UnicodeString contentAngle(GetElement(srcData, "angle"));
		sAIStringFormatUtils->StringToReal(numFormat, contentAngle, angle);
	}
}

void FreeGridDialogController::GetScale(ai::UnicodeString srcData, float& size)
{
	if(GetElement(srcData, "scale").empty())
		return;
	else
	{
		ai::NumberFormat numFormat;
		ai::UnicodeString contentSize(GetElement(srcData, "size"));
		sAIStringFormatUtils->StringToReal(numFormat, contentSize, size);
		float scale = 100.0;
		size= size / scale;
	}
}

void FreeGridDialogController::GetMoveTo(ai::UnicodeString srcData, GridPlaneType& plane)
{
	if(GetElement(srcData, "moveto").empty())
		return;
	else
	{
		ai::UnicodeString contentPlane(GetElement(srcData, "plane"));
		
		ai::UnicodeString LeftPlane("LeftPlane");
		ai::UnicodeString RightPlane("RightPlane");
		ai::UnicodeString FloorPlane("FloorPlane");
		if(contentPlane.compare(LeftPlane)==0)
		{
			plane = kGridLeftPlane;
		}
		else if(contentPlane.compare(FloorPlane)==0)
		{
			plane = kGridFloorPlane;
		}
		else
		{
			plane = kGridRightPlane;
		}
	}
}
