# RobotSystem2020_task1
このリポジトリはロボットシステム学2020の課題１です。

## リポジトリの概要
入力した文字数に応じてLEDが点滅する。

## 動作環境および部品
### 環境
*OS:ubuntu18.04

### 部品リスト
*RaspberryPi4 ModelB 4GB<br>
*RaspberryPi用電源アダプタ(TypeC)<br>
*ブレッドボード<br>
*LED　１個<br>
*抵抗100Ω　１個<br>
*ジャンパワイヤ　２本<br>


## インストール方法

このリポジトリをクローンしてから下記コマンドを順に入力。
```
make
sudo insmod myled.ko
sudo chmod 666 /dev/myled0
```

## 使用方法
例
```
echo aaaa > /dev/myled0
```
この場合エンター含め5回LEDが点滅する。

