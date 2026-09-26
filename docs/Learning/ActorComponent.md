# ActorComponent

## 概要

ActorComponentは、Actorへ機能を追加するためのコンポーネントです。
Actor本体から処理を分離することで、責務を明確にし、再利用しやすい設計を実現できます。

## 主な役割

* Actorへ機能を追加する
* 処理を分離する
* 複数のActorで共通機能を再利用する
* Actorの責務を小さく保つ

## 今回検証した内容

* Blueprint ActorComponentの作成
* Actorへの追加方法
* ActorからComponentの取得
* Component内で回転処理を実装
* Blueprint Interfaceと組み合わせた処理の呼び出し

## ActorComponentの特徴

### レベルへ配置できない

ActorComponent単体ではレベルへ配置できません。
必ずActorへ追加して利用します。

### 再利用しやすい

同じComponentを複数のActorへ追加できるため、共通機能をまとめられます。

### 責務を分離できる

Actorへすべての処理を書くのではなく、機能ごとにComponentへ分離できます。
その結果、保守性や可読性の向上が期待できます。

## 今回学んだこと

回転処理をActorComponentへ分離することで、Actor本体には必要最低限の処理のみを記述できることを確認しました。

また、Blueprint Interfaceと組み合わせることで、Actorは処理の詳細を意識せず、Component側へ処理を委譲できる構成を実現できました。

## 利用例

* 回転処理
* 体力管理
* ダメージ処理
* インタラクト処理
* インベントリ管理
* AI共通処理

## 関連するクラス

* Actor
* SceneComponent
* PrimitiveComponent
* Blueprint Interface

## 今後確認する内容

* SceneComponentとの違い
* PrimitiveComponentとの違い
* C++によるActorComponentの実装
* Component間の連携方法
