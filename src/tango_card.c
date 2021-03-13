/**
 * tango_card.c
 * 
 * Copyright (c) 2021 Toshita
 * This software is released under the MIT License, see LICENSE, see LICENSE.
 * This website content is released under the CC BY 4.0 License, see LICENSE.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tango_card.h"

#define DEBUG 0

/**
 * 引数で与えたファイルから単語帳を作る関数
 * 引数：単語帳のデータファイル名
 * 戻り値：単語帳
 **/
tango_book *create_book(char *fileName)
{
	tango_book *tc_book = (tango_book *)malloc(sizeof(tango_book));
	if (tc_book == NULL)
	{
		return NULL;
	}

	int line_count = 0;	  // 読み込んだ行数カウント
	int question_num = 0; // 問題数をカウント
	FILE *tango_file;
	char readline[MAX_LINE_LENGTH] = {'\0'};
	card *last_tc = &tc_book->head;

	tc_book->mode = INIT; // モード：初期に設定
	tc_book->head.no = 0;
	strcpy(tc_book->head.question, "");
	strcpy(tc_book->head.answer, "");

	// ファイル名の確認
	if (fileName == NULL)
	{
		free(tc_book);
		return NULL;
	}

	// ファイルを開く処理
	tango_file = fopen(fileName, "r");
	if (tango_file == NULL)
	{
		free(tc_book);
		return NULL;
	}

	// ファイルを1行ずつ読み込んで単語カードを作成
	while (fgets(readline, MAX_LINE_LENGTH, tango_file) != NULL)
	{
		int tmp_no;
		char tmp_question[MAX_LINE_LENGTH];
		char tmp_answer[MAX_LINE_LENGTH];
		card *tc;

		if (strlen(readline) > 1 && readline[0] != '#')
		{
			if (line_count % 2 == 0)
			{
				// 問題番号をインクリメント
				question_num++;
				// 問題番号を保存
				tmp_no = question_num;
				// 問題文を格納
				strcpy(tmp_question, readline);
			}
			else
			{
				// 回答を格納
				strcpy(tmp_answer, readline);
				// カードを作成する
				tc = create_card(tmp_no, tmp_question, tmp_answer);
				// カード作成に成功した場合は、リストに追加する
				if (tc != NULL)
				{
					last_tc->next = tc;
					last_tc = tc;
				}
			}

			// 読み込んだ行数をインクリメント
			line_count++;
		}
	}
	// ファイルを閉じる
	fclose(tango_file);
	// 単語カードの枚数を格納する
	tc_book->sheets_size = question_num;
	// 単語カードの先頭を現在のカードとする
	tc_book->cur = &tc_book->head;
	// 単語カードが0枚の場合はエラーとする
	if (tc_book->sheets_size == 0)
	{
		free(tc_book);
		return NULL;
	}

	return tc_book;
}

/**
 * 単語カードを作る関数
 **/
card *
create_card(int no, char *question, char *answer)
{
	card *tc;

	if (no < 0 || question == NULL || answer == NULL)
	{
		return NULL;
	}

	// メモリを確保する
	tc = (card *)malloc(sizeof(card));
	if (tc == NULL)
	{
		return NULL;
	}
	else
	{
		tc->next = NULL;
		tc->no = no;
		strcpy(tc->question, question);
		strcpy(tc->answer, answer);
		tc->result = NOT_YET;
	}
	return tc;
}

/**
 * 後処理の関数（単語カードのメモリを開放）
 **/
int term(tango_book *tc_book)
{
	card *tc_head, *next, *cur;
	tc_head = &tc_book->head;
	cur = tc_head->next;

	while (1)
	{
		next = cur->next;
		// printf("No.%2d %p\n", cur->no, cur);
		free(cur);
		if (next != NULL)
		{
			cur = next;
		}
		else
		{
			break;
		}
	}

	return 0;
}

/**
 * 現在のカードを取得する関数
 **/
card *
get_card(tango_book *tc_book)
{
	card *cur = tc_book->cur;
	if (cur == NULL)
	{
		return NULL;
	}

	return cur;
}

/**
 * 単語カード単位で答え合わせをする関数
 * 戻り値：
 * 　0 正解、
 * 　1 不正解、
 * 　-1 答え合わせする問題がない
 **/
int check_answer(tango_book *tc_book, char *answer)
{
	if (tc_book->cur == NULL)
	{
		return -1;
	}

	if (strcmp(tc_book->cur->answer, answer) == 0)
	{
		tc_book->cur->result = CORRECT;
		return 0;
	}
	tc_book->cur->result = WRONG;
	return 1;
}

/**
 * 次のカードを取得する関数
 * 戻り値：
 * 　0　次のカードあり
 * 　1　次のカードがない場合（先頭に戻す）
 **/
int next_card(tango_book *tc_book)
{
	if (tc_book->cur == NULL)
	{
		tc_book->cur = &tc_book->head;
		return 1;
	}

	tc_book->cur = tc_book->cur->next;

	return 0;
}

/**
 *  カードを先頭に戻し、単語帳モードを待機にする関数
 **/
int reset_card(tango_book *tc_book)
{
	if (tc_book == NULL)
	{
		return 1;
	}
	tc_book->mode = WAIT;		   // 待機状態
	tc_book->cur = &tc_book->head; // 先頭に戻す
	return 0;
}

/**
 * 採点結果を集計する関数
 **/
score score_total(tango_book *tc_book)
{
	card *cur;
	score rs_score;

	// 採点対象がない場合
	if (tc_book->mode != ANSWERING && tc_book->mode != WRONG)
	{
		rs_score.status = 1;
		return rs_score;
	}

	// 位置を先頭に戻す
	tc_book->cur = &tc_book->head;
	// 初期値をセット
	rs_score.status = 0;
	rs_score.count = 0;
	rs_score.correct = 0;
	rs_score.wrong = 0;
	tc_book->mode = SCORE; // モードを採点中にする
#if DEBUG
	debug_mode_print(tc_book);
#endif
	cur = &tc_book->head;
	while (1)
	{
		if (cur->no != 0)
		{
			rs_score.count++;
			if (cur->result == CORRECT)
			{
				rs_score.correct++;
			}
			else if (cur->result == WRONG)
			{
				rs_score.wrong++;
			}
		}
		cur = cur->next;
		if (cur == NULL)
		{
			break;
		}
	}

	tc_book->mode = WAIT; // モードを採点中にする
#if DEBUG
	debug_mode_print(tc_book);
#endif
	return rs_score;
}

/**
 * 単語帳モードを変更する関数
 **/
int set_tango_book_mode(tango_book *tc_book, char op)
{
	if (op == 'a' && (tc_book->mode == INIT || tc_book->mode == WAIT))
	{
		tc_book->mode = ANSWERING;
		return 0;
	}

	if (op == 'v' && (tc_book->mode == INIT || tc_book->mode == WAIT))
	{
		tc_book->mode = VIWE;
		return 0;
	}

	if (op == 'w' && (tc_book->mode == INIT || tc_book->mode == WAIT))
	{
		tc_book->mode = WRONG;
		return 0;
	}

	if (op == 'q' && (tc_book->mode == INIT || tc_book->mode == WAIT))
	{
		tc_book->mode = END;
		return 0;
	}

	return 1;
}

/**
 * デバッグ用関数：単語帳を表示する
 **/
int debug_book_print(tango_book *tc_book)
{
	printf("== Debug ============================================\n");
	printf(" sheets size : %d\n", tc_book->sheets_size);

	card *cur = &tc_book->head;

	while (1)
	{
		printf("  No.%d Addr=%p\n", cur->no, cur);
		printf("    Q : %s\n", cur->question);
		printf("    A : %s\n", cur->answer);

		cur = cur->next;
		if (cur == NULL)
		{
			break;
		}
	}
	printf("=====================================================\n");

	return 0;
}

/**
 * デバッグ用関数：単語カードを表示する
 **/
int debug_card_print(card *card)
{
	printf("== Debug ============================================\n");
	if (card == NULL)
	{
		printf("表示する問題はありません。");
		return 1;
	}

	printf("  No.%d Addr=%p\n", card->no, card);
	printf("    Q : %s\n", card->question);
	printf("    A : %s\n", card->answer);
	printf("=====================================================\n");

	return 0;
}

/**
 * デバッグ用関数：単語帳のモードを表示する
 **/
int debug_mode_print(tango_book *tc_book)
{
	printf("== Debug ============================================\n");
	if (tc_book == NULL)
	{
		return 1;
	}
	printf(" tango_book mode = %d\n", tc_book->mode);
	printf("=====================================================\n");

	return 0;
}