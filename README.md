# RobotSystem2020_task1
このリポジトリはロボットシステム学2020の課題１です。

## 改造した点
デバイスドライバで改造した点:入力した文字数に応じてLEDが点滅する。

## 準備
raspberry piのGPIO 25ピンとGNDピン間に抵抗とLEDを接続。

このリポジトリをクローンしてから下記コマンドを順に入力。
```
make
sudo insmod myled.ko
sudo chmod 666 /dev/myled0
```

## 動作方法
echoに続けて好きな言葉、半角小文字の英字で入力すると、
それに応じたモールス信号がLEDで点滅する。

例
```
echo aaaa > /dev/myled0
```
この場合4回LEDが点滅する。

