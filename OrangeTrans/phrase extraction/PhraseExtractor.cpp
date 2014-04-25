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
#include "PhraseExtractor.h";

using namespace std;

namespace OrangeTraining
{
	PhraseExtractor::PhraseExtractor(WordAlignment &wordAlignment, PhraseCollection &phraseCollection)
		: m_wordAlignment(wordAlignment), m_phraseCollection(phraseCollection) {}


	//! implementation of phrase pair extraction algorithm
	bool PhraseExtractor::ExtractPhrasePair(RuleOptions &options) const
	{
		//traverse all possible spans of target phrase
		size_t targetLength = m_wordAlignment.TargetLength();
		size_t sourceLength = m_wordAlignment.SourceLength();
		for (size_t tgtStartPos = 0; tgtStartPos < targetLength; ++tgtStartPos){
			for (size_t tgtEndPos = tgtStartPos;
				tgtEndPos < targetLength && (tgtEndPos - tgtStartPos + 1) <= options.tgtMaxLen; ++tgtEndPos){
				//find the minimally matching foreign phrase
				size_t srcStartPos = sourceLength;
				size_t srcEndPos = 0;
				for (size_t k = tgtStartPos; k <= tgtEndPos; ++k){
					for (vector<size_t>::const_iterator iter = m_wordAlignment.GetTargetAlignmentAtPos(k).begin();
						iter != m_wordAlignment.GetTargetAlignmentAtPos(k).end(); ++iter){
						srcStartPos = (*iter < srcStartPos) ? *iter : srcStartPos;
						srcEndPos = (*iter > srcStartPos) ? *iter : srcStartPos;
					}
				}

				ValidateRuleConsistency(PhrasePair(srcStartPos, srcEndPos, tgtStartPos, tgtEndPos, m_wordAlignment),
					m_wordAlignment, options);
			}
		}

		//add X ||| NULL or NULL ||| X
		if (options.isIncludeNullAlign){
			for each (size_t pos in m_wordAlignment.GetSourceNullAligned()){
				PhrasePair phrasePair = PhrasePair(pos, pos, 1, 0, m_wordAlignment);
				AddRule(phrasePair, true);
			}

			for each(size_t pos in m_wordAlignment.GetTargetNullAligned()){
				PhrasePair phrasePair = PhrasePair(1, 0, pos, pos, m_wordAlignment);
				AddRule(phrasePair, true);
			}
		}

		return true;
	}

	//! check whether the extracted phrase pair is consistent with the word alignment
	bool PhraseExtractor::ValidateRuleConsistency(PhrasePair &phrasePair, WordAlignment &wordAlignment,
		RuleOptions &options) const
	{
		//first check if the source phrase length exceed maximum length
		if ((phrasePair.SourceEndPos() - phrasePair.SourceStartPos() + 1) > options.srcMaxLen){
			return false;
		}
		//check if at least one alignment point
		if (phrasePair.SourceStartPos() > phrasePair.SourceEndPos()){
			return false;
		}
		//check if the rule breaks consistency
		for (size_t k = phrasePair.SourceStartPos(); k <= phrasePair.SourceEndPos(); ++k){
			for (vector<size_t>::const_iterator iter = wordAlignment.GetSourceAlignmentAtPos(k).begin();
				iter != wordAlignment.GetSourceAlignmentAtPos(k).end(); ++iter){
				if (*iter < phrasePair.TargetStartPos() || *iter > phrasePair.TargetEndPos()){
					return false; //break the consistency rule
				}
			}
		}

		//expand the phrase pairs to include additional unaligned source
		for (int sourceStart = phrasePair.SourceStartPos(); 
			sourceStart >= 0 
			&& (sourceStart == phrasePair.SourceStartPos() || wordAlignment.GetSourceAlignmentCountAtPos(sourceStart) == 0)
			&& ((phrasePair.SourceStartPos() - sourceStart) <= options.srcMaxNullExpand); --sourceStart){
			for (size_t sourceEnd = phrasePair.SourceEndPos(); 
				sourceEnd < wordAlignment.SourceLength()
				&& (sourceEnd == phrasePair.SourceEndPos() || wordAlignment.GetSourceAlignmentCountAtPos(sourceEnd) == 0)
				&& (sourceEnd - sourceStart + 1) <= options.srcMaxLen 
				&& (sourceEnd - phrasePair.SourceEndPos()) <= options.srcMaxNullExpand; ++sourceEnd){
				//build a new phrase instance
				PhrasePair newPhrasePair = PhrasePair(sourceStart, sourceEnd, phrasePair.TargetStartPos(), phrasePair.TargetEndPos(), wordAlignment);
				AddRule(newPhrasePair, false);
			}
		}

		return true;
	}


	//!Add a new phrase pair to the collection
	bool PhraseExtractor::AddRule(PhrasePair &phrasePair, bool hasEmptyRule) const
	{
		this->m_phraseCollection.AddPhrasePair(phrasePair, hasEmptyRule);
		return true;
	}


	//code for RuleOptions 

	RuleOptions::RuleOptions()
		:srcMaxLen(3)
		, tgtMaxLen(5)
		, srcMaxNullExpand(3)
		, tgtMaxNullExpand(5)
		, isIncludeNullAlign(true)
	{}
}
