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

#include "LexicalTransPipeline.h";
#include "LexicalTranslation.h";

#include <iostream>

namespace OrangeTraining
{
	LexicalTransPipeline::LexicalTransPipeline(){}

	void LexicalTransPipeline::Start(string &psrc, string &ptgt, string &palign
		, string &ps2t, string &pt2s)
	{
		if (psrc == "" || ptgt == "" || palign == ""
			|| ps2t == "" || pt2s == ""){
			cerr << "ERROR: Please check file paths!" << endl;
			return;
		}

		LexicalTranslation* lexbuilder = new LexicalTranslation();
		cerr << "Building lexicon translation..." << endl;
		lexbuilder->BuildLexicon(psrc, ptgt, palign);
		cerr << "Writing result..." << endl;
		lexbuilder->Write(ps2t, pt2s);
	}
}