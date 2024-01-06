#include "Auto.h"

void findMob() {
    int iMobKill = 0, iMotionInfo = 0, iState = 0, lpCurPlayer = 0, x = 0, y = 0, z = 0;

    while (true) {
        Sleep(200);
        int chrOtherPlayer = 0x0B0A218, somaOtherPlayer = 0x4CF0;

        for (int i = 0; i < 1024; i++) {
            while (
                read(chrOtherPlayer + 0x390C, 2) != read(chrOtherPlayer + 0x3910, 2) &&
                read(chrOtherPlayer + 0x3A40, 2) > 0 &&         //Mob Life
                read(chrOtherPlayer + 0x39C4, 4) == 1 &&       //smCHAR_STATE_ENEMY
                read(chrOtherPlayer + 0x397D, 1) == 0          //Crystal
                )
            {
                lpCurPlayer = read(0xAFE60C, 4);
                x = read(chrOtherPlayer + 0x1D8, 4) - read(lpCurPlayer + 0x1E8, 4);
                y = read(chrOtherPlayer + 0x1DC, 4) - read(lpCurPlayer + 0x1EC, 4);
                z = read(chrOtherPlayer + 0x1E0, 4) - read(lpCurPlayer + 0x1F0, 4);

                if (abs(x) < 60000 && abs(z) < 60000) {
                    if (bAutoBot) {
                        iMotionInfo = read(lpCurPlayer + 0x47A4, 4);
                        iState = read(iMotionInfo, 4); //CHRMOTION_STATE_

                        //CHRMOTION_STATE_DEAD - CHRMOTION_STATE_EAT - 
                        if (iState != 0x120 && iState != 0x140) {
                            iMobKill = chrOtherPlayer - 0x10;

                            write(lpCurPlayer + 0x29C, &iMobKill, 4); //target
                            write(0xAFE688, (BYTE*)"\x2", 1); //skill 1 or skill 2
                            write(0xAFE688 + 0x8, &iMobKill, 4); //target
                            write(0xAFE688 - 0x4, (BYTE*)"\x1", 1); //move
                            write(0xAFE688 + 0x14, (BYTE*)"\x1", 1); //move

                            Sleep(200);
                        }
                        else {
                            write(0xAFE688 - 0x4, (BYTE*)"\x0", 1); //move
                            write(0xAFE688 + 0x14, (BYTE*)"\x0", 1); //move
                        }
                    }
                }
                else
                    break;
            }
            if (chrOtherPlayer < 0x1E46218)
                chrOtherPlayer += somaOtherPlayer;
        }
    }
}