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

> その後、以下の内容の /usr/bin/www-brower ファイルを作り chmod +x しておくと、
 > `xdg-open https://github.com/tendai22/`でページが開くことを確認した。

```
#! /bin/sh
exec '/mnt/c/Program Files/Google/Chrome/Application/chrome.exe' $*
```

> これで `git push origin rc2014only`が動作した。って、前回もそれで動かしてるやんか。

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

## allclean してフルビルド

make allclean してフルビルドした。

今度は、`osd::debugger::qt::` 関連で大量のundefinedが出た。

make clean が未完成状態ということ。

## osd/modules/debugger コードを外してビルド

make all clean

おそらく、`osdobj_common.cpp`の`#include "modules/debugger/debug_modules.h"`を外してビルドすることになるだろう。

エラーが出たので、osdobj_common.cppから、

```
	//REGISTER_MODULE(m_mod_man, DEBUG_WINDOWS);
	//REGISTER_MODULE(m_mod_man, DEBUG_QT);
	//REGISTER_MODULE(m_mod_man, DEBUG_IMGUI);
	//REGISTER_MODULE(m_mod_man, DEBUG_GDBSTUB);
	//REGISTER_MODULE(m_mod_man, DEBUG_NONE);
```

をコメントアウトしたらリンクが通った。

## osdobj_common.cpp

ここでosd関連モジュールの組み込みしているようだ。
なんとかNONEがいっぱいあるので、なんとかNONEだけ残して他をコメントアウトしてみよう。

SOUND_NONEも消していたが、これも復活させるとよいかもしれない。

osd関連のモジュール組み込みは、ここでコメントアウトする、かつ、関連ソースコードをビルド対象から除外する。

ちょっと見えてきた。

1. モジュールの組み込みは、`REGISTER_MODULE`で行う。

```
	REGISTER_MODULE(m_mod_man, MONITOR_SDL);
```

2. `MODULE_DEFINITION`が、`REGISTER_MODULE`の名前(例: MONITOR_SDL)とコード実体(クラス)と対応付ける。

```
MODULE_DEFINITION(MONITOR_SDL, sdl_monitor_module)
```

3. クラス名からファイル名を探し出し、*.luaの組み込み定義から外す。

これでいけそうだ。

## GCC-12 再ビルド

https://stackoverflow.com/questions/70835585/how-to-install-gcc-12-on-ubuntu

git cloneしたgcc-sourceにて


```
$ cd gcc-source
$ git branch -a
$ git checkout remotes/origin/releases/gcc-12
```

ビルドディレクトリを用意して、configure

--prefix指定なし、デフォルトで指定した。

```
mkdir ../gcc-12-build
$ cd ../gcc-12-build/
$ ./../gcc-source/configure --enable-languages=c,c++
```

必要なライブラリを確認した。すべて最新版がすでに入っていた。

```
$ sudo apt-get install libmpfrc++-dev libmpc-dev libgmp-dev gcc-multilib
Reading package lists... Done
Building dependency tree
Reading state information... Done
gcc-multilib is already the newest version (4:9.3.0-1ubuntu2).
libmpc-dev is already the newest version (1.1.0-1).
libmpfrc++-dev is already the newest version (3.6.6+ds-1).
libgmp-dev is already the newest version (2:6.2.0+dfsg-4ubuntu0.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
```

以前の configure のままで進んでOKだろう。

ビルド開始した。

```
$ make -j5
```

今日は時間切れになりそう。

## 結局、原因は、LD_LIBRARY_PATH 不備だったらしい。

```
    LD_LIBRARY_PATH="/usr/local/lib64"
    LD_RUN_PATH="/usr/local/lib64"
    export LD_LIBRARY_PATH
    export LD_RUN_PATH
```

を、.profile に含めることでmame起動してもライブラリがないと言われなくなった。

```
Libraries have been installed in:
   /home/kuma/opt/gcc-12.4.1/lib/../lib64

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the `-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the `LD_RUN_PATH' environment variable
     during linking
   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to `/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
 /usr/bin/mkdir -p '/home/kuma/opt/gcc-12.4.1/share/info'
 /usr/bin/install -c -m 644 ./libitm.info '/home/kuma/opt/gcc-12.4.1/share/info'
 install-info --info-dir='/home/kuma/opt/gcc-12.4.1/share/info' '/home/kuma/opt/gcc-12.4.1/share/info/libitm.info'
make[4]: Leaving directory '/home/kuma/gcc-12-build/x86_64-pc-linux-gnu/libitm'
make[3]: Leaving directory '/home/kuma/gcc-12-build/x86_64-pc-linux-gnu/libitm'
make[2]: Leaving directory '/home/kuma/gcc-12-build/x86_64-pc-linux-gnu/libitm'
make[1]: Leaving directory '/home/kuma/gcc-12-build'
$ echo $LIBDIR
```

LD_LIBRARY_PATH, LD_RUN_PATH を環境変数に追加することで動作した。

## exception: All sound modules failed to initialize

```
kuma@PC-C2387:~/mame-test$ ./mame
Ignoring MAME exception: All sound modules failed to initialize
Fatal error: All sound modules failed to initialize
```

sound/none.cppを復活させた。

## -verbose オプション

osd_printf_verbose関数のログ出力を有効にするために、-verboseオプションを付けて起動する。

## メンバm_monitor_moduleを削除した。

monitorrendor モジュールを削除したため、osd_common_t クラスから m_monitor_module を削除した。メンバ定義と参照箇所、元クラスを削除して再ビルド、実行した。

## Warning + Core dump

```
$ ./mame
Warning: -video none doesn't make much sense without -seconds_to_run
Segmentation fault (core dumped)
```

ということで、この Warning メッセージからソースを探る。

video_none::init関数内の最初のチェック。

## gdb 上で実行してみた

```
(gdb) r
Starting program: /home/kuma/mame-test/mame
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
warning: File "/usr/local/lib64/libstdc++.so.6.0.30-gdb.py" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
To enable execution of this file add
        add-auto-load-safe-path /usr/local/lib64/libstdc++.so.6.0.30-gdb.py
line to your configuration file "/home/kuma/.gdbinit".
To completely disable this security protection add
        set auto-load safe-path /
line to your configuration file "/home/kuma/.gdbinit".
For more information about this security protection see the
"Auto-loading safe path" section in the GDB manual.  E.g., run from the shell:
        info "(gdb)Auto-loading safe path"
[Detaching after fork from child process 2960]
Warning: -video none doesn't make much sense without -seconds_to_run
[New Thread 0x7ffff3004700 (LWP 2961)]

Thread 1 "mame" received signal SIGSEGV, Segmentation fault.
0x0000000001a1b108 in emulator_info::draw_user_interface(running_machine&) ()
(gdb) quit
```

と言われたので、~/.gdbinit に、

```
add-auto-load-safe-path /usr/local/lib64/libstdc++.so.6.0.30-gdb.py
```

を追記して再度 gdb mame 起動、run すると、

```
(gdb) r
Starting program: /home/kuma/mame-test/mame
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[Detaching after fork from child process 3104]
Warning: -video none doesn't make much sense without -seconds_to_run
[New Thread 0x7ffff3004700 (LWP 3105)]

Thread 1 "mame" received signal SIGSEGV, Segmentation fault.
0x0000000001a1b108 in emulator_info::draw_user_interface(running_machine&) ()
(gdb) bt
#0  0x0000000001a1b108 in emulator_info::draw_user_interface(running_machine&) ()
#1  0x000000000172951b in video_manager::frame_update(bool) ()
#2  0x000000000167b215 in running_machine::start() ()
#3  0x000000000167ddc5 in running_machine::run(bool) ()
#4  0x0000000001a1f908 in mame_machine_manager::execute() ()
#5  0x0000000001ad4705 in cli_frontend::start_execution(mame_machine_manager*, std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&) ()
#6  0x0000000001ad48fb in cli_frontend::execute(std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >&) ()
#7  0x0000000001a1b0e1 in emulator_info::start_frontend(emu_options&, osd_interface&, std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >&) ()
#8  0x000000000049cdcd in main ()
(gdb)
```

となった。いよいよ、emulator_info::start_frontend まで来た。frontend を外すか、シリアルデバイスで動くように改造するか。

その前に、-g オプションを付けてコンパイルしておきたい。CFLAGS に -g を追加する方法を探す。

## CFLAGS に -g を追加する。

と思ってソースツリーを漁ったが、explicit に CFLAGS を参照・定義・更新しているところが見つからなかった。

makefilesはGENieを使って生成されているようだ。ここは一発 GENie を調べてみよう。

## GENie

[GENie](https://github.com/bkaradzic/genie)はgithub.com 上にリポジトリがある。

* project generator tool: ビルド環境(project)を生成するツール。
* Lua script から projectを生成する。
* setting 一つで、複数のプロジェクトを生成できる。
* 対応ビルドツール(supported project genetors)
  + GNU Makefile
  + JSON Compilation Database (何じゃこれ?)
  + Ninja
  + Visual Studio (2010,12,13,15,17,19,2022)
  + Xcode
  だそうです。

MAMEはGENieを使ってビルド環境を構築している様子。

Documentation が scriptiong reference、リファレンスしかなく「辞書項目の列挙」状態で、「読める」文書を見つけられていない。

* BSD 3-clause Lisence の様子。

* premake (Release 4.4 beta 5)からforkした様子。

* [premake5についての解説](https://qiita.com/ousttrue/items/6d837d6daba47b51bd8e)。GENieの全体構造、Luaスクリプトお作法の参考になりそう。

# まっさらマシンに WSL2 入れてみた。

同人誌の環境構築ネタを補強した。本LOG.md最初の記載にマージする。

環境整備でやったこと。

### wslのインストール

```
> wsl.exe --install Ubuntu-22.04
```

```
$ sudo apt-get update
$ sudo apt-get upgrade
```

### VScode を使えるようにする。

最初に Windows版 VScode をインストール、Remote - WSL Extensionをインストールするのだ。これで WSL内部で `code .`と叩くとうまくいく。

今回は Windows版を入れずに、Linux 版 VScode をインストールしてしまい往生した。これでも code . と叩くと起動するのだ。WSL の Xサーバ機能がすごすぎるということなのだが、

* 日本語文字が豆腐になる。/usr/share/fonts/windows にフォントファイルをコピー、fc-cacheすると日本語文字は表示されるが MS Gothic で汚い。
* 日本語IME入力ができない。これは致命的。

```
$ sudo apt-get remove code
```

で Linux版を削除したが、今度は `code .`と叩いてもcodeが起動しない。

これは、`/mnt/c/Users/no-kumagai/AppData/Local/Programs/Microsoft VS Code/bin` にある `code`が vscode-server らしく、これを手で起動することでサーバのダウンロードが始まり、同時に PATH にも入った。

以後は `code .`と叩くと Windows版 VScode が起動して、日本語文字入力もできるようになった。

当たり前だが、

* Windows版 VScode をインストール。
* Windows版 VScode を起動して、Remote - WSL 拡張機能をインストールする

ことで動作する。今回はまっさらのWindowsから立ち上げているので、VScode をインストールする前に WSL内部でがんがん作業を進めていたのが敗因。

> Linux版remove後に、rehash していればよかったのかもしれない。もとから /mnt/c... はパスに入っていて、/usr/bin/code がなくなったので起動できないといわれていたように見えた。rehash も忘れるとは焼きが回ったもんだ。

### GCC-12 のインストール

#### 準備

* GNU make, flex, bison を事前に手でインストールしておく。  
  以下の download_prerequisites スクリプトでも入れてくれない。

半ば意図的に Ubuntu-20.44 を入れている。なので、apt-get install gcc で入る GCC は Version 12 ではない。手でインストールする。

#### インストール

[dsrevkovさんのスクリプト](https://gist.github.com/dstrebkov/ebe070c1e35d94f859c6cacae8d642ef)がよろしい。大まかな手順は

* `https://gcc.gnu.org/git/gcc,git`をcloneする。
* `remotes/origin/releases/gcc-12`をcheckoutする。
* `./contrib/download_prerequisites`(シェルスクリプト)を実行する。これで例の数値計算ライブラリ系のtarballがダウンロードされる。
* gcc-12-build ディレクトリをつくる。
* gcc-12-buildの下で configureを実行する。
  + --prefix=/home/kuma/opt/gcc-12.4.1 (手元にインストールする前提)
  + --enable-languages=c,c++,fortran,go
  + --disable-multilib
* make -j5
* make install (手元にインストールするので sudo不要)

#### パスを通す。

* .profile に /home/kuma/opt/gcc-12.4.1/bin を入れておくこと。
* LD_LIBRARY_PATH, LD_RUN_PATHの設定

```
# set envs for gcc-12.4.1
if [ -d "$HOME/opt/gcc-12.4.1/bin" ] ; then
    PATH="$HOME/opt/gcc-12.4.1/bin:$PATH"
fi
LD_LIBRARY_PATH=$HOME/opt/gcc-12.4.1/lib64
LD_RUN_PATH=$HOME/opt/gcc-12.4.1/lib64
export LD_LIBRARY_PATH LD_RUN_PATH
```

# mame-test のcloseとビルド

まっさらな環境ではビルドできない。sdl2, alsa, fontconfig, Qt5Widgetsがないといわれて怒られる。

ならば、この4つなしでもビルドできるようにしようじゃないか。

## alsa

alsa を外す方法は不明。pkg-config で alsa.pcが見つからないと言っている。が、pkg-configを呼び出しているところがわからない。configure でやっているらしいので、luaスクリプトを見てもわからない。portaudioが怪しい。

portaudioを切り離した(3rdparty/portaudioディレクトリ以下を抹消した)つもりだが、まだ出てくる。

## sdl2

これは、 OSD := nosdl として nosdlフォルダを掘り、nosdl.lua, nosdl_cfg.luaを適当に作成した。

## bgfx

bgfxも MAME_FILE をコメントアウトしまくった。

```
GCC 12.4.1 detected
fatal: No names found, cannot describe anything.
Compiling src/osd/modules/opengl/gl_shader_mgr.cpp...
次のファイルから読み込み:  ../../../../../src/osd/modules/opengl/gl_shader_tool.h:26,
         次から読み込み:  ../../../../../src/osd/modules/opengl/gl_shader_mgr.h:8,
         次から読み込み:  ../../../../../src/osd/modules/opengl/gl_shader_mgr.cpp:4:
../../../../../src/osd/modules/opengl/osd_opengl.h:39:26: 致命的エラー: SDL2/SDL_version.h: そのようなファイルやディレクトリはありません
   39 |                 #include <SDL2/SDL_version.h>
      |                          ^~~~~~~~~~~~~~~~~~~~
コンパイルを停止しました。
make[2]: *** [osd_noosd.make:1026: ../../../../linux_gcc/obj/x64/Release/osd_noosd/src/osd/modules/opengl/gl_shader_mgr.o] エラー 1
make[1]: *** [Makefile:19: osd_noosd] エラー 2
make: *** [makefile:1297: linux_x64] エラー 2
```

src/osd/modules/opengl も外したい。今日(11/15)はここまで。

# リンク遊びは控えて、ソースコードにまじめに取り組む。

* cpu を構成・起動して、それが必要とする rom/ram デバイスのみでハードウェアを構成する。
* z80 を題材に……と思ったが、物が多すぎて検索結果がとっちらかる。
* マイナーなCPU, 68HC11 で当ててみよう。

## 68hc11を使うハードウェア

cp2024, cdd2000 の2つぐらいか？

### cp2024 ... 25inch FDD box?

68hc11を搭載したFDCコントローラかな？いや、ConnerのHDDのようだ。cs0, cs1の名前も見える。

```
#ifndef MAME_BUS_ATA_CP2024_H
#define MAME_BUS_ATA_CP2024_H
```

このあたりから、ATA接続するデバイスに見える。ワンボードコンピュータとしてはわかりやすいデバイスだが。

### cdd2000

これは CR-Rデバイスらしい。

とすると、

* sdl2, alsa, ... なしでビルドして、
* cp2024 をデバイスとして立ち上げて、外から read_cs0とかで叩いてみればいいのか？

## ならば、rc2014のインタフェースを見て serial I/O の立て方を見ればいい？

rc2014は bus システムらしく、busの中にz80含めてデバイスがぶら下がっているイメージ。コード構造は複雑だ。

やっぱり cp2024 だけを起動するようにするか？

# osd, frontend をすべて外す。

* frontend.lua の MAME_DIR の src/frontend 含む行をコメントアウト。
* genie.lua をかなりいじる。_OPTIONS["osd"]のところをすべてコメントアウト。
* makefile で指定していた NO_X11 = 1 などをすべてコメントアウト。

```
Compiling src/emu/sound.cpp...
Compiling src/emu/speaker.cpp...
Compiling src/emu/tilemap.cpp...
Compiling src/emu/uiinput.cpp...
Compiling src/emu/validity.cpp...
Compiling src/emu/video.cpp...
Compiling src/emu/video/generic.cpp...
Compiling src/emu/video/resnet.cpp...
Compiling src/emu/video/rgbgen.cpp...
Compiling src/emu/video/rgbsse.cpp...
Compiling src/emu/video/rgbvmx.cpp...
```

これらソースも外したい。あと、エラー1か所出た。

```
Compiling 3rdparty/flac/src/libFLAC/bitreader.c...
../../../../3rdparty/expat/lib/xmlparse.c:109:4: エラー: #error You do not have support for any sources of high quality entropy enabled. For end user security, that is probably not what you want. Your options include: * Linux >=3.17 + glibc >=2.25 (getrandom): HAVE_GETRANDOM, * Linux >=3.17 + glibc (including <2.25) (syscall SYS_getrandom): HAVE_SYSCALL_GETRANDOM, * BSD / macOS >=10.7 (arc4random_buf): HAVE_ARC4RANDOM_BUF, * BSD / macOS (including <10.7) (arc4random): HAVE_ARC4RANDOM, * libbsd (arc4random_buf): HAVE_ARC4RANDOM_BUF + HAVE_LIBBSD, * libbsd (arc4random): HAVE_ARC4RANDOM + HAVE_LIBBSD, * Linux (including <3.17) / BSD / macOS (including <10.7) (/dev/urandom): XML_DEV_URANDOM, * Windows >=Vista (rand_s): _WIN32. If insist on not using any of these, bypass this error by defining XML_POOR_ENTROPY; you have been warned. If you have reasons to patch this detection code away or need changes to the build system, please open a bug. Thank you!
  109 | #  error You do not have support for any sources of high quality entropy \
      |    ^~~~~
```

なんのこと？

```
#  error You do not have support for any sources of high quality entropy \
    enabled.  For end user security, that is probably not what you want. \
    \
    Your options include: \
      * Linux >=3.17 + glibc >=2.25 (getrandom): HAVE_GETRANDOM, \
      * Linux >=3.17 + glibc (including <2.25) (syscall SYS_getrandom): HAVE_SYSCALL_GETRANDOM, \
      * BSD / macOS >=10.7 (arc4random_buf): HAVE_ARC4RANDOM_BUF, \
      * BSD / macOS (including <10.7) (arc4random): HAVE_ARC4RANDOM, \
      * libbsd (arc4random_buf): HAVE_ARC4RANDOM_BUF + HAVE_LIBBSD, \
      * libbsd (arc4random): HAVE_ARC4RANDOM + HAVE_LIBBSD, \
      * Linux (including <3.17) / BSD / macOS (including <10.7) (/dev/urandom): XML_DEV_URANDOM, \
      * Windows >=Vista (rand_s): _WIN32. \
    \
    If insist on not using any of these, bypass this error by defining \
    XML_POOR_ENTROPY; you have been warned. \
    \
    If you have reasons to patch this detection code away or need changes \
    to the build system, please open a bug.  Thank you!

```

こう読むのが正しいらしい。とりあえず、`#define XML_POOR_ENTROPY`して先に進む。

### libfrontend.a がないといわれる。

当たり前なのだが、このリンク要求を出させないためにどうするか＿

```
if (STANDALONE~=true) then
	links {
--		"frontend",
	}
```

が残っていたのでコメントアウトした。

### 返す刀で 3rdparty/flac も外した。

extlib.lua 中の flac 記載部分をコメントアウトした。

あちこちの lua ファイルに、`ext_includedir("flac")`, `ext_lib("flac")`があるので全部消すかコメントアウトした。

### libfrontend.a

makefile の SCRIPTS マクロ定義中に

```
	scripts/src/mame/frontend.lua \
```

があったので消した。

### libjpeg.h 

```
Compiling 3rdparty/libjpeg/jfdctfst.c...
次のファイルから読み込み:  ../../../../src/lib/util/avhuff.h:18,
         次から読み込み:  ../../../../src/lib/util/avhuff.cpp:61:
../../../../src/lib/util/flac.h:18:10: 致命的エラー: FLAC/all.h: そのようなファイルやディレクトリはありません
   18 | #include <FLAC/all.h>
      |          ^~~~~~~~~~~~
コンパイルを停止しました。
make[2]: *** [utils.make:566: ../../../linux_gcc/obj/x64/Release/src/lib/util/avhuff.o] エラー 1
make[2]: *** 未完了のジョブを待っています....
```

lib.lua から、`#include "flac.h"`を含むファイル

```
		--MAME_DIR .. "src/lib/util/chd.cpp",
		--MAME_DIR .. "src/lib/util/chd.h",
		--MAME_DIR .. "src/lib/util/chdcodec.cpp",
		--MAME_DIR .. "src/lib/util/chdcodec.h",
		--MAME_DIR .. "src/lib/util/avhuff.cpp",
		--MAME_DIR .. "src/lib/util/avhuff.h",

		--MAME_DIR .. "src/lib/util/flac.cpp",
		--MAME_DIR .. "src/lib/util/flac.h",
```

を外した。

### <alsa/asoundlib.h>

思い切って 3rdparty/portmidi も外す。

3rdparty.lua の project("portmidi")パートを全部消した

### rendfont.o がビルドできない。

```
make[2]: *** '../../../linux_gcc/obj/x64/Release/src/emu/rendfont.o' に必要なターゲット '../../../generated/emu/ui/uicmd14.fh' を make するルールがありません.  中止.
make[1]: *** [Makefile:76: emu] エラー 2
make: *** [makefile:1293: linux_x64] エラー 2
```

rendfont.cpp も外す。

### mameのリンクまで来た。

undefined 55件

```
kuma@PC-C3251:~/mame-test$ grep /usr/bin/ld xxx|sed  's/^.*references* to //' |sort -u
`chd_category()'
`chd_file::chd_file()'
`chd_file::clone_all_metadata(chd_file&)'
`chd_file::create(std::basic_string_view<char, std::char_traits<char> >, unsigned long, unsigned int, unsigned int const (&) [4], chd_file&)'
`chd_file::open(std::basic_string_view<char, std::char_traits<char> >, bool, chd_file*, std::function<std::unique_ptr<chd_file, std::default_delete<chd_file> > (util::sha1_t const&)> const&)'
`chd_file::parent_missing() const'
`chd_file::sha1()'
`chd_file::~chd_file()'
`chd_file::~chd_file()' follow
`emulator_info::periodic_check()'
`osd::directory::open(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
`osd::input_seq::backspace()'
`osd::input_seq::empty_seq'
`osd::input_seq::length() const'
`osd::input_seq::operator+=(input_code)'
`osd_file::open(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, std::unique_ptr<osd_file, std::default_delete<osd_file> >&, unsigned long&)'
`osd_get_full_path(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
`osd_subst_env[abi:cxx11](std::basic_string_view<char, std::char_traits<char> >)'
`osd_ticks()'
`osd_ticks_per_second()'
`osd_vprintf_error(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_error(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`osd_vprintf_info(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_info(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`osd_vprintf_verbose(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_warning(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_warning(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`render_font::char_width(float, float, char32_t)'
`render_font::get_scaled_bitmap_and_bounds(bitmap_argb32&, float, float, char32_t, rectangle&)'
`sound_manager::mute(bool, unsigned char)'
`sound_manager::sound_manager(running_machine&)'
`sound_manager::start_recording()'
`sound_manager::~sound_manager()'
`tilemap_manager::tilemap_manager(running_machine&)'
`tilemap_manager::~tilemap_manager()'
`validity_checker::validate_tag(char const*)'
`video_manager::begin_recording(char const*, movie_recording::format)'
`video_manager::frame_update(bool)'
`video_manager::save_snapshot(screen_device*, util::core_file&)'
`video_manager::video_manager(running_machine&)'
```

## 順につぶしてゆく

## chd

src/lib/util/chd.cpp あたりが元ネタか。

>     MAME Compressed Hunks of Data file format

だそうです。

使用しない前提で調べた。romload.cpp で使用しているだけだし、ROMデータ読み込みは使いたいとも考えたが、
外してみたら undefined 多数になったので、復活させて flac 形式のみ削除する作戦で進める。

## speaker.cpp/h

参照多数なので、speaker.cpp の処理をダミー化した。

```
void speaker_device::mix(stream_buffer::sample_t *leftmix, stream_buffer::sample_t *rightmix, attotime start, attotime end, int expected_samples, bool suppress)
```

stream_buffer が存在しないので、mix(...)を削除した。外部から参照されていたら個別に書き換える(削除する)

## device_state_entry/device_state_interface

汎用性があるようすだが、mame-sbcには今はいらなさそう。丸ごと消すか関数は残してスタブにするか。

device_state_entry: 検索結果635箇所、全部を消すのは辛そうだ。スタブ化を考える。

distate.cppを戻す。

これだけで、device_state_entry, devise_state_interface の undef はなくなった。

## chd_huffman_compressor/decompressor

chdcodec.cpp内部クラスなので、定義まるごと#if 0コメントアウトして除外した。

```
`osd::directory::open(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
`osd::input_seq::backspace()'
`osd::input_seq::empty_seq'
`osd::input_seq::length() const'
`osd::input_seq::operator+=(input_code)'
`osd_file::open(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, std::unique_ptr<osd_file, std::default_delete<osd_file> >&, unsigned long&)'
`osd_get_full_path(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
`osd_subst_env[abi:cxx11](std::basic_string_view<char, std::char_traits<char> >)'
`osd_ticks()'
`osd_ticks_per_second()'
`osd_vprintf_error(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_error(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`osd_vprintf_info(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_info(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`osd_vprintf_verbose(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_warning(util::detail::format_argument_pack<char, std::char_traits<char> > const&)'
`osd_vprintf_warning(util::detail::format_argument_pack<char, std::char_traits<char> > const&)' follow
`osd_work_queue_alloc(int)'
`osd_work_queue_free(osd_work_queue*)'
`render_font::char_width(float, float, char32_t)'
`render_font::get_scaled_bitmap_and_bounds(bitmap_argb32&, float, float, char32_t, rectangle&)'
`tilemap_manager::tilemap_manager(running_machine&)'
`tilemap_manager::~tilemap_manager()'
`validity_checker::validate_tag(char const*)'
`video_manager::begin_recording(char const*, movie_recording::format)'
`video_manager::frame_update(bool)'
`video_manager::save_snapshot(screen_device*, util::core_file&)'
`video_manager::video_manager(running_machine&)'
```

だいぶ減った。

## video_manager

* MAME_DIR から video.cpp/video.hを外す。
* video_manager を引いているクラスメンバをコメントアウトする。

## screen_device

* MAME_DIR から screen.h/cppを外す。
* debugcpu.cppからいくつか外す。
* dvstate.cpp
* diexec.cpp, render.cpp, rendlay.cpp
* diexec.cpp 中の`typeinfo for screen_device'が消せない。

## make clean (allcleanでなく)して再ビルドしたら screen_device消えた。

よくわからんがメモとして。

## render_font

* render.cpp, render.hを外す。

## #include "screen.h" が効いている。

MAME_DIR で外しても、#include されていれば参照されてしまう。ファイルをjunk に移してエラーを見る。