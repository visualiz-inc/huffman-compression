#pragma once



// 数値ごとの出現数や算出されたエンコード後のビット列や、結合部分の情報等の構造体
struct HUFFMAN_NODE {
	int Weight;                  // 出現数( 結合データでは出現数を足したモノ )
	int BitNum;                  // 圧縮後のビット列のビット数( 結合データでは使わない )
	unsigned char BitArray[32];  // 圧縮後のビット列( 結合データでは使わない )
	int Index;                   // 結合データに割り当てられた参照インデックス( 0 or 1 )

	int ParentNode;              // このデータを従えている結合データの要素配列のインデックス
	int ChildNode[2];            // このデータが結合させた２要素の要素配列インデックス( 結合データではない場合はどちらも -1 )
};

// 圧縮データの情報
struct HUFFMAN_ENCODE_INFO {
	int OriginalSize;  // 圧縮前のデータのサイズ(バイト数)
	int PressSize;     // 圧縮後のデータのサイズ(この構造体のサイズも含む)
	int Weight[256];   // 各数値ごとの出現回数
};



/* データ構造
* HUFFMAN_ENCODE_INFO  (1032 byte) 
* data                 (   n byte)
*/


/* 改善課題
* 安全性確保のためにヘッダーに識別子付ける
* 戻り値制御今してない
* 
*/




