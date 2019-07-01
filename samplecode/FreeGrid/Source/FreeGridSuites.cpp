//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridSuites.cpp $
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
#include "FreeGridSuites.h"

extern "C" 
{ 
	
	AIArtSuite *sAIArt;
	AITransformArtSuite*	sAITransformArt ;
	AIMatchingArtSuite *sAIMatchingArt;
	AIMdMemorySuite *sAIMdMemory;
	AIMenuSuite *sAIMenu;
	AIDictionarySuite *sAIDictionary;
	AIPathSuite *sAIPath;
	AIPathStyleSuite *sAIPathStyle;
	AILayerSuite *sAILayer;
	AIUndoSuite *sAIUndo;
	AIDataFilterSuite *sAIDataFilter;
	AIPerspectiveGridSuite *sAIPerspectiveGrid;
	AIPerspectiveTransformSuite *sAIPerspectiveTransform;
	AIArraySuite *sAIArray;
	AIEntrySuite *sAIEntry;
	AIFilterSuite *sAIFilter;
	AILiveEffectSuite *sAILiveEffect;
	AIRealMathSuite *sAIRealMath;
	AIArtStyleSuite *sAIArtStyle;
	AIArtStyleParserSuite *sAIArtStyleParser;
	AITextFrameSuite *sAITextFrame;
	AIFontSuite *sAIFont;
	AIUnicodeStringSuite *sAIUnicodeString;
	AIStringFormatUtilsSuite *sAIStringFormatUtils;
	SPBlocksSuite *sSPBlocks;

	EXTERN_TEXT_SUITES 
}

ImportSuite gImportSuites[] = 
{
	
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemory,
	kAIMenuSuite, kAIMenuSuiteVersion, &sAIMenu,
	kAIDictionarySuite, kAIDictionarySuiteVersion, &sAIDictionary,
	kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
	kAILayerSuite, kAILayerSuiteVersion, &sAILayer,
	kAIUndoSuite, kAIUndoSuiteVersion, &sAIUndo,
	kAIDataFilterSuite, kAIDataFilterSuiteVersion, &sAIDataFilter,
	kAIPerspectiveGridSuite, kAIPerspectiveGridSuiteVersion, &sAIPerspectiveGrid,
	kAIPerspectiveTransformSuite, kAIPerspectiveTransformSuiteVersion, &sAIPerspectiveTransform,
	kAIArraySuite, kAIArraySuiteVersion, &sAIArray,
	kAIEntrySuite, kAIEntrySuiteVersion, &sAIEntry,
	kAIFilterSuite, kAIFilterSuiteVersion, &sAIFilter,
	kAILiveEffectSuite, kAILiveEffectVersion, &sAILiveEffect,
	kAIRealMathSuite, kAIRealMathVersion, &sAIRealMath,
	kAIArtStyleSuite, kAIArtStyleSuiteVersion, &sAIArtStyle,
	kAIArtStyleParserSuite,	kAIArtStyleParserSuiteVersion, &sAIArtStyleParser,
	kAITextFrameSuite, kAITextFrameSuiteVersion, &sAITextFrame,
	kAIFontSuite, kAIFontSuiteVersion, &sAIFont,
	kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion, &sAIUnicodeString,
	kAIStringFormatUtilsSuite, kAIStringFormatUtilsSuiteVersion, &sAIStringFormatUtils,
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAITransformArtSuite, kAITransformArtSuiteVersion, &sAITransformArt,
	IMPORT_TEXT_SUITES

	nil, 0, nil
};
// end FreeGridSuites.cpp
