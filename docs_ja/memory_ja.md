# Emulated system memory and address spaces management

.. contents:: :local:


# 1. Overview

The memory subsystem (emumem and addrmap) combines multiple functions useful for system emulation:

* address bus decoding and dispatching with caching
* static descriptions of an address map
* RAM allocation and registration for state saving
* interaction with memory regions to access ROM

Devices create address spaces, e.g. decodable buses, through the `device_memory_interface`.  The machine configuration sets up address maps to put in the address spaces, then the device can do read and writes through the bus.

メモリサブシステム(emumemとaddrmap)は、システムエミュレーションに有用な複数の機能を兼ね備えています: 

* アドレスバスのデコードとディスパッチとキャッシュ
* アドレスマップの静的記述
* 状態保存のためのRAM割り当てと登録
* ROM にアクセスするためのメモリ領域との相互作用

デバイスは `device_memory_interface` を使ってアドレス空間、例えばデコード可能なバスを作成する。 マシンコンフィギュレーションはアドレス空間に置くアドレスマップを設定し、デバイスがバスを通じて読み書きできるようにする。

# 2. Basic concepts

## 2.1 Address spaces

An address space, implemented in the class **address_space**, represents an addressable bus with potentially multiple sub-devices connected requiring a decode.  It has a number of data lines (8, 16, 32 or 64) called data width, a number of address lines (1 to 32) called address width and an Endianness.  In addition an address shift allows for buses that have an atomic granularity different than a byte.

Address space objects provide a series of methods for read and write access, and a second series of methods for dynamically changing the decode.

クラス**address_space**で実装されるアドレス空間は、デコードを必要とする複数のサブデバイスが接続される可能性のある、アドレス指定可能なバスを表す。 アドレス空間は、データ幅と呼ばれるデータ線の数(8、16、32、64)、アドレス幅と呼ばれるアドレス線の数(1～32)、エンディアンを持つ。 さらに、アドレスシフトによって、1バイトとは異なるアトミックな粒度を持つバスを使用することができます。

アドレス空間オブジェクトは、読み取りと書き込みアクセスのための一連のメソッドと、動的にデコードを変更するための2番目の一連のメソッドを提供します。


## 2.2 Address maps

An address map is a static description of the decode expected when using a bus.  It connects to memory, other devices and methods, and is installed, usually at startup, in an address space.  That description is stored in an **address_map** structure which is filled programmatically.

アドレスマップは、バスを使用する際に予想されるデコードを静的に記述したものである。 メモリ、他のデバイス、メソッドに接続され、通常は起動時にアドレス空間にインストールされる。 その記述は**address_map**構造体に格納され、プログラムによって満たされる。

## 2.3 Shares, banks and regions

Memory shares are allocated memory zones that can be put in multiple places in the same or different address spaces, and can also be directly accessed from devices.

Memory banks are zones that indirect memory access, giving the possibility to dynamically and efficiently change where a zone actually points to.

Memory regions are read-only memory zones in which ROMs are loaded.

All of these have names allowing to access them.

メモリシェアは、割り当てられたメモリゾーンであり、同じアドレス空間や異なるアドレス空間の複数の場所に置くことができ、デバイスから直接アクセスすることもできる。

メモリバンクは、メモリアクセスを間接的に行うゾーンであり、ゾーンが実際に指す場所を動的かつ効率的に変更できる可能性を提供する。

メモリ領域は、ROMがロードされる読み出し専用メモリゾーンです。

これらすべては、アクセスするための名前を持ちます。

## 2.4 Views

Views are a way to multiplex different submaps over a memory range with fast switching.  It is to be used when multiple devices map at the same addresses and are switched in externally.  They must be created as an object of the device and then setup either statically in a memory map or dynamically through `install_*` calls.

Switchable submaps, aka variants, are named through an integer.  An internal indirection through a map ensures that any integer value can be used.

ビューは、高速スイッチングでメモリ範囲上に異なるサブマップを多重化する方法である。 複数のデバイスが同じアドレスにマッピングされ、外部から切り替えられる場合に使用される。 デバイスのオブジェクトとして作成し、メモリマップに静的にセットアップするか、`install_*`コールで動的にセットアップする必要があります。

切り替え可能なサブマップ(別名バリアント)は整数で名前を付けます。 マップを介した内部インダイレクトにより、どのような整数値でも使用できるようになります。

# 3. Memory objects

## 3.1 Shares - memory_share

```C++
    class memory_share {
        const std::string &name() const;
        void *ptr() const;
        size_t bytes() const;
        endianness_t endianness() const;
        u8 bitwidth() const;
        u8 bytewidth() const;
    };
```

A memory share is a named allocated memory zone that is automatically saved in save states and can be mapped in address spaces.  It is the standard container for memory that is shared between spaces, but also shared between an emulated CPU and a driver.  As such one has easy access to its contents from the driver class.

メモリ共有は、セーブステートで自動的に保存され、アドレス空間にマッピングできる、名前付き割り当てメモリゾーンです。 スペース間で共有されるメモリの標準的なコンテナですが、エミュレートされたCPUとドライバの間でも共有されます。 そのため、ドライバクラスからその内容に簡単にアクセスすることができます。

```C++
    required_shared_ptr<uNN> m_share_ptr;
    optional_shared_ptr<uNN> m_share_ptr;
    required_shared_ptr_array<uNN, count> m_share_ptr_array;
    optional_shared_ptr_array<uNN, count> m_share_ptr_array;

    [device constructor] m_share_ptr(*this, "name"),
    [device constructor] m_share_ptr_array(*this, "name%u", 0U),
```

At the device level, a pointer to the memory zone can easily be retrieved by building one of these four finders.  Note that like for every finder calling `target()` on the finder gives you the base pointer of the `memory_share` object.

デバイスレベルでは、これら4つのファインダのいずれかをビルドすることで、メモリゾーンへのポインタを簡単に取得することができる。 すべてのファインダのように、ファインダ上で `target()` を呼び出すと、 `memory_share` オブジェクトのベースポインタが得られることに注意してください。

```C++
    memory_share_creator<uNN> m_share;

    [device constructor] m_share(*this, "name", size, endianness),
```

A memory share can be created if it doesn’t exist in a memory map through that creator class.  If it already exists it is just retrieved.  That class behaves like a pointer but also has the `target()`, `length()`, `bytes()`, `endianness()`, `bitwidth()` and `bytewidth()` methods for share information.  The desired size is specified in bytes.

メモリシェアは、メモリマップに存在しなければ、そのクリエータクラスを通して作成することができる。 すでに存在する場合は、それを取得するだけです。 このクラスはポインタのように振る舞うが、共有情報のための `target()`、`length()`、`bytes()`、`endianness()`、`bitwidth()`、`bytewidth()` メソッドも持っている。 必要なサイズはバイト数で指定する。

```
    memory_share *memshare(string tag) const;
```

The `memshare` device method retrieves a memory share by name.  Beware that the lookup can be expensive, prefer finders instead.

デバイスメソッド `memshare` はメモリ共有を名前で検索する。 検索には時間がかかるので注意しよう。

## 3.2 Banks - memory_bank

```C++
    class memory_bank {
        const std::string &tag() const;
        int entry() const;
        void set_entry(int entrynum);
        void configure_entry(int entrynum, void *base);
        void configure_entries(int startentry, int numentry, void *base, offs_t stride);
        void set_base(void *base);
        void *base() const;
    };
```

A memory bank is a named memory zone indirection that can be mapped in address spaces.  It points to `nullptr` when created. `configure_entry` associates an entry number and a base pointer. `configure_entries` does the same for multiple consecutive entries spanning a memory zone.

`set_base` sets the base address for the active entry.  If there are no entries, entry 0 (zero) is automatically created and selected.  Use of `set_base` should be avoided in favour of pre-configured entries unless there are an impractically large number of possible base addresses.

`set_entry` dynamically and efficiently selects the active entry, `entry()` returns the active entry number, and `base()` gets the associated base pointer.

メモリバンクは、アドレス空間にマッピングできる名前付きメモリゾーンのインダイ レクションである。 作成時には `nullptr` を指す。`configure_entry` はエントリ番号とベースポインタを関連付ける。`configure_entries` は、メモリゾーンにまたがる連続した複数のエントリに対して同じことを行う。

`set_base` はアクティブなエントリのベースアドレスを設定する。 エントリがない場合は、エントリ 0 (ゼロ) が自動的に作成され、選択される。 `set_base`の使用は、現実的でないほど多くのベースアドレスが存在しない限り、事前に設定されたエントリを優先して避けるべきである。

`set_entry` はアクティブなエントリを動的に効率よく選択し、 `entry()` はアクティブなエントリ番号を返し、 `base()` は関連するベースポインタを取得する。

```C++
    required_memory_bank m_bank;
    optional_memory_bank m_bank;
    required_memory_bank_array<count> m_bank_array;
    optional_memory_bank_array<count> m_bank_array;

    [device constructor] m_bank(*this, "name"),
    [device constructor] m_bank_array(*this, "name%u", 0U),
```

At the device level, a pointer to the memory bank object can easily be retrieved by building one of these four finders.

デバイスレベルでは、メモリバンクオブジェクトへのポインタは、これら4つのファインダのいずれかを構築することで簡単に取り出すことができる。

```C++
    memory_bank_creator m_bank;

    [device constructor] m_bank(*this, "name"),
```

A memory bank can be created if it doesn’t exist in a memory map through that creator class.  If it already exists it is just retrieved.

メモリバンクは、メモリマップに存在しなければ、そのクリエータクラスを通して作成することができる。 すでに存在する場合は、取得されるだけだ。

```C++
    memory_bank *membank(string tag) const;
```

The `membank` device method retrieves a memory bank by name.  Beware that the lookup can be expensive, prefer finders instead.

`membank`デバイスメソッドは、メモリーバンクを名前で検索する。 ルックアップは高くつくことがあるので注意しよう。

## 3.3 Regions - memory_region

```C+
    class memory_region {
        u8 *base();
        u8 *end();
        u32 bytes() const;
        const std::string &name() const;
        endianness_t endianness() const;
        u8 bitwidth() const;
        u8 bytewidth() const;
        u8 &as_u8(offs_t offset = 0);
        u16 &as_u16(offs_t offset = 0);
        u32 &as_u32(offs_t offset = 0);
        u64 &as_u64(offs_t offset = 0);
    }
```

A region is used to store read-only data like ROMs or the result of fixed decryptions.  Their contents are not saved, which is why they should not being written to from the emulated system.  They don’t really have an intrinsic width (`base()` returns an `u8 *` always), which is historical and pretty much unfixable at this point.  The `as_*` methods allow for accessing them at a given width.

領域は、ROMのような読み取り専用のデータや、固定デクリプションの結果を格納するために使用される。 その内容は保存されないので、エミュレートされたシステムから書き込んではいけません。 本質的な幅は決まらない(`base()` は常に `u8` を返す)。これは歴史的なことであり、現時点では修正することは不可能である。 `as_*`メソッドを使えば、指定した幅でアクセスできる。

```C++
    required_memory_region m_region;
    optional_memory_region m_region;
    required_memory_region_array<count> m_region_array;
    optional_memory_region_array<count> m_region_array;

    [device constructor] m_region(*this, "name"),
    [device constructor] m_region_array(*this, "name%u", 0U),
```

At the device level, a pointer to the memory region object can easily be retrieved by building one of these four finders.

デバイスレベルでは、これら4つのファインダのいずれかを構築することで、メモリ領域オブジェクトへのポインタを簡単に取得することができる。

```C++
    memory_region *memregion(string tag) const;
```

The `memregion` device method retrieves a memory region by name. Beware that the lookup can be expensive, prefer finders instead.

デバイスメソッド `memregion` はメモリ領域を名前で検索する。検索には時間がかかるので、代わりにファインダを使うとよい。


## 3.4 Views - memory_view

```C++
    class memory_view {
        memory_view(device_t &device, std::string name);
        memory_view_entry &operator[](int slot);

        void select(int entry);
        void disable();

        const std::string &name() const;
    }
```

A view allows to switch part of a memory map between multiple possibilities, or even disable it entirely to see what was there before.  It is created as an object of the device.

ビューは、メモリーマップの一部を複数の可能性の間で切り替えたり、あるいは完全に無効にして前にあったものを見たりすることができる。 デバイスのオブジェクトとして作成されます。

```C++
    memory_view m_view;

    [device constructor] m_view(*this, "name"),
```

It is then setup through the address map API or dynamically.  At runtime, a numbered variant can be selected using the `select` method, or the view can be disabled using the `disable` method.  A disabled view can be re-enabled at any time.

その後、アドレスマップ API を通して、あるいは動的に設定される。 実行時に、`select` メソッドで番号付きバリアントを選択したり、`disable` メソッドでビューを無効にしたりすることができる。 無効にしたビューはいつでも再び有効にすることができる。


## 3.5 Bus contention handling

Some specific CPUs have been upgraded to be interruptible which allows to add bus contention and wait states capabitilites.  Being interruptible means, in practice, that an instruction can be interrupted at any time and the execute_run method of the core exited. Other devices can then run, then eventually controls returns to the core and the instruction continues from the point it was started. Importantly, this can be triggered from a handler and even be used to interrupt just before the access that is currently done (e.g. continuation will redo the access).

The CPUs supporting that declare their capability by overriding the method `cpu_is_interruptible` to return true.

Three intermediate contention handlers can be added to accesses:

* `before_delay`: wait a number of cycles before doing the access.
* `after_delay`: wait a number of cycles after doing the access.
* `before_time`: wait for a given time before doing the access.

For the delay handlers, a method or lambda is called which returns the number of cycles to wait (as a u32).

一部のCPUは割り込み可能なようにアップグレードされ、バス競合や待機状態の機能を追加できるようになった。 割り込み可能であるということは、実際には、命令をいつでも割り込むことができ、コアのexecute_runメソッドが終了することを意味する。その後、他のデバイスが実行され、最終的に制御がコアに戻り、命令が開始された時点から継続される。重要なのは、これをハンドラからトリガすることができ、現在行われているアクセスの直前に割り込む(例えば、継続がアクセスをやり直す)ために使うこともできることだ。

これをサポートするCPUは、`cpu_is_interruptible`メソッドをオーバーライドしてtrueを返すことで、その能力を宣言する。

アクセスには3つの中間競合ハンドラを追加できる：

* `before_delay`: アクセスを行う前に何サイクルか待つ。
* `after_delay`: アクセスを行った後、サイクル数待つ。
* `after_delay`: アクセスを行った後、何サイクルか待つ。

delay ハンドラでは、待つサイクル数を返すメソッドまたはラムダが呼び出される(u32)。

The `before_time` is special.  First, the time is compared to the current value of cpu->total_cycles().  That value is the number of cycles elapsed since the last reset of the cpu. It is passed as a parameter to the method as a u64 and must return the earliest time as a u64 when the access can be done, which can be equal to the passed-in time.  From there two things can happen: either the running cpu has enough cycles left to consume to reach that time.  In that case, the necessary number of cycles is consumed, and the access is done. Otherwise, when there isn't enough, the remaining cycles are consumed, the access aborted, scheduling happens, and eventually the access is redone.  In that case the method is called again with the new current time, and must return the (probably same) earliest time again.  This will happen until enough cycles to consume are available to directly do the access.

This approach allows to for instance handle consecutive DMAs.  A first DMA grabs the bus for a transfer.  This shows up as the method answering for the earliest time for access the time of the end of the dma.  If no timer happens until that time the access will then happen just after the dma finishes.  But if a timer elapses before that and as a consequence another dma is queued while the first is running, the cycle will be aborted for lack of remaining time, and the method will eventually be called again.  It will then give the time of when the second dma will finish, and all will be well.

`before_time`は特別である。 まず、この時間はcpu->total_cycles()の現在の値と比較される。 この値は、CPUが最後にリセットされてから経過したサイクル数である。これはu64としてメソッドにパラメータとして渡され、アクセスが可能な最も早い時刻をu64として返さなければならない。 この時刻は、渡された時刻に等しい。そこから2つのことが起こり得る。 その場合、必要なサイクル数が消費され、アクセスが行われる。そうでない場合は、残りのサイクルが消費され、アクセスが中断され、スケジューリングが行われ、最終的にアクセスがやり直される。 その場合、メソッドは新しい現在時刻で再度呼び出され、(おそらく同じ)最も早い時刻を返さなければならない。 これは、アクセスを直接実行するのに十分な消費サイクルが利用可能になるまで続く。

この方法により、例えば連続したDMAを処理することができる。 最初のDMAは転送のためにバスをつかむ。 これは、DMAが終了する時刻に、最も早いアクセス時刻に応答するメソッドとして表示される。 その時間までタイマが起きなければ、アクセスはDMA終了直後に行われる。 しかし、その前にタイマが経過し、その結果、最初のdmaが実行されている間に別のdmaがキューに入れられた場合、サイクルは残り時間がないために中断され、最終的にこのメソッドが再び呼び出されることになる。 そして、2つ目のdmaが終了する時刻が表示され、すべてが完了する。

It can also allow to reduce said earlier time when circumstances require it.  For instance a PIO latch that waits up to 64 cycles that data arrives can indicate that current time + 64 as a target (which will trigger a bus error for instance) but if a timer elapses and fills the latch meanwhile the method will be called again and that time can just return the current time to let the access pass though. Beware that if the timer elapsing did not fill the latch then the method must return the time it returned previously, e.g. the initial access time + 64, otherwise irrelevant timers happening or simply scheduling quantum effects will delay the timeout, possibly to infinity if the quantum is small enough.

Contention handlers on the same address are taken into account in the `before_time`, `before_delay` then `after_delay` order. Contention handlers of the same type on the same address at last-one-wins.  Installing any non-contention handler on a range where a contention handler was removes it.

また、状況がそれを必要とする場合、先の時間を短縮することもできる。 例えば、データが到着するのを最大64サイクル待つPIOラッチは、ターゲットとして現在時刻＋64を示すことができる(これは例えばバスエラーをトリガする)が、その間にタイマが経過してラッチが満たされると、メソッドは再び呼び出され、その時はアクセスを通過させるために現在時刻を返すことができる。そうでなければ、無関係なタイマが発生したり、単に量子効果がスケジューリングされたりして、タイムアウトが遅れてしまう。

同じアドレスの競合ハンドラは `before_time`、`before_delay`、`after_delay`の順に考慮される。同じアドレス上の同じタイプの競合ハンドラは最後に1つ勝つ。 競合ハンドラがあった範囲に非競合ハンドラを設置すると、そのハンドラは削除される。

# 4. Address maps API

## 4.1 General API structure

An address map is a method of a device which fills an **address_map** structure, usually called **map**, passed by reference.  The method then can set some global configuration through specific methods and then provide address range-oriented entries which indicate what should happen when a specific range is accessed.

The general syntax for entries uses method chaining:

アドレスマップは、参照渡しされた**address_map**構造体(通常は**map**と呼ばれる)を埋めるデバイスのメソッドである。 このメソッドは、特定のメソッドを通じてグローバルな設定を行い、特定の範囲にアクセスしたときに何が起こるかを示すアドレス範囲指向のエントリを提供することができる。

エントリの一般的な構文は、メソッドチェインを使用します：

```C++
    map(start, end).handler(...).handler_qualifier(...).\
        range_qualifier().contention();
```

The values start and end define the range, the handler() block determines how the access is handled, the handler_qualifier() block specifies some aspects of the handler (memory sharing for instance) and the range_qualifier() block refines the range (mirroring, masking, lane selection, etc.).  The contention methods handle bus contention and wait states for cpus supporting them.

The map follows a “last one wins” principle, where the handler specified last is selected when multiple handlers match a given address.

startとendは範囲を定義し、handler()ブロックはアクセスの処理方法を決定し、handler_qualifier()ブロックはハンドラのいくつかの側面(例えばメモリ共有)を指定し、range_qualifier()ブロックは範囲を絞り込む(ミラーリング、マスキング、レーン選択など)。 コンテンションメソッドは、バスのコンテンションと、それをサポートするCPUのウェイトステートを処理する。

マップは 「Last one wins」の原則に従い、複数のハンドラが指定されたアドレスにマッチした場合、最後に指定されたハンドラが選択される。

## 4.2 Global configurations

### 4.2.1 Global masking

```C++
    map.global_mask(offs_t mask);
```

Specifies a mask to be applied to all addresses when accessing the space that map is installed in.

マップがインストールされているスペースにアクセスする際に、すべてのアドレスに適用されるマスクを指定する。

### 4.2.2 Returned value on unmapped/nop-ed read

```C++
    map.unmap_value_low();
    map.unmap_value_high();
    map.unmap_value(u8 value);
```

Sets the value to return on reads to an unmapped or nopped-out address. Low means 0, high ~0.

マップされていない、またはノップアウトされたアドレスへのリード時に返す値を設定する。Lowは0、Highは～0を意味する。

## 4.3 Handler setting

### 4.3.1 Method on the current device

```C++
    (...).r(FUNC(my_device::read_method))
    (...).w(FUNC(my_device::write_method))
    (...).rw(FUNC(my_device::read_method), FUNC(my_device::write_method))

    uNN my_device::read_method(address_space &space, offs_t offset, uNN mem_mask)
    uNN my_device::read_method(address_space &space, offs_t offset)
    uNN my_device::read_method(address_space &space)
    uNN my_device::read_method(offs_t offset, uNN mem_mask)
    uNN my_device::read_method(offs_t offset)
    uNN my_device::read_method()

    void my_device::write_method(address_space &space, offs_t offset, uNN data, uNN mem_mask)
    void my_device::write_method(address_space &space, offs_t offset, uNN data)
    void my_device::write_method(address_space &space, uNN data)
    void my_device::write_method(offs_t offset, uNN data, uNN mem_mask)
    void my_device::write_method(offs_t offset, uNN data)
    void my_device::write_method(uNN data)
```

Sets a method of the current device or driver to read, write or both for the current entry.  The prototype of the method can take multiple forms making some elements optional.  `uNN` represents `u8`, `u16`, `u32` or `u64` depending on the data width of the handler. The handler can be narrower than the bus itself (for instance an 8-bit device on a 32-bit bus).

The offset passed in is built from the access address.  It starts at zero at the start of the range, and increments for each `uNN` unit. An `u8` handler will get an offset in bytes, an `u32` one in double words.  The `mem_mask` has its bits set where the accessors actually drive the bit.  It’s usually built in byte units, but in some cases of I/O chips ports with per-bit direction registers the resolution can be at the bit level.

現在のデバイスまたはドライバのメソッドを設定し、現在のエントリの読み取り、書き込み、またはその両方を行う。 メソッドのプロトタイプは複数の形式を取ることができ、いくつかの要素はオプションになります。 `uNN` はハンドラのデータ幅に応じて `u8`、`u16`、`u32` または `u64` を表す。ハンドラのデータ幅はバス自体よりも狭くすることができる(例えば、32 ビットバス上の 8 ビットデバイス)。

渡されるオフセットはアクセスアドレスから作られる。 範囲の開始点は 0 から始まり、`uNN` ユニットごとに増加します。`u8` ハンドラではバイト単位、`u32` ハンドラではダブルワード単位のオフセットが渡される。 `mem_mask`はアクセッサが実際にビットを駆動する場所にビットが設定される。 通常はバイト単位でビルドされるが、ビット単位のディレクションレジスタを持つI/Oチップポートの場合、ビットレベルの分解能になることもある。

### 4.3.2 Method on a different device

```
    (...).r(m_other_device, FUNC(other_device::read_method))
    (...).r("other-device-tag", FUNC(other_device::read_method))
    (...).w(m_other_device, FUNC(other_device::write_method))
    (...).w("other-device-tag", FUNC(other_device::write_method))
    (...).rw(m_other_device, FUNC(other_device::read_method), FUNC(other_device::write_method))
    (...).rw("other-device-tag", FUNC(other_device::read_method), FUNC(other_device::write_method))
```

Sets a method of another device, designated by an object finder (usually `required_device` or `optional_device`) or its tag, to read, write or both for the current entry.

オブジェクトファインダ(通常は `required_device`または `optional_device`)またはそのタグで指定された別のデバイスのメソッドを、現在のエントリの読み込み、書き込み、またはその両方に設定する。


### 4.3.3 Lambda function

```C++
    (...).lr{8,16,32,64}(NAME([...](address_space &space, offs_t offset, uNN mem_mask) -> uNN { ... }))
    (...).lr{8,16,32,64}([...](address_space &space, offs_t offset, uNN mem_mask) -> uNN { ... }, "name")
    (...).lw{8,16,32,64}(NAME([...](address_space &space, offs_t offset, uNN data, uNN mem_mask) -> void { ... }))
    (...).lw{8,16,32,64}([...](address_space &space, offs_t offset, uNN data, uNN mem_mask) -> void { ... }, "name")
    (...).lrw{8,16,32,64}(NAME(read), NAME(write))
    (...).lrw{8,16,32,64}(read, "name_r", write, "name_w")
```

Sets a lambda called on read, write or both.  The lambda prototype can be any of the six available for methods.  One can either use `NAME()` over the whole lambda, or provide a name after the lambda definition. The number is the data width of the access, e.g. the NN.

読み込み、書き込み、またはその両方で呼び出されるラムダを設定する。 ラムダのプロトタイプは、メソッドで利用可能な6つのうちのどれでもよい。 ラムダ全体で `NAME()` を使うか、ラムダ定義の後に名前を指定する。数字はアクセスのデータ幅で、例えばNNである。

### 4.3.4 Direct memory access

```C++
    (...).rom()
    (...).writeonly()
    (...).ram()
```

Selects the range to access a memory zone as read-only, write-only or read/write respectively.  Specific handler qualifiers specify the location of this memory zone.  There are two cases when no qualifier is acceptable:

* `ram()` gives an anonymous RAM zone not accessible outside of the address space.

* `rom()` when the memory map is used in an `AS_PROGRAM` space of a (CPU) device which names is also the name of a region.   Then the memory zone points to that region at the offset   corresponding to the start of the zone.

メモリゾーンにアクセスする範囲を、それぞれ読み取り専用、書き込み専用、読み取り/書き込み専用から選択する。 特定のハンドラ修飾子は、このメモリゾーンの位置を指定する。 修飾子なしでもよい場合は2つある：

* `ram()`は、アドレス空間の外からはアクセスできない匿名RAMゾーンを与える。

* `rom()` は、 (CPU)デバイスの `AS_PROGRAM` 空間でメモリマップが使用され、その名前が領域の名前でもある場合。  その場合、メモリゾーンは、ゾーンの開始点に対応するオフセットでその領域を指す。

```C++
    (...).rom().region("name", offset)
```

The `region` qualifier causes a read-only zone point to the contents of a given region at a given offset.

`region`修飾子は、指定されたオフセットの指定された領域の内容を読み取り専用のゾーンポイントにする。


```C++
    (...).rom().share("name")
    (...).writeonly.share("name")
    (...).ram().share("name")
```

The `share` qualifier causes the zone point to a shared memory region identified by its name.  If the share is present in multiple spaces, the size, bus width, and, if the bus is more than byte-wide, the Endianness must match.

`share`修飾子は、ゾーンがその名前で識別される共有メモリ領域を指すようにする。 shareが複数の空間に存在する場合、サイズ、バス幅、バスがバイト幅を超える場合はエンディアンが一致しなければならない。


### 4.3.5 Bank access

```C++
    (...).bankr("name")
    (...).bankw("name")
    (...).bankrw("name")
```

Sets the range to point at the contents of a memory bank in read, write or read/write mode.

読み出し、書き込み、または読み出し/書き込みモードで、メモリバンクの内容を指す範囲を設定する。

### 4.3.6 Port access

```C++
    (...).portr("name")
    (...).portw("name")
    (...).portrw("name")
```

Sets the range to point at an I/O port.

I/Oポートを指す範囲を設定する。


### 4.3.7 Dropped access

```C++
    (...).nopr()
    (...).nopw()
    (...).noprw()
```

Sets the range to drop the access without logging.  When reading, the unmap value is returned.

ログを取らずにアクセスをドロップする範囲を設定する。 読み込み時には、マップ解除値が返されます。

### 4.3.8 Unmapped access

```C++
    (...).unmapr()
    (...).unmapw()
    (...).unmaprw()
```

Sets the range to drop the access with logging.  When reading, the unmap value is returned.

ロギングでアクセスをドロップする範囲を設定する。 読み込み時には、マップ解除値が返されます。

### 4.3.9 Subdevice mapping

```C++
    (...).m(m_other_device, FUNC(other_device::map_method))
    (...).m("other-device-tag", FUNC(other_device::map_method))
```

Includes a device-defined submap.  The start of the range indicates where the address zero of the submap ends up, and the end of the range clips the submap if needed.  Note that range qualifiers (defined later) apply.

Currently, only handlers are allowed in submaps and not memory zones or banks.

デバイス定義のサブマップを含む。 範囲の開始点は、サブマップのアドレスゼロの終点を示し、範囲の終了点は、必要であればサブマップをクリップする。 範囲修飾子(後で定義)が適用されることに注意。

現在のところ、サブマップではハンドラだけが許され、メモリゾーンやバンクは許されない。


## 4.4 Range qualifiers

### 4.4.1 Mirroring

```C++
    (...).mirror(mask)
```

Duplicate the range on the addresses reachable by setting any of the 1 bits present in mask.  For instance, a range 0-0x1f with mirror 0x300 will be present on 0-0x1f, 0x100-0x11f, 0x200-0x21f and 0x300-0x31f. The addresses passed in to the handler stay in the 0-0x1f range, the mirror bits are not seen by the handler.

maskに存在するいずれかの1ビットを設定することで、到達可能なアドレスの範囲を複製する。 例えば、ミラー0x300を持つ0-0x1fの範囲は、0-0x1f、0x100-0x11f、0x200-0x21f、0x300-0x31fに存在することになる。ハンドラに渡されるアドレスは0-0x1fの範囲に留まり、ミラービットはハンドラには見えない。

### 4.4.2 Masking

```C++
    (...).mask(mask)
```

Only valid with handlers, the address will be masked with the mask before being passed to the handler.

ハンドラでのみ有効で、アドレスはハンドラに渡される前にマスクで覆われる。


### 4.4.3 Selection

```C++
    (...).select(mask)
```

Only valid with handlers, the range will be mirrored as with mirror, but the mirror address bits are preserved in the offset passed to the handler when it is called.  This is useful for devices like sound chips where the low bits of the address select a function and the high bits a voice number.

ハンドラでのみ有効で、範囲はミラーと同様にミラーされるが、ミラーアドレスビットは、ハンドラが呼び出されたときにハンドラに渡されるオフセットに保持される。 これは、サウンドチップのように、アドレスの下位ビットが機能を選択し、上位ビットが音声番号を選択するデバイスに便利です。

### 4.4.4 Sub-unit selection

```C++
    (...).umask16(16-bits mask)
    (...).umask32(32-bits mask)
    (...).umask64(64-bits mask)
````

Only valid with handlers and submaps, selects which data lines of the bus are actually connected to the handler or the device.  The mask value should be a multiple of a byte, e.g. the mask is a series of 00 and ff. The offset will be adjusted accordingly, so that a difference of 1 means the next handled unit in the access.

If the mask is narrower than the bus width, the mask is replicated in the upper lines.

ハンドラとサブマップでのみ有効で、バスのどのデータ線がハンドラまたはデバイスに実際に接続されるかを選択する。 マスク値は1バイトの倍数でなければなりません。例えば、マスクは00とffの連続です。オフセットはそれに応じて調整され、1 の差はアクセスで次に処理されるユニットを意味します。

マスクがバス幅より狭い場合、マスクは上位行に複製されます。

### 4.4.5 Chip select handling on sub-unit

```C++
    (...).cselect(16/32/64)
```

When a device is connected to part of the bus, like a byte on a 16-bits bus, the target handler is only activated when that part is actually accessed.  In some cases, very often byte access on a 68000 16-bits bus, the actual hardware only checks the word address and not if the correct byte is accessed.  `cswidth` tells the memory system to trigger the handler if a wider part of the bus is accessed.  The parameter is that trigger width (would be 16 in the 68000 case).

デバイスがバスの一部、例えば16ビットバスのバイトに接続されている場合、ターゲットハンドラはその一部が実際にアクセスされた時にのみ起動される。 場合によっては、68000の16ビットバスのバイトアクセスが非常に多いが、実際のハードウェアはワードアドレスをチェックするだけで、正しいバイトがアクセスされたかどうかはチェックしない。 `cswidth`は、バスのより広い部分がアクセスされた場合にハンドラをトリガするよう、 メモリシステムに指示する。 パラメータはトリガ幅(68000の場合は16)である。

### 4.4.6 User flags

```C++
    (...).flags(16-bits mask)
```

This parameter allows to set user-defined flags on the handler which can then be retrieved by an accessing device to change their behaviour.  An example of use the i960 which marks burstable zones that way (they have a specific hardware-level support).

このパラメーターを使用すると、ハンドラにユーザー定義のフラグを設定することができ、そのフラグをアクセスするデバイスが取得することで、ハンドラの動作を変更することができる。 使用例としては、バースト可能ゾーンをこのようにマークするi960がある(これらは特定のハードウェアレベルのサポートを持っている)。


## 4.5 Contention

```C++
    (...).before_time(method).(...)
    (...).before_delay(method).(...)
    (...).after_delay(method).(...)
```

These three methods allow to add the contention methods to a handler. See section `3.5`_.  Multiple methods can be handler to one handler.

これら3つのメソッドはハンドラにコンテンションメソッドを追加することができます。セクション `3.5`_ を参照してください。 1つのハンドラに複数のメソッドを追加することができます。

## 4.6 View setup

```C++
    map(start, end).view(m_view);
    m_view[0](start1, end1).[...];
```

A view is setup in a address map with the view method.  The only qualifier accepted is mirror.  The “disabled” version of the view will include what was in the range prior to the view setup.

The different variants are setup by indexing the view with the variant number and setting up an entry in the usual way.  The entries within a variant must of course stay within the range.  There are no other additional constraints.  The contents of a variant, by default, are what was there before, i.e. the contents of the disabled view, and setting it up allows part or all of it to be overridden.

Variants can only be setup once the view itself has been setup with the `view` method.

A view can only be put in one address map and in only one position. If multiple views have identical or similar contents, remember that setting up a map is nothing more than a method call, and creating a second method to setup a view is perfectly reasonable.  A view is of type `memory_view` and an indexed entry (e.g. a variant to setup) is of type `memory_view::memory_view_entry &`.

A view can be installed in another view, but don’t forget that a view can be installed only once.  A view can also be part of “what was there before”.

ビューはビューメソッドでアドレスマップにセットアップされる。 唯一の修飾子はミラーである。 ビューの「無効」バージョンは、ビューのセットアップの前に範囲にあったものを含みます。

異なるバリアントはバリアント番号でビューのインデックスを作り、 通常の方法でエントリを設定することで設定されます。 variant 内のエントリはもちろん範囲内に収まっていなければなりません。 それ以外の制約はありません。 variant の内容は、デフォルトでは前にあったもの、つまり無効にされたビューの内容です。

バリアントは `view` メソッドでビューそのものをセットアップしてからでないとセットアップできません。

ビューは1つのアドレスマップに1つだけ配置することができます。複数のビューが同一または類似の内容を持つ場合、マップをセットアップすることはメソッド呼び出しに過ぎず、ビューをセットアップするために2つ目のメソッドを作成することは非常に合理的であることを覚えておいてください。 ビューは `memory_view` 型で、インデックス付きエントリ(セットアップのためのバリアントなど)は `memory_view::memory_view_entry &` 型です。

ビューは別のビューにインストールすることができますが、ビューは一度しかインストールできないことを忘れないでください。 ビューは 「what was there before 」の一部になることもできます。

# 5. Address space dynamic mapping API

## 5.1 General API structure

A series of methods allow the bus decoding of an address space to be changed on-the-fly.  They’re powerful but have some issues:

* changing the mappings repeatedly can be slow
* the address space state is not saved in the saved states, so it has to be rebuilt after state load
* they can be hidden anywhere rather than be grouped in an address map, which can be less readable

The methods, rather than decomposing the information in handler, handler qualifier and range qualifier, put them all together as method parameters.  To make things a little more readable, lots of them are optional.

一連のメソッドにより、アドレス空間のバスデコーディングをその場で変更することができる。 これらは強力だが、いくつかの問題がある：

* マッピングを繰り返し変更するのは時間がかかる。
* アドレス空間のステートはセーブステートに保存されないため、ステートロード後に再構築する必要がある。
* アドレスマップにグループ化するのではなく、任意の場所に隠すことができるため、可読性が低下する可能性がある。

メソッドでは、情報をハンドラ、ハンドラ修飾子、範囲修飾子で分解するのではなく、メソッドのパラメーターとしてまとめている。 もう少し読みやすくするために、それらの多くはオプションになっている。

## 5.2 Handler mapping

```C++
    uNN my_device::read_method(address_space &space, offs_t offset, uNN mem_mask)
    uNN my_device::read_method_m(address_space &space, offs_t offset)
    uNN my_device::read_method_mo(address_space &space)
    uNN my_device::read_method_s(offs_t offset, uNN mem_mask)
    uNN my_device::read_method_sm(offs_t offset)
    uNN my_device::read_method_smo()

    void my_device::write_method(address_space &space, offs_t offset, uNN data, uNN mem_mask)
    void my_device::write_method_m(address_space &space, offs_t offset, uNN data)
    void my_device::write_method_mo(address_space &space, uNN data)
    void my_device::write_method_s(offs_t offset, uNN data, uNN mem_mask)
    void my_device::write_method_sm(offs_t offset, uNN data)
    void my_device::write_method_smo(uNN data)

    readNN_delegate   (device, FUNC(read_method))
    readNNm_delegate  (device, FUNC(read_method_m))
    readNNmo_delegate (device, FUNC(read_method_mo))
    readNNs_delegate  (device, FUNC(read_method_s))
    readNNsm_delegate (device, FUNC(read_method_sm))
    readNNsmo_delegate(device, FUNC(read_method_smo))

    writeNN_delegate   (device, FUNC(write_method))
    writeNNm_delegate  (device, FUNC(write_method_m))
    writeNNmo_delegate (device, FUNC(write_method_mo))
    writeNNs_delegate  (device, FUNC(write_method_s))
    writeNNsm_delegate (device, FUNC(write_method_sm))
    writeNNsmo_delegate(device, FUNC(write_method_smo))
```

To be added to a map, a method call and the device it is called onto have to be wrapped in the appropriate delegate type.  There are twelve types, for read and for write and for all six possible prototypes. Note that as all delegates, they can also wrap lambdas.

マップに追加するには、メソッド呼び出しとそれが呼び出されるデバイスを、適切なデリゲートタイプでラップする必要がある。 12種類の型があり、読み込み用と書き込み用、そして6種類のプロトタイプがある。他のデリゲートと同様に、ラムダをラップすることもできる。

```C++
    space.install_read_handler(addrstart, addrend, read_delegate, unitmask, cswidth, flags)
    space.install_read_handler(addrstart, addrend, addrmask, addrmirror, addrselect, read_delegate, unitmask, cswidth, flags)
    space.install_write_handler(addrstart, addrend, write_delegate, unitmask, cswidth, flags)
    space.install_write_handler(addrstart, addrend, addrmask, addrmirror, addrselect, write_delegate, unitmask, cswidth, flags)
    space.install_readwrite_handler(addrstart, addrend, read_delegate, write_delegate, unitmask, cswidth, flags)
    space.install_readwrite_handler(addrstart, addrend, addrmask, addrmirror, addrselect, read_delegate, write_delegate, unitmask, cswidth, flags)
```

These six methods allow to install delegate-wrapped handlers in a live address space. Either plain or with mask, mirror and select.  In the read/write case both delegates must be of the same flavor (`smo` stuff) to avoid a combinatorial explosion of method types.  The `unitmask`, `cswidth` and `flags` arguments are optional.

これら6つのメソッドは、ライブアドレス空間にデリゲートでラップされたハンドラをインストールすることを可能にする。プレーンでもマスク、ミラー、セレクトを使ってもよい。 読み込み/書き込みの場合、メソッドタイプの組み合わせによる爆発を避けるために、両方のデリゲートは同じフレーバー(`smo`のもの)でなければならない。 引数の `unitmask`、`cswidth`、`flags` はオプションである。

## 5.3 Direct memory range mapping

```C++
    space.install_rom(addrstart, addrend, void *pointer)
    space.install_rom(addrstart, addrend, addrmirror, void *pointer)
    space.install_rom(addrstart, addrend, addrmirror, flags, void *pointer)
    space.install_writeonly(addrstart, addrend, void *pointer)
    space.install_writeonly(addrstart, addrend, addrmirror, void *pointer)
    space.install_writeonly(addrstart, addrend, addrmirror, flags, void *pointer)
    space.install_ram(addrstart, addrend, void *pointer)
    space.install_ram(addrstart, addrend, addrmirror, void *pointer)
    space.install_ram(addrstart, addrend, addrmirror, flags, void *pointer)
```

Installs a memory block in an address space, with or without mirror and flags.  `_rom` is read-only, `_ram` is read/write, `_writeonly` is write-only.  The pointer must be non-null, this method will not allocate the memory.

ミラーとフラグの有無にかかわらず、メモリブロックをアドレス空間にインストールする。 `_rom`は読み取り専用、`_ram`は読み取り/書き込み、`_writeonly`は書き込み専用である。 このメソッドはメモリを割り当てない。

## 5.4 Bank mapping

```C++
    space.install_read_bank(addrstart, addrend, memory_bank *bank)
    space.install_read_bank(addrstart, addrend, addrmirror, memory_bank *bank)
    space.install_read_bank(addrstart, addrend, addrmirror, flags, memory_bank *bank)
    space.install_write_bank(addrstart, addrend, memory_bank *bank)
    space.install_write_bank(addrstart, addrend, addrmirror, memory_bank *bank)
    space.install_write_bank(addrstart, addrend, addrmirror, flags, memory_bank *bank)
    space.install_readwrite_bank(addrstart, addrend, memory_bank *bank)
    space.install_readwrite_bank(addrstart, addrend, addrmirror, memory_bank *bank)
    space.install_readwrite_bank(addrstart, addrend, addrmirror, flags, memory_bank *bank)
```

Install an existing memory bank for reading, writing or both in an address space.

既存のメモリバンクをアドレス空間に読み出し、書き込み、またはその両方を行うためにインストールする。

## 5.5 Port mapping

```C++
    space.install_read_port(addrstart, addrend, const char *rtag)
    space.install_read_port(addrstart, addrend, addrmirror, const char *rtag)
    space.install_read_port(addrstart, addrend, addrmirror, flags, const char *rtag)
    space.install_write_port(addrstart, addrend, const char *wtag)
    space.install_write_port(addrstart, addrend, addrmirror, const char *wtag)
    space.install_write_port(addrstart, addrend, addrmirror, flags, const char *wtag)
    space.install_readwrite_port(addrstart, addrend, const char *rtag, const char *wtag)
    space.install_readwrite_port(addrstart, addrend, addrmirror, const char *rtag, const char *wtag)
    space.install_readwrite_port(addrstart, addrend, addrmirror, flags, const char *rtag, const char *wtag)
```

Install ports by name for reading, writing or both.

読み込み、書き込み、またはその両方のために、ポート名を指定してポートをインストールする。

## 5.6 Dropped accesses

```C++
    space.nop_read(addrstart, addrend, addrmirror, flags)
    space.nop_write(addrstart, addrend, addrmirror, flags)
    space.nop_readwrite(addrstart, addrend, addrmirror, flags)
```

Drops the accesses for a given range with an optional mirror and flags;

指定された範囲のアクセスを、オプションのミラーとフラグで削除します；


## 5.7 Unmapped accesses

```C++
    space.unmap_read(addrstart, addrend, addrmirror, flags)
    space.unmap_write(addrstart, addrend, addrmirror, flags)
    space.unmap_readwrite(addrstart, addrend, addrmirror, flags)
```

Unmaps the accesses (e.g. logs the access as unmapped) for a given range with an optional mirror and flags.

オプションのミラーとフラグで、指定された範囲のアクセスをアンマップする(例えば、アクセスをアンマップとしてログに記録する)。


## 5.8 Device map installation

```C++
    space.install_device(addrstart, addrend, device, map, unitmask, cswidth, flags)
```

Install a device address with an address map in a space.  The `unitmask`, `cswidth` and `flags` arguments are optional.

アドレスマップを持つデバイスアドレスをスペースにインストールする。 引数の `unitmask`、`cswidth`、`flags` はオプションである。


## 5.9 Contention

```C++
    using ws_time_delegate  = device_delegate<u64 (offs_t, u64)>;
    using ws_delay_delegate = device_delegate<u32 (offs_t)>;

    space.install_read_before_time(addrstart, addrend, addrmirror, ws_time_delegate)
    space.install_write_before_time(addrstart, addrend, addrmirror, ws_time_delegate)
    space.install_readwrite_before_time(addrstart, addrend, addrmirror, ws_time_delegate)

    space.install_read_before_delay(addrstart, addrend, addrmirror, ws_delay_delegate)
    space.install_write_before_delay(addrstart, addrend, addrmirror, ws_delay_delegate)
    space.install_readwrite_before_delay(addrstart, addrend, addrmirror, ws_delay_delegate)

    space.install_read_after_delay(addrstart, addrend, addrmirror, ws_delay_delegate)
    space.install_write_after_delay(addrstart, addrend, addrmirror, ws_delay_delegate)
    space.install_readwrite_after_delay(addrstart, addrend, addrmirror, ws_delay_delegate)
```

Install a contention handler in the decode path.  The addrmirror parameter is optional.

デコードパスに競合ハンドラをインストールする。 addrmirrorパラメータはオプションである。

## 5.10 View installation

```C++
    space.install_view(addrstart, addrend, view)
    space.install_view(addrstart, addrend, addrmirror, view)

    view[0].install...
```

Installs a view in a space.  This can be only done once and in only one space, and the view must not have been setup through the address map API before.  Once the view is installed, variants can be selected by indexing to call a dynamic mapping method on it.

A view can be installed into a variant of another view without issues, with only the usual constraint of single installation.

スペースにビューをインストールします。 これは一度だけ、一つのスペースにのみ行うことができ、そのビューは以前にアドレスマップAPIを通してセットアップされたものであってはならない。 ビューがインストールされると、バリアントはインデックスによって選択され、ダイナミックマッピングメソッドを呼び出すことができます。

ビューは他のビューのバリアントに問題なくインストールすることができます。

## 5.11 Taps

```C++
    using tap = std::function<void (offs_t offset, uNN &data, uNN mem_mask)

    memory_passthrough_handler mph = space.install_read_tap(addrstart, addrend, name, read_tap, &mph);
    memory_passthrough_handler mph = space.install_write_tap(addrstart, addrend, name, write_tap, &mph);
    memory_passthrough_handler mph = space.install_readwrite_tap(addrstart, addrend, name, read_tap, write_tap, &mph);

    mph.remove();
```

A tap is a method that is be called when a specific range of addresses is accessed without overriding the actual access.  Taps can change the data passed around.  A write tap happens before the access, and can change the value to be written.  A read tap happens after the access, and can change the value returned.

Taps must be of the same width and alignement than the bus.  Multiple taps can act over the same addresses.

The `memory_passthrough_handler` object collates a number of taps and allow to remove them all in one call.  The `mph` parameter is optional and a new one will be created if absent.

Taps are lost when a new handler is installed at the same addresses (under the usual principle of last one wins).  If they need to be preserved, one should install a change notifier on the address space, and remove + reinstall the taps when notified.

タップとは、実際のアクセスをオーバーライドすることなく、特定のアドレス範囲にアクセスするときに呼び出されるメソッドである。 タップは、渡されるデータを変更することができる。 書き込みタップはアクセスの前に行われ、書き込まれる値を変更することができる。 リードタップはアクセス後に起こり、返される値を変更することができる。

タップはバスと同じ幅とアラインメントでなければならない。 複数のタップが同じアドレスに対して作用することができる。

`memory_passthrough_handler` オブジェクトは複数のタップを連結し、1回の呼び出しで全てのタップを削除できるようにする。 `mph` パラメータは省略可能で、省略した場合は新しいハンドラが生成される。

新しいハンドラが同じアドレスにインストールされると、タップは失われる (最後にインストールされたものが勝つという通常の原則に従う)。 タップを保持する必要がある場合は、アドレス空間に変更通知器を設置し、通知されたときにタップを削除+再インストールする必要がある。

