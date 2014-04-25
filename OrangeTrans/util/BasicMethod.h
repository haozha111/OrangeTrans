/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:
*$Version:
*0.0.1
*$Created by:
*Haoliang Zhang(A30041839@gmail.com)
*$Last Modified:
**/

#pragma once
#ifndef BASIC_METHOD_H_INCLUDED_
#define BASIC_METHOD_H_INCLUDED_

#include <string>
#include <vector>

namespace OrangeTrans
{
	/** The BasicMethod class provides basic functions that are publicly accessible
	**/
	class BasicMethod
	{
	public:
		//! split string with delimiter
		static std::vector<std::string> Split(const std::string &stringtoSplit, const std::string &delimiter);
		//! convert a sentence to a vector of words
		static std::vector<std::string> ConvertStringtoVec(const std::string &sourceString);
	};
}

#endif