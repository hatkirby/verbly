#include <iostream>
#include <exception>
#include "generator.h"

void printUsage()
{
  std::cout << "usage: generator verbnet agid wordnet cmudict imagenet output" << std::endl;
  std::cout << "verbnet  :: path to a VerbNet data directory" << std::endl;
  std::cout << "agid     :: path to an AGID infl.txt file" << std::endl;
  std::cout << "wordnet  :: path to a WordNet prolog data directory" << std::endl;
  std::cout << "cmudict  :: path to a CMUDICT pronunciation file" << std::endl;
  std::cout << "imagenet :: path to an ImageNet urls.txt file" << std::endl;
  std::cout << "output   :: datafile output path" << std::endl;
}

int main(int argc, char** argv)
{
  if (argc == 7)
  {
    try
    {
      verbly::generator::generator app(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

      try
      {
        app.run();
      } catch (const std::exception& e)
      {
        std::cout << e.what() << std::endl;
      }
    } catch (const std::exception& e)
    {
      std::cout << e.what() << std::endl;
      printUsage();
    }
  } else {
    std::cout << "verbly datafile generator" << std::endl;
    printUsage();
  }
}
