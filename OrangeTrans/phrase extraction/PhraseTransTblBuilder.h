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
  //! The entry type of phrase table.
  class PhraseTblEntry
  {
  public:
    double m_phraseTransWgt; //! Phrase translation weight estimated by MLE
    double m_lexicalWgt;  //! lexical translation weight of phrase pair
    size_t m_freq; //! total frequency of phrase pair
    string m_align;
    string m_srcphrase;
    string m_tgtphrase;

    //methods
    bool CreatePhraseTblEntry(string &srcPhrase, string &tgtPhrase
      , string &align);

    //properties
    const vector<string>& SourcePhraseVec() const;
    const vector<string>& TargetPhraseVec() const;
    const vector<vector<size_t> >& AlignedToSource() const;
    const vector<vector<size_t> >& AlignedToTarget() const;
    size_t SourceLength() const;
    size_t TargetLength() const;
    bool Equals(const PhraseTblEntry &entry) const;
    
  private:
    vector<vector<size_t> > m_alignedToSrc; //! Alignment info on source phrase
    vector<vector<size_t> > m_alignedToTgt; //! Alignment info on target phrase
    vector<string> m_srcphraseVec;
    vector<string> m_tgtphraseVec;
    size_t m_srclen;
    size_t m_tgtlen;
  };

  //! The Lexibal class is implemented for storing pairs of 
  // word trnaslations and their lexical probability
  class LexicalTable
  {
  public:
    void LoadLexicalTable(string &pLexicaltable);

    //! method for computing lexical weight
    double GetLexicalWeight(PhraseTblEntry &entry);
  private:
    map<string, map<string, double> > m_lexTable;
  };
  //! Phrase table stores phrase translation probability,
  //! lexical translation weight, aligment information and
  //! other features.
  class PhraseTable
  {
  public:
    PhraseTable(LexicalTable &lexicalTble
      , ofstream &output);
    //!Insert one entry to the phrase table.
    void Insert(PhraseTblEntry &entry);
  private:
    vector<PhraseTblEntry> m_phraseTable;//! where phrase table entry is stored
    size_t m_count; //!count of phrases with same source in phrase table
    LexicalTable &m_lexicalTbl;
    ofstream &m_output; //!output file stream
  };


  class PhraseTransTblBuilder
  {
  public:
    PhraseTransTblBuilder(string pRule
      , string pInvRule
      , string pOutput
      , string pLexs2t
      , string pLext2s);
   
    //method for building translation table
    bool BuildPhraseTransTbl();
  private:
    void BuildOneDirection(ifstream &fin, ofstream &fout, LexicalTable &lex);
    void MergeTable(ifstream &fin1, ifstream &fin2, ofstream &fout);
    string m_rule; //!reader for rule table.
    string m_invrule; //!reader for inverted rule table.
    string m_output;
    string m_plexs2t; //! path to lexical trans table
    string m_plext2s;
    LexicalTable m_lexs2t; //! the lexical trans table
    LexicalTable m_lext2s;
  };

}

#endif
