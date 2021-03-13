/**
 * tc_main.c
 * 
 * Copyright (c) 2021 Toshita
 * This software is released under the MIT License, see LICENSE, see LICENSE.
 * This website content is released under the CC BY 4.0 License, see LICENSE.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tango_card.h"

#define VERSION "0.8"

#define DEBUG 0

// 結果表示用の文字配列
const char *RESULT_STRINGS[] = {
	"-",
	"o",
	"x"};

/**
 * 単語帳アプリのメイン処理
 **/
int main(int argc, char *argv[])
{
	char input_op[MAX_LINE_LENGTH];
	char input_ans[MAX_LINE_LENGTH];

	printf("Tango Card App. Ver.%s\n", VERSION);

	// 引数をチェックする
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s {data_file}\n", argv[0]);
		exit(1);
	}

	// ファイルから問題を取得し単語帳を作る
	tango_book *tc_book = create_book(argv[1]);
	if (tc_book == NULL)
	{
		perror(argv[1]);
		free(tc_book);
		exit(1);
	}
#if DEBUG
	debug_book_print(tc_book);
#endif

	// メインループ
	while (1)
	{
		card *cur_card;
#if DEBUG
		debug_mode_print(tc_book);
#endif
		printf("input(a/w/v/q) :");
		fgets(input_op, MAX_LINE_LENGTH, stdin);
		char op;
		strncpy(&op, input_op, 1);
		// モードを変更する
		set_tango_book_mode(tc_book, op);

		// 終了処理
		if ('q' == op)
		{
			break;
		}

		do
		{
			// カードを取得する
			cur_card = get_card(tc_book);
			// debug_print(cur_card);

			if (cur_card->no != 0)
			{
				if ('a' == op || 'v' == op || ('w' == op && (cur_card->result == WRONG || cur_card->result == NOT_YET)))
				{
					printf("No.%2d\n", cur_card->no);
					printf(" Q:%s", cur_card->question);
					if ('v' != op)
					{
						printf(" > ");
						fgets(input_ans, MAX_LINE_LENGTH, stdin);

						printf(" A:%s", cur_card->answer);
						if (check_answer(tc_book, input_ans) == 0)
						{
							printf(" Correct!!\n");
						}
						else
						{
							printf(" Wrong!!\n");
						}
						printf("\n");
					}
					else
					{
						fgets(input_ans, MAX_LINE_LENGTH, stdin);
						printf(" A:%s\n", cur_card->answer);
					}
				}
			}

			// 次のカードに進める
			next_card(tc_book);

		} while (cur_card->next != NULL);

		// 採点処理
		if ('a' == op || 'w' == op)
		{
			printf("----------------------------------\n");
			score sc = score_total(tc_book);
			card *cd;
			do
			{
				cd = get_card(tc_book);
				if (cd->no != 0)
				{
					printf(" No.%2d: %s\n", cd->no,
						   RESULT_STRINGS[cd->result]);
				}
				// 次のカードに進める
				next_card(tc_book);
			} while (cd->next != NULL);

			printf("----------------------------------\n");
			printf("Total:%2d, Correct:%2d, Wrong: %2d\n\n", sc.count,
				   sc.correct, sc.wrong);
		}

		// リセット処理
		if (reset_card(tc_book))
		{
			printf("error");
		}
	}

#if DEBUG
	debug_mode_print(tc_book);
#endif

	// 後処理（領域を開放する）
	if (tc_book != NULL)
	{
		term(tc_book);
		free(tc_book);
	}

	return 0;
}