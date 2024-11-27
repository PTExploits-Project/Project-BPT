#include "hItem.h"

DWORD GetSpeedSum(const char* szName)
{
    DWORD cnt;
    DWORD Sum1, Sum2;
    BYTE ch;
    DWORD  dwSum;

    Sum2 = 0;
    dwSum = 0;

    cnt = 0;

    while (1)
    {
        ch = (BYTE)szName[cnt];
        if (ch == 0) break;
        if (ch >= 'a' && ch <= 'z')
        {
            Sum2 += (ch - 0x20) * (cnt + 1);
            dwSum += (ch - 0x20) * (cnt * cnt);
        }
        else
        {
            Sum2 += (ch * (cnt + 1));
            dwSum += ch * (cnt * cnt);
        }
        cnt++;
    }

    Sum1 = cnt;

    return (dwSum << 24) | (Sum1 << 16) | Sum2;
}

DWORD rsITEM_SecCode_Xor[16] = {
        0xB79C64A6 ,        //0
        0x5F74E386 ,        //1
        0xA8C9E234 ,        //2
        0x6F5D87CA ,        //3
        0x35367D53 ,        //4
        0x8BF37E4A ,        //5
        0x8F389477 ,        //6
        0xFB63C792 ,        //7
        0x2E4E58C6 ,        //8
        0x7E2DC5B5 ,        //9
        0xB9C57EC5 ,        //A
        0x2A96851B ,        //B
        0xC58754C6 ,        //C
        0x7BA3D545 ,        //D
        0xE5C63A9B ,        //E
        0xBEC545C9        //F
};

DWORD rsRegist_EnterKeyNew(DWORD a1, DWORD a2, DWORD a3) {
    DWORD ecx4;
    DWORD eax5;

    ecx4 = rsITEM_SecCode_Xor[((a1 + a2) & 0xF)];
    eax5 = rsITEM_SecCode_Xor[(a1 + ecx4 & 0xF)];
    return rsITEM_SecCode_Xor[(eax5 + a2 + a3 & 0xF)] ^ eax5 << 3 ^ ecx4 >> 3 ^ (a3 * 0x7);
}

static DWORD ReformStateCode(char* lpState, int size)
{
    int cnt;

    for (cnt = 0; cnt < size; cnt++) {
        iFormCode += ((DWORD)lpState[cnt]) * ((iFromCnt & iConst) + 1);
        iFromCnt++;
    }

    return iFormCode;
}

DWORD GetRefromItemCode(sITEMINFO* lpItemInfo, int Key) {

    iFormCode = 0;
    iFromCnt = Key;

    ReformStateCode((char*)&lpItemInfo->CODE, sizeof(DWORD));
    ReformStateCode((char*)lpItemInfo->ItemName, 32);

    ReformStateCode((char*)&lpItemInfo->Weight, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Price, sizeof(int));

    ReformStateCode((char*)lpItemInfo->Resistance, sizeof(short) * 8);
    ReformStateCode((char*)lpItemInfo->Damage, sizeof(short) * 2);

    ReformStateCode((char*)&lpItemInfo->fAbsorb, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->Defence, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->fBlock_Rating, sizeof(float));

    ReformStateCode((char*)&lpItemInfo->Shooting_Range, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Attack_Speed, sizeof(int));

    ReformStateCode((char*)&lpItemInfo->Attack_Rating, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Critical_Hit, sizeof(int));

    ReformStateCode((char*)&lpItemInfo->fSpeed, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->Potion_Space, sizeof(int));

    ReformStateCode((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fMana_Regen, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fLife_Regen, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
    ReformStateCode((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

    ReformStateCode((char*)&lpItemInfo->Level, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Strength, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Spirit, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Talent, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Dexterity, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->Health, sizeof(int));

    ReformStateCode((char*)lpItemInfo->Mana, sizeof(short) * 2);
    ReformStateCode((char*)lpItemInfo->Life, sizeof(short) * 2);
    ReformStateCode((char*)lpItemInfo->Stamina, sizeof(short) * 2);

    ReformStateCode((char*)&lpItemInfo->Money, sizeof(int));
    ReformStateCode((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));
    ReformStateCode((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

    ReformStateCode((char*)&lpItemInfo->UniqueItem, sizeof(DWORD));
    ReformStateCode((char*)lpItemInfo->EffectBlink, sizeof(short) * 2);
    ReformStateCode((char*)lpItemInfo->EffectColor, sizeof(short) * 4);
    ReformStateCode((char*)lpItemInfo->ScaleBlink, sizeof(short) * 2);
    ReformStateCode((char*)&lpItemInfo->DispEffect, sizeof(DWORD));

    return iFormCode;
}

static DWORD rsReformStateCode2(char* lpState, int size)
{
    int cnt;

    for (cnt = 0; cnt < size; cnt++) {
        iFormCode += (((DWORD)lpState[cnt]) + ITEM_FORM_ADD1) * ((iFromCnt & ITEM_FORM_CODE_MASK) + ITEM_FORM_ADD2);
        iFromCnt += (cnt + lpState[cnt] + 1) * ITEM_FORM_MULT;
    }

    return iFormCode;
}

static DWORD rsGetRefromItemCode2(sITEMINFO* lpItemInfo, int Key)
{
    iFormCode = (Key * lpItemInfo->CODE) & ITEM_FORM_START_MASK;
    iFromCnt = Key;

    rsReformStateCode2((char*)&lpItemInfo->CODE, sizeof(DWORD));
    rsReformStateCode2((char*)lpItemInfo->ItemName, 32);

    rsReformStateCode2((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fMana_Regen, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fLife_Regen, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

    rsReformStateCode2((char*)&lpItemInfo->Level, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Strength, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Spirit, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Talent, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Dexterity, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Health, sizeof(int));

    rsReformStateCode2((char*)lpItemInfo->Mana, sizeof(short) * 2);
    rsReformStateCode2((char*)lpItemInfo->Life, sizeof(short) * 2);
    rsReformStateCode2((char*)lpItemInfo->Stamina, sizeof(short) * 2);

    rsReformStateCode2((char*)&lpItemInfo->Money, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));
    rsReformStateCode2((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

    rsReformStateCode2((char*)&lpItemInfo->UniqueItem, sizeof(DWORD));
    rsReformStateCode2((char*)lpItemInfo->EffectBlink, sizeof(short) * 2);
    rsReformStateCode2((char*)lpItemInfo->EffectColor, sizeof(short) * 4);
    rsReformStateCode2((char*)lpItemInfo->ScaleBlink, sizeof(short) * 2);
    rsReformStateCode2((char*)&lpItemInfo->DispEffect, sizeof(DWORD));

    /////////////////////////////////////////////////////////////////////////////
    rsReformStateCode2((char*)&lpItemInfo->Weight, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Price, sizeof(int));

    rsReformStateCode2((char*)lpItemInfo->Resistance, sizeof(short) * 8);
    rsReformStateCode2((char*)lpItemInfo->Damage, sizeof(short) * 2);

    rsReformStateCode2((char*)&lpItemInfo->fAbsorb, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->Defence, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->fBlock_Rating, sizeof(float));

    rsReformStateCode2((char*)&lpItemInfo->Shooting_Range, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Attack_Speed, sizeof(int));

    rsReformStateCode2((char*)&lpItemInfo->Attack_Rating, sizeof(int));
    rsReformStateCode2((char*)&lpItemInfo->Critical_Hit, sizeof(int));

    rsReformStateCode2((char*)&lpItemInfo->fSpeed, sizeof(float));
    rsReformStateCode2((char*)&lpItemInfo->Potion_Space, sizeof(int));

    return iFormCode;
}

DWORD rsGetItem_ServerForm(sITEMINFO* lpItemInfo) {
    DWORD dwCode;

    //_NEW_ITEM_FORMCODE2
    dwCode = rsGetRefromItemCode2(lpItemInfo, lpItemInfo->ItemHeader.dwChkSum);
    dwCode = dwCode ^ ((lpItemInfo->ItemAgingProtect[0] * 7) ^ (lpItemInfo->ItemAgingProtect[1] * 9));
    dwCode += (lpItemInfo->dwCreateTime & 0xFFFF) << 2;
    dwCode = dwCode ^ (lpItemInfo->ItemHeader.Head ^ ((lpItemInfo->ItemHeader.dwChkSum + dwCode) << ITEM_FORM_CODE_SHIFT));

    return dwCode;
}

// Get item authentication
// see was: rsReformItem_Server(sITEMINFO* lpItemInfo);
// ValidateItem
int	ValidateItem(sITEMINFO* lpItemInfo)
{
    DWORD dwCode;

    dwCode = rsGetItem_ServerForm(lpItemInfo);
    lpItemInfo->ItemHeader.dwTime = dwCode;

    return TRUE;
}

// Get item certification
// see was: ReformItem(sITEMINFO* lpItemInfo);
// Reform Item
int ReformItem(sITEMINFO* lpItem) {
    lpItem->ItemHeader.Head = ItemFormKey + GetCurrentTime(); //dwHead

    DWORD dwCode = GetRefromItemCode(lpItem, lpItem->ItemHeader.Head);
    lpItem->ItemHeader.dwChkSum = dwCode; //dwChkSum
    lpItem->ItemHeader.dwVersion = ItemFormVersion;
    lpItem->dwCreateTime = GetCurrentTime() * 2;

    lpItem->BackUpKey = lpItem->ItemHeader.Head;
    lpItem->BackUpChkSum = lpItem->ItemHeader.dwChkSum;

    //sItem authentication
    //ReformMixItem(lpItem);
    ValidateItem(lpItem); //rsReformItem_Server

    lpItem->Temp0 = lpItem->CODE + lpItem->ItemHeader.dwChkSum;

    ItemFormKey++;

    return TRUE;
}

int ReformItem2(sITEMINFO* lpItem) {
    lpItem->ItemHeader.Head = ItemFormKey + GetCurrentTime(); //dwHead

    DWORD dwCode = GetRefromItemCode(lpItem, lpItem->ItemHeader.Head);
    lpItem->ItemHeader.dwChkSum = dwCode; //dwChkSum
    lpItem->ItemHeader.dwVersion = ItemFormVersion;
    lpItem->dwCreateTime = 0;

    //lpItem->BackUpKey = lpItem->ItemHeader.Head;
    //lpItem->BackUpChkSum = lpItem->ItemHeader.dwChkSum;

    //sItem authentication
    //ReformMixItem(lpItem);
    ValidateItem(lpItem); //rsReformItem_Server

    lpItem->Temp0 = lpItem->CODE + lpItem->ItemHeader.dwChkSum;

    ItemFormKey++;

    return TRUE;
}

DWORD naosei_[]{
    00000000,
    00000001,
    00000002,
    00000003,
    00000001,
    00000003,
    00000002,
    00000000,
    00000000,
    00000002,
    00000003,
    00000001,
    00000001,
    00000003,
    00000000,
    00000002,
    00000003,
    00000002,
    00000001,
    00000000,
    00000002,
    00000000,
    00000003,
    00000001,
    00000003,
    00000001,
    00000000,
    00000002,
    00000002,
    00000001,
    00000003,
    00000000
};

int rsCheck_ItemSecCode(TRANS_ITEMINFO TransThrowItem, char* szName) {
    DWORD dwSeCode[4] = { 0 };

    DWORD dwCode_Name = GetSpeedSum(szName);
    DWORD dwObjectSerial = read(0x033DEC00, 4);

    DWORD dwKey = (dwObjectSerial & 7) << 4;
    DWORD dwItemChkSum = TransThrowItem.Item.ItemHeader.Head ^ TransThrowItem.Item.ItemHeader.dwChkSum;

    //--------//

    int iCode_0 = naosei_[(dwKey / 4) + 3];
    DWORD dwSeCode_0 = rsRegist_EnterKeyNew(dwCode_Name, 8, dwObjectSerial) ^ TransThrowItem.dwSeCode[iCode_0];

    DWORD iCode_1 = naosei_[(dwKey / 4)];
    DWORD dwSeCode_1 = rsRegist_EnterKeyNew(dwSeCode_0, 0, dwObjectSerial) ^ TransThrowItem.dwSeCode[iCode_1];

    int iCode_2 = naosei_[(dwKey / 4) + 1];
    DWORD dwSeCode_2 = rsRegist_EnterKeyNew(dwSeCode_0, 1, dwObjectSerial) ^ TransThrowItem.dwSeCode[iCode_2];

    int iCode_3 = naosei_[(dwKey / 4) + 2];
    DWORD dwSeCode_3 = rsRegist_EnterKeyNew(dwSeCode_0, 2, dwObjectSerial) ^ TransThrowItem.dwSeCode[iCode_3];

    //iItemCnt
    dwSeCode[iCode_1] = dwSeCode_1 ^ XOR_ITEM_SeCode_0;

    //verificação de item 1

    dwSeCode[iCode_2] = dwSeCode_2 ^ (XOR_ITEM_SeCode_1 ^ rsRegist_EnterKeyNew(dwSeCode[iCode_1], 5, dwObjectSerial));
    dwSeCode[iCode_2] ^= ((dwCode_Name & 0xFFFF) | (dwObjectSerial << 16)) ^ dwItemChkSum;

    //verificação de item 2

    dwSeCode[iCode_3] = dwSeCode_3 ^ (XOR_ITEM_SeCode_2 ^ rsRegist_EnterKeyNew(dwSeCode[iCode_1], 6, dwObjectSerial));
    dwSeCode[iCode_3] ^= (((dwCode_Name & 0xFFFF) | (dwObjectSerial << 16)) ^ dwItemChkSum) ^ TransThrowItem.Item.Money ^ dwSeCode[iCode_1];

    //imprime tela
    /*
    std::cout << std::hex << std::uppercase << "Current dwSeCode[" << iCode_2 << "] = 0x" << TransThrowItem.dwSeCode[iCode_2] << std::endl;
    std::cout << std::hex << std::uppercase << "Current dwSeCode[" << iCode_3 << "] = 0x" << TransThrowItem.dwSeCode[iCode_3] << std::endl << std::endl;

    std::cout << std::hex << std::uppercase << "Resolved dwSeCode[" << iCode_2 << "] = 0x" << dwSeCode[iCode_2] << std::endl;
    std::cout << std::hex << std::uppercase << "Resolved dwSeCode[" << iCode_3 << "] = 0x" << dwSeCode[iCode_3] << std::endl;
    */

    //DWORD dwSecCode_0 = (0 << 16) | (1 & 0xFFFF);
    //dwSecCode_0 &= 0x7FFFFFFF;

    TransThrowItem.dwSeCode[iCode_2] ^= dwItemChkSum;
    TransThrowItem.dwSeCode[iCode_3] ^= dwItemChkSum;

    strcpy(TransThrowItem.Item.ItemName, "700000000Ouro");

    ReformItem(&TransThrowItem.Item);

    DWORD dwNewItemChkSum = TransThrowItem.Item.ItemHeader.Head ^ TransThrowItem.Item.ItemHeader.dwChkSum;

    //dwSecCode_0 = (0 << 16) | (2 & 0xFFFF);
    //dwSecCode_0 &= 0x7FFFFFFF;

    TransThrowItem.dwSeCode[iCode_2] ^= dwNewItemChkSum;
    TransThrowItem.dwSeCode[iCode_3] ^= dwNewItemChkSum;

    memcpy((void*)(0x03808E98), &TransThrowItem, sizeof(TRANS_ITEMINFO));

    //PushRecvTransItemQue
    /*typedef void(__cdecl* lfreformItem)(TRANS_ITEMINFO*);
    lfreformItem lreformItem = (lfreformItem)0x005D9BBC;
    lreformItem(&TransThrowItem);

    smTRANS_COMMAND_EX TransCommandEx;
    ZeroMemory(&TransCommandEx, sizeof(smTRANS_COMMAND_EX));

    TransCommandEx.code = smTRANSCODE_GETITEM;
    TransCommandEx.size = sizeof(smTRANS_COMMAND_EX);
    TransCommandEx.WParam = TransThrowItem.Item.CODE;
    TransCommandEx.LParam = TransThrowItem.Item.ItemHeader.Head;
    TransCommandEx.SParam = TransThrowItem.Item.ItemHeader.dwChkSum;
    TransCommandEx.EParam = TransThrowItem.Item.Money ^ smTRANSCODE_GETITEM ^ TransThrowItem.Item.ItemHeader.dwChkSum;

    TransCommandEx.WxParam = TransThrowItem.dwSeCode[0];
    TransCommandEx.LxParam = TransThrowItem.dwSeCode[1];
    TransCommandEx.SxParam = TransThrowItem.dwSeCode[2];
    TransCommandEx.ExParam = TransThrowItem.dwSeCode[3];

    SendGameServer((char*)&TransCommandEx, TransCommandEx.size, TRUE);*/

    return TRUE;
}

int SendBuyItemToServer(sITEM* pItem, int iItemCount) {
    TRANS_BUY_SHOPITEM	TransBuyShopItem;

    TransBuyShopItem.code = smTRANSCODE_SHOPTITEM;
    TransBuyShopItem.size = sizeof(TRANS_BUY_SHOPITEM);
    TransBuyShopItem.ItemCount = iItemCount;

    memcpy(&TransBuyShopItem.sItem, pItem, sizeof(sITEM));

    //TransBuyShopItem.sItem.sItemInfo.Price = 0; - Se setar 0 gera log
    ReformItem2(&TransBuyShopItem.sItem.sItemInfo);

    SendGameServer((char*)&TransBuyShopItem, TransBuyShopItem.size, TRUE);

    return TRUE;
}

int param1 = 0x00628780;
int volta = 0x0045F5B0;
__declspec(naked) void ssend() {
    __asm {
        push ebp
        mov ebp, esp
        mov eax, 0x00002004
        call[param1]
        mov eax, dword ptr ds:[0x0880FB4]
        xor eax, dword ptr ds:[ebp + 0x04]
        push ebx
        mov ebx, dword ptr ds:[ebp + 0x0C]
        cmp ebx, 0x00001FF0
        jmp [volta]
    }
}

int send_ = 0x0045F591;

__declspec(naked) void SendGameServer(char* sendData, int len, int impMode) {
    __asm {
        push[esp + 0xC]
        push[esp + 0xC]
        push[esp + 0xC]
        mov ecx, dword ptr ds : [0x038581DC] //smWSockDataServer
        call ssend
        //add esp, 0xC
        retn
    }
}