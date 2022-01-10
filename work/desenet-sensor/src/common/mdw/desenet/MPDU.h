#ifndef MPDU_H
#define MPDU_H

#include <stdint.h>
#include "desenet/types.h"
#include "desenet/frame.h"
#include"platform-config.h"
namespace desenet {

struct EPDUH{
    //reverse order!
    unsigned count : 3;
    unsigned svGroupOrEvId : 4;
    unsigned type : 1;
};

union EPDUH_U8{
    EPDUH epduh;
    uint8_t byte;
};

#define EPDUCOUNT_OFFSET 1
#define EPDU_HEADER_SIZE 1
#define SENSORID_OFFSET 0
#define MPDU_HEADER_SIZE 2
#define FOOTER_SIZE 0


class MPDU: public Frame {
//    static const uint8_t MTU = Frame::HEADER_SIZE +32 + FOOTER_SIZE;
public:
    MPDU();
    uint8_t getEPDUCount();
    void commitSv(SvGroup g, uint8_t byteCount);
    void commitEv(EvId id, uint8_t byteCount);
    SharedByteBuffer insertBuffer();
    void Reset();

    void setEPDUCount(int cnt);
    uint8_t* getBuffer();
    uint8_t getRemainingSpace();
    size_t evPDUwrite(const SharedByteBuffer &evData);
private:
    int mtu = 37;
    int ePDUCount;
    SharedByteBuffer mBuffer;
    uint8_t actualLength;
//    Frame frame;
};
}

#endif // MPDU_H
