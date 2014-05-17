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
#ifndef TRIE_H_INCLUDED_
#define TRIE_H_INCLUDED_

#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

namespace OrangeDecoder
{
  struct TranslationOption
  {
    string translation;
    //translation probabilities
    double p;
    double plex;
    double pinv;
    double plexinv;
    //msd reordering info
    double prevM;
    double prevS;
    double prevD;
    double nextM;
    double nextS;
    double nextD;
  };

  struct TrieNode
  {
    string word;
    bool terminate;
    vector<TranslationOption*> transptr;
    TrieNode* next; //sibling
    TrieNode* son; //first son node

    TrieNode();
    ~TrieNode();
    void AddTranslationOption(TranslationOption* option);
    double GetCheapestTransCost() const;
    static bool cmp(TranslationOption* a, TranslationOption* b);
  };

  class Trie
  {
  public:
    Trie();
    ~Trie();
    void CreateTrieTree();
    TrieNode* Insert(vector<string> &srcphra);
    TrieNode* Find(vector<string> &srcphra);
    TranslationOption* FindPhrasePair(vector<string> &srcphra, string translation);
  private:
    TrieNode* m_root;
    TrieNode* InsertNode(vector<string> &srcphra, TrieNode* start, size_t pos);
    TrieNode* FindNode(vector<string> &srcphra, TrieNode* start, size_t pos);
    void Clear(TrieNode* start);
  };
}

#endif