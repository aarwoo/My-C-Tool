#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include"eval.h"
#include"agets.h"
int main(){
	char * expr=NULL;
	const char * ans=NULL;
	while(true){
		system("cls");/*It maybe do something unexcept*/
		printf("version 2.0\n"
		   	   "The Variable i is constant equals to sqrt(-1) in any case\n"
		   	   "The Variable nan is constant equals to 0/0 in any case\n"
		   	   "The Variable inf is constant equals to 1/0 in any case\n"
 		   	   "The Variable e is constant equals to exp(1) in any case\n"
 		   	   "The Variable pi is constant equals to arccos(-1) in any case\n"
 		   	   "Supports operator: ^ * / + -\n"
 		   	   "Supports function: sin cos tan arcsin arccos arctan\n"
		   	   "                   sinh cosh tanh arcsinh arccosh arctanh\n"
 		   	   "                   abs conj proj real imag arg log lg ln sqrt exp\n"
 		   	   "Supports number set:complex\n");	   
		if(expr!=NULL){
			printf("%s=%s\n",expr,ans);
			expr=realloc(expr,0);
			ans=realloc((void *)ans,0);
		}else{
			/*Pass*/
		}
		printf(">>>");
		expr=agets(stdin);
		ans=tostr_number(eval(expr,NULL));
	}
}
