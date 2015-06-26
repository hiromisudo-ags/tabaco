#pragma once


#include <vector>

class MatchN {
public:

	enum Error {
		ERR_ID = 0xffffffff
	};

	// 検索方向指定フラグ
	static const unsigned Horizontal = 1; // 水平方向
	static const unsigned Vertical = 2; // 垂直方向
	static const unsigned Diagonal = 4; // 斜め方向
	static const unsigned All = Horizontal | Vertical | Diagonal; // 全方向

	// 有効列構造体
	struct MatchIDInfo {
		int val_; // 対象ID
		std::vector<int> ids_; // マスID
	};

protected:
	// 検索イテレータ
	class Iterator {
	protected:
		unsigned x_, y_, minN_, w_, h_;
	public:
		Iterator(unsigned w, unsigned h, unsigned minN) : x_(), y_(), minN_(minN), w_(w), h_(h) {}
		virtual bool isEnd() = 0;
		virtual void next() = 0;
		void getXY(unsigned &x, unsigned &y) {
			x = x_; y = y_;
		}
	};

	// 縦・斜め方向用イテレータ
	class HIterator : public Iterator {
		unsigned counter_;
	public:
		HIterator(int w, int h, unsigned minN) : Iterator(w, h, minN), counter_() {
			y_ = minN_ - 1;
		}
		virtual bool isEnd() {
			return (y_ >= h_);
		}
		virtual void next() {
			counter_++;
			x_ = counter_ % w_;
			y_ = minN_ - 1 + minN_ * (counter_ / w_);
		}
	};

	// 横方向用イテレータ
	class VIterator : public Iterator {
		unsigned counter_;
	public:
		VIterator(int w, int h, unsigned minN) : Iterator(w, h, minN), counter_() {
			x_ = minN_ - 1;
		}
		virtual bool isEnd() {
			return (x_ >= w_);
		}
		virtual void next() {
			counter_++;
			y_ = counter_ % h_;
			x_ = minN_ - 1 + minN_ * (counter_ / h_);
		}
	};

	// 指定位置イテレータ
	class PosIterator : public Iterator {
		bool isNext_;
	public:
		PosIterator(int w, int h, unsigned minN, unsigned x, unsigned y) : Iterator(w, h, minN), isNext_() {
			x_ = x; y_ = y;
		}
		virtual bool isEnd() {
			return isNext_;
		}
		virtual void next() {
			isNext_ = true;
		}
	};

	std::vector< int > board_; // 盤（width_×height）
	unsigned width_; // 盤の列数
	unsigned height_; // 盤の行数
	const int nullity_; // 空の値

protected:
	// 指定連結方向のチェック
	// idArys: 有効列を格納する配列
	// minN : 最小連結数
	// maxN : 最大連結数
	// : 調べる方向
	unsigned checkDir(std::vector<MatchIDInfo> &idArys, int ofsX, int ofsY, unsigned minN, unsigned maxN, Iterator &it) const;

	// 次のマッチをチェック
	// checkedIds: すでにチェックされているId群
	// x : 直前のX位置
	// y : 直前のY位置
	// ofsX : チェック方向X
	// ofsY : チェック方向Y
	// info : 連結格納
	// minN : 最小連結数
	// maxN : 最大連結数
	bool checkNextMatch(std::vector<int> &checkedIds, unsigned x, unsigned y, int ofsX, int ofsY, MatchIDInfo &info, unsigned minN, unsigned maxN) const;

public:
	// コンストラクタ
	// nullity: 空の値
	// width : 盤の列数（幅）
	// height : 盤の行数（高さ）
	MatchN(int nullity);
	MatchN(int nullity, unsigned width, unsigned height);

	// ボードを設定
	// width : 盤の列数（幅）
	// height : 盤の行数（高さ）
	bool init(unsigned width, unsigned height);

	// ボードの大きさを取得
	// width : 盤の列数（幅）
	// height : 盤の行数（高さ）
	void getBoardSize(unsigned &width, unsigned &height) const;

	// ボードの値をクリア
	void clear();

	// XY位置に値をセット
	// x: 盤の行（横位置）
	// y: 盤の列（縦位置）
	bool setVal(unsigned x, unsigned y, int val);

	// Idに値をセット
	// id: 盤のマスID
	bool setVal(unsigned id, int val);

	// XY位置をIdに変換
	// x: 盤の行（横位置）
	// y: 盤の列（縦位置）
	unsigned getId(unsigned x, unsigned y) const;

	// XY位置の値を取得
	// x: 盤の行（横位置）
	// y: 盤の列（縦位置）
	int getVal(unsigned x, unsigned y) const;

	// Idの値を取得
	// id: 盤のマスID
	int getVal(unsigned id) const;

	// 指定タイプでN連マッチを検出
	// idArys : マッチ情報を出力
	// checkTypes: 縦横斜めのチェック方向フラグ（orで連結）
	// minN : 最小連結数
	// maxN : 最大連結数
	unsigned checkAll(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN) const;

	// 指定位置がN連マッチしているか検出
	// idArys : マッチ情報を出力
	// checkTypes: 縦横斜めのチェック方向フラグ（orで連結）
	// minN : 最小連結数
	// maxN : 最大連結数
	// x : 盤の行（横位置）
	// y : 盤の列（縦位置）
	unsigned check(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN, unsigned x, unsigned y) const;
};



//////////////////////
// ユーティリティ

class MatchNUtil {
public:
	// マッチしたIDを重複無く列挙
	static std::vector<int> getMatchIds(const std::vector<MatchN::MatchIDInfo> &idArys);

	// MatchNが保持している値配列をクローン取得
	static std::vector<int> cloneValueAry(const MatchN &match);
};

