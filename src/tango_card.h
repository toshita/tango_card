/**
 * tango_card.h
 * 
 * Copyright (c) 2021 Toshita
 * This software is released under the MIT License, see LICENSE, see LICENSE.
 * This website content is released under the CC BY 4.0 License, see LICENSE.
 **/
#ifndef MODULE_TANGO_CARD_H_
#define MODULE_TANGO_CARD_H_
#define MAX_LINE_LENGTH 256

// 単語カードの型定義
typedef struct struct_card
{
	int no;
	char question[MAX_LINE_LENGTH];
	char answer[MAX_LINE_LENGTH];
	int result;
	struct struct_card *next;
} card;

// 単語帳の型定義
typedef struct struct_tango_card
{
	card head;		 // 先頭を管理
	int sheets_size; // カード数を管理
	int mode;		 // モードを管理
	card *cur;		 // 現在選択しているカードを管理
} tango_book;

// 採点結果の型定義
typedef struct struct_score
{
	int status;	 // ステータス
	int count;	 // 問題数
	int correct; // 正解数
	int wrong;	 // 誤答数
} score;

// 単語帳のモードの列挙子
typedef enum
{
	INIT,	   // 初期
	ANSWERING, // 回答中
	SCORE,	   // 採点中
	VIWE,	   // 参照
	RETRY,	   // 回答中（誤答のみ）
	WAIT,	   // 待機
	END		   // 終了
} TB_MODE;

// 回答のステータスの列挙子
typedef enum
{
	NOT_YET, // 未回答
	CORRECT, // 正答
	WRONG	 // 誤答
} TC_STATUS;

// 単語カードを作る関数
tango_book *create_book(char *fileName);
card *create_card(int no, char *question, char *answer);
int term(tango_book *tc_book);
// カードを取得する関数
card *get_card(tango_book *tc_book);
// 単語カード単位で答え合わせをする関数
int check_answer(tango_book *tc_book, char *answer);
// カードを次に進める関数
int next_card(tango_book *tc_book);
// カードを先頭に戻し、単語帳モードを待機にする関数
int reset_card(tango_book *tc_book);
// 採点結果を集計する関数
score score_total(tango_book *tc_book);
// 単語帳モードを変更する関数
int set_tango_book_mode(tango_book *tc_book, char op);

// デバッグ
// 単語帳を表示する関数
int debug_book_print(tango_book *tc_book);
// 単語カードを表示する関数
int debug_card_print(card *card);
// 単語帳モードを表示する関数
int debug_mode_print(tango_book *tc_book);
#endif