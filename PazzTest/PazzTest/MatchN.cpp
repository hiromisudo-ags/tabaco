#include "stdafx.h"
#include "MatchN.h"


#include <set>

MatchN::MatchN(int nullity) : width_(), height_(), nullity_(nullity) {
}

MatchN::MatchN(int nullity, unsigned width, unsigned height) : width_(width), height_(height), nullity_(nullity) {
	board_ = std::vector< int >(width * height, nullity);
}

// �{�[�h��ݒ�
bool MatchN::init(unsigned width, unsigned height) {

	if (width == 0 || height == 0)
		return false;

	width_ = width;
	height_ = height;

	// �����̃{�[�h�͏����l�ŃN���A
	clear();

	return (width != 0 && height != 0);
}

// �{�[�h�̑傫�����擾
// width : �Ղ̗񐔁i���j
// height : �Ղ̍s���i�����j
void MatchN::getBoardSize(unsigned &width, unsigned &height) const {
	width = width_;
	height = height_;
}

// �{�[�h���N���A
void MatchN::clear() {

	board_ = std::vector< int >(width_ * height_, nullity_);
}

// XY�ʒu�ɒl���Z�b�g
bool MatchN::setVal(unsigned x, unsigned y, int val) {
	int id = getId(x, y);
	return setVal(id, val);
}

// Id�ɒl���Z�b�g
bool MatchN::setVal(unsigned id, int val) {

	if (id >= board_.size())
		return false;

	board_[id] = val;

	return true;
}

// XY�ʒu��Id�ɕϊ�
unsigned MatchN::getId(unsigned x, unsigned y) const {

	if (x >= width_ || y >= height_)
		return ERR_ID;

	return width_ * y + x;
}

// XY�ʒu�̒l���擾
int MatchN::getVal(unsigned x, unsigned y) const {

	if (x >= width_ || y >= height_)
		return ERR_ID;

	return board_[getId(x, y)];
}

// Id�̒l���擾
int MatchN::getVal(unsigned id) const {

	if (id >= board_.size())
		return nullity_;

	return board_[id];
}

// �c���΂߂�N�̃}�b�`�����o
unsigned MatchN::checkAll(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN) const {

	// �{�[�h�������ꍇ�͖���
	if (board_.size() == 0)
		return 0;

	// �o�͔z��͋��
	idArys.clear();

	// �ŏ��񐔂�1���ƈӖ�������
	if (minN <= 1)
		return 0;

	// �ő�񐔂��ŏ��񐔂�菬�����ꍇ�͍ŏ��񐔂ɍ��킹��
	if (maxN < minN)
		maxN = minN;

	// �������̃`�F�b�N
	if (checkTypes & Horizontal) {
		VIterator it(width_, height_, minN);
		checkDir(idArys, 1, 0, minN, maxN, it);
	}

	// �c�����̃`�F�b�N
	if (checkTypes & Vertical) {
		HIterator it(width_, height_, minN);
		checkDir(idArys, 0, 1, minN, maxN, it);
	}

	// �΂ߕ����̃`�F�b�N
	if (checkTypes & Diagonal) {

		HIterator it1(width_, height_, minN);
		checkDir(idArys, 1, 1, minN, maxN, it1);

		HIterator it2(width_, height_, minN);
		checkDir(idArys, -1, 1, minN, maxN, it2);
	}

	return idArys.size();
}

// �w��ʒu��N�A�}�b�`���Ă��邩���o
unsigned MatchN::check(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN, unsigned x, unsigned y) const {

	// �{�[�h�������ꍇ�͖���
	if (board_.size() == 0)
		return 0;

	// �o�͔z��͋��
	idArys.clear();

	// �ŏ��񐔂�1���ƈӖ�������
	if (minN <= 1)
		return 0;

	// �ő�񐔂��ŏ��񐔂�菬�����ꍇ�͍ŏ��񐔂ɍ��킹��
	if (maxN < minN)
		maxN = minN;

	// �������̃`�F�b�N
	if (checkTypes & Horizontal) {
		PosIterator it(width_, height_, minN, x, y);
		checkDir(idArys, 1, 0, minN, maxN, it);
	}

	// �c�����̃`�F�b�N
	if (checkTypes & Vertical) {
		PosIterator it(width_, height_, minN, x, y);
		checkDir(idArys, 0, 1, minN, maxN, it);
	}

	// �΂ߕ����̃`�F�b�N
	if (checkTypes & Diagonal) {

		PosIterator it1(width_, height_, minN, x, y);
		checkDir(idArys, 1, 1, minN, maxN, it1);

		PosIterator it2(width_, height_, minN, x, y);
		checkDir(idArys, -1, 1, minN, maxN, it2);
	}

	return idArys.size();
}

// �w��A�������̃`�F�b�N
unsigned MatchN::checkDir(std::vector<MatchIDInfo> &idArys, int ofsX, int ofsY, unsigned minN, unsigned maxN, Iterator &it) const {

	std::vector<int> checkedIds(board_.size(), 0); // �}�[�N�z��

	// �w������̃`�F�b�N
	for (; !it.isEnd(); it.next()) {
		unsigned x, y;
		it.getXY(x, y);

		MatchIDInfo info;
		info.val_ = getVal(x, y);

		if (info.val_ == nullity_)
			continue; // ��l�͕]�����Ȃ�

		unsigned id = getId(x, y);
		if (checkedIds[id] == 1)
			continue; // �`�F�b�N�ς݂������̂Ō������΂�

		// ���Ε����̃}�b�`�`�F�b�N
		info.ids_.push_back(id);
		checkNextMatch(checkedIds, x, y, -ofsX, -ofsY, info, minN, maxN);

		// ����id���t����
		if (info.ids_.size() > 0) {
			std::vector<int> ids;
			for (unsigned i = 0; i < info.ids_.size(); i++)
				ids.push_back(info.ids_[info.ids_.size() - i - 1]);
			info.ids_ = ids;
		}

		// �������̃}�b�`�`�F�b�N
		if (checkNextMatch(checkedIds, x, y, ofsX, ofsY, info, minN, maxN) == true)
			idArys.push_back(info);

		checkedIds[id] = 1; // ���݂̊�_�ʒu�Ƀ`�F�b�N�����
	}

	return idArys.size();
}

// ���̃}�b�`���`�F�b�N
bool MatchN::checkNextMatch(std::vector<int> &checkedIds, unsigned x, unsigned y, int ofsX, int ofsY, MatchIDInfo &info, unsigned minN, unsigned maxN) const {

	// �}�b�`���͔͈͓��H
	if (info.ids_.size() >= maxN) {
		// �͈͊O�Ȃ̂ŏI��
		return (info.ids_.size() >= minN);
	}

	// �`�F�b�N����ʒu
	unsigned tx = x + ofsX;
	unsigned ty = y + ofsY;

	unsigned id = getId(tx, ty);
	if (id == ERR_ID) {
		// �g�O�Ȃ̂ŏI��
		return (info.ids_.size() >= minN);
	}
	if (checkedIds[id] == 1)
		return (info.ids_.size() >= minN); // ���łɃ`�F�b�N�ς݂�����

	// ID�͗L��
	// �l�������Ȃ�}�b�`����
	int val = getVal(tx, ty);

	if (val == info.val_) {

		info.ids_.push_back(id);
		checkedIds[id] = 1;

		// ����Ƀ}�b�`�`�F�b�N
		return checkNextMatch(checkedIds, tx, ty, ofsX, ofsY, info, minN, maxN);
	}

	// �}�b�`���Ȃ�����
	return (info.ids_.size() >= minN);
}


//////////////////////
// ���[�e�B���e�B

// �}�b�`����ID���d��������
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

// MatchN���ێ����Ă���l�z����N���[���擾
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