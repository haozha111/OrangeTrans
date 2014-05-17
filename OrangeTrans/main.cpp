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

#include "phrase extraction\PhraseExtractionPipeline.h"
#include "phrase extraction\LexicalTransPipeline.h"
#include "phrase extraction\PhraseTransTblBuilder.h"
#include "phrase extraction\PhraseTransTblBuilder.h"
#include "phrase extraction\PhraseTblFilter.h"
#include "lexical reordering\msd.h"
#include "decoder\StackDecoder.h"

using namespace std;

void main(int argc, char* argv[])
{
  using namespace OrangeTraining;
  using namespace OrangeDecoder;

  string src = "C:\\Users\\Administrator\\Desktop\\orange\\data\\chinese.txt";
  string tgt = "C:\\Users\\Administrator\\Desktop\\orange\\data\\english.txt";
  string align = "C:\\Users\\Administrator\\Desktop\\orange\\data\\Alignment.txt";
  string rule = "C:\\Users\\Administrator\\Desktop\\orange\\work\\extract\\rule";
  string ruleinv = "C:\\Users\\Administrator\\Desktop\\orange\\work\\extract\\rule.inv";
  //PhraseExtractionPipeline p = PhraseExtractionPipeline(src, tgt, align, rule, ruleinv);
  //p.StartPhraseExtraction();

  /*******************************/
  string s2t = "C:\\Users\\Administrator\\Desktop\\orange\\work\\lex\\s2t.lex";
  string t2s = "C:\\Users\\Administrator\\Desktop\\orange\\work\\lex\\t2s.lex";

  //LexicalTransPipeline lexpipe;
  //lexpipe.Start(src, tgt, align, s2t, t2s);

  string plexs2t = "C:\\Users\\Administrator\\Desktop\\orange\\work\\lex\\s2t.lex";
  string plext2s = "C:\\Users\\Administrator\\Desktop\\orange\\work\\lex\\t2s.lex";
  string output = "C:\\Users\\Administrator\\Desktop\\orange\\work\\model\\phrasetable";
  //PhraseTransTblBuilder builder = PhraseTransTblBuilder(rule + ".sorted", ruleinv + ".sorted", output, plexs2t, plext2s);
  //builder.BuildPhraseTransTbl();

  //PhraseTblFilter filter = PhraseTblFilter(output, output + ".pruned");
  //filter.Filter(30);

  string msdoutputPath = "C:\\Users\\Administrator\\Desktop\\orange\\work\\model\\";

  //MSD reorderTrainer = MSD(src, tgt, align, msdoutputPath);
  //reorderTrainer.TrainMSD();

  string lmpath = "C:\\Users\\Administrator\\Desktop\\orange\\work\\model\\KnerserNeyBackOff\\";
  string testset = "C:\\Users\\Administrator\\Desktop\\orange\\work\\test\\test.txt";
  string output1best = "C:\\Users\\Administrator\\Desktop\\orange\\work\\1best.txt";
  DecodeParams param;
  DecodeOption option = DecodeOption(PruningMethod::Threshold);
  StackDecoder* stackDecoder = new StackDecoder(param, option);
  stackDecoder->Decode(output + ".pruned", msdoutputPath + "msd.reorder.table", lmpath, testset, output1best);

  system("pause");
}

