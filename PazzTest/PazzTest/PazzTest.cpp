// PazzTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "matchN.h"
#include <time.h>
#include <set>

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(0));

	//unsigned w = 16;
	//unsigned h = 16;
	unsigned w = 4;
	unsigned h = 4;

	// マッチN検索オブジェクト
	MatchN match(0, w, h);

	// 盤に値を設定（0 or 1）
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			int val = rand() % 2;
			match.setVal(x, y, val);    // 盤に値を設定
			printf("%s", val ? "○" : "□");
		}
		printf("\n");
	}

	// 5連以上8連以下の有効列をすべてチェック
	std::vector< MatchN::MatchIDInfo > infos;
	//match.checkAll(infos, MatchN::All, 5, 8);

	//検索対象：縦横 連鎖範囲：2～4連鎖 検索場所：X=1 Y=1
	match.check(infos, MatchN::Horizontal | MatchN::Vertical, 2, 4, 1, 1);

	// match.checkAll( infos, MatchN::Diagonal | MatchN::Horizontal, 5, 8 );  // 「斜めと横だけ」どかできます

	// マッチしたIDを列挙
	std::vector<int> ids = MatchNUtil::getMatchIds(infos);

	// MatchNが保持している値配列をクローン取得
	std::vector<int> vals = MatchNUtil::cloneValueAry(match);

	// マッチしている部分を"2"に
	for (unsigned i = 0; i < ids.size(); i++)
		vals[ids[i]] = 2;

	// マッチ箇所を黒丸で表示
	printf("\n-------------\n");
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			int val = vals[w * y + x];
			printf("%s", val == 2 ? "●" : (val == 1 ? "○" : "□"));
		}
		printf("\n");
	}

	system("pause");
	return 0;
}

