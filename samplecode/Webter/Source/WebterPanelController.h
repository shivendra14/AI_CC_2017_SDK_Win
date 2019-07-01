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
#ifndef __WEBTERPANELCONTROLLER_H__
#define __WEBTERPANELCONTROLLER_H__

#include "SDKPlugPlug.h"
#include "IllustratorSDK.h"
#include "WebterID.h"
#include "FlashUIController.h"
#include "WebterPlugin.hpp"
#include "Dictionary.h"

extern WebterPlugin *gPlugin;

enum ItemType
{
	DOCUMENT,
	ART
};

class WebterPanelController : public FlashUIController
{
public:
	WebterPanelController(void);

	~WebterPanelController(void);

	csxs::event::EventErrorCode RegisterCSXSEventListeners();

	csxs::event::EventErrorCode RemoveEventListeners();

	ASErr SendData() { return kNoErr;};

	void ParseData(const char* eventData);

	void SetDocumentDictionary(AIDictionaryRef docDictionary);

	void SendDocDictionaryToPanel();
	
	void SetArtDictionary(AIDictionaryRef artDictionary, const ai::UnicodeString& artName);

	void SendArtDictionaryToPanel();
	
	void UpdateDefinitionAndType();

	void AddEntryToDictionary();

	static string GetElement(string srcData, const char* value);

private:
	
	//std::string buildStringOfEntriesFromDictionary(AIDictionaryRef dict);

	std::string getDocDictionaryPayloadFromStringOfEntries(const string& entryList);
	std::string getArtDictionaryPayloadFromStringOfEntries(const string& entryList);

	csxs::event::EventErrorCode dispatchDictionaryUpdateEvent(const string& payload);
	
	Dictionary artDictionary;
	Dictionary docDictionary;
	
	std::string displayNameOfSelectedArt;
	
	ItemType itemType;
	AIEntryType entryType;
	string stringValue;
	string entryString;	
};

#endif
