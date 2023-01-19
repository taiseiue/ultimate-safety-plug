#include <M5Atom.h>
#include <WiFi.h>       //WiFi接続用
#include <WebServer.h>  //サーバー設定用

// アクセスポイント設定
const char ssid_AP[] = "ultimate-safety-plug";  //SSID
const char pass_AP[] = "prozemi-ie";  //パスワード(8文字以上)

// FastLEDライブラリの設定（CRGB構造体）
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 16) | (g << 8) | b);
}
// LEDマトリクス表示指定配列（0:消灯、1以上：色指定配列で指定した色に指定（color配列1次要素番号））
int ok[5][5] = {{0,0,3,0,0},
                    {0,3,0,3,0},
                    {3,0,0,0,3},
                    {0,3,0,3,0},
                    {0,0,3,0,0}}; //十字と4色表示

int ng[5][5] =     {{1,0,0,0,1},
                    {0,1,0,1,0},
                    {0,0,1,0,0},
                    {0,1,0,1,0},
                    {1,0,0,0,1}}; //十字と4色表示

int off[5][5] =    {{0,0,0,0,0},
                    {0,0,0,0,0},
                    {0,0,0,0,0},
                    {0,0,0,0,0},
                    {0,0,0,0,0}}; //十字と4色表示

// 色指定配列（1次要素を色番号として表示する色を2次要素の3原色{赤、緑、青}で設定）
//       LED色： {{0:消灯}  , {1:ピンク}   , {2:オレンジ}  , {3:グリーン} , {4:パープル}  , {5:ホワイト}}
int color[][3] = {{0, 0, 0}, {255, 0, 00}, {255, 255, 0}, {70, 255, 0}, {70, 0, 255}, {255, 255, 255}};

// 変数宣言
int num = 0;  //matrix配列の色番号格納用



// サーバー設定ポート80で接続
WebServer server(80);
// htmlメイン画面データ（「"」は「\"」に置き換え、htmlの改行は「\n」、コードの改行は「\」）
char html[] = "\
<!DOCTYPE html><html lang=\"jp\"><head>\n\
<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
<title>アルティメットセーフティプラグ</title>\n\
</head><body>\n\
  <h1>アルティメットセーフティプラグ遠隔操作</h1>\n\
  <div>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_on'\">ブザーオン</button>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_off'\">ブザーオフ</button>\n\
    <br/>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/mx_ok'\">マトリクスOK</button>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/mx_ng'\">マトリクスNG</button>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/mx_off'\">マトリクス消灯</button>\n\
  </div>\n\
</body></html>\n";


//-------------------------------------------------
// サーバーリクエスト時処理関数
//-------------------------------------------------
// ルートアクセス時の応答関数
void handleRoot() {
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
// エラー（Webページが見つからない）時の応答関数
void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found!");  //text送信
}
// ブラウザONボタン処理
void btnOn() {
  digitalWrite(33, HIGH);                   //出力0 ON（青色LED）
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
// ブラウザOFFボタン処理
void btnOff() {
  digitalWrite(33, LOW);                    //出力0 OFF（青色LED）
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}

void mx_ok() {
  drawMatrix(ok);
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
void mx_ng() {
  drawMatrix(ng);
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
void mx_off() {
  drawMatrix(off);
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}

// マトリクス描画処理
void drawMatrix(int matrix[5][5]){
  digitalWrite(33, HIGH);
  delay(10);
  digitalWrite(33, LOW);
  for (int i = 0; i < 5; i++) {     //matrix行ループ
    for (int j = 0; j < 5; j++) {   //matrix列ループ
      
      num = matrix[i][j];           //matrix配列の色番号取得
      //matrix行ごと（0,5,10,15,20）に左から右へ色番号の色で表示
      M5.dis.drawpix(i*5+j, dispColor(color[num][0], color[num][1], color[num][2]));
      M5.dis.setBrightness(255);
      
    }
  }
}

//-------------------------------------------------
// 初期設定
//-------------------------------------------------
void setup() {
  M5.begin(true, false, true);    //Serial,POWER,LED
  Serial.begin(9600);             //標準のシリアル通信設定

  // アクセスポイント設定
  WiFi.softAP(ssid_AP, pass_AP);  //ソフトAP設定

  // サーバー設定
  server.on("/", handleRoot);         //ルートアクセス時の応答関数を設定
  server.onNotFound(handleNotFound);  //Webページが見つからない時の応答関数を設定
  server.on("/get/btn_on", btnOn);    //ボタンオン受信処理
  server.on("/get/btn_off", btnOff);  //ボタンオフ受信処理
  server.on("/get/mx_ok", mx_ok);  //ボタンオフ受信処理
  server.on("/get/mx_ng", mx_ng);  //ボタンオフ受信処理
  server.on("/get/mx_off", mx_off);  //ボタンオフ受信処理
  server.begin();                     //Webサーバー開始

  // シリアル出力
  Serial.printf("\nSSID:%s\n", ssid_AP);//SSID
  Serial.printf("PASS:%s\n", pass_AP);  //PASSWORD
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());      //IPアドレス（配列）

  // 本体LED設定
  M5.dis.drawpix(0, dispColor(20, 20, 20)); //LED点灯（白）
  // 入出力端子設定
  pinMode(33, OUTPUT);  //出力端子
}
//-------------------------------------------------
// メイン
//-------------------------------------------------
void loop() {
  server.handleClient();  //クライアントからのアクセス確認
  delay(200);             //遅延時間（ms）
}
