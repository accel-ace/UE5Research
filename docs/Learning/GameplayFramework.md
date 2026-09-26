# Gameplay Framework

## 概要

Gameplay Frameworkは、Unreal Engineでゲーム全体の処理を構成するための基本的なフレームワークです。

プレイヤー操作、ゲームルール、プレイヤー情報、ゲーム全体の状態などを役割ごとにクラスへ分割して管理します。

## 主な目的

* ゲーム全体の役割を明確にする
* 責務ごとにクラスを分離する
* シングルプレイ・マルチプレイの両方へ対応しやすい設計を提供する

## 主なクラス

### Pawn

プレイヤーやAIが操作する対象。

**役割**

* ワールド上へ存在する
* 移動・ジャンプなどの動作
* Componentを保持する

---

### PlayerController

入力や操作を管理し、PawnをPossessする。

**役割**

* 入力処理
* カメラ制御
* UIとの連携

---

### GameMode

ゲームルールを管理する。基本的にサーバー側でのみ存在する。

**役割**

* ゲーム開始処理
* Pawn生成
* 勝敗判定
* リスポーン管理

---

### GameState

GameStateは試合全体の共有状態を管理する。
PlayerArrayを通じて、参加中プレイヤーのPlayerState一覧を参照できる。

**例**

* 制限時間
* スコア
* ラウンド情報

マルチプレイではクライアントへ同期されます。

---

### PlayerState

PlayerStateは各プレイヤーごとの共有情報を持つ。
Scoreなど、全クライアントに見せたい個人情報はPlayerStateに置く。
Clientから値を直接変更せず、Server RPC経由でServer側の値を更新する。
更新されたPlayerStateの値は各ClientへReplicateされる。

**例**

* プレイヤー名
* スコア
* 所持ポイント

Pawnが変更されても情報を保持できます。

---

### GameInstance

ゲーム起動中を通して保持されるオブジェクトです。

**例**

* 設定情報
* セーブデータ
* タイトル画面からゲーム画面への情報受け渡し

レベル遷移後も破棄されません。

---

### Multiplayerでの関係
GameModeはサーバー専用。
GameStateとPlayerStateはクライアントにも共有される。
Pawnは操作対象として同期対象になる。

---

### Replicateについて

RepNotifyは、Server側で変更されたReplicated変数がClientへ同期された際に呼ばれる通知処理。
UI更新や演出再生など、値の同期後にClient側で処理を行いたい場合に利用する。
Server側ではOnRepが自動実行されないため、Server側でも同様の処理が必要な場合は、値変更後に更新処理を直接呼び出す

### Authorityについて

Authorityは、そのActorの正しい状態を管理する側を示す。基本的にServerがAuthorityを持つ。
Client側で自分が操作するPawnはAutonomousProxyとなる。
他ClientのPawnはSimulatedProxyとして扱われる。
LocalRoleは現在実行中の環境におけるActorのRoleを示し、RemoteRoleは接続先から見たRoleを示す。

---

## クラス同士の関係

```text
GameInstance
        │
        ▼
GameMode
 ├── GameState
 ├── PlayerController
 │      │
 │      ▼
 │    Pawn
 │
 └── PlayerState
```

## 今回実施した内容

* Gameplay Framework検証用レベルの作成
* PlayerStartの配置
* Pawn Blueprintの作成
* PlayerController Blueprintの作成
* 学習環境の構築

## 今回学んだこと

Gameplay Frameworkでは、ゲーム全体を1つのクラスへまとめるのではなく、それぞれの責務に応じてクラスを分割する考え方を採用していることを確認しました。

