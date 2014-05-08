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
#ifndef UTILITY_H_INCLUDED_
#define UTILITY_H_INCLUDED_

#include <string>
#include <vector>
#include <cstdlib>

namespace OrangeTrans
{
  /** The Utility class provides basic functions that are publicly accessible
  **/
  class Utility
  {
  public:
    //! split string with delimiter
    static std::vector<std::string> Split(const std::string &stringtoSplit, const std::string &delimiter);

    //! convert a sentence to a vector of words, use space to delimit
    static std::vector<std::string> Split(const std::string &stringtoSplit);

    //! sort the given file. requires cygwin(64) installed
    static void Sort(std::string &inputFile);

    //! delete the input file
    static void Delete(std::string &inputFile);

    //! rename the input file
    static void Rename(std::string &inputFile, std::string &outputFile);
  };
}

#endif