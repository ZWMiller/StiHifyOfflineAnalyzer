#ifndef anaConstE_H
#define anaConstE_H

namespace anaConst
{
  // Set Cut Values
   const int numEtaBins = 20;
   float etaBinLow[20]  = {-1.0,-0.9,-0.8,-0.7,-0.6,
                           -0.5,-0.4,-0.3,-0.2,-0.1,
                            0.0, 0.1, 0.2, 0.3, 0.4,
                            0.5, 0.6, 0.7, 0.8, 0.9};
   float etaBinHigh[20] = {-0.9,-0.8,-0.7,-0.6,-0.5,
                           -0.4,-0.3,-0.2,-0.1, 0.0, 
                            0.1, 0.2, 0.3, 0.4, 0.5, 
                            0.6, 0.7, 0.8, 0.9, 1.0};

   const int numPtBins = 8;
   float ptBinLow[10]  = {0.0, 0.5, 1.0, 1.5, 2.0,
                          3.0, 4.0, 5.0};
   float ptBinHigh[10] = {0.5, 1.0, 1.5, 2.0, 3.0,
                          4.0, 5.0, 8.0};
}
#endif
