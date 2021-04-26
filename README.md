# ImmatureShell

## Image
<img width="1280" alt="スクリーンショット 2021-04-26 15 07 11" src="https://user-images.githubusercontent.com/60251958/116036241-591dc200-a6a1-11eb-94be-ab0646dda15b.png">

## Overview
C言語で自作shell開発
自作でshellを作ってみたいと思っている方の参考になればと思い作成させて頂きました。

## Features
- コマンド実行(実装済)
- ビルトインコマンド(実装済)
- シグナル(実装済)
- パイプ(未実装)
- リダイレクト(未実装)

## Requirement
- gcc
- x86_64

## How to install
```shell
git https://github.com/fumiya5863/ImmatureShell.git
```

## How to use

```shell
make
./main
```

## Folder structure
```
- common/
    - constants.h
    - functions.c
    - functions.h
- Makefile
- built_in_command.c
- built_in_command.h
- main.c
```

## Author
[fumiya5863](https://github.com/fumiya5863)

## License
The source code is licensed MIT.
