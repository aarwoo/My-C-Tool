/*You cannot mix agets and other functions to read the file
Or else,you will meet the Bug.*/
#ifndef agets_h
#define agets_h
#include<stdio.h>
#include<stdlib.h>
char * _agets(FILE * src,char * str,size_t size,int ch,char * tmp){
	ch=fgetc(src);
	tmp=realloc(str,(size+1)*sizeof(char));
	if(tmp==NULL){
		free(str);
		return NULL;
	}else if(((char)ch)=='\n'||ch==EOF){
		*(tmp+size)='\0';
		return tmp;
	}else{
		*(tmp+size)=((char)ch);
		return _agets(src,tmp,size+1,0,NULL);
	}
}
char * agets(FILE * src){
	return _agets(src,NULL,0,0,NULL);
}
#endif