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
#include "PhraseTblFilter.h";
#include "..\util\Utility.h";
#include <fstream>
#include <map>
#include <algorithm>

using namespace OrangeTrans;

namespace OrangeTraining
{
  PhraseTblFilter::PhraseTblFilter(const string &pruletable, const string &output)
    : m_pruletable(pruletable)
    , m_output(output)
  {}

  bool PhraseTblFilter::cmp(const pair<string, size_t> &a, const pair<string, size_t> &b)
  {
    return a.second > b.second;
  }


  /**Filter the translation table with a threshold topK
  *Phrase pairs share the same source are pruned according
  *their frequency. We only keep topK translation options for
  *each source phrase.
  */
  void PhraseTblFilter::Filter(size_t topK) const
  {
    ifstream fin(m_pruletable);
    ofstream fout(m_output);
    if (!fin){
      cerr << "Could not open " << m_pruletable << endl;
      return;
    }
    if (!fout){
      cerr << "Could not create " << m_output << endl;
      return;
    }
    string line;
    vector<pair<string, size_t> >buffer;
    string lastsource;
    int lineNo = 1;
    while (getline(fin, line)){
      if (lineNo % 10000 == 0){
        cerr << "Processed " << lineNo << " lines." << endl;
      }
      lineNo++;
      vector<string> tmp = Utility::Split(line, " ||| ");
      size_t freq = atoi(tmp[3].c_str());
      if (buffer.empty()){
        buffer.push_back(make_pair(line, freq));
        lastsource = tmp[0];
      }
      else{
        if (tmp[0] == lastsource){
          buffer.push_back(make_pair(line, freq));
        }
        else{
          sort(buffer.begin(), buffer.end(), cmp);
          for (int i = 0; i < buffer.size() && i < topK; ++i){
            fout << buffer[i].first << endl;
          }
          buffer.clear();
          lastsource = tmp[0];
          buffer.push_back(make_pair(line, freq));
        }
      }
    }
    //don't forget the left items in buffer
    sort(buffer.begin(), buffer.end(), cmp);
    for (int i = 0; i < buffer.size() && i < topK; ++i){
      fout << buffer[i].first << endl;
    }
    buffer.clear();
    fin.close();
    fout.close();
  }
}