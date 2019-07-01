//========================================================================================
//  
//  $File: //ai/mainline/devtech/sandbox/lomorgan/public/samplecode/Webter/Source/DictionaryEntryType.h $
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

#include "AIEntry.h"

#ifndef __DICTIONARYENTRYTYPE_H__
#define __DICTIONARYENTRYTYPE_H__

/** An enum representing the types of dictionary entries that we're interested in.
	Specifically integers, reals, booleans and strings (values we can display), and
	then everything else.
*/
enum DictionaryDatatype
{
	DDT_INT,
	DDT_REAL,
	DDT_BOOLEAN,
	DDT_STRING,
	DDT_OTHER
};

/** A wrapper for the AIEntryType that makes it easy to work only
	with the types that we're interested in.
 
	In particular it lets us get string representations of the type
	and can give us the type as a DictionaryDatatype which allows us
	to switch over the types we're interested in and ignore the rest.
*/
class DictionaryEntryType
{
public:
	
	/** Constructs a new DictionaryEntryType.
		@param AIEntryType the AI type of the entry
	*/
	DictionaryEntryType(AIEntryType aiType);
	
	/** Gets the type of the entry as a DictionaryDatatype
		@returns the type: int, real, boolean, string or other.
	*/
	DictionaryDatatype type();
	
	/** Get a string represenation of the type
		@returns A string description of the type, in English
	*/
	std::string to_str();
	
	/** Get the underlying type of the entry
		@returns the underlying type, an AIEntryType
	*/
	AIEntryType getUnderlyingType() { return underlyingType; }
	
private:
	/** The underlying AIEntryType */
	AIEntryType underlyingType;
};

#endif
