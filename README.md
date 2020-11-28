# PCA9956 sample Code for Arduino
This is Arduino code sample for I2C controlled LED driver PCA9956. The chips is capable to handle 24 channels of LEDs, however there is practically **NO** sample code nor information except this rather incomplehensible [datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf). So I hope this code can help someone who are struggling now to understand this guy. I have tested this code with [Switchscience's breakout board](https://www.switch-science.com/catalog/2677/). When all three address setting pins of PCA9956 are open state, the i2c address will be 0x3F. 

i2cLEDドライバー、PCA9956のArduino用サンプルコードです。24チャンネル制御できるとても便利なICなのですが、サンプルコードが全然見つからず、[データシート](https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf)もとても分かりにくいものでしたので、ご参考になればと思います。[スイッチサイエンス製の評価ボード](https://www.switch-science.com/catalog/2677/)にて動作を確認しています。アドレス設定ピン3本すべてオープン（未接続）の場合は、Ox3Fです。


[Device data-sheet / デバイスのデータシートはこちら](https://www.switch-science.com/catalog/2677/)

[Product page of Switchscience / Switch Scienceの商品ページ](https://www.switch-science.com/catalog/2677/)


## library
PCA9965 library is now available.
[https://github.com/yuskegoto/PCA9956](https://github.com/yuskegoto/PCA9956)


## Procedure つなぎ方

Following is simplified procedure for hooking up the device.
簡単ですが以下に手順を記載します。

1．Connect the power and i2c wires / i2c及び電源を接続
   
Arduino   PCA9956
  
5V        VDD
  
GND       GND
  
SDA       SDA
  
SCL       SCL

2. Add pull up resistors between Vcc and each i2c wires (SDA / SCL). Please don't forget pull-up regestors!

   i2c用プルアップ抵抗をドライバ側のSDA SCLとVCC間に接続（スイッチサイエンス製の基板にはプルアップ抵抗がついていないので注意！4.7kOhmとかでOK）

3. Upload your sketch!

   スケッチをアップロード

## Resistor Setting レジスタの設定

As you can see on the code, I did only on/off and pwm control. Group dimming is also possible, even I haven't implemented yet. The resistor setting precedure for On/Off control is like this.

コードを見て頂ければわかりますが、今回はON/OFF制御とPWMのみ実装しております。ただし実装していない機能としてグループ調光等も可能です。ON/OFF制御の場合の設定手順は以下となります。

1. Set up MODE1(Ox00) resistor

   MODE1(0x00)レジスタを設定
   
2. Set up output mode setting LEDOUT0-5(0x02 - 0x7). One resistor has 4 LED's control state. Therefore one LED has two bits of control state... See the datasheet page 17. 

   LEDOUT0-5(0x02 - 0x7) ドライバ出力状態設定。1レジスタにつき4LEDの制御状態が格納される。そのため1LEDにつき2ビット制御フラグが存在する。データシート17ページ参照
   
    00: LED driver is off
    
    01: LED driver is fully on
    
    10: LED driver can be indivisually controlled from PWMx resistor or PWMALL resistor for all control
    
    LEDドライバーがPWMxレジスターまたはPWMALLレジスターより制御可能
    
    11: LED driver's individual brightness and group's dimming/blinking can be controlled from PWMx or GRPPWM resistors
    LEDドライバーの輝度及びグループの点滅・ディミングがPWMxレジスターまたはGRPPWMレジスターより制御可能   
    
3. IREF0 - 23(0x22 - 0x39) out put current control　/ 出力電流設定

This procedure is **not written cleary** on the datasheet. So I guess there are bunch of folks giving up before finding this procedure.

この手順はデータシートの**どこにも**書いておらずここにたどり着く前に挫折してしまう人が多いようです。

Actually I found this procedure from a comment I found on Amazon.jp's product page. Thanks! 

ちなみに私はアマゾンの評価に載っていたRaspiで設定した人の[コメント](https://www.amazon.co.jp/gp/aw/reviews/B01FJHF760)で手順をようやく理解しました（ありがとうございます！）。
