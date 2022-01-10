#ifndef JOYSTICKAPPLICATION_H
#define JOYSTICKAPPLICATION_H

#include "platform-config.h"
#include "xf/xfreactive.h"
#include "desenet/sensor/abstractapplication.h"
#include "../common/platform/platform-common/board/interfaces/ijoystickobserver.h"

namespace board {
    class Joystick;
}

#define JOYSTICK_POSCHANGED_EVENT 1

namespace app
{

class JoystickApplication : public XFReactive, public desenet::sensor::AbstractApplication, public IJoystickObserver
{
public:
    JoystickApplication();
    virtual ~JoystickApplication();

    void initialize();
    void start();

protected:
    virtual EventStatus processEvent();		///< Implements the state machine.

protected:

    // State machine states
    typedef enum{
        STATE_NONE = 0,
        STATE_INIT = 1,
        STATE_WAIT = 2,
        STATE_POSCHANGED = 3
    } eSmState;
    eSmState currentState; // Remember current state


protected:
    board::Joystick & joystick();

    IJoystick::Position _readJoystickValue(); // Read actual joystick value

    void onPositionChange( IJoystick::Position position );

protected:
    uint8_t joystickData;		// Joystick values read at sync indication
};

} // namespace app

#endif // JOYSTICKAPPLICATION_H

