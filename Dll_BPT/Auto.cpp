#include "Auto.h"

void findMob() {
    int chrOtherPlayer = 0, somaOtherPlayer = 0x4CF0;
    int iMobKill = 0, iMotionInfo = 0, iState = 0, lpCurPlayer = 0, x = 0, z = 0;

    int iTime = 0, iMobLife = 0, iOldMobLife = 0, iMobTarget = 0, iOldMobTarget = 0;
    bool bTeste = false, bFound = false;

    while (true) {
        Sleep(200);

        if (bAutoBot) {
            bool bStartCount = false;

            //find the nearest targer 
            for (int j = 0; j < 60000; j += 1000) {
                bFound = false;
                chrOtherPlayer = 0x0B0A218;

                for (int k = 0; k < 400; k++) {
                    lpCurPlayer = read(0xAFE60C, 4);
                    x = read(chrOtherPlayer + 0x1D8, 4) - read(lpCurPlayer + 0x1E8, 4);
                    z = read(chrOtherPlayer + 0x1E0, 4) - read(lpCurPlayer + 0x1F0, 4);

                    if (read(chrOtherPlayer + 0x39C4, 4) == 1 &&
                        read(chrOtherPlayer + 0x47A0, 4) != 0x120) {

                        if (abs(x) < j && abs(z) < j) {
                            bFound = true;
                            break;
                        }
                    }

                    chrOtherPlayer += somaOtherPlayer;
                }

                if (bFound)
                    break;
            }

            if (bFound) {
                while (
                    bAutoBot &&
                    read(chrOtherPlayer + 0x39C4, 4) == 1 &&
                    read(chrOtherPlayer + 0x47A0, 4) != 0x120
                    )
                {
                    lpCurPlayer = read(0xAFE60C, 4);
                    iMotionInfo = read(lpCurPlayer + 0x47A4, 4);
                    iState = read(iMotionInfo, 4);

                    //CHRMOTION_STATE_DEAD - CHRMOTION_STATE_EAT - 
                    if (iState != 0x120 && iState != 0x140) {
                        if (!bStartCount) {
                            iTime = GetTickCount();

                            bStartCount = true;
                        }

                        iMobKill = chrOtherPlayer - 0x10;

                        if (!bTeste) {
                            iMobLife = read(chrOtherPlayer + 0x3A40, 2);
                            iMobTarget = iMobKill;

                            bTeste = true;
                        }

                        write(0xAFE684, (BYTE*)"\x1", 1); //move
                        write(0xAFE684 + 0x4, (BYTE*)"\x2", 1); //skill 1 or skill 2
                        write(0xAFE684 + 0x8, &iMobKill, 4); //target
                        write(0xAFE684 + 0xC, &iMobKill, 4); //target
                        write(0xAFE684 + 0x3C, &iMobKill, 4); //target
                        write(lpCurPlayer + 0x29C, &iMobKill, 4); //target

                        Sleep(200);

                        if (GetTickCount() - iTime > 1500) {
                            iTime = GetTickCount();

                            iOldMobLife = read(chrOtherPlayer + 0x3A40, 2);
                            iOldMobTarget = iMobKill;

                            if (iMobLife == iOldMobLife && iMobTarget == iOldMobTarget) {
                                //Beep(500, 1000);
                                bTeste = false;

                                break;
                            }
                            else
                                bTeste = false;
                        }
                    }
                    else {
                        bAutoBot = false;
                        break;
                    }
                }
            }
        }
    }
}