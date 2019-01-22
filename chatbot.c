#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 256 //入力できる文字数

//プロトタイプ宣言
void trimEnd(char str[]);
void oumu(char userchat[], char botchat[]);
int endJudge(char userchat[], char botchat[]);
int cliche(char userchat[], char botchat[]);
int randchat(char botchat[]);
int memory(char userchat[], char botchat[], int mem_count);
int quiz(char userchat[], char botchat[]);

int main(void){
  char userchat[N]; //ユーザーが入力した内容
  char botchat[N]; //ボットが返す内容
  int mem_count = 0; //ボットが覚えた回数
  int prob; //確率
  int conf; //ファイルを開けたかどうか

  while(1){
    //入力
    printf("you:");
    fgets(userchat, N, stdin);
    if(strcmp(userchat, "\n") == 0) continue; //enterのみのとき繰り返し
    trimEnd(userchat);
    
    if(cliche(userchat, botchat) == 0){

      prob = rand() % 100 + 1; //確率を計算

      if(0 < prob && prob <= 10){ //オウム返し
        oumu(userchat, botchat);
        printf("(as オウム返しボット)\n");
      }else if(10 < prob && prob <= 40){ //テキトー返し
        conf = randchat(botchat);
        if(conf == -1){
          return 0;
        }
        printf("(as テキトー返しボット)\n");
      }else if(40 < prob && prob <= 100){ //勉強する
        conf = memory(userchat, botchat, mem_count);
        if(conf == -1){
          return 0;
        }
        printf("(as 勉強するボット)\n");
        mem_count++;
      }

    }

    printf("bot:%s \n", botchat);

    if(endJudge(userchat, botchat)){
      break;
    }
  }
  return 0;

}

//文字列の改行コードを取り除く
void trimEnd(char str[]){
  int i = 0;
  while(str[i] != '\n' && i < strlen(str)){
    i++;
  }
  str[i] = '\0';
}

//オウム返し
void oumu(char userchat[], char botchat[]){
  strcpy(botchat, userchat); //botchatにuserchatをコピー
}

//終わるか確認
int endJudge(char userchat[], char botchat[]){
  if(strcmp(userchat,"またね") == 0 || strcmp(userchat, "じゃあね") == 0){
    return 1;
  }else if(strcmp(botchat, "もうおぼえられない...") == 0){
    return 1;
  }

  return 0;
}

//決まり文句の確認
int cliche(char userchat[], char botchat[]){
  if(strcmp(userchat, "じゃあね") == 0){
    printf("(as 決まり文句ボット)\n");
    strcpy(botchat, "また話そうね");
    return 1;
  }
  if(strcmp(userchat, "またね") == 0){
    printf("(as 決まり文句ボット)\n");
    strcpy(botchat, "ばいばい");
    return 1;
  }

  //クイズ用
  if(strcmp(userchat, "クイズ") == 0 || strcmp(userchat, "quiz") == 0){
    printf("(as クイズボット)\n");
    quiz(userchat, botchat);
    return 1;
  }

  return 0;
}

//ランダム返し
int randchat(char botchat[]){
  int random;
  int i = 0;
  FILE *fp;
  char randstr[N][N];

  //読み込み専用でファイルを開く
  fp = fopen("random.txt", "r");

  //ファイルが開けたか確認
  if(fp == NULL){
    printf("ファイルが開けませんでした\n");
    return -1;
  }

  //ファイルから配列に文字列を読み込み
  while(fgets(randstr[i], N, fp) != NULL){
    i++;
  }
  fclose(fp);

  srand(time(NULL));
  random = rand() % 5; // 0 ~ 4のランダムな値
  trimEnd(randstr[random]);
  strcpy(botchat, randstr[random]);

  return 0;
}

//記憶するボット
int memory(char userchat[], char botchat[], int mem_count){
  FILE *fp;
  int random;
  int i = 0;
  char memstr[N][N];

  fp = fopen("memory.txt", "a"); //追加書き込み専用で開く

  if(fp == NULL){
    printf("ファイルが開けませんでした\n");
    return -1;
  }

  fprintf(fp, "%s\n", userchat);
  fclose(fp);

  fp = fopen("memory.txt", "r"); //読み出し専用で開く

  while(fgets(memstr[i], N, fp) != NULL){
    i++;
  }
  fclose(fp);
  
  srand(time(NULL));

  if(mem_count >= 4){ //覚えた文が4以上なら終了
    strcpy(botchat, "もうおぼえられない...");
  }else if(mem_count <= 4){
    do{
      random = rand() % i; // 0 ~ iの中からランダム選択
    }while(random == 0); //0の時もう一度ランダム(最初改行が入るため)

    trimEnd(memstr[random]);
    strcpy(botchat, memstr[random]);
  }
  return 0;
}

int quiz(char userchat[], char botchat[]){
  FILE *fp;
  int random;
  int i = 0;
  char quizchat[N][N];

  //問題文の選定
  fp = fopen("question.txt", "r"); //読み出し専用で開く
  if(fp == NULL){
    printf("ファイルが開けませんでした\n");
    return -1;
  }

  while(fgets(quizchat[i], N, fp) != NULL){
    i++;
  }
  fclose(fp);

  srand(time(NULL));
  random = rand() % i;

  printf("bot:%s", quizchat[random]);

  //ユーザーの答え
  printf("you:");
  do{
    fgets(userchat, N, stdin);
  }while(strcmp(userchat, "\n") == 0);
  trimEnd(userchat);

  //答えとあっているか
  fp = fopen("answer.txt", "r"); //読み出し専用で開く
  if(fp == NULL){
    printf("ファイルが開けませんでした\n");
    return -1;
  }
  i = 0;
  while(fgets(quizchat[i], N, fp) != NULL){
    i++;
  }
  fclose(fp);
  trimEnd(quizchat[random]);

  if(strcmp(userchat, quizchat[random]) == 0){
    strcpy(botchat, "正解です");
  }else{
    strcpy(botchat, "不正解です。正解は");
    strcat(botchat, quizchat[random]);
    strcat(botchat, "です");
  }

  return 0;
}
