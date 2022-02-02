#include <AntaresESP8266HTTP.h>
#include <DHT.h>
//---------------------------------------------------
#define ACCESSKEY "b959908289b08ce9:1803c49f1064b664"
#define WIFISSID "Kumbung"
#define PASSWORD "kumbung1"
#define projectName "KJC"
#define deviceName "DHT22"
AntaresESP8266HTTP antares(ACCESSKEY);
//---------------------------------------------------
#define DHTPIN 14 //D0
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//---------------------------------------------------
#define ensprayer 5 //D1
#define sprayer 4 //D2
#define fan 0 //D3
#define enfan 2 //D4
#define LED 16 //D5

int hum;
int temp;
int matic;
int manual;
int sprayerON;
int sprayerOFF;
int fanON;
int fanOFF;

//Deklarasi Variabel Inferensi
float Fan_rule1;
float Spray_rule1;
float Fan_rule2a;
float Fan_rule2b;
float Spray_rule2;
float Fan_rule3a;
float Fan_rule3b;
float Spray_rule3;
float Fan_rule4a;
float Fan_rule4b;
float Spray_rule4a;
float Spray_rule4b;
float Fan_rule5a;
float Fan_rule5b;
float Spray_rule5;
float Fan_rule6a;
float Fan_rule6b;
float Spray_rule6a;
float Spray_rule6b;
float Fan_rule7a;
float Fan_rule7b;
float Spray_rule7a;
float Spray_rule7b;
float Fan_rule8a;
float Fan_rule8b;
float Spray_rule8a;
float Spray_rule8b;
float Fan_rule9;
float Spray_rule9;

float Ne1;
float Ze1;
float Pe1;
float Nde1;
float Zde1;
float Pde1;
float Ne2;
float Ze2;
float Pe2;
float Nde2;
float Zde2;
float Pde2;
float Ne3;
float Ze3;
float Pe3;
float Nde3;
float Zde3;
float Pde3;

//Deklarasi Variabel Fuzzifikasi
float Ne;
float Ze;
float Pe;
float Nde;
float Zde;
float Pde;
float z_Fan = 0;
float z_Spray = 0;

float pwm_Fan = 0;
float pwm_Spray = 0;

float Nilai_error;
float Nilai_error1;
float Nilai_deltaError;

//fuzzifikasi error
unsigned char error_Ne(){
  Nilai_error = dht.readHumidity();
  if (Nilai_error<=-15){Ne=1;}
  else if (Nilai_error>=-15 && Nilai_error<=0){Ne=(0-Nilai_error)/15;}
  else if (Nilai_error>=0){Ne=0;}
  return Ne;
  Nilai_error1 = Nilai_error;
}
unsigned char error_Ze(){
  Nilai_error = dht.readHumidity();
  if (Nilai_error<=-15 || Nilai_error>=15){Ze =0;}
  else if (Nilai_error>=-15 && Nilai_error<=0){Ze=(Nilai_error+15)/15;}
  else if (Nilai_error>=0 && Nilai_error<=15){Ze=(15-Nilai_error)/15;}
  return Ze;
  Nilai_error1 = Nilai_error;
}
unsigned char error_Pe(){
  Nilai_error = dht.readHumidity();
  if (Nilai_error<=0){Pe =0;}
  else if (Nilai_error>=0 && Nilai_error<=70){Pe=(Nilai_error-0)/70;}
  else if (Nilai_error>=70){Pe =1;}
  return Pe;
  Nilai_error1 = Nilai_error;
}

//fuzzifikasi deltaError
unsigned char deltaError_Nde(){
  Nilai_deltaError = Nilai_error - Nilai_error1;
  if (Nilai_deltaError<=-50){Nde=1;}
  else if (Nilai_deltaError>=-50 && Nilai_deltaError<=0){Nde=(0-Nilai_deltaError)/50;}
  else if (Nilai_deltaError>=0){Nde=0;}
  return Nde;
}
unsigned char deltaError_Zde(){
  Nilai_deltaError = Nilai_error - Nilai_error1;
  if (Nilai_deltaError<=-50 || Nilai_deltaError>=50){Zde =0;}
  else if (Nilai_deltaError>=-50 && Nilai_deltaError<=0){Zde=(Nilai_deltaError+50)/50;}
  else if (Nilai_deltaError>=0 && Nilai_deltaError<=50){Zde=(50-Nilai_deltaError)/50;}
  return Zde;
}
unsigned char deltaError_Pde(){
  Nilai_deltaError = Nilai_error - Nilai_error1;
  if (Nilai_deltaError<=0){Pde =0;}
  else if (Nilai_deltaError>=0 && Nilai_deltaError<=50){Pde=(Nilai_deltaError-0)/50;}
  else if (Nilai_deltaError>=50){Pde =1;}
  return Pde;
}

void fuzzifikasi (){
  error_Ne();
  error_Ze();
  error_Pe();
  deltaError_Nde();
  deltaError_Zde();
  deltaError_Pde();
}

void fuzzyrule(){

  fuzzifikasi();

  //-------------------Inferensi-----------------------
  //1. jika Ne dan Nde maka Spray Stop dan Fan S.Cepat
  if(Ne<=Nde && Ne!=0 && Nde!=0){
    Fan_rule1 = 190 + (Ne*40);
    Spray_rule1 = 0;
    Ne1 = Ne;
    Nde1 = 0;
  }
  else if(Nde<=Ne && Ne!=0 && Nde!=0){
    Fan_rule1 = 190 + (Nde*40);
    Spray_rule1 = 0;
    Nde1 = Nde;
    Ne1 = 0;
  }
  //2. jika Ne dan Zde maka Spray Stop dan Fan Cepat
  if(Ne<=Zde && Ne!=0 && Zde!=0){
    Fan_rule2a = 140 + (Ne*40);
    Fan_rule2b = 220 - (Ne*40);
    Spray_rule2 = 0;
    Ne2 = Ne;
    Zde1 = 0;
  }
  else if(Zde<=Ne && Ne!=0 && Zde!=0){
    Fan_rule2a = 140 + (Zde*40);
    Fan_rule2b = 220 - (Zde*40);
    Spray_rule2 = 0;
    Zde1 = Zde;
    Ne2 = 0;
  }
  //3. jika Ne dan Pde maka Spray Stop dan Fan Sedang
  if(Ne<=Pde && Ne!=0 && Pde!=0){
    Fan_rule3a = 90 + (Ne*40);
    Fan_rule3b = 170 - (Ne*40);
    Spray_rule3 = 0;
    Ne3 = Ne;
    Pde1 = 0;
  }
  else if(Pde<=Ne && Ne!=0 && Pde!=0){
    Fan_rule3a = 90 + (Pde*40);
    Fan_rule3b = 170 - (Pde*40);
    Spray_rule3 = 0;
    Pde1 = Pde;
    Ne3 = 0;
  }
  //4. jika Ze dan Nde maka Spray Pelan dan Fan Lambat
  if(Ze<=Nde && Ze!=0 && Nde!=0){
    Fan_rule4a = 40 + (Ze*40);
    Fan_rule4b = 80 - (Ze*40);
    Spray_rule4a = 90 + (Ze*45);
    Spray_rule4b = 180 - (Ze*45);
    Ze1 = Ze;
    Nde2 = 0;
  }
  else if(Nde<=Ze && Ze!=0 && Nde!=0){
    Fan_rule4a = 40 + (Nde*40);
    Fan_rule4b = 80 - (Nde*40);
    Spray_rule4a = 90 + (Nde*45);
    Spray_rule4b = 180 - (Nde*45);
    Nde2 = Nde;
    Ze1 = 0;
  }
  //5. jika Ze dan Zde maka Spray Stop dan Fan Sedang
  if(Ze<=Zde && Ze!=0 && Zde!=0){
    Fan_rule5a = 90 + (Ze*40);
    Fan_rule5b = 170 - (Ze*40);
    Spray_rule5 = 0;
    Ze2 = Ze;
    Zde2 = 0;
  }
  else if(Zde<=Ze && Ze!=0 && Zde!=0){
    Fan_rule5a = 90 + (Zde*40);
    Fan_rule5b = 170 - (Zde*40);
    Spray_rule5 = 0;
    Zde2 = Zde;
    Ze2 = 0;
  }
  //6. jika Ze dan Pde maka Spray Pelan dan Fan Cepat
  if(Ze<=Pde && Ze!=0 && Pde!=0){
    Fan_rule6a = 140 + (Ze*40);
    Fan_rule6b = 220 - (Ze*40);
    Spray_rule6a = 90 + (Ze*45);
    Spray_rule6b = 180 - (Ze*45);
    Ze3 = Ze;
    Pde2 = 0;
  }
  else if(Pde<=Ze && Ze!=0 && Pde!=0){
    Fan_rule6a = 140 + (Pde*40);
    Fan_rule6b = 220 - (Pde*40);
    Spray_rule6a = 90 + (Pde*45);
    Spray_rule6b = 180 - (Pde*45);
    Pde2 = Pde;
    Ze3 = 0;
  }
  //7. jika Pe dan Nde maka Spray Pelan dan Fan Sedang
  if(Pe<=Nde && Pe!=0 && Nde!=0){
    Fan_rule7a = 90 + (Pe*40);
    Fan_rule7b = 170 - (Pe*40);
    Spray_rule7a = 90 + (Pe*45);
    Spray_rule7b = 180 - (Pe*45);
    Pe1 = Pe;
    Nde3 = 0;
  }
  else if(Nde<=Pe && Pe!=0 && Nde!=0){
    Fan_rule7a = 90 + (Nde*40);
    Fan_rule7b = 170 - (Nde*40);
    Spray_rule7a = 90 + (Nde*45);
    Spray_rule7b = 180 - (Nde*45);
    Nde3 = Nde;
    Pe1 = 0;
  }
  //8. jika Pe dan Zde maka Spray Cepat dan Fan Lambat
  if(Pe<=Zde && Pe!=0 && Zde!=0){
    Fan_rule8a = 40 + (Pe*40);
    Fan_rule8b = 80 - (Pe*40);
    Spray_rule8a = 130 + (Pe*45);
    Spray_rule8b = 225 - (Pe*45);
    Pe2 = Pe;
    Zde3 = 0;
  }
  else if(Zde<=Pe && Pe!=0 && Zde!=0){
    Fan_rule8a = 40 + (Zde*40);
    Fan_rule8b = 80 - (Zde*40);
    Spray_rule8a = 130 + (Zde*50);
    Spray_rule8b = 225 - (Zde*45);
    Zde3 = Zde;
    Pe2 = 0;
  }
  //9. jika Pe dan Pde maka Spray S.Cepat dan Fan S.Lambat
  if(Pe<=Pde && Pe!=0 && Pde!=0){
    Fan_rule9 = 60 - (Pe*20);
    Spray_rule9 = 200 + (Pe*30);
    Pe3 = Pe;
    Pde3 = 0;
  }
  else if(Pde<=Pe && Pe!=0 && Pde!=0){
    Fan_rule9 = 60 - (Pde*20);
    Spray_rule9 = 200 + (Pde*30);
    Pde3 = Pde;
    Pe3 = 0;
  }
  
  z_Fan = ((Fan_rule1*Ne1)+ (Fan_rule1*Nde1) + (Fan_rule2a*Ne2)+ (Fan_rule2b*Ne2)+ (Fan_rule2a*Zde1)+ (Fan_rule2b*Zde1)+ 
          (Fan_rule3a*Ne3)+ (Fan_rule3b*Ne3)+ (Fan_rule3a*Pde1)+ (Fan_rule3b*Pde1)+ 
          (Fan_rule4a*Ze1)+ (Fan_rule4b*Ze1)+ (Fan_rule4a*Nde2)+ (Fan_rule4b*Nde2)+ 
          (Fan_rule5a*Ze2)+ (Fan_rule5b*Ze2)+ (Fan_rule5a*Zde2)+ (Fan_rule5b*Zde2)+ 
          (Fan_rule6a*Ze3)+ (Fan_rule6b*Ze3)+ (Fan_rule6a*Pde2)+ (Fan_rule6b*Pde2)+ 
          (Fan_rule7a*Pe1)+ (Fan_rule7b*Pe1)+ (Fan_rule7a*Nde3)+ (Fan_rule7b*Nde3)+ 
          (Fan_rule8a*Pe2)+ (Fan_rule8b*Pe2)+ (Fan_rule8a*Zde3)+ (Fan_rule8b*Zde3)+ 
          (Fan_rule9*Pe3)+ (Fan_rule9*Pde3)) / 
          (Ne1+Nde1+Ne2+Ne2+Zde1+Zde1+Ne3+Ne3+Pde1+Pde1 +Ze1+Ze1+Nde2+Nde2+Ze2+Ze2+Zde2+Zde2+Ze3+Ze3+Pde2+Pde2 +Pe1+Pe1+Nde3+Nde3+Pe2+Pe2+Zde3+Zde3+Pe3+Pe3+Pde3+Pde3);
  z_Spray = ((Spray_rule1*Ne1) + (Spray_rule1*Nde1) + (Spray_rule2*Ne2) + (Spray_rule2*Zde1)+ (Spray_rule3*Ne3) + (Spray_rule3*Pde1)+ 
            (Spray_rule4a*Ze1) + (Spray_rule4b*Ze1)+ (Spray_rule4a*Nde2) + (Spray_rule4b*Nde2)+ (Spray_rule5*Ze2) + (Spray_rule5*Zde2)+ 
            (Spray_rule6a*Ze3) + (Spray_rule6b*Ze3)+ (Spray_rule6a*Pde2) + (Spray_rule6b*Pde2)+ 
            (Spray_rule7a*Pe1) + (Spray_rule7b*Pe1)+ (Spray_rule7a*Nde3) + (Spray_rule7b*Nde3)+ 
            (Spray_rule8a*Pe2) + (Spray_rule8b*Pe2)+ (Spray_rule8a*Zde3) + (Spray_rule8b*Zde3)+ (Spray_rule9*Pe3) + (Spray_rule9*Pde3)) / 
            (Ne1+Nde1+Ne2+Zde1+Ne3+Pde1 +Ze1+Ze1+Nde2+Nde2+Ze2+Zde2+Ze3+Ze3+Pde2+Pde2 +Pe1+Pe1+Nde3+Nde3+Pe2+Pe2+Zde3+Zde3+Pe3+Pde3);
  pwm_Fan = z_Fan;
  if(z_Spray == 0){
    pwm_Spray = 0;
  }
  else{
    pwm_Spray = z_Spray;
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(sprayer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(ensprayer, OUTPUT);
  pinMode(enfan, OUTPUT);
  pinMode(LED, OUTPUT);
  
  WiFi.begin(WIFISSID,PASSWORD);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    digitalWrite(LED, LOW);
  }
  digitalWrite(LED, HIGH);
  //--------------------------------------------------
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID,PASSWORD);
  //--------------------------------------------------
  dht.begin();
}

void loop() {
  //-------------Baca Kelembaban dan Suhu--------------
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Kelembaban : ");
  Serial.println(hum);
  Serial.print("Suhu : ");
  Serial.println(temp);
  
  //------------Mengirim data ke Antares---------------
  antares.add("humidity", hum);
  antares.add("temperature", temp);
  antares.send(projectName, deviceName);
  delay(5000);
  
  //-----------------Get data MODE---------------------
  antares.get(projectName, "MODE"); // Store latest value in buffer
  if(antares.getSuccess()) {
   // Mengambil data
   matic = antares.getInt("automatic");
   manual = antares.getInt("manual");
 
   // menampilkan di serial monitor
   matic=matic;
   manual=manual;
   Serial.print("AUTOMATIC: ");
   Serial.println(matic);
   Serial.print("MANUAL: ");
   Serial.println(manual);
  }

 //----------------------AUTOMATIC---------------------
 if(matic==1 && manual==0){
  //-------------------Proses Fuzzy--------------------
  fuzzyrule();
  Serial.print("pwm fan : ");
  Serial.println(z_Fan);
  Serial.print("pwm spray : ");
  Serial.println(z_Spray);
  analogWrite(ensprayer, pwm_Spray);
  analogWrite(enfan, pwm_Fan);
  digitalWrite(sprayer, HIGH);
  digitalWrite(fan, HIGH);
  delay(5000);
  digitalWrite(sprayer, LOW);
 }

 //------------------------MANUAL-----------------------
 else if(matic==0 && manual==1){
  //-----------------Get data Sprayer-------------------
  antares.get(projectName, "SPRAYER");
  if(antares.getSuccess()) {
   // Mengambil data
   sprayerON = antares.getInt("on");
   sprayerOFF = antares.getInt("off");
 
   // menampilkan di serial monitor
   sprayerON=sprayerON;
   sprayerOFF=sprayerOFF;
   Serial.print("Sprayer ON: ");
   Serial.println(sprayerON);
   Serial.print("Sprayer OFF: ");
   Serial.println(sprayerOFF);
  }

  //-------------------Get data FAN-----------------------
  antares.get(projectName, "FAN");
  if(antares.getSuccess()) {
   // Mengambil data
   fanON = antares.getInt("on");
   fanOFF = antares.getInt("off");
 
   // menampilkan di serial monitor
   fanON=fanON;
   fanOFF=fanOFF;
   Serial.print("Fan ON: ");
   Serial.println(fanON);
   Serial.print("Fan OFF: ");
   Serial.println(fanOFF);
  }

  //-----------------Kontrol Manual FAN dan SPRAYER-------------------
  if(sprayerON==1 && fanON==1){
    analogWrite(ensprayer, 1000);
    analogWrite(enfan, 1000);
    digitalWrite(sprayer, HIGH);
    digitalWrite(fan, HIGH);
    delay(10000);
    digitalWrite(sprayer, LOW);
  }
  if(sprayerON==1 && fanON==0){
    analogWrite(ensprayer, 1000);
    digitalWrite(sprayer, HIGH);
    delay(10000);
    digitalWrite(sprayer, LOW);
  }
  else if(sprayerON==0 && sprayerOFF==1){
    digitalWrite(sprayer, LOW);
  }
  
  if(fanON==1 && sprayerON==0){
    analogWrite(enfan, 1000);
    digitalWrite(fan, HIGH);
  }
  
  else if(fanON==0 && fanOFF==1){
    digitalWrite(fan, LOW);
  }
 }
}
