#include "MPDU.h"

//#include <qdebug.h>


desenet::MPDU::MPDU():Frame(Mtu){
    setType(FrameType::MPDU);
}

uint8_t MPDU::getEPDUCount()
{
    uint8_t count;
    std::memcpy(&count,buffer() + HEADER_SIZE + EPDUCOUNT_OFFSET , sizeof (uint8_t)); // HeaderSIZE gets frametype position , EPDU count is 8 bytes later (1 FrameType, 7 Sensor ID)
    return  count;
}

void desenet::MPDU::commitSv(SvGroup g, uint8_t byteCount){
    EPDUH_U8 header;
    header.epduh.type = 0;
    header.epduh.svGroupOrEvId = g;
    header.epduh.count = byteCount;
    // Copy in buffer
    std::memcpy(buffer()+length(),&header.byte,EPDU_HEADER_SIZE);
    int count = getEPDUCount();
    setEPDUCount(count+1);
    setLength(length() + byteCount + EPDU_HEADER_SIZE);
}

void MPDU::commitEv(EvId id, uint8_t byteCount)
{
    EPDUH_U8 header;
    header.epduh.type = 1; // Type = 1 for event PDU
    header.epduh.svGroupOrEvId = id;
    header.epduh.count = byteCount;

    //Write in buffer
    std::memcpy(buffer()+length(),&header.byte,EPDU_HEADER_SIZE);
    int count = getEPDUCount()+1;
    setEPDUCount(count);
    setLength(length()+ byteCount+ EPDU_HEADER_SIZE);
}
SharedByteBuffer MPDU::insertBuffer()
{
    SharedByteBuffer b;
    return b.proxy(buffer()+length()+ EPDU_HEADER_SIZE,reservedLength()-length()-1);
}
void MPDU::Reset()
{
    setDestination(GATEWAY_ADDRESS);
    SlotNumber sensorID = DESENET_SLOT_NUMBER;
    std::memcpy(buffer() + HEADER_SIZE + SENSORID_OFFSET,&sensorID,sizeof (desenet::SlotNumber));
    setType(FrameType::MPDU);
    setEPDUCount(0);
    setLength(HEADER_SIZE+FOOTER_SIZE+MPDU_HEADER_SIZE);
}
void MPDU::setEPDUCount(int cnt)
{
    ePDUCount = cnt;
    std::memcpy(buffer()+ HEADER_SIZE + EPDUCOUNT_OFFSET,&ePDUCount,sizeof (uint8_t));
}

uint8_t *MPDU::getBuffer()
{
    return buffer();
}

uint8_t MPDU::getRemainingSpace()
{
    return Mtu - HEADER_SIZE - FOOTER_SIZE - length(); //Return the remaining bytes size in the MPDU buffer
}

size_t MPDU::evPDUwrite(const SharedByteBuffer &evData){ //Used for events PDU to write the data in the MPDU

    //qDebug() << "evData content : " << *evData.data();
    //qDebug() << "evData length : " << evData.length();
    //qDebug() << "sizeof evData : " << sizeof (evData);
    //uint8_t* ptTest = new uint8_t();
    //*ptTest = 4;
    //qDebug() << "Test pointer value : " << *ptTest;
    std::memcpy(buffer()+length()+EPDU_HEADER_SIZE,evData.data(),evData.length());
    return evData.length();
}
