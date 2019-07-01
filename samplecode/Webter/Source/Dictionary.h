//========================================================================================
//  
//  $File: //ai/mainline/devtech/sandbox/lomorgan/public/samplecode/Webter/Source/Dictionary.h $
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
#include "DictionaryEntryType.h"

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

/** A wrapper class for the AIDictionary type, with only
	the functions we are interested in. The class increases the
	reference count of the AIDictionaryRef passed to its
	constructor, and decreases it again on destruction. Thus to avoid
	memory leaks you should release any handles you have on the
	AIDictionaryRef object after constructing the Dictionary object.
*/
class Dictionary
{

public:
	/** Constructor.
		@param aiDictionary IN reference to an AI dictionary. Constructor will
		increase the reference count of this dictionary by one.
	*/
	Dictionary(AIDictionaryRef aiDictionary);
	
	/** Destructor.
		Will decrease the references count to the underlying AI dictionary by one.
	*/
	~Dictionary(void);
	
	/** Copy constructor.
		New instance will increase the reference count on the underlying dictionary.
	*/
	Dictionary(const Dictionary& from);
	
	/** Assignment operator.
		Will manage references to the underlying AI Dictionaries in the expected way
	*/
	Dictionary& operator= (Dictionary const& that);
	
	/** Get an integer entry
		@param key IN string key for the entry
		@return The value corresponding to that key
	*/
	ai::int32 getIntegerEntry(const std::string& key);
	
	/** Set an integer entry
		@param key IN string key for the entry
		@param value IN the value for entry
	*/
	void setIntegerEntry(const std::string& key, const ai::int32 value);
	
	/** Get a real entry
		@param key IN string key for the entry
		@return The value corresponding to that key
	 */
	AIReal getRealEntry(const std::string& key);
	
	/** Set a real entry
		@param key IN string key for the entry
		@param value IN the value for entry
	*/
	void setRealEntry(const std::string& key, const AIReal value);
	
	/** Get a boolean entry
		@param key IN string key for the entry
		@return The value corresponding to that key
	*/
	AIBoolean getBooleanEntry(const std::string& key);
	
	/** Set a boolean entry
		@param key IN string key for the entry
		@param value IN the value for entry
	*/
	void setBooleanEntry(const std::string& key, const AIBoolean value);
	
	/** Get a string entry
		@param key IN string key for the entry
		@return The value corresponding to that key
	*/
	std::string getStringEntry(const std::string& key);
	
	/** Set a string entry
		@param key IN string key for the entry
		@param value IN the value for entry
	*/
	void setStringEntry(const std::string& key, const std::string& value);
	
	std::string toCommaSeparatedString(void);
	
	/** Get the type of a dictionary entry
		@param key IN string key for the entry
		@returns the DictionaryEntryType of the entry
	*/
	DictionaryEntryType getTypeOfEntryWithKey(const std::string& key);
	
	/**	Checks whether the underlying AI dictionary that was passed
		to the constructor was null
	*/
	AIBoolean isUnderlyingDictionaryNull() { return aiDict == NULL; };
	
private:
	/** A reference to the underlying AI Dictionary */
	AIDictionaryRef aiDict;
	
	/** Construct an AI Dictionary key from a plain string
		@param key IN the string key for which we want the AI Dict key.
		@returns an AI Dict key for that string value
	*/
	AIDictKey getKeyFromString(const std::string& key);

	/** The following functions are used to manage the underlying dictionary */
	void initializeFrom(const Dictionary& otherDictionary);
	
	void finalize(void);
		
	void incDictRefCount();
	
	void decDictRefCount();
	
};

#endif
