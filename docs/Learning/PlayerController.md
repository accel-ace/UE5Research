# PlayerController

## 概要

PlayerControllerは、プレイヤーからの入力を受け取り、Pawnへ指示を送るクラスです。

Gameplay Frameworkでは、プレイヤー自身を表すクラスとして機能し、操作対象となるPawnをPossess（所有）して制御します。

## 主な役割

* プレイヤー入力の受付
* Pawnの操作
* カメラ制御
* UIとの連携
* プレイヤー固有の処理の管理

## PlayerControllerの特徴

### 入力を受け取る

キーボード、マウス、ゲームパッドなどの入力を受け取り、対応する処理を実行します。

### Pawnを操作する

PlayerControllerはPossessしたPawnへ移動やジャンプなどの指示を送ります。

Pawn側は受け取った指示に従って実際の動作を行います。

### Pawnが変更されても利用できる

プレイヤーが乗り物へ乗ったり別のキャラクターへ切り替わった場合でも、同じPlayerControllerが新しいPawnを操作できます。

## Pawnとの関係

```text
Player
   │
   ▼
PlayerController
   │
 Possess
   │
   ▼
Pawn
```

PlayerControllerは入力を管理し、Pawnはゲーム内で実際に動作する役割を担当します。

## 今回実施した内容

* PlayerController Blueprintの作成
* Gameplay Framework検証用フォルダへの配置
* Pawnとの連携準備

## 今回学んだこと

PlayerControllerはゲーム内のキャラクターそのものではなく、プレイヤーからの入力を管理する役割であることを理解しました。

また、入力処理とゲームオブジェクトの処理を分離することで、責務を明確にできることを確認しました。

## 利用例

* キャラクター操作
* カメラ切り替え
* UI表示・非表示
* メニュー操作
* ポーズ画面の制御
* プレイヤーごとの入力設定

## 関連クラス

* Pawn
* Character
* GameMode
* PlayerState
* GameInstance

## 今後確認する内容

* Enhanced Inputとの連携
* PossessとUnPossessの動作
* Pawn切り替え時の挙動
* カメラ制御
* マルチプレイにおけるPlayerControllerの役割
* C++によるPlayerControllerの実装
