#include <Windows.h>

extern int read(DWORD src, int len);

struct sITEM_SPECIAL
{
    float	Add_fAbsorb;
    int     Add_Defence;
    float   Add_fSpeed;
    float   Add_fBlock_Rating;
    int     Add_Attack_Speed;
    int     Add_Critical_Hit;
    int     Add_Shooting_Range;
    float   Add_fMagic_Mastery;
    short	Add_Resistance[8];

    short	Lev_Attack_Resistance[8];
    int		Lev_Mana;
    int		Lev_Life;
    int     Lev_Attack_Rating;
    short	Lev_Damage[2];

    float	Per_Mana_Regen;
    float	Per_Life_Regen;
    float	Per_Stamina_Regen;

    DWORD	dwTemp[32];
};

//Attributes
struct	sITEM_CREATE
{
    DWORD	        Head;       //Header
    DWORD	        dwVersion;  //Generated version
    DWORD	        dwTime;     //Creation time
    DWORD	        dwChkSum;   //Items chksum
};

//Statistics
struct sITEMINFO
{
    DWORD           dwSize;         //Item structure size         
    sITEM_CREATE    ItemHeader;     //Attributes

    short	        Durability[2];  //Integrity
    DWORD	        CODE;           //CODE
    char	        ItemName[32];   //ItemName
    int             Weight;         //Weight
    int             Price;          //Price
    int		        Index;          //Index
    int		        PotionCount;    //PotionCount
    short	        Resistance[8];  //Resistance
    int		        Sight;          //Sight
    DWORD	        Temp0;          //Temp0
    short	        Damage[2];      //Damage
    int             Shooting_Range; //Shooting Range
    int             Attack_Speed;   //Attack Range
    int             Attack_Rating;  //Attack Rating
    int             Critical_Hit;   //Critical Hit
    float	        fAbsorb;        //Abs
    int             Defence;        //Def
    float           fBlock_Rating;  //Block
    float           fSpeed;         //Move Speed
    int		        Potion_Space;   //Potion Space
    float           fMagic_Mastery;
    float           fMana_Regen;
    float           fLife_Regen;
    float           fStamina_Regen;
    float           fIncrease_Life;
    float           fIncrease_Mana;
    float           fIncrease_Stamina;

    int             Level;
    int             Strength;
    int             Spirit;
    int             Talent;
    int             Dexterity;
    int             Health;

    short           Mana[2];
    short           Life[2];
    short           Stamina[2];

    int		        Money;
    int             NotUseFlag;

    DWORD	        BackUpKey;
    DWORD	        BackUpChkSum;

    short           ScaleBlink[2];
    DWORD	        UniqueItem;     //UniqueItem (0 = normal, 2 = quest weapon)
    short	        EffectBlink[2];
    short	        EffectColor[4];

    DWORD	        DispEffect;

    DWORD			JobCodeMask;
    sITEM_SPECIAL	JobItem;

    DWORD           ItemKindCode;
    DWORD           ItemKindMask;

    short           ItemAgingNum[2];
    short           ItemAgingCount[2];
    short           ItemAgingProtect[2];

    short           SpecialItemFlag[2];

    DWORD	        dwCreateTime;
    DWORD	        dwTemp[8];

    int		        iMixType = 0;
};

struct sITEM
{
    DWORD	CODE;
    char	ItemNameIndex[32];	//Item Name	

    /*-------------------------*
    *	The set during initialization
    *--------------------------*/
    char	LastCategory[16];   //The final category	
    int		w;
    int     h;			        //Item size
    char	ItemFilePath[64];   //Items loaded when a file path
    DWORD	Class;		        //The type of item
    char    DorpItem[64];       //When the item drops
    DWORD   SetModelPosi;		//Position the item to be set
    int		SoundIndex;			//Sound item
    int     WeaponClass;		//Recognize that the close-launched type


    //--------------------------

    int Flag;			        //Flags items
    int x, y;			        //The item is drawn coordinates
    int SetX, SetY;				//Gives the item is set to be put in place

    DWORD	lpItem;             //Pointer required to load the picture
    int		ItemPosition;       //This item gives a position on the presence of inventory (right hand, left hand, both hands, gapot ...)

    int     PotionCount;		//Only when the potion counts
    int     NotUseFlag;			//It gives the flag when the required value is not satisfied
    int     SellPrice;			//Items sold price

    int     OldX, OldY;          //Doetdeon coordinates the item is set in the past
    DWORD	lpTempItem;         //Pointer required to load the picture
    //int		LimitTimeFlag;		    //Shelf Life

    sITEMINFO sItemInfo;

};

struct TRANS_ITEMINFO
{
    int	size, code;

    sITEMINFO	Item;

    int x, y, z;

    DWORD	dwSeCode[4];
};

struct	TRANS_BUY_SHOPITEM {
    int	size, code;

    sITEM	sItem;			
    int		ItemCount;		
};

struct smTRANS_COMMAND_EX
{
    int	size;
    int code;

    int LParam;
    int WParam;
    int SParam;
    int EParam;

    int LxParam;
    int WxParam;
    int SxParam;
    int ExParam;

};

#define smTRANSCODE_GETITEM			0x4847005A
#define smTRANSCODE_SHOPTITEM		0x50320210

#define XOR_ITEM_SeCode_0    0xB6A55C36
#define XOR_ITEM_SeCode_1    0xC96A35DB
#define XOR_ITEM_SeCode_2    0xE6A6CBC5

//Brazil
#define	ITEM_FORM_ADD1			1
#define	ITEM_FORM_ADD2			5
#define	ITEM_FORM_MULT			43
#define	ITEM_FORM_START_MASK	0xF5F5
#define	ITEM_FORM_CODE_MASK		0xFCF
#define	ITEM_FORM_CODE_SHIFT	14

static DWORD iConst = 0x1FFF;

static int ItemFormVersion = 12;
static int ItemFormKey = 0;

static DWORD iFormCode;
static DWORD iFromCnt;

DWORD GetSpeedSum(const char* szName);
DWORD rsRegist_EnterKeyNew(DWORD a1, DWORD a2, DWORD a3);
static DWORD ReformStateCode(char* lpState, int size);
DWORD GetRefromItemCode(sITEMINFO* lpItemInfo, int Key);

DWORD rsGetItem_ServerForm(sITEMINFO* lpItemInfo);

int	ValidateItem(sITEMINFO* lpItemInfo);
int ReformItem(sITEMINFO* lpItem);

int rsCheck_ItemSecCode(TRANS_ITEMINFO TransThrowItem, char* szName);
int SendBuyItemToServer(sITEM* pItem, int iItemCount);

void SendGameServer(char* sendData, int len, int impMode);