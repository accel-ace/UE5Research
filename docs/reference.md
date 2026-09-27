# Reference

## Unreal Engine

- Epic Games Launcher
  https://www.unrealengine.com/download

## Xcode

- Apple Developer Download（旧バージョン含む）
  https://developer.apple.com/download/all/
- 参考
  https://qiita.com/JunkiHiroi/items/ad3d34a4c996b992bf40

※ Apple IDログインが必要

## Metal shader 関連

### ビルドエラー: Metal compiler not found
- Xcodeが正しく選択されていない可能性
- `xcode-select` を確認

### シェーダーが更新されない
- DerivedDataCache削除
- エディタ再起動