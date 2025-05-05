#define kizil_ileri A1
#define kizil_sag A2
#define kizil_sol A3
#define kizil_geri A4

const int ena = 2; // Sağ Motor Hız 
const int enb = 3; // Sol Motor Hız
const int ina = 4; // Sağ Motor 1 
const int inb = 5; // Sağ Motor 2
const int inc = 6; // Sol Motor 1
const int ind = 7; // Sol Motor 2
const int sagSensor = 8; // Sol Sensör
const int solSensor = 9; // Sağ Sensör
const int uzaklikSensor = 10; // Uzaklık Sensörü

const int aci = 2;
const int max_uzaklik = 77;
const int minimumMesafe = 2;
const float HizSureKatsayi = 3.0;

bool sensorConnected = false;

int yon;
bool orta;
bool bulundu = false;
int state = 0; // Basla -> 0, Ara -> 1, Saldir -> 2, Geri Çekil -> 3
bool rakip_yon = false; // false -> Sağ, true -> Sol

// Sensörler
bool sag;
bool sol;
int dist;

int iena=1;
int gena=1;
int saena=0;
int soena=0;

bool durmak=1;
bool ilerlemek=0;
bool solmak=0;
bool sagmak=0;

bool sagda;
bool solda;
bool dibinde;
bool ortada;

bool problem;

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
  // sensor.setTimeout(500); // sensor diye bir şey yok

  if (!mes.init()) {
    Serial.println("VL53L1X sensörü başlatılamadı!");
    while (1);
  }
  mes.setDistanceMode(VL53L1X::Long);
  mes.setMeasurementTimingBudget(50000); // 50 ms (maks. hassasiyet)
  mes.startContinuous(50); // 50 ms'de bir okuma
  //mes2=mes.read()/10; // tekrar setup yazılsın
}

void loop() { 
  switch(state){
    case 0:
      basla();
      break;
    case 1:
      ara();
      break;
    case 2:
      saldir();
      break;
    case 3:
      geriCekil();
      break;
    default:
      state = 1;
      break;
  }
}

/*
  -> ALGORİTMA KODLARI
*/

void basla(){
  delay(1500); // 1,5 Saniye Bekle
}

void ara(){
  if(bulundu){
    state = 2;
  }
  else{
    sensorOku();
    if(dist > minimumMesafe){
      bulundu = true;
    }
    else if(sag){
      sagGit(1);
    }
    else if(sol){
      solGit(1);
    }
    else{
      switch(rakip_yon){
        case false:
          sagGit(1);
          break;
        case true:
          solGit(1);
          break;
      }
    }
  }
}

void saldir(){
  sensorOku();
  int sonGidis = 1200;
  if(dist > minimumMesafe){
    float t = dist*HizSureKatsayi;
    while(t > 0){
      ileri(1);
      sensorOku();
      if(sag && sol){
        ileri(sonGidis);
        dur();
        state = 3;
        break;
      }
    } 
  }
  else{
    state = 1;
    dur();
  }
}

void geriCekil(){
  geri(500); // Yarım Saniye, Daha Sonra Değiştirilecek
  state = 1;
}

/*
  -> DONANIM KODLARI
*/

// Motor Kodları
// Verilen Süre Milisaniye Cinsinden Olmalı
void ileri(int x){
  while(x > 0){
    sensorOku();
    if(iena || gena || saena || soena){
      dur();
      break;
    }
    digitalWrite(ena, 1);
    digitalWrite(enb, 1);

    digitalWrite(ina, 1);
    digitalWrite(inb, 0);

    digitalWrite(inc, 1);
    digitalWrite(ind, 0);
    x--;
  }
}

void geri(int x){
  while(x > 0){
    sensorOku();
    if(iena || gena || saena || soena){
      dur();
      break;
    }
    digitalWrite(ena, 1);
    digitalWrite(enb, 1);

    digitalWrite(ina, 0);
    digitalWrite(inb, 1);

    digitalWrite(inc, 0);
    digitalWrite(ind, 1);
    x--;
  }
}

int aciSureHesaplayici(int x){
  return x * aci;
}

// x Derece Açı Cinsinden Açı Olacak
void sagGit(int x){
  int t = aciSureHesaplayici(x);
  while(t > 0){
    sensorOku();
    if(iena || gena || saena || soena){
      dur();
      break;
    }
    digitalWrite(ena, 1);
    digitalWrite(enb, 1);

    digitalWrite(ina, 0);
    digitalWrite(inb, 1);

    digitalWrite(inc, 1);
    digitalWrite(ind, 0);
    x--;
  }
}

void solGit(int x){
  int t = aciSureHesaplayici(x);
  while(t > 0){
    sensorOku();
    if(iena || gena || saena || soena){
      dur();
      break;
    }
    digitalWrite(ena, 1);
    digitalWrite(enb, 1);

    digitalWrite(ina, 1);
    digitalWrite(inb, 0);

    digitalWrite(inc, 0);
    digitalWrite(ind, 1);
    x--;
  }
}

void dur(){
  mpuOku();
  switch(yon){
    case 1:
      geri(velocityY * 5); // Statik Ayarlama, Burası Değiştirlecek 
      break;
    case -1:
      ileri(velocityY * 5); // Statik Ayarlama, Burası Değiştirlecek 
      break;
    default:
      digitalWrite(ena, 0);
      digitalWrite(enb, 0);

      digitalWrite(ina, 0);
      digitalWrite(inb, 0);

      digitalWrite(inc, 0);
      digitalWrite(ind, 0);
      break;
  }
}

// Sensör Okuma Kodları
void kizilKontrol(){
  iena=(kizil_ileri>1000) ? 0 : 1;
  gena=(kizil_geri>1000) ? 0 : 1;
  saena=(kizil_sag>1000) ? 0 : 1;
  soena=(kizil_sol>1000) ? 0: 1;
}

void sagKontrol(){
  sag = digitalRead(sagSensor) == 0 ? 0 : 1;
  rakip_yon = sagGit == 1 ? false : rakip_yon;
}

void solKontrol(){
  sol = digitalRead(solSensor) == 0 ? 0 : 1;
  rakip_yon = sol == 1 ? true : rakip_yon;
}

// Uzaklık Hesaplayıcı Maksimum Sınır Ötesinde Ne Döner? Ona Göre Tekrar Elden Geçir
void uzaklikBak(){
  dist = analogRead(uzaklikSensor) > max_uzaklik ? max_uzaklik : analogRead(uzaklikSensor);
}

void sensorOku(){
  kizilKontrol();
  sagKontrol();
  solKontrol();
  uzaklikBak();
}
// Sensör Okuma Kodları

int previousVelocityX = 0;

void mpuOku(){
  if (!mpu.testConnection()) {
    sensorConnected = false;
    Serial.println("MPU bağlantı kaybedildi. Yeniden bağlanmayı deniyor...");
    reconnectMPU();
    delay(500);
    return;
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

  float fark = velocityX - previousVelocityX;
  float diff = fark > 0 ? fark : -1*fark;

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

int lastReconnectAttempt = 0;

void reconnectMPU() {
  unsigned long now = millis();
  if (now - lastReconnectAttempt > 10) {  // 10 milisaniyede bir tekrar deniyor :-|
    Serial.println("Yeniden bağlanmayı deniyor...");
    sensorConnected = initializeMPU();
    lastReconnectAttempt = now;
  }
}