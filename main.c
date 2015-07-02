//========================================================
//
//	SearchPart is an utility for search 
//	signatures in files.
//
//	Author: BessonovAlexanderEvgenivich
//	Author e-mail: bessonomap@yandex.ru
//	Author web site: http://bessonov.in.ua
//	License: GPL 
//	Was tested in Ubuntu 14.04
//
//========================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

void read_dir(char *dir_name);
void read_file(char *filename, char *srcs);

static char log_file[255] = "---";
static int make_log = 0;
static int dir_not_opened = 0;
static int search_counter = 0;
static int dir_counter = 0;
static int all_file_counter = 0;
static int errors_file_counter = 0;
static char search_path[255] = "---";
static char search_signature[1024] = "---";
static char file_types[20] = "---";

main(int argc, char *argv[]){
	if((argc >= 2) && (strstr(argv[1],"-l"))){
		if(argv[2] != NULL){
			printf("Start with \"make log\" in  %s file \n",argv[2]);
			make_log = 1;
			strcpy(log_file,argv[2]);
		}
	
	}
	if((argc > 1) && (strstr(argv[1],"-h"))){
		printf("\n=======================================\n \n");
		printf(" Help for search-part utility \n");
		printf(" search-part -l [log-file-name] : will make and write in logfile all search output \n");
		printf(" Yout can use \"search-part\" without -l, just enter \"search-part\" in console and folow questions for make searching in files. \n");
		printf("\n=======================================\n ");
	exit(0);
	}
	printf("Where will searching? ");
	scanf("%s",search_path);
	printf("\nWhat do you want to find? ");
	scanf("%s",search_signature);
	printf("\nWhat a files (type \"all\" or \".php\" or some other) ");
	scanf("%s",file_types);
	if(search_path[strlen(search_path)-1] != '/'){
		strcat(search_path,"/");
	}
	if(strlen(file_types) < 1) {
		printf("\n You did not wrote file_type! \n");
		exit(0);
	}
	if(strlen(search_signature) == 0){
		printf("\n You did not wrote search signature! \n");
		exit(0);
	}
	read_dir(search_path);
	printf("\n ================================== \n Was found %d \n", search_counter);
	printf(" Looked in %d files \n", all_file_counter );
	printf(" Total directory %d \n",dir_counter);
	printf(" Not opened directory %d \n",dir_not_opened);
	printf(" Not opened files %d \n", errors_file_counter);
	printf(" ================================= \n");
return 0;
}

void read_dir(char *dirname){
	//printf("read dir -- %s \n",dirname);
	char new_dir[255] = "---";
	char file_path[555] = "---";
	DIR *m_dir;
	struct dirent *items;
	m_dir = opendir(dirname);
	if(m_dir != NULL){
		dir_counter++;
		while((items = readdir(m_dir)) != NULL){
			//printf("%d | %s %d \n ",file_counter,items->d_name,items->d_type);
			if(items->d_type == 8) {
				strcpy(file_path,dirname);
				strcat(file_path,items->d_name);
				if(strstr(file_types,"all")){
					read_file(file_path,search_signature);
				} else {
					if(strstr(file_path,file_types)){
					read_file(file_path,search_signature);
					}
				}
			}
			if((items->d_type == 4) && (strcmp(items->d_name,".")) && (strcmp(items->d_name,".."))){
				strcpy(new_dir,dirname);
				strcat(new_dir,items->d_name);
				strcat(new_dir,"/");
				read_dir(new_dir);	
			}
		}
	closedir(m_dir);
	} else {
		dir_not_opened++;
		printf("CannotOpenDir %s \n",dirname);
	}
	

}

void read_file(char *file_name, char *srcs){
	
	FILE *filep;
	FILE *log;
	int i = 0;
	char fline[1124] = "-";
	char cur_line[1024] = "--";
	if(!strlen(file_name) < 1){
		filep = fopen(file_name,"r");
		if(filep != NULL){
			all_file_counter++;
			while((fgets(cur_line,1024,filep)) != NULL){
				i++;
				if(strstr(cur_line,srcs)){
					search_counter++;
					printf("WAS FOUND IN %d::%s in line %s \n",i,file_name,cur_line);
					if(make_log == 1){
						log = fopen(log_file,"a+");
						sprintf(fline,"\n ======== \n WAS FOUND IN %d::%s in line %s \n",i,file_name,cur_line);
						fputs(fline,log);
						fclose(log);
					}			
				}		
				
				
			}		
		close(filep);
		} else {
			printf("CanNotOpenFile %s \n",file_name);
			errors_file_counter++;
		} 
	}
	
}

