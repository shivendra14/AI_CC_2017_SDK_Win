//========================================================================================
//  
//  $File: //ai/mainline/devtech/sandbox/lomorgan/public/samplecode/Webter/Source/DictionaryEntryType.cpp $
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

#include "DictionaryEntryType.h"

DictionaryEntryType::DictionaryEntryType(AIEntryType aiType)
: underlyingType(aiType)
{}

DictionaryDatatype DictionaryEntryType::type()
{
	DictionaryDatatype retVal;
	
	switch (underlyingType)
	{
		case IntegerType:
			retVal = DDT_INT;
			break;
		case BooleanType:
			retVal = DDT_BOOLEAN;
			break;
		case RealType:
			retVal = DDT_REAL;
			break;
		case StringType:
			retVal = DDT_STRING;
			break;
		default:
			retVal = DDT_OTHER;
			break;
	}
	
	return retVal;
}

std::string DictionaryEntryType::to_str()
{
	std::string stringValue;
	
	switch (underlyingType)
	{
		case UnknownType:
			stringValue = "Unknown Type";
			break;
		case IntegerType:
			stringValue = "Integer";
			break;
		case BooleanType:
			stringValue = "Boolean";
			break;
		case RealType:
			stringValue = "Real";
			break;
		case StringType:
			stringValue = "String";
			break;
		case DictType:
			stringValue = "DictType";
			break;
		case ArrayType:
			stringValue = "ArrayType";
			break;
		case BinaryType:
			stringValue = "BinaryType";
			break;
		case PointType:
			stringValue = "PointType";
			break;
		case MatrixType:
			stringValue = "MatrixType";
			break;
		case PatternRefType:
			stringValue = "PatternRefType";
			break;
		case BrushPatternRefType:
			stringValue = "BrushPatternRefType";
			break;
		case CustomColorRefType:
			stringValue = "CustomColorRefType";
			break;
		case GradientRefType:
			stringValue = "GradientRefType";
			break;
		case PluginObjectRefType:
			stringValue = "PluginObjectRefType";
			break;
		case FillStyleType:
			stringValue = "FillStyleType";
			break;
		case StrokeStyleType:
			stringValue = "StrokeStyleType";
			break;
		case UIDType:
			stringValue = "UIDType";
			break;
		case UIDREFType:
			stringValue = "UIDREFType";
			break;
		case XMLNodeType:
			stringValue = "XMLNodeType";
			break;
		case SVGFilterType:
			stringValue = "SVGFilterType";
			break;
		case ArtStyleType:
			stringValue = "ArtStyleType";
			break;
		default:
			stringValue = "Error! Unrecognized type!";
			break;
	}
	
	return stringValue;
}
