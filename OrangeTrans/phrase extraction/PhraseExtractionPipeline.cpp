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
#include "PhraseExtractionPipeline.h";
#include "PhraseExtractor.h";
#include "Phrase.h";

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
		string srcline, tgtline, alignline;
		size_t sentenceID = 1;
		RuleOptions ruleOptions = RuleOptions();

		ifstream srcTrainset(m_srcTrainset.c_str());
		ifstream tgtTrainset(m_tgtTrainset.c_str());
		ifstream align(m_align.c_str());
		ofstream ruleOutput(m_ruleOuput.c_str());
		ofstream ruleInvOutput(m_ruleInvOutput.c_str());

		while (getline(srcTrainset, srcline)
			&& getline(tgtTrainset, tgtline)
			&& getline(align, alignline)){
			//create a new word alignment
			PhraseCollection phraseCollection;
			WordAlignment wordAlignment;
			wordAlignment.CreateAlignment(srcline, tgtline,
				alignline, sentenceID);
			
			clock_t start = clock();

			//extract phrase pairs from this alignment
			cerr <<  "Processing line No." << sentenceID << endl;
			PhraseExtractor phraseExtractor = PhraseExtractor(wordAlignment, phraseCollection);
			//cout << clock() - start << endl;

			phraseExtractor.ExtractPhrasePair(ruleOptions);
			//cout << clock() - start << endl;

			phraseCollection.Write(ruleOutput, ruleInvOutput);
			//cout << clock() - start << endl;
			sentenceID++;
		}
		srcTrainset.close();
		tgtTrainset.close();
		align.close();
		ruleOutput.close();
		ruleInvOutput.close();
		return true;
	}
}