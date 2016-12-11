// CardSize.h: interface for the CCardSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_)
#define AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//651 499
//660 496
//669 496
//#define nWindWidth0 597
//#define nWindWidth0 623
//#define nWindWidth0 631
#define nWindWidth0 641

class CCardSize
{
public:
	CCardSize();
	virtual ~CCardSize();
	VOID SetCardSize(int nWinWidth = m_nWinWidth);
	//VOID SetCardSize(int nWinWidth = nWindWidth0);

public:
	static int m_nWinWidth;	// 1210
	double dRate;

	CPoint ptOrg;		//(1,17);		// 牌局左上角
	
	int CARD_HEI;		// = 94;		// 每张牌的高度height
	int CARD_WID;		// = 69;		// 每张牌的宽度width
	int CARD_INT;		// = 8;			// 代表1-8列中每两堆牌之间的间隔(interval)。(第9,16两列分别与窗口两边的间距也是这个值)
	int PILE_VINT;		// = 8;			// 牌列与缓存列间垂直方向上的间隔
	int PILE_HINT;		// = 9*CARD_INT;// 缓存列与回收列之间的水平间隔
	int CARD_UNCOVER;	// = 18;		// 当被一张牌压着的时候，此露出的部分的高度
	
	//usage: clr[BIG/SML][HT/HX/MH/FK][UP/DN][X/Y]...
	//UCHAR BIG=0, SML=1, big=16, sml=8;
	UCHAR BIG, SML, big, sml;
	UCHAR clr[2][4][2][2];
	/* = {
		{	{	{0 *big,1 *big},//黑桃上
				{1 *big,1 *big},//黑桃下
			},{	{0 *big,2 *big},//红桃上
				{1 *big,2 *big},//红桃下
			},{	{0 *big,3 *big},//梅花上
				{1 *big,3 *big},//梅花下
			},{	{0 *big,4 *big},//方块上
				{1 *big,4 *big},//方块下
		}
		},{	{	{0 *sml,0 *sml},
				{0 *sml,1 *sml}
			},{	{1 *sml,0 *sml},
				{1 *sml,1 *sml}
			},{	{2 *sml,0 *sml},
				{2 *sml,1 *sml}
			},{	{3 *sml,0 *sml},
				{3 *sml,1 *sml}
		}
		}
	};*/

	//牌A－－牌10的图像数据
	UCHAR x12      ;	// = CARD_WID      /  2;
	UCHAR x13      ;	// = CARD_WID *  9 / 30;
	UCHAR x23      ;	// = CARD_WID * 21 / 30;
	UCHAR y12      ;	// = CARD_HEI      /  2;
	UCHAR y15      ;	// = CARD_HEI      /  5;
	UCHAR y25      ;	// = CARD_HEI *  2 /  5;
	UCHAR y35      ;	// = CARD_HEI *  3 /  5;
	UCHAR y45      ;	// = CARD_HEI *  4 /  5;
	UCHAR y310     ;	// = CARD_HEI *  3 / 10;
	UCHAR y710     ;	// = CARD_HEI *  7 / 10;
	UCHAR y720     ;	// = CARD_HEI *  7 / 20;
	UCHAR y1320    ;	// = CARD_HEI * 13 / 20;
	
	UCHAR cA[3]    ;	// = { x12 , y12 , 1, };
	UCHAR c2[6]    ;	// = { x12 , y15 , 1, x12 , y45 , 0, };
	UCHAR c3[9]    ;	// = { x12 , y15 , 1, x12 , y45 , 0, x12 , y12 , 1, };
	UCHAR c4[12]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, };
	UCHAR c5[15]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x12 , y12 , 1, };
	UCHAR c6[18]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, };
	UCHAR c7[21]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , };
	UCHAR c8[24]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 };
	UCHAR c9[27]   ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y12  , 1 , };
	UCHAR c10[30]  ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y25 , 1, x23 , y25 , 1, x13 , y35 , 0, x23 , y35 , 0, x13 , y45  , 0 , x23 , y45   , 0 , x12 , y310 , 1 , x12 , y710 , 0 , };
	UCHAR c8FK[24] ;	// = { x13 , y15 , 1, x23 , y15 , 1, x13 , y45 , 0, x23 , y45 , 0, x13 , y12 , 1, x23 , y12 , 1, x12 , y720 , 1 , x12 , y1320 , 0 , };

	//步骤计数的字体高度
	UINT stepFont;		// = 22;
};

#endif // !defined(AFX_CARDSIZE_H__5F9E903B_D211_40D4_873F_8B2D2DEAEDC4__INCLUDED_)
