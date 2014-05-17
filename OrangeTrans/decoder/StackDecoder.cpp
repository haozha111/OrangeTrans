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

#include "StackDecoder.h"
#include "..\util\Utility.h"
#include <iostream>
#include <stack>

using namespace OrangeTrans;

namespace OrangeDecoder
{
  DecodeParams::DecodeParams()
    : transWgt(0.25)
    , lmWgt(0.25)
    , reorderWgt(0.25)
    , futurecostWgt(0.25)
    , transpWgt(0.25)
    , transplexWgt(0.25)
    , transpinvWgt(0.25)
    , transplexinvWgt(0.25)
    , tgtlen(15){}

  StackDecoder::StackDecoder(DecodeParams &decodeParams, DecodeOption &decodeOption)
  {
    m_decodeParams = decodeParams;
    m_decodeOption = decodeOption;
    m_futureCost = NULL;
    m_lmProvider = new KnerserNeyBackOff();
  }

  void StackDecoder::ReadPhraseTable(string path)
  {
    ifstream fin(path);
    if (!fin){
      cerr << "Cannot load phrase table. Please check the path: "
        << path << endl;
      return;
    }

    cerr << "Start loading phrase table." << endl;
    string line;
    size_t lineNo = 1;
    while (getline(fin, line)){
      if (lineNo % 100000 == 0){
        cerr << "Processed line: " << lineNo << "." << endl;
      }
      lineNo++;
      vector<string> tmp = Utility::Split(line, " ||| ");
      string srcphra = tmp[0];
      TranslationOption* translationOption = new TranslationOption();
      translationOption->translation = tmp[1];
      double a, b;
      sscanf_s(tmp[2].c_str(), "%lf %lf %lf %lf %lf %lf",
        &translationOption->p, &translationOption->plex,
        &translationOption->pinv, &translationOption->plexinv,
        &a, &b);
      m_phratbl.AddTranslationOption(srcphra, translationOption);
    }
    fin.close();
    cerr << "Successfully loaded phrase table." << endl;
  }

  void StackDecoder::ReadMSDTable(string path)
  {
    ifstream fin(path);
    if (!fin){
      cerr << "Cannot load msd reordering table. Please check the path: "
        << path << endl;
      return;
    }

    cerr << "Start loading msd reordering table." << endl;
    string line;
    size_t lineNo = 1;
    while (getline(fin, line)){
      if (lineNo % 100000 == 0){
        cerr << "Processed line: " << lineNo << "." << endl;
      }
      lineNo++;
      vector<string> tmp = Utility::Split(line, " ||| ");
      string srcphra = tmp[0];
      string tgtphra = tmp[1];
      double prevM, prevS, prevD, nextM, nextS, nextD;
      sscanf_s(tmp[2].c_str(), "%lf %lf %lf %lf %lf %lf",
        &prevM, &prevS, &prevD,
        &nextM, &nextS, &nextD);
      TranslationOption* option = m_phratbl.FindTransOption(srcphra, tgtphra);
      if (option){
        option->prevM = prevM;
        option->prevS = prevS;
        option->prevD = prevD;
        option->nextM = nextM;
        option->nextS = nextS;
        option->nextD = nextD;
      }
    }
    fin.close();
    cerr << "Successfully loaded msd reordering table." << endl;
  }

  void StackDecoder::Output1Best()
  {
    stack<Hypothesis*> backtrack;
    for (Hypothesis* hypo = m_hypoStacks[m_srclen].m_hypoStack[0];
      hypo->m_bestprevHypo != NULL; hypo = hypo->m_bestprevHypo){
      backtrack.push(hypo);
    }
    while (backtrack.size() > 0){
      Hypothesis* curhypo = backtrack.top();
      if (backtrack.size() > 1){
        if (curhypo->m_addedTgtphra != "NULL"){
          m_1bestOutput << curhypo->m_addedTgtphra << " ";
        }
      }
      else{
        if (curhypo->m_addedTgtphra != "NULL"){
          m_1bestOutput << curhypo->m_addedTgtphra << endl;
        }
      }
      backtrack.pop();
    }
  }

  void StackDecoder::EstimateFutureCost()
  {
    //allocate
    m_futureCost = new double*[m_srclen];
    for (int i = 0; i < m_srclen; ++i){
      m_futureCost[i] = new double[m_srclen];
      memset(m_futureCost[i], 0, m_srclen * sizeof(double));
    }


    //compute future cost using dynamic programming
    for (size_t len = 1; len <= m_srclen; ++len){
      for (size_t start = 0; start <= m_srclen - len; ++start){
        size_t end = start + len - 1;
        //unseen phrase will be assigned a score of -10
        m_futureCost[start][end] = -10;
        string phra = "";
        for (int k = start; k <= end; ++k){
          if (k < end){
            phra += m_srcsentn[k] + " ";
          }
          else{
            phra += m_srcsentn[k];
          }
        }

        if (m_phratbl.m_table.count(phra)){
          m_futureCost[start][end] = m_phratbl.GetCheapestTransCost(phra);
          m_futureCost[start][end] += m_phratbl.GetCheapestLMCost(phra, m_lmProvider);
        }

        for (size_t i = start; i < end; ++i){
          if (m_futureCost[start][i] + m_futureCost[i + 1][end] > m_futureCost[start][end]){
            m_futureCost[start][end] = m_futureCost[start][i] + m_futureCost[i + 1][end];
          }
        }
      }
    }
  }

  void StackDecoder::DecodeSentence(string srcsentn)
  {
    m_srcsentn = Utility::Split(srcsentn);
    m_srclen = m_srcsentn.size();
    //find all applicable translation options
    map<string, vector<TranslationOption*> > translationOptions;
    for (int i = 0; i < m_srclen; ++i){
      for (int j = i; j < m_srclen; ++j){
        string phra = "";
        for (int k = i; k <= j; ++k){
          if (k < j){
            phra += m_srcsentn[k] + " ";
          }
          else{
            phra += m_srcsentn[k];
          }
        }
        if (m_phratbl.m_table.count(phra)){
          translationOptions[phra] = m_phratbl.m_table[phra];
        }
        else if (j - i == 0) {
          vector<TranslationOption*> transptr;
          TranslationOption* option = new TranslationOption();
          option->p = option->plex = option->pinv = option->plexinv = 1;
          option->translation = phra;
          transptr.push_back(option);
          translationOptions[phra] = transptr;
        }
      }
    }

    // estimate future cost
    EstimateFutureCost();

    //!init hypo stacks
    for (int i = 0; i <= m_srclen; ++i){
      m_hypoStacks.push_back(HypothesisStack(m_decodeOption));
    }
    //create initial empty hypo
    Hypothesis* emptyHypo = new Hypothesis();
    emptyHypo->CreateEmptyHypo(m_srclen);
    m_hypoStacks[0].Add(emptyHypo);

    //now do hypo expansion
    for (int s = 0; s < m_hypoStacks.size() - 1; ++s){
      for (Hypothesis* currentHypo : m_hypoStacks[s].m_hypoStack){
        //apply translation options
        for (int i = 0; i < m_srclen; ++i){
          if (currentHypo->m_coverage[i] == 1){
            continue;
          }
          for (int j = i; j < m_srclen; ++j){
            if (currentHypo->m_coverage[j] == 1){
              break;
            }
            string word = "";
            for (int k = i; k <= j; ++k){
              if (k < j){
                word += m_srcsentn[k] + " ";
              }
              else{
                word += m_srcsentn[k];
              }
            }
            if (translationOptions.count(word)){
              for (TranslationOption* option : translationOptions[word]){
                Hypothesis* newhypo = new Hypothesis();
                newhypo->CreateHypo(m_srclen);
                newhypo->m_addedTgtphra = option->translation;
                newhypo->m_addedOption = option;
                for (int p = 0; p < m_srclen; ++p){
                  if (p >= i && p <= j){
                    newhypo->m_coverage[p] = 1;
                  }
                  else{
                    newhypo->m_coverage[p] = currentHypo->m_coverage[p];
                  }
                }
                newhypo->m_covercount = currentHypo->m_covercount + j - i + 1;
                newhypo->m_endofLastSrcphra = m_srcsentn[j];
                newhypo->m_endposofLastSrcPhra = j;
                newhypo->m_startposofLastSrcPhra = i;
                vector<string> tmp = Utility::Split(option->translation);

                if (tmp.size() >= 2){
                  newhypo->m_lastTwo = make_pair(tmp[tmp.size() - 2], tmp[tmp.size() - 1]);
                }
                else{
                  newhypo->m_lastTwo = make_pair(currentHypo->m_lastTwo.second, tmp[0]);
                }

                newhypo->m_prevhypo.insert(currentHypo);
                newhypo->m_bestprevHypo = currentHypo;
                //compute future cost for current hypo
                double futurecost = 0;
                int beg = -1, end;
                for (int i = 0; i < m_srclen; ++i){
                  if (newhypo->m_coverage[i] == 0){
                    if (beg == -1){
                      beg = i;
                    }
                  }
                  if (newhypo->m_coverage[i] == 1){
                    if (beg == -1){
                      continue;
                    }
                    else{
                      end = i - 1;
                      futurecost += m_futureCost[beg][end];
                      beg = -1;
                    }
                  }
                }
                if (beg != -1){
                  end = m_srclen - 1;
                  futurecost += m_futureCost[beg][end];
                }

                newhypo->m_futurecost = futurecost;

                /*compute reordering score
                  recognize reordering type: pM, pS, pD, nM, nS, nD*/
                double reordercost = 0;
                if (currentHypo->m_endposofLastSrcPhra == -1){
                  //indicating we are expanding from an empty hypo
                  if (newhypo->m_startposofLastSrcPhra == 0){
                    reordercost = option->prevM;
                  }
                  else{
                    reordercost = option->prevD;
                  }
                }
                else{
                  if (currentHypo->m_endposofLastSrcPhra == newhypo->m_startposofLastSrcPhra - 1){
                    reordercost = option->prevM + currentHypo->m_addedOption->nextM;
                  }
                  else if (currentHypo->m_startposofLastSrcPhra == newhypo->m_endposofLastSrcPhra + 1){
                    reordercost = option->prevS + currentHypo->m_addedOption->nextS;
                  }
                  else{
                    reordercost = option->prevD + currentHypo->m_addedOption->nextD;
                  }
                }

                //compute language model score
                double lmscore = 0;
                vector<string> sentence;
                sentence.push_back(currentHypo->m_lastTwo.first);
                sentence.push_back(currentHypo->m_lastTwo.second);
                for (string w : Utility::Split(newhypo->m_addedTgtphra)){
                  sentence.push_back(w);
                }
                lmscore = m_lmProvider->ScoreSentenceWithContext(sentence);

                newhypo->m_prob = currentHypo->m_prob
                  + (option->p * m_decodeParams.transpWgt
                  + option->plex * m_decodeParams.transplexWgt
                  + option->pinv * m_decodeParams.transpinvWgt
                  + option->plexinv * m_decodeParams.transplexinvWgt) * m_decodeParams.transWgt
                  + futurecost * m_decodeParams.futurecostWgt
                  + reordercost * m_decodeParams.reorderWgt
                  + lmscore * m_decodeParams.lmWgt;


                //place the new hypo into the appropriate stack
                m_hypoStacks[newhypo->m_covercount].Add(newhypo);
              }
            }
          }
        }
      }
    }

    //update the reordering score in the last stack
    for (Hypothesis* currentHypo : m_hypoStacks[m_srclen].m_hypoStack){
      if (currentHypo->m_endposofLastSrcPhra == m_srclen - 1){
        currentHypo->m_prob += currentHypo->m_addedOption->nextM * m_decodeParams.reorderWgt;
      }
      else{
        currentHypo->m_prob += currentHypo->m_addedOption->nextD * m_decodeParams.reorderWgt;
      }
    }

    //we now generate n-best list and output 1-best translations
    Output1Best();
  }

  void StackDecoder::Decode(string phrasetbl, string msdtable, string lmpath, string testset, string output1best)
  {
    cerr << "Init decoder..." << endl;
    ReadPhraseTable(phrasetbl);
    ReadMSDTable(msdtable);
    m_lmProvider->LoadGramPro(lmpath);

    ifstream fin(testset);
    m_1bestOutput.open(output1best);
    if (!fin){
      cerr << "Could not load test set: " << testset << endl;
      return;
    }
    if (!m_1bestOutput){
      cerr << "Could not create 1best output file: " << output1best << endl;
      return;
    }
    string line;
    size_t lineNo = 1;
    while (getline(fin, line)){
      cerr << "Decoding sentence: " << lineNo << "." << endl;
      DecodeSentence(line);
      Clear();
      lineNo++;
    }
    fin.close();
    m_1bestOutput.close();
  }

  StackDecoder::~StackDecoder()
  {
    m_phratbl.Clear();
    m_lmProvider->ReleaseSource();
    Clear();
  }

  void StackDecoder::Clear()
  {
    m_hypoStacks.clear();
    if (m_futureCost){
      for (int i = 0; i < m_srclen; ++i){
        delete[] m_futureCost[i];
      }
      delete[] m_futureCost;
    }
  }

}