// CMakeProjectTest001.cpp : アプリケーションのエントリ ポイントを定義します。
//

#define _CRT_SECURE_NO_WARNINGS
#include "CMakeProjectTest001.h"
using namespace std;




// ToDo リファレンスの作成
extern int Huffman_Encode(const void* Src, unsigned int SrcSize, void** Dest);
extern int Huffman_Decode(const void* Press, void** Dest);










/*
* ハフマン圧縮と展開のテスト
* 　入力データを圧縮して　a.binに保存
* 　展開して a.txtに保存
* 
* 
* dll化や、データ制御API化したdll開発は後日
*/
int main(){
	char* buf1, * buf2;		//圧縮・展開されたデータの保存バッファ（手動解放必要）
	const char* ss = u8"あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほ。1234567890";	//テストデータ
	const int inputSize = (int)strlen(ss);
	int outputSize;





	system("chcp 65001");	//UTF8対応
	cout << "圧縮前サイズ" << inputSize << endl;

	//圧縮実験
	outputSize = Huffman_Encode(ss, inputSize, (void**)&buf1);
	FILE* fp;
	fp = fopen("a.bin", "w");
	fwrite(buf1, 1, outputSize, fp);
	fclose(fp);
	cout << "圧縮後サイズ 1032+" << outputSize - 1032 << endl;


	//展開実験
	outputSize = Huffman_Decode(buf1, (void**)&buf2);
	fp = fopen("a.txt", "w");
	fwrite(buf2, 1, outputSize, fp);
	fclose(fp);
	cout << "展開後サイズ" << outputSize << endl;;


	//メモリ解放
	free(buf1);
	free(buf2);


	return 0;
}


