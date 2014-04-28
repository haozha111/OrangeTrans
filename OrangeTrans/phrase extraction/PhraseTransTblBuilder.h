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
#ifndef PHRASE_SCORER_H_INCLUDED_
#define PHRASE_SCORER_H_INCLUDED_

using namespace std;

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

namespace OrangeTraining
{
  //! The Lexibal class is implemented for storing pairs of 
  // word trnaslations and their lexical probability
  class LexicalTable
  {
  public:
    void LoadLexicalTable(string &pLexicaltable);

    //! method for computing lexical weight
    double GetLexicalWeight(const PhraseTblEntry &entry);
  private:
    map<string, map<string, double> > m_lexTable;
  };

  //! The entry type of phrase table.
  class PhraseTblEntry
  {
  public:
    //methods
    bool CreatePhraseTblEntry(const string &srcPhrase, const string &tgtPhrase
      , const string &align);

    //properties
    const vector<string>& SourcePhraseVec() const;
    const vector<string>& TargetPhraseVec() const;
    string SourcePhrase() const;
    string TargetPhrase() const;
    const vector<vector<size_t> >& AlignedToSource() const;
    const vector<vector<size_t> >& AlignedToTarget() const;
    size_t SourceLength() const;
    size_t TargetLength() const;
    void SetFrequency(size_t freq);
  private:
    double m_phraseTransWgt; //! Phrase translation weight estimated by MLE
    double m_lexicalWgt;  //! lexical translation weight of phrase pair
    size_t m_freq; //! total frequency of phrase pair
    vector<vector<size_t> > m_alignedToSrc; //! Alignment info on source phrase
    vector<vector<size_t> > m_alignedToTgt; //! Alignment info on target phrase
    vector<string> m_srcphraseVec;
    vector<string> m_tgtphraseVec;
    string m_srcphrase;
    string m_tgtphrase;
    size_t m_srclen;
    size_t m_tgtlen;
  };

  //! Phrase table stores phrase translation probability,
  //! lexical translation weight, aligment information and
  //! other features.
  class PhraseTable
  {
  public:
    PhraseTable();

    //!Insert one entry to the phrase table.
    void Insert(PhraseTblEntry &entry){}
  private:
    vector<PhraseTblEntry> m_phraseTable;//! where phrase table entry is stored
    size_t m_size; //!num of phrase table entry
  };

  class PhraseTransTblBuilder
  {
  public:
    PhraseTransTblBuilder(string pRule, string pInvRule);
   
    //method for building translation table
    void BuildPhraseTransTbl();
  private:
    string m_rule; //!reader for rule table.
    string m_invrule; //!reader for inverted rule table.
  };
}

#endif
