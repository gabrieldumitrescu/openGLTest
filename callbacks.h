/* 
 * File:   callbacks.h
 * Author: quentin
 *
 * Created on April 28, 2014, 7:08 PM
 */

#ifndef CALLBACKS_H
#define	CALLBACKS_H

class ICallbacks{
public:
    virtual void specialKeyboardCB(int Key, int x, int y)=0;
    virtual void keyboardCB(unsigned char Key, int x, int y)=0;
    virtual void passiveMouseCB(int x, int y)=0;
    virtual void renderSceneCB()=0;
    virtual void idleCB()=0;
};


#endif	/* CALLBACKS_H */

