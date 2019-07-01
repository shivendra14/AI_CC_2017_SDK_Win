//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/Webter/Source/WebterPanelController.h $
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

#include "WebterPanelController.h"
#include "AppContext.hpp"
#include "SDKErrors.h"
#include "Plugin.hpp"
#include "AIDictionary.h"
#include "AIStringFormatUtils.h"
#include "DictionaryEntryType.h"
#include "DictionaryToXmlConverter.h"

#define WEBTER_UI_EXTENSION				"com.adobe.illustrator.webterui.panel"
#define ILST_APPID						"ILST"

// Events we listen out for
#define EVENT_TYPE_ADDENTRY_CLICKED		"com.adobe.csxs.events.WebterAddEntry"
#define EVENT_TYPE_ITEM_SELECTED		"com.adobe.csxs.events.WebterItemSelect"
#define EVENT_TYPE_PANEL_READY			"com.adobe.csxs.events.WebterPanelReady"

// Events we dispatch
#define EVENT_UPDATE_ENTRY				"com.adobe.csxs.events.WebterUpdateEntry"
#define EVENT_UPDATE_DEFN				"com.adobe.csxs.events.WebterUpdateDefinitionAndType"

static void AddEntryToDictionaryClickedFunc(const csxs::event::Event* const eventParam, void* const context)
{
	WebterPanelController *webterPanelController = (WebterPanelController *)context;
	if ( NULL == webterPanelController || eventParam == NULL )
		return;
	webterPanelController->ParseData(eventParam->data);
	webterPanelController->AddEntryToDictionary();
}

static void ItemSelectedFunc(const csxs::event::Event* const eventParam, void* const context)
{
	WebterPanelController *webterPanelController = (WebterPanelController *)context;
	if ( NULL == webterPanelController || eventParam == NULL )
		return;
	webterPanelController->ParseData(eventParam->data);
	webterPanelController->UpdateDefinitionAndType();
}

static void PanelReadyFunc(const csxs::event::Event* const eventParam, void* const context)
{
	WebterPanelController *webterPanelController = (WebterPanelController *)context;
	if ( NULL == webterPanelController || eventParam == NULL )
		return;
	
	webterPanelController->SendDocDictionaryToPanel();
	webterPanelController->SendArtDictionaryToPanel();
}

WebterPanelController::WebterPanelController(void)
	: FlashUIController(WEBTER_UI_EXTENSION),
	artDictionary(NULL),
	docDictionary(NULL)
{
}

WebterPanelController::~WebterPanelController(void)
{
}

/* Add event listeners
*/
csxs::event::EventErrorCode WebterPanelController::RegisterCSXSEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	do
	{
		result = fPPLib.AddEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
		if (result != csxs::event::kEventErrorCode_Success)
		{
			break;
		}
	
		result =  fPPLib.AddEventListener(EVENT_TYPE_ADDENTRY_CLICKED, AddEntryToDictionaryClickedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}

		result =  fPPLib.AddEventListener(EVENT_TYPE_ITEM_SELECTED, ItemSelectedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}
	}
	while(false);
	return result;
}

/* Remove event listeners
*/
csxs::event::EventErrorCode WebterPanelController::RemoveEventListeners()
{
	csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
	do
	{
		result = fPPLib.RemoveEventListener(EVENT_TYPE_PANEL_READY, PanelReadyFunc, this);
		if (result != csxs::event::kEventErrorCode_Success)
		{
			break;
		}
		
		result =  fPPLib.RemoveEventListener(EVENT_TYPE_ADDENTRY_CLICKED, AddEntryToDictionaryClickedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}

		result =  fPPLib.RemoveEventListener(EVENT_TYPE_ITEM_SELECTED, ItemSelectedFunc, this);
		if (result != csxs::event::kEventErrorCode_Success) 
		{
			break;
		}
	}
	while(false);
	return result;
}

void WebterPanelController::ParseData(const char* eventData)
{
	string srcData(eventData);
	
	ai::NumberFormat numFormat;
	string itemStr = WebterPanelController::GetElement(srcData, "entry");
	if ( !itemStr.empty() )
	{
		ASInt32 temp = atoi(itemStr.c_str());
		itemType = (ItemType)temp;
	}

	string entryStr = WebterPanelController::GetElement(srcData, "entrystring");
	if ( !entryStr.empty() )
	{
		entryString = entryStr;
	}

	string defintionStr = WebterPanelController::GetElement(srcData, "value");
	if ( !defintionStr.empty() )
	{
		stringValue = defintionStr;
	}

	string entryTypeStr = WebterPanelController::GetElement(srcData, "entrytype");
	if ( !entryTypeStr.empty() )
	{
		entryType= atoi(entryTypeStr.c_str());
	}

}

void WebterPanelController::SetDocumentDictionary(AIDictionaryRef documentDictionary)
{
	docDictionary = documentDictionary;
	
	SendDocDictionaryToPanel();
}

void WebterPanelController::SendDocDictionaryToPanel()
{
	DocDictionaryToXmlConverter converter(docDictionary, entryString);
	string payload = converter.GetPayload();
	dispatchDictionaryUpdateEvent(payload);
}

csxs::event::EventErrorCode WebterPanelController::dispatchDictionaryUpdateEvent(const string& payload)
{
	csxs::event::Event event = {EVENT_UPDATE_ENTRY,
		csxs::event::kEventScope_Application,
		ILST_APPID,
		NULL,
		payload.c_str()};
	
	return fPPLib.DispatchEvent(&event);
}

void WebterPanelController::SetArtDictionary(AIDictionaryRef artDictionaryRef, const ai::UnicodeString& artName)
{
	artDictionary = artDictionaryRef;

	ai::UnicodeString::size_type strLength = artName.length();
	if (strLength > 0) 
	{
		displayNameOfSelectedArt = artName.as_Roman();
		
		// Strip '<' & '>' if present
		if(displayNameOfSelectedArt.at(0) == '<')
			displayNameOfSelectedArt = displayNameOfSelectedArt.substr(1,
				displayNameOfSelectedArt.length()-2);
	}
	else
	{
		displayNameOfSelectedArt = "No art selected:";
	}
	
	SendArtDictionaryToPanel();
}

void WebterPanelController::SendArtDictionaryToPanel()
{
	ArtDictionaryToXmlConverter converter(artDictionary, displayNameOfSelectedArt);
	std::string payload = converter.GetPayload();
	dispatchDictionaryUpdateEvent(payload);
}

/* Called when an item is selected. Determines which dictionary to look up,
 * then looks up the selected item, gets a string representation of the entry,
 * if the entry is an integer/real/boolean/string, otherwise gets the type of
 * the entry and converts it to a string.
 * Then it dispatches this information to the flash panel.
 */
void WebterPanelController::UpdateDefinitionAndType()
{
	Dictionary dict(itemType == DOCUMENT ? docDictionary : artDictionary);
	
	DictionaryEntryType typeOfEntry = dict.getTypeOfEntryWithKey(entryString);
	
	string value;
	
	// Now get the value, if we can
	switch (typeOfEntry.type())
	{
		case DDT_INT:
		{
			char IntBuffer[33];
			ai::int32 intValue = dict.getIntegerEntry(entryString);
			sprintf(IntBuffer,"%d",intValue);
			value = IntBuffer;
			break;
		}
			
		case DDT_REAL:
		{
			char Realbuffer[33];
			AIReal realValue = dict.getRealEntry(entryString);
			sprintf(Realbuffer,"%f",realValue);
			value = Realbuffer;			
			break;
		}
			
		case DDT_STRING:
		{
			value = dict.getStringEntry(entryString);
			break;
		}
			
		case DDT_BOOLEAN:
		{
			AIBoolean boolEntry = dict.getBooleanEntry(entryString);
			value = (boolEntry) ? "True" : "False";
			break;
		}
			
		default:
		{
			value = "---";
			break;
		}
	}
	
	AIEntryType type = typeOfEntry.getUnderlyingType();
	
	stringstream payloadStream;
	payloadStream << "<payload><dict>" << ((itemType == DOCUMENT) ? "doc" : "art")
	<< "</dict><key>" << entryString << "</key><value>" << value
	<< "</value><type>" << type << "</type></payload>";
	
	std::string payloadString = payloadStream.str();
	// Create and dispatch event
	csxs::event::Event event = {EVENT_UPDATE_DEFN,
								 csxs::event::kEventScope_Application,
								 ILST_APPID,
								 NULL,
								 payloadString.c_str()};
	fPPLib.DispatchEvent(&event);
}

void WebterPanelController::AddEntryToDictionary()
{
	Dictionary dict(itemType == DOCUMENT ? docDictionary : artDictionary);
	
	DictionaryEntryType type(entryType);
	
	AIBoolean success = true;
	switch (type.type())
	{
		case DDT_INT:
		{
			ai::int32 intVal = atoi(stringValue.c_str());
			dict.setIntegerEntry(entryString, intVal);
			break;
		}
		case DDT_REAL:
		{	
			AIReal floatVal = (float)atof(stringValue.c_str());
			dict.setRealEntry(entryString, floatVal);
			break;
		}
		case DDT_STRING:
		{
			dict.setStringEntry(entryString, stringValue);
			break;
		}
		case DDT_BOOLEAN:
		{
			// TODO: Revisit this once we change the xml sent from the panel
			AIBoolean boolVal = atoi(stringValue.c_str());
			dict.setBooleanEntry(entryString, boolVal);
			break;
		}
		default:
		{
			success = false;
			sAIUser->ErrorAlert(ai::UnicodeString("I don't support that type!"));
			break;
		}
	}

	if (success) // Let's send the updated dictionary to the panel
	{
		if (itemType == DOCUMENT)
			SendDocDictionaryToPanel();
		else
			SendArtDictionaryToPanel();
	}
}

string WebterPanelController::GetElement(string srcData, const char* value)
{
	string openStr("<");
	string contentStr(value);
	string endStr(">");
	openStr.append(contentStr);
	openStr.append(endStr);
	size_t openIndex = srcData.find(openStr);
	openIndex = openIndex + openStr.length();
	string strValue("");

    string colseStr("</");
	colseStr.append(contentStr);
	colseStr.append(endStr);
	size_t closeIndex = srcData.find(colseStr);
	size_t strLength = closeIndex - openIndex;
	if ( strLength > 0 && strLength < srcData.length())
	{
		strValue = srcData.substr(openIndex, strLength);
	}
	
	return strValue;
}
