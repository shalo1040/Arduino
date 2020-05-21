//온습도 센서와 조도 센서로부터 값을 읽고
//평균 수준일 때와 평균 이상일 때의 LED 불빛을 변경하는 코드

#include <SimpleDHT.h>  //온습도 센서(DHT11) 활용 위한 라이브러리

//LED와 연결된 디지털핀 번호
const int RED = 9;
const int GREEN = 10;
const int BLUE = 11;

//온습도 센서와 조도 센서
int humidityPin = 2;
int lightPin = A0;

SimpleDHT11 dht11(humidityPin);

//LED 색상값 저장 변수
int r = 0;
int g = 0;
int b = 0;

void setup() {
  Serial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  
  //습도 센서 값 읽고 오류 발생시 오류 출력
  if((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    return ;
  }

  //조도 센서 값 읽기
  int light = analogRead(lightPin);  
  
  //조도 센서 값(0~1023)을 LED 범위값(0~255)으로 맵핑
  light = map(light, 0, 1023, 0, 255);
  
  //변환값의 최저값과 최고값을 0과 255로 고정
  light = constrain(light, 0, 255);
    
  //습도 출력
  Serial.print("humidity ");
  Serial.print(humidity);
  Serial.print("% ");

  //조도 출력
  Serial.print("light ");
  Serial.println(light);
  delay(2000);

  //평균(습도센서: 51~59, 조도센서: 171~199) => 초록색
  if(humidity > 50 && humidity < 60 && light > 170 && light < 200) {
    r = 0;
    g = 255;
    b = 0;
  }
  //평균 이상(습도센서: 60~255, 조도센서: 200~255) => 빨간색
  else if(humidity >= 60 || light >= 200) {
    r = 255;
    g = 0;
    b = 0;
  } else {  //평균 이하일 때 => 노란색
    r = 255;
    g = 255;
    b = 0;
  }

  //LED 밝히기
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}
