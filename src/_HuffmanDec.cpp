#include "CMakeProjectTest001.h"
#include "Huffman.h"
using namespace std;





// ���k�f�[�^����
//
// Dest�͕K�v�ɉ�����free()���邱��
// �߂�l:�𓀌�̃T�C�Y
int Huffman_Decode(const void* Press, void** Dest1){
    // �����f�[�^�Ɛ��l�f�[�^�A�O�`�Q�T�T�܂ł����l�f�[�^
    HUFFMAN_NODE Node[256 + 255];

    int PressSizeCounter, DestSizeCounter, DestSize;
    unsigned char* PressPoint, * DestPoint;
    int i;
    HUFFMAN_ENCODE_INFO EncodeInfo;


    

    // void �^�̃|�C���^�ł̓A�h���X�̑��삪�o���Ȃ��̂� unsigned char �^�̃|�C���^�ɂ���
    PressPoint = (unsigned char*)Press;
    

    // ���k�f�[�^�̏����擾����
    memcpy(&EncodeInfo, Press, sizeof(HUFFMAN_ENCODE_INFO));

    // �𓀌�̃f�[�^�̃T�C�Y���擾����
    DestSize = EncodeInfo.OriginalSize;

    //�W�J�p�̈�m��
    unsigned char* Dest2 = (unsigned char*)malloc(DestSize);
    DestPoint = Dest2;



    // �e���l�̌����f�[�^���\�z����
    {
        int NodeIndex, MinNode1, MinNode2;
        int NodeNum, DataNum;

        // ���l�f�[�^������������
        for (i = 0; i < 256; i++)
        {
            Node[i].Weight = EncodeInfo.Weight[i];    // �o�����͕ۑ����Ă������f�[�^����R�s�[
            Node[i].ChildNode[0] = -1;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ChildNode[1] = -1;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ParentNode = -1;      // �܂��ǂ̗v�f�Ƃ���������Ă��Ȃ��̂� -1 ���Z�b�g����
        }

        // �o�����̏��Ȃ����l�f�[�^ or �����f�[�^���q����
        // �V���������f�[�^���쐬�A�S�Ă̗v�f���q���Ŏc��P�ɂȂ�܂ŌJ��Ԃ�
        // (���k���Ɠ����R�[�h�ł�)
        DataNum = 256; // �c��v�f��
        NodeNum = 256; // ���ɐV������錋���f�[�^�̗v�f�z��̃C���f�b�N�X
        while (DataNum > 1)
        {
            // �o�����l�̒Ⴂ�v�f���T��
            {
                MinNode1 = -1;
                MinNode2 = -1;

                // �c���Ă���v�f�S�Ă𒲂ׂ�܂Ń��[�v
                NodeIndex = 0;
                for (i = 0; i < DataNum; NodeIndex++)
                {
                    // �������ɉ������̗v�f�ƌ�������Ă���ꍇ�͑ΏۊO
                    if (Node[NodeIndex].ParentNode != -1) continue;

                    i++;

                    // �܂��L���ȗv�f���Z�b�g���Ă��Ȃ����A���o�����l��
                    // ���Ȃ��v�f������������X�V
                    if (MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight)
                    {
                        // ���܂ň�ԏo�����l�����Ȃ������Ǝv��ꂽ
                        // �v�f�͓�Ԗڂɍ~�i
                        MinNode2 = MinNode1;

                        // �V������Ԃ̗v�f�̗v�f�z��̃C���f�b�N�X��ۑ�
                        MinNode1 = NodeIndex;
                    }
                    else
                    {
                        // ��Ԃ��͏o�����l�������Ă��A��Ԗڂ��͏o�����l��
                        // ���Ȃ���������Ȃ��̂ňꉞ�`�F�b�N(���͓�Ԗڂɏo�����l��
                        // ���Ȃ��v�f���Z�b�g����Ă��Ȃ������ꍇ���Z�b�g)
                        if (MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight)
                        {
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
    }

    // �𓀏���
    {
        unsigned char* PressData;
        int PressBitCounter, PressBitData, Index, NodeIndex;

        // ���k�f�[�^�{�̂̐擪�A�h���X���Z�b�g
        // (���k�f�[�^�{�̂͌��̃T�C�Y�A���k��̃T�C�Y�A�e���l�̏o��������
        // �i�[����f�[�^�̈�̌�ɂ���)
        PressData = PressPoint + sizeof(HUFFMAN_ENCODE_INFO);

        // ���k�f�[�^�̎Q�ƃA�h���X��������
        PressSizeCounter = 0;

        // ���k�r�b�g�f�[�^�̃J�E���^��������
        PressBitCounter = 0;

        // ���k�f�[�^�̂P�o�C�g�ڂ��Z�b�g
        PressBitData = PressData[PressSizeCounter];

        // ���k�O�̃f�[�^�T�C�Y�ɂȂ�܂ŉ𓀏������J��Ԃ�
        for (DestSizeCounter = 0; DestSizeCounter < DestSize; DestSizeCounter++){
            // �r�b�g�񂩂琔�l�f�[�^����������
            {
                // �����f�[�^�̓V�ӂ͈�ԍŌ�̌����f�[�^���i�[�����T�P�O�Ԗ�(�O�Ԃ��琔����)
                // �V�ӂ��珇�ɉ��ɍ~��Ă���
                NodeIndex = 510;

                // ���l�f�[�^�ɒH�蒅���܂Ō����f�[�^������Ă���
                while (Node[NodeIndex].ChildNode[0] != -1 || Node[NodeIndex].ChildNode[1] != -1)
                {
                    // ���� PressBitData �Ɋi�[����Ă���S�Ă�
                    // �r�b�g�f�[�^���g���؂��Ă��܂����ꍇ�͎���
                    // �r�b�g�f�[�^���Z�b�g����
                    if (PressBitCounter == 8)
                    {
                        PressSizeCounter++;
                        PressBitData = PressData[PressSizeCounter];
                        PressBitCounter = 0;
                    }

                    // �P�r�b�g�擾����
                    Index = PressBitData & 1;

                    // �g�p�����P�r�b�g�������E�ɃV�t�g����
                    PressBitData >>= 1;

                    // �g�p�����r�b�g��������₷
                    PressBitCounter++;

                    // ���̗v�f(�����f�[�^�����l�f�[�^���͂܂�������Ȃ�)�Ɉڂ�
                    NodeIndex = Node[NodeIndex].ChildNode[Index];
                }
            }

            // �H�蒅�������l�f�[�^���o��
            DestPoint[DestSizeCounter] = (unsigned char)NodeIndex;
        }
    }


    //�m�ۂ����z���n��
    *Dest1 = Dest2;

    // �𓀌�̃T�C�Y��Ԃ�
    return EncodeInfo.OriginalSize;
}





// END