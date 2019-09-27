#ifndef LINE_TREE
#define LINE_TREE
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
typedef int val_t;
typedef struct _line_tree * line_tree;
typedef struct _line_tree{
	line_tree l;
	line_tree r;
	size_t begin;
	size_t end;
	val_t val;
}_line_tree;
line_tree _new_line_tree(val_t * vals,size_t begin ,size_t end,val_t (*fold)(val_t,val_t)){
	line_tree tmp=(line_tree)calloc(1,sizeof (_line_tree));
	size_t mid=(begin+end)/2;
	if(tmp==NULL){
		perror("Memory full!");
	}else{
		/*Pass*/
	}
	tmp->begin=begin;
	tmp->end=end;
	if(begin==end){
		tmp->val=*(vals+begin);
	}else{
		tmp->l=_new_line_tree(vals,begin,mid,fold);
		tmp->r=_new_line_tree(vals,mid+1,end,fold);
		tmp->val=(*fold)(tmp->l->val,tmp->r->val);
	}
	return tmp;
}
line_tree new_line_tree(val_t * vals,size_t size,val_t (*fold)(val_t,val_t)){
	if(vals==NULL||fold==NULL){
		return NULL;
	}else{
		return _new_line_tree(vals,0,size-1,fold);
	}
}
void delete_line_tree(line_tree x){
	if(x==NULL){
		/*Pass*/
	}else{
		delete_line_tree(x->l);
		delete_line_tree(x->r);
		free(x);
	}
	return;
}
val_t _valof_line_tree(line_tree x,size_t begin,size_t end,val_t (*fold)(val_t,val_t)){
	if(x->begin==begin&&x->end==end){
		return x->val;
	}else{
		size_t mid=(x->begin+x->end)/2;
		if(mid<begin){
			return _valof_line_tree(x->r,begin,end,fold);
		}else if(mid<end){
			return (*fold)(_valof_line_tree(x->l,begin,mid,fold),_valof_line_tree(x->r,mid+1,end,fold));
		}else{
			return _valof_line_tree(x->l,begin,end,fold);
		}
	}
}
val_t valof_line_tree(line_tree x,size_t begin,size_t end,val_t (*fold)(val_t,val_t)){
	if(x==NULL||fold==NULL){
		return 0;
	}else{
		if(begin>end){
			size_t tmp=begin;
			begin=end;
			end=tmp;
		}else{
			/*Pass*/ 
		}
		begin=max(begin,x->begin);
		end=min(end,x->end);
		return _valof_line_tree(x,begin,end,fold);
	}
}
void _fmap_line_tree(line_tree x,val_t (*f)(val_t),size_t begin,size_t end,val_t (*fold)(val_t,val_t)){
	if(x->begin==begin&&x->end==end&&begin==end){
		x->val=(*f)(x->val);
	}else{
		size_t mid=(x->begin+x->end)/2;
		if(mid<begin){
			_fmap_line_tree(x->r,f,begin,end,fold);
		}else if(mid<end){
			_fmap_line_tree(x->l,f,begin,mid,fold);
			_fmap_line_tree(x->r,f,mid+1,end,fold);
		}else{
			_fmap_line_tree(x->l,f,begin,end,fold);
		}
		x->val=(*fold)(x->l->val,x->r->val);
	}
	return;
}
line_tree fmap_line_tree(line_tree x,val_t (*f)(val_t),size_t begin,size_t end,val_t (*fold)(val_t,val_t)){
	if(x==NULL||f==NULL||fold==NULL){
		/*Pass*/
	}else{
		if(begin>end){
			size_t tmp=begin;
			begin=end;
			end=tmp;
		}else{
			/*Pass*/ 
		}
		begin=max(begin,x->begin);
		end=min(end,x->end);
		_fmap_line_tree(x,f,begin,end,fold);
	}
	return x;
}
val_t _modify_line_tree(val_t x){
	static bool mark=false;
	static val_t new_val;
	if(mark==false){
		new_val=x;
		mark=true;
	}else{
		mark=false;
	}
	return new_val;
}
line_tree modify_line_tree(line_tree x,size_t at,val_t new_val,val_t (*fold)(val_t,val_t)){
	_modify_line_tree(new_val);
	return fmap_line_tree(x,_modify_line_tree,at,at,fold);
}
#endif
