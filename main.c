#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
int isKeyword(char token[]){
	char keywords[32][10] = {"break","case","char","const","continue","do","else","enum","float","for","goto",
                            "if","int","long","record","return","static","while"};
	int i,k= 0;
	int y=0;
    if(token[0]!='"'){
        for(y=0;y<strlen(token);y++){
            token[y]=tolower(token[y]);
	}
    }

	for(i = 0; i < 32; ++i){
		if(strcmp(keywords[i], token) == 0){
			k=1;
			break;
		}
	}
	return k;
}
int isOperator(char token1[]){
    char operators[7][3] = {"++","--","+","-","*","/",":="};
	int i,k=0;
	for(i = 0; i < 7 ; ++i){
		if(strcmp(operators[i],token1) == 0){
			k=1;
			break;
		}
	}
	return k;
}
int main(){
    int boyut=0;
	char ch; //DOSYADAN OKUNAN KARAKTER
	char token1[3]={'0'}; //OPERATOR KARAKTERLERINI TOPLAYAN DOSYA
	char *token; // DIGER KARAKTERLERIN TOPLANDIGI DOSYA
	char parantez[6]="[]{}()"; //PARANTEZ DOSYASI
	char endofline[2]=";"; //SATIR SONU DOSYASI
	FILE *fileR; //OKUNAN DOSYA
	FILE *fileW; // YAZILAN DOSYA
	int t=0;
	int i=0; //FOR ICIN DEGER
	int k=0; //OPERATORLERI DOSYADAKI KONUMU
	int spaceCounter=0; // CODE.PSI DOSYASINDAKI BOSLUK SAYACI
	int j=0; //OPERATOR HARICI KARAKTERLERIN DOSYA KONUMU
	int z=0; //FOR ICIN BASKA BIR DEGER
	fileW=fopen("code.lex.txt","w");
	fileR=fopen("code.psi.txt","r");

    if((fileR=fopen("code.psi.txt","r+"))==NULL){
        printf("File Not Found !!\n");
    }
    else{
        while((ch=fgetc(fileR))!=EOF){
              boyut++;
        }

	rewind(fileR);
    token=(char *)malloc(boyut*sizeof(char)); //DINAMIK HAFIZA
    }


	if(fileR== NULL){
		printf("File not found !!\n"); //DOSYA VARLIK KONTROLU
		exit(0);
	}
	while((ch = fgetc(fileR)) != EOF){


        if((isalnum(ch)!=0) || (ch=='_') || (ch=='"') || (ch==' ')){ //KARAKTERLER TOKEN DOSYASINA AKTARILIR
   			token[j++] = ch;
   		}

   		else if ( j != 0)  { //TOKEN DOSYASINDA 0 DAN FARKLI KARAKTER VARDIR YANI ICI DOLUDUR
   				token[j] = '\0';
   				if(isKeyword(token) == 1){//KEYWORD KONTROLU YAPILIR
   					fprintf(fileW,"keyword(%s)\n", token);
   					j = 0;//BU DEGER DOSYANIN POINTERINI BASINA GONDERIR
   				}
   				else if((isalpha(token[0])!=0) && (token[0]!='"')) { //KEYWORD DEGILSE VE ILK KARAKTERI (") ISARETI DEGILSE YANI STRING DEGILSE BURAYA GIRER
                    	for(t=0;t<strlen(token);t++){ //IDENTIFIER TOUPPER OLDUKTAN SONRA PRINTLENIR
                            token[t]=toupper(token[t]);
                    }
                    t=0;
                    j=0;
   				    if (strlen(token)>20){
                        fprintf(fileW,"Maksimum identifier size is 20. \n"); // SIZE KONTROLU STRLEN()
                        j=0;

   				    }
   				    else {
                        fprintf(fileW,"Identifier(%s)\n",token); //HATA VERMEZSE DOSYANIN ICINE YAZILIR
                        j = 0;
   				    }
   				}
   				else if ((isalpha(token[0])==0) && (token[0]!='"')){ //TOKEN DOSYASININ ICINDEKI ILK DEGER SAYI ISE BU BLOK CALISIR
                    if (strlen(token)>10){
                        fprintf(fileW,"Integer size Error(Maksimum size 10 digits.) \n");
                        j=0;
   				    }
                    else{
                        for(z=0;z<strlen(token);z++){ //ELSE BLOGU OLDUGU ICIN BOSLUK KARAKTERLERINI INTEGER DEGER ALGILIYORDU BU YUZDEN BOSLUK SAYACI TUTTUM!!
                            if(token[z]==' '){
                                spaceCounter++;
                                j=0;
                            }
                        }
                        if(spaceCounter==0){
                        fprintf(fileW,"Integer(%s)\n",token);
                        j = 0;
                        }
                        spaceCounter=0;
                    }
   				}
   				else if ((token[0]=='"'))  { // (") ISARETI ILE BASLIYORSA BU BLOGA GIRIYOR
                    if((token[j-1]!='"')){ // STRING EGER (") ISARETI ILE BITMIYORSA STRING HATASI VERIR
                        fprintf(fileW,"String Error \n");
                        j=0;
                    }
                    else{ //AKSI DURUMDA STRING YAZILIR
                        fprintf(fileW,"String(%s)\n",token);
                        j= 0;
                    }
   				}
   				}

         if(ispunct(ch)){
            for(i = 0; i < 6; ++i){ //PARANTEZ KONTROLU YAPILAN FOR DONGUSU
                if((ch == parantez[i])) {
                    if(ch=='{'){
                        fprintf(fileW,"LeftCurlyBracket\n"); }
                    else if(ch=='}'){
                        fprintf(fileW,"RighyCurlyBracket\n"); }
                    else if(ch=='['){
                        fprintf(fileW,"LeftSquareBracket\n"); }
                    else if(ch==']'){
                        fprintf(fileW,"RightSquareBracket\n"); }
                    else if(ch=='('){
                            ch=fgetc(fileR);
                            if (ch=='*') {
                                ch=fgetc(fileR);
                                while(ch!='*'){
                                    ch=fgetc(fileR);
                                }
                                ch=fgetc(fileR);
                                if (ch==')'){
                                    ch=fgetc(fileR);
                                }
                           }
                           else {
                               ungetc(ch,fileR);
                               fprintf(fileW,"LeftPar\n");
                        }
                    }
                    else if(ch==')'){
                        fprintf(fileW,"RightPar\n"); }
        }
   		}
        }

   		if(ch=='-' || ch=='+' || ch=='*' || ch=='/' || ch==':'){ //OPERATORLERIN TOPLANDIGI BLOK
            token1[k++]=ch;
            ch=fgetc(fileR);
            if ((ch=='-') || (ch=='+') || (ch=='=')){ // CIFT OPERATOR KONTROLU BURDA YAPILIR --/++/:= GIBI
                token1[k++]=ch;
            }
            if(strlen(token1)<2){
                ungetc(ch,fileR);
            }
   		}
        if (k != 0)  { //TOKEN1 LISTESININ ICI BOS DEGILSE BU BLOK CALISIR

                if(isOperator(token1)==1){ // OPERATORUMUZ "TANIMLI MI?" KONTROLU
                    fprintf(fileW,"operator(%s)\n",token1);
                    k=0;
                    token1[0]='\0'; //MAKSIMUM 2 ELEMANLI OPERATOR OLABILECEGI ICIN TUM ELEMANLARI NULL DEGER ATANIR
                    token1[1]='\0';
   				}
        }
   		if (ch==endofline[0]){ //ENDOFLINE KONTROLU
   				fprintf(fileW,"EndOfLine\n");
			   }
	}
	fclose(fileR);
	fclose(fileW);
	return 0;
}
