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
#ifndef PHRASE_PAIR_H_INCLUDED_
#define PHRASE_PAIR_H_INCLUDED_

#include <fstream>
#include <vector>
#include "WordAlignment.h";

namespace OrangeTraining
{
	class PhrasePair
	{
	public:
		PhrasePair(size_t srcStart, size_t srcEnd, size_t tgtStart, size_t tgtEnd
			, const WordAlignment &wordAlignment);
		
		//method

		

		//atributes

		size_t SourceStartPos() const;
		size_t SourceEndPos() const;
		size_t TargetStartPos() const;
		size_t TargetEndPos() const;
		std::string SourcePhrase() const;
		std::string TargetPhrase() const;
		const std::vector<std::pair<size_t, size_t> > & AlignInfo() const;

	private:
		size_t m_srcStart;
		size_t m_srcEnd;
		size_t m_tgtStart;
		size_t m_tgtEnd;
		std::vector<std::pair<size_t, size_t> > m_alignInfo;
		std::string m_srcPhrase;
		std::string m_tgtPhrase;
	};

	//! The PhraseCollection class is to store the extracted phrase pairs
	class PhraseCollection{
	public:
		PhraseCollection();
		//! Add a new phrasePair to the collection

		//methods

		bool AddPhrasePair(const PhrasePair & phrasePair);
		//! print all rules in this collection to file
		bool WritetoFile(std::ofstream &output, std::ofstream &outputinv) const;
		
		//attributes

		size_t TotalRuleCount() const;
		size_t NullRuleCount() const;
	private:
		std::vector<PhrasePair> m_phraseCollection;
		size_t totalRuleCount; 
		size_t nullRuleCount; //! count of Null rule (X ||| NULL) in the collection

	};


}
#endif