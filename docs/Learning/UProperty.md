# UPROPERTY Specifier メモ

## 基本

| Specifier | 意味 | 用途 |
|---|---|---|
| `EditAnywhere` | どこでも編集可能 | エディタから値を変更する |
| `EditDefaultsOnly` | デフォルト値のみ編集可能 | Blueprintクラスの初期値設定 |
| `EditInstanceOnly` | 配置したインスタンスのみ編集可能 | レベル配置ごとに値を変える |
| `VisibleAnywhere` | 表示のみ（編集不可） | 状態確認用 |
| `VisibleDefaultsOnly` | デフォルト値のみ表示 | Blueprintクラス確認用 |
| `VisibleInstanceOnly` | インスタンスのみ表示 | 配置Actor確認用 |

---

## Blueprint公開

| Specifier | 意味 | 用途 |
|---|---|---|
| `BlueprintReadOnly` | Blueprintから読み取りのみ可能 | Getter用途 |
| `BlueprintReadWrite` | Blueprintから読み書き可能 | 一般的な変数 |
| `BlueprintGetter=関数名` | Getter関数経由で取得 | アクセス制御 |
| `BlueprintSetter=関数名` | Setter関数経由で更新 | 値変更時に処理を行う |

---

## 通信（Replication）

| Specifier | 意味 | 用途 |
|---|---|---|
| `Replicated` | ネットワーク同期する | 通常の同期変数 |
| `ReplicatedUsing=関数名` | 同期時にOnRep関数を呼ぶ | UI更新・演出など |

---

## 保存・設定
| Specifier | 意味 | 用途 |
|---|---|---|
| `Config` | Configファイルから読み込む | ゲーム設定 |
| `GlobalConfig` | 全体Configを使用 | エンジン設定 |
| `SaveGame` | セーブデータ対象 | セーブ・ロード |

---

## エディタ表示

| Specifier | 意味 | 用途 |
|---|---|---|
| `Category="..."` | Detailsパネルのカテゴリ | 整理 |
| `DisplayName="..."` | 表示名変更 | 見やすさ向上 |
| `ToolTip="..."` | ツールチップ表示 | 説明追加 |

※ DisplayName と ToolTip は通常 `meta=(...)` 内で指定する。

---

## メタデータ（meta）

| Specifier | 意味 | 用途 |
|---|---|---|
| `ClampMin` | 最小値制限 | 数値入力 |
| `ClampMax` | 最大値制限 | 数値入力 |
| `UIMin` | スライダー最小値 | エディタ表示 |
| `UIMax` | スライダー最大値 | エディタ表示 |
| `DisplayName` | 表示名変更 | エディタ表示 |
| `ToolTip` | 説明表示 | エディタ表示 |

例

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Status",
    meta=(ClampMin="0", ClampMax="100")
)
int32 Health;
```

---

## UObject参照

| Specifier | 意味 | 用途 |
|---|---|---|
| `Instanced` | インスタンスとして保持 | Component等 |
| `Transient` | 保存対象外 | 一時データ |
| `DuplicateTransient` | コピー時に複製しない | 一時情報 |

---

# 読み方

`UPROPERTY(...)` は、

**変数をUEがどう扱うか設定するためのマクロ**

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Player",
    Replicated
)
int32 Health = 100;
```

これは以下を意味する。

| 要素 | 意味 |
|---|---|
| `EditAnywhere` | エディタで編集可能 |
| `BlueprintReadWrite` | Blueprintから読み書き可能 |
| `Category="Player"` | Playerカテゴリへ表示 |
| `Replicated` | ネットワーク同期する |

---

# 注意点

Specifierは複数指定できる。

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Replicated,
    Category="Player"
)
```

のように組み合わせて使用する。

ただし、

```cpp
VisibleAnywhere
```

と

```cpp
EditAnywhere
```

のように、

**表示専用**と**編集可能**を同時指定するなど、意味が矛盾する組み合わせはできない。

---

# 覚える優先順位

★★★★★

- EditAnywhere
- BlueprintReadWrite
- BlueprintReadOnly
- Category
- Replicated
- ReplicatedUsing

★★★★☆

- EditDefaultsOnly
- VisibleAnywhere
- SaveGame

★★★☆☆

- meta
- Config
- Instanced
- Transient

★★☆☆☆

- その他のSpecifier（必要になったら調べる）

## よく使う組み合わせ

```

// エディタで編集 + Blueprintから利用
UPROPERTY(EditAnywhere, BlueprintReadWrite)

// 通信同期
UPROPERTY(Replicated)

// RepNotify
UPROPERTY(ReplicatedUsing=OnRep_Health)

// セーブ対象
UPROPERTY(EditAnywhere, SaveGame)

// エディタ表示のみ
UPROPERTY(VisibleAnywhere, BlueprintReadOnly)

```