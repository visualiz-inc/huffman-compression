#include "CMakeProjectTest001.h"
#include "Huffman.h"
using namespace std;









// データを圧縮
//
// Destは必要に応じてfree()すること
// 戻り値:圧縮後のサイズ
int Huffman_Encode(const void* Src, unsigned int SrcSize, void** Dest1) {
	*Dest1 = malloc(sizeof(HUFFMAN_ENCODE_INFO) + SrcSize);
	memset(*Dest1, 0, SrcSize);




	// 結合データと数値データ、０～２５５までが数値データ
	// (結合データの数と圧縮するデータの種類の数を足すと必ず『種類の数＋(種類の数－１)』になる。
	HUFFMAN_NODE Node[256 + 255];

	unsigned char* SrcPoint, * DestPoint;
	int PressBitCounter, PressSizeCounter, SrcSizeCounter;
	HUFFMAN_ENCODE_INFO EncodeInfo;

	// void 型のポインタではアドレスの操作が出来ないので unsigned char 型のポインタにする
	SrcPoint = (unsigned char*)Src;
	DestPoint = (unsigned char*)*Dest1;

	// 各数値の圧縮後のビット列を算出する
	{
		int NodeIndex, MinNode1, MinNode2;
		int NodeNum, DataNum;

		// 数値データを初期化する
		for (unsigned int i = 0; i < 256; i++) {
			Node[i].Weight = 0;           // 出現数はこれから算出するので０に初期化
			Node[i].ChildNode[0] = -1;    // 数値データが終点なので -1 をセットする
			Node[i].ChildNode[1] = -1;    // 数値データが終点なので -1 をセットする
			Node[i].ParentNode = -1;      // まだどの要素とも結合されていないので -1 をセットする
		}

		// 各数値の出現数をカウント
		for (unsigned int i = 0; i < SrcSize; i++) {
			Node[SrcPoint[i]].Weight++;
		}

		// 出現数の少ない数値データ or 結合データを繋いで
		// 新しい結合データを作成、全ての要素を繋いで残り１個になるまで繰り返す
		DataNum = 256; // 残り要素数
		NodeNum = 256; // 次に新しく作る結合データの要素配列のインデックス
		while (DataNum > 1) {
			// 出現数値の低い要素二つを探す
			{
				MinNode1 = -1;
				MinNode2 = -1;

				// 残っている要素全てを調べるまでループ
				NodeIndex = 0;
				for (unsigned int i = 0; i < DataNum; NodeIndex++)
				{
					// もう既に何処かの要素と結合されている場合は対象外
					if (Node[NodeIndex].ParentNode != -1) continue;
					i++;

					// まだ有効な要素をセットしていないか、より出現数値の
					// 少ない要素が見つかったら更新
					if (MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight) {
						// 今まで一番出現数値が少なかったと思われた
						// 要素は二番目に降格
						MinNode2 = MinNode1;

						// 新しい一番の要素の要素配列のインデックスを保存
						MinNode1 = NodeIndex;
					}
					else {
						// 一番よりは出現数値が多くても、二番目よりは出現数値が
						// 少ないかもしれないので一応チェック(又は二番目に出現数値の
						// 少ない要素がセットされていなかった場合もセット)
						if (MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight) {
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

		// 各数値の圧縮後のビット列を割り出す
		{
			unsigned char TempBitArray[32];
			int TempBitIndex, TempBitCount, BitIndex, BitCount;

			// 数値データの種類の数だけ繰り返す
			for (unsigned int i = 0; i < 256; i++) {
				// 数値データから結合データを上へ上へと辿ってビット数を数える
				{
					// ビット数を初期化しておく
					Node[i].BitNum = 0;

					// 一時的に数値データから遡っていったときのビット列を保存する処理の準備
					TempBitIndex = 0;
					TempBitCount = 0;
					TempBitArray[TempBitIndex] = 0;

					// 何処かと結合されている限りカウントし続ける(天辺は何処とも結合されていないので終点だと分かる)
					for (NodeIndex = i; Node[NodeIndex].ParentNode != -1; NodeIndex = Node[NodeIndex].ParentNode) {
						// 配列要素一つに入るビットデータは８個なので、同じ配列要素に
						// 既に８個保存していたら次の配列要素に保存先を変更する
						if (TempBitCount == 8) {
							TempBitCount = 0;
							TempBitIndex++;
							TempBitArray[TempBitIndex] = 0;
						}

						// 新しく書き込む情報で今までのデータを上書きしてしまわないように１ビット左にシフトする
						TempBitArray[TempBitIndex] <<= 1;

						// 結合データに割り振られたインデックスを最下位ビット(一番右側のビット)に書き込む
						TempBitArray[TempBitIndex] |= (unsigned char)Node[NodeIndex].Index;

						// 保存したビット数を増やす
						TempBitCount++;

						// ビット数を増やす
						Node[i].BitNum++;
					}
				}

				// TempBitArray に溜まったデータは数値データから結合データを天辺に向かって
				// 上へ上へと遡っていった時のビット列なので、逆さまにしないと圧縮後のビット
				// 配列として使えない(展開時に天辺の結合データから数値データまで辿ることが
				// 出来ない)ので、順序を逆さまにしたものを数値データ内のビット列バッファに保存する
				{
					BitCount = 0;
					BitIndex = 0;

					// 最初のバッファを初期化しておく
					// (全部 論理和(or)演算 で書き込むので、最初から１になっている
					// ビットに０を書き込んでも１のままになってしまうため)
					Node[i].BitArray[BitIndex] = 0;

					// 一時的に保存しておいたビット列の最初まで遡る
					while (TempBitIndex >= 0) {
						// 書き込んだビット数が一つの配列要素に入る８ビットに
						// 達してしまったら次の配列要素に移る
						if (BitCount == 8) {
							BitCount = 0;
							BitIndex++;
							Node[i].BitArray[BitIndex] = 0;
						}

						// まだ何も書き込まれていないビットアドレスに１ビット書き込む
						Node[i].BitArray[BitIndex] |= (unsigned char)((TempBitArray[TempBitIndex] & 1) << BitCount);

						// 書き込み終わったビットはもういらないので次のビットを
						// 書き込めるように１ビット右にシフトする
						TempBitArray[TempBitIndex] >>= 1;

						// １ビット書き込んだので残りビット数を１個減らす
						TempBitCount--;

						// もし現在書き込み元となっている配列要素に書き込んでいない
						// ビット情報が無くなったら次の配列要素に移る
						if (TempBitCount == 0) {
							TempBitIndex--;
							TempBitCount = 8;
						}

						// 書き込んだビット数を増やす
						BitCount++;
					}
				}
			}
		}
	}

	// 変換処理
	{
		unsigned char* PressData;
		int BitData, BitCounter, BitIndex, BitNum, NodeIndex;

		// 圧縮データを格納するアドレスをセット
		// (圧縮データ本体は元のサイズ、圧縮後のサイズ、各数値の出現数等を
		// 格納するデータ領域の後に格納する)
		PressData = DestPoint + sizeof(HUFFMAN_ENCODE_INFO);

		// 圧縮するデータの参照アドレスを初期化
		SrcSizeCounter = 0;

		// 圧縮したデータの参照アドレスを初期化
		PressSizeCounter = 0;

		// 圧縮したビットデータのカウンタを初期化
		PressBitCounter = 0;

		// 圧縮データの最初のバイトを初期化しておく
		PressData[PressSizeCounter] = 0;

		// 圧縮対照のデータを全て圧縮後のビット列に変換するまでループ
		for (SrcSizeCounter = 0; SrcSizeCounter < SrcSize; SrcSizeCounter++) {
			// 保存する数値データのインデックスを取得
			NodeIndex = SrcPoint[SrcSizeCounter];

			// 指定の数値データの圧縮後のビット列を出力
			{
				// 参照する配列のインデックスを初期化
				BitIndex = 0;

				// 配列要素中の出力したビット数の初期化
				BitNum = 0;

				// 最初に書き込むビット列の配列要素をセット
				BitData = Node[NodeIndex].BitArray[0];

				// 全てのビットを出力するまでループ
				for (BitCounter = 0; BitCounter < Node[NodeIndex].BitNum; BitCounter++) {
					// もし書き込んだビット数が８個になっていたら次の配列要素に移る
					if (PressBitCounter == 8) {
						PressSizeCounter++;
						PressData[PressSizeCounter] = 0;
						PressBitCounter = 0;
					}

					// もし書き出したビット数が８個になっていたら次の配列要素に移る
					if (BitNum == 8) {
						BitIndex++;
						BitData = Node[NodeIndex].BitArray[BitIndex];
						BitNum = 0;
					}

					// まだ何も書き込まれていないビットアドレスに１ビット書き込む
					PressData[PressSizeCounter] |= (unsigned char)((BitData & 1) << PressBitCounter);

					// 書き込んだビット数を増やす
					PressBitCounter++;

					// 次に書き出すビットを最下位ビット(一番右のビット)にする為に
					// １ビット右シフトする
					BitData >>= 1;

					// 書き出したビット数を増やす
					BitNum++;
				}
			}
		}

		// 最後の１バイト分のサイズを足す
		PressSizeCounter++;
	}

	// 圧縮データの情報を保存する
	{
		// 元のデータのサイズをセット
		EncodeInfo.OriginalSize = SrcSize;

		// 圧縮後のデータのサイズをセット
		EncodeInfo.PressSize = PressSizeCounter + sizeof(HUFFMAN_ENCODE_INFO);

		// 各数値の出現数を保存する
		for (unsigned int i = 0; i < 256; i++) {
			EncodeInfo.Weight[i] = Node[i].Weight;
		}
	}

	// 圧縮データの情報を圧縮データにコピーする
	memcpy(DestPoint, &EncodeInfo, sizeof(HUFFMAN_ENCODE_INFO));


	//配列のリサイズ
	*Dest1 = realloc(*Dest1, EncodeInfo.PressSize);

	// 圧縮後のサイズを返す
	return EncodeInfo.PressSize;
}



// END