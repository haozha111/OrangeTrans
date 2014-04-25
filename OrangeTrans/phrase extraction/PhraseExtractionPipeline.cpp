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
#include "PhraseExtractionPipeline.h";
#include "PhraseExtractor.h";
#include "Phrase.h";

using namespace std;

namespace OrangeTraining
{
	PhraseExtractionPipeline::PhraseExtractionPipeline(string sourceTrainset
		, string targetTrainset
		, string alignment
		, string ruleOuputFile
		, string ruleInvertedOutputFile)
		:m_sourceTrainset(sourceTrainset)
		, m_targetTrainset(targetTrainset)
		, m_alignment(alignment)
		, m_ruleOuputFile(ruleOuputFile)
		, m_ruleInvertedOutputFile(ruleInvertedOutputFile) {}

	bool PhraseExtractionPipeline::StartPhraseExtraction()
	{
		string line_source, line_target, line_alignment;
		PhraseCollection phraseCollection;
		size_t sentenceID = 1;
		RuleOptions ruleOptions = RuleOptions();

		ifstream sourceTrainset(m_sourceTrainset.c_str());
		ifstream targetTrainset(m_targetTrainset.c_str());
		ifstream alignment(m_alignment.c_str());
		ofstream ruleOutputFile(m_ruleOuputFile.c_str());
		ofstream ruleInvertedOutputFile(m_ruleInvertedOutputFile.c_str());

		while (getline(sourceTrainset, line_source)
			&& getline(targetTrainset, line_target)
			&& getline(alignment, line_alignment)){
			//create a new word alignment
			WordAlignment wordAlignment = WordAlignment(line_source, line_target,
				line_alignment, sentenceID);
			//extract phrase pairs from this alignment
			cerr <<  "Processing line No." << sentenceID << endl;
			PhraseExtractor phraseExtractor = PhraseExtractor(wordAlignment, phraseCollection);
			phraseExtractor.ExtractPhrasePair(ruleOptions);
			phraseCollection.WritetoFile(ruleOutputFile, ruleInvertedOutputFile);
			sentenceID++;
		}

		return true;
	}
}