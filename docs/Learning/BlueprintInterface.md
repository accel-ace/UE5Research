# Blueprint Interface

## 概要

Blueprint Interface（BPI）は、異なるBlueprint間で共通の関数を定義するための仕組みです。

Interfaceを利用することで、呼び出し元は相手の具体的なクラスを意識せずに処理を実行できます。

## 主な役割

* Blueprint間の共通インターフェースを定義する
* Actor間の疎結合な通信を実現する
* クラスへの依存を減らす
* 共通機能を複数のActorへ提供する

## 今回検証した内容

* Blueprint Interfaceの作成
* Interface関数の定義
* ActorへのInterface実装
* Interface Messageによる関数呼び出し
* ActorComponentと組み合わせた処理の実行

## Blueprint Interfaceの特徴

### 実装を持たない

Blueprint Interfaceでは関数の定義のみを行います。
実際の処理はInterfaceを実装したBlueprint側で記述します。

### 疎結合な設計ができる

呼び出し元は対象のクラス名を意識する必要がありません。
Interfaceを実装していれば同じ関数を呼び出せます。

### 共通の窓口を提供できる

複数のActorが同じInterfaceを実装することで、呼び出し側は同じ方法で処理を実行できます。

## 今回学んだこと

Interfaceを利用することで、特定のBlueprintへ依存しないActor間通信を実現できることを確認しました。

また、ActorComponentと組み合わせることで、ActorはInterfaceを受け付ける窓口となり、実際の処理はComponentへ委譲する構成を作成しました。

## 利用例

* スイッチによるギミック起動
* ドアの開閉
* ダメージ処理
* NPCとの会話開始
* アイテム取得
* インタラクト処理

## 利点

* Castの利用を減らせる
* Blueprint同士の依存関係を小さくできる
* 機能追加時の修正範囲を抑えられる
* 保守性・再利用性を向上できる

## 関連するクラス

* Actor
* ActorComponent
* Pawn
* PlayerController

## 今後確認する内容

* Interfaceと継承の使い分け
* InterfaceとEvent Dispatcherの違い
* C++によるInterface実装
* Gameplay Frameworkでの活用方法
