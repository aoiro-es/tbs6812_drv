## tbs6812_drv

ISDB-T/ISDB-S/ISDB-S3対応Linux向けTBS6812 a.k.a. PT4Kドライバ

カーネルモジュールのビルド環境を構築後、次のようにしてビルド (例):

```sh
make -C "/lib/modules/$(uname -r)/build" "M=$(pwd)" tbs6812.ko cxd2857.ko
```

ロード:

```sh
modprobe dvb-core
insmod ./cxd2857.ko
insmod ./tbs6812.ko
```

DVBデバイスとして見えるのでdvb-toolsなどを導入するとストリームを得ることができます。

```sh
dvbv5-zap -a 0 -c default-channels.conf defaultTLV -r -P -o defaultTLV.mmts
dvbv5-zap -a 0 -c default-channels.conf defaultTS -r -P -o defaultTS.ts
```

### 仕様
ISDB-SおよびISDB-S3のストリームID及びTSIDは16ビットですが、DVBドライバのインターフェースの`STREAM_ID`は32ビットです。
この`STREAM_ID`の上位16ビットで動作が変わります。

- `STREAM_ID`の上位16ビットが`0x0000`のとき
  - `DELIVERY_SYSTEM = ISDBS`かつ`STREAM_ID`の下位16ビットが8未満のときISDB-Sの相対TS番号で選局されます
  - `DELIVERY_SYSTEM = ISDBS`かつ`STREAM_ID`の下位16ビットのうち、上位4ビットが0xbまたは0xcのときISDB-S3のストリームIDで選局、それ以外のときISDB-SのTSIDで選局されます。

- `STREAM_ID`の上位16ビットが`0x0001`のとき
  - `DELIVERY_SYSTEM = ISDBS`かつ`STREAM_ID`の下位16ビットが8未満のときISDB-Sの相対TS番号で選局されます。そうでない場合は`STREAM_ID`をそのままISDB-SのTSIDとして選局します。

- `STREAM_ID`の上位16ビットが`0x0003`のとき
  - `DELIVERY_SYSTEM = ISDBS`かつ`STREAM_ID`の下位16ビットが16未満のときISDB-Sの相対TS番号で選局されます。そうでない場合は`STREAM_ID`をそのままISDB-S3のストリームIDとして選局します。
