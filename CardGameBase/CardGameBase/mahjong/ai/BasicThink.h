#pragma once



class CBasicThink  
{
public:
	CBasicThink();
	virtual ~CBasicThink();
	void GetGoodTeam(int &pai1,int &pai2,int &pai3);			//获得出牌三张组合
	void SetMjPaiArray(int *nPailist, int nNum);				//设置麻将组
	int GetBadly();												//获得最差的牌
	int GetMaxScore();											//获得最大分值

protected:
	void SetTeamPai(int type);									//设置出牌组合
	bool IsEdge(int pai1,int pai2);								//门子是否是边缘
	void SortMj(int *who,int end);								//整理

	bool IsStepTeam(int pai,int &pos1,int &pos2);				//判断总牌中是否有顺子
	bool IsSameTeam(int pai,int &pos1,int &pos2);				//判断总牌中是否有3个的

	bool IsStepTeamThree(int pai,int type,int &pos1,int nStart);//判断剔除三张以后是否存在门子
	bool IsSameTeamThree(int pai,int &pos1,int nStart);			//判断剔除三张以后是否有对子

	int FindPos(int pai,int nStart=0);							//查找牌在总牌中存在位置
	int FindPosThree(int pai,int nStart=0);						//查找牌在剔除三张组合后的存在位置

protected:
	int m_nMajiang[14];
	int m_nRemainThree[14];										//三张剔除后剩余的牌		
	int m_nRemainTwo[14];										//两张剔除后剩余的牌
	bool m_bMjSelect[14];										//麻将是否被选择
	
	int m_nPaiTeamThree[15];									//三张牌的组合
	int m_nCheckNum;											//三张牌的组合数

	int m_nGoodTeamThree[3];									//临时派组
	int m_nTotalTeam[3];										//最总派组
	int m_nNeedPai;												//需要的那长牌

	int m_nRemainNumThree,m_nRemainNumTwo,m_nRemainNumOne;		//每次剔除后剩余的牌
	int m_nMjNum;												//麻将的数量
	int m_nScoreThree,m_nScoreTwo;								//遍历后的3张和两张的分数
	int m_nMaxScoreThree,m_nMaxScoreTwo,m_nMaxScoreOne;			//各组合最大的分数
	int m_nBadly;												//最差的一张牌

	bool m_bHaveDui;											//是否有将
};

