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
    private:
        map<string, map<string, double> > m_lexTable;
    };

    class PhraseTableEntry
    {
    public:
        PhraseTableEntry();
    private:
        double m_phraseTransWgt; //! Phrase translation weight estimated by MLE
        double m_lexicalWgt;  //! lexical translation weight of phrase pair

    };

    class PhraseTable
    {
    public:

    private:
        
    };

    class PhraseScorer
    {
    public:

    private:

    };
}

#endif
