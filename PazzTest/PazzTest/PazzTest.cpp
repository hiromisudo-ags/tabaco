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

	// �}�b�`N�����I�u�W�F�N�g
	MatchN match(0, w, h);

	// �Ղɒl��ݒ�i0 or 1�j
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			int val = rand() % 2;
			match.setVal(x, y, val);    // �Ղɒl��ݒ�
			printf("%s", val ? "��" : "��");
		}
		printf("\n");
	}

	// 5�A�ȏ�8�A�ȉ��̗L��������ׂă`�F�b�N
	std::vector< MatchN::MatchIDInfo > infos;
	//match.checkAll(infos, MatchN::All, 5, 8);

	//�����ΏہF�c�� �A���͈́F2�`4�A�� �����ꏊ�FX=1 Y=1
	match.check(infos, MatchN::Horizontal | MatchN::Vertical, 2, 4, 1, 1);

	// match.checkAll( infos, MatchN::Diagonal | MatchN::Horizontal, 5, 8 );  // �u�΂߂Ɖ������v�ǂ��ł��܂�

	// �}�b�`����ID���
	std::vector<int> ids = MatchNUtil::getMatchIds(infos);

	// MatchN���ێ����Ă���l�z����N���[���擾
	std::vector<int> vals = MatchNUtil::cloneValueAry(match);

	// �}�b�`���Ă��镔����"2"��
	for (unsigned i = 0; i < ids.size(); i++)
		vals[ids[i]] = 2;

	// �}�b�`�ӏ������ۂŕ\��
	printf("\n-------------\n");
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			int val = vals[w * y + x];
			printf("%s", val == 2 ? "��" : (val == 1 ? "��" : "��"));
		}
		printf("\n");
	}

	system("pause");
	return 0;
}

