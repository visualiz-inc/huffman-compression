#include "CMakeProjectTest001.h"
#include "Huffman.h"
using namespace std;









// �f�[�^�����k
//
// Dest�͕K�v�ɉ�����free()���邱��
// �߂�l:���k��̃T�C�Y
int Huffman_Encode(const void* Src, unsigned int SrcSize, void** Dest1) {
	*Dest1 = malloc(sizeof(HUFFMAN_ENCODE_INFO) + SrcSize);
	memset(*Dest1, 0, SrcSize);




	// �����f�[�^�Ɛ��l�f�[�^�A�O�`�Q�T�T�܂ł����l�f�[�^
	// (�����f�[�^�̐��ƈ��k����f�[�^�̎�ނ̐��𑫂��ƕK���w��ނ̐��{(��ނ̐��|�P)�x�ɂȂ�B
	HUFFMAN_NODE Node[256 + 255];

	unsigned char* SrcPoint, * DestPoint;
	int PressBitCounter, PressSizeCounter, SrcSizeCounter;
	HUFFMAN_ENCODE_INFO EncodeInfo;

	// void �^�̃|�C���^�ł̓A�h���X�̑��삪�o���Ȃ��̂� unsigned char �^�̃|�C���^�ɂ���
	SrcPoint = (unsigned char*)Src;
	DestPoint = (unsigned char*)*Dest1;

	// �e���l�̈��k��̃r�b�g����Z�o����
	{
		int NodeIndex, MinNode1, MinNode2;
		int NodeNum, DataNum;

		// ���l�f�[�^������������
		for (unsigned int i = 0; i < 256; i++) {
			Node[i].Weight = 0;           // �o�����͂��ꂩ��Z�o����̂łO�ɏ�����
			Node[i].ChildNode[0] = -1;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
			Node[i].ChildNode[1] = -1;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
			Node[i].ParentNode = -1;      // �܂��ǂ̗v�f�Ƃ���������Ă��Ȃ��̂� -1 ���Z�b�g����
		}

		// �e���l�̏o�������J�E���g
		for (unsigned int i = 0; i < SrcSize; i++) {
			Node[SrcPoint[i]].Weight++;
		}

		// �o�����̏��Ȃ����l�f�[�^ or �����f�[�^���q����
		// �V���������f�[�^���쐬�A�S�Ă̗v�f���q���Ŏc��P�ɂȂ�܂ŌJ��Ԃ�
		DataNum = 256; // �c��v�f��
		NodeNum = 256; // ���ɐV������錋���f�[�^�̗v�f�z��̃C���f�b�N�X
		while (DataNum > 1) {
			// �o�����l�̒Ⴂ�v�f���T��
			{
				MinNode1 = -1;
				MinNode2 = -1;

				// �c���Ă���v�f�S�Ă𒲂ׂ�܂Ń��[�v
				NodeIndex = 0;
				for (unsigned int i = 0; i < DataNum; NodeIndex++)
				{
					// �������ɉ������̗v�f�ƌ�������Ă���ꍇ�͑ΏۊO
					if (Node[NodeIndex].ParentNode != -1) continue;
					i++;

					// �܂��L���ȗv�f���Z�b�g���Ă��Ȃ����A���o�����l��
					// ���Ȃ��v�f������������X�V
					if (MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight) {
						// ���܂ň�ԏo�����l�����Ȃ������Ǝv��ꂽ
						// �v�f�͓�Ԗڂɍ~�i
						MinNode2 = MinNode1;

						// �V������Ԃ̗v�f�̗v�f�z��̃C���f�b�N�X��ۑ�
						MinNode1 = NodeIndex;
					}
					else {
						// ��Ԃ��͏o�����l�������Ă��A��Ԗڂ��͏o�����l��
						// ���Ȃ���������Ȃ��̂ňꉞ�`�F�b�N(���͓�Ԗڂɏo�����l��
						// ���Ȃ��v�f���Z�b�g����Ă��Ȃ������ꍇ���Z�b�g)
						if (MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight) {
							MinNode2 = NodeIndex;
						}
					}
				}
			}

			// ��̗v�f���q���ŐV�����v�f(�����f�[�^)�����
			Node[NodeNum].ParentNode = -1;  // �V�����f�[�^�͓��R�܂������Ƃ��q�����Ă��Ȃ��̂� -1 
			Node[NodeNum].Weight = Node[MinNode1].Weight + Node[MinNode2].Weight;    // �o�����l�͓�̐��l�𑫂������̂��Z�b�g����
			Node[NodeNum].ChildNode[0] = MinNode1;    // ���̌������� 0 ��I�񂾂�o�����l����ԏ��Ȃ��v�f�Ɍq����
			Node[NodeNum].ChildNode[1] = MinNode2;    // ���̌������� 1 ��I�񂾂�o�����l����Ԗڂɏ��Ȃ��v�f�Ɍq����

			// �������ꂽ�v�f��ɁA�����B�ɉ��̒l�����蓖�Ă�ꂽ�����Z�b�g����
			Node[MinNode1].Index = 0;    // ��ԏo�����l�����Ȃ��v�f�� 0 ��
			Node[MinNode2].Index = 1;    // ��Ԗڂɏo�����l�����Ȃ��v�f�� 1 ��

			// �������ꂽ�v�f��ɁA�����B���������������f�[�^�̗v�f�z��C���f�b�N�X���Z�b�g����
			Node[MinNode1].ParentNode = NodeNum;
			Node[MinNode2].ParentNode = NodeNum;

			// �v�f�̐�������₷
			NodeNum++;

			// �c��v�f�̐��́A��v�f���V�����ǉ����ꂽ�����
			// ��̗v�f����������Č����̑Ώۂ���O�ꂽ�̂�
			// ���� 1 - 2 �� -1 
			DataNum--;
		}

		// �e���l�̈��k��̃r�b�g�������o��
		{
			unsigned char TempBitArray[32];
			int TempBitIndex, TempBitCount, BitIndex, BitCount;

			// ���l�f�[�^�̎�ނ̐������J��Ԃ�
			for (unsigned int i = 0; i < 256; i++) {
				// ���l�f�[�^���猋���f�[�^����֏�ւƒH���ăr�b�g���𐔂���
				{
					// �r�b�g�������������Ă���
					Node[i].BitNum = 0;

					// �ꎞ�I�ɐ��l�f�[�^����k���Ă������Ƃ��̃r�b�g���ۑ����鏈���̏���
					TempBitIndex = 0;
					TempBitCount = 0;
					TempBitArray[TempBitIndex] = 0;

					// �������ƌ�������Ă������J�E���g��������(�V�ӂ͉����Ƃ���������Ă��Ȃ��̂ŏI�_���ƕ�����)
					for (NodeIndex = i; Node[NodeIndex].ParentNode != -1; NodeIndex = Node[NodeIndex].ParentNode) {
						// �z��v�f��ɓ���r�b�g�f�[�^�͂W�Ȃ̂ŁA�����z��v�f��
						// ���ɂW�ۑ����Ă����玟�̔z��v�f�ɕۑ����ύX����
						if (TempBitCount == 8) {
							TempBitCount = 0;
							TempBitIndex++;
							TempBitArray[TempBitIndex] = 0;
						}

						// �V�����������ޏ��ō��܂ł̃f�[�^���㏑�����Ă��܂�Ȃ��悤�ɂP�r�b�g���ɃV�t�g����
						TempBitArray[TempBitIndex] <<= 1;

						// �����f�[�^�Ɋ���U��ꂽ�C���f�b�N�X���ŉ��ʃr�b�g(��ԉE���̃r�b�g)�ɏ�������
						TempBitArray[TempBitIndex] |= (unsigned char)Node[NodeIndex].Index;

						// �ۑ������r�b�g���𑝂₷
						TempBitCount++;

						// �r�b�g���𑝂₷
						Node[i].BitNum++;
					}
				}

				// TempBitArray �ɗ��܂����f�[�^�͐��l�f�[�^���猋���f�[�^��V�ӂɌ�������
				// ��֏�ւƑk���Ă��������̃r�b�g��Ȃ̂ŁA�t���܂ɂ��Ȃ��ƈ��k��̃r�b�g
				// �z��Ƃ��Ďg���Ȃ�(�W�J���ɓV�ӂ̌����f�[�^���琔�l�f�[�^�܂ŒH�邱�Ƃ�
				// �o���Ȃ�)�̂ŁA�������t���܂ɂ������̂𐔒l�f�[�^���̃r�b�g��o�b�t�@�ɕۑ�����
				{
					BitCount = 0;
					BitIndex = 0;

					// �ŏ��̃o�b�t�@�����������Ă���
					// (�S�� �_���a(or)���Z �ŏ������ނ̂ŁA�ŏ�����P�ɂȂ��Ă���
					// �r�b�g�ɂO����������ł��P�̂܂܂ɂȂ��Ă��܂�����)
					Node[i].BitArray[BitIndex] = 0;

					// �ꎞ�I�ɕۑ����Ă������r�b�g��̍ŏ��܂ők��
					while (TempBitIndex >= 0) {
						// �������񂾃r�b�g������̔z��v�f�ɓ���W�r�b�g��
						// �B���Ă��܂����玟�̔z��v�f�Ɉڂ�
						if (BitCount == 8) {
							BitCount = 0;
							BitIndex++;
							Node[i].BitArray[BitIndex] = 0;
						}

						// �܂������������܂�Ă��Ȃ��r�b�g�A�h���X�ɂP�r�b�g��������
						Node[i].BitArray[BitIndex] |= (unsigned char)((TempBitArray[TempBitIndex] & 1) << BitCount);

						// �������ݏI������r�b�g�͂�������Ȃ��̂Ŏ��̃r�b�g��
						// �������߂�悤�ɂP�r�b�g�E�ɃV�t�g����
						TempBitArray[TempBitIndex] >>= 1;

						// �P�r�b�g�������񂾂̂Ŏc��r�b�g�����P���炷
						TempBitCount--;

						// �������ݏ������݌��ƂȂ��Ă���z��v�f�ɏ�������ł��Ȃ�
						// �r�b�g��񂪖����Ȃ����玟�̔z��v�f�Ɉڂ�
						if (TempBitCount == 0) {
							TempBitIndex--;
							TempBitCount = 8;
						}

						// �������񂾃r�b�g���𑝂₷
						BitCount++;
					}
				}
			}
		}
	}

	// �ϊ�����
	{
		unsigned char* PressData;
		int BitData, BitCounter, BitIndex, BitNum, NodeIndex;

		// ���k�f�[�^���i�[����A�h���X���Z�b�g
		// (���k�f�[�^�{�̂͌��̃T�C�Y�A���k��̃T�C�Y�A�e���l�̏o��������
		// �i�[����f�[�^�̈�̌�Ɋi�[����)
		PressData = DestPoint + sizeof(HUFFMAN_ENCODE_INFO);

		// ���k����f�[�^�̎Q�ƃA�h���X��������
		SrcSizeCounter = 0;

		// ���k�����f�[�^�̎Q�ƃA�h���X��������
		PressSizeCounter = 0;

		// ���k�����r�b�g�f�[�^�̃J�E���^��������
		PressBitCounter = 0;

		// ���k�f�[�^�̍ŏ��̃o�C�g�����������Ă���
		PressData[PressSizeCounter] = 0;

		// ���k�ΏƂ̃f�[�^��S�Ĉ��k��̃r�b�g��ɕϊ�����܂Ń��[�v
		for (SrcSizeCounter = 0; SrcSizeCounter < SrcSize; SrcSizeCounter++) {
			// �ۑ����鐔�l�f�[�^�̃C���f�b�N�X���擾
			NodeIndex = SrcPoint[SrcSizeCounter];

			// �w��̐��l�f�[�^�̈��k��̃r�b�g����o��
			{
				// �Q�Ƃ���z��̃C���f�b�N�X��������
				BitIndex = 0;

				// �z��v�f���̏o�͂����r�b�g���̏�����
				BitNum = 0;

				// �ŏ��ɏ������ރr�b�g��̔z��v�f���Z�b�g
				BitData = Node[NodeIndex].BitArray[0];

				// �S�Ẵr�b�g���o�͂���܂Ń��[�v
				for (BitCounter = 0; BitCounter < Node[NodeIndex].BitNum; BitCounter++) {
					// �����������񂾃r�b�g�����W�ɂȂ��Ă����玟�̔z��v�f�Ɉڂ�
					if (PressBitCounter == 8) {
						PressSizeCounter++;
						PressData[PressSizeCounter] = 0;
						PressBitCounter = 0;
					}

					// ���������o�����r�b�g�����W�ɂȂ��Ă����玟�̔z��v�f�Ɉڂ�
					if (BitNum == 8) {
						BitIndex++;
						BitData = Node[NodeIndex].BitArray[BitIndex];
						BitNum = 0;
					}

					// �܂������������܂�Ă��Ȃ��r�b�g�A�h���X�ɂP�r�b�g��������
					PressData[PressSizeCounter] |= (unsigned char)((BitData & 1) << PressBitCounter);

					// �������񂾃r�b�g���𑝂₷
					PressBitCounter++;

					// ���ɏ����o���r�b�g���ŉ��ʃr�b�g(��ԉE�̃r�b�g)�ɂ���ׂ�
					// �P�r�b�g�E�V�t�g����
					BitData >>= 1;

					// �����o�����r�b�g���𑝂₷
					BitNum++;
				}
			}
		}

		// �Ō�̂P�o�C�g���̃T�C�Y�𑫂�
		PressSizeCounter++;
	}

	// ���k�f�[�^�̏���ۑ�����
	{
		// ���̃f�[�^�̃T�C�Y���Z�b�g
		EncodeInfo.OriginalSize = SrcSize;

		// ���k��̃f�[�^�̃T�C�Y���Z�b�g
		EncodeInfo.PressSize = PressSizeCounter + sizeof(HUFFMAN_ENCODE_INFO);

		// �e���l�̏o������ۑ�����
		for (unsigned int i = 0; i < 256; i++) {
			EncodeInfo.Weight[i] = Node[i].Weight;
		}
	}

	// ���k�f�[�^�̏������k�f�[�^�ɃR�s�[����
	memcpy(DestPoint, &EncodeInfo, sizeof(HUFFMAN_ENCODE_INFO));


	//�z��̃��T�C�Y
	*Dest1 = realloc(*Dest1, EncodeInfo.PressSize);

	// ���k��̃T�C�Y��Ԃ�
	return EncodeInfo.PressSize;
}



// END