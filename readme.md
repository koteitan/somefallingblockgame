#some falling block puzzle game

![playing movie](play.gif)

* English
 * How to Play
 * How to load
 * Attention
 * Tested Environment
  
* 日本語
 * 遊び方
 * ロードの仕方
 * ご注意
 * テスト済み環境
 
# How to Play

Under construction
* Key:
 * DLR ... motion
 * AB  ... lotation

#How to load

* Install Arduino IDE
 * https://www.arduino.cc/en/Main/Software
* Set the preferences below of Arduino IDE:
 * File > Preferences > Settings Tab > Additional Boards Manager URLs = 
   https://arduboy.github.io/board-support/package_arduboy_index.json
 * Tool > Board       = Arduboy
 * Tool > Serial Port = the port of Arduboy Leonardo
* Get the sketch
 * git clone xttps://github.com/koteitan/somefallingblockgame.git or https://github.com/koteitan/somefallingblockgame/archive/master.zip
* open "somefallingblockgame/somefallingblockgame.ino" with Arduino IDE.
* Connect USB cable between your pc and Arduboy.
* Press upload button.

# Attention

* After uploading this game sketch, you may not be able to uploading next game sketch. If you get the error message below, please use the recover method below to upload the next sketch:
 * Error message:
    Couldn't find a Board on the selected port. Check that you have the correct port selected.  If it is correct, try pressing the board's reset button after initiating the upload.
 * Recover method (PORTS-Reset Method):
  1. Please set: ArduinoIDE > File > Preferences > Settings > Show verbose output during: upload = checked
  2. Keep to press a reset button of Arduboy at the side of the USB cable.
  3. Push upload button of Arduino IDE.
  4. Release the reset button of Arduboy when a message "PORTS {} / {} => {}" is output continuously.

# Tested Environment
* Arduboy 1.0
* Arduboy Library:
 * https://github.com/Arduboy/Arduboy/
 * revision: 3c409fefb
 * branch  : master
* Arduino IDE 1.6.10
---------------------------
遊び方  
---------------------------
- キー:
 * DLR ... motion
 * AB  ... lotation
---------------------------
ロードの仕方
---------------------------
- Arduino IDE をインストールしてください:
  - https://www.arduino.cc/en/Main/Software
- 下記のArduino IDE の設定をして下さい:
  - ファイル > 環境設定 > 追加のボードマネージャのURL = 
    https://arduboy.github.io/board-support/package_arduboy_index.json
  - ツール > ボード         = Arduboy
  - ツール > シリアルポート = Arduboy Leonardo があるポート
- スケッチをダウンロードしてください:
  - git clone https://github.com/koteitan/somefallingblockgame.git
    または
  - https://github.com/koteitan/somefallingblockgame/archive/master.zip
- "somefallingblockgame/somefallingblockgame.ino" を Arduino IDE で開いてください。
- Arduboy を USB ケーブル で PC につないで下さい。
- 「マイコンボードに書き込む」ボタンを押して下さい。
---------------------------
 ご注意
---------------------------
- このゲームのスケッチを転送した後に次のゲームのスケッチをアップロードできなくなるかもしれません。もし下記のようなエラーメッセージがでたら、下記の修復方法で修復し、次のスケッチを転送してください。
  - エラーメッセージ:
    - "Couldn't find a Board on the selected port. Check that you have the correct port selected.  If it is correct, try pressing the board's reset button after initiating the upload."
  - 修復方法(PORTS-リセット法):
    (1) ArduinoIDE > ファイル > 環境設定> 設定 > より詳細な情報を表示する: 書き込み = チェック有にします。
    (2) Arduboy のUSBケーブルの横のリセットボタンを押したままにする。(爪楊枝の先を爪切りで1mm切ったものを使うと便利です)
    (3) Arduino IDE の書き込みボタンを押します。
    (4) Arduino IDE に "PORTS {} / {} => {}" というメッセージが連続して出力される状態の時に Arduboy のリセットボタンを離します。 
---------------------------
テスト済み環境
---------------------------
- Arduboy 1.0
- Arduboy Library:
  - https://github.com/Arduboy/Arduboy/
  - revision: 3c409fefb
  - branch  : master
- Arduino IDE 1.6.10
EOF
