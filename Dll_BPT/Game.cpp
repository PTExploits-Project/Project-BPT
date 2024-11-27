#include "Game.h"

#define	smTRANSCODE_HEALING				0x484700D2

int menu() {
	while (true) {
		Sleep(10);

		/*if (GetAsyncKeyState(VK_F1) < 0) {
			Sleep(200);
			Beep(500, 500);

			TRANS_ITEMINFO	TransThrowItem;
			ZeroMemory(&TransThrowItem, sizeof(TRANS_ITEMINFO));

			memcpy(&TransThrowItem, (void*)(0x03808E98), sizeof(TRANS_ITEMINFO));

			rsCheck_ItemSecCode(TransThrowItem, (char*)"001293");
		}

		if (GetAsyncKeyState(VK_F2) < 0) {
			Sleep(200);
			Beep(500, 500);

			TRANS_BUY_SHOPITEM	TransBuyShopItem;
			ZeroMemory(&TransBuyShopItem, sizeof(TRANS_BUY_SHOPITEM));

			//memcpy(&TransBuyShopItem.sItem, (void*)(0x034057F0), sizeof(sITEM));
			memcpy(&TransBuyShopItem.sItem, (void*)(0x033DC450), sizeof(sITEM));

			SendBuyItemToServer(&TransBuyShopItem.sItem, 1);
		}*/

		/*if (GetAsyncKeyState(VK_F3) < 0) {
			Sleep(200);
			
			sITEMINFO sItem;
			ZeroMemory(&sItem, sizeof(sITEMINFO));

			memcpy(&sItem, (void*)0x033DC550, sizeof(sITEMINFO));

			ReformItem(&sItem);

			//sinSetInvenItem
			typedef void(__cdecl* lfsinSetInvenItem)(sITEMINFO*);
			lfsinSetInvenItem lsinSetInvenItem = (lfsinSetInvenItem)0x004E447A;
			lsinSetInvenItem(&sItem);

			//ResetInvenItemCode
			typedef void(__cdecl* lfResetInvenItemCode)();
			lfResetInvenItemCode lResetInvenItemCode = (lfResetInvenItemCode)0x00450DB6;
			lResetInvenItemCode();
		}*/

		if (rsCompString((char*)"/nohpon") && !bDisableAll)
			bNoHp = true;

		if (rsCompString((char*)"/nohpoff"))
			bNoHp = false;

		if (rsCompString((char*)"/nompon") && !bDisableAll)
			bNoMp = true;

		if (rsCompString((char*)"/nompoff"))
			bNoMp = false;

		if (rsCompString((char*)"/nostmon") && !bDisableAll)
			bNoStm = true;

		if (rsCompString((char*)"/nostmoff"))
			bNoStm = false;

		if (rsCompString((char*)"/autoboton"))
			bAutoBot = true;

		if (rsCompString((char*)"/autobotoff"))
			bAutoBot = false;
	}

	return 0;
}