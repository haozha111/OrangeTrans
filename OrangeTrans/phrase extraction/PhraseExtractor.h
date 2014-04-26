/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:The function of PhraseExtractor is to extract phrases (and assign scores) that are consistent with
the input word alignment.

*$Version:
*0.0.1
*$Created by:
*Haoliang Zhang(A30041839@gmail.com)
*$Last Modified:
**/

#pragma once
#ifndef PHRASE_EXTRACTOR_H_INCLUDED_
#define PHRASE_EXTRACTOR_H_INCLUDED_

#include <iostream>
#include <string>
#include <fstream>

#include "WordAlignment.h";
#include "Phrase.h";

namespace OrangeTraining
{
	enum class RuleType { Phrase, HieroPhrase };
	class RuleOptions{
	public:
		RuleOptions();
		size_t srcMaxLen;
		size_t tgtMaxLen;
		size_t srcMaxNullExp; //! maximum number of expansion of NULL align in source
		size_t tgtMaxNullExp; //! maximum number of expansion of NULL align in target
		bool includeNullAlign; //! allows for NULL rule	
	};


	class PhraseExtractor
	{
	public:
		//! constructor for PhraseExtractor class
		PhraseExtractor(
			WordAlignment &wordAlignment,
			PhraseCollection &phraseCollection
			);

		//! extract phrase pair
		bool ExtractPhrasePair(RuleOptions &option) const;
	private:
		WordAlignment &m_wordAlignment; //! the word alignment
		PhraseCollection &m_phraseCollection;

		void ValidateRuleConsistency(PhrasePair &phrasePair,
			WordAlignment &wordAlignment,
			RuleOptions &options) const;
		void AddRule(PhrasePair &phrasePair, bool hasEmptyRule) const;

	};
}

#endif