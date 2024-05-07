/*
 *     MakePPF3.c
 *     written by Icarus/Paradox
 *     suggestions and some fixes by <Hu Kares>, thanks.
 *
 *     Creates PPF3.0 Patches.
 *     Feel free to use this source in and for your own
 *     programms.
 *
 *     Visual C++ is needed in order to compile this source.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


//////////////////////////////////////////////////////////////////////
// Used global variables.
int ppf=0, bin=0, mod=0, fileid=0;


void *a, *b;
unsigned char *x, *y;
typedef struct Arg
{
	int  undo;
	int  blockcheck;
	int  imagetype;
	int  desc;
	int  fileid;
	char *description;
	char *fileidname;
	char *origname;
	char *modname;
	char *ppfname;
}Argumentblock;
Argumentblock Arg;

//////////////////////////////////////////////////////////////////////
// Used prototypes.
int		OpenFilesForCreate(void);
int		PPFCreateHeader(void);
int		PPFGetChanges(void);
int		WriteChanges(int amount, __int64 chunk);
int		CheckIfPPF3(void);
int		CheckIfFileId(void);
int		PPFAddFileId(void);
void	CloseAllFiles(void);
void	PPFCreatePatch(void);
void	CheckSwitches(int argc, char **argv);
void	PPFShowPatchInfo(void);

//////////////////////////////////////////////////////////////////////
// Main routine.
int main(int argc, char **argv){
	printf("MakePPF v3.0 by =Icarus/Paradox= %s\n", __DATE__);
	if(argc==1){
		printf("Usage: PPF <command> [-<sw> [-<sw>...]] <original bin> <modified bin> <ppf>\n");
		printf("<Commands>\n");
		printf("  c : create PPF3.0 patch            a : add file_id.diz\n");
		printf("  s : show patchinfomation\n");
		printf("<Switches>\n");
		printf(" -u        : include undo data (default=off)\n");
		printf(" -x        : disable patchvalidation (default=off)\n");
		printf(" -i [0/1]  : imagetype, 0 = BIN, 1 = GI (default=bin)\n");
		printf(" -d \"text\" : use \"text\" as description\n");
		printf(" -f \"file\" : add \"file\" as file_id.diz\n");

		printf("\nExamples: PPF c -u -i 1 -d \"my elite patch\" game.bin patch.bin output.ppf\n");
		printf("          PPF a patch.ppf myfileid.txt\n");
		return(0);
	}

	//////////////////////////////////////////////////////////////////////
	// Setting defaults.
	Arg.undo=0;
	Arg.blockcheck=1;
	Arg.imagetype=0;
	Arg.fileid=0;
	Arg.desc=0;

	switch(*argv[1]){
	case 'c':			if(argc<5){
							printf("Error: need more input for command '%s'\n",argv[1]);
							break;
						}
						CheckSwitches(argc, argv);
						if(OpenFilesForCreate()){
							PPFCreatePatch();
						} else {
							break;
						}
						CloseAllFiles();
						break;

		case 's':		
						if(argc<3){
							printf("Error: need more input for command '%s'\n",argv[1]);
							break;
						}
						ppf=_open(argv[2], _O_RDONLY | _O_BINARY);
						if(ppf==-1){
							printf("Error: cannot open file '%s': ",argv[2]); perror("");
							break;
						}
						if(!CheckIfPPF3()){
							printf("Error: file '%s' is no PPF3.0 patch\n", argv[2]);
							_close(ppf);
							break;
						}

						PPFShowPatchInfo();
						_close(ppf);
						break;
		case 'a':		
						if(argc<4){
							printf("Error: need more input for command '%s'\n",argv[1]);
							break;
						}
						ppf=_open(argv[2], _O_BINARY | _O_RDWR);
						if(ppf==-1){
							printf("Error: cannot open file '%s': ",argv[2]); perror("");
							break;
						}

						fileid=_open(argv[3], _O_RDONLY | _O_BINARY);
						if(fileid==-1){
							printf("Error: cannot open file '%s': ",argv[3]); perror("");
							CloseAllFiles();
							break;
						}
						
						if(!CheckIfPPF3()){
							printf("Error: file '%s' is no PPF3.0 patch\n", argv[2]);
							CloseAllFiles();
							break;
						}

						if(!CheckIfFileId()){
							PPFAddFileId();
						} else {
							printf("Error: patch already contains a file_id.diz\n");
						}

						CloseAllFiles();
						break;
		default :		printf("Error: unknown command '%s'\n",argv[1]); break;
	}

	printf("Done.\n");
	return(0);
}

//////////////////////////////////////////////////////////////////////
// Start to create the patch.
void PPFCreatePatch(void){

	if(PPFCreateHeader()){ printf("Error: headercreation failed\n"); return; }

	PPFGetChanges();
	if(Arg.fileid) PPFAddFileId();

	return;
}

//////////////////////////////////////////////////////////////////////
// Create PPF3.0 Header.
// Return: 1 - Failed
// Return: 0 - Success
int PPFCreateHeader(void){
	unsigned char method=0x02, description[128], binblock[1024], dummy=0, i=0;
	unsigned char magic[]="PPF30";

	printf("Writing header... "); fflush(stdout);
	memset(description,0x20,50);

	if(Arg.desc){
		for(i=0;i<strlen(Arg.description);i++){
			description[i]=Arg.description[i];
		}
	}

	if((_write(ppf, &magic, 5)) == -1 ) return(1);
	if((_write(ppf, &method, 1)) == -1 ) return(1);
	if((_write(ppf, &description, 50)) == -1 ) return(1);
	if((_write(ppf, &Arg.imagetype, 1)) == -1 ) return(1);
	if((_write(ppf, &Arg.blockcheck, 1)) == -1 ) return(1);
	if((_write(ppf, &Arg.undo, 1)) == -1 ) return(1);
	if((_write(ppf, &dummy, 1)) == -1 ) return(1);

	if(Arg.blockcheck){
		if(Arg.imagetype){
			_lseeki64(bin,0x80A0,SEEK_SET);
		} else {
			_lseeki64(bin,0x9320,SEEK_SET);
		}
		_read(bin,&binblock,1024);
		if((_write(ppf, &binblock, 1024)) == -1 ) return(1);
	}

	printf("done.\n");
	return(0);
}


//////////////////////////////////////////////////////////////////////
// Part of the PPF3.0 algorythm to find file-changes. Please note that
// 16 MegaBit is needed for the engine. Allocated by malloc();
// Return: 1 - Failed
// Return: 0 - Success
int PPFGetChanges(void){
	int read=0, eightmb=1048576, changes=0;
	unsigned long found=0;
	__int64 chunk=0, filesize;
	float percent;

	//Allocate memory (8 Megabit = 1 Chunk)
	a=malloc(eightmb);
	x=(unsigned char*)(a);
	if(x==NULL){ printf("Error: insufficient memory available\n"); return(1); }

	//Allocate memory (8 Megabit = 1 Chunk)	
	b=malloc(eightmb);
	y=(unsigned char*)(b);
	if(y==NULL){ printf("Error: insufficient memory available\n"); free(x); return(1); }

	filesize=_filelengthi64(bin);
	if(filesize == 0){ printf("Error: filesize of bin file is zero!\n"); free(x); free(y); return(1);}

	_lseeki64(bin,0,SEEK_SET);
	_lseeki64(mod,0,SEEK_SET);

	printf("Finding differences... \n");
	printf("Progress: "); fflush(stdout);
	do{
		read=_read(bin,x,eightmb);
		if(read!=0){
			if(read==eightmb){
				_read(mod,y,eightmb);
				changes=WriteChanges(eightmb, chunk);
			} else {
				_read(mod,y,read);
				changes=WriteChanges(read, chunk);
			}			
		}
		chunk+=eightmb;
		found+=changes;

		percent=(float)chunk/filesize;
		printf("%6.2f %%\b\b\b\b\b\b\b\b",percent*100); fflush(stdout);

	} while (read!=0);

	printf("100.00 %% (%d entries found).\n", found);

	//Free memory.
	free(x); free(y);
	return(0);
}

//////////////////////////////////////////////////////////////////////
// This function actually scans the 8 Mbit blocks and writes down the
// patchdata.
// Return: Found differences.
int WriteChanges(int amount, __int64 chunk){
	int found=0;
	unsigned __int64 i=0, offset;

	for(i=0;i<amount;i++){
		if(x[i]!=y[i]){
			unsigned char k=0;
			offset=chunk+i;
			_write(ppf, &offset,8);
			do{				
				k++; i++;
			} while (i<amount&&x[i]!=y[i]&&k!=0xff);			  			
			_write(ppf, &k, 1);
			_write(ppf, &y[i-k], k);
			found++;
			if(Arg.undo){
				_write(ppf, &x[i-k], k); // Write undo data aswell
			}
			if(k==0xff) i--;
		}
	}
	return(found);
}


//////////////////////////////////////////////////////////////////////
// Check all switches given in commandline and fill Arg structure.
// Return: 0 - Failed
// Return: 1 - Success
int OpenFilesForCreate(void){

	bin=_open(Arg.origname, _O_RDONLY | _O_BINARY | _O_SEQUENTIAL);
	if(bin==-1){
		printf("Error: cannot open file '%s': ",Arg.origname); perror("");
		CloseAllFiles();
		return(0);
	}	
	mod=_open(Arg.modname,  _O_RDONLY | _O_BINARY | _O_SEQUENTIAL);
	if(mod==-1){
		printf("Error: cannot open file '%s': ",Arg.modname); perror("");
		CloseAllFiles();
		return(0);
	}

	//Check if files have same size.
	if((_filelengthi64(bin)) != (_filelengthi64(mod))){
		printf("Error: bin files are different in size.");
		CloseAllFiles();
		return(0);
	}

	if(Arg.fileid){
		fileid=_open(Arg.fileidname, _O_RDONLY | _O_BINARY);
		if(fileid==-1){
			printf("Error: cannot open file '%s': ",Arg.fileidname); perror("");
			CloseAllFiles();
			return(0);
		}
	}


	ppf=_open(Arg.ppfname, _O_BINARY | _O_CREAT | _O_RDWR, _S_IREAD | _S_IWRITE);
	if(ppf==-1){
		printf("Error: cannot create file '%s': ",Arg.ppfname); perror("");
		CloseAllFiles();
		return(0);
	}


	return(1);
}

//////////////////////////////////////////////////////////////////////
// Closing all files which are currently opened.
void CloseAllFiles(void){
	if(ppf>0) _close(ppf);
	if(bin>0) _close(bin);
	if(mod>0) _close(mod);
	if(fileid>0) _close(fileid);
}

//////////////////////////////////////////////////////////////////////
// Check if a file_id.diz is available.
// Return: 0 - No file_id.diz
// Return: 1 - Yes.
int CheckIfFileId(){
	unsigned int chkmagic;

	_lseeki64(ppf,-6,SEEK_END);
	_read(ppf, &chkmagic, 4);
	if(chkmagic=='ZID.'){ return(1); }
	
	return(0);
}

//////////////////////////////////////////////////////////////////////
// Check if a file is a PPF3.0 Patch.
// Return: 0 - No PPF3.0
// Return: 1 - PPF3.0
int CheckIfPPF3(){
	unsigned int chkmagic;

	_lseeki64(ppf,0,SEEK_SET);
	_read(ppf, &chkmagic,4);
	if(chkmagic=='3FPP'){ return(1); }
	
	return(0);
}

//////////////////////////////////////////////////////////////////////
// Show various patch information. (PPF3.0 Only)
void PPFShowPatchInfo(void){
	unsigned char x, desc[50], id[3072];
	unsigned short y;

	printf("Showing patchinfo... \n");
	printf("Version     : PPF3.0\n");
	printf("Enc.Method  : 2\n");
	_lseeki64(ppf,56,SEEK_SET);
	_read(ppf, &x, 1);
	printf("Imagetype   : ");
	if(!x){
		printf("BIN\n");
	} else {
		printf("GI\n");
	}

	_lseeki64(ppf,57,SEEK_SET);
	_read(ppf, &x, 1);
	printf("Validation  : ");
	if(!x){
		printf("Disabled\n");
	} else {
		printf("Enabled\n");
	}

	_lseeki64(ppf,58,SEEK_SET);
	_read(ppf, &x, 1);
	printf("Undo Data   : ");
	if(!x){
		printf("Not available\n");
	} else {
		printf("Available\n");
	}
	
	_lseeki64(ppf,6,SEEK_SET);
	_read(ppf, &desc, 50);
	desc[50]=0;
	printf("Description : %s\n",desc);

	printf("File.id_diz : ");
	if(!CheckIfFileId()){
		printf("Not available\n");
	} else {
		printf("Available\n");
		_lseeki64(ppf,-2,SEEK_END);
		_read(ppf, &y, 2);
       if (y > 3072) {
			y = 3072;
       }
		_lseeki64(ppf,-(y+18),SEEK_END);
		_read(ppf, &id, y);
		id[y]=0;
		printf("%s\n",id);
	}	
}

//////////////////////////////////////////////////////////////////////
// This routine adds a file_id.diz to a PPF3.0 patch.
// Return: 0 - Okay
// Return: 1 - Failed.
int PPFAddFileId(void){
	unsigned short fileidlength=0;
	unsigned char fileidstart[]="@BEGIN_FILE_ID.DIZ", fileidend[]="@END_FILE_ID.DIZ", buffer[3072];

	_lseeki64(fileid,0,SEEK_END);
	fileidlength=(unsigned short)_tell(fileid);
	if(fileidlength>3072) fileidlength=3072;
	_lseeki64(fileid,0,SEEK_SET);

	_read(fileid, &buffer, fileidlength);
	_lseeki64(ppf,0,SEEK_END);

	printf("Adding file_id.diz... "); fflush(stdout);

	if((_write(ppf, &fileidstart, 18)) == -1) return(1);
	if((_write(ppf, &buffer, fileidlength)) == -1) return(1);
	if((_write(ppf, &fileidend, 16)) == -1) return(1);
	if((_write(ppf, &fileidlength, 2)) == -1) return(1);
	
	printf("done.\n");
	return(0);
}

//////////////////////////////////////////////////////////////////////
// Check all switches given in commandline and fill Arg structure.
void CheckSwitches(int argc, char **argv){
	int i;
	unsigned char *x;

	for(i=2;i<argc;i++){
		x=(unsigned char*)(argv[i]);
		if(x[0]=='-'){
		
			switch(x[1]){
				case 'u'	:	Arg.undo=1; break;
				case 'x'	:	Arg.blockcheck=0; break;
				case 'i'	:	if(*argv[i+1]=='0') Arg.imagetype=0;
								if(*argv[i+1]=='1') Arg.imagetype=1;
								i++;
								break;
				case 'd'	:	Arg.desc=1; Arg.description=argv[i+1];
								i++;
								break;
				case 'f'	:	Arg.fileid=1; Arg.fileidname=argv[i+1];
								i++;
								break;
				default		:	break;
			}

		}
	}
	Arg.ppfname=argv[argc-1];
	Arg.modname=argv[argc-2];
	Arg.origname=argv[argc-3];
}
