#ifndef PROGRESS_H_A34EF856
#define PROGRESS_H_A34EF856

#include <string>

class progress {
  private:
    std::string message;
    int total;
    int cur = 0;
    int lprint = 0;
    
  public:
    progress(std::string message, int total) : message(message), total(total)
    {
      std::cout << message << "   0%" << std::flush;
    }
    
    void update(int val)
    {
      if (val <= total)
      {
        cur = val;
      } else {
        cur = total;
      }
      
      int pp = cur * 100 / total;
      if (pp != lprint)
      {
        lprint = pp;
      
        std::cout << "\b\b\b\b" << std::right;
        std::cout.width(3);
        std::cout << pp << "%" << std::flush;
      }
    }
    
    void update()
    {
      update(cur+1);
    }
    
    ~progress()
    {
      std::cout << "\b\b\b\b100%" << std::endl;
    }
};

#endif /* end of include guard: PROGRESS_H_A34EF856 */
