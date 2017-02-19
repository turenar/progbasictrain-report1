このプログラムはプログラミング基礎演習レポート1用です。

# ディレクトリ構造

主なファイルのみ列挙します。

## Cソース

* src/ - 基本的なソースコードはこの中に有ります。
  * src/codec - ファイル入出力やGUI表示に使用するコードがここに有ります
    * src/codec/codec\_pbm.c - PBM入出力
    * src/codec/codec\_png.c - libpngを使用してPNG入出力
    * src/codec/codec\_sixel.c - libsixelを使用してsixel format出力
    * src/codec/codec\_gtk.c - GTKで表示
    * src/codec/codec\_cv.c - OpenCVで表示
	* src/codec/codec\_count.c - 黒の点を数えて出力。擬似出力形式です
  * src/filter - フィルタ関数がここに含まれます
    * src/filter/filter\_icm.c - ICMを使用してノイズ除去を行います。
    * src/filter/filter\_icm2.c - パラメータが指定できるICMを使用してノイズ除去を行います。
    * src/filter/filter\_noise.c - ノイズ付与を行います。
  * src/pbm.c - 内部で使用するPBM構造用の関数が含まれます
* test/ - テスト用のコードやシェルスクリプトが含まれます

## 出力されるバイナリ (ビルド後のみ)

* (バイナリディレクトリ/)src/
  * src/noiser\_main.c - ノイズ付与を行います
  * src/denoiser\_main.c - ノイズ除去を行います
  * src/general\_main - 汎用的画像フィルタです

## 同梱ライブラリ

* externals/cutest - テストフレームワークCuTestのソースコード一式が含まれます
