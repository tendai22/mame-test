## mameのビルド

新しいマシンでWSLを入れるところから記録をとった。

### wslを入れる

* まず、管理者モードでPowerShellを起動して、`wsl.exe --install`とたたく。
* すぐだんまりになる。
* BIOSで、VT-d を enable にした。
* これで `wsl --install`で入る。
* 再起動してログインすると、Terminalに wsl2が起動した。
* Ubuntu 24.04が入った。

### VScodeを入れる

* Windowsにインストール: PowerShellから`winget install -e -id MicrosoftVisualStudioCode`で入る。
* apt -y update
* apt -y upgrade
* Windows側でVScodeを起動し、"Remote Development" 拡張機能をインストール
* Linux側に`code`をインストール:  
  `sudo snap install code`ではエラーが出る。
* `sudo snap install code --classic`でインストール。
* `code .`でVScodeが起動する。
* 日本語化: `Japanese Language Extensions`をインストール、再起動。
* 白モード: Ctrl-Shift-pでコマンドプロンプトを出して`light`とたたくと白黒反転コマンドが選べる。

### 旧マシンで Ubuntu 24.04を入れた時の話

* Ubuntu-24.04を入れた。
* 別環境、ホームフォルダも新規
* apt -y update
* apt -y upgrade
* gitの設定
* git のインストール(apt install git)

### git-credential-managerのインストール

dotnet-sdk, aspnetcore-runtime をインストールするが、Ubuntu-24.04だと dotnet-sdk-8.0が入ってしまう。

git-credential-managerは、dotnet-sdk-7.0が必要。

### dotnet-dsk-7.0のインストール

[Ubuntu の .NET バックポート パッケージ リポジトリ](https://learn.microsoft.com/ja-jp/dotnet/core/install/linux-ubuntu#ubuntu-net-backports-package-repository)を見て入れる。

Ubuntu の .NET バックポート パッケージ リポジトリを登録する

```
$ sudo add-apt-repository ppa:dotnet/backports
```

Microsoft パッケージ リポジトリを登録する

packages-microsoft-prod.debをダウンロード、インストールする。そして`sudo apt update`

```
# Get OS version info which adds the $ID and $VERSION_ID variables
source /etc/os-release

# Download Microsoft signing key and repository
wget https://packages.microsoft.com/config/$ID/$VERSION_ID/packages-microsoft-prod.deb -O packages-microsoft-prod.deb

# Install Microsoft signing key and repository
sudo dpkg -i packages-microsoft-prod.deb

# Clean up
rm packages-microsoft-prod.deb

# Update packages
sudo apt update
```

これで普通にdotnet-sdk-7.0がインストールできる。

```
# 必要ならアンインストールする。
$ sudo apt-get remove dotnet-sdk-8.0

$ sudo apt upgrade dotnet-sdk-7.0
..
..(わちゃわちゃ入る)
..
$ sudo apt install aspnetcore-runtime-7.0
(既に入っているらしい)

$ dotnet --list-sdks
7.0.119 [/usr/lib/dotnet/sdk]
$ dotnet --list-runtimes
Microsoft.AspNetCore.App 7.0.19 [/usr/lib/dotnet/shared/Microsoft.AspNetCore.App]
Microsoft.NETCore.App 7.0.19 [/usr/lib/dotnet/shared/Microsoft.NETCore.App]
```

### git-credential-manager のインストール・設定

> dotnet-7.0 が必要と聞いていたが、2024-10-6やってみたら、net8.0が必要とおっしゃられた。

> Package git-credential-manager 2.6.0 supports: net8.0 (.NETCoreApp,Version=v8.0) / any

ということで、net7.0 をuninstallしてnet8.0を入れ直した。

```
$ sudo apt-get remove dotnet-sdk-7.0
$ sudo apt-get install dotnet-sdk-8.0
$ sudo apt install aspnetcore-runtime-8.0
$ sudo apt autoremove
```

```
$ dotnet tool install -g git-credential-manager
# インストールされているかのチェック
# .bash_profileにPATHを追加しろと言われた。
$ cat .bash_profile
# Add .NET Core SDK tools
export PATH="$PATH:/home/kuma/.dotnet/tools"
$ which git-credential-manager
# 初期設定 (.gitconfigに書き込まれる）
$ git-credential-manager configure
```

device/personal access tokenしか選択肢が出てこなかった。結局personal access tokenを使った。

LAURELEYでのインストールでは何も出ない。大丈夫か？とりあえず先に進む。

```
kuma@LAURELEY:~$ which git-credential-manager
/home/kuma/.dotnet/tools/git-credential-manager
kuma@LAURELEY:~$ git-credential-manager configure
Configuring component 'Git Credential Manager'...
Configuring component 'Azure Repos provider'...
kuma@LAURELEY:~$
```

## mame を動かす。

* gcc-12を入れた。(gcc-11以上が必要)
* make を入れた。
* sudo ln -s /usr/bin/gcc-12 /usr/bin/gcc

## gcc-12を入れる。

```
$ sudo apt-get install gcc-12
```

## 必要なツールを入れる。

[mameのビルドページ](https://docs.mamedev.org/initialsetup/compilingmame.html)
のDebian/Ubuntuの項目

```
$ sudo apt-get install git build-essential python3 libsdl2-dev libsdl2-ttf-dev libfontconfig-dev libpulse-dev qtbase5-dev qtbase5-dev-tools qtchooser qt5-qmake
```

GCC13-2が入るので、最初のgcc-12インストールは不要だったようだ。

### mameソースコード

```
$ git clone https://github.com/mamedev/mame
```

どうやら make するだけらしい。

```
$ make -j3
```

これでビルドが通ってしまった。ワーニング山盛りで、かつ1時間ぐらいかかったような気もしますが。

# shrunk版を作る

rc2014があることに気づいた。これ中心で縮小版を作る。

メモリとシリアルだけあれば十分。それ以外は片っ端から消してゆく。

とはいえ、devisesの下は通せるものは通す。


## video, audioを消す。

* 一気に消してしまった。
* フォルダを消すだけでは不十分で luaスクリプトの中のエントリもばっさばっさ消した。

## ビルド開始

* `#include "sound/xxxx.h"`でエラーがでる。
* コメントアウトすると、xxxx.hで定義されているクラスの参照でエラー -> 全部コメントアウト
* 

# 新マシンでgit-credential-helperが動かない。

ヘルパは起動するが、ブラウザが起動しない。

```
xdg-open https://github.com/tendai22/
```

で起動するようにしないといけない。ブラウザはWindows版exeを起動する。

xdgのブラウザ定義は、ファイル`~/.local/share/applications/chrome.desktop`を作り、以下の内容を入れておく。

```
[Desktop Entry]
Encoding=UTF-8
Version=1.0
Type=Application
NoDisplay=true
Exec=/mnt/c/Program Files/Google/Chrome/Application/chrome.exe
Name=Chrome
Comment=Microsoft Chrome
MimeType=x-scheme-handler/unknown;x-scheme-handler/about;x-scheme-handler/https;x-scheme-handler/http;text/html;
```

これでも`xdg-open https://github.com/tendai22/`で起動しない。

`Exec`の値に空白が含まれると起動しない。

`xdg-open`はシェルスクリプトなのでデバッグした。コマンドパスに空白が含まれていると、ブラウザの第1引数に`Files/Google/Chrome/Application/chrome.exee`がわたってしまう。

xdg-openを改造して「第１引数が`Files`で始まるときは無理やりshiftする」としてブラウザが起動するようにした。ブラウザでログインできていれば問題なく認証が通り、`git push origin main`が成功するようになった。

> PATHに`/mnt/c/Program Files`を通しておいて、エントリに`Google/Chrome/Application/chrome.exe`を登録しておく手もあるかもしれない。試していないが。

ということで、

* git-credential-helper をインストールする。

ソースを入れてビルドする方法と、debファイルをダウンロードして入れる方法とあるようだ。今回はdebファイルをインストールしたが。

## WSL環境にブラウザを起動するシェルスクリプトを置けばいいんじゃね？

https://qiita.com/Ryusuke-Kawasaki/items/3ca0e9674ec41238ab8e
にそんなことが書いてあるような気がする。

```
~/bin/chrome.sh
#! /bin/sh
exec "/mnt/c/Program Files/Google/Chrome/Application/chrome.exe" "$@"
```

とシェルスクリプトを作成し、

```
[Desktop Entry]
Encoding=UTF-8
Version=1.0
Type=Application
NoDisplay=true
Exec=/home/kuma/bin/chrome.sh
Name=Chrome
Comment=Microsoft Chrome
MimeType=x-scheme-handler/unknown;x-scheme-handler/about;x-scheme-handler/https;x-scheme-handler/http;text/html;
```

でいけるわ。よかった。

> `xdg-open` も元のものに戻した。

## ビルド再開

## homebrew/*.cpp:

#include video|sound/を含むcppファイルを消した。sbc6809などは残して、ボード内のsoundチップを消すことにした。

z80clockに残っている。z80clock.cppも消していいかもしれない。

linux4004.cppを消した。

machine/6522via.hを復活させた。

このように、

* src/mame/homebrew/ 下でコンパイルできないものを消す(video, sound下のヘッダをインクルードしているcppファイル)
* src/devices/machine/ 下の不足物を足してゆく

を繰り返して進めている。src/devices/machine 下を全部復活させようかとも思ったが、SCSI関連などもあるので、そこまでしていない。

## *.lh ファイル

`src/mame/layout/*.lay` が変換され `build/genated/mame/layout/*.lh` になる。

`src/mame/layout` に `z80dev.lay` を置いておけばよい。

## a1bus, a2bus を消した。

Apple II busだそうだ。

## src/lib/formats の下を掃除した。

使いそうにない *.h, *.cppを全部消した。

## src/lib/formats/all.cpp を通す

この辺バッサリ削り落とした。mfi_dsk.h, dfi_dsk.h をコピーしたが駄目だった。
genieからやり直せばいけると思ったが行けなかった。

とにかく以下をコメントアウトしてall.cppを通した。

```
//	en.add(FLOPPY_MFI_FORMAT); // mfi_dsk.h
//	en.add(FLOPPY_DFI_FORMAT); // dfi_dsk.h
#ifdef HAS_FORMATS_FS_FAT
	en.add(fs::PC_FAT);
#endif

	en.category("Container FM/MFM");
//	en.add(FLOPPY_HFE_FORMAT); // hxchfe_dsk.h
//	en.add(FLOPPY_MFM_FORMAT); // hxcmfm_dsk.h
//	en.add(FLOPPY_TD0_FORMAT); // td0_dsk.h
//	en.add(FLOPPY_IMD_FORMAT); // imd_dsk.h

	en.category("Container MFM");
//	en.add(FLOPPY_D88_FORMAT); // d88_dsk.h
//	en.add(FLOPPY_CQM_FORMAT); // cqm_dsk.h
//	en.add(FLOPPY_DSK_FORMAT); // dsk_dsk.h
```

## genie.lua に記載がある。

```
generate_has_header("CPUS", CPUS)
generate_has_header("SOUNDS", SOUNDS)
generate_has_header("MACHINES", MACHINES)
generate_has_header("VIDEOS", VIDEOS)
generate_has_header("BUSES", BUSES)
generate_has_header("FORMATS", FORMATS)
```

これらについては、各フォルダの中を覗いて`.h`ファイルを探して、`build/generated/has_formats.h`ファイルを生成`HAS_XXXX`マクロを定義しているようだ。

```
#define HAS_FORMATS_FS_FAT
#define HAS_FORMATS_KIM1_CAS
#define HAS_FORMATS_MDOS_DSK
#define HAS_FORMATS_MSX_DSK
#define HAS_FORMATS_NASCOM_DSK
#define HAS_FORMATS_OS9_DSK
#define HAS_FORMATS_WD177X_DSK
```

all.cpp 中で、マクロ `HAS_XXXX`を参照してifdefコンパイルしているので、各フォルダのヘッダを消すことでシュリンクを進めるべき。

## コンパイル継続

* COPYINGがない。COPYINGをコピーした。
* mpu401.hがない。src/devices/machine/mpu401.*をコピーした。
* src/devices/bus/cbus/pc9801_118.h: sound/ymopn.h。
  cbusを外す。
* scripts/src/bus.luaからエントリ相当削除した。マクロ定義で無効かできるようなのだが、その定義をどのように渡しているのかがわかっていない。luaよく知らない。
* imagedev/floppy.hのビルドが通らない。#include "sound/samples.h"をコメントアウトしたかららしい。
* floppy_sound_device クラス定義を削除して進めた。
* machine/intelfsh.\*, ds1302.\*, コピーした。

## rc2014/sound.cppどうする？

これを使わないようにする。bus.luaからsound.cpp/.hを外す。makeたたくだけでmakefile再構成される(REGENIE=1が効いているのかな?)。

## bus/rs232c, bus/s100 からデバイスを外す。

bus.luaの両エントリから files 中の使わなさそうなエントリを消していった。

## machine/diablo_hd.\*

diablo_hd.h/cppファイルを復活させた。

## machine/下のファイル

エラーが出るたびにコピーしていった。

```
pla.h, cammu.h, gt913_kbd.h, vic_pl192.h, diablo_hd.h
```

## cpu/h6280/h6280.h が sound/c6280.h を求めている

これはsound/c6280.hを外してコンパイルを通す。ソースを編集し関連クラス定義を消した。

## cpu/h8/swx00.h

Yamaha sound generator swx00だそうなので、swx00.h/cppを外す。

cpu.luaの swx00.h/cppエントリを消した。

## cpu/m6502/gew7.cpp

Yamaha GEW7, GEW7I, GEW7S (65c02-based)サウンドデバイスだそうなので、これもcpu.luaから消す。

## cpu/m6502/rp2a03.cpp

6502, NES variant、もったいないが多分使わないので外す。

## cpu/mips/mips3.cpp:

video/ps2gs.hで引っかかる。video/ps2*, machine/ps2* をコピーした。

## cpu/nec/v5x.h:

machine/am8517a.h がない。DMAコントローラらしいのでコピーした。

## imagedev/cdromimg.cpp

1. check_if_gd()がない

src/lib/util/chd.h:に enum class errorを返す check_if_gd()を作った。

```
	enum error check_is_cd() { return is_cd() ? error(0) : error::METADATA_NOT_FOUND; }
	enum error check_is_gd() { return is_gd() ? error(0) : error::METADATA_NOT_FOUND; }
	enum error check_is_dvd() { return is_dvd() ? error(0) : error::METADATA_NOT_FOUND; }
```

これで適切かどうかはわからない。

2. error: invalid use of incomplete type ‘class floppy_sound_device’

結局、`DEFINE_DEVICE_TYPE(FLOPPYSOUND, floppy_sound_device, "flopsnd", "Floppy sound")`

の第1引数FLOPPYSOUNDがテンプレート型になっていてそれで引っかかった。このマクロをコメントアウトすることで乗り切った。

## machine.lua エントリ消しまくり、machine/*.h/cpp コピーしまくり

でなんとかビルドして、

ld: -lshared がない、

まで漕ぎつけた。

## -lshared を消した

-lshared を LIBS に追加しているmakefileを見つけて消した。
makefileは生成物で、生成過程のどこで追加しているのかがよくわからない。

## drivlist.o で参照しているブツが大量にundefined

なので、drivlist.o がどうやってできているかを見た。

* もちろん、drivlist.cpp をコンパイルしている。
* drivlist.cpp は自動生成物である。
* genie で生成しているらしい。どのようにして生成しているかは読みこなせていない。
* mame.list を元ネタとしていることが分かった
* mame.list を消して make -d したときのログ参照(以下)

```
Reaping winning child 0x555974c76f70 PID 5996
Removing child 0x555974c76f70 PID 5996 from chain.
    Successfully remade target file '../../../../linux_gcc/obj/x64/Release/src/mame/mame.o'.
    Considering target file '../../../../linux_gcc/obj/x64/Release/generated/mame/mame/drivlist.o'.
     File '../../../../linux_gcc/obj/x64/Release/generated/mame/mame/drivlist.o' does not exist.
      Considering target file '../../../../generated/mame/mame/drivlist.cpp'.
       File '../../../../generated/mame/mame/drivlist.cpp' does not exist.
        Considering target file '../../../../../src/mame/mame.lst'.
         File '../../../../../src/mame/mame.lst' does not exist.
         Looking for an implicit rule for '../../../../../src/mame/mame.lst'.
         No implicit rule found for '../../../../../src/mame/mame.lst'.
         Finished prerequisites of target file '../../../../../src/mame/mame.lst'.
        Must remake target '../../../../../src/mame/mame.lst'.
make[2]: *** No rule to make target '../../../../../src/mame/mame.lst', needed by '../../../../generated/mame/mame/drivlist.cpp'.  Stop.
Reaping losing child 0x55d2e146a7e0 PID 5990
make[1]: *** [Makefile:112: mame] Error 2
Removing child 0x55d2e146a7e0 PID 5990 from chain.
Reaping losing child 0x55a042f6c450 PID 419
make: *** [makefile:1288: linux_x64] Error 2
Removing child 0x55a042f6c450 PID 419 from chain.
kuma@:~/mame-test$ exit
```

* mame.list 中には膨大な数のエントリがあった。

## mame.list 空ファイルでビルドしてみた。

リンクフェーズに入ったが、やはり -lshared がないと言われた。

```
kuma@:~/mame-test$ touch src/mame/mame.lst
kuma@:~/mame-test$ make
GCC 12.4.1 detected
fatal: No names found, cannot describe anything.
Building driver list...
0 driver(s) found
Compiling generated/mame/mame/drivlist.cpp...
Compiling generated/version.cpp...
Linking mame...
/usr/bin/ld: cannot find -lshared
collect2: error: ld returned 1 exit status
make[2]: *** [mame.make:269: ../../../../../mame] Error 1
make[1]: *** [Makefile:112: mame] Error 2
make: *** [makefile:1288: linux_x64] Error 2
kuma@:~/mame-test$
```

## -lshared

`find . -type f |xargs grep lshared`を実行して探す。`mame.make`の中にあった。

```
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
./build/projects/sdl/mame/gmake-linux/mame.make:  LIBS               += $(LDDEPS) -ldl -lrt -lSDL2 -lm -lpthread -lutil -lshared -lGL -lasound -lQt5Widgets -lQt5Gui -lQt5Core -lpulse -lX11 -lXinerama -lXext -lXi -lSDL2_ttf -lfontconfig -lfreetype
```

## リンクエラー

未定義が大量に出る。

```
osdobj_common.cpp:(.text+0x4334): undefined reference to `SOUND_DSOUND'
/usr/bin/ld: osdobj_common.cpp:(.text+0x4341): undefined reference to `SOUND_XAUDIO2'
/usr/bin/ld: osdobj_common.cpp:(.text+0x434e): undefined reference to `SOUND_COREAUDIO'
/usr/bin/ld: osdobj_common.cpp:(.text+0x435b): undefined reference to `SOUND_JS'
/usr/bin/ld: osdobj_common.cpp:(.text+0x4368): undefined reference to `SOUND_SDL'
/usr/bin/ld: osdobj_common.cpp:(.text+0x4375): undefined reference to `SOUND_PORTAUDIO'
/usr/bin/ld: osdobj_common.cpp:(.text+0x4382): undefined reference to `SOUND_PULSEAUDIO'
/usr/bin/ld: osdobj_common.cpp:(.text+0x438f): undefined reference to `SOUND_NONE'
```

以下の定義をコメントアウト。

```
//MODULE_DEFINITION(SOUND_DSOUND, osd::sound_direct_sound)
```

```
//REGISTER_MODULE(m_mod_man, SOUND_PULSEAUDIO);
```

これは`NO_USE_PULSEAUDIO`マクロを定義すれば外せる。

```
//REGISTER_MODULE(m_mod_man, SOUND_XAUDIO2);
```
の2エントリがある。
```
//MODULE_DEFINITION(SOUND_XAUDIO2, osd::sound_xaudio2)
```

どうやら、コンパイル時に NO_USE_XXXXAUDIO を定義すると外せるらしい。

> makefile 中にNO_USE_PORTAUDIO = 1, NO_USE_PULSEAUDIO = 1がある。他はない。
> とりあえず両者を1にしておいた。

## SDL_GameControllerXXX

```
input_sdl.cpp:(.text+0x37d7): undefined reference to `SDL_GameControllerGetSerial'
/usr/bin/ld: input_sdl.cpp:(.text+0x38ba): undefined reference to `SDL_JoystickGetSerial'
/usr/bin/ld: ../../../../linux_gcc/bin/x64/Release/mame_mame/libosd_sdl.a(input_sdl.o): in function `osd::(anonymous namespace)::sdl_joystick_module::handle_event(SDL_Event const&)':
input_sdl.cpp:(.text+0x3bf1): undefined reference to `SDL_JoystickGetSerial'
/usr/bin/ld: ../../../../linux_gcc/bin/x64/Release/mame_mame/libosd_sdl.a(input_sdl.o): in function `osd::(anonymous namespace)::sdl_game_controller_device::configure(osd::input_device&)':
input_sdl.cpp:(.text+0x4bf8): undefined reference to `SDL_GameControllerGetType'
/usr/bin/ld: input_sdl.cpp:(.text+0x4e8b): undefined reference to `SDL_GameControllerHasAxis'
/usr/bin/ld: input_sdl.cpp:(.text+0x548c): undefined reference to `SDL_GameControllerHasButton'
/usr/bin/ld: input_sdl.cpp:(.text+0x54d8): undefined reference to `SDL_GameControllerHasAxis'
/usr/bin/ld: input_sdl.cpp:(.text+0x5791): undefined reference to `SDL_GameControllerHasButton'
```

ソース参照箇所を全部コメントアウトした。とりあえず false を入れて置けばよさそうだったので。

## floppy_imaga::~floppy_image()

デストラクタ未定義とのこと、src/lib/formats/flopimg.* をコピー追加しておいた。

ここでmake clean して再ビルド。

## mame.lst 自動生成？

make clean したが make.lst は前のまま。

mame.lst を消して make clean したが、mame.lstができなかった。
ということで、これは自動生成とみなさない、でよさそう。

消したままでビルドを進めてみる。

```
Compiling src/devices/cpu/z80/z80.cpp...
Compiling src/devices/cpu/z80/z80n.cpp...
Archiving liboptional.a...
make[2]: *** No rule to make target '../../../../../src/mame/mame.lst', needed by '../../../../generated/mame/mame/drivlist.cpp'.  Stop.
make[2]: *** Waiting for unfinished jobs....
Compiling src/mame/mame.cpp...
make[1]: *** [Makefile:112: mame] Error 2
make: *** [makefile:1288: linux_x64] Error 2
kuma@:~/mame-test$
```

ということで、ここまでmame.lstは参照されていない。

## mame.lst をバッサリ削る。

homebrew/rc2014 以外をばっさり削った。

これでリンクするとundefined多数。floppy_imaga, ata_interface, intelsh, 39sf40_deviceなど。romram.cppからの参照が多いので、romram.cpp内部をバッサリ削ることになるだろう。

## upd765.cpp/.hを外した。

## intelfsh.cpp/.hを追加した。

## コンパイル通った！

```
Compiling src/devices/cpu/z80/z80.cpp...
Compiling src/devices/cpu/z80/z80n.cpp...
Compiling src/devices/machine/exorterm.cpp...
Compiling src/devices/machine/ie15.cpp...
Archiving liboptional.a...
Compiling src/mame/mame.cpp...
Building driver list...
Compiling generated/version.cpp...
40743 driver(s) found
Compiling generated/mame/mame/drivlist.cpp...
Linking mame...
kuma@LAURELEY:~/mame$
```

mame.make の -lshared も外していないのに、なんで？

## と思ったらmameオリジナルだった。がっかり。

## floppy.o, pc_dsk.oでundefined多発

imagedev/の下をすべて外してみた。

## ドバドバ消した。

cpu.lua, machine.lua

ビルドして、

```
Compiling src/devices/machine/exorterm.cpp...
Compiling src/devices/machine/ie15.cpp...
Archiving liboptional.a...
make[2]: *** No rule to make target '../../../../../src/mame/mame.cpp', needed by '../../../../linux_gcc/obj/x64/Release/src/mame/mame.o'.  Stop.
make[2]: *** Waiting for unfinished jobs....
make[1]: *** [Makefile:112: mame] Error 2
make: *** [makefile:1290: linux_x64] Error 2
kuma@LAURELEY:~/mame$
```

mame.cppがない。今日はここまで。

## libformats.aが存在しない。で黙って止まる。

src/

```
Live child 0x558b1f9d4660 (../../../../linux_gcc/bin/x64/Release/mame_mame/libformats.a) PID 3540
Archiving libformats.a...
Reaping winning child 0x558b1f9d4660 PID 3540
Live child 0x558b1f9d4660 (../../../../linux_gcc/bin/x64/Release/mame_mame/libformats.a) PID 3541
Reaping winning child 0x558b1f9d4660 PID 3541
Live child 0x558b1f9d4660 (../../../../linux_gcc/bin/x64/Release/mame_mame/libformats.a) PID 3543
ar: '../../../../linux_gcc/bin/x64/Release/mame_mame/libformats.a': No such file
Reaping losing child 0x558b1f9d4660 PID 3543
make[2]: *** [formats.make:266: ../../../../linux_gcc/bin/x64/Release/mame_mame/libformats.a] Error 1
Removing child 0x558b1f9d4660 PID 3543 from chain.
Reaping losing child 0x5638717e51e0 PID 3539
make[1]: *** [Makefile:28: formats] Error 2
Removing child 0x5638717e51e0 PID 3539 from chain.
Reaping losing child 0x55d76b5427e0 PID 32746
make: *** [makefile:1288: linux_x64] Error 2
Removing child 0x55d76b5427e0 PID 32746 from chain.
kuma@:~/mame-test$ exit
```

formats.luaのエントリをall.cppだけ復活させて再ビルドすると、-lsharedまで来た。再度-lsharedを消してビルドを進める。

> -lshared は build/products/sdl/gmake-linux/mame.make にある。


## romram.cpp

./src/devices/bus/rc2014/romram.cpp

romram.cppを外すわけにはいかない。この中のundefinedを削除してゆく。

* メンバ m_flash 削除
* デバイス sc119_rom 削除

## serial.cpp

./src/devices/bus/rc2014/serial.cpp

rc2014パッケージからserial.cppを外してみる。
-lsharedが出たところで中断。今日はここまで。いったんcommit/push

## ビルド時間の短縮

3rdpartyなんて毎回再ビルドしなくてもいいんじゃないか。bus.luaを書き換えたらリンク対象ファイルリストだけ再計算すればいいんじゃないか。

ターゲットcleanでは、buildディレクトリ以下をすべて消している。これが消し過ぎになっていたと推察する。

ターゲットallcleanを作って全クリアをそちらに移動。

ターゲットcleanでは、buildsの下で消すもの最小限とする。

例えば、

```
allclean: genieclean
	@echo Cleaning...
	-$(SILENT)rm -f language/*/*.mo
	-$(SILENT)rm -rf $(BUILDDIR)
	-$(SILENT)rm -rf 3rdparty/bgfx/.build

clean:
	@echo Cleaning...
	-$(SILENT)rm -rf $(BUILDDIR)/projects/sdl/mame/gmake-linux
```

gmake-linux直下の`*.make`だけ消して再構成するようにしてみる。

### lib*.aを消す。

`bus.lua`からエントリを消して`make clean`だけでは足りなかった。

```
rm build/linux_gcc/bin/x64/Release/mame_mame/lib*.a
```

これでmakeで再ビルドが効率的にできるようになった。

`make clean`なしで`rm ..../lib*.a`だけでもいいかもしれない。

## debugimgui.o ... floopy_device_image 

これは debugimgui.cpp を触って外すしかないだろう。

`src/osd/modules/debugger/debugimgui.cpp`

floppy形式関連の処理をばっさと切った。コンパイルは通った。
画面処理全体に影響がでそうだ。

が、先に進もう。

## modules.cpp:

```
modules.cpp:(.text+0x83): undefined reference to `RC2014_SERIAL_IO'
/usr/bin/ld: modules.cpp:(.text+0x95): undefined reference to `RC2014_DUAL_SERIAL_40P'
/usr/bin/ld: modules.cpp:(.text+0xa7): undefined reference to `RC2014_COMPACT_FLASH'
/usr/bin/ld: modules.cpp:(.text+0xb9): undefined reference to `RC2014_ROM_RAM_512'
/usr/bin/ld: modules.cpp:(.text+0xdd): undefined reference to `RC2014_YM2149_SOUND'
/usr/bin/ld: modules.cpp:(.text+0xef): undefined reference to `RC2014_AY8190_SOUND'
/usr/bin/ld: modules.cpp:(.text+0x101): undefined reference to `RC2014_82C55_IDE'
/usr/bin/ld: modules.cpp:(.text+0x113): undefined reference to `RC2014_IDE_HDD'
/usr/bin/ld: modules.cpp:(.text+0x125): undefined reference to `RC2014_FDC9266'
/usr/bin/ld: modules.cpp:(.text+0x137): undefined reference to `RC2014_WD37C65'
/usr/bin/ld: modules.cpp:(.text+0x149): undefined reference to `RC2014_MICRO'
```

こういうのが一杯出る。device.option_addで指定しているので、これらを片っ端から消す。

```
	//device.option_add("serial", RC2014_SERIAL_IO);
	//device.option_add("sio_40p", RC2014_DUAL_SERIAL_40P);
```

## 一応リンクも通った。

```
kuma@:~/mame-test$ make
GCC 12.4.1 detected
fatal: No names found, cannot describe anything.
Compiling src/devices/bus/rc2014/modules.cpp...
Archiving liboptional.a...
Linking mame...
kuma@:~/mame-test$
```

さてどうなるか。

## バイナリが起動しない。

```
kuma@:~/mame-test$ ./mame
./mame: /lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.30' not found (required by ./mame)
./mame: /lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.29' not found (required by ./mame)
./mame: /lib/x86_64-linux-gnu/libstdc++.so.6: version `CXXABI_1.3.13' not found (required by ./mame)
kuma@:~/mame-test$
```

GCC-12のインストールに失敗している様子。

