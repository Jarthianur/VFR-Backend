#ifndef CONNECTINADSB_H_
#define CONNECTINADSB_H_

#include "ConnectIn.h"

class ConnectInADSB : public ConnectIn
{
public:
   ConnectInADSB(const char*, const int);
   virtual ~ConnectInADSB();

   int connectIn();

   int getAdsbInSock() const;

};

#endif /* CONNECTINADSB_H_ */