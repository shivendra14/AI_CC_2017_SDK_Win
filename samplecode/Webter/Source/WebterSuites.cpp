//========================================================================================
//  
//  $File: //ai_stream/rel_21_0/devtech/sdk/public/samplecode/Webter/Source/WebterSuites.cpp $
//
//  $Revision: #1 $
//
//  Copyright 1987 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "IllustratorSDK.h"
#include "WebterSuites.h"

extern "C" {
	SPBlocksSuite				*sSPBlocks = NULL;
	AIArtSuite					*sAIArt = NULL;
	AIDocumentSuite				*sAIDocument = NULL;
	AIMatchingArtSuite			*sAIMatchingArt = NULL;
	AIMdMemorySuite				*sAIMdMemory = NULL;
	AIPreferenceSuite			*sAIPreference = NULL;
	AIMenuSuite					*sAIMenu = NULL;
	AIDictionarySuite			*sAIDictionary = NULL;
	AIDictionaryIteratorSuite	*sAIDictionaryIterator = NULL;
	AIUnicodeStringSuite		*sAIUnicodeString = NULL;
	AIStringFormatUtilsSuite	*sAIStringFormatUtils = NULL;
}

ImportSuite gImportSuites[] = 
{
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIDocumentSuite, kAIDocumentVersion, &sAIDocument,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
	kAIPreferenceSuite, kAIPreferenceSuiteVersion, &sAIPreference,
	kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
	kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
	kAIDictionaryIteratorSuite, kAIDictionaryIteratorSuiteVersion, &sAIDictionaryIterator,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion,&sAIUnicodeString,
	kAIStringFormatUtilsSuite, kAIStringFormatUtilsSuiteVersion, &sAIStringFormatUtils,
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	nil, 0, nil
};

