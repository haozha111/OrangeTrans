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
      for (auto& align : wordAlignment.GetTargetAlign(k)){
        m_alignInfo.push_back(make_pair(k - tgtStart, align - srcStart));
      }
    }
    //add phrase string
    m_srcPhrase = wordAlignment.GetSourcePhrase(srcStart, srcEnd);
    m_tgtPhrase = wordAlignment.GetTargetPhrase(tgtStart, tgtEnd);

  }

  size_t PhrasePair::SourceStart() const
  {
    return m_srcStart;
  }

  size_t PhrasePair::SourceEnd() const
  {
    return m_srcEnd;
  }

  size_t PhrasePair::TargetStart() const
  {
    return m_tgtStart;
  }

  size_t PhrasePair::TargetEnd() const
  {
    return m_tgtEnd;
  }

  string PhrasePair::SourcePhrase() const
  {
    return m_srcPhrase;
  }

  string PhrasePair::TargetPhrase() const
  {
    return m_tgtPhrase;
  }

  const vector<pair<size_t, size_t> > & PhrasePair::AlignInfo() const
  {
    return m_alignInfo;
  }

  //PhraseCollection code

  PhraseCollection::PhraseCollection(){}


  void PhraseCollection::AddPhrasePair(const PhrasePair &phrasePair, bool hasEmptyPhrase)
  {
    m_phraseCollection.push_back(phrasePair);
    if (hasEmptyPhrase){
      m_nullRuleCount++;
    }
    m_totalRuleCount++;
  }

  void PhraseCollection::Write(ofstream &output, ofstream &outputinv) const
  {
    for (PhrasePair rule : m_phraseCollection){
      output << rule.SourcePhrase() << " ||| " << rule.TargetPhrase() << " |||";
      outputinv << rule.TargetPhrase() << " ||| " << rule.SourcePhrase() << " |||";
      for (pair<size_t, size_t> align : rule.AlignInfo()){
        output << " " << align.second << "-" << align.first;
        outputinv << " " << align.first << "-" << align.second;
      }
      output << endl;
      outputinv << endl;
    }
  }


  size_t PhraseCollection::TotalRuleCount() const
  {
    return m_totalRuleCount;
  }

  size_t PhraseCollection::NullRuleCount() const
  {
    return m_nullRuleCount;
  }


}