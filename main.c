#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>


int line_number=1;



void StringW(FILE *fptr, FILE *wfptr, char ch) {
    int i=0;
    ch= fgetc(fptr) ;
    char* mystring= (char *) malloc(100);
    int max_lenght=100;

    while (ch != '"' && i<=max_lenght && ch!=EOF)
    {

        *(mystring+i)=ch;
        i++;
        ch= fgetc(fptr) ;

    }
    *(mystring+i)='\0';
    if(i<=100 ) {
        if(ch!=EOF) {
            fprintf(wfptr, "StringConstant (%s)\n", mystring);
        }
        else{
            printf("Match Error (\") character at Line: %d \n",line_number);
        }


    }


    free(mystring);
}

void charDetector(char ch, FILE *fptr, FILE *wfptr) {
    if(!(isalnum(ch)) && ch!='-')
        switch (ch)
        {
            case '"':

                StringW(fptr,wfptr,ch);
                break;
            case '{':
                while ((ch = fgetc(fptr)) != '}') {
					if(ch == EOF){
						printf("Not found End fParanthesis:%d\n",line_number);
						break;
					}}
                break;
            case '}': //fancy paranthesis
                printf("Not found start fParanthesis:%d\n",line_number);
                break;
            case '-':case ' ':case'\t':break;
            case '\n': line_number++; break;
            case '[':
                fprintf(wfptr,"OpenBlock \n");
                break;
            case ']':
                fprintf(wfptr,"CloseBlock \n");
                break;
            case '.':
                fprintf(wfptr,"EndOfLine \n");
                break;
            case ',':
            	fprintf(wfptr,"Seperator \n");
            	break;

            default:
                if(ch=='_')
                {fprintf(wfptr,"Underscore character\n");
                }
                else{
                    printf("Unrecognized Character (%c) Line : %d\n",ch,line_number);
                }
                ch=fgetc(fptr);
                if(isalnum(ch))
                {printf("Invalid identifier word Line : %d\n",line_number);

                    while(isalnum(ch) || ch=='_')
                        ch=fgetc(fptr);
                }
                if(ch!=EOF)
                    fseek(fptr,-1,SEEK_CUR);
        }
}
int checkDigit(char* word,FILE *f,FILE* w)
{
    int error=0,i=0;
    while(word[i])
    {
        if(isdigit(word[i])== 0)
        {
            error=1;
            charDetector(word[i], f, w);
            return 0;
        }
        i++;
    }
    return 1;
}

void constWriter(char* word,int minus,FILE* f,FILE* w)
{

    {if(checkDigit(word,f,w))
        {
            if(minus==1)
            {
                fprintf(w,"IntConstant (%s)\n",word-1);
            }

            else
            {fprintf(w,"IntConstant (%s)\n",word);}
        }
        else
        {
            if(minus==1)
                printf("Error identifier word (%s) line:%d\n",word-1,line_number);
            else
                printf("Error word (%s) line:%d\n",word,line_number);
        }
    }
}
int wordParser(FILE * f,char* word,char **keywords,FILE* w)
{
	if((isdigit(*word))&(*word+1) =='.'){
		printf("Invalid Usage Float Line: %d\n",line_number);
	}
    else if(isdigit(*word)||*word=='-')
    {
        if(*word=='-')
            if(isdigit(*(word+1)))
                constWriter(word+1,1,f,w);
            else
            {
                int a=0;

                while(word[a])
                {char t = word[a];
                    if(word[a]=='-')
                    {
                        printf("Error minus sign usage Line: %d\n",line_number);
                    }
                    else
                    {
                        charDetector(word[a], f, w);
                    }
                    a++;

                }



            }
        else
            constWriter(word,0,f,w);

    }
    else
    {
    	int charSum = 0;
        int iskeyword=0,i=0;
        int flagTypo = 0;
        int counterIdentifier = 0;
		int k;
		for (k = 0; word[k] != '\0'; ++k){
				charSum += (int)word[k];
				counterIdentifier++;
			}
        for(i=0;i<10;i++)
        {

            char *key=keywords[i];;

			int j;
			int charSumKey = 0;
			for (j = 0;key[j] != '\0';j++){
				charSumKey += (int)key[j];
			}


			if ((charSumKey == charSum) & strcmp(key,word)!=0 ){
				flagTypo = 1;
				printf("Typo Error (%s) line : %d \n ",word,line_number);
            	break;
			}
			else if (counterIdentifier >20){

				break;
			}
            else if( strcmp(key,word)==0)
            {
                fprintf(w,"Keyword '%s'\n",keywords[i]);
                iskeyword=1;
                break;
            }

        }
        if(!iskeyword)
        {   if(isdigit(word[0])||word[0]=='_')
            {
                printf("Invalid identifier token (%s) line%d\n",word,line_number);
                return 0;
            }
            else {
                int error=0;
                for(i=0;i<strlen(word);i++)
                {
                    char a =word[i];
                    if((!isalnum((word[i]))  && word[i]!='_' ))
                    {
                        error=1;
                        charDetector(word[i], f, w);

                    }
                }
                if((error==0) & (counterIdentifier <=20)  )
                {fprintf(w,"Identifier '%s'\n",word);}
                else if(flagTypo ==1)
                {
                }
                else{
                	if (counterIdentifier >20){
                		printf("Invalid identifier length '%s' Line : %d\n",word,line_number);
					}
					else{
						printf("Invalid identifier Token '%s' Line : %d\n",word,line_number);
					}

				}
            }
        }

    }

}




int main(int argc, char *argv[]) {

  char* keywords[]={"move","add","sub","out","loop","to","from","times","int","newline"};
    setlocale(LC_ALL,"Turkish");
	char *SourcefilePath = argv[1];
	char ba[] = ".ba";
	strncat(SourcefilePath, ba,3);
    FILE* file;
    FILE* writen;
 	char ch;
 	int chr=0;
 	file=fopen(SourcefilePath,"r");
 	writen=fopen("myscript.lx","w");


 if(file==NULL)
 {
    printf("\nsorry! something gone wrong!\nFile couldn't open :(\n\n");
    return 0;
 }
 else
 {
    printf("\nFile Opened.Reading...\n\n");
 }




 int a=1;

 while((ch=fgetc(file))!=EOF)
 {
     charDetector(ch, file, writen);


     chr=0;
     if(isalnum(ch)|| ch=='-')
     {
         char* ptrword=(char *)malloc(100);
         chr=0;
         while(ch!=' '&& ch!='\n'&& ch!='\t'&& ch!=',' && ch!='.')
         {
             *(ptrword+chr)=ch;
             chr++;
             ch=fgetc(file);
             if(ch==EOF)
              {

               break;
              }
         }
      fseek(file,-1,SEEK_CUR);
         *(ptrword+chr)='\0';

         wordParser(file,ptrword,keywords,writen);
         free(ptrword);
     }

 }


 fclose(file);
 fclose(writen);
    printf("\n\nAnalysis Completed With:\n\n");
    printf("\n\n...Process Completed...\n\n");

    return 0;

}

