#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 128 /* 配列の大きさ */
#define DATASIZE 1000

typedef struct address { /* 構造体の宣言 (typedefを使用して以後srtuctを省略できるようにした) */
  char szName[21]; /* 氏名 */
  char szPos[8]; /* 郵便番号 */
  char szAdd[51];  /* 住所 */
  char szTel[11];  /* 電話番号*/
  char szMail[30]; /* メールアドレス */
    
} Address;

int search(Address *ap, int cnt); /* 関数のプロトタイプ宣言 */
int add(Address *ap, int cnt); /* ジャンプテーブルに入れるため引数及び返却値型はすべて同じとした */
int amend(Address *ap, int cnt);
int del(Address *ap, int cnt);
int printall(Address *ap, int cnt);

int (*func[])(Address *, int) = {search, add, amend, del, printall}; /* 関数のジャンプテーブル */

Address ad[DATASIZE]; /* 構造体の配列 大きくてスタックを消費する可能性があるので静的に定義した */

 /* 処理を番号で選ばせ、所定の関数に飛ばす */
 /* 処理によっては配列の要素数が変化する事があるので常に要素数 cnt を返させる */

int main(void)
{
  char dummy[BUFSIZE];
  int i, cnt = 0;
  
  while (1) {
    puts("●メニューを入力して下さい");
    puts("1.検索\n2.追加\n3.修正\n4.削除\n5.全体表示\n6.終了");
    scanf("%d", &i); gets(dummy);
    if (1 <= i && i <= 5) cnt = func[i - 1](ad, cnt);
    else if (i == 6) break;
  }

  return 0;
}

/*格納された構造体から入力されたデータを持つものを探す */
/* 検索するデータをswitch文によって処理を選択 */

int search(Address *ap, int cnt)
{
  char buf[BUFSIZE], dummy[BUFSIZE];
  char *target[] = {"氏名", "郵便番号", "住所","電話番号","メールアドレス"};
  int i, j = 0, count = 0;
  Address *t = ap;
  
  while (1) {
    puts("何で探しますか？\n1.氏名 2.郵便番号 3.住所 4.電話番号 5.メールアドレス");
    scanf("%d", &i); gets(dummy);
    if (1 <= i && i <= 3) break;
  }
  printf("%sを入力して下さい:", target[--i]);
  gets(buf);
  
  while (j++ < cnt) {
    switch (i) {
    case 0: /* 氏名 */
      if (!strcmp(t->szName, buf)) goto print;
      break;
    case 1: /* 郵便番号 */
      if (!strcmp(t->szPos, buf)) goto print;
      break;
    case 2: /* 住所 */
      if (!strcmp(t->szAdd, buf)) goto print;
      break;
    case 3: /* 電話番号 */
      if (!strcmp(t->szTel, buf)) goto print;
      break;
    case 4: /* メールアドレス */
      if (!strcmp(t->szMail, buf)) goto print;
      break;
            
    default:
      break;
    }
    t++;
    continue;
print:;
    printf("%d: 氏名:%s 郵便番号:%s 住所:%s 電話番号:%s メールアドレス:%s \n", j, t->szName, t->szPos, t->szAdd, t->szTel, t->szMail);
    t++;
    count++;
  }
  printf("%d件見つかりました\n", count);
  
  return cnt;
}

/* 構造体の*最後*の次に新しいデータを付け加える */
/* 追加に成功したら構造体の要素数が 1 増えるので cnt + 1 を返す */
int add(Address *ap, int cnt)
{
  Address *t = ap + cnt; /* 配列の最後の次のアドレスの計算 */
  
  if (cnt == DATASIZE) {
    puts("これ以上追加できません");
    return cnt;
  }
  printf("氏名："); /* 氏名 */
  gets(t->szName);
  printf("郵便番号："); /* 郵便番号 */
  gets(t->szPos);
  printf("住所："); /* 住所 */
  gets(t->szAdd);
  printf("電話番号："); /* 電話番号 */
  gets(t->szTel);
  printf("メールアドレス："); /* メールアドレス */
  gets(t->szMail);

  return cnt + 1;
}

/* 任意の構造体の内容を修正する */
/* 修正しない要素はreturnで飛ばせるようにした*/
int amend(Address *ap, int cnt)
{
  Address *t;
  char buf[BUFSIZE];
  int i;
  
  puts("何番を修正しますか？");
  scanf("%d", &i); gets(buf);
  
  if (i > 0 && i <= cnt) {
    t = ap + i - 1; /* 配列のアドレスの計算 */
    puts("変更しない場合は[return]を押して下さい");
    printf("氏名: %s -> ", t->szName);
    gets(buf);
    if (strlen(buf)) strcpy(t->szName, buf);
    printf("郵便番号: %s -> ", t->szPos);
    gets(buf);
    if (strlen(buf)) strcpy(t->szPos, buf);
    printf("住所: %s -> ", t->szAdd);
    gets(buf);
    if (strlen(buf)) strcpy(t->szAdd, buf);
    printf("電話番号: %s -> ", t->szTel);
    gets(buf);
    if (strlen(buf)) strcpy(t->szTel, buf);
    printf("メールアドレス: %s -> ", t->szMail);
    gets(buf);
    if (strlen(buf)) strcpy(t->szMail, buf);
      
  } else
    puts("番号が不適切です");
  return cnt;
}

/* 任意の構造体を削除する */
/* 削除方法はそれより後ろの構造体を一つ前にずらし cnt - 1 を返す */
/* i == cnt の時は実際には cnt - 1 を返すだけである */
int del(Address *ap, int cnt)
{
  char buf[BUFSIZE];
  int i, j;

  puts("何番を削除しますか？");
  scanf("%d", &i); gets(buf);
  
  if (i > 0 && i <= cnt) {
    for (j = i - 1; j < cnt - 1; j++)
      ap[j]= ap[j + 1];
    return cnt - 1;
  }

  puts("番号が不適切です");
  return cnt;
}

/* 格納された構造体を全部表示する */
/* 構造体には番号が付いてないので、カウンタ i で内部的に処理する(構造体の添え字 + 1) */
int printall(Address *ap, int cnt)
{
  int i = 0;

  while (i < cnt) {
    printf("%d: 氏名:%s 郵便番号:%s 住所:%s 電話番号:%s メールアドレス:%s\n", ++i, ap->szName, ap->szPos, ap->szAdd, ap->szTel, ap->szMail);
    ap++; /* 配列へのポインタが次の配列要素を指すようにする */
  }

  return cnt;
}
