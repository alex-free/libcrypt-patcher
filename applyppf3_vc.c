/*
 *     ApplyPPF3.c
 *     written by Icarus/Paradox
 *     suggestions and some fixes by <Hu Kares>, thanks.
 *
 *     Applies PPF1.0, PPF2.0 & PPF3.0 Patches (including PPF3.0 Undo support)
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
int ppf, bin;
char binblock[1024], ppfblock[1024];
unsigned char ppfmem[512];
#define APPLY 1
#define UNDO 2

//////////////////////////////////////////////////////////////////////
// Used prototypes.
int		OpenFiles(char* file1, char* file2);
int		PPFVersion(int ppf);
void	ApplyPPF1Patch(int ppf, int bin);
void	ApplyPPF2Patch(int ppf, int bin);
void	ApplyPPF3Patch(int ppf, int bin, char mode);
int		ShowFileId(int ppf, int ppfver);

int main(int argc, char **argv){
	printf("ApplyPPF v3.0 by =Icarus/Paradox= %s\n", __DATE__);
	if(argc!=4){
		printf("Usage: ApplyPPF <command> <binfile> <patchfile>\n");
		printf("<Commands>\n");
		printf("  a : apply PPF1/2/3 patch\n");
		printf("  u : undo patch (PPF3 only)\n");

		printf("\nExample: ApplyPPF.exe a game.bin patch.ppf\n");
		return(0);
	}

	switch(*argv[1]){
			case 'a'	:	if(OpenFiles(argv[2], argv[3])) return(0);
							int x;
							x=PPFVersion(ppf);
							if(x){
								if(x==1){ ApplyPPF1Patch(ppf, bin); break; }
								if(x==2){ ApplyPPF2Patch(ppf, bin); break; }
								if(x==3){ ApplyPPF3Patch(ppf, bin, APPLY); break; }
							} else{ break; }
							break;
			case 'u'	:	if(OpenFiles(argv[2], argv[3])) return(0);
							x=PPFVersion(ppf);
							if(x){
								if(x!=3){
									printf("Undo function is supported by PPF3.0 only\n");
								} else {
									ApplyPPF3Patch(ppf, bin, UNDO);
								}
							} else{ break; }
							break;

			default		:
							printf("Error: unknown command: \"%s\"\n",argv[1]);
							return(0);
							break;
	}

	_close(bin);
	_close(ppf);
	return(0);
}

//////////////////////////////////////////////////////////////////////
// Applies a PPF1.0 patch.
void ApplyPPF1Patch(int ppf, int bin){
	char desc[50];
	int pos;
	unsigned int count, seekpos;
	unsigned char anz;


	_lseeki64(ppf, 6,SEEK_SET);  /* Read Desc.line */
	_read(ppf, &desc, 50); desc[50]=0;
	printf("Patchfile is a PPF1.0 patch. Patch Information:\n");
	printf("Description : %s\n",desc);
	printf("File_id.diz : no\n");

	printf("Patching... "); fflush(stdout);
	_lseeki64(ppf, 0, SEEK_END);
	count=_tell(ppf);
	count-=56;
	seekpos=56;
	printf("Patching ... ");

	do{
		printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		_lseeki64(ppf, seekpos, SEEK_SET);
		_read(ppf, &pos, 4);
		_read(ppf, &anz, 1);
		_read(ppf, &ppfmem, anz);
		_lseeki64(bin, pos, SEEK_SET);
		printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		_write(bin, &ppfmem, anz);
		seekpos=seekpos+5+anz;
		count=count-5-anz;
	} while(count!=0);

	printf("successful.\n");

}

//////////////////////////////////////////////////////////////////////
// Applies a PPF2.0 patch.
void ApplyPPF2Patch(int ppf, int bin){
		char desc[50], in;
		unsigned int binlen, obinlen, count, seekpos;
		int idlen, pos;
		unsigned char anz;



		_lseeki64(ppf, 6,SEEK_SET);
		_read(ppf, &desc, 50); desc[50]=0;
		printf("Patchfile is a PPF2.0 patch. Patch Information:\n");
		printf("Description : %s\n",desc);
		printf("File_id.diz : ");
		idlen=ShowFileId(ppf, 2);
		if(!idlen) printf("not available\n");

		_lseeki64(ppf, 56, SEEK_SET);
		_read(ppf, &obinlen, 4);

        _lseeki64(bin, 0, SEEK_END);
        binlen=_tell(bin);
        if(obinlen!=binlen){
			printf("The size of the bin file isn't correct, continue ? (y/n): "); fflush(stdout);
			in=getc(stdin);
			if(in!='y'&&in!='Y'){
				printf("Aborted...\n");
				return;
			}
		}

		fflush(stdin);
		_lseeki64(ppf, 60, SEEK_SET);
		_read(ppf, &ppfblock, 1024);
		_lseeki64(bin, 0x9320, SEEK_SET);
		_read(bin, &binblock, 1024);
		in=memcmp(ppfblock, binblock, 1024);
		if(in!=0){
			printf("Binblock/Patchvalidation failed. continue ? (y/n): "); fflush(stdout);
			in=getc(stdin);
			if(in!='y'&&in!='Y'){
				printf("Aborted...\n");
				return;
			}
		}

		printf("Patching... "); fflush(stdout);
		_lseeki64(ppf, 0, SEEK_END);
		count=_tell(ppf);
		seekpos=1084;
		count-=1084;
		if(idlen) count-=idlen+38;

        do{
			printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
			_lseeki64(ppf, seekpos, SEEK_SET);
			_read(ppf, &pos, 4);
			_read(ppf, &anz, 1);
			_read(ppf, &ppfmem, anz);
			_lseeki64(bin, pos, SEEK_SET);
			printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
			_write(bin, &ppfmem, anz);
			seekpos=seekpos+5+anz;
			count=count-5-anz;
        } while(count!=0);

		printf("successful.\n");
}

//////////////////////////////////////////////////////////////////////
// Applies a PPF3.0 patch.
void ApplyPPF3Patch(int ppf, int bin, char mode){
	unsigned char desc[50], imagetype=0, undo=0, blockcheck=0, in;
	int idlen;
	__int64 offset, count;
	unsigned int seekpos;
	unsigned char anz=0;


	_lseeki64(ppf, 6,SEEK_SET);  /* Read Desc.line */
	_read(ppf, &desc, 50); desc[50]=0;
	printf("Patchfile is a PPF3.0 patch. Patch Information:\n");
	printf("Description : %s\n",desc);
	printf("File_id.diz : ");

	idlen=ShowFileId(ppf, 3);
	if(!idlen) printf("not available\n");

	_lseeki64(ppf, 56, SEEK_SET);
	_read(ppf, &imagetype, 1);
	_lseeki64(ppf, 57, SEEK_SET);
	_read(ppf, &blockcheck, 1);
	_lseeki64(ppf, 58, SEEK_SET);
	_read(ppf, &undo, 1);

	if(mode==UNDO){
		if(!undo){
			printf("Error: no undo data available\n");
			return;
		}
	}

	if(blockcheck){
		fflush(stdin);
		_lseeki64(ppf, 60, SEEK_SET);
		_read(ppf, &ppfblock, 1024);

		if(imagetype){
			_lseeki64(bin, 0x80A0, SEEK_SET);
		} else {
			_lseeki64(bin, 0x9320, SEEK_SET);
		}
		_read(bin, &binblock, 1024);
		in=memcmp(ppfblock, binblock, 1024);
		if(in!=0){
			printf("Binblock/Patchvalidation failed. continue ? (y/n): "); fflush(stdout);
			in=getc(stdin);
			if(in!='y'&&in!='Y'){
				printf("Aborted...\n");
				return;
			}
		}
	}

	_lseeki64(ppf, 0, SEEK_END);
	count=_tell(ppf);
	_lseeki64(ppf, 0, SEEK_SET);
	
	if(blockcheck){
		seekpos=1084;
		count-=1084;
	} else {
		seekpos=60;
		count-=60;
	}

	if(idlen) count-=(idlen+18+16+2);
	

	printf("Patching ... ");
	_lseeki64(ppf, seekpos, SEEK_SET);
	do{
		printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		_read(ppf, &offset, 8);
		_read(ppf, &anz, 1);

		if(mode==APPLY){
			_read(ppf, &ppfmem, anz);
			if(undo) _lseeki64(ppf, anz, SEEK_CUR);
		}

		if(mode==UNDO){
			_lseeki64(ppf, anz, SEEK_CUR);
			_read(ppf, &ppfmem, anz);
		}

		printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);		
		_lseeki64(bin, offset, SEEK_SET);
		_write(bin, &ppfmem, anz);
		count-=(anz+9);
		if(undo) count-=anz;

	} while(count!=0);

		printf("successful.\n");

}


//////////////////////////////////////////////////////////////////////
// Shows File_Id.diz of a PPF2.0 / PPF3.0 patch.
// Input: 2 = PPF2.0
// Input: 3 = PPF3.0
// Return 0 = Error/no fileid.
// Return>0 = Length of fileid.
int ShowFileId(int ppf, int ppfver){
	char buffer2[3073];
	unsigned int idmagic;
	int lenidx=0, idlen=0, orglen=0;


	if(ppfver==2){
		lenidx=4;
	} else {
		lenidx=2;
	}

	_lseeki64(ppf,-(lenidx+4),SEEK_END);
	_read(ppf, &idmagic, 4);
	if(idmagic!='ZID.'){
		return(0);
	} else {
		_lseeki64(ppf,-lenidx,SEEK_END);
		_read(ppf, &idlen, lenidx);
        orglen = idlen;
        if (idlen > 3072) {
			idlen = 3072;
        }
		_lseeki64(ppf,-(lenidx+16+idlen),SEEK_END);
		_read(ppf, &buffer2, idlen);
		buffer2[idlen]=0;
		printf("available\n%s\n",buffer2);
	}

	return(orglen);
}

//////////////////////////////////////////////////////////////////////
// Check what PPF version we have.
// Return: 0 - File is no PPF.
// Return: 1 - File is a PPF1.0
// Return: 2 - File is a PPF2.0
// Return: 3 - File is a PPF3.0
int PPFVersion(int ppf){
	unsigned int magic;

	_lseeki64(ppf,0,SEEK_SET);
	_read(ppf, &magic, 4);
	switch(magic){
			case '1FPP'		:	return(1);
			case '2FPP'		:	return(2);
			case '3FPP'		:	return(3);
			default			:   printf("Error: patchfile is no ppf patch\n"); break;
	}

	return(0);
}


//////////////////////////////////////////////////////////////////////
// Open all needed files.
// Return: 0 - Successful
// Return: 1 - Failed.
int OpenFiles(char* file1, char* file2){

	bin=_open(file1, _O_BINARY | _O_RDWR);
	if(bin==-1){
		printf("Error: cannot open file '%s': ",file1); perror("");
		return(1);
	}

	ppf=_open(file2,  _O_RDONLY | _O_BINARY);
	if(ppf==-1){
		printf("Error: cannot open file '%s': ",file2); perror("");
		_close(bin);
		return(1);
	}

	return(0);
}
