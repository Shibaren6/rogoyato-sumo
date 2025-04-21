int enA = 2;  // Hız kontrolü için PWM pini
int in1 = 4;  // Yön kontrolü pini
int in2 = 5;  // Yön kontrolü pini
int enB=3;
int in3=6;
int in4=7;

int echo1=8;
int triq=9;
int echo2=11;
int echo3=12;
int echo4=13;
int echo5=A4

int fk = A0;
int rk = A1;
int lk = A2;
int bk = A3;

int ele=10;

int ik = 0;
int sak = 0;
int sok = 0;
int gk = 0;

int ik2=0;
int sak2=0;
int sok2=0;
int gk2=0;

int fd = 0;
int rd = 0;
int ld = 0;
int bd = 0;

long duration1;
long duration2;
long duration3;
long duration4;
long duration5;
float distancm1;
float distancm2;
float distancm3;
float distancm4;
float distancm5;

int es=20;

int hiz=255;

int yon;

int yonel;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(triq,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(echo2,INPUT);
  pinMode(echo3,INPUT);
  pinMode(echo4,INPUT);
  pinMode(echo5,INPUT);
  pinMode(ele,OUTPUT);
  pinMode(fr,INPUT);
  pinMode(fl,INPUT);
  pinMode(br,INPUT);
  pinMode(bl,INPUT);
  digitalWrite(triq,LOW);
  Serial.begin(9600);
}

// Motoru belirli hızda ileri-geri döndürme
void loop() {

  digitalWrite(ele,HIGH);
  ik2=ik;
  sak2=sak;
  sok2=sok;
  gk=gk2;
  ik = analogRead(fk);
  sak = analogRead(rk);
  sok = analogRead(lk);
  gk = analogRead(bk);
  
  if (ik2-ik > es) {
    fd = 1;  // Siyah
  } else {
    fd = 0;  // Beyaz
  }

  if (sak2-sak > es) {
    rd = 1;  // Siyah
  } else {
    rd = 0;  // Beyaz
  }

  if (sok2-sok > es) {
    ld = 1;  // Siyah
  } else {
    ld = 0;  // Beyaz
  }

  if (gk2-gk > es) {
    bd = 1;  // Siyah
  } else {
    bd = 0;  // Beyaz
  }
 // Yön bilgisi için kontrol


  digitalWrite(triq,LOW);
  delayMicroseconds(2);
  digitalWrite(triq,HIGH);
  delayMicroseconds(10);
  digitalWrite(triq,LOW);
  duration1=pulseIn(echo1,HIGH);
  duration2=pulseIn(echo2,HIGH);
  duration3=pulseIn(echo3,HIGH);
  duration4=pulseIn(echo4,HIGH);
  duration5=pulseIn(echo5,HIGH);
  distancm1=(duration1 / 2.0) * 0.0343;
  distancm2=(duration2 / 2.0) * 0.0343;
  distancm3=(duration3 / 2.0) * 0.0343;
  distancm4=(duration4 / 2.0) * 0.0343;
  distancm5=(duration5 / 2.0) * 0.0343;

  if(((distancm1<77 && distancm2<77 && distancm3<77) || (distancm1<77 && distancm2>77 && distancm3>77 )) && (fd!=1 || sak!=1 || sok!=1 || bk!=1) && distancm4>77 && distancm5>77){
    ig(hiz);
    yonel=0;
    yon=0;
  }
  else if(distancm2<77 && distancm3>77){
    yon=1;
    sog(255);
    yonel=1;
    delay(100);
    durs();
  }
  else if(distancm2>77 && distancm3<77){
    yon=2;
    sag(255);
    yonel=2;
    delay(100);
    durs()
  }
  else if(distancm4<77){
    //kon=1;
    if(yonel!=1){
      kacin(255, 1);
    }
    else{
      sog(hiz);
      delay(200);
      durs();
    }
  }
  else if(distancm5<77){
    //kon=2;
    if(yonel!=2){
      kacin(255, 2);
    }
    else{
      sag(hiz);
      delay(200);
      durs();
    }
  }
  else if(fd==1 && sak==1 && sok==1 && bk==1){
  }
  else if (fd == 1) {
    durh(255);
    sag(255);
    delay(100);
    ig(255);
    delay(100);
    durs();
  }
   else if (rd == 1) {
    durh(255);
    sog(255);
    delay(100);
    ig(255);
    delay(100);
    durs();
  }
  else if (ld == 1) {
    durh(255);
    sag(255);
    delay(100);
    ig(255);
    delay(100);
    durs();
  }
  else if (bd == 1) {
    ig(255);
    delay(100);
    durs();
  }
  else if(distancm1>77 && distancm3<77){
    while(distancm1<77){
      sag(255);
    }
  }
  else if(distancm1>77 && distancm2<77){
    while(distancm1<77){
      sog(255);
    }
  }
  else if(distancm1 >77 && distancm2>77 && distancm3>77){
    if(yon==1){
      while(distancm2>77 || distancm3>77 || distancm1>77){
        sog(hiz);
      }
    }
    else{
      while(distancm2>77 || distancm3>77 || distancm1 >77){
        sag(hiz);
      }
    }
    else if(distancm1<77 && distancm2<77 && distancm3<77){
      carp();
    }
  }
  
  














  Serial.print("ileri sol: ");
  Serial.println(isol);
  Serial.print("ileri sağ: ");
  Serial.println(isag);
  Serial.print("geri sol: ");
  Serial.println(gsol);
  Serial.print("geri sağ: ");
  Serial.println(gsag);
  delay(500);
  



}
void ig(int hiz) {
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void gg(int hiz) {
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void sag(int hiz){
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void sog(int hiz){
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void durs() { // dikkat et!
  analogWrite(enA,0);
  analogWrite(enB,0);
  digitalWrite(in1,LOW);
  digitalWrite(in1,LOW);
  digitalWrite(in1,LOW);
  digitalWrite(in1,LOW);
}
void durh(hiz){ //burası ile oynayacaksın, direnme kısmı ile aynı olabilir.
  analogWrite(enA,hiz);
  analogWrite(enB,hiz);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}
void diren(){ //buraya rakibe hangi yönde direndiğini algılamak için bir sistem bulup eklemen lazım.

}
void carp(){
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void kacin(hiz, kon){ //Dikkat et! Test ET!!!
  if(kon=2){
    sag(hiz);
    delay(100);
    ig(hiz);
    delay(100);
    float zam=hiz;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    while(zam>2){
      analogWrite(enA,zam);
      analogWrite(enB,hiz);
      delay(1);
      zam=zam-1;
    }
    while(zam<hiz){
      analogWrite(enA,hiz);
      analogWrite(enB,zam);
      zam=zam+hiz/zam;
    }
  }
  if(kon=1){
    sog(hiz);
    delay(100);
    ig(hiz);
    delay(100);
    float zam=hiz;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    while(zam>2){
      analogWrite(enB,zam);
      analogWrite(enA,hiz);
      delay(1);
      zam=zam-1;
    }
    while(zam<hiz){
      analogWrite(enB,hiz);
      analogWrite(enA,zam);
      zam=zam+hiz/zam;
    }
  }
}