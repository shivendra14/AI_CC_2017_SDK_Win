//========================================================================================
//  
//  $File: //ai/mainline/devtech/sdk/public/samplecode/DrawArt/Source/DrawArtSuites.cpp $
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

#include "IllustratorSDK.h"
#include "DrawArtSuites.h"

// Suite externs
extern "C"
{
	SPBlocksSuite*			sSPBlocks = NULL;
	AIUnicodeStringSuite*	sAIUnicodeString = NULL;
	AIArtSuite*				sAIArt = NULL;
	AIDrawArtSuite*			sAIDrawArtSuite = NULL;
	AISwatchListSuite*		sAISwatchList = NULL;
    AICSXSExtensionSuite		*sAICSXSExtension;
}

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIDrawArtSuite, kAIDrawArtSuiteVersion, &sAIDrawArtSuite,
	kAISwatchListSuite, kAISwatchListSuiteVersion, &sAISwatchList,
	nil, 0, nil
};

ImportSuite gPostStartupSuites[] =
{
	kAICSXSExtensionSuite, kAICSXSExtensionSuiteVersion, &sAICSXSExtension,
	nil, 0, nil
};
