# UE5 Research

## Overview

Unreal Engine 5 の技術調査および、オンラインマルチプレイ機能を使用したPvPサンプル実装用リポジトリ。

BlueprintとC++の連携、Gameplay Framework、Replication、セッション管理など、UE5における基本的なマルチプレイ実装の検証を目的とする。





## 環境構築
環境構築の詳細は `docs/setup.md` を参照

## Investigation Scope

### 1. UE5基礎検証、C++連携検証

- UE5エディタの基本操作
- Actor / Component の役割確認
- BlueprintとC++の連携
- Gameplay Frameworkの調査
- Asset管理方法の調査
- Enhanced Inputを使用した入力処理
- Delegateを使用したイベント通知

### 2. オンライン通信検証

- Client / Serverモデルの調査
- Listen Serverを使用した接続確認
- RPC の実装検証
- セッションの作成・検索・参加・破棄
- 接続および切断時の挙動確認
- タイトル遷移後のセッション再作成・再参加

### 3. マルチプレイ検証

- Replication の検証
- ReplicatedUsing / OnRepを使用した値の同期の検証
- CharacterおよびPlayerStateの同期検証
- GameMode / GameState / PlayerStateの役割確認
- ホスト・クライアント間のゲーム進行同期
- Dedicated Server の調査

### 4. PvPサンプルゲーム作成

- プレイヤー移動
- カメラ操作
- 攻撃処理
- 死亡処理
- ダメージ処理
- 勝敗判定
- リザルト表示
- 対戦終了後の入力停止
- ネットワーク同期


## 実装機能

### Session

- セッションの作成
- セッションの検索
- 検索結果の一覧表示
- セッションへの参加
- セッションの破棄
- ホスト名、参加人数、Ping、ルーム名の表示
- タイトル画面へ戻った後のセッション再作成
- ホストとクライアントを入れ替えた状態での再接続

### PvP

- Listen Server環境での2プレイヤー接続
- プレイヤーの移動およびカメラ操作
- 攻撃判定
- サーバー側でのダメージ処理
- Healthの同期
- 死亡後の移動および攻撃入力の停止
- 勝敗判定
- ホスト・クライアントそれぞれへのリザルト表示

## 進捗

### Environment Setup

- [x] GitHub Repository作成
- [x] Git LFS導入
- [x] UE5 C++ Third Person Project作成
- [x] SourceTree連携

### UE5 Fundamentals

- [x] UE5基礎調査
- [x] Gameplay Framework調査
- [x] Blueprint / C++連携調査
- [x] Actor / Component構成の検証
- [x] Enhanced Inputの検証

### Online Features

- [x] オンライン通信検証
- [x] セッション管理機能の実装
- [x] マルチプレイ検証
- [x] Replication / RPCの検証
- [x] 接続・切断・再接続時の挙動確認

### Sample Development

- [x] PvPサンプルゲーム作成
- [x] 攻撃・ダメージ・死亡処理
- [x] 勝敗判定・リザルト表示
- [x] セッション再作成・再参加の確認
- [x] ホスト・クライアント入れ替え時の動作確認

## 検証内容

以下の一連の操作が正常に行えることを確認済み。

1. ホストによるセッション作成
2. クライアントによるセッション検索・参加
3. プレイヤー移動および攻撃
4. ダメージ・死亡処理
5. 勝敗判定およびリザルト表示
6. タイトル画面への遷移
7. セッションの再作成・再参加
8. ホストとクライアントを入れ替えた状態での再実行
