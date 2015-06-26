#pragma once


#include <vector>

class MatchN {
public:

	enum Error {
		ERR_ID = 0xffffffff
	};

	// ���������w��t���O
	static const unsigned Horizontal = 1; // ��������
	static const unsigned Vertical = 2; // ��������
	static const unsigned Diagonal = 4; // �΂ߕ���
	static const unsigned All = Horizontal | Vertical | Diagonal; // �S����

	// �L����\����
	struct MatchIDInfo {
		int val_; // �Ώ�ID
		std::vector<int> ids_; // �}�XID
	};

protected:
	// �����C�e���[�^
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

	// �c�E�΂ߕ����p�C�e���[�^
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

	// �������p�C�e���[�^
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

	// �w��ʒu�C�e���[�^
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

	std::vector< int > board_; // �Ձiwidth_�~height�j
	unsigned width_; // �Ղ̗�
	unsigned height_; // �Ղ̍s��
	const int nullity_; // ��̒l

protected:
	// �w��A�������̃`�F�b�N
	// idArys: �L������i�[����z��
	// minN : �ŏ��A����
	// maxN : �ő�A����
	// : ���ׂ����
	unsigned checkDir(std::vector<MatchIDInfo> &idArys, int ofsX, int ofsY, unsigned minN, unsigned maxN, Iterator &it) const;

	// ���̃}�b�`���`�F�b�N
	// checkedIds: ���łɃ`�F�b�N����Ă���Id�Q
	// x : ���O��X�ʒu
	// y : ���O��Y�ʒu
	// ofsX : �`�F�b�N����X
	// ofsY : �`�F�b�N����Y
	// info : �A���i�[
	// minN : �ŏ��A����
	// maxN : �ő�A����
	bool checkNextMatch(std::vector<int> &checkedIds, unsigned x, unsigned y, int ofsX, int ofsY, MatchIDInfo &info, unsigned minN, unsigned maxN) const;

public:
	// �R���X�g���N�^
	// nullity: ��̒l
	// width : �Ղ̗񐔁i���j
	// height : �Ղ̍s���i�����j
	MatchN(int nullity);
	MatchN(int nullity, unsigned width, unsigned height);

	// �{�[�h��ݒ�
	// width : �Ղ̗񐔁i���j
	// height : �Ղ̍s���i�����j
	bool init(unsigned width, unsigned height);

	// �{�[�h�̑傫�����擾
	// width : �Ղ̗񐔁i���j
	// height : �Ղ̍s���i�����j
	void getBoardSize(unsigned &width, unsigned &height) const;

	// �{�[�h�̒l���N���A
	void clear();

	// XY�ʒu�ɒl���Z�b�g
	// x: �Ղ̍s�i���ʒu�j
	// y: �Ղ̗�i�c�ʒu�j
	bool setVal(unsigned x, unsigned y, int val);

	// Id�ɒl���Z�b�g
	// id: �Ղ̃}�XID
	bool setVal(unsigned id, int val);

	// XY�ʒu��Id�ɕϊ�
	// x: �Ղ̍s�i���ʒu�j
	// y: �Ղ̗�i�c�ʒu�j
	unsigned getId(unsigned x, unsigned y) const;

	// XY�ʒu�̒l���擾
	// x: �Ղ̍s�i���ʒu�j
	// y: �Ղ̗�i�c�ʒu�j
	int getVal(unsigned x, unsigned y) const;

	// Id�̒l���擾
	// id: �Ղ̃}�XID
	int getVal(unsigned id) const;

	// �w��^�C�v��N�A�}�b�`�����o
	// idArys : �}�b�`�����o��
	// checkTypes: �c���΂߂̃`�F�b�N�����t���O�ior�ŘA���j
	// minN : �ŏ��A����
	// maxN : �ő�A����
	unsigned checkAll(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN) const;

	// �w��ʒu��N�A�}�b�`���Ă��邩���o
	// idArys : �}�b�`�����o��
	// checkTypes: �c���΂߂̃`�F�b�N�����t���O�ior�ŘA���j
	// minN : �ŏ��A����
	// maxN : �ő�A����
	// x : �Ղ̍s�i���ʒu�j
	// y : �Ղ̗�i�c�ʒu�j
	unsigned check(std::vector<MatchIDInfo> &idArys, unsigned checkTypes, unsigned minN, unsigned maxN, unsigned x, unsigned y) const;
};



//////////////////////
// ���[�e�B���e�B

class MatchNUtil {
public:
	// �}�b�`����ID���d��������
	static std::vector<int> getMatchIds(const std::vector<MatchN::MatchIDInfo> &idArys);

	// MatchN���ێ����Ă���l�z����N���[���擾
	static std::vector<int> cloneValueAry(const MatchN &match);
};

