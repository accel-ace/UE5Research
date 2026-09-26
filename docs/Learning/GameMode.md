# GameMode

## 概要

GameModeは、ゲーム全体のルールや進行を管理するクラスです。

Gameplay Frameworkでは、どのPawnを生成するか、ゲーム開始時の初期化、勝敗判定など、ゲーム全体に関わる処理を担当します。

なお、GameModeはサーバー上でのみ動作し、クライアントには存在しません。

## 主な役割

* ゲームルールの管理
* ゲーム開始・終了処理
* Pawnの生成設定
* PlayerControllerの生成設定
* プレイヤー参加・離脱時の処理
* 勝敗判定

## GameModeの特徴

### ゲーム全体を管理する

ゲーム全体の進行やルールを管理します。

例

* 制限時間
* リスポーン
* 勝利条件
* 敗北条件

### Default Classを設定する

GameModeではゲーム開始時に使用するクラスを設定できます。

設定例

* Default Pawn Class
* PlayerController Class
* HUD Class
* GameState Class
* PlayerState Class

### サーバーのみ存在する

GameModeはサーバーだけが保持します。

そのため、クライアントからGameModeへ直接アクセスすることはできません。

ゲーム全体で共有する情報はGameStateへ保持します。

## Gameplay Frameworkでの位置付け

```text
GameMode
├── GameState
├── PlayerController
├── PlayerState
└── Default Pawn
```

GameModeはGameplay Framework全体の管理者として各クラスの設定やゲームルールを管理します。

## 今回実施した内容

* Gameplay Framework学習用GameMode Blueprintの作成
* Default Pawn Class設定の準備
* PlayerController設定の準備

## 今回学んだこと

GameModeはプレイヤーを操作するクラスではなく、ゲーム全体を管理する役割であることを理解しました。

また、Gameplay Frameworkでは役割ごとにクラスが分かれており、GameModeはゲーム全体の責任を持つクラスとして機能することを確認しました。

## 利用例

* プレイヤーのスポーン管理
* ゲーム開始・終了
* ラウンド管理
* 制限時間管理
* 勝敗判定
* リスポーン処理

## 関連クラス

* Pawn
* PlayerController
* GameState
* PlayerState
* GameInstance

## 今後確認する内容

* Default Pawn Classの設定方法
* PlayerControllerとの連携
* Match Stateの仕組み
* マルチプレイにおけるGameModeの動作
* GameStateとの役割の違い
* C++によるGameModeの実装
