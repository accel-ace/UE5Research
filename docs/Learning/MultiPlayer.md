# Multiplayer

## 概要

Multiplayerは、複数のプレイヤーが同じゲーム空間で遊ぶための仕組みです。

Unreal Engineでは、サーバーとクライアントの役割を分け、必要な情報を同期することでマルチプレイを実現します。

## 主な目的

* 複数プレイヤーの同時参加
* プレイヤー位置や状態の同期
* ゲーム進行状況の共有
* サーバーによるゲームルール管理

## 基本的な考え方

### サーバー

ゲームの正しい状態を管理する側です。

主に以下を担当します。

* ゲームルールの管理
* プレイヤー参加・退出
* スコア管理
* 当たり判定
* 重要な状態変更

### クライアント

各プレイヤーの操作端末です。

主に以下を担当します。

* 入力の送信
* 画面表示
* UI操作
* サーバーから受け取った状態の反映

## Gameplay Frameworkとの関係

```text
Server
 ├── GameMode
 ├── GameState
 ├── PlayerController
 ├── PlayerState
 └── Pawn

Client
 ├── GameState
 ├── PlayerController
 ├── PlayerState
 └── Pawn
```

GameModeはサーバー側にのみ存在します。
GameStateやPlayerStateは、サーバーからクライアントへ同期される情報を保持します。

## 主な用語

### Replication

サーバー上のActorや変数の状態をクライアントへ同期する仕組みです。

### RPC

Remote Procedure Callの略です。
サーバーやクライアントに対して、ネットワーク越しに関数を実行する仕組みです。

### Authority

そのActorや処理に対する正しい管理権限を持つ側を指します。
基本的にはサーバーがAuthorityを持ちます。

### Listen Server

プレイヤーの1人がサーバーも兼ねる方式です。

### Dedicated Server

専用のサーバーを用意し、クライアントとは別にゲーム状態を管理する方式です。

## 今回実施した内容

* Multiplayer学習用ドキュメントの作成
* Gameplay FrameworkとMultiplayerの関係を整理

※実装は今後実施予定です。

## 今回学んだこと

マルチプレイでは、すべての端末が自由に状態を変更するのではなく、サーバーを中心に正しい状態を管理することが重要であると理解しました。

また、GameMode、GameState、PlayerStateなどのGameplay Frameworkのクラスが、マルチプレイを前提とした責務分離になっていることを確認しました。

## 今後確認する内容

* 複数クライアントでの起動確認
* Replicationの基本
* 変数の同期
* Actorの同期
* RPCの基本
* Server RPC / Client RPC / Multicast RPC
* Pawn移動の同期
* スコアの同期
* Listen ServerとDedicated Serverの違い
* Online Subsystemの調査
* Steam連携の調査
