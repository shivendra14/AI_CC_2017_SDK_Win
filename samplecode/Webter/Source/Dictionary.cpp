//========================================================================================
//  
//  $File: //ai/mainline/devtech/sandbox/lomorgan/public/samplecode/Webter/Source/Dictionary.cpp $
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
#include "Dictionary.h"
#include "AIDictionary.h"
#include "Plugin.hpp"

extern "C" AIDictionarySuite *sAIDictionary;
extern "C" AIDictionaryIteratorSuite *sAIDictionaryIterator;

Dictionary::Dictionary(AIDictionaryRef aiDictionary)
: aiDict(aiDictionary)
{
	incDictRefCount();
}

Dictionary::~Dictionary()
{
	finalize();
}

Dictionary::Dictionary(const Dictionary& from)
{
	initializeFrom(from);
}

Dictionary& Dictionary::operator= (Dictionary const& that)
{
	if (this == &that)
		return *this;
	
	finalize();
	
	initializeFrom(that);
	
	return *this;
}

void Dictionary::initializeFrom(const Dictionary& otherDictionary)
{
	aiDict = otherDictionary.aiDict;
	incDictRefCount();
}

void Dictionary::finalize()
{
	decDictRefCount();
}

void Dictionary::incDictRefCount()
{
	if(aiDict)
		sAIDictionary->AddRef(aiDict);
}

void Dictionary::decDictRefCount()
{
	if(aiDict)
		sAIDictionary->Release(aiDict);
}

DictionaryEntryType Dictionary::getTypeOfEntryWithKey(const string& key)
{
	AIEntryType entryType;
	AIDictKey dictKey = getKeyFromString(key);
	
	AIErr error = sAIDictionary->GetEntryType( aiDict, dictKey, &entryType);
	if (error)
	{
		string temp(key);
		temp.at(0) = '-';
		dictKey = getKeyFromString(temp);
		sAIDictionary->GetEntryType( aiDict, dictKey, &entryType);
	}

	DictionaryEntryType retVal(entryType);
	return retVal;
}

ai::int32 Dictionary::getIntegerEntry(const string& key)
{
	ai::int32 value;
	sAIDictionary->GetIntegerEntry(aiDict, getKeyFromString(key), &value);
	return value;
}

void Dictionary::setIntegerEntry(const string& key, const ai::int32 value)
{
	AIDictKey dictKey = getKeyFromString(key);
	sAIDictionary->SetIntegerEntry(aiDict, dictKey, value);
}

AIBoolean Dictionary::getBooleanEntry(const string& key)
{
	AIBoolean value;
	sAIDictionary->GetBooleanEntry(aiDict, getKeyFromString(key), &value);
	return value;
}

void Dictionary::setBooleanEntry(const string& key, const AIBoolean value)
{
	AIDictKey dictKey = getKeyFromString(key);
	sAIDictionary->SetBooleanEntry(aiDict, dictKey, value);
}

AIReal Dictionary::getRealEntry(const string& key)
{
	AIReal value;
	sAIDictionary->GetRealEntry(aiDict, getKeyFromString(key), &value);
	return value;
}

void Dictionary::setRealEntry(const string& key, const AIReal value)
{
	AIDictKey dictKey = getKeyFromString(key);
	sAIDictionary->SetRealEntry(aiDict, dictKey, value);
}

std::string Dictionary::getStringEntry(const string& key)
{
	const char* value;
	sAIDictionary->GetStringEntry(aiDict, getKeyFromString(key), &value);
	return string(value);
}

void Dictionary::setStringEntry(const string& key, const string& value)
{
	AIDictKey dictKey = getKeyFromString(key);
	sAIDictionary->SetStringEntry(aiDict, dictKey, value.c_str());
}

AIDictKey Dictionary::getKeyFromString(const string& key)
{
	return sAIDictionary->Key(key.c_str());
}

std::string Dictionary::toCommaSeparatedString()
{
	string retVal;
	if (aiDict)
	{
		stringstream entryNameList;
		AIDictionaryIterator dictionaryIter = NULL;
		sAIDictionary->Begin(aiDict, &dictionaryIter);
		int loopCounter = 0;
		
		printf("Number of entries is: %d", sAIDictionary->Size(aiDict));
		
		while ( !sAIDictionaryIterator->AtEnd(dictionaryIter) )
		{
			AIDictKey dictKey = sAIDictionaryIterator->GetKey( dictionaryIter );
			const char* keyString = sAIDictionary->GetKeyString(dictKey);
			char betterKeyString[256];
			strcpy(betterKeyString, keyString);
			if (betterKeyString[0] == '-')
				betterKeyString[0] = ' ';
			entryNameList<< "," << betterKeyString << "#id#=" << loopCounter;
			sAIDictionaryIterator->Next(dictionaryIter);
			loopCounter++;
		}
		
		sAIDictionaryIterator->Release( dictionaryIter );
		
		retVal = entryNameList.str();
	}
	
	return retVal;
}
