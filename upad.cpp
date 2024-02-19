#include "upad.h"

typedef struct button_struct {
    uint32_t orig_button;
    UPADButtons u_button;
};

//Home, A, B, X, Y, Plus, Minus, Up, Down, Left, Right
static button_struct vpad_buttons[] = {
    {VPAD_BUTTON_HOME, UPAD_BUTTON_HOME},
    {VPAD_BUTTON_A, UPAD_BUTTON_A},
    {VPAD_BUTTON_B, UPAD_BUTTON_B},
    {VPAD_BUTTON_X, UPAD_BUTTON_X},
    {VPAD_BUTTON_Y, UPAD_BUTTON_Y},
    {VPAD_BUTTON_PLUS, UPAD_BUTTON_PLUS},
    {VPAD_BUTTON_MINUS, UPAD_BUTTON_MINUS},
    {VPAD_BUTTON_UP | VPAD_STICK_L_EMULATION_UP, UPAD_BUTTON_UP},
    {VPAD_BUTTON_DOWN | VPAD_STICK_L_EMULATION_DOWN, UPAD_BUTTON_DOWN},
    {VPAD_BUTTON_LEFT | VPAD_STICK_L_EMULATION_LEFT, UPAD_BUTTON_LEFT},
    {VPAD_BUTTON_RIGHT | VPAD_STICK_L_EMULATION_RIGHT, UPAD_BUTTON_RIGHT},
};

//Home, A, B, Plus, Minus, Up, Down, Left, Right
static button_struct kpad_base_buttons[9] = {
    {WPAD_BUTTON_HOME, UPAD_BUTTON_HOME},
    {WPAD_BUTTON_A, UPAD_BUTTON_A},
    {WPAD_BUTTON_B, UPAD_BUTTON_B},
    {WPAD_BUTTON_PLUS, UPAD_BUTTON_PLUS},
    {WPAD_BUTTON_MINUS, UPAD_BUTTON_MINUS},
    {WPAD_BUTTON_UP, UPAD_BUTTON_UP},
    {WPAD_BUTTON_DOWN, UPAD_BUTTON_DOWN},
    {WPAD_BUTTON_LEFT, UPAD_BUTTON_LEFT},
    {WPAD_BUTTON_RIGHT, UPAD_BUTTON_RIGHT},
};

//X, Y
static button_struct kpad_standalone_buttons[] = {
    {WPAD_BUTTON_1, UPAD_BUTTON_X},
    {WPAD_BUTTON_2, UPAD_BUTTON_Y},
};

//X, Y, Up, Down, Left, Right
static button_struct kpad_nun_buttons[] = {
    {WPAD_NUNCHUK_BUTTON_C, UPAD_BUTTON_X},
    {WPAD_NUNCHUK_BUTTON_Z, UPAD_BUTTON_Y},
    {WPAD_NUNCHUK_STICK_EMULATION_UP, UPAD_BUTTON_UP},
    {WPAD_NUNCHUK_STICK_EMULATION_DOWN, UPAD_BUTTON_DOWN},
    {WPAD_NUNCHUK_STICK_EMULATION_LEFT, UPAD_BUTTON_LEFT},
    {WPAD_NUNCHUK_STICK_EMULATION_RIGHT, UPAD_BUTTON_RIGHT},
};

//Home, A, B, X, Y, Plus, Minus, Up, Down, Left, Right
static button_struct kpad_classic_buttons[] = {
    {WPAD_CLASSIC_BUTTON_HOME, UPAD_BUTTON_HOME},
    {WPAD_CLASSIC_BUTTON_A, UPAD_BUTTON_A},
    {WPAD_CLASSIC_BUTTON_B, UPAD_BUTTON_B},
    {WPAD_CLASSIC_BUTTON_X, UPAD_BUTTON_X},
    {WPAD_CLASSIC_BUTTON_Y, UPAD_BUTTON_Y},
    {WPAD_CLASSIC_BUTTON_PLUS, UPAD_BUTTON_PLUS},
    {WPAD_CLASSIC_BUTTON_MINUS, UPAD_BUTTON_MINUS},
    {WPAD_CLASSIC_BUTTON_UP | WPAD_CLASSIC_STICK_L_EMULATION_UP, UPAD_BUTTON_UP},
    {WPAD_CLASSIC_BUTTON_DOWN | WPAD_CLASSIC_STICK_L_EMULATION_DOWN, UPAD_BUTTON_DOWN},
    {WPAD_CLASSIC_BUTTON_LEFT | WPAD_CLASSIC_STICK_L_EMULATION_LEFT, UPAD_BUTTON_LEFT},
    {WPAD_CLASSIC_BUTTON_RIGHT | WPAD_CLASSIC_STICK_L_EMULATION_RIGHT, UPAD_BUTTON_RIGHT},
};

//Home, A, B, X, Y, Plus, Minus, Up, Down, Left, Right
static button_struct kpad_pro_buttons[] = {
    {WPAD_PRO_BUTTON_HOME, UPAD_BUTTON_HOME},
    {WPAD_PRO_BUTTON_A, UPAD_BUTTON_A},
    {WPAD_PRO_BUTTON_B, UPAD_BUTTON_B},
    {WPAD_PRO_BUTTON_X, UPAD_BUTTON_X},
    {WPAD_PRO_BUTTON_Y, UPAD_BUTTON_Y},
    {WPAD_PRO_BUTTON_PLUS, UPAD_BUTTON_PLUS},
    {WPAD_PRO_BUTTON_MINUS, UPAD_BUTTON_MINUS},
    {WPAD_PRO_BUTTON_UP | WPAD_PRO_STICK_L_EMULATION_UP, UPAD_BUTTON_UP},
    {WPAD_PRO_BUTTON_DOWN | WPAD_PRO_STICK_L_EMULATION_DOWN, UPAD_BUTTON_DOWN},
    {WPAD_PRO_BUTTON_LEFT | WPAD_PRO_STICK_L_EMULATION_LEFT, UPAD_BUTTON_LEFT},
    {WPAD_PRO_BUTTON_RIGHT | WPAD_PRO_STICK_L_EMULATION_RIGHT, UPAD_BUTTON_RIGHT},
};

UPAD::UPAD(){
    /*
    WPADInit();
    KPADInit();
    VPADInit();
    */
};

UPAD::~UPAD(){
    /*
    KPADShutdown();
    WPADShutdown();
    VPADShutdown();
    */
};

void UPAD::read(){
    error = UPAD_READ_SUCCESS;
    trigger &= ~trigger; //Magically sets it to 0 :trollface:
    hold &= ~hold;
    release &= ~release;
    VPADtoUPAD(VPAD_CHAN_0);
    VPADtoUPAD(VPAD_CHAN_1);
    uint8_t kpad_iterate = 0;
    for (kpad_iterate = 0; kpad_iterate < 4; kpad_iterate++){
        KPADtoUPAD((KPADChan) kpad_iterate);
    }
    //Check to see if literally any input has been read
    if (!(trigger || hold || release)){
        error = UPAD_NO_CONTROLLERS;
    }
}

void UPAD::VPADtoUPAD(VPADChan chanl){
    VPADStatus drc;
    VPADReadError err;
    VPADRead(chanl, &drc, 1, &err);
    if (err != VPAD_READ_SUCCESS){
        return;
    }
    trigger |= internal_read(drc.trigger, vpad_buttons, 12);
    hold |= internal_read(drc.hold, vpad_buttons, 12);
    release |= internal_read(drc.release, vpad_buttons, 12);

}

void UPAD::KPADtoUPAD(KPADChan chanl){
    KPADStatus wiimote;
    KPADError err;
    KPADReadEx(chanl, &wiimote, 1, &err);
    //Insert error check here

    switch (wiimote.extensionType)
    {
    case WPAD_EXT_CORE:
    case WPAD_EXT_MPLUS:
    case WPAD_EXT_NUNCHUK:
    case WPAD_EXT_MPLUS_NUNCHUK:
        //Shared mappings between Wiimote and Wiimote + Nunchuck
        trigger |= internal_read(wiimote.trigger, kpad_base_buttons, 10);
        hold |= internal_read(wiimote.hold, kpad_base_buttons, 10);
        release |= internal_read(wiimote.release, kpad_base_buttons, 10);

        //Wiimote standalone unique mappings
        if (wiimote.extensionType == WPAD_EXT_CORE || wiimote.extensionType == WPAD_EXT_MPLUS){
            trigger |= internal_read(wiimote.trigger, kpad_standalone_buttons, 3);
            hold |= internal_read(wiimote.hold, kpad_standalone_buttons, 3);
            release |= internal_read(wiimote.release, kpad_standalone_buttons, 3);
        }
        //If it has a nunchuck, since that's used in tandem to the Wiimote
        else {
            trigger |= internal_read(wiimote.nunchuck.trigger, kpad_nun_buttons, 7);
            hold |= internal_read(wiimote.nunchuck.hold, kpad_nun_buttons, 7);
            release |= internal_read(wiimote.nunchuck.release, kpad_nun_buttons, 7);            
        }
        break;
    
    //If a classic controller is connected, the parent Wiimote is ignored
    case WPAD_EXT_CLASSIC:
    case WPAD_EXT_MPLUS_CLASSIC:
        trigger |= internal_read(wiimote.classic.trigger, kpad_classic_buttons, 12);
        hold |= internal_read(wiimote.classic.hold, kpad_classic_buttons, 12);
        release |= internal_read(wiimote.classic.release, kpad_classic_buttons, 12);
        break;

    case WPAD_EXT_PRO_CONTROLLER:
        trigger |= internal_read(wiimote.pro.trigger, kpad_pro_buttons, 12);
        hold |= internal_read(wiimote.pro.hold, kpad_pro_buttons, 12);
        release |= internal_read(wiimote.pro.release, kpad_pro_buttons, 12);
        break;

    default:
        error = UPAD_NO_CONTROLLERS;
        break;
    }
}

uint32_t internal_read(uint32_t &in_buttons, button_struct *button_chart, uint8_t chart_size_plus_1){
    uint32_t output;
    uint8_t iter;
    button_struct current;
    for (iter = 0; iter < chart_size_plus_1; iter++){
        current = button_chart[iter];
        if (in_buttons & current.orig_button){
        output |= current.u_button;
        }
    }
    return output;
}
