/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   filter.h
 *
 *  \brief
 *    Interface file for simple filter creator
 */

#include <vector>

typedef struct SAMPLES SAMPLES;

class Filter
{
  struct COEFFICENT
  {
    float coefficent = 0.f;
    unsigned sampleDelay = 0u;
  };

  public:
    Filter();
    ~Filter();

    void add_coefficent(const COEFFICENT &coefficent);

    void apply_filter(SAMPLES &samples);
  private:
  std::vector<COEFFICENT> coefficents;
};
