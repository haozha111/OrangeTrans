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

#include "trie.h"
#include <algorithm>

namespace OrangeDecoder
{
  //code for TrieNode
  TrieNode::TrieNode()
    :word("")
    , terminate(false)
    , next(NULL)
    , son(NULL)
  {}

  TrieNode::~TrieNode()
  {
    if (terminate){
      for (TranslationOption* ptr : transptr){
        delete ptr;
      }
    }
  }

  bool TrieNode::cmp(TranslationOption* a, TranslationOption* b)
  {
    return
      (a->p + a->plex + a->pinv + a->plexinv) >
      (b->p + b->plex + b->pinv + b->plexinv);
  }

  void TrieNode::AddTranslationOption(TranslationOption* option)
  {
    this->transptr.push_back(option);
    sort(this->transptr.begin(), this->transptr.end(), TrieNode::cmp);
  }

  double TrieNode::GetCheapestTransCost() const
  {
    if (terminate){
      return
        (this->transptr[0]->p + this->transptr[0]->plex
        + this->transptr[0]->pinv + this->transptr[0]->plexinv) / 4.0;
    }
  }

  //code for Trie
  Trie::Trie() : m_root(NULL){}

  Trie::~Trie()
  {
    Clear(m_root);
  }

  void Trie::CreateTrieTree()
  {
    if (m_root){
      Clear(m_root);
    }
    m_root = new TrieNode();
  }

  TrieNode* Trie::Insert(vector<string> &srcphra)
  {
    if (srcphra.size() == 0){
      return NULL;
    }
    return InsertNode(srcphra, m_root, 0);
  }

  TrieNode* Trie::InsertNode(vector<string> &srcphra, TrieNode* start, size_t pos)
  {
    if (pos == srcphra.size()){
      start->terminate = true;
      return start;
    }
    string curword = srcphra[pos];
    TrieNode* ptr = NULL;
    TrieNode* prevptr = NULL;
    for (TrieNode* tmpptr = start->son; tmpptr != NULL;
      prevptr = tmpptr, tmpptr = tmpptr->next){
      if (tmpptr->word == curword){
        ptr = tmpptr;
        break;
      }
    }
    if (ptr){
      return InsertNode(srcphra, ptr, ++pos);
    }
    TrieNode* newNode = new TrieNode();
    newNode->word = curword;
    if (prevptr){
      prevptr->next = newNode;
    }
    else{
      start->son = newNode;
    }
    return InsertNode(srcphra, newNode, ++pos);
  }

  TrieNode* Trie::Find(vector<string> &srcphra)
  {
    return FindNode(srcphra, m_root, 0);
  }

  TranslationOption* Trie::FindPhrasePair(vector<string> &srcphra, string translation)
  {
    TrieNode* node = FindNode(srcphra, m_root, 0);
    if (!node){
      return NULL;
    }
    else{
      for (TranslationOption* option : node->transptr){
        if (option->translation == translation){
          return option;
        }
      }
      return NULL;
    }
  }

  TrieNode* Trie::FindNode(vector<string> &srcphra, TrieNode* start, size_t pos)
  {
    if (pos == srcphra.size()){
      if (start->terminate){
        return start;
      }
      else{
        return NULL;
      }
    }
    string curword = srcphra[pos];
    TrieNode* ptr = start->son;
    while (ptr){
      if (ptr->word == curword){
        return FindNode(srcphra, ptr, ++pos);
      }
      ptr = ptr->next;
    }
    if (!ptr){
      return NULL;
    }
  }

  void Trie::Clear(TrieNode* start)
  {
    if (start->son){
      Clear(start->son);
    }
    else if (start->next){
      Clear(start->next);
    }
    else{
      delete start;
    }
  }
}