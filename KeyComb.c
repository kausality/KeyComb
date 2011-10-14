/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE":
 * <turiyafy (AT) gmail (DOT) com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Kaustubh Pratap Chand
 * ----------------------------------------------------------------------------
 */


#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#define  BUFF 512
#define  WIN  "KeyComb"

typedef struct{

    struct{
        char key_buff[BUFF]; /*Buffer for keystrokes*/
        int  bpos;           /*Position of KEY_BUFF*/
    }buff;

    struct{
        char keys[56];      /*Key combination to match*/
        int  kpos;          /*Number of characters matched*/
    }key;

    char  msg[512];        /*Message to display on pattern match*/
    char prog[512];        /*Program to run on pattern match*/

}KEYCOMB ;

KEYCOMB kc;
int max_idx;               /*MAX INDEX of kc.key.keys*/

void strtoupper(char *s)
{
    while(*s=toupper(*s))
          s++;
}

int get_input()
{
    int l;

    fputs("*****KeyComb v2.1*****\n\n[Just Enter newline to skip]\n\n",stdout);
    fputs("Enter keys combination:[Max length:50]:",stdout);
    fgets(kc.key.keys,sizeof(kc.key.keys)/sizeof(kc.key.keys[0]),stdin);
    l=strlen(kc.key.keys);
    if(l==1){
       fputs("Invalid Input!\n",stderr);
       return 0;
    }
    /*Condition false only if input length is more then buffer,in that case the last character is not newline*/
    if(kc.key.keys[l-1]=='\x0A')
       kc.key.keys[l-1]=0;

    fputs("Enter the message to be displayed:",stdout);
    fgets(kc.msg,sizeof(kc.msg)/sizeof(kc.msg[0]),stdin);
    l=strlen(kc.msg);
    if(kc.msg[l-1]=='\x0A')
       kc.msg[l-1]=0;

    fputs("Enter the program to run on a match:",stdout);
    fgets(kc.prog,sizeof(kc.prog)/sizeof(kc.prog[0]),stdin);
    l=strlen(kc.prog);
    if(kc.prog[l-1]=='\x0A')
       kc.prog[l-1]=0;

    fputs("\n\n[Now KeyComb will run silently in the background.....]\n",stdout);
    return 1;
}

void hideme()
{
    HWND hwnd=FindWindow(NULL,WIN);
    ShowWindow(hwnd,SW_HIDE);
}

/*GetAsyncKeyState returns last key pressed.
  This function ensures that the subsequent calls to the GetAsyncKeyState
  function don't return the keys pressed when inputting
*/
void flush_keys()
{
    int t;
    for(t=0x0;t<0xFE;t++)    /*Flush any possible key*/
        GetAsyncKeyState(t);
}

/*Pops all keys from the buffer and zeroes the number of keys matched*/
void reset()
{
    kc.buff.bpos=0;
    kc.key.kpos=0;
}

/*Tests whether the keys combination is there in the buffer*/
int key_index()
{
    int i,j,k;
    char *buffer=kc.buff.key_buff;
    char *keys=kc.key.keys;
    int  len=kc.buff.bpos;

    for(i=0;i<len;i++){
        for(j=i,k=0;keys[k] && keys[k]==buffer[j] && j<len;j++,k++)
        ;
        if(k>0 && !keys[k])
           return 1;
    }
    return 0;
}

/*Pushes a key in the buffer*/
void push_key(int k)
{
    if(kc.buff.bpos==sizeof(kc.buff.key_buff)/sizeof(kc.buff.key_buff[0]))
       reset();

    kc.buff.key_buff[kc.buff.bpos++]=k;

}

/*Pops a key from the buffer*/
void pop_key()
{
    if(kc.buff.bpos!=0)
       --kc.buff.bpos;
    else
       kc.key.kpos=0;
}

void keycomb(int k)
{
    int c=k;
    char *key=kc.key.keys;
    int *i=&kc.key.kpos;
    push_key(c);

    if(key[*i]==c){
        if(*i==max_idx){
           if(key_index()){
              if(kc.prog[0])
                 ShellExecute(NULL,"open",kc.prog,NULL,NULL,SW_SHOW);

              if(kc.msg[0])
                 MessageBox(NULL,kc.msg,WIN,MB_ICONASTERISK);
        /*The keys combination was found in the keystoke buffer,start off again*/
           reset();
        }
        }
        else
           *i+=1;
    }
    else if(c=='\x08'){/*Emulates backspace key*/
        pop_key();/*pop the backspace key itself*/
        pop_key();/*delete the last character,just as backspace do*/
    }
    Sleep(75);
}

int main(int argc,char *argv[])
{
    int c;
    SetConsoleTitle(WIN);
    if(!get_input())
       return -1;
    flush_keys();
    strtoupper(kc.key.keys);
    Sleep(2000);
    hideme();

    max_idx=strlen(kc.key.keys)-1;
    while(1){
        for(c=0x0;c<0xBE;c++)
            if(GetAsyncKeyState(c)&1==1)
               keycomb(c);
    }
    return 0;
}







