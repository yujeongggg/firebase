#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include "ServoControl.h"

// WiFi 설정
#define WIFI_SSID "KT_GiGA_2G_Wave2_A928"
#define WIFI_PASSWORD "xd97cx3337"

// Firebase 설정
#define FIREBASE_HOST "fir-dec15-default-rtdb.firebaseio.com" // https 빼야함
#define API_KEY "AIzaSyA4mxD94b8V4_Q5FZjfeyVQ0HbaymBYBK8" // Firebase 프로젝트의 API 키

// Firebase Authentication 정보
#define USER_EMAIL "dbwjd0111@gmail.com"
#define USER_PASSWORD "moonauh8884!!"

// DHT 센서 설정
#define DHTPIN 2 // DHT 센서 핀 (GPIO 2)
#define DHTTYPE DHT11 // DHT11 또는 DHT22

DHT dht(DHTPIN, DHTTYPE);

// 서보 모터 설정
#define SERVOPIN 14 // 서보 모터 핀 (GPIO 14)
ServoControl servoControl(SERVOPIN);

// 온도 임계값 설정
#define TEMPERATURE_THRESHOLD 27.0 // 임계값 (예: 30도)


// Firebase 객체 생성
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);

  // WiFi 연결
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // DHT 센서 초기화
  dht.begin();

  // Firebase 설정
  config.host = FIREBASE_HOST;
  config.api_key = API_KEY; // API 키 설정

  // Firebase 초기화
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Firebase Authentication 설정
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Firebase Authentication 로그인
  if (Firebase.signUp(&config, &auth, USER_EMAIL, USER_PASSWORD)) {
    Serial.println("Firebase Authentication succeeded");
  } else {
    Serial.printf("Firebase Authentication failed: %s\n", config.signer.signupError.message.c_str());
  }

  // 로그인 상태 확인
  if (Firebase.ready()) {
    Serial.println("Firebase is ready");
  } else {
    Serial.println("Firebase is not ready");
  }
}

void loop() {
  // DHT 센서에서 온도와 습도 값 읽기
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 읽기 오류 확인
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 사용자 고유 ID 가져오기
  String uid = auth.token.uid.c_str();

  // Firebase에 사용자 고유 ID 경로에 온도와 습도 값 쓰기
  String path = "/users/" + uid + "/sensor";
  if (Firebase.setFloat(firebaseData, path + "/temperature", temperature) &&
      Firebase.setFloat(firebaseData, path + "/humidity", humidity)) {
    Serial.println("Sensor values written successfully");
  } else {
    Serial.print("Error writing sensor values: ");
    Serial.println(firebaseData.errorReason());
  }

  // 온도 임계값 초과 시 서보 모터 구동
  if (temperature > TEMPERATURE_THRESHOLD) {
    Serial.println("Temperature threshold exceeded! Servo motor activated.");
    servoControl.activateServo(5, 15); // 서보 모터를 5번 반복, 각도 변경 간 15ms 대기
    Serial.println("Servo motor deactivated.");
  }

  delay(5000); // 5초 대기
}