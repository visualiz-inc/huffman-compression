#pragma once



// ���l���Ƃ̏o������Z�o���ꂽ�G���R�[�h��̃r�b�g���A���������̏�񓙂̍\����
struct HUFFMAN_NODE {
	int Weight;                  // �o����( �����f�[�^�ł͏o�����𑫂������m )
	int BitNum;                  // ���k��̃r�b�g��̃r�b�g��( �����f�[�^�ł͎g��Ȃ� )
	unsigned char BitArray[32];  // ���k��̃r�b�g��( �����f�[�^�ł͎g��Ȃ� )
	int Index;                   // �����f�[�^�Ɋ��蓖�Ă�ꂽ�Q�ƃC���f�b�N�X( 0 or 1 )

	int ParentNode;              // ���̃f�[�^���]���Ă��錋���f�[�^�̗v�f�z��̃C���f�b�N�X
	int ChildNode[2];            // ���̃f�[�^�������������Q�v�f�̗v�f�z��C���f�b�N�X( �����f�[�^�ł͂Ȃ��ꍇ�͂ǂ���� -1 )
};

// ���k�f�[�^�̏��
struct HUFFMAN_ENCODE_INFO {
	int OriginalSize;  // ���k�O�̃f�[�^�̃T�C�Y(�o�C�g��)
	int PressSize;     // ���k��̃f�[�^�̃T�C�Y(���̍\���̂̃T�C�Y���܂�)
	int Weight[256];   // �e���l���Ƃ̏o����
};



/* �f�[�^�\��
* HUFFMAN_ENCODE_INFO  (1032 byte) 
* data                 (   n byte)
*/


/* ���P�ۑ�
* ���S���m�ۂ̂��߂Ƀw�b�_�[�Ɏ��ʎq�t����
* �߂�l���䍡���ĂȂ�
* 
*/




