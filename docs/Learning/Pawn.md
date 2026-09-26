# Pawn

## 概要

Pawnは、プレイヤーやAIが操作するゲーム内オブジェクトの基底クラスです。

Gameplay Frameworkでは、PlayerControllerから入力や指示を受け取り、ワールド上で移動やアクションなどの動作を行います。

## 継承関係

```text
Actor
 └─ Pawn
     └─ Character
```

PawnはActorを継承しており、CharacterはPawnを継承しています。

## 主な役割

* プレイヤーまたはAIに操作されるオブジェクト
* ワールド上で移動・回転などを行う
* Componentを保持する
* PlayerControllerやAIControllerから操作される

## Pawnの特徴

### ワールドへ配置できる

PawnはActorと同様にレベルへ配置できます。

### 操作対象となる

PlayerControllerがPossess（所有）することで、プレイヤーから操作できるようになります。

### Componentを追加できる

Actorと同様にComponentを追加し、機能を拡張できます。

## Characterとの違い

### Pawn

最低限の操作対象となるクラスです。

利用例

* 車
* 飛行機
* 戦車
* ドローン
* ボール

### Character

人型キャラクター向けのPawnです。

標準で以下の機能を持っています。

* Character Movement Component
* Capsule Component
* Skeletal Mesh

歩行・ジャンプなどを簡単に実装できます。

## 今回実施した内容

* Pawn Blueprintの作成
* Gameplay Framework検証用フォルダの作成
* 今後の入力処理実装に向けた準備

## 今回学んだこと

Pawnは「プレイヤーそのもの」ではなく、「プレイヤーやAIが操作する対象」であることを理解しました。

また、入力を直接受け取る役割ではなく、PlayerControllerから受け取った指示を実行する役割を担うことを確認しました。

## 関連クラス

* Actor
* Character
* PlayerController
* AIController
* GameMode

## 今後確認する内容

* PlayerControllerからPawnへの入力処理
* Possessの仕組み
* Default Pawn Classの設定
* Auto Possess Playerの動作
* PawnへのCamera・SpringArm追加
* C++によるPawnの実装
