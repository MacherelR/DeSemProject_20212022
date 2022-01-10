#include "joystickapplication.h"
#include "joystick.h"
#include "factory.h"
#include "xf/xfevent.h"
#include "platform-config.h"
#include "trace/trace.h"


using app::JoystickApplication;

//Initialize state and data
JoystickApplication::JoystickApplication() : currentState(STATE_NONE){
    joystickData = 0;
}

JoystickApplication::~JoystickApplication(){} //Nothing special to do

void JoystickApplication::initialize(){
    //Sync registering
    svSyncRequest();
}

void JoystickApplication::start(){
    startBehavior(); // Start xfReactive behavior by generating initial event
}

void JoystickApplication::onPositionChange(IJoystick::Position position){
    //qDebug() << "Position changed ! ";
    GEN(XFEvent(JOYSTICK_POSCHANGED_EVENT));
}


EventStatus JoystickApplication::processEvent(){ //Processing state machine on event reception
    eSmState nState = currentState; // Maintain new state
    SharedByteBuffer buff = SharedByteBuffer(1); // 1 byte shared buffer to contain joystick data
    IJoystick::Position currentPos; //Joystick position
    //qDebug() << "ENtering state machine with state " << currentState;
    switch(currentState){
        case STATE_NONE:
            //Do nothing
        case STATE_INIT:
        //If received event is init then move to wait state
            if(getCurrentEvent()->getEventType() == IXFEvent::Initial){
                nState = STATE_WAIT;
            }
            break;
        case STATE_WAIT:
        //need to check if the event received is a move event and if so, move to posChanged event
            if(getCurrentEvent()->getId() == JOYSTICK_POSCHANGED_EVENT && getCurrentEvent()->getEventType() == IXFEvent::Event){
                //qDebug() << "State changed first SM switch";
                nState = STATE_POSCHANGED;
            }
            break;
        case STATE_POSCHANGED:
        //Return to wait state
            nState = STATE_WAIT;
            break;
    }

    //Check if the state has changed in the state machine The only case in which we have to do something is if the position has changed (meaning another value), otherwise nothing to do
    if(nState != currentState){
        switch(nState){
            case STATE_NONE:
            //do nothing
                break;
            case STATE_INIT:
            //Do nothing
                break;
            case STATE_WAIT:
            //Do nothing
                break;
            case STATE_POSCHANGED:
                currentPos = _readJoystickValue(); //get current joystick position
                joystickData = currentPos.pressedButtons;
                buff[0] = joystickData; //Write data informations into sharedbyteBuffer
                //qDebug() << "State changed to (joystick value) " << joystickData;
                //Request to publish the event data
                //qDebug() << "buff = " << buff.data();
                evPublishRequest(EVID_JOYSTICK,buff);
                //Return to null state
                GEN(XFNullTransition());
                break;
        }
    }
    currentState = nState; // save new state value for the next call
    return EventStatus::Consumed;
}

board::Joystick &JoystickApplication::joystick()
{
    return Factory::instance().joystick();
}

IJoystick::Position JoystickApplication::_readJoystickValue()
{
    return joystick().position();
}


