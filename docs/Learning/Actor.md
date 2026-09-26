# Actor

## 概要

ActorはUnreal Engineにおけるゲームオブジェクトの基本単位です。
レベル上へ配置できるオブジェクトは基本的にActor、またはActorを継承したクラスになります。

## 主な役割

* ゲーム内オブジェクトを表現する
* ワールドへ配置する
* Componentを保持する
* イベントや処理を実行する

## 今回検証した内容

* Blueprint Actorの作成
* レベルへの配置
* 変数の公開および編集
* 関数の作成・呼び出し
* Event BeginPlayおよびEvent Tickの動作確認
* ActorComponentの追加
* Blueprint Interfaceを利用したActor間通信

## Actorの特徴

### レベルへ配置できる

Actorはレベル上へ直接配置できます。

### Componentを持てる

Actorは複数のComponentを保持し、機能を追加できます。
処理をActorComponentへ分離することで再利用性を高められます。

### 継承できる

BlueprintやC++から独自のActorを作成できます。
共通処理を親クラスへまとめることで実装を共通化できます。

## 今回学んだこと

Actorは単純にオブジェクトを配置するだけではなく、ゲーム内の様々な処理の土台となるクラスであることを理解しました。

また、処理をActorへ直接記述するのではなく、ActorComponentやBlueprint Interfaceを組み合わせることで、責務を分離した設計が可能になることを確認しました。

## 関連するクラス

* ActorComponent
* SceneComponent
* Pawn
* Character
* PlayerController
* GameMode
* GameState

## 今後確認する内容

* Pawnとの役割の違い
* Characterとの違い
* Gameplay FrameworkにおけるActorの位置付け
* C++からActorを実装する方法
