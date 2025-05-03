const int ena 2;
const int enb 3;
const int ina 4;
const int inb 5;
const int inc 6;
const int ind 7;
const int mes1 8;
const int mes2 9;
const int mes3 10;
#define kizi A1
#define kizsa A2
#define kizso A3
#define kizg A4

int yon;
bool orta;
bool sag;
bool sol;

int dist;

int iena=1;
int gena=1;

bool durmak=1;
bool ilerlemek=0;
bool solmak=0;
bool sagmak=0;

bool sagda;
bool solda;
bool dibinde;
bool ortada;

int bizyon=1;

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

unsigned long lastTime = 0;
float velocityX = 0;
float velocityY = 0;

#include <VL53L1X.h>

VL53L1X mes;

void setup() { //// SETUUUPP!!!
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 bağlantı başarısız!");
    while (1);
  }

  lastTime = millis();

  delay(10);
  sensor.setTimeout(500);

  if (!mes.init()) {
    Serial.println("VL53L1X sensörü başlatılamadı!");
    while (1);
  }
  mes.setDistanceMode(VL53L1X::Long);
  mes.setMeasurementTimingBudget(50000); // 50 ms (maks. hassasiyet)
  mes.startContinuous(50); // 50 ms'de bir okuma
  mes2=mes.read()/10;
}

void loop() { ///// LOOPPP!!!!

}

void ara(){
  switch (yon){
    case 0:
    sol=0;
    sag=0;
    yon=0;
    orta=1;
    break;
    case 1:
    while(mes2>55){
      sol();
    }
    sol=0;
    sag=0;
    yon=0;
    orta=1;
    break;
    case 2:
    while(mes2>55){
      sag();
    }
    sol=0;
    sag=0;
    yon=0;
    orta=1;
    break;
    default:
    while(mes2>52){
      sol();
    }
    sol=0;
    sag=0;
    orta=1;
    yon=0;
  }
}
void ileri(int x){
  if(iena){
      dur();
      sol(180);
  }
  else{
    while(x>0){
      analogWrite(ena,255);
      analogWrite(enb,255);
      digitalWrite(ina,HIGH);
      digitalWrite(inb,LOW);
      digitalWrite(inc,HIGH);
      digitalWrite(ind,LOW);
      x--;
      durmak=0;
    }
  }
}
void geri(){
  if(gena){
      dur();
      sol(180);
  }
  else{
    while(x>0){
      analogWrite(ena,255);
      analogWrite(enb,255);
      digitalWrite(ina,LOW);
      digitalWrite(inb,HIGH);
      digitalWrite(inc,LOW);
      digitalWrite(ind,HIGH);
      x--;
      durmak=0;
    }
  }
}
void sag(int x){
  while(x>0){
    analogWrite(ena,255);
    analogWrite(enb,255);
    digitalWrite(ina,LOW);
    digitalWrite(inb,HIGH);
    digitalWrite(inc,HIGH);
    digitalWrite(ind,LOW);
    delay(1); // aci*x; x.k, k=>delay(t):t*k, 1:k?;
    x--;
    durmak=0;
  }
}
void sol(int x){
  while(x>0){
    analogWrite(ena,255);
    analogWrite(enb,255);
    digitalWrite(ina,HIGH);
    digitalWrite(inb,LOW);
    digitalWrite(inc,LOW);
    digitalWrite(ind,HIGH);
    delay(1);
    x--;
    durmak=0;
  }
}
void isayay(){

}
void isoyay(){

}
void gsayay(){

}
void gsoyay(){

}
void tursa(){
  sag(90);
  ileri(1000);
  if(iena){
    ileri(1)
  }
  else{
    sag(15);
  }
  if(saena){
    ileri(1);
  }
  else{
    sag(5);
  }
}
void turso(){
  sol(90);
  ileri(1000);
  if(iena){
    ileri(1)
  }
  else{
    sol(15);
  }
  if(soena){
    ileri(1);
  }
  else{
    sol(5);
  }
}
int yon(){
  if(mes<52){
    orta=1;
  }
  sag=digitalRead(mes3);
  sol=digitalRead(mes1);
  sagda=sag && !orta;
  solda=sol && !orta;
  dibinde=sag && sol;
  ortada=orta || (sag && sol) || (orta && (sag || sol));
  orta=!(sagda || solda);
  if(!sagda){
    if(!solda){
      yon=0;
    }
    else{
      yon=1;
    }
  }
  else{
    yon=2;
  }

  return yon;
}
void bizyon(){

}
void dur(){
  switch(bizyon){
    case 0:
    ileri(5);
    analogWrite(ena,0);
    analogWrite(enb,0);
    break;
    case 1:
    analogWrite(ena,0);
    analogWrite(enb,0);
    break;
    case 2:
    geri(5);
    analogWrite(ena,0);
    analogWrite(enb,0);
    break;
  }
  durmak=1;
}
void diren(){ //diren yerine bir yere kaçmayı da yapabiliriz :-}
  if(durmak && problem){//bu if aslında, ya bilmiyorum bir sıkıntı var loop'un içinde sürekli test etmemiz gerekiyor!
    switch(bizyon){
      case 0: //ileri itiliyoruz, düşman arkada, sıkıntı!! tekeri yere değdir dön ve kaç!!
      ileri(5);
      sag(90);
      ileri(15);
      break;
      case 2: //önden itiyor, ilerii!!!; veya bir dön felan bir şeyler yap, bence dönmek mantıklı olur
      ileri(15);
      break;
    }
  }
}
void kiz(){
// kizi=kiz1>1000;
// kizsa=kiz2>1000;
// kizso=kiz3>1000;
// kizg=kiz4>1000;
iena=(kizi>1000) ? 0 : 1;
gena=(kizg>1000) ? 0 : 1;
saena=(kizsa>1000) ? 0 : 1;
soena=(kizso>1000) ? 0: 1;
}





void mpu(){
  if (!mpu.testConnection()) {
    sensorConnected = false;
    Serial.println("MPU bağlantı kaybedildi. Yeniden bağlanmayı deniyor...");
    reconnectMPU();
    delay(500);
    return;  // veri işlemeyi atla
  }

  sensorConnected = true;
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az); // raw değerler

  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // saniye cinsinden delta t
  lastTime = currentTime;

  // MPU6050 scale: 1g ≈ 16384
  float accelX = ax / 16384.0; // g cinsinden
  float accelY = ay / 16384.0;

  // g → m/s² dönüşümü
  accelX *= 9.81;
  accelY *= 9.81;

  // Basit entegrasyon (ivme * zaman)
  velocityX += accelX * dt;
  velocityY += accelY * dt;

  // Basit gürültü filtreleme (drift önleme)
  if (abs(accelX) < 0.1) velocityX = 0;
  if (abs(accelY) < 0.1) velocityY = 0;

  float diff = abs(velocityX - previousVelocityX);

  if (diff > 0.03) {
    problem = true;
    yon = (velocityX - previousVelocityX > 0) ? 1 : -1;
  } else {
    problem = false;
    yon = 10;
  }
  previousVelocityX = velocityX;
}

bool initializeMPU() {
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU bağlantısı başarılı.");
    return true;
  } else {
    Serial.println("MPU bağlantı hatası!");
    return false;
  }
}
void reconnectMPU() {
  unsigned long now = millis();
  if (now - lastReconnectAttempt > 10) {  // 10 milisaniyede bir tekrar deniyor :-|
    Serial.println("Yeniden bağlanmayı deniyor...");
    sensorConnected = initializeMPU();
    lastReconnectAttempt = now;
  }
}