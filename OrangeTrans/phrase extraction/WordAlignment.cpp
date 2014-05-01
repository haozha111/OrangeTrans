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

#include "WordAlignment.h";
#include "..\util\BasicMethod.h";
#include <iostream>;

using namespace OrangeTrans;

namespace OrangeTraining
{
  /**Create a word alignment.
  *\param srcSentence  the source language sentence
  *\param tgtSentence  the target language sentence
  *\param alignment  the aligment between sentence pair
  */

  void WordAlignment::CreateAlignment(
    const string &srcSentence
    , const string &tgtSentence
    , const string &alignment
    , unsigned int sentenceID)
  {
    if (srcSentence == ""){
      cerr << endl
        << " WARNING: EMPTY sentence detected: " << sentenceID << " line in source file." << endl;
    }
    if (tgtSentence == ""){
      cerr << endl
        << " WARNING: EMPTY sentence detected: " << sentenceID << " line in target file." << endl;
    }
    if (alignment == ""){
      cerr << endl
        << " WARNING: EMPTY alignment detected: " << sentenceID << " line in alignment file." << endl;
    }

    m_srcSentence = BasicMethod::Split(srcSentence);
    m_tgtSentence = BasicMethod::Split(tgtSentence);
    m_sentenceID = sentenceID;
    m_srcLength = m_srcSentence.size();
    m_tgtLength = m_tgtSentence.size();

    vector<string> tmpVec = BasicMethod::Split(alignment);
    size_t srclen = m_srcSentence.size();
    size_t tgtlen = m_tgtSentence.size();

    //intialize data structures
    m_srcAlign.assign(srclen, vector<size_t>());
    m_tgtAlign.assign(tgtlen, vector<size_t>());
    m_srcAlignCount.assign(srclen, 0);
    m_tgtAlignCount.assign(tgtlen, 0);

    for (auto& align : tmpVec)
    {
      unsigned int srcWordId, tgtWordId;
      //check aligment format
      if (!sscanf_s(align.c_str(), "%d-%d", &srcWordId, &tgtWordId)){
        cerr << "WARNING: " << align << " is a bad aligment point in sentence " << sentenceID << endl;
        cerr << "T: " << tgtSentence << endl << "S: " << srcSentence << endl;
      }
      //check out of bound case
      if ((size_t)srcWordId >= m_srcSentence.size() || (size_t)tgtWordId >= m_tgtSentence.size()){
        cerr << "WARNING: " << align << " out of bounds." << endl
          << "T: " << tgtSentence << endl << "S: " << srcSentence << endl;
      }

      m_srcAlign[srcWordId].push_back(tgtWordId);
      m_tgtAlign[tgtWordId].push_back(srcWordId);
      m_srcAlignCount[srcWordId]++;
      m_tgtAlignCount[tgtWordId]++;
    }

  }

  //!get vector representation of souce sentence
  const vector<string> & WordAlignment::SourceSentence() const
  {
    return m_srcSentence;
  }

  //! get vector representation of target sentence
  const vector<string> & WordAlignment::TargetSentence() const
  {
    return m_tgtSentence;
  }

  //! return source sentence length
  size_t WordAlignment::SourceLength() const
  {
    return m_srcLength;
  }

  //! return target sentence length
  size_t WordAlignment::TargetLength() const
  {
    return m_tgtLength;
  }

  //! Get alignment points of source word at position pos
  const vector<size_t> & WordAlignment::GetSourceAlign(size_t pos) const
  {
    return m_srcAlign[pos];
  }

  //! Get alignment points of target word at position pos
  const vector<size_t> & WordAlignment::GetTargetAlign(size_t pos) const
  {
    return m_tgtAlign[pos];
  }

  //! Get count of alignment points of source word at position pos
  size_t WordAlignment::GetSourceAlignCount(size_t pos) const
  {
    if (pos >= 0 && pos < m_srcAlignCount.size()){
      return m_srcAlignCount[pos];
    }
    else{
      throw exception("WordAlignment.GetSourceAlignmentCountAtPos(size_t pos): index out of bounds!");
    }
  }

  //! Get count of alignment points of target word at position pos
  size_t WordAlignment::GetTargetAlignCount(size_t pos) const
  {
    if (pos >= 0 && pos < m_tgtAlignCount.size()){
      return m_tgtAlignCount[pos];
    }
    else{
      throw exception("WordAlignment.GetTargetAlignmentCountAtPos(size_t pos): index out of bounds!");
    }
  }

  //! Get substring(phrase) of source sentence
  string WordAlignment::GetSourcePhrase(size_t startPos, size_t endPos) const
  {
    if (startPos > endPos){
      return "";
    }
    if ((startPos < 0 || startPos >= m_srcSentence.size())
      || (endPos < 0 || endPos >= m_srcSentence.size())){
      throw exception("WordAlignment.GetSourceSubstring(size_t startPos, size_t endPos): index out of bounds!");
    }
    string src = "";
    for (size_t k = startPos; k <= endPos; ++k){
      src += m_srcSentence[k];
      if (k != endPos){
        src += " ";
      }
    }
    return src;
  }

  //! Get substring(phrase) of target sentence
  string WordAlignment::GetTargetPhrase(size_t startPos, size_t endPos) const
  {
    if (startPos > endPos){
      return "";
    }
    if ((startPos < 0 || startPos >= m_tgtSentence.size())
      || (endPos < 0 || endPos >= m_tgtSentence.size())){
      throw exception("WordAlignment.GetTargetSubstring(size_t startPos, size_t endPos): index out of bounds!");
    }
    string tgt = "";
    for (size_t k = startPos; k <= endPos; ++k){
      tgt += m_tgtSentence[k];
      if (k != endPos){
        tgt += " ";
      }
    }
    return tgt;
  }

  //! Get the null-aligned source positions
  vector<size_t> WordAlignment::GetSourceNullAligned() const
  {
    vector<size_t> result;
    for (size_t i = 0; i < m_srcLength; ++i){
      if (m_srcAlign[i].size() == 0){
        result.push_back(i);
      }
    }
    return result;
  }

  //! Get the null-aligned target positions
  vector<size_t> WordAlignment::GetTargetNullAligned() const
  {
    vector<size_t> result;
    for (size_t i = 0; i < m_tgtLength; ++i){
      if (m_tgtAlign[i].size() == 0){
        result.push_back(i);
      }
    }
    return result;
  }
}