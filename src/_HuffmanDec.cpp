#include "CMakeProjectTest001.h"
#include "Huffman.h"
using namespace std;





// 圧縮データを解凍
//
// Destは必要に応じてfree()すること
// 戻り値:解凍後のサイズ
int Huffman_Decode(const void* Press, void** Dest1){
    // 結合データと数値データ、０～２５５までが数値データ
    HUFFMAN_NODE Node[256 + 255];

    int PressSizeCounter, DestSizeCounter, DestSize;
    unsigned char* PressPoint, * DestPoint;
    int i;
    HUFFMAN_ENCODE_INFO EncodeInfo;


    

    // void 型のポインタではアドレスの操作が出来ないので unsigned char 型のポインタにする
    PressPoint = (unsigned char*)Press;
    

    // 圧縮データの情報を取得する
    memcpy(&EncodeInfo, Press, sizeof(HUFFMAN_ENCODE_INFO));

    // 解凍後のデータのサイズを取得する
    DestSize = EncodeInfo.OriginalSize;

    //展開用領域確保
    unsigned char* Dest2 = (unsigned char*)malloc(DestSize);
    DestPoint = Dest2;



    // 各数値の結合データを構築する
    {
        int NodeIndex, MinNode1, MinNode2;
        int NodeNum, DataNum;

        // 数値データを初期化する
        for (i = 0; i < 256; i++)
        {
            Node[i].Weight = EncodeInfo.Weight[i];    // 出現数は保存しておいたデータからコピー
            Node[i].ChildNode[0] = -1;    // 数値データが終点なので -1 をセットする
            Node[i].ChildNode[1] = -1;    // 数値データが終点なので -1 をセットする
            Node[i].ParentNode = -1;      // まだどの要素とも結合されていないので -1 をセットする
        }

        // 出現数の少ない数値データ or 結合データを繋いで
        // 新しい結合データを作成、全ての要素を繋いで残り１個になるまで繰り返す
        // (圧縮時と同じコードです)
        DataNum = 256; // 残り要素数
        NodeNum = 256; // 次に新しく作る結合データの要素配列のインデックス
        while (DataNum > 1)
        {
            // 出現数値の低い要素二つを探す
            {
                MinNode1 = -1;
                MinNode2 = -1;

                // 残っている要素全てを調べるまでループ
                NodeIndex = 0;
                for (i = 0; i < DataNum; NodeIndex++)
                {
                    // もう既に何処かの要素と結合されている場合は対象外
                    if (Node[NodeIndex].ParentNode != -1) continue;

                    i++;

                    // まだ有効な要素をセットしていないか、より出現数値の
                    // 少ない要素が見つかったら更新
                    if (MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight)
                    {
                        // 今まで一番出現数値が少なかったと思われた
                        // 要素は二番目に降格
                        MinNode2 = MinNode1;

                        // 新しい一番の要素の要素配列のインデックスを保存
                        MinNode1 = NodeIndex;
                    }
                    else
                    {
                        // 一番よりは出現数値が多くても、二番目よりは出現数値が
                        // 少ないかもしれないので一応チェック(又は二番目に出現数値の
                        // 少ない要素がセットされていなかった場合もセット)
                        if (MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight)
                        {
                            MinNode2 = NodeIndex;
                        }
                    }
                }
            }

            // 二つの要素を繋いで新しい要素(結合データ)を作る
            Node[NodeNum].ParentNode = -1;  // 新しいデータは当然まだ何処とも繋がっていないので -1 
            Node[NodeNum].Weight = Node[MinNode1].Weight + Node[MinNode2].Weight;    // 出現数値は二つの数値を足したものをセットする
            Node[NodeNum].ChildNode[0] = MinNode1;    // この結合部で 0 を選んだら出現数値が一番少ない要素に繋がる
            Node[NodeNum].ChildNode[1] = MinNode2;    // この結合部で 1 を選んだら出現数値が二番目に少ない要素に繋がる

            // 結合された要素二つに、自分達に何の値が割り当てられたかをセットする
            Node[MinNode1].Index = 0;    // 一番出現数値が少ない要素は 0 番
            Node[MinNode2].Index = 1;    // 二番目に出現数値が少ない要素は 1 番

            // 結合された要素二つに、自分達を結合した結合データの要素配列インデックスをセットする
            Node[MinNode1].ParentNode = NodeNum;
            Node[MinNode2].ParentNode = NodeNum;

            // 要素の数を一個増やす
            NodeNum++;

            // 残り要素の数は、一つ要素が新しく追加された代わりに
            // 二つの要素が結合されて検索の対象から外れたので
            // 結果 1 - 2 で -1 
            DataNum--;
        }
    }

    // 解凍処理
    {
        unsigned char* PressData;
        int PressBitCounter, PressBitData, Index, NodeIndex;

        // 圧縮データ本体の先頭アドレスをセット
        // (圧縮データ本体は元のサイズ、圧縮後のサイズ、各数値の出現数等を
        // 格納するデータ領域の後にある)
        PressData = PressPoint + sizeof(HUFFMAN_ENCODE_INFO);

        // 圧縮データの参照アドレスを初期化
        PressSizeCounter = 0;

        // 圧縮ビットデータのカウンタを初期化
        PressBitCounter = 0;

        // 圧縮データの１バイト目をセット
        PressBitData = PressData[PressSizeCounter];

        // 圧縮前のデータサイズになるまで解凍処理を繰り返す
        for (DestSizeCounter = 0; DestSizeCounter < DestSize; DestSizeCounter++){
            // ビット列から数値データを検索する
            {
                // 結合データの天辺は一番最後の結合データが格納される５１０番目(０番から数える)
                // 天辺から順に下に降りていく
                NodeIndex = 510;

                // 数値データに辿り着くまで結合データを下りていく
                while (Node[NodeIndex].ChildNode[0] != -1 || Node[NodeIndex].ChildNode[1] != -1)
                {
                    // もし PressBitData に格納されている全ての
                    // ビットデータを使い切ってしまった場合は次の
                    // ビットデータをセットする
                    if (PressBitCounter == 8)
                    {
                        PressSizeCounter++;
                        PressBitData = PressData[PressSizeCounter];
                        PressBitCounter = 0;
                    }

                    // １ビット取得する
                    Index = PressBitData & 1;

                    // 使用した１ビット分だけ右にシフトする
                    PressBitData >>= 1;

                    // 使用したビット数を一個増やす
                    PressBitCounter++;

                    // 次の要素(結合データか数値データかはまだ分からない)に移る
                    NodeIndex = Node[NodeIndex].ChildNode[Index];
                }
            }

            // 辿り着いた数値データを出力
            DestPoint[DestSizeCounter] = (unsigned char)NodeIndex;
        }
    }


    //確保した配列を渡す
    *Dest1 = Dest2;

    // 解凍後のサイズを返す
    return EncodeInfo.OriginalSize;
}





// END