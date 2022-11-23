/// @brief マトリクスLEDを操作するためのクラス
class DisplayUtility {
public:

/// @brief エラー時の色です
static int color_Error;
/// @brief 警告時の色です
static int color_Warning;
/// @brief 通常状態の色です
static int color_Moderate;
/// @brief LED消灯です
static int color_Empty;
/// @brief 特定の色でLEDを全点灯させます
/// @param color 点灯色
static void Fill(int color);

/// @brief LEDをすべて消灯します
static void Clear();
/// @brief LEDを指定したエラー色で表示します
static void Error();
/// @brief LEDを指定した警告色で表示します
static void Warning();
/// @brief LEDを指定した通常色で表示します
static void Moderate();
};