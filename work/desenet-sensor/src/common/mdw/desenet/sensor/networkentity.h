#ifndef SENSOR_NETWORK_ENTITY_H
#define SENSOR_NETWORK_ENTITY_H

#include <assert.h>
#include <list>
#include <array>
#include "platform-config.h"
#include "desenet/desenet.h"
#include "desenet/networkinterfacedriver.h"
#include "desenet/itimeslotmanager.h"
#include "desenet/MPDU.h"

using desenet::NetworkInterfaceDriver;
using namespace desenet;

class LedController;

namespace desenet {
namespace sensor {

class AbstractApplication;
class Net;

#define SVGROUP_MAX 16
/**
 * @brief Implements the desenet protocol on the network layer.
 */
class NetworkEntity : public ITimeSlotManager::Observer
{
    friend class AbstractApplication;
    friend class Net;

public:
    NetworkEntity();
    virtual ~NetworkEntity();

    void initialize(const desenet::SlotNumber& slotNumber);																						///< Initializes the instance.
    void initializeRelations(ITimeSlotManager & timeSlotManager, NetworkInterfaceDriver & transceiver);		///< Initializes all relations needed by the instance.

    static NetworkEntity & instance();																		///< Returns reference to single instance.
    void svSyncRequest(AbstractApplication* app);
    bool svPublishRequest(SvGroup group, AbstractApplication* app);
    void eventReceived(const EvId& id, const SharedByteBuffer& evData);
    void onTimeSlotSignal(const ITimeSlotManager & timeSlotManager, const ITimeSlotManager::SIG & signal);

protected:
    /**
     * @brief Holds event information.
     */
    struct EventElement
    {
        EventElement(EvId id, const SharedByteBuffer & data)
         : id(id), data(data)
        {}

        EvId id;						///< Event identifier (ex. EVID_JOYSTICK).
        const SharedByteBuffer data;	///< Data that goes together with the event.
    };

    // desenet::NetworkInterfaceDriver::Callback callback
protected:
    /**
     * @brief Called by the lower layer after reception of a new frame
     */
    void onReceive(NetworkInterfaceDriver & driver, const uint32_t receptionTime, const uint8_t * const buffer, size_t length);

protected:
    inline ITimeSlotManager & timeSlotManager() const { assert(_pTimeSlotManager); return *_pTimeSlotManager; }	///< Internal access to TimeSlotManager
    inline NetworkInterfaceDriver & transceiver() const { assert(_pTransceiver); return *_pTransceiver; }		///< Internal access to Transceiver

protected:
    typedef std::list<AbstractApplication *> ApplicationSyncList;
    typedef std::array<AbstractApplication *, 16> ApplicationPublishersArray;
    typedef std::list<EventElement> EventElementList;

protected:
    static NetworkEntity * _pInstance;				///< Pointer to single instance.
    ITimeSlotManager * _pTimeSlotManager;			///< Pointer to TimeSlotManager.
    NetworkInterfaceDriver * _pTransceiver;			///< Pointer to transceiver.
    EventElementList evList;
    ApplicationPublishersArray publishArray;
    ApplicationSyncList syncList;
    MPDU responseMPDU;
    desenet::SlotNumber slotNumber;
    EventElementList evQueue;


};

} // sensor
} // desenet
#endif // SENSOR_NETWORK_ENTITY_H
