#include <assert.h>
#include <array>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include "platform-config.h"
#include "board/ledcontroller.h"
#include "desenet/frame.h"
#include "desenet/beacon.h"
#include "desenet/timeslotmanager.h"
#include "abstractapplication.h"
#include "networkentity.h"

using std::array;
using std::list;
using std::map;
using std::make_pair;
using std::bind;
using std::pair;
using std::vector;

using desenet::sensor::NetworkEntity;

NetworkEntity * NetworkEntity::_pInstance(nullptr);		// Instantiation of static attribute

NetworkEntity::NetworkEntity()
 : _pTimeSlotManager(nullptr),
   _pTransceiver(nullptr)
{
	assert(!_pInstance);		// Only one instance allowed
	_pInstance = this;
    evList = EventElementList();
    for(SvGroup it = 0 ; it < 16 ; it ++){ //Reinitialize publishArray
        publishArray[it] = nullptr;
    }

}

NetworkEntity::~NetworkEntity()
{
}

void NetworkEntity::initialize(const desenet::SlotNumber& slotNumber)
{
    this->slotNumber = slotNumber;

}

void NetworkEntity::initializeRelations(ITimeSlotManager & timeSlotManager, NetworkInterfaceDriver & transceiver)
{
	_pTimeSlotManager = &timeSlotManager;
    _pTransceiver = &transceiver;

    // Add this network entity as observer in timeslotmanager
    _pTimeSlotManager->initializeRelations(*this);

     // Set the receive callback between transceiver and network. Bind this pointer to member function
    transceiver.setReceptionHandler(std::bind(&NetworkEntity::onReceive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

/**
 * This method does not automatically create an instance if there is none created so far.
 * It is up the the developer to create an instance of this class prior to access the instance() method.
 */
//static
NetworkEntity & NetworkEntity::instance()
{
	assert(_pInstance);
    return *_pInstance;
}

void NetworkEntity::svSyncRequest(AbstractApplication *app)
{
    syncList.push_back(app);

}

bool NetworkEntity::svPublishRequest(SvGroup group, AbstractApplication *app)
{
    if(publishArray[group] != nullptr){
        return false;
     }
     else{
        publishArray[group] = app;
        return true;
     }
}

void NetworkEntity::eventReceived(const EvId &id, const SharedByteBuffer &evData)
{
    // Create evPDU frame and add it in the queue
    //qDebug() << "evData = " << evData.data();
    //qDebug() << "evData[0]" << evData.data()[0];
    EventElement newEvent(id, evData.copy());
    evQueue.push_back(newEvent);
}

void NetworkEntity::onTimeSlotSignal(const ITimeSlotManager &timeSlotManager, const ITimeSlotManager::SIG &signal)
{
    if (signal == ITimeSlotManager::SIG::OWN_SLOT_START) {
        // Send the MPDU frame
        transceiver().transmit(responseMPDU.getBuffer(),responseMPDU.length());
    }
}

/**
 * Called by the NetworkInterfaceDriver (layer below) after reception of a new frame
 */
void NetworkEntity::onReceive(NetworkInterfaceDriver & driver, const uint32_t receptionTime, const uint8_t * const buffer, size_t length)
{
    (void)(driver);
    (void)(receptionTime);
	Frame frame = Frame::useBuffer(buffer, length);
    // Actions differs on frame type
    switch (frame.type()) {
    case FrameType::Beacon:
    {
        responseMPDU.Reset();
        LedController::instance().flashLed(0);
        Beacon beacon(frame);
        //NOtify TimeSlotManager of reception
        timeSlotManager().onBeaconReceived((beacon.slotDuration()));

        //inform all sync registered applications of beacon reception
        for(ApplicationSyncList::const_iterator it = syncList.begin(); it != syncList.end(); it++){
            (*it)->svSyncIndication(beacon.networkTime());
        }
        // Write values into MPDU
        for(SvGroup svGroupIt = 0; svGroupIt < SVGROUP_MAX ; svGroupIt++){
            if(publishArray[svGroupIt] != nullptr){
                if(beacon.svGroupMask()[svGroupIt]){ //Check if the beacons asks for the current group
                    SharedByteBuffer svBuff = responseMPDU.insertBuffer();
                    //Add values into the buffer
                    SharedByteBuffer::sizeType length = publishArray[svGroupIt]->svPublishIndication(svGroupIt,svBuff);
                    responseMPDU.commitSv(svGroupIt,length);
                    //qDebug() << length;
                }
            }
        }
        // Add as many events as possible by the remaining size of the MPDU
        if(!evQueue.empty()){//Check that we have queued events
            while(evQueue.begin()->data.size() <= responseMPDU.getRemainingSpace()){ //Check that there's enough remaining space for a evPDU and insert them while the space is enough
                uint8_t length = responseMPDU.evPDUwrite(evQueue.begin()->data); //Write evPDU with first element of the list
                responseMPDU.commitEv(evQueue.begin()->id,length); //Commit that evPDU
                evQueue.pop_front(); // Remove first element of the list
            }
        }
        break;
    }
    case FrameType::MPDU:
    {
        printf(("Frame received \n"));
        break;
    }
    case FrameType::Invalid:
        printf("invalid frameType !\n");
        break;

    default:
        break;
    }
}

