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
#include <vector>
#include <string>

#include "phrase extraction\PhraseExtractionPipeline.h";

using namespace std;

void main(int argc, char* argv[])
{
	using namespace OrangeTraining;
	string src = "C:\\Users\\v-haozha\\Desktop\\NiuTrans_1.3.0_CWMT2013\\sample-data\\sample\\sample-submission-version\\TM-training-set\\chinese.txt";
	string tgt = "C:\\Users\\v-haozha\\Desktop\\NiuTrans_1.3.0_CWMT2013\\sample-data\\sample\\sample-submission-version\\TM-training-set\\english.txt";
	string align = "C:\\Users\\v-haozha\\Desktop\\NiuTrans_1.3.0_CWMT2013\\sample-data\\sample\\sample-submission-version\\TM-training-set\\Alignment.txt";
	string rule = "C:\\Users\\v-haozha\\Desktop\\orange\\rule.txt";
	string ruleinv = "C:\\Users\\v-haozha\\Desktop\\orange\\ruleinv.txt";
	PhraseExtractionPipeline p = PhraseExtractionPipeline(src, tgt, align, rule, ruleinv);
	p.StartPhraseExtraction();
}

