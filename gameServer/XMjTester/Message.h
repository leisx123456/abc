// ***************************************************************
//  GameAlgorithm   version:  1.0   ·  date: 14/7/2011
//  -------------------------------------------------------------

//		麻将游戏测试工具消息协议\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#ifndef AFC_XMJTester_MESSAGE_HEAD_FILE
#define AFC_XMJTester_MESSAGE_HEAD_FILE

//调试类消息
#define ASS_FINISH_ROUND_REQ			200				//结束本轮游戏
#define ASS_TEST_FETCH_CW				201				//测试取牌
#define ASS_TEST_FETCH_AW				202				//测试取牌
#define ASS_TEST_FETCH_INFO				203				//测试取牌(客户端指定下发的手牌)
#define ASS_TEST_CH_HAND				204				//篡改手牌
#define ASS_TEST_CH_FETCH				205				//指定抓牌值
#define ASS_TEST_GAMEFINISH				206				//线束游戏
#define ASS_TEST_CLOSE_DESK				207				//快速收桌
#define ASS_TEST_GIVETOKEN				208				//触发服务端发令牌


#endif