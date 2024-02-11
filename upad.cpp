#include "upad.h"

struct KPAD_pointers {
    uint32_t *core_buttons;
    union buttons_for {
        uint32_t *nunchuck;
        uint32_t *classic;
        uint32_t *pro;
    } buttons_for;
    WPADExtensionType *ext;
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
    trigger = 0;
    hold = 0;
    release = 0;
    VPADtoUPAD(VPAD_CHAN_0);
    VPADtoUPAD(VPAD_CHAN_1);
    KPADtoUPAD(WPAD_CHAN_0);
    KPADtoUPAD(WPAD_CHAN_1);
    KPADtoUPAD(WPAD_CHAN_2);
    KPADtoUPAD(WPAD_CHAN_3);

}

void UPAD::VPADtoUPAD(VPADChan chanl){
    VPADStatus drc;
    VPADReadError err;
    VPADRead(chanl, &drc, 1, &err);
    if (err != VPAD_READ_SUCCESS){
        return;
    }
    trigger = trigger | VPAD_internal_read(drc.trigger);
    hold = hold | VPAD_internal_read(drc.hold);
    release = release | VPAD_internal_read(drc.release);

}



void UPAD::KPADtoUPAD(KPADChan chanl){
    KPADStatus wiimote;
    KPADError err;
    KPADReadEx(chanl, &wiimote, 1, &err);
    //Insert error check here
    KPAD_pointers pointers;
    pointers.ext = (WPADExtensionType *) &wiimote.extensionType;

    /*
    Not quite sure if this is how it should be done, since the union means this data is 
    being copied several times ig?
    */
    //trigger
    pointers.core_buttons = &wiimote.trigger;
    pointers.buttons_for.nunchuck = &wiimote.nunchuck.trigger;
    pointers.buttons_for.classic = &wiimote.classic.trigger;
    pointers.buttons_for.pro = &wiimote.pro.trigger;
    trigger = KPAD_internal_read(&pointers);
    //hold
    pointers.core_buttons = &wiimote.hold;
    pointers.buttons_for.nunchuck = &wiimote.nunchuck.hold;
    pointers.buttons_for.classic = &wiimote.classic.hold;
    pointers.buttons_for.pro = &wiimote.pro.hold;
    hold = KPAD_internal_read(&pointers);
    //release
    pointers.core_buttons = &wiimote.release;
    pointers.buttons_for.nunchuck = &wiimote.nunchuck.release;
    pointers.buttons_for.classic = &wiimote.classic.release;
    pointers.buttons_for.pro = &wiimote.pro.release;
    release = KPAD_internal_read(&pointers);
}

uint32_t VPAD_internal_read(uint32_t &in_buttons){
    uint32_t output;
    if (in_buttons & VPAD_BUTTON_A){
        output = output | UPAD_BUTTON_A;
    }
    if (in_buttons & VPAD_BUTTON_HOME){
        output = output | UPAD_BUTTON_HOME;
    }
    if (in_buttons & VPAD_BUTTON_A)
    {
        output = output | UPAD_BUTTON_A;
    }
    if (in_buttons & VPAD_BUTTON_B)
    {
        output = output | UPAD_BUTTON_B;
    }
    if (in_buttons & VPAD_BUTTON_X)
    {
        output = output | UPAD_BUTTON_X;
    }
    if (in_buttons & VPAD_BUTTON_Y)
    {
        output = output | UPAD_BUTTON_Y;
    }
    if (in_buttons & VPAD_BUTTON_PLUS)
    {
        output = output | UPAD_BUTTON_PLUS;
    }
    if (in_buttons & VPAD_BUTTON_MINUS)
    {
        output = output | UPAD_BUTTON_MINUS;
    }
    if (in_buttons & (VPAD_BUTTON_UP | VPAD_STICK_L_EMULATION_UP))
    {
        output = output | UPAD_BUTTON_UP;
    }
    if (in_buttons & (VPAD_BUTTON_DOWN | VPAD_STICK_L_EMULATION_DOWN))
    {
        output = output | UPAD_BUTTON_DOWN;
    }
    if (in_buttons & (VPAD_BUTTON_LEFT | VPAD_STICK_L_EMULATION_LEFT))
    {
        output = output | UPAD_BUTTON_LEFT;
    }
    if (in_buttons & (VPAD_BUTTON_RIGHT | VPAD_STICK_L_EMULATION_RIGHT))
    {
        output = output | UPAD_BUTTON_RIGHT;
    }
    return output;
}

uint32_t KPAD_internal_read(KPAD_pointers* ptrs){
    uint32_t output, buttons_pressed;
    //This gets verbose with the if statements, sorry.
    //I'll slim down these down into some sort of for loop once I learn how to in C++

    //Shared mappings between Wiimote and Wiimote + Nunchuck
    if (*ptrs->ext == WPAD_EXT_CORE || 
    *ptrs->ext == WPAD_EXT_MPLUS ||
    *ptrs->ext == WPAD_EXT_NUNCHUK ||
    *ptrs->ext == WPAD_EXT_MPLUS_NUNCHUK
    ){
        buttons_pressed = *ptrs->core_buttons;
        if (buttons_pressed & WPAD_BUTTON_HOME){
            output = output | UPAD_BUTTON_HOME;
        }
        if (buttons_pressed & WPAD_BUTTON_A){
            output = output | UPAD_BUTTON_A;
        }
        if (buttons_pressed & WPAD_BUTTON_B){
            output = output | UPAD_BUTTON_B;
        }
        if (buttons_pressed & WPAD_BUTTON_PLUS){
            output = output | UPAD_BUTTON_PLUS;
        }
        if (buttons_pressed & WPAD_BUTTON_MINUS){
            output = output | UPAD_BUTTON_MINUS;
        }
        if (buttons_pressed & WPAD_BUTTON_UP){
            output = output | UPAD_BUTTON_UP;
        }
        if (buttons_pressed & WPAD_BUTTON_DOWN){
            output = output | UPAD_BUTTON_DOWN;
        }
        if (buttons_pressed & WPAD_BUTTON_LEFT){
            output = output | UPAD_BUTTON_LEFT;
        }
        if (buttons_pressed & WPAD_BUTTON_RIGHT){
            output = output | UPAD_BUTTON_RIGHT;
        }        

    }
    //Wiimote standalone unique mappings
    if (*ptrs->ext == WPAD_EXT_CORE || *ptrs->ext == WPAD_EXT_MPLUS){
        if (buttons_pressed & WPAD_BUTTON_1){
            output = output | UPAD_BUTTON_X;
        }
        if (buttons_pressed & WPAD_BUTTON_2){
            output = output | UPAD_BUTTON_Y;
        }
    }
    //If it has a nunchuck, since that's an extension of the Wiimote
    else if (*ptrs->ext == WPAD_EXT_NUNCHUK || *ptrs->ext == WPAD_EXT_MPLUS_NUNCHUK){
        buttons_pressed = *ptrs->buttons_for.nunchuck;
        if (buttons_pressed & WPAD_NUNCHUK_BUTTON_C){
            output = output | UPAD_BUTTON_X;
        }
        if (buttons_pressed & WPAD_NUNCHUK_BUTTON_Z){
            output = output | UPAD_BUTTON_Y;
        }
        if (buttons_pressed & WPAD_NUNCHUK_STICK_EMULATION_UP){
            output = output | UPAD_BUTTON_UP;
        }
        if (buttons_pressed & WPAD_NUNCHUK_STICK_EMULATION_DOWN){
            output = output | UPAD_BUTTON_DOWN;
        }
        if (buttons_pressed & WPAD_NUNCHUK_STICK_EMULATION_LEFT){
            output = output | UPAD_BUTTON_LEFT;
        }
        if (buttons_pressed & WPAD_NUNCHUK_STICK_EMULATION_RIGHT){
            output = output | UPAD_BUTTON_RIGHT;
        }
    }
    //This is so that if a classic controller is connected, the parent Wiimote is ignored
    else if (*ptrs->ext == WPAD_EXT_CLASSIC || *ptrs->ext == WPAD_EXT_MPLUS_CLASSIC){
        buttons_pressed = *ptrs->buttons_for.classic;
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_HOME){
            output = output | UPAD_BUTTON_HOME;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_A){
            output = output | UPAD_BUTTON_A;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_B){
            output = output | UPAD_BUTTON_B;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_X){
            output = output | UPAD_BUTTON_X;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_Y){
            output = output | UPAD_BUTTON_Y;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_PLUS){
            output = output | UPAD_BUTTON_PLUS;
        }
        if (buttons_pressed & WPAD_CLASSIC_BUTTON_MINUS){
            output = output | UPAD_BUTTON_MINUS;
        }
        if (buttons_pressed & (WPAD_CLASSIC_BUTTON_UP | WPAD_CLASSIC_STICK_L_EMULATION_UP)){
            output = output | UPAD_BUTTON_UP;
        }
        if (buttons_pressed & (WPAD_CLASSIC_BUTTON_DOWN | WPAD_CLASSIC_STICK_L_EMULATION_DOWN)){
            output = output | UPAD_BUTTON_DOWN;
        }
        if (buttons_pressed & (WPAD_CLASSIC_BUTTON_LEFT | WPAD_CLASSIC_STICK_L_EMULATION_LEFT)){
            output = output | UPAD_BUTTON_LEFT;
        }
        if (buttons_pressed & (WPAD_CLASSIC_BUTTON_RIGHT | WPAD_CLASSIC_STICK_L_EMULATION_RIGHT)){
            output = output | UPAD_BUTTON_RIGHT;
        }
    }
    //Pro controller mappings
    else if (*ptrs->ext == WPAD_EXT_PRO_CONTROLLER){
        buttons_pressed = *ptrs->buttons_for.pro;
        if (buttons_pressed & WPAD_PRO_BUTTON_HOME){
            output = output | UPAD_BUTTON_HOME;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_A){
            output = output | UPAD_BUTTON_A;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_B){
            output = output | UPAD_BUTTON_B;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_X){
            output = output | UPAD_BUTTON_X;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_Y){
            output = output | UPAD_BUTTON_Y;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_PLUS){
            output = output | UPAD_BUTTON_PLUS;
        }
        if (buttons_pressed & WPAD_PRO_BUTTON_MINUS){
            output = output | UPAD_BUTTON_MINUS;
        }
        if (buttons_pressed & (WPAD_PRO_BUTTON_UP | WPAD_PRO_STICK_L_EMULATION_UP)){
            output = output | UPAD_BUTTON_UP;
        }
        if (buttons_pressed & (WPAD_PRO_BUTTON_DOWN | WPAD_PRO_STICK_L_EMULATION_DOWN)){
            output = output | UPAD_BUTTON_DOWN;
        }
        if (buttons_pressed & (WPAD_PRO_BUTTON_LEFT | WPAD_PRO_STICK_L_EMULATION_LEFT)){
            output = output | UPAD_BUTTON_LEFT;
        }
        if (buttons_pressed & (WPAD_PRO_BUTTON_RIGHT | WPAD_PRO_STICK_L_EMULATION_RIGHT)){
            output = output | UPAD_BUTTON_RIGHT;
        }
    }
    return output;
}