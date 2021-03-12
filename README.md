# 単語帳アプリ
コマンドラインで利用する単語帳アプリです。

# DEMO
テキスト形式の単語帳データ指定して起動すると、問題が出題されます。
モードは、問題を解くモード(a)、間違った問題を解くモード(w)、参照モード（v）を指摘できます。

問題を解くモードと、間違った問題を解くモードでは、1問毎に採点され、正しい回答、正解、不正解が表示されます。そして、全ての問題が終わると、表示されます。

以下は、sample.txtを指定して実行し、問題を解くモード(a)で実行したデモです。

    $ ./tango_card ./sample.txt 
    Tango Card App. Ver.0.8
    input(a/w/v/q) :a
    No. 1
    Q:りんご
    > apple
    A:apple
    Correct!!

    No. 2
    Q:バナナ
    > banana
    A:banana
    Correct!!

    No. 3
    Q:ぶどう
    > grape
    A:grape
    Correct!!

    ----------------------------------
    No. 1: o
    No. 2: o
    No. 3: o
    ----------------------------------
    Total: 3, Correct: 3, Wrong:  0

    input(a/w/v/q) :q

# Features
コマンドラインのアプリケーションでシンプルに単語帳を実現しています。

# Requirement
gcc, make のバージョンは以下の通り。

gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04) 
gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04) 

# Installation
特になし

# Usage
``` 
$ ./tango_card {単語帳のデータファイル名}
```

## 単語帳のデータファイルの形式
問題文、回答の順で記入する。#で始まる行や空行はスキップする。
下記のサンプルでは、1問目が「りんご」で回答が「apple」、2問目は「バナナ」で回答が「banana」、3問目は「ぶどう」で回答が「grape」

    # サンプル問題
    りんご
    apple

    バナナ
    banana

    ぶどう
    grape

# Note
特になし

# Author
* Toshita

# License
The source code is licensed MIT. The website content is licensed CC BY 4.0,see LICENSE.
