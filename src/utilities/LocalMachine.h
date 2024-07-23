/**
 * @file LocalMachine.h
 * @brief LocalMachine is use to find the font file in the local machine.
 * based on the operating system. local machine will find the font file.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-23
 */

#ifndef CBIT2D_LOCALMACHINE_H
#define CBIT2D_LOCALMACHINE_H


class LocalMachine {
public:
    static const char *getFontPath();
};


#endif //CBIT2D_LOCALMACHINE_H
