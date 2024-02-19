#include <vpad/input.h>
#include <padscore/kpad.h>

enum UPADError {
    //No problem
    UPAD_READ_SUCCESS,
    //No controllers could be detected
    UPAD_NO_CONTROLLERS,
    
};

enum UPADButtons {
    //A button on a controller.
    UPAD_BUTTON_A = 1 << 0,
    //B button on a controller.
    UPAD_BUTTON_B = 1 << 1,
    //X button on a controller. Also mapped to 1 on standalone Wiimote.
    UPAD_BUTTON_X = 1 << 2,
    //Y button on a controller. Also mapped to 2 on standalone Wiimote.
    UPAD_BUTTON_Y = 1 << 3,
    //D-pad left on a controller.
    UPAD_BUTTON_LEFT = 1 << 4,
    //D-pad right on a controller. Will also count for any virtual d-pad for a thumbstick.
    UPAD_BUTTON_RIGHT = 1 << 5,
    //D-pad up on a controller. Will also count for any virtual d-pad for a thumbstick.
    UPAD_BUTTON_UP = 1 << 6,
    //D-pad down on a controller. Will also count for any virtual d-pad for a thumbstick.
    UPAD_BUTTON_DOWN = 1 << 7,
    //Plus button on a controller. Maps to start, guide, etc. on relevant controllers.
    UPAD_BUTTON_PLUS = 1 << 8,
    //Minus button on a controller. Maps to select, menu, etc. buttons on relevant controllers.
    UPAD_BUTTON_MINUS = 1 << 9,
    //Home button on a controller. Maps to Xbox, Playstation, etc. buttons on relevant controllers.
    UPAD_BUTTON_HOME = 1 << 10,
};

class UPAD {
private:

    void VPADtoUPAD(VPADChan chanl);
    void KPADtoUPAD(KPADChan chanl);    

public:
    /**
     * @brief The error(s) encountered by UPAD, if any. 
     * 
     * Resets after calling read().
     */
    UPADError error;
    
    uint32_t hold;
    uint32_t trigger;
    uint32_t release;

    /**
     * @brief Read controller data. Check the error variable for errors. 
     * 
     */
    void read();

    UPAD();
    ~UPAD();

};
