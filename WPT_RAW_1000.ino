#include "si5351.h"
#include "Wire.h"

unsigned long long freq = 5000000ULL;//出力周波数50kHz（これをいじって周波数を変える）freq*0.01=周波数Hz
unsigned long long pll_freq = 70500000000ULL;//PLL周波数（いじるな）

Si5351 si5351;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);//通信速度
  pinMode(A0,INPUT);//アナログピン設定
  pinMode(A1,INPUT);//アナログピン設定
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);//振動子負荷容量（使うモジュールが8pFなのでこれ）
  
  //unsigned long long freq = 5000000ULL;//出力周波数50kHz（これをいじって周波数を変える）freq*0.01=周波数Hz
  //unsigned long long pll_freq = 70500000000ULL;//PLL周波数（いじるな）

  si5351.set_freq_manual(freq, pll_freq, SI5351_CLK0);//出力周波数,PLL周波数,設定先出力ピン設定
  
  si5351.set_phase(SI5351_CLK0, 0);//位相（今回特に意味はない）
  si5351.pll_reset(SI5351_PLLA);//PLLをリセット（使う前に一回リセット）

  si5351.update_status();//si5351のステータスを読む（今回特に使っていない）
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long intfreq;
  double effectance,efraw,a0,a1,w1,w2;
  int i;
  unsigned long long freq = 5000000ULL;//出力周波数50kHz
  //ここから周波数自動探索を書いておく（freqをインクリメントしていくだけ）
  Serial.println("Wat1,Wat2 | Wat1,Wat2 | ...Ten times");
  freq = 500000;//探索開始周波数（下限）
  Serial.println("start");
  while(freq<=10000000){//探索終了周波（上限）
    si5351.set_freq_manual(freq,pll_freq, SI5351_CLK0);//周波数設定更新
    si5351.pll_reset(SI5351_PLLA);//PLLをリセット（一応）
    
    delay(3000);
    for(i=0;i<100;i++){
    a0 = analogRead(A0);
    a1 = analogRead(A1);
    w1 = (( a0 / 1023) * 5);
    w2 = (( a1 / 1023) * 5);
    efraw = w2 / w1;
    effectance = efraw * 100;
    intfreq=freq*0.01;
    Serial.print(w1,4);
    Serial.print(", ");
    Serial.print(w2,4);
    Serial.print(", ");
    Serial.print(effectance);
    Serial.print(" | ");
    }
    //Serial.println(" Hz");
    Serial.print(intfreq);
    Serial.println("");
    freq = freq+10000;
    }
  Serial.print("finish");

  while(true);//永久ループ（終了）
}
