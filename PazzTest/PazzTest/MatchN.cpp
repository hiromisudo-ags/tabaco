#include "stdafx.h"
#include "MatchN.h"


#include <set>

MatchN::MatchN(int nullity) : width_(), height_(), nullity_(nullity) {
}

MatchN::MatchN(int nullity, unsigned width, unsigned height) : width_(width), height_(height), nullity_(nullity) {
	board_ = std::vector< int >(width * height, nullity);
}

// ボードを設定
bool MatchN::init(unsigned width, unsigned height) {

	if (width == 0 || height == 0)
		return false;

	width_ = width;
	height_ = height;

	// 既存のボードは初期値でクリア
	clear();

	return (width != 0 && height != 0);
}

// ボードの大きさを取得
// width : 盤の列数（幅）
// height : 盤の行数（高さ）
void MatchN::getBoardSize(unsigned &width, unsigned &height) const {
	width = width_;
	height = height_;
}

// ボードをクリア
void MatchN::clear() {

	board_ = std::vector< int >(width_ * height_, nullity_);
}

// XY位置に値をセット
bool MatchN::setVal(unsigned x, unsigned y, int val) {
	int id = getId(x, y);
	return setVal(id, val);
}

// Idに値をセット
bool MatchN::setVal(unsigned id, int val) {

	if (id >= board_.size())
		return false;

	board_[id] = val;

	return true;
}

// XY位置をIdに変換
unsigned MatchN::getId(unsigned x, unsigned y) const {

	if (x >= width_ || y >= height_)
		return ERR_ID;

	return width_ * y + x;
}

// XY位置の値を取得
int MatchN::getVal(unsigned x, unsigned y) const {

	if (x >= width_ || y >= height_)
		return ERR_ID;

	return board_[getId(x, y)];
}

// Idの値を取得
int MatchN::getVal(unsigned id) const {

	if (id >= board_.size())
		return nullity_;

	return board_[id];
}

// 縦横斜めでN個のマッチを検出
unsigned MatchN::checkAll(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN) const {

	// ボードが無い場合は無効
	if (board_.size() == 0)
		return 0;

	// 出力配列は空に
	idArys.clear();

	// 最小列数が1だと意味が無い
	if (minN <= 1)
		return 0;

	// 最大列数が最小列数より小さい場合は最小列数に合わせる
	if (maxN < minN)
		maxN = minN;

	// 横方向のチェック
	if (checkTypes & Horizontal) {
		VIterator it(width_, height_, minN);
		checkDir(idArys, 1, 0, minN, maxN, it);
	}

	// 縦方向のチェック
	if (checkTypes & Vertical) {
		HIterator it(width_, height_, minN);
		checkDir(idArys, 0, 1, minN, maxN, it);
	}

	// 斜め方向のチェック
	if (checkTypes & Diagonal) {

		HIterator it1(width_, height_, minN);
		checkDir(idArys, 1, 1, minN, maxN, it1);

		HIterator it2(width_, height_, minN);
		checkDir(idArys, -1, 1, minN, maxN, it2);
	}

	return idArys.size();
}

// 指定位置がN連マッチしているか検出
unsigned MatchN::check(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN, unsigned x, unsigned y) const {

	// ボードが無い場合は無効
	if (board_.size() == 0)
		return 0;

	// 出力配列は空に
	idArys.clear();

	// 最小列数が1だと意味が無い
	if (minN <= 1)
		return 0;

	// 最大列数が最小列数より小さい場合は最小列数に合わせる
	if (maxN < minN)
		maxN = minN;

	// 横方向のチェック
	if (checkTypes & Horizontal) {
		PosIterator it(width_, height_, minN, x, y);
		checkDir(idArys, 1, 0, minN, maxN, it);
	}

	// 縦方向のチェック
	if (checkTypes & Vertical) {
		PosIterator it(width_, height_, minN, x, y);
		checkDir(idArys, 0, 1, minN, maxN, it);
	}

	// 斜め方向のチェック
	if (checkTypes & Diagonal) {

		PosIterator it1(width_, height_, minN, x, y);
		checkDir(idArys, 1, 1, minN, maxN, it1);

		PosIterator it2(width_, height_, minN, x, y);
		checkDir(idArys, -1, 1, minN, maxN, it2);
	}

	return idArys.size();
}

// 指定連結方向のチェック
unsigned MatchN::checkDir(std::vector<MatchIDInfo> &idArys, int ofsX, int ofsY, unsigned minN, unsigned maxN, Iterator &it) const {

	std::vector<int> checkedIds(board_.size(), 0); // マーク配列

	// 指定方向のチェック
	for (; !it.isEnd(); it.next()) {
		unsigned x, y;
		it.getXY(x, y);

		MatchIDInfo info;
		info.val_ = getVal(x, y);

		if (info.val_ == nullity_)
			continue; // 空値は評価しない

		unsigned id = getId(x, y);
		if (checkedIds[id] == 1)
			continue; // チェック済みだったので検索を飛ばす

		// 反対方向のマッチチェック
		info.ids_.push_back(id);
		checkNextMatch(checkedIds, x, y, -ofsX, -ofsY, info, minN, maxN);

		// 検索idを逆順に
		if (info.ids_.size() > 0) {
			std::vector<int> ids;
			for (unsigned i = 0; i < info.ids_.size(); i++)
				ids.push_back(info.ids_[info.ids_.size() - i - 1]);
			info.ids_ = ids;
		}

		// 正方向のマッチチェック
		if (checkNextMatch(checkedIds, x, y, ofsX, ofsY, info, minN, maxN) == true)
			idArys.push_back(info);

		checkedIds[id] = 1; // 現在の基点位置にチェック入れる
	}

	return idArys.size();
}

// 次のマッチをチェック
bool MatchN::checkNextMatch(std::vector<int> &checkedIds, unsigned x, unsigned y, int ofsX, int ofsY, MatchIDInfo &info, unsigned minN, unsigned maxN) const {

	// マッチ数は範囲内？
	if (info.ids_.size() >= maxN) {
		// 範囲外なので終了
		return (info.ids_.size() >= minN);
	}

	// チェックする位置
	unsigned tx = x + ofsX;
	unsigned ty = y + ofsY;

	unsigned id = getId(tx, ty);
	if (id == ERR_ID) {
		// 枠外なので終了
		return (info.ids_.size() >= minN);
	}
	if (checkedIds[id] == 1)
		return (info.ids_.size() >= minN); // すでにチェック済みだった

	// IDは有効
	// 値が同じならマッチ成立
	int val = getVal(tx, ty);

	if (val == info.val_) {

		info.ids_.push_back(id);
		checkedIds[id] = 1;

		// さらにマッチチェック
		return checkNextMatch(checkedIds, tx, ty, ofsX, ofsY, info, minN, maxN);
	}

	// マッチしなかった
	return (info.ids_.size() >= minN);
}


//////////////////////
// ユーティリティ

// マッチしたIDを重複無く列挙
std::vector<int> MatchNUtil::getMatchIds(const std::vector<MatchN::MatchIDInfo> &idArys) {

	std::vector<int> outAry;
	std::set<int> checker;
	for (unsigned i = 0; i < idArys.size(); i++) {
		for (unsigned j = 0; j < idArys[i].ids_.size(); j++) {
			int id = idArys[i].ids_[j];
			if (checker.find(id) == checker.end())
				outAry.push_back(id);
		}
	}

	return outAry;
}

// MatchNが保持している値配列をクローン取得
std::vector<int> MatchNUtil::cloneValueAry(const MatchN &match) {

	unsigned w, h;
	match.getBoardSize(w, h);

	std::vector<int> outAry;

	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			outAry.push_back(match.getVal(x, y));
		}
	}

	return outAry;
}