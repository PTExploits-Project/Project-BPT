#include "Auto.h"

void findMob() {
    int iMobKill = 0, iMotionInfo = 0, iState = 0, lpCurPlayer = 0, x = 0, z = 0;

    int iTime = 0, iMobLife = 0, iOldMobLife = 0, iMobTarget = 0, iOldMobTarget = 0;
    bool bTeste = false;

    while (true) {
        Sleep(200);

        int chrOtherPlayer = 0x0B0A218, somaOtherPlayer = 0x4CF0;

        for (int i = 0; i < 400; i++) {
            bool bStartCount = false;

            while (
                read(chrOtherPlayer + 0x39C4, 4) == 1 &&      //State               - smCHAR_STATE_ENEMY
                read(chrOtherPlayer + 0x47A0, 4) != 0x120     //MotionInfo->Status  - CHRMOTION_STATE_DEAD
                )
            {
                lpCurPlayer = read(0xAFE60C, 4);
                x = read(chrOtherPlayer + 0x1D8, 4) - read(lpCurPlayer + 0x1E8, 4);
                z = read(chrOtherPlayer + 0x1E0, 4) - read(lpCurPlayer + 0x1F0, 4);

                if (abs(x) < 60000 && abs(z) < 60000) {
                    if (bAutoBot) {
                        while (
                            bAutoBot &&
                            read(chrOtherPlayer + 0x39C4, 4) == 1 &&
                            read(chrOtherPlayer + 0x47A0, 4) != 0x120
                            )
                        {
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

                                write(lpCurPlayer + 0x29C, &iMobKill, 4); //target
                                write(0xAFE688, (BYTE*)"\x2", 1); //skill 1 or skill 2
                                write(0xAFE688 + 0x8, &iMobKill, 4); //target
                                write(0xAFE688 - 0x4, (BYTE*)"\x1", 1); //move
                                write(0xAFE688 + 0x14, (BYTE*)"\x1", 1); //move

                                Sleep(200);

                                if (GetTickCount() - iTime > 2500) {
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
                                write(0xAFE688 + 0x8, (BYTE*)"\x0", 4);
                                write(0xAFE688 - 0x4, (BYTE*)"\x0", 1); //move
                                write(0xAFE688 + 0x14, (BYTE*)"\x0", 1); //move

                                break;
                            }
                        }

                        break;
                    }
                }
                else
                    break;
            }

            chrOtherPlayer += somaOtherPlayer;
        }
    }
}