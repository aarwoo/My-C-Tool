#ifndef AGETS
#define AGETS
#include<stdio.h>
#include<stdlib.h>
char * agets(FILE * src);
/**
 *agets:read a line with auto allocate memory for return value from src (Not include '\n' at last)
 *If anything is error,it returns NULL
 */
char * agets(FILE * src){
	char * ret=NULL,*tmp_ret;
	size_t ret_size=0;
	int tmp;
	for(tmp=fgetc(src);tmp!=EOF&&tmp!='\n';tmp=fgetc(src)){
		ret_size=ret_size+1;
		tmp_ret=realloc(ret,ret_size*sizeof(char));
		if(tmp_ret==NULL){
			free(ret);
			return NULL;
		}else{
			ret=tmp_ret;
			*(ret+ret_size-1)=(char)tmp;
		}
	}
	ret_size=ret_size+1;
	tmp_ret=realloc(ret,ret_size*sizeof(char));
	if(tmp_ret==NULL){
		free(ret);
		return NULL;
	}else{
		ret=tmp_ret;
		*(ret+ret_size-1)='\0';
		return ret;
	}
}
#endif
