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
#ifndef PHRASE_EXTRACTION_PIPELIN_H_INCLUDED_
#define PHRASE_EXTRACTION_PIPELIN_H_INCLUDED_

#include <fstream>
#include <string>

namespace OrangeTraining
{
	class PhraseExtractionPipeline
	{
	public:
		PhraseExtractionPipeline(std::string sourceTrainset
			, std::string targetTrainset
			, std::string alignment
			, std::string ruleOuputFile
			, std::string ruleInvertedOutputFile);

		bool StartPhraseExtraction();
	private:
		std::string m_sourceTrainset;
		std::string m_targetTrainset;
		std::string m_alignment;
		std::string m_ruleOuputFile;
		std::string m_ruleInvertedOutputFile;
	};
}

#endif