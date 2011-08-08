#include <stdio.h>
/* argp */
#include <argp.h>

#define handle_error(msg) \
do { perror(msg); exit(1); } while (0)
	//declare root struct
	struct node{
		int data;
		struct node* left;
		struct node* right;
	};

struct node* makeempty(struct node*);
struct node* find(int x,struct node*);
struct node* findmin(struct node*);
struct node* findmax(struct node*);

struct node* insert(struct node*,int x);
struct node* delete(struct node*,int);
int retrive(struct node*);

int totalnodes,files_count,size_count;
char wcd[100];//use as working directory.

//make empty root
struct node* makeempty(struct node* root){
	if (root !=NULL){
		makeempty(root->left);
		makeempty(root->right);
		free(root);
		}
		
	return NULL;
}


//find for bst.

struct node* find(int x ,struct node* root){
	while( root !=NULL && x !=root->data)
	if(x < root->data)
		root=root->left;
	else 
		root=root->right;
return root;
}

//findmin
struct node* findmin(struct node* root){
struct node* current=root;
	if(root == NULL)
		return NULL;
	
	while(current->left!=NULL){
	current=current->left;
	}
	return current->data;
}



//findmax
struct node* findmax(struct node* root){
struct node* current=root;
	if(root == NULL)
		return NULL;
	
	while(current->right!=NULL){
	current=current->right;
	}
	return current->data;
}

//newnode
struct node* newnode(int data){
struct node* newnode=NULL;

	newnode=malloc(sizeof(struct node));
	newnode->data=data;
	newnode->left=NULL;
	newnode->right=NULL;
	
return newnode;
}

//insert
struct node* insert(struct node* root,int data){
	if(root == NULL)
		return (newnode(data));
	else{
	if(data <= root->data)
	root->left=insert(root->left,data);
	else 
	root->right=insert(root->right,data);
	}
return root;
}
//delete 
struct node* delete(struct node *root,int data){
	struct node* tmp;

	if (root == NULL)
		return NULL;
		
	if (data < root->data)	
		root->left=delete(root->left,data);
	else  if (data > root->data)	
		root->right=delete(root->right,data);
	else
	/* data found */
	if(root->left && root -> right) {	//tobe delete node has two child
	tmp=findmin(root->right);
	root->data=tmp;
	root->right=delete(root->right,root->data);
	}else {//one or zero child
	tmp=root;
	if(root->left == NULL)
		root=root->right;
	else if (root->right ==NULL)
		root=root->left;
	free(tmp);
	}
	
	return root;

}
void creat_files(int l){
int i=0,fp=0,s;
char fname[50];
int buf[1024];
char junk='g';

	for (;i<files_count;i++){
	sprintf(fname,"file_L%d_%d",l,i);
	s=size_count;

	fp=creat(fname,0777);
	if(fp<0){
		handle_error("\ntreeop : file creation failed");
		}
	while(s-- >0){
	memset(buf,junk,1024);
	write(fp,buf,1024);
	}
	close(fp);
	}
	
}
void create_dirs(struct node* node,int level){
	char str_path[25];
	if(node==NULL){
	if(chdir("..")<0)
	handle_error("\n treeop:chdir() failed");
	return;
	}else{
	//first set dirname , create and move into it
	sprintf(str_path,"dir_L%d_%d",level,node->data);
	mkdir(str_path,0777);

	if(chdir(str_path)<0)
	handle_error("\n treeop:chdir() failed");

	create_dirs(node->left,++level);

	//first set dirname , create and move into right 
	level--;
	sprintf(str_path,"dir_L%d_%d",level,node->data);
	mkdir(str_path,0777);
	if(chdir(str_path)<0)
	handle_error("\n treeop:chdir() failed");
	
	create_dirs(node->right,++level);

	//get back from inter-mediate nodes
	if(chdir("..")<0)
	handle_error("\n treeop:chdir() failed");
	}
}

void create_files(struct node* node,int level){
	char str_path[25];
	if(node==NULL){
	if(chdir("..")<0)
	handle_error("\n treeop:chdir() failed");
	return;
	}else{
	//first  move into it
	sprintf(str_path,"dir_L%d_%d",level,node->data);
	if(chdir(str_path)<0)
	handle_error("\n treeop:chdir() failed");
	creat_files(level);

	create_files(node->left,++level);

	// move into right 
	level--;
	sprintf(str_path,"dir_L%d_%d",level,node->data);
	if(chdir(str_path)<0)
	handle_error("\n treeop:chdir() failed");
	
	creat_files(level);
	create_files(node->right,++level);

	//get back from inter-mediate nodes
	if(chdir("..")<0)
	handle_error("\n treeop:chdir() failed");
	}
}

/* void organize_input(int item[],int size){
int n=0;
	//first sort the items
	sort(item,size);
	printf("\n");
	while(n<size)
	printf("==>%d",item[n++]);
	split_items(item,size);
} */


struct node* buildtree(struct node* root,int size){
int n=size;

	while(n--)
 	root=insert(root,rand()%(size*5));

return root;
}

const char *argp_program_version = "treeop - alpha (06-Aug-2011) ";
const char *argp_program_bug_address = "lakshmipathi.g@gmail.com";

static char args_doc[] = "<WDIR> ";
static char doc[] = "treeop - A File System Testing tool.";

/* parse arg list */
struct arguments
{
   char *args[1];		 
   char *string1,*string2,*string3;   /* arguments for -d -f -s */
};
static struct argp_option options[] =
{  
  {"dir", 'd',  "DIR", 0, "Total number of nodes/directories in tree. (default=25)"},
  {"file", 'f', "FILE", 0,"Number of files under each node/directory. (default=10)"},
  {"size", 's', "SIZE", 0,"Size count of each file in KB. (default=1)"},
  {"type", 't', "TYPE", 0,"Create specific tree layout. (default:BST)"},
  {0}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'd':
      arguments->string1=arg;
      break;
    case 'f':
      arguments->string2=arg;
      break;
    case 's':
      arguments->string3=arg;
      break;

    case ARGP_KEY_ARG:
       if (state->arg_num > 3)
	argp_usage(state);

       arguments->args[state->arg_num] = arg;    
	break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};


int main(int argc,char* argv[]){
	
	struct node* root=NULL;
	struct node* findval=NULL;


	struct arguments arguments;
	/* default values */

	arguments.string1="25";
	arguments.string2="10";
	arguments.string3="1";
	arguments.args[0]=".";

	argp_parse (&argp, argc, argv, 0, 0, &arguments);


	totalnodes=atoi(arguments.string1);
	files_count=atoi(arguments.string2);
	size_count=atoi(arguments.string3);
	strcpy(wcd,arguments.args[0]);
	if(wcd[0]!='/')
	handle_error("\ntreeop: Please provide absolute name with target dir");

	//buildtree
	root=buildtree(root,totalnodes);
	
	//min and max value
	printf("\nMinvalue of Tree:%d",findmin(root));
	printf("\nMaxvalue of Tree:%d\n",findmax(root));

	if(chdir(wcd)<0)
	handle_error("\n treeop: Can't cd into target directory.I got the message");
	create_dirs(root,0);

	if(chdir(wcd)<0)
	handle_error("\n treeop: Can't cd into target directory.I got the message");
	create_files(root,0);

	//delete item
	printf("\n Delete data from Tree:");
	if(delete(root,(findmax(root)))!=NULL)
	printf("\nDeleted Max from tree");
	else
	printf("\n  item Not found");

	//find a value
	printf("\nFind value of Tree:20");
	findval=find(20,root);
	if(findval!=NULL)
	printf("\n Found %d",findval->data);
	else
	printf("\n Data not found");

	//make empty 
	if(makeempty(root) ==NULL);
	printf("\n Tree is empty");

}
	

