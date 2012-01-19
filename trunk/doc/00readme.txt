                     AppRecog

概要：

コンパイル実行手順：

 - コンパイル
  > make
  >

 - 実行：
   > build/bin/AppRecogComp

 - RTCの接続：
   図はdoc/rtc_connection.png
   CameraCaptureを使用

 - 実行環境：
   Ubuntu Linux 10.04 LTS
   OpenRTM-aist 1.0.0-RELEASE C++版
   OpenCV 2.3

カメラRTC：
 キャリブレーションデータについて
 画像キャプチャーコンポーネントより、カメラパラメータを受け取る．
 カメラマトリックスはImg::TimedCameraImageに含まれる
 data.intrinsic.matrix_element[i] 
 分配係数はImg::TimedCameraImageに含まれる
 data.intrinsic.distortion_coefficient[i]
 に含まれる。

インタフェース：
 データポート
入力:9Img::TimedCameraImageによる、入力画像
出力:処理画像出力,対象物体の位置姿勢の出力

サービスポート:モデルの変更を行うことが出きる。
あらかじめ./ModelFiles/ModelList.txtに定義した、
モデルファイル名と、モデルIDを用いる。
setModelID i でi番のモデルを使用する。

認識結果の出力形式について：
認識結果はoTimedRecognitionResultによって出力する。
中身は以下のようになっている。
  0: 0, 1: 0, 2: 0, 3: 0, 4: 0
  5: 0, 6: 0, 7: 0, 
   8: R00,  9: R01, 10: R02, 11: Tx
  12: R10, 13: R11, 14: R12, 15: Ty
  16: R20, 17: R21, 18: R22, 19: Tz


モデルの指定：
 コンフィグ、サービスによるモデルの設定
 モデルは./ModelFileの中に設置する。
 モデルファイルは頂点と辺によって構成されている。
 他のモデルファイルを参照することで生成することも出きるが、
 save.cppによっても生成することができる。

動画,静止画：
 doc/rtc_connection.png
 doc/test.avi

謝辞：
 AppRecog uses SimGALib. Thanks a lot to the author, Tomas V. Arredondo.
