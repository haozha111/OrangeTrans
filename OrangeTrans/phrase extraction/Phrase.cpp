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

#include "Phrase.h";

using namespace std;

namespace OrangeTraining
{
	//PhrasePair code
	PhrasePair::PhrasePair(size_t srcStart, size_t srcEnd, size_t tgtStart, size_t tgtEnd
		, const WordAlignment &wordAlignment)
		: m_srcStart(srcStart)
		, m_srcEnd(srcEnd)
		, m_tgtStart(tgtStart)
		, m_tgtEnd(tgtEnd)
	{
		//add alignment info
		for (size_t k = tgtStart; k <= tgtEnd; ++k){

			for (vector<size_t>::const_iterator iter = wordAlignment.GetTargetAlignmentAtPos(k).begin();
				iter != wordAlignment.GetTargetAlignmentAtPos(k).end(); ++iter){
				this->m_alignInfo.push_back(make_pair(k - tgtStart, *iter - srcStart));
			}
		}
		//add phrase string
		this->m_srcPhrase = wordAlignment.GetSourceSubstring(srcStart, srcEnd);
		this->m_tgtPhrase = wordAlignment.GetTargetSubstring(tgtStart, tgtEnd);

	}

	size_t PhrasePair::SourceStartPos() const
	{
		return this->m_srcStart;
	}

	size_t PhrasePair::SourceEndPos() const
	{
		return this->m_srcEnd;
	}

	size_t PhrasePair::TargetStartPos() const
	{
		return this->m_tgtStart;
	}

	size_t PhrasePair::TargetEndPos() const
	{
		return this->m_tgtEnd;
	}

	string PhrasePair::SourcePhrase() const
	{
		return this->m_srcPhrase;
	}

	string PhrasePair::TargetPhrase() const
	{
		return this->m_tgtPhrase;
	}

	const vector<pair<size_t, size_t> > & PhrasePair::AlignInfo() const
	{
		return this->m_alignInfo;
	}

	//PhraseCollection code

	PhraseCollection::PhraseCollection()
	{}

	bool PhraseCollection::AddPhrasePair(const PhrasePair &phrasePair)
	{
		this->m_phraseCollection.push_back(phrasePair);
		return true;
	}

	bool PhraseCollection::WritetoFile(ofstream &output, ofstream &outputinv) const
	{
		for (vector<PhrasePair>::const_iterator iter = this->m_phraseCollection.begin();
			iter != this->m_phraseCollection.end(); ++iter){
			output << iter->SourcePhrase() << " ||| " << iter->TargetPhrase() << " |||";
			outputinv << iter->TargetPhrase() << " ||| " << iter->SourcePhrase() << " |||";
			for each (pair<size_t, size_t> align in iter->AlignInfo()){
				output << " " << align.second << "-" << align.first;
				outputinv << " " << align.first << "-" << align.second;
			}
			output << endl;
			outputinv << endl;
		}

		return true;
	}

	size_t PhraseCollection::TotalRuleCount() const
	{
		return this->totalRuleCount;
	}

	size_t PhraseCollection::NullRuleCount() const
	{
		return this->nullRuleCount;
	}


}