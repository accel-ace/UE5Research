# UFUNCTION Specifier メモ

## 基本

| Specifier | 意味 | 用途 |
|---|---|---|
| `BlueprintCallable` | Blueprintから呼び出せる | 通常の関数公開 |
| `BlueprintPure` | Blueprintで実行ピンなしの関数にする | Getter、計算、状態変更しない処理 |
| `Category="..."` | Blueprint上のカテゴリを指定する | ノード整理 |
| `meta=(...)` | 表示名や補足情報を指定する | DisplayName、ToolTipなど |

## Blueprint連携

| Specifier | 意味 | 用途 |
|---|---|---|
| `BlueprintImplementableEvent` | Blueprint側で実装するイベント | C++からBPへ処理を委譲 |
| `BlueprintNativeEvent` | C++実装を持ちつつBlueprintで上書き可能 | デフォルト処理＋BP拡張 |
| `BlueprintAuthorityOnly` | Authority側でのみ実行されるBlueprint関数 | サーバー権限が必要な処理 |
| `BlueprintCosmetic` | 見た目用処理として扱う | Dedicated Serverで不要な演出処理 |

## RPC / 通信

| Specifier | 意味 | Blueprintでの対応 |
|---|---|---|
| `Server` | Server RPC | Run on Server |
| `Client` | Client RPC | Run on Owning Client |
| `NetMulticast` | 全Clientへ通知するRPC | Multicast |
| `Reliable` | RPCを確実に届ける | Reliable |
| `Unreliable` | RPCが落ちても許容する | Unreliable |

## エディタ・デバッグ系

| Specifier | 意味 | 用途 |
|---|---|---|
| `CallInEditor` | Detailsパネルからボタン実行できる | エディタ用ツール |
| `Exec` | コンソールコマンドから呼び出せる | デバッグ、開発用コマンド |

## 非推奨化

| Specifier | 意味 | 用途 |
|---|---|---|
| `DeprecatedFunction` | 非推奨関数として警告を出す | 古いAPIの移行 |
| `DeprecationMessage="..."` | 非推奨時の警告メッセージ | 代替関数の案内 |

## 読み方

`UFUNCTION(...)` は、関数に対してUEがどう扱うかを設定するためのマクロ。

```cpp
UFUNCTION(BlueprintCallable, Server, Reliable, Category="Network")
void Server_AddScore();