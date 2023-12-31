/*
 *     ApplyPPF3.c (Linux Version)
 *     written by Icarus/Paradox
 *
 *     Big Endian support by Hu Kares.
 *
 *     Applies PPF1.0, PPF2.0 & PPF3.0 Patches (including PPF3.0 Undo support)
 *     Feel free to use this source in and for your own
 *     programms.
 *
 *     To compile enter:
 *     gcc -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE applyppf3_linux.c
 * 
 */

// NOTE: I have commented out but KEPT all the old code that has been changed and or is no longer necessary, for reference - Alex Free

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//#define DEBUG

/*
#if defined(__APPLE__) || defined (MACOSX)

//////////////////////////////////////////////////////////////////////
// fseeko is already 64 bit for Darwin/MacOS X!
// fseeko64 undefined for Darwin/MacOS X!

#define	fseeko64		fseeko

//////////////////////////////////////////////////////////////////////
// ftellko is already 64 bit for Darwin/MacOS X!
// ftello64 undefined for Darwin/MacOS X!

#define	ftello64		ftello

//////////////////////////////////////////////////////////////////////
// "off_t" is already 64 bit for Darwin/MacOS X!
// "__off64_t" undefined for Darwin/MacOS X!

typedef	off_t			__off64_t;

#endif */ /* __APPLE__ || MACOSX */

//////////////////////////////////////////////////////////////////////
// Macros for little to big Endian conversion.

#ifdef __BIG_ENDIAN__

#define Endian16_Swap(value)	(value = (((((unsigned short) value) << 8) & 0xFF00)  | \
                                          ((((unsigned short) value) >> 8) & 0x00FF)))

#define Endian32_Swap(value)    (value = (((((unsigned long) value) << 24) & 0xFF000000)  | \
                                          ((((unsigned long) value) <<  8) & 0x00FF0000)  | \
                                          ((((unsigned long) value) >>  8) & 0x0000FF00)  | \
                                          ((((unsigned long) value) >> 24) & 0x000000FF)))

#define Endian64_Swap(value)	(value = (((((unsigned long long) value) << 56) & 0xFF00000000000000ULL)  | \
                                          ((((unsigned long long) value) << 40) & 0x00FF000000000000ULL)  | \
                                          ((((unsigned long long) value) << 24) & 0x0000FF0000000000ULL)  | \
                                          ((((unsigned long long) value) <<  8) & 0x000000FF00000000ULL)  | \
                                          ((((unsigned long long) value) >>  8) & 0x00000000FF000000ULL)  | \
                                          ((((unsigned long long) value) >> 24) & 0x0000000000FF0000ULL)  | \
                                          ((((unsigned long long) value) >> 40) & 0x000000000000FF00ULL)  | \
                                          ((((unsigned long long) value) >> 56) & 0x00000000000000FFULL)))

#else

#define	Endian16_Swap(value)
#define	Endian32_Swap(value)
#define	Endian64_Swap(value)

#endif /* __BIG_ENDIAN__ */

//////////////////////////////////////////////////////////////////////
// Used global variables.
//FILE *ppf, *bin;
FILE *bin;
char binblock[1024], ppfblock[1024];
unsigned char ppfmem[512];
#define APPLY 1
#define UNDO 2

//////////////////////////////////////////////////////////////////////
// Used prototypes.
//int	PPFVersion(FILE *ppf);
//int PPFVersion(const unsigned char ppf[])
//int	OpenFiles(char* file1, char* file2);
//int	ShowFileId(FILE *ppf, int ppfver);
int	ShowFileId(const unsigned char ppf[], unsigned int ppf_len, int ppfver);
//void	ApplyPPF1Patch(FILE *ppf, FILE *bin);
//void	ApplyPPF2Patch(FILE *ppf, FILE *bin);
//void	ApplyPPF3Patch(FILE *ppf, FILE *bin, char mode);
//void	ApplyPPF1Patch(const unsigned char ppf[], FILE *bin);
//void	ApplyPPF2Patch(const unsigned char ppf[], FILE *bin);
//void	ApplyPPF3Patch(const unsigned char ppf[], FILE *bin, char mode);

/*
int main(int argc, char **argv){
	int x;
	printf("ApplyPPF v3.0 by =Icarus/Paradox= %s\n", __DATE__);
#ifdef __BIG_ENDIAN__
        printf("Big Endian support by =Hu Kares=\n\n");			// <Hu Kares> sum credz
#endif */ /* __BIG_ENDIAN__ */     
/*   
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

	fclose(bin);
	fclose(ppf);
	return(0);
}
*/
//////////////////////////////////////////////////////////////////////
// Applies a PPF1.0 patch.
//void ApplyPPF1Patch(FILE *ppf, FILE *bin){
void ApplyPPF1Patch(const unsigned char ppf[], unsigned int ppf_len, FILE *bin) {
	//char desc[50];
	char desc[51]; // old bug fixed
	unsigned int pos;
	unsigned int count, seekpos;
	unsigned char anz;


	//fseeko64(ppf, 6,SEEK_SET);  /* Read Desc.line */
	
	//printf("PPF Byte 1: %02X\n", ppf[0]);
	//printf("PPF Byte 1: %02X\n", ppf[1]);
	//printf("PPF Byte 1: %02X\n", ppf[2]);

	for(int i = 0; i < 50; i++)
	{
		desc[i] = ppf[i + 6];
#if defined DEBUG
		printf("Desc Byte %02X\n", desc[i]);
#endif
	}
	desc[50]=0; // terminate


	//fread(&desc, 1, 50, ppf); desc[50]=0;
	//printf("Patchfile is a PPF1.0 patch. Patch Information:\n");
	printf("PPF1.0 patch information:\n");
	printf("Description : %s\n", desc);
	printf("File_id.diz : no\n");

	printf("Patching... "); fflush(stdout);
	//fseeko64(ppf, 0, SEEK_END);
	
	//count=ftell(ppf);
	count = ppf_len;
	//printf("PPF count: %X", count);
	
	count-=56;
	seekpos=56;

	do{
		printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		//fseeko64(ppf, seekpos, SEEK_SET);

		pos = ppf[seekpos] + (ppf[seekpos + 1] << 8) + (ppf[seekpos + 2] << 16) + (ppf[seekpos + 3] << 24);
		//printf("PPF POS Byte 0: %02X\n", ppf[seekpos]);
		//printf("PPF POS Byte 1: %02X\n", ppf[seekpos + 1]);
		//printf("PPF POS Byte 2: %02X\n", ppf[seekpos + 2]);
		//printf("PPF POS Byte 3: %02X\n", ppf[seekpos + 3]);
#if defined DEBUG
		printf("POS: %08X\n", pos);
#endif
		//fread(&pos, 1, 4, ppf);
        Endian32_Swap (pos);			// <Hu Kares> little to big endian
		
		anz = ppf[seekpos + 4];
		//fread(&anz, 1, 1, ppf);
#if defined DEBUG
		printf("ANZ: %02X\n", anz);
#endif
		for(int i = 0; i < anz; i++)
		{
			ppfmem[i] = ppf[i + seekpos + 4 + 1];
#if defined DEBUG
			printf("PPFMEM: %02X\n", ppfmem[i]);
#endif
		}
		//fread(&ppfmem, 1, anz, ppf);

		//fseeko64(bin, pos, SEEK_SET);
		fseeko(bin, pos, SEEK_SET); // -D_FILE_OFFSET_BITS=64 allows us to use fseeko
		//printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		fwrite(&ppfmem, 1, anz, bin);
		seekpos=seekpos+5+anz;
		count=count-5-anz;
#if defined DEBUG
		printf("Count %08X\n", count);
#endif

	} while(count!=0);

	printf("successful.\n");
}

//////////////////////////////////////////////////////////////////////
// Applies a PPF2.0 patch.
void ApplyPPF2Patch(const unsigned char ppf[], unsigned int ppf_len, FILE *bin){
		//char desc[50], in;
		char desc[51], in; // old bug fixed
		unsigned int binlen, obinlen, count, seekpos;
		int idlen, pos;
		unsigned char anz;

		//fseeko64(ppf, 6,SEEK_SET);
		//fread(&desc, 1, 50, ppf); desc[50]=0;
		for(int i = 0; i < 50; i++)
		{
			desc[i] = ppf[i + 6];
#if defined DEBUG
			printf("Desc Byte %02X\n", desc[i]);
#endif
		}
		desc[50]=0; // terminate

		//printf("Patchfile is a PPF2.0 patch. Patch Information:\n");
		printf("PPF2.0 patch information:\n");
		printf("Description : %s\n",desc);
		printf("File_id.diz : ");
		idlen=ShowFileId(ppf, ppf_len, 2);
		if(!idlen) printf("not available\n");
			//fseeko64(ppf, 56, SEEK_SET);
			//fread(&obinlen, 1, 4, ppf);
 			obinlen = ppf[56] + (ppf[56 + 1] << 8) + (ppf[56 + 2] << 16) + (ppf[56 + 3] << 24); // NOT BIG-ENDIAN SAFE    
			Endian32_Swap (obinlen);		// <Hu Kares> little to big endian
        	// fseeko64(bin, 0, SEEK_END);
			fseeko(bin, 0, SEEK_END); // -D_FILE_OFFSET_BITS=64 allows us to use fseeko
        	binlen=ftell(bin);
        	if(obinlen!=binlen){
			printf("The size of the bin file isn't correct, continue ? (y/n): "); fflush(stdout);
			in=getc(stdin);
		
			if(in!='y'&&in!='Y'){
				printf("Aborted...\n");
				return;
			}
		}

		fflush(stdin);
		//fseeko64(ppf, 60, SEEK_SET);
		//fread(&ppfblock, 1, 1024, ppf);
		for(int i = 0; i < 1024; i++)
		{
			ppfblock[i] = ppf[60 + i];
		}
		
		//fseeko64(bin, 0x9320, SEEK_SET); 
		fseeko(bin, 0x9320, SEEK_SET); // -D_FILE_OFFSET_BITS=64 allows us to use fseeko

		//fread(&binblock, 1, 1024, bin);
		if(fread(&binblock, 1, 1024, bin) != 1024)
		{
			printf("Error: Can not read bin block\n");
			return;
		}
		
		in=memcmp(ppfblock, binblock, 1024);
		if(in!=0){
			printf("Binblock/Patchvalidation failed. continue ? (y/n): "); fflush(stdout);

#if defined(__APPLE__) || defined (MACOSX)

                        if(obinlen!=binlen) {		// <Hu Kares> required, since fflush doesn't flush '\n'!
                            in=getc(stdin);
                        }

#endif /* __APPLE__ || MACOSX */
			in=getc(stdin);
			if(in!='y'&&in!='Y'){
				printf("Aborted...\n");
				return;
			}
		}

		printf("Patching... "); fflush(stdout);
		//fseeko64(ppf, 0, SEEK_END);
		//count=ftell(ppf);
		count = ppf_len;
		seekpos=1084;
		count-=1084;
		if(idlen) count-=idlen+38;

        do{
			printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
			//fseeko64(ppf, seekpos, SEEK_SET);
			
			//fread(&pos, 1, 4, ppf);
			//pos = ppf[seekpos] + (ppf[seekpos + 1] << 8) + (ppf[seekpos + 2] << 16) + (ppf[seekpos + 3] << 24); // NOT BIG-ENDIAN SAFE    

			memcpy(&pos, &ppf[seekpos], 4);
            Endian32_Swap (pos);		// <Hu Kares> little to big endian
#if defined DEBUG
			printf("POS: %08X\n", pos);
#endif
			//fread(&anz, 1, 1, ppf);
			anz = ppf[seekpos + 4];
#if defined DEBUG
			printf("ANZ: %02X\n", anz);
#endif
			//fread(&ppfmem, 1, anz, ppf);
			for(int i = 0; i < anz; i++)
			{
				ppfmem[i] = ppf[i + seekpos + 4 + 1];
#if defined DEBUG				
				printf("PPFMEM: %02X\n", ppfmem[i]);
#endif			
			}
			// fseeko64(bin, pos, SEEK_SET);
			fseeko(bin, pos, SEEK_SET); // -D_FILE_OFFSET_BITS=64 allows us to use fseeko

			printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
			fwrite(&ppfmem, 1, anz, bin);
			seekpos=seekpos+5+anz;
			count=count-5-anz;
#if defined DEBUG
			printf("Count %08X\n", count);
#endif
        } while(count!=0);

		printf("successful.\n");
}
//////////////////////////////////////////////////////////////////////
// Applies a PPF3.0 patch.
//void ApplyPPF3Patch(FILE *ppf, FILE *bin, char mode){
void ApplyPPF3Patch(const unsigned char ppf[], unsigned int ppf_len, FILE *bin, char mode){
	//char desc[50], imagetype=0, in;
    char desc[51], imagetype=0, in; // old bug fixed
	unsigned char	undo=0, blockcheck=0;
	int idlen;
	//__off64_t offset, count;			// <Hu Kares> count has to be 64 bit!
	off_t offset, count;
	unsigned int seekpos;
	unsigned char anz=0;

	//fseeko64(ppf, 6,SEEK_SET);  /* Read Desc.line */
	//fread(&desc, 1, 50, ppf); desc[50]=0;
	for(int i = 0; i < 50; i++)
	{
		desc[i] = ppf[i + 6];
#if defined DEBUG		
		printf("Desc Byte %02X\n", desc[i]);
#endif
	}
	desc[50]=0; // terminate

	//printf("Patchfile is a PPF3.0 patch. Patch Information:\n");
	printf("PPF3.0 patch information:\n");
	printf("Description : %s\n",desc);
	printf("File_id.diz : ");

	idlen=ShowFileId(ppf, ppf_len, 3);
	if(!idlen) printf("not available\n");

	//fseeko64(ppf, 56, SEEK_SET);
	//fread(&imagetype, 1, 1, ppf);
	imagetype = ppf[56];

	//fseeko64(ppf, 57, SEEK_SET);
	//fread(&blockcheck, 1, 1, ppf);
	blockcheck = ppf[57];
#if defined DEBUG	
	printf("Block Check: %02X\n", blockcheck);
#endif
	//fseeko64(ppf, 58, SEEK_SET);
	//fread(&undo, 1, 1, ppf);
	undo = ppf[58];
#if defined DEBUG	
	printf("Undo Support: %02x\n", undo);
#endif
	if(mode==UNDO){
		if(!undo){
			printf("Error: no undo data available\n");
			return;
		}
	}

	if(blockcheck){
		fflush(stdin);
		//fseeko64(ppf, 60, SEEK_SET);
		//fread(&ppfblock, 1, 1024, ppf);
		for(int i = 0; i < 1024; i++)
		{
			ppfblock[i] = ppf[60 + i];
		}
		
		if(imagetype){
			//fseeko64(bin, 0x80A0, SEEK_SET);
			fseeko(bin, 0x80A0, SEEK_SET);  // -D_FILE_OFFSET_BITS=64 allows us to use fseeko
		} else {
			//fseeko64(bin, 0x9320, SEEK_SET);
			fseeko(bin, 0x9320, SEEK_SET); //  -D_FILE_OFFSET_BITS=64 allows us to use fseeko
		}
		//fread(&binblock, 1, 1024, bin);
		
		if(fread(&binblock, 1, 1024, bin) != 1024)
		{
			printf("Error: Can not read bin block\n");
			return;
		}
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

	//fseeko64(ppf, 0, SEEK_END);
	//count=ftello64(ppf);				// <Hu Kares> 64 bit!
	count = ppf_len;
	//fseeko64(ppf, 0, SEEK_SET);
	
	if(blockcheck){
		seekpos=1084;
		count-=1084;
	} else {
		seekpos=60;
		count-=60;
	}

	if(idlen) count-=(idlen+18+16+2);
	

	printf("Patching ... ");
	//fseeko64(ppf, seekpos, SEEK_SET);
	do{
		printf("reading...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);
		//fread(&offset, 1, 8, ppf);
		memcpy(&offset, &ppf[seekpos], 8);
        Endian64_Swap(offset);			// <Hu Kares> little to big endian
#if defined DEBUG		
		printf("Offset: %jX\n", offset);
#endif
		//fread(&anz, 1, 1, ppf);
		anz = ppf[seekpos + 8];
#if defined DEBUG
		printf("ANZ: %02X\n", anz);
#endif

		if(mode==APPLY){
		//	fread(&ppfmem, 1, anz, ppf);

			for(int i = 0; i < anz; i++)
			{
				ppfmem[i] = ppf[i + seekpos + 8 + 1];
#if defined DEBUG
				printf("PPFMEM: %02X\n", ppfmem[i]);
#endif			
			}
			//if(undo) fseeko64(ppf, anz, SEEK_CUR);
		} else {
            if(mode==UNDO){
                //fseeko64(ppf, anz, SEEK_CUR);
                //fread(&ppfmem, 1, anz, ppf);
				for(int i = 0; i < anz; i++)
				{
					ppfmem[i] = ppf[i + seekpos + 8 + 1 + anz];
#if defined DEBUG					
					printf("PPFMEM: %02X\n", ppfmem[i]);
#endif				
				}
            }
        }
    
		printf("writing...\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);		
		//fseeko64(bin, offset, SEEK_SET);
		fseeko(bin, offset, SEEK_SET);

		fwrite(&ppfmem, 1, anz, bin);
		count-=(anz+9);

		if(undo)
		{
			seekpos=seekpos+5+4+anz+anz;
		} else {
			seekpos=seekpos+5+4+anz;
		}

		if(undo) count-=anz;

#if defined DEBUG
		printf("Count %jX\n", count);
#endif
	} while(count!=0);

		printf("successful.\n");
}

//////////////////////////////////////////////////////////////////////
// Shows File_Id.diz of a PPF2.0 / PPF3.0 patch.
// Input: 2 = PPF2.0
// Input: 3 = PPF3.0
// Return 0 = Error/no fileid.
// Return>0 = Length of fileid.
//int ShowFileId(FILE *ppf, int ppfver);
int ShowFileId(const unsigned char ppf[], unsigned int ppf_len, int ppfver){
	char buffer2[3073];
	unsigned int idmagic;
	int lenidx=0, idlen=0, orglen=0;

	if(ppfver==2){
		lenidx=4;
	} else {
		lenidx=2;
	}

	//fseeko64(ppf,-(lenidx+4),SEEK_END);
	//fread(&idmagic, 1, 4, ppf);
	memcpy(&idmagic, &ppf[(ppf_len-(lenidx+4))], 4);
        Endian32_Swap (idmagic);			// <Hu Kares> little to big endian

	//printf("IDMagic: %08X\n", idmagic);

	//if(idmagic!='ZID.'){
	if(idmagic != 0x5A49442E)
	{
		return(0);
	} else {
		//fseeko64(ppf,-lenidx,SEEK_END);
		//fread(&idlen, 1, lenidx, ppf);
		memcpy(&idlen, &ppf[(ppf_len -lenidx)], lenidx);
        Endian32_Swap (idlen);			// <Hu Kares> little to big endian
		//printf("IDLen: %02X\n", idlen);
        orglen = idlen;
        if (idlen > 3072) {			// <Hu Kares> to be secure: avoid segmentation fault!
            idlen = 3072;
        }
		//fseeko64(ppf,-(lenidx+16+idlen),SEEK_END);
		//fread(&buffer2, 1, idlen, ppf);
		memcpy(&buffer2, &ppf[(ppf_len-(lenidx+16+idlen))], idlen);
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
//int PPFVersion(FILE *ppf){
int PPFVersion(const unsigned char ppf[])
{
	unsigned int magic;
	//fseeko64(ppf,0,SEEK_SET);
	//fread(&magic, 1, 4, ppf);
	
	magic = ppf[0] + (ppf[1] << 8) + (ppf[2] << 16) + (ppf[3] << 24); 

        Endian32_Swap (magic);				// <Hu Kares> little to big endian
#if defined DEBUG
		printf("PPF Version Magic: %08X\n", magic);
#endif

	switch(magic){
			//case '1FPP'		:	return(1);
			case 0x31465050		:	return(1);
			// case '2FPP'		:	return(2);
			case 0x32465050		:	return(2);
			// case '3FPP'		:	return(3);
			case 0x33465050		:	return(3);
			default			:   printf("Error: patchfile is no ppf patch\n"); break;
	}

	return(0);
}

void apply_ppf(const unsigned char ppf[], unsigned int ppf_len, FILE *bin)
{
	int x;
	x=PPFVersion(ppf);
	if(x==1)
	{ 
		ApplyPPF1Patch(ppf, ppf_len, bin);
	}else if(x==2) { 
		ApplyPPF2Patch(ppf, ppf_len, bin);
	} else if(x==3) { 
		ApplyPPF3Patch(ppf, ppf_len, bin, APPLY);
	} else{ 
		printf("Error: Unknown PPF Patch version %d\n", x);
	}
}

//////////////////////////////////////////////////////////////////////
// Open all needed files.
// Return: 0 - Successful
// Return: 1 - Failed.
/*
int OpenFiles(char* file1, char* file2){

	bin=fopen(file1, "rb+");
	if(!bin){
		printf("Error: cannot open file '%s' ",file1);
		return(1);
	}

	ppf=fopen(file2,  "rb");
	if(!ppf){
		printf("Error: cannot open file '%s' ",file2);
		fclose(bin);
		return(1);
	}

	return(0);
}
*/
