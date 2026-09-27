# Setup Guide

このドキュメントは本プロジェクトの開発環境構築手順をまとめたものです。  
本プロジェクトは macOS 環境において Metal を使用します。

## 開発環境

- Unreal Engine 5.8 (Launcher version)
- C++
- Git
- Git LFS
- macOS (Editor execution only, no packaged build)
- Xcode 26.4.1
- Metal / Command Line Tools 26.4.1 (17E188)
  
※各ツールの取得法はrefarence.mdを参照

### UE5導入
1. Epic Game Launcherをインストール
2. ライブラリタブからEngine バージョンで+ボタンを押下
3. 追加された項目からバージョンを指定してダウンロード、インストール

### Xcode導入

1. Xcode をインストール
2. Xcode Command Line Tools を有効化
```bash
sudo xcode-select -s /Applications/Xcode.app
```

### Metal導入
1. Xcode Setting>Components画面へ遷移
2. Metal Toolchain横のGetボタンを押下

## リポジトリ取得

```bash
git lfs install
git clone https://github.com/accel-ace/UE5Research.git
cd UE5Research
git lfs pull
```

### 起動方法
1. プロジェクトの基底階層でuprojectを右クリック
2. このアプリケーションで開く>その他を選択
3. 導入したUEをデフォルトに指定してプロジェクトを開く
