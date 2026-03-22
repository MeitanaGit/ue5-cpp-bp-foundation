# 【UE5】C++とBPでつくるゲーム開発の土台（4日間チャレンジ）

### 移動と基本操作
![movement](images/movement.gif)

### 攻撃・弾の挙動と切り替え
![attack](images/attack.gif)
![reflect](images/reflect.gif)

このリポジトリは、Zennの記事  
「【UE5初心者】AIのGemini活用でC++とBPを使って4日でゲーム開発の土台を作成」  
のサンプルコードです。

This repository contains sample code for building a game development foundation in Unreal Engine 5 using C++ and Blueprint.

## 概要
Geminiを活用しながら、Unreal Engine 5でアクションゲームの基礎となる機能を4日間で実装したプロジェクトです。

## 開発環境
- **Unreal Engine:** 5.7
- **IDE:** Visual Studio 2022 / JetBrains Rider (開発用)
- **言語:** C++, Blueprint

## 主な実装機能
- WASDによるキャラクター移動（Enhanced Input）
- 物理挙動を伴う飛び道具（Projectile）
- 敵の移動 / 当たり判定 / スポーン / 撃破エフェクト
- EXPオーブ取得
- スコア表示
- 反射弾の実装
- トレイル（軌跡）追加

## 使い方
1. このリポジトリをクローンまたはダウンロードします。
2. `.uproject` ファイルを右クリックし → Generate Visual Studio project files
3. 作成された `.sln` ファイルを開き、ビルドを行ってください。  
4. Unreal Engine 5でプロジェクトを開いてください。

## 関連記事
詳しい解説は、こちらのZenn記事をご覧ください。
https://zenn.dev/arianankururu/articles/6ccbc1e251c382

---
## License
© 2026 Meitana
