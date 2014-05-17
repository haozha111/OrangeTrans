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

#include <iostream>
#include <ctime>
#include "PhraseExtractionPipeline.h"
#include "PhraseExtractor.h"
#include "Phrase.h"

using namespace std;

namespace OrangeTraining
{
  PhraseExtractionPipeline::PhraseExtractionPipeline(string srcTrainset
    , string tgtTrainset
    , string alignment
    , string ruleOuputFile
    , string ruleInvOutput)
    :m_srcTrainset(srcTrainset)
    , m_tgtTrainset(tgtTrainset)
    , m_align(alignment)
    , m_ruleOuput(ruleOuputFile)
    , m_ruleInvOutput(ruleInvOutput) {}

  bool PhraseExtractionPipeline::StartPhraseExtraction()
  {
    using namespace OrangeTrans;

    string srcline, tgtline, alignline;
    size_t sentenceID = 1;
    RuleOptions ruleOptions = RuleOptions();

    ifstream srcTrainset(m_srcTrainset.c_str());
    ifstream tgtTrainset(m_tgtTrainset.c_str());
    ifstream align(m_align.c_str());
    ofstream ruleOutput(m_ruleOuput.c_str());
    ofstream ruleInvOutput(m_ruleInvOutput.c_str());

    cerr << "Start phrase extraction..." << endl;

    PhraseCollection phraseCollection;

    while (getline(srcTrainset, srcline)
      && getline(tgtTrainset, tgtline)
      && getline(align, alignline)){
      //create a new word alignment
      WordAlignment wordAlignment;
      wordAlignment.CreateAlignment(srcline, tgtline,
        alignline, sentenceID);
      //extract phrase pairs from this alignment
      PhraseExtractor phraseExtractor = PhraseExtractor(wordAlignment, phraseCollection);
      phraseExtractor.ExtractPhrasePair(ruleOptions);
      phraseCollection.Write(ruleOutput, ruleInvOutput);
      phraseCollection.Clear();
      sentenceID++;
      if (sentenceID % 1000 == 0){
        cerr << "Processed " << sentenceID << " lines." << endl;
      }
    }
    srcTrainset.close();
    tgtTrainset.close();
    align.close();
    ruleOutput.close();
    ruleInvOutput.close();

    cerr << "Total number of phrase pair extracted: "
      << phraseCollection.TotalRuleCount() << endl;
    cerr << "Number of null phrase pair extracted: "
      << phraseCollection.NullRuleCount() << endl;
    //sort the rule table for phrase table generation phase
    Utility::Sort(m_ruleOuput);
    Utility::Sort(m_ruleInvOutput);
    //delete old rule tables
    Utility::Delete(m_ruleOuput);
    Utility::Delete(m_ruleInvOutput);
    return true;
  }
}