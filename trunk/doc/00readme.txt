                     AppRecog

概要：
  モデルとして与えられたエッジ情報を手がかりとして，二次元的に対象物の
  位置と姿勢，スケールを推定するモジュールです．

コンパイル実行手順：

 - コンパイル
  > make

 - 実行：
  > build/bin/AppRecogComp

 - RTCの接続：
   doc/rtc_connection.pngの図を参照してください．

 - 実行環境：
   Ubuntu Linux 10.04 LTS
   OpenRTM-aist 1.0.0-RELEASE C++版
   OpenCV 2.3


インタフェース：
 データポート
  入力: Img::TimedCameraImage (Img.idl)
      画像出力共通インタフェース準拠のカメラモジュールから，
      画像及び，カメラパラメータを受取ります．
  出力: TimedRecognitionResult (Vision.idl)
      認識結果共通インタフェースにしたがい，対象物体の位置姿勢を出力します．
      Img::TimedCameraImage 処理結果を画像として出力します．

 サービスポート
      認識対象のモデルを設定するために使います．
      あらかじめ．ModelFiles/ModelList.txtにモデルIDとモデル定義ファイ
      ル名を記述し，モデルIDを引数としてサービスコールを行います．
      setModelID(i)は，i番のモデルを使用することを意味します．

認識結果の出力形式について：
 認識結果はTimedRecognitionResultによって出力されます．
 具体的な出力内容は以下の通りです．現在，対象物の姿勢以外は入っていま
 せん．
  0: 0, 1: 0, 2: 0, 3: 0, 4: 0
  5: 0, 6: 0, 7: 0, 
  8: R00,  9: R01, 10: R02, 11: Tx
 12: R10, 13: R11, 14: R12, 15: Ty
 16: R20, 17: R21, 18: R22, 19: Tz

モデルについて：
 コンフィグあるいはサービスによりモデルの設定を行います．
 モデル定義ファイルは./ModelFileの中に置きます．
 ファイルは頂点と辺によって構成されています．

サンプル動画：
 doc/test.avi

謝辞：
 AppRecog uses SimGALib. Thanks a lot to the author, Tomas V. Arredondo.
