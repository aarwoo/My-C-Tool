/*It maybe unsafety.*/
#ifndef EVAL
#define EVAL
#include<tgmath.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
typedef long double complex number;
number eval(const char * expr,number (*var_map)(const char * var));
/**
 *eval:calcutate an expression
 *expr:expression
 *var_map:A function that receive variable name and return its value,it could be NULL
 *The Variable i is constant equals to sqrt(-1) in any case
 *The Variable nan is constant equals to 0/0 in any case 
 *The Variable inf is constant equals to 1/0 in any case 
 *The Variable e is constant equals to exp(1) in any case
 *The Variable pi is constant equals to arccos(-1) in any case
 *Supports operator: ^ * / + -
 *Supports function: sin cos tan arcsin arccos arctan sinh cosh tanh arcsinh arccosh arctanh 
 *					 abs conj proj real imag arg log lg ln sqrt exp
 *Supports number set:complex
 */
const char * astrncpy(const char * src,size_t length){
	char * dst=calloc(length+1,sizeof(char));
	if(dst!=NULL){
		memmove(dst,src,length*sizeof(char));
	}else{
		/*Pass*/
	}
	return dst;
}
const char * tostr_number(number x){
	long double a=creal(x),b=cimag(x);
	char tmp[2][256];
	char * w,* r;
	if(b==0){
		__mingw_snprintf(tmp[0],256,"%.15Lg",a);
	}else{
		if(a==0){
			if(fabs(b)==1){
				__mingw_snprintf(tmp[0],256,"%si",b>0?"":"-");
			}else{
				__mingw_snprintf(tmp[0],256,"%s%.15Lg*i",(b>0||isnan(b)==true)?"+":"-",fabs(b));
			}
		}else{
			if(fabs(b)==1){
				__mingw_snprintf(tmp[0],256,"%.15Lg%si",a,(b>0||isnan(b)==true)?"+":"-");
			}else{
				__mingw_snprintf(tmp[0],256,"%.15Lg%s%.15Lg*i",a,(b>0||isnan(b)==true)?"+":"-",fabs(b));
			}
		}
	}
	w=tmp[1];
	for(r=tmp[0];*r!='\0';r=r+1){
		if(*r!='e'){
			*w=*r;
			w=w+1;
		}else{
			*w='*';
			*(w+1)='1';
			*(w+2)='0';
			*(w+3)='^';
			w=w+4;
			for(/*Pass*/;*r!='\0';r=r+1){
				if(*r=='e'||*r=='+'||*r=='0'){
					/*Pass*/
				}else{
					break;
				}
			}
			r=r-1;
		}
	}
	*w='\0';
	return astrncpy(tmp[1],strlen(tmp[1]));
}
const char * skip_sub_expr(const char * expr){
	char match_list[]=")(";
	ptrdiff_t lv_of_sub_expr=0,diff; 
	if(*expr=='('){
		while(*expr!='\0'){
			diff=strchr(match_list,*expr)-match_list;
			diff=(diff>=0)?(diff*2-1):(0);
			lv_of_sub_expr=lv_of_sub_expr+diff;
			if(lv_of_sub_expr==0){
				break;
			}else{
				/*Pass*/
			}
			expr=expr+1;
		}
	}else{
		/*Pass*/
	}
	return expr;
}
const char * expr_op(const char * expr){
	const char match_list[]="^*/+-";
	const char * op=NULL,* iter=expr;
	ptrdiff_t op_priority=-1,tmp;
	#define __check_ch__(ch,xch) (('0'<=(ch)&&(ch)<='9')||\
								  ('a'<=(ch)&&(ch)<='z')||('A'<=(ch)&&(ch)<='Z')||\
								  ((ch)==(xch)))
	if(iter!=NULL){
		while(*iter!='\0'){
			tmp=strchr(match_list,*iter)-match_list;
			tmp=tmp+tmp%2;
			if((op_priority<=tmp)&&
			   (iter>expr&&__check_ch__(*(iter-1),')'))&&
			   (__check_ch__(*(iter+1),'(')||*(iter+1)=='-')){
				op=iter;
				op_priority=tmp;
			}else{
				/*Pass*/
			}
			iter=skip_sub_expr(iter)+1;
		}
	}else{
		/*Pass*/
	}
	return op;
}
number expr_calc(number x,const char op,number y){
	switch(op){
		case '^':return pow(x,y);
		case '*':return x*y;
		case '/':return x/y;
		case '+':return x+y;
		case '-':return x-y;
		default:return NAN;
	}
}
bool is_vaild_var(const char * var){
	while(*var!='\0'){
		if(('a'<=*var&&*var<='z')||('A'<=*var&&*var<='Z')){
			/*Pass*/
		}else{
			return false;
		}
		var=var+1;
	}
	return true;
}
bool is_vaild_number(const char * num){
	bool accept_dot=true;
	while(*num!='\0'){
		if('0'<=*num&&*num<='9'){
			/*Pass*/
		}else if(accept_dot=true&&*num=='.'){
			accept_dot=false;
		}else{
			return false;
		}
		num=num+1;
	}
	return true;
}
number eval(const char * expr,number (*var_map)(const char * var));
number sub_eval(const char * expr,size_t length,number (*var_map)(const char * var)){
	number ans;
	expr=astrncpy(expr,length);
	ans=eval(expr,var_map);
	free((void *)expr);
	return ans;
}
number atom_calc(const char * atom,number (*var_map)(const char * var)){
	number ans=NAN,sgn=1;
	const char * var,* tmp,* split_argv;
	if(atom!=NULL){
		if(*atom=='-'){
			sgn=-1;
			atom=atom+1;
		}else{
			/*Pass*/
		}
		if(('0'<=*atom&&*atom<='9')){
			if(is_vaild_number(atom)==true){
				ans=strtold(atom,NULL);
			}else{
				ans=NAN;
			}
		}else if(*atom=='('){
			ans=sub_eval(atom+1,strlen(atom)-2,var_map);
		}else{
			tmp=(const char *)strchr(atom,'(');
			var=astrncpy(atom,(tmp==NULL)?strlen(atom):(tmp-atom));
			if(tmp==NULL){
				if(var_map!=NULL&&is_vaild_var(var)==true){
					ans=(*var_map)(var);
				}else{
					ans=NAN;
				}
				if(strcmp(var,"i")==0){
					ans=I;
				}else if(strcmp(var,"inf")==0){
					ans=1.0/0.0;
				}else if(strcmp(var,"nan")==0){
					ans=NAN;
				}else if(strcmp(var,"e")==0){
					ans=exp(1);
				}else if(strcmp(var,"pi")==0){
					ans=acos(-1);
				}else{
					/*Pass*/
				}
			}else{
				#define __log__(x,y) (log(y)/log(x))
				#define __lg__(x) __log__(10,x)
				#define __func_calc__1(c_func) ans=c_func(sub_eval(tmp+1,strlen(tmp+1)-1,var_map));
				#define __func_calc__2(c_func) \
						for(split_argv=tmp+1;*(split_argv+1)!='\0';split_argv=skip_sub_expr(split_argv)+1){\
							if(*split_argv==','){\
								break;\
							}else{\
								/*Pass*/\
							}\
						}\
						ans=c_func(sub_eval(tmp+1,split_argv-(tmp+1),var_map),\
								   sub_eval(split_argv+1,strlen(split_argv+1)-1,var_map));
				#define __func_calc__(math_func,c_func,number_of_argv) \
						if(strcmp(var,#math_func)==0){\
							__func_calc__##number_of_argv(c_func)\
						}else
    			__func_calc__(sin,sin,1)
    			__func_calc__(cos,cos,1)
    			__func_calc__(tan,tan,1)
    			__func_calc__(arcsin,asin,1)
    			__func_calc__(arccos,acos,1)
    			__func_calc__(arctan,atan,1)
    			__func_calc__(sinh,sinh,1)
    			__func_calc__(cosh,cosh,1)
    			__func_calc__(tanh,tanh,1)
    			__func_calc__(arcsinh,asinh,1)
    			__func_calc__(arccosh,acosh,1)
    			__func_calc__(arctanh,atanh,1)
    			__func_calc__(abs,fabs,1)
    			__func_calc__(conj,conj,1)
    			__func_calc__(proj,cproj,1)
    			__func_calc__(real,creal,1)
    			__func_calc__(imag,cimag,1)
    			__func_calc__(arg,carg,1)
    			__func_calc__(log,__log__,2)
    			__func_calc__(lg,__lg__,1)
    			__func_calc__(ln,log,1)
    			__func_calc__(sqrt,sqrt,1)
    			__func_calc__(exp,exp,1)
    			{
    				ans=NAN;
				}
			}
		}
	}else{
		/*Pass*/
	}
	return sgn*ans;
}
bool check_expr(const char * expr){
		char match_list[]=")(";
		ptrdiff_t lv_of_sub_expr=0,diff; 
		while(*expr!='\0'){
			diff=strchr(match_list,*expr)-match_list;
			diff=(diff>=0)?(diff*2-1):(0);
			lv_of_sub_expr=lv_of_sub_expr+diff;
			if(lv_of_sub_expr<0){
				break;
			}else{
				/*Pass*/
			}
			expr=expr+1;
		}
	return lv_of_sub_expr==0;
}
number eval(const char * expr,number (*var_map)(const char * var)){
	const char * op,tmp;
	if(check_expr(op)==true){
		op=expr_op(expr);
		if(op==NULL){
			return atom_calc(expr,var_map);
		}else{
			return expr_calc(sub_eval(expr,op-expr,var_map),*op,sub_eval(op+1,strlen(op+1),var_map));
		}
	}else{
		return NAN;
	}
	
}
#endif
