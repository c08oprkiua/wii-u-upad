#include "upad.h"


UPAD::UPAD(){
    WPADInit();
    KPADInit();
    //VPADInit();
};

UPAD::~UPAD(){
    KPADShutdown();
    WPADShutdown();
    //VPADShutdown();
};

void UPAD::VPADtoUPAD(VPADChan chanl){
    VPADStatus *drc;
    VPADReadError *err;
    VPADRead(chanl, drc, 1, err);
    /*if (err != VPAD_READ_SUCCESS){


        return;
    }*/

}

void UPAD::KPADtoUPAD(KPADChan chanl){
    KPADStatus *wiimote;
    KPADError *err;
    KPADReadEx(chanl, wiimote, sizeof(KPADStatus), err);
    //Insert error check here
    KPADExtensionType controller_type = (KPADExtensionType) wiimote->extensionType;
    switch (controller_type){
        case WPAD_EXT_CORE:
        wiimote->trigger;
        break;
        case WPAD_EXT_NUNCHUK:
        wiimote->nunchuck.trigger;
        break;
        case WPAD_EXT_CLASSIC:
        wiimote->classic.trigger;
        break;
        case WPAD_EXT_PRO_CONTROLLER:
        wiimote->pro.trigger;
        break;


}