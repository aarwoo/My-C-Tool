#ifndef tuple_h
#define tuple_h
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef struct tuple{
	void * data;
	void (*delete_data)(void *data);
	char * (*typeof_data)(void * data);
	struct tuple * next;
}tuple;
tuple * add_tuple(tuple * x,tuple * y){
	if(x==NULL){
		return y;
	}else if(y==NULL){
		return x;
	}else{
		tuple * tmp=y->next;
		y->next=x;
		return add_tuple(y,tmp);
	}
}
tuple make_tmp_tuple(tuple src){
	src.next=NULL;
	return src;
}
bool _foreach_tuple(tuple * _tuple,bool (*func)(tuple * x,void * ex_argv),void * ex_argv,bool is_ok){
	if(_tuple!=NULL&&func!=NULL){
		tuple tmp=make_tmp_tuple(*_tuple);
		is_ok=(*func)(&tmp,ex_argv)&&is_ok;
		if(tmp.data!=NULL&&tmp.delete_data!=NULL&&tmp.typeof_data!=NULL){
			*_tuple=(tuple){.data=tmp.data,
				         .delete_data=tmp.delete_data,
				         .typeof_data=tmp.typeof_data,
				         .next=_tuple->next};
			return _foreach_tuple(_tuple->next,func,ex_argv,is_ok);
		}else{
			puts("fmap_tuple(tuple,func,ex_argv):func is illegal");
			getchar();
			exit(1);
		}
	}else{
		return is_ok;
	}
}
bool foreach_tuple(tuple * _tuple,bool (*func)(tuple * x,void * ex_argv),void * ex_argv){
	return _foreach_tuple(_tuple,func,ex_argv,true);
}
tuple *  _remove_if_tuple(tuple * virtual_head_tuple,tuple * last_tuple,tuple * _tuple,bool (*func_if)(const tuple * x,void * ex_argv),void * ex_argv){
	if(_tuple!=NULL&&func_if!=NULL){
		tuple tmp=make_tmp_tuple(*_tuple);
		if((*func_if)(&tmp,ex_argv)==false){
			return _remove_if_tuple(virtual_head_tuple,_tuple,_tuple->next,func_if,ex_argv);
		}else{
			(*(_tuple->delete_data))(_tuple->data);
			last_tuple->next=_tuple->next;
			free(_tuple);
			return _remove_if_tuple(virtual_head_tuple,last_tuple,last_tuple->next,func_if,ex_argv);
		}
	}else{
		return virtual_head_tuple->next;
	}
}
tuple * remove_if_tuple(tuple * _tuple,bool (*func_if)(const tuple * x,void * ex_argv),void * ex_argv){
	tuple virtual_head_tuple;
	virtual_head_tuple.next=_tuple;
	return _remove_if_tuple(&virtual_head_tuple,&virtual_head_tuple,virtual_head_tuple.next,func_if,ex_argv);
}
void * reduce_tuple(tuple * _tuple,void * (*func)(void * reduce_ans,const tuple * x),void * init_reduce_ans){
	if(_tuple!=NULL&&func!=NULL){
		tuple tmp=make_tmp_tuple(*_tuple);
		return reduce_tuple(_tuple->next,func,(*func)(init_reduce_ans,&tmp));
	}else{
		return init_reduce_ans;
	}
}
tuple * new_tuple(void * data,void (*delete_data)(void *data),char * (*typeof_data)(void * data)){
	tuple * _tuple=calloc(1,sizeof(tuple));
	if(_tuple!=NULL&&data!=NULL&&delete_data!=NULL&&typeof_data!=NULL){
		*_tuple=(tuple){.data=data,
			       .delete_data=delete_data,
			       .typeof_data=typeof_data,
			       .next=NULL};
		return _tuple;
	}else{
		return _tuple;
	}
}
tuple * delete_tuple(tuple * _tuple){
	if(_tuple!=NULL){
		(*(_tuple->delete_data))(_tuple->data);
		tuple * tmp=_tuple->next;
		free(_tuple);
		return delete_tuple(tmp);
	}else{
		return _tuple;
	}
}
#endif