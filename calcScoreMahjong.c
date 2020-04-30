#include <stdio.h>
#include <string.h>
#include <math.h>

#define OK          0
#define NG          -1
#define STR_PARENT  "親"
#define STR_CHILD   "子"
#define STR_MANGAN  "【満貫】"
#define STR_HANEMAN "【跳満】"
#define STR_BAIMAN  "【倍満】"
#define STR_SANBAI  "【三倍満】"
#define STR_YAKUMAN "【役満】"
#define MAG_CHILD   1.0     /* 子の倍率    */
#define MAG_PARENT  1.5     /* 親の倍率    */
#define FLG_CHILD   1       /* 子フラグ    */
#define FLG_PARENT  2       /* 親フラグ    */
#define MANGAN      8000    /* 満貫の点数  */
#define HAN_MIN     1       /* 飜数:最小   */
#define FU_MIN      20      /* 符数:最小   */
#define FU_MAX      110     /* 符数:最大   */

int  calcScoreMahjong();
void calcScore(int, int, double, int*, char*);
int  roundup10(int);
int  checkInput(int, int, int);

/**************/
/* メイン関数 */
/**************/
int main()
{
  int ret = NG;

  ret = calcScoreMahjong();
  if(ret != OK)
  {
    return ret;
  }
  return 0;
}

/**************/
/* 点数計算   */
/**************/
int calcScoreMahjong()
{
  int    ret           = NG;
  char   strPar[8]     = STR_CHILD;
  char   strFu[8]      = "";
  char   strMan[8]     = "";         /* 満貫名称 */
  char   strResult[32] = "";         /* 結果出力 */
  int    han           = 0;          /* 飜数     */
  int    fu            = 0;          /* 符数     */
  int    flgParent     = 0;          /* 親フラグ */
  double magni         = MAG_CHILD;  /* 倍率     */
  int    score         = 0;          /* 得点     */

  /* 親子、飜の入力 */
  printf("親子 %d:子 %d:親\n→ ", FLG_CHILD, FLG_PARENT);
  scanf("%d", &flgParent);
  printf("飜→ ");
  scanf("%d", &han);
  /* 4飜以下で符数入力 */
  if(han < 5)
  {
    printf("符→ ");
    scanf("%d", &fu);
  }

  /* 入力値チェック */
  ret = checkInput(flgParent, han, fu);
  if(ret != OK)
  {
    printf("=================================\n");
    printf("異常終了\n");
    printf("=================================\n");
    return ret;
  }

  /* 親なら倍率を1.5倍にする */
  if(flgParent == FLG_PARENT)
  {
    magni = MAG_PARENT;
    strcpy(strPar, STR_PARENT);
  }

  /* 点数計算 */
  calcScore(han, fu, magni, &score, strMan);

  /* 出力文字列作成 */
  sprintf(strResult, "%s %d飜", strPar, han);
  if(fu != 0)
  {
    sprintf(strFu,"%d符", fu);
    strcat(strResult, strFu);
  }
  strcat(strResult, strMan);

  printf("=================================\n");
  printf("%s\n", strResult);
  printf("%d点\n", score);
  printf("=================================\n");
  return OK;
}

/******************/
/* 入力値チェック */
/******************/
int checkInput(
  int flg,    /* (o)親子フラグ */
  int han,    /* (o)飜数       */
  int fu      /* (o)符数       */
)
{
  /* フラグチェック */
  if(flg != FLG_CHILD && flg != FLG_PARENT)
  {
    printf("フラグエラー\n");
    return NG;
  }

  /* 飜数チェック */
  if(han < HAN_MIN)
  {
    printf("飜数エラー\n");
    return NG;
  }

  /* 符数チェック */
  if((fu < FU_MIN) ||
     (fu > FU_MAX) ||
     ((fu%10) != 0))
  {
    /* 七対子判定 */
    if(fu != 25)
    {
      /* 入力省略判定 */
      if(han < 4 && fu == 0)
      {
        printf("符数エラー\n");
        return NG;
      }
    }
  }
  /* 1飜20符、25符は除外 */
  if(han == 1 &&
     fu < 30)
  {
    printf("計算不可\n");
    return NG;
  }
  return OK;
}

/****************/
/* 得点計算関数 */
/****************/
void calcScore(
  int    han,       /* (i)飜数             */
  int    fu,        /* (i)符数             */
  double magni,     /* (i)倍率             */
  int    *score,    /* (o)得点             */
  char   *man       /* (o)満貫以上の文字列 */
)
{
  int    buff     = 0;               /* 得点バッファ   */
  double mangan   = MANGAN * magni;  /* 満貫の点数設定 */

  switch (han)
  {
    case 1:
    case 2:
    case 3:
    case 4:
      /* 基本計算:符 * 4 * 倍率 * 2の飜数乗(場ゾロの2飜を足す) */
      buff = fu * 4 * magni * pow(2, (double)(han + 2));
      /* 10の位で切り上げる */
      buff = roundup10(buff);

      /* 満貫以上は置換する */
      if(buff > mangan)
      {
        buff = mangan;
      }
      break;
    case 5:                /* 満貫 */
      buff = mangan;
      strcpy(man, STR_MANGAN);
      break;
    case 6:                /* 跳満 */
    case 7:
      buff = mangan * 1.5;
      strcpy(man, STR_HANEMAN);
      break;
    case 8:                /* 倍満 */
    case 9:
    case 10:
      buff = mangan * 2;
      strcpy(man, STR_BAIMAN);
      break;
    case 11:               /* 三倍満 */
    case 12:
      buff = mangan * 3;
      strcpy(man, STR_SANBAI);
      break;
    default:               /* 役満 */
      buff = mangan * 4;
      strcpy(man, STR_YAKUMAN);
      break;
  }
  *score = (int)buff;
}

/************************/
/* 切り上げ関数(10の位) */
/************************/
int roundup10(
  int score    /* (i)繰り上げる数 */
)
{
  int div = 0;
  int rem = 0;

  div = score / 100;
  rem = score % 100;

  if(rem != 0)
  {
    div = div + 1;
  }
  return div * 100;
}
