#include "applyppf3_linux.c"
#include <stdio.h>
#include <stdbool.h>
#include "patches.h"
//#include "test-ppf3-without-undo.h"
//#include "test-ppf3-with-undo.h"

unsigned char sectors[0x1800]; // 0x800 * 3

unsigned char ver_check_val; // for games that have the same bootfile we use the first different byte to detect different revisions/versions

bool match_SCES, match_SLES;

const unsigned char SCES[] = { 
    0x53, 0x43, 0x45, 0x53, 0x5F // SCES_
};

const unsigned char SLES[] = { 
    0x53, 0x4C, 0x45, 0x53, 0x5F // SLES_
};

#if defined WIN32 // system("pause"); sucks we can do better
	#include <conio.h>
	
	void pause()
	{
		printf("\nPress any key to continue...\n");
		_getch();
	}
#endif

void do_exit(unsigned char exit_code)
{
	#if defined WIN32
		pause();
	#endif
	exit(exit_code);
}

int main (int argc, const char * argv[]) 
{
	unsigned int bin_size;
    printf("LibCrypt Patcher %s By Alex Free (C)2023-2024 (3-BSD)\nhttps://alex-free.github.io/libcrypt-patcher\n\n", VERSION);

    if(argc != 2) {
        printf("incorrect number of arguments\n\nUsage:\n\nlcp <track 1 bin file of libcrypt protected game>\n");
		do_exit(1);
    }

    bin = fopen(argv[1], "rb+");

    if(bin == NULL)
    {
        printf("Error: Can not open: %s\n", argv[1]);
		do_exit(1);
    }

    fseek(bin, 0, SEEK_END);
    bin_size = ftell(bin);

    if(bin_size < 0x930 * 25) // Minimum expected for correctness of detection functions below - 22-25
    {
	    printf("Error: %s is too small to be a patchable track 01 bin file\n", argv[1]);
        fclose(bin);        
		do_exit(1);
    }
	unsigned char psx_string[] = {
		0x50, 0x4C, 0x41, 0x59, 0x53, 0x54, 0x41, 0x54, 0x49, 0x4F, 0x4E, 0x20 // PLAYSTATION<space>
	};

	bool is_psx_string;
	is_psx_string = true;

	unsigned char check;

	for(int i =  0; i < 12; i++)
	{
		fseek(bin, (0x9320 + i), SEEK_SET);
		check = fgetc(bin);

		if(check != psx_string[i])
		{
			is_psx_string = false;
		}
	}

	if(!is_psx_string)
	{
		printf("Error: %s does not appear to be a PlayStation data track bin file.\n", argv[1]);
		fclose(bin);
		do_exit(1);
	}

    fseek(bin, (0xCA20 + 0x18), SEEK_SET);// 0x930 * 22 = Directory Record Sector + 0x18 to skip header
            
    for(int directory_record_sector = 0; directory_record_sector < 3; directory_record_sector++){
        //printf("\n\nDirectory Record Sector %d\n\n", directory_record_sector);        
        for(int i=0; i < 0x800; i++)
        {
            sectors[i + (directory_record_sector * 0x800)] = fgetc(bin);
            //printf("%02X ", sectors[i + (directory_record_sector * 0x800)]);
            //printf("Directory Sector Multi: %d\n", (directory_record_sector * 0x800)) ;
        }
        fseek(bin, (0x118 + 0x18), SEEK_CUR); // skip EDC/EEC/Header
    }

    fseek(bin, 0, SEEK_SET);

   	char bootfile[12];

    for(int s=0; s < 0x1800; s++){

        match_SCES = true;
        for(int i=0; i < 5; i++)
        {                
            if((SCES[i] != sectors[s + i]))
            {
                match_SCES = false;
            }
        }
        
        match_SLES = true;
        for(int i=0; i < 5; i++)
        {                
            if((SLES[i] != sectors[s + i]))
            {
                match_SLES = false;
            }
        }

        if((match_SCES) || (match_SLES))
        {
            for(int i = 0; i < 11; i++)
            {
                bootfile[i] = sectors[s + i];
            }
            printf("BOOTFILE: %s\n", bootfile);
            break;
        }
    }

    // we don't have any LibCrypt PPF3.0 patches, so I use the english translation of King's Field as a test case.
    //ApplyPPF3Patch(test_ppf30_without_undo, test_ppf30_without_undo_len, bin, APPLY); // MD5SUM: c56cfb36f19f2608d3c180c104ace9dd
    //ApplyPPF3Patch(test_ppf30_without_undo, test_ppf30_without_undo_len, bin, UNDO); // code catches that this does not support undo

    // While the King's Field patch was PPF3.0, it did not have undo support.  So I just re-did the patch with undo support to test both apply and undo type patches.
    //ApplyPPF3Patch(test_ppf30_with_undo, test_ppf30_with_undo_len, bin, APPLY); // MD5SUM: c56cfb36f19f2608d3c180c104ace9dd
    //ApplyPPF3Patch(test_ppf30_with_undo, test_ppf30_with_undo_len, bin, UNDO); // MD5SUM: c2b8b1652407c6c8107b0c93e20624a6

	// MediEvil (Europe)
	if((strcmp(bootfile, "SCES_003.11") == 0))
	{
		apply_ppf(SCES_003_11_PPF, SCES_003_11_PPF_len, bin);
	}

	// Disney's Tarzan (Europe)
	else if((strcmp(bootfile, "SCES_014.31") == 0))
	{
		apply_ppf(SCES_014_31_PPF, SCES_014_31_PPF_len, bin);
	}
	
	// Jackie Chan Stuntmaster (Europe) (Recalled) /  Jackie Chan Stuntmaster (Europe)
	else if((strcmp(bootfile, "SCES_014.44") == 0))
	{
		apply_ppf(SCES_014_44_PPF, SCES_014_44_PPF_len, bin);
	}
	
	// MediEvil (France)
	else if((strcmp(bootfile, "SCES_014.92") == 0))
	{
		apply_ppf(SCES_014_92_PPF, SCES_014_92_PPF_len, bin);
	}
	
	// MediEvil (Germany)
	else if((strcmp(bootfile, "SCES_014.93") == 0))
	{
		apply_ppf(SCES_014_93_PPF, SCES_014_93_PPF_len, bin);
	}
	
	// MediEvil (Italy)
	else if((strcmp(bootfile, "SCES_014.94") == 0))
	{
		apply_ppf(SCES_014_94_PPF, SCES_014_94_PPF_len, bin);
	}
	
	// MediEvil (Spain)
	else if((strcmp(bootfile, "SCES_014.95") == 0))
	{
		apply_ppf(SCES_014_95_PPF, SCES_014_95_PPF_len, bin);
	}
	
	// Disney Tarzan (France)
	else if((strcmp(bootfile, "SCES_015.16") == 0))
	{
		apply_ppf(SCES_015_16_PPF, SCES_015_16_PPF_len, bin);
	}
	
	// Disneys Tarzan (Germany)
	else if((strcmp(bootfile, "SCES_015.17") == 0))
	{
		apply_ppf(SCES_015_17_PPF, SCES_015_17_PPF_len, bin);
	}
	
	// Disney's Tarzan (Italy)
	else if((strcmp(bootfile, "SCES_015.18") == 0))
	{
		apply_ppf(SCES_015_18_PPF, SCES_015_18_PPF_len, bin);
	}
	
	// Disney's Tarzan (Spain)
	else if((strcmp(bootfile, "SCES_015.19") == 0))
	{
		apply_ppf(SCES_015_19_PPF, SCES_015_19_PPF_len, bin);
	}
	
	// Ape Escape (Europe)
	else if((strcmp(bootfile, "SCES_015.64") == 0))
	{
		apply_ppf(SCES_015_64_PPF, SCES_015_64_PPF_len, bin);
	}
	
	// Disney's Mulan: Story Studio (Europe)
	else if((strcmp(bootfile, "SCES_016.95") == 0))
	{
		apply_ppf(SCES_016_95_PPF, SCES_016_95_PPF_len, bin);
	}
	
	// This Is Football (Europe)
	else if((strcmp(bootfile, "SCES_017.00") == 0))
	{
		apply_ppf(SCES_017_00_PPF, SCES_017_00_PPF_len, bin);
	}
	
	// Le Monde des Bleus: Le Jeu Officiel de l'Équipe de France (France)
	else if((strcmp(bootfile, "SCES_017.01") == 0))
	{
		apply_ppf(SCES_017_01_PPF, SCES_017_01_PPF_len, bin);
	}
	
	// Fussball Live (Germany)
	else if((strcmp(bootfile, "SCES_017.02") == 0))
	{
		apply_ppf(SCES_017_02_PPF, SCES_017_02_PPF_len, bin);
	}
	
	// This Is Football (Italy)
	else if((strcmp(bootfile, "SCES_017.03") == 0))
	{
		apply_ppf(SCES_017_03_PPF, SCES_017_03_PPF_len, bin);
	}
	
	// Esto Es Futbol Spain
	else if((strcmp(bootfile, "SCES_017.04") == 0))
	{
		apply_ppf(SCES_017_04_PPF, SCES_017_04_PPF_len, bin);
	}
	
	// Speed Freaks (Europe)
	else if((strcmp(bootfile, "SCES_017.63") == 0))
	{
		apply_ppf(SCES_017_63_PPF, SCES_017_63_PPF_len, bin);
	}
	
	// This is Football (Europe) (Fr,NI)
	else if((strcmp(bootfile, "SCES_018.82") == 0))
	{
		apply_ppf(SCES_018_82_PPF, SCES_018_82_PPF_len, bin);
	}
	
	// Wip3out (Europe)
	else if((strcmp(bootfile, "SCES_019.09") == 0))
	{
		apply_ppf(SCES_019_09_PPF, SCES_019_09_PPF_len, bin);
	}
	
	// Formula One 99 (Europe) (En,Fr,De,It)
	else if((strcmp(bootfile, "SCES_019.79") == 0))
	{
		apply_ppf(SCES_019_79_PPF, SCES_019_79_PPF_len, bin);
	}
	
	// Disney Fais Ton Histoire! Mulan (France)
	else if((strcmp(bootfile, "SCES_020.04") == 0))
	{
		apply_ppf(SCES_020_04_PPF, SCES_020_04_PPF_len, bin);
	}
	
	// Disneys Interaktive Abenteuer: Mulan (Disneys Interaktive Abenteuer: Mulan)(Germany)
	else if((strcmp(bootfile, "SCES_020.05") == 0))
	{
		apply_ppf(SCES_020_05_PPF, SCES_020_05_PPF_len, bin);
	}
	
	// Disney Libro Animato Creativo: Mulan (Italy)
	else if((strcmp(bootfile, "SCES_020.06") == 0))
	{
		apply_ppf(SCES_020_06_PPF, SCES_020_06_PPF_len, bin);
	}
	
	// Disney's Aventura Interactiva: Mulan (Spain)
	else if((strcmp(bootfile, "SCES_020.07") == 0))
	{
		apply_ppf(SCES_020_07_PPF, SCES_020_07_PPF_len, bin);
	}
	
	// Ape Escape (France)
	else if((strcmp(bootfile, "SCES_020.28") == 0))
	{
		apply_ppf(SCES_020_28_PPF, SCES_020_28_PPF_len, bin);
	}
	
	// Ape Escape (Germany)
	else if((strcmp(bootfile, "SCES_020.29") == 0))
	{
		apply_ppf(SCES_020_29_PPF, SCES_020_29_PPF_len, bin);
	}
	
	// Ape Escape (Italy)
	else if((strcmp(bootfile, "SCES_020.30") == 0))
	{
		apply_ppf(SCES_020_30_PPF, SCES_020_30_PPF_len, bin);
	}
	
	// Ape Escape: La Invasión de los Monos (Spain)
	else if((strcmp(bootfile, "SCES_020.31") == 0))
	{
		apply_ppf(SCES_020_31_PPF, SCES_020_31_PPF_len, bin);
	}
	
	// Spyro 2: Gateway to Glimmer (Europe)
	else if((strcmp(bootfile, "SCES_021.04") == 0))
	{
		apply_ppf(SCES_021_04_PPF, SCES_021_04_PPF_len, bin);
	}
	
	// CTR: Crash Team Racing (Europe) / CTR: Crash Team Racing (EDC) (Europe)
	else if((strcmp(bootfile, "SCES_021.05") == 0))
	{
		apply_ppf(SCES_021_05_PPF, SCES_021_05_PPF_len, bin);
	}
	
	// Disneys Tarzan (Denmark)
	else if((strcmp(bootfile, "SCES_021.81") == 0))
	{
		apply_ppf(SCES_021_81_PPF, SCES_021_81_PPF_len, bin);
	}
	
	// Disneys Tarzan (Sweden)
	else if((strcmp(bootfile, "SCES_021.82") == 0))
	{
		apply_ppf(SCES_021_82_PPF, SCES_021_82_PPF_len, bin);
	}
	
	// Disneyn Tarzan (Finland)
	else if((strcmp(bootfile, "SCES_021.84") == 0))
	{
		apply_ppf(SCES_021_84_PPF, SCES_021_84_PPF_len, bin);
	}
	
	// Disney's Tarzan (Netherlands)
	else if((strcmp(bootfile, "SCES_021.85") == 0))
	{
		apply_ppf(SCES_021_85_PPF, SCES_021_85_PPF_len, bin);
	}
	
	// Formula One 99 (Europe) (En, Es, Fi)
	else if((strcmp(bootfile, "SCES_022.22") == 0))
	{
		apply_ppf(SCES_022_22_PPF, SCES_022_22_PPF_len, bin);
	}
	
	// Disney's Verhalenstudio: Mulan (Netherlands)
	else if((strcmp(bootfile, "SCES_022.64") == 0))
	{
		apply_ppf(SCES_022_64_PPF, SCES_022_64_PPF_len, bin);
	}
	
	// This Is Soccer (Australia)
	else if((strcmp(bootfile, "SCES_022.69") == 0))
	{
		apply_ppf(SCES_022_69_PPF, SCES_022_69_PPF_len, bin);
	}
	
	// Space Debris (Europe)
	else if((strcmp(bootfile, "SCES_022.90") == 0))
	{
		apply_ppf(SCES_022_90_PPF, SCES_022_90_PPF_len, bin);
	}
	
	// Barbie: Race & Ride (Europe)
	else if((strcmp(bootfile, "SCES_023.65") == 0))
	{
		apply_ppf(SCES_023_65_PPF, SCES_023_65_PPF_len, bin);
	}
	
	// Barbie: Aventure Equestre (France)
	else if((strcmp(bootfile, "SCES_023.66") == 0))
	{
		apply_ppf(SCES_023_66_PPF, SCES_023_66_PPF_len, bin);
	}
	
	// Barbie: Race & Ride (Germany)
	else if((strcmp(bootfile, "SCES_023.67") == 0))
	{
		apply_ppf(SCES_023_67_PPF, SCES_023_67_PPF_len, bin);
	}
	
	// Barbie: Race & Ride (Italy)
	else if((strcmp(bootfile, "SCES_023.68") == 0))
	{
		apply_ppf(SCES_023_68_PPF, SCES_023_68_PPF_len, bin);
	}
	
	// Barbie: Race & Ride (Spain)
	else if((strcmp(bootfile, "SCES_023.69") == 0))
	{
		apply_ppf(SCES_023_69_PPF, SCES_023_69_PPF_len, bin);
	}
	
	// Space Debris (France)
	else if((strcmp(bootfile, "SCES_024.30") == 0))
	{
		apply_ppf(SCES_024_30_PPF, SCES_024_30_PPF_len, bin);
	}
	
	// Space Debris (Germany)
	// TODO: Find Preproduction version
	else if((strcmp(bootfile, "SCES_024.31") == 0))
	{
		apply_ppf(SCES_024_31_PPF, SCES_024_31_PPF_len, bin);
	}
	
	// Space Debris (Italy)
	else if((strcmp(bootfile, "SCES_024.32") == 0))
	{
		apply_ppf(SCES_024_32_PPF, SCES_024_32_PPF_len, bin);
	}
	
	// Space Debris (Spain)
	else if((strcmp(bootfile, "SCES_024.33") == 0))
	{
		apply_ppf(SCES_024_33_PPF, SCES_024_33_PPF_len, bin);
	}
	
	// Barbie: Super Sports (Europe)
	else if((strcmp(bootfile, "SCES_024.87") == 0))
	{
		apply_ppf(SCES_024_87_PPF, SCES_024_87_PPF_len, bin);
	}
	
	// Barbie: Sports Extrême (France)
	else if((strcmp(bootfile, "SCES_024.88") == 0))
	{
		apply_ppf(SCES_024_88_PPF, SCES_024_88_PPF_len, bin);
	}
	
	// Barbie: Super Sport (Germany)
	else if((strcmp(bootfile, "SCES_024.89") == 0))
	{
		apply_ppf(SCES_024_89_PPF, SCES_024_89_PPF_len, bin);
	}
	
	// Barbie: Super Sports (Italy)
	else if((strcmp(bootfile, "SCES_024.90") == 0))
	{
		apply_ppf(SCES_024_90_PPF, SCES_024_90_PPF_len, bin);
	}
	
	// Barbie: Super Sports (Spain)
	else if((strcmp(bootfile, "SCES_024.91") == 0))
	{
		apply_ppf(SCES_024_91_PPF, SCES_024_91_PPF_len, bin);
	}
	
	// MediEvil 2 (En,Fr,De) (Europe)
	else if((strcmp(bootfile, "SCES_025.44") == 0))
	{
		apply_ppf(SCES_025_44_PPF, SCES_025_44_PPF_len, bin);
	}
	
	// MediEvil 2 (Es, It, Pt) (Europe)
	else if((strcmp(bootfile, "SCES_025.45") == 0))
	{
		apply_ppf(SCES_025_45_PPF, SCES_025_45_PPF_len, bin);
	}
	
	// MediEvil 2 (Russia)
	else if((strcmp(bootfile, "SCES_025.46") == 0))
	{
		apply_ppf(SCES_025_46_PPF, SCES_025_46_PPF_len, bin);
	}
	
	// Crash Bash (En,Fr,De,Es,It,Nl) (Europe)
	else if((strcmp(bootfile, "SCES_028.34") == 0))
	{
		apply_ppf(SCES_028_34_PPF, SCES_028_34_PPF_len, bin);
	}

	// Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.0) / Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.1)
	else if((strcmp(bootfile, "SCES_028.35") == 0))
	{
		// 000093C7 09 0A
		fseek(bin, 0x93C7, SEEK_SET);
		ver_check_val = fgetc(bin);
		fseek(bin, 0, SEEK_SET);
		if(ver_check_val == 0x09) // Rev 0
		{
			printf("Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.0)\n");
			apply_ppf(SCES_028_35_REV_0_PPF, SCES_028_35_REV_0_PPF_len, bin);
		} else if(ver_check_val == 0x0A) { // Rev 1
			printf("Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.1)\n");
			apply_ppf(SCES_028_35_REV_1_PPF, SCES_028_35_REV_1_PPF_len, bin);
		} else {
			printf("Unknown version\n");
		}
	}
	
	// Prince Naseem Boxing (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_000.17") == 0))
	{
		apply_ppf(SLES_000_17_PPF, SLES_000_17_PPF_len, bin);
	}
	
	// Ronaldo V-Football (En,Fr,Nl,Sv) (Europe)
	else if((strcmp(bootfile, "SLES_009.95") == 0))
	{
		apply_ppf(SLES_009_95_PPF, SLES_009_95_PPF_len, bin);
	}
	
	// Hogs of War (Europe)
	else if((strcmp(bootfile, "SLES_010.41") == 0))
	{
		apply_ppf(SLES_010_41_PPF, SLES_010_41_PPF_len, bin);
	}
	
	// Actua Ice Hockey 2 (Europe)
	else if((strcmp(bootfile, "SLES_012.26") == 0))
	{
		apply_ppf(SLES_012_26_PPF, SLES_012_26_PPF_len, bin);
	}
	
	// Gekido: Urban Fighters (Europe)
	else if((strcmp(bootfile, "SLES_012.41") == 0))
	{
		apply_ppf(SLES_012_41_PPF, SLES_012_41_PPF_len, bin);
	}
	
	// Legacy of Kain: Soul Reaver (Europe)
	else if((strcmp(bootfile, "SLES_013.01") == 0))
	{
		apply_ppf(SLES_013_01_PPF, SLES_013_01_PPF_len, bin);
	}
	
	// Le Mans 24 Hours (Europe) (En,Fr,De,Es,It,Pt)
	else if((strcmp(bootfile, "SLES_013.62") == 0))
	{
		apply_ppf(SLES_013_62_PPF, SLES_013_62_PPF_len, bin);
	}
	
	// Michelin Rally Masters: Race of Champions (Europe) (En,De,Sv)
	else if((strcmp(bootfile, "SLES_015.45") == 0))
	{
		apply_ppf(SLES_015_45_PPF, SLES_015_45_PPF_len, bin);
	}
	
	// Eagle One: Harrier Attack (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_017.15") == 0))
	{
		apply_ppf(SLES_017_15_PPF, SLES_017_15_PPF_len, bin);
	}
	
	// UEFA Striker (Europe) (En,Fr,De,Es,It,Nl)
	else if((strcmp(bootfile, "SLES_017.33") == 0))
	{
		apply_ppf(SLES_017_33_PPF, SLES_017_33_PPF_len, bin);
	}
	
	//  Mission: Impossible (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_019.06") == 0))
	{
		apply_ppf(SLES_019_06_PPF, SLES_019_06_PPF_len, bin);
	}
	
	// V-Rally: Championship Edition 2 (Europe) (En,Fr,De)
	else if((strcmp(bootfile, "SLES_019.07") == 0))
	{
		apply_ppf(SLES_019_07_PPF, SLES_019_07_PPF_len, bin);
	}
	
	// Radikal Bikers (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_019.53") == 0))
	{
		apply_ppf(SLES_019_43_PPF, SLES_019_43_PPF_len, bin);
	}
	
	// Legacy of Kain: Soul Reaver (France)
	else if((strcmp(bootfile, "SLES_020.24") == 0))
	{
		apply_ppf(SLES_020_24_PPF, SLES_020_24_PPF_len, bin);
	}
	
	// Legacy of Kain: Soul Reaver (Germany)
	else if((strcmp(bootfile, "SLES_020.25") == 0))
	{
		apply_ppf(SLES_020_25_PPF, SLES_020_25_PPF_len, bin);
	}
	
	// Legacy of Kain: Soul Reaver (Spain)
	else if((strcmp(bootfile, "SLES_020.26") == 0))
	{
		apply_ppf(SLES_020_26_PPF, SLES_020_26_PPF_len, bin);
	}
	
	// Legacy of Kain: Soul Reaver (Italy)
	else if((strcmp(bootfile, "SLES_020.27") == 0))
	{
		apply_ppf(SLES_020_27_PPF, SLES_020_27_PPF_len, bin);
	}
	
	// PGA European Tour Golf (En,De) (Europe)
	else if((strcmp(bootfile, "SLES_020.61") == 0))
	{
		apply_ppf(SLES_020_61_PPF, SLES_020_61_PPF_len, bin);
	}
	
	// Urban Chaos (Europe) (En,Es,It)
	else if((strcmp(bootfile, "SLES_020.71") == 0))
	{
		apply_ppf(SLES_020_71_PPF, SLES_020_71_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 1) (Europe)
	else if((strcmp(bootfile, "SLES_020.80") == 0))
	{
		apply_ppf(SLES_020_80_PPF, SLES_020_80_PPF_len, bin);
	}
	
	// N-Gen Racing (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_020.86") == 0))
	{
		apply_ppf(SLES_020_86_PPF, SLES_020_86_PPF_len, bin);
	}
	
	// SaGa Frontier 2 (Europe)
	else if((strcmp(bootfile, "SLES_021.12") == 0))
	{
		apply_ppf(SLES_021_12_PPF, SLES_021_12_PPF_len, bin);
	}
	
	// SaGa Frontier 2 (France)
	else if((strcmp(bootfile, "SLES_021.13") == 0))
	{
		apply_ppf(SLES_021_13_PPF, SLES_021_13_PPF_len, bin);
	}
	
	// SaGa Frontier 2 (Germany)
	else if((strcmp(bootfile, "SLES_021.18") == 0))
	{
		apply_ppf(SLES_021_18_PPF, SLES_021_18_PPF_len, bin);
	}
	
	// Dino Crisis (Europe)
	else if((strcmp(bootfile, "SLES_022.07") == 0))
	{
		apply_ppf(SLES_022_07_PPF, SLES_022_07_PPF_len, bin);
	}
	
	// Dino Crisis (France)
	else if((strcmp(bootfile, "SLES_022.08") == 0))
	{
		apply_ppf(SLES_022_08_PPF, SLES_022_08_PPF_len, bin);
	}
	
	// Dino Crisis (Germany)
	else if((strcmp(bootfile, "SLES_022.09") == 0))
	{
		apply_ppf(SLES_022_09_PPF, SLES_022_09_PPF_len, bin);
	}
	
	// Dino Crisis (Italy)
	else if((strcmp(bootfile, "SLES_022.10") == 0))
	{
		apply_ppf(SLES_022_10_PPF, SLES_022_10_PPF_len, bin);
	}
	
	// Dino Crisis (Spain)
	else if((strcmp(bootfile, "SLES_022.11") == 0))
	{
		apply_ppf(SLES_022_11_PPF, SLES_022_11_PPF_len, bin);
	}
	
	// Premier Manager 2000 (Europe)
	else if((strcmp(bootfile, "SLES_022.92") == 0))
	{
		apply_ppf(SLES_022_92_PPF, SLES_022_92_PPF_len, bin);
	}
	
	// Canal+ Premier Manager (Europe) (Fr,Es,It)
	else if((strcmp(bootfile, "SLES_022.93") == 0))
	{
		apply_ppf(SLES_022_93_PPF, SLES_022_93_PPF_len, bin);
	}

	// Galerians (Europe) (Disc 1)
	else if((strcmp(bootfile, "SLES_023.28") == 0))
	{
		apply_ppf(SLES_023_28_PPF, SLES_023_28_PPF_len, bin);
	}

	// Galerians (France) (Disc 1)
	else if((strcmp(bootfile, "SLES_023.29") == 0))
	{
		apply_ppf(SLES_023_29_PPF, SLES_023_29_PPF_len, bin);
	}

	// Galerians (Germany) (Disc 1)
	else if((strcmp(bootfile, "SLES_023.30") == 0))
	{
		apply_ppf(SLES_023_30_PPF, SLES_023_30_PPF_len, bin);
	}
	
	// Urban Chaos (France)
	else if((strcmp(bootfile, "SLES_023.54") == 0))
	{
		apply_ppf(SLES_023_54_PPF, SLES_023_54_PPF_len, bin);
	}
	
	// Urban Chaos (Germany)
	else if((strcmp(bootfile, "SLES_023.55") == 0))
	{
		apply_ppf(SLES_023_55_PPF, SLES_023_55_PPF_len, bin);
	}
	
	// Michelin Rally Masters: Race of Champions (Europe) (Fr,Es,It)
	else if((strcmp(bootfile, "SLES_023.95") == 0))
	{
		apply_ppf(SLES_023_95_PPF, SLES_023_95_PPF_len, bin);
	}
	
	// PGA European Tour Golf (Europe) (Fr, It, Es) / PGA European Tour Golf (Europe) (Fr, It, Es) (Alt)
	else if((strcmp(bootfile, "SLES_023.96") == 0))
	{
		fseek(bin, 0x9820AB8, SEEK_SET);
		ver_check_val = fgetc(bin);
		fseek(bin, 0, SEEK_SET);
		if(ver_check_val == 0x53)
		{
			printf("PGA European Tour Golf (Europe) (Fr, It, Es)\n");
			apply_ppf(SLES_023_96_PPF, SLES_023_96_PPF_len, bin);
		} else if(ver_check_val == 0xCC) {
			printf("PGA European Tour Golf (Europe) (Fr, It, Es) (Alt)\n");
			apply_ppf(SLES_023_96_ALT_PPF, SLES_023_96_ALT_PPF_len, bin);
		} else {
			printf("Error, Unknown version\n");
			return(1);
		}
	}
	
	// Mánager de Liga (Spain) (Preproduction) 
	else if((strcmp(bootfile, "SLES_024.02") == 0))
	{
		// 00009368 EE F1
		fseek(bin, 0x9368, SEEK_SET);
		ver_check_val = fgetc(bin);
		fseek(bin, 0, SEEK_SET);
		if(ver_check_val == 0xEE) // Final release without LibCrypt protection
		{
			printf("This version of the game does not contain LibCrypt protection, no patching required.\n");
		} else if(ver_check_val == 0xF1) {
			apply_ppf(SLES_024_02_PPF, SLES_024_02_PPF_len, bin);
		}
	}
	
	// Resident Evil 3: Nemesis (Europe)
	else if((strcmp(bootfile, "SLES_025.29") == 0))
	{
		apply_ppf(SLES_025_29_PPF, SLES_025_29_PPF_len, bin);
	}
	
	// Resident Evil 3: Nemesis (France)
	else if((strcmp(bootfile, "SLES_025.30") == 0))
	{
		apply_ppf(SLES_025_30_PPF, SLES_025_30_PPF_len, bin);
	}
	
	// Resident Evil 3: Nemesis (Germany)
	else if((strcmp(bootfile, "SLES_025.31") == 0))
	{
		apply_ppf(SLES_025_31_PPF, SLES_025_31_PPF_len, bin);
	}
	
	// Resident Evil 3: Nemesis (Spain)
	else if((strcmp(bootfile, "SLES_025.32") == 0))
	{
		apply_ppf(SLES_025_32_PPF, SLES_025_32_PPF_len, bin);
	}
	
	// Resident Evil 3: Nemesis (Italy)
	else if((strcmp(bootfile, "SLES_025.33") == 0))
	{
		apply_ppf(SLES_025_33_PPF, SLES_025_33_PPF_len, bin);
	}
	
	// EA Sports Superbike 2000 (Europe) (En,Fr,De,Es,It,Sv)
	else if((strcmp(bootfile, "SLES_025.38") == 0))
	{
		apply_ppf(SLES_025_38_PPF, SLES_025_38_PPF_len, bin);
	}
	
	// Anstoss: Premier Manager (Germany)
	else if((strcmp(bootfile, "SLES_025.63") == 0))
	{
		apply_ppf(SLES_025_63_PPF, SLES_025_63_PPF_len, bin);
	}
	
	// TOCA World Touring Cars (Europe) (En,Fr,De)
	else if((strcmp(bootfile, "SLES_025.72") == 0))
	{
		apply_ppf(SLES_025_72_PPF, SLES_025_72_PPF_len, bin);
	}
	
	// TOCA World Touring Cars (Europe) (Es,It)
	else if((strcmp(bootfile, "SLES_025.73") == 0))
	{
		apply_ppf(SLES_025_73_PPF, SLES_025_73_PPF_len, bin);
	}
	
	// Ronaldo V-Football (En,Fr,De,Es,Nl,Sv) (Europe)
	else if((strcmp(bootfile, "SLES_026.81") == 0))
	{
		apply_ppf(SLES_026_81_PPF, SLES_026_81_PPF_len, bin);
	}
	
	// Theme Park World (Europe) (En,Fr,De,Es,It,Nl,Sv)
	else if((strcmp(bootfile, "SLES_026.88") == 0))
	{
		apply_ppf(SLES_026_88_PPF, SLES_026_88_PPF_len, bin);
	}
	
	// Need for Speed: Porsche 2000 (Europe) (En,De,Sv)
	else if((strcmp(bootfile, "SLES_026.89") == 0))
	{
		apply_ppf(SLES_026_89_PPF, SLES_026_89_PPF_len, bin);
	}
	
	// Resident Evil 3: Nemesis (Ireland)
	else if((strcmp(bootfile, "SLES_026.98") == 0))
	{
		apply_ppf(SLES_026_98_PPF, SLES_026_98_PPF_len, bin);
	}
	
	// Need for Speed: Porsche 2000 (Europe) (Fr,It,Es)
	else if((strcmp(bootfile, "SLES_027.00") == 0))
	{
		apply_ppf(SLES_027_00_PPF, SLES_027_00_PPF_len, bin);
	}
	
	// UEFA Euro 2000 (Europe)
	else if((strcmp(bootfile, "SLES_027.04") == 0))
	{
		apply_ppf(SLES_027_04_PPF, SLES_027_04_PPF_len, bin);
	}
	
	// UEFA Euro 2000 (France)
	else if((strcmp(bootfile, "SLES_027.05") == 0))
	{
		apply_ppf(SLES_027_05_PPF, SLES_027_05_PPF_len, bin);
	}
	
	// UEFA Euro 2000 (Germany)
	else if((strcmp(bootfile, "SLES_027.06") == 0))
	{
		apply_ppf(SLES_027_06_PPF, SLES_027_06_PPF_len, bin);
	}
	
	// UEFA Euro 2000 (Italy)
	else if((strcmp(bootfile, "SLES_027.07") == 0))
	{
		apply_ppf(SLES_027_07_PPF, SLES_027_07_PPF_len, bin);
	}
	
	// UEFA Euro 2000 (Spain)
	else if((strcmp(bootfile, "SLES_027.08") == 0))
	{
		apply_ppf(SLES_027_08_PPF, SLES_027_08_PPF_len, bin);
	}
	
	// F1 2000 (Europe) (En,Fr,De,Nl)
	else if((strcmp(bootfile, "SLES_027.22") == 0))
	{
		apply_ppf(SLES_027_22_PPF, SLES_027_22_PPF_len, bin);
	}
	
	// F1 2000 (Europe) (En,Es,Sv,Da,Fi)
	else if((strcmp(bootfile, "SLES_027.23") == 0))
	{
		apply_ppf(SLES_027_23_PPF, SLES_027_23_PPF_len, bin);
	}
	
	// F1 2000 (Italy)
	else if((strcmp(bootfile, "SLES_027.24") == 0))
	{
		apply_ppf(SLES_027_24_PPF, SLES_027_24_PPF_len, bin);
	}
	
	// Walt Disney World Quest: Magical Racing Tour (Europe) (En,Fr,De,Es,It,Nl,Sv,No,Da)
	else if((strcmp(bootfile, "SLES_027.33") == 0))
	{
		apply_ppf(SLES_027_33_PPF, SLES_027_33_PPF_len, bin);
	}
	
	// Vagrant Story (Europe)
	else if((strcmp(bootfile, "SLES_027.54") == 0))
	{
		apply_ppf(SLES_027_54_PPF, SLES_027_54_PPF_len, bin);
	}
	
	// Vagrant Story (France)
	else if((strcmp(bootfile, "SLES_027.55") == 0))
	{
		apply_ppf(SLES_027_55_PPF, SLES_027_55_PPF_len, bin);
	}
	
	// Vagrant Story (Germany)
	else if((strcmp(bootfile, "SLES_027.56") == 0))
	{
		apply_ppf(SLES_027_56_PPF, SLES_027_56_PPF_len, bin);
	}
	
	// SnoCross Championship Racing (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_027.63") == 0))
	{
		apply_ppf(SLES_027_63_PPF, SLES_027_63_PPF_len, bin);
	}
	
	// Les Cochons de Guerre  (France)
	// TODO: Find Best of Infogrames version and verify it also works (ALT version)
	else if((strcmp(bootfile, "SLES_027.66") == 0))
	{
		apply_ppf(SLES_027_66_PPF, SLES_027_66_PPF_len, bin);
	}
	
	// Frontschweine (Germany)
	else if((strcmp(bootfile, "SLES_027.67") == 0))
	{
		apply_ppf(SLES_027_67_PPF, SLES_027_67_PPF_len, bin);
	}
	
	// Marranos en Guerra (Spain)
	else if((strcmp(bootfile, "SLES_027.68") == 0))
	{
		apply_ppf(SLES_027_68_PPF, SLES_027_68_PPF_len, bin);
	}
	
	// Hogs of War: Nati per Soffritto (Italy)
	else if((strcmp(bootfile, "SLES_027.69") == 0))
	{
		apply_ppf(SLES_027_69_PPF, SLES_027_69_PPF_len, bin);
	}
	
	// RC Revenge (Europe) (En,Fr,De,Es)
	else if((strcmp(bootfile, "SLES_028.24") == 0))
	{
		apply_ppf(SLES_028_24_PPF, SLES_028_24_PPF_len, bin);
	}
	
	// MoHo (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_028.30") == 0))
	{
		apply_ppf(SLES_028_30_PPF, SLES_028_30_PPF_len, bin);
	}
	
	// TechnoMage: Die Rückkehr der Ewigkeit (Germany)
	else if((strcmp(bootfile, "SLES_028.31") == 0))
	{
		apply_ppf(SLES_028_31_PPF, SLES_028_31_PPF_len, bin);
	}
	
	// Mike Tyson Boxing (Europe) (En,Fr,De,Es,It)
	else if((strcmp(bootfile, "SLES_028.39") == 0))
	{
		apply_ppf(SLES_028_39_PPF, SLES_028_39_PPF_len, bin);
	}
	
	// Sydney 2000 (Europe)
	else if((strcmp(bootfile, "SLES_028.57") == 0))
	{
		apply_ppf(SLES_028_57_PPF, SLES_028_57_PPF_len, bin);
	}
	
	// Sydney 2000 (France)
	else if((strcmp(bootfile, "SLES_028.58") == 0))
	{
		apply_ppf(SLES_028_58_PPF, SLES_028_58_PPF_len, bin);
	}
	
	// Sydney 2000 (Germany)
	else if((strcmp(bootfile, "SLES_028.59") == 0))
	{
		apply_ppf(SLES_028_59_PPF, SLES_028_59_PPF_len, bin);
	}
	
	// Sydney 2000 (Italy)
	else if((strcmp(bootfile, "SLES_028.60") == 0))
	{
		apply_ppf(SLES_028_60_PPF, SLES_028_60_PPF_len, bin);
	}
	
	// Sydney 2000 (Spain)
	else if((strcmp(bootfile, "SLES_028.61") == 0))
	{
		apply_ppf(SLES_028_61_PPF, SLES_028_61_PPF_len, bin);
	}
	
	// Sydney 2000 (Australia)
	else if((strcmp(bootfile, "SLES_028.62") == 0))
	{
		apply_ppf(SLES_028_62_PPF, SLES_028_62_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 1) (Europe)
	else if((strcmp(bootfile, "SLES_029.65") == 0))
	{
		apply_ppf(SLES_029_65_PPF, SLES_029_65_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 1) (France)
	else if((strcmp(bootfile, "SLES_029.66") == 0))
	{
		apply_ppf(SLES_029_66_PPF, SLES_029_66_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 1) (Germany)
	else if((strcmp(bootfile, "SLES_029.67") == 0))
	{
		apply_ppf(SLES_029_67_PPF, SLES_029_67_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 1) (Italy)
	else if((strcmp(bootfile, "SLES_029.68") == 0))
	{
		apply_ppf(SLES_029_68_PPF, SLES_029_68_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 1) (Spain)
	else if((strcmp(bootfile, "SLES_029.69") == 0))
	{
		apply_ppf(SLES_029_69_PPF, SLES_029_69_PPF_len, bin);
	}
	
	// Galerians (Europe) (Disc 2)
	else if((strcmp(bootfile, "SLES_123.28") == 0))
	{
		apply_ppf(SLES_123_28_PPF, SLES_123_28_PPF_len, bin);
	}

	// Galerians (France) (Disc 2)
	else if((strcmp(bootfile, "SLES_123.29") == 0))
	{
		apply_ppf(SLES_123_29_PPF, SLES_123_29_PPF_len, bin);
	}

	// Galerians (Germany) (Disc 2)
	else if((strcmp(bootfile, "SLES_123.30") == 0))
	{
		apply_ppf(SLES_123_30_PPF, SLES_123_30_PPF_len, bin);
	}

	// Final Fantasy IX (Disc 2) (Europe)
	else if((strcmp(bootfile, "SLES_129.65") == 0))
	{
		apply_ppf(SLES_129_65_PPF, SLES_129_65_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 2) (France)
	else if((strcmp(bootfile, "SLES_129.66") == 0))
	{
		apply_ppf(SLES_129_66_PPF, SLES_129_66_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 2) (Germany)
	else if((strcmp(bootfile, "SLES_129.67") == 0))
	{
		apply_ppf(SLES_129_67_PPF, SLES_129_67_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 2) (Italy)
	else if((strcmp(bootfile, "SLES_129.68") == 0))
	{
		apply_ppf(SLES_129_68_PPF, SLES_129_68_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 2) (Spain)
	else if((strcmp(bootfile, "SLES_129.69") == 0))
	{
		apply_ppf(SLES_129_69_PPF, SLES_129_69_PPF_len, bin);
	}
	
	// Galerians (Europe) (Disc 2)
	else if((strcmp(bootfile, "SLES_223.28") == 0))
	{
		apply_ppf(SLES_223_28_PPF, SLES_223_28_PPF_len, bin);
	}

	// Galerians (France) (Disc 3)
	else if((strcmp(bootfile, "SLES_223.29") == 0))
	{
		apply_ppf(SLES_223_29_PPF, SLES_223_29_PPF_len, bin);
	}
	
	// Galerians (Germany) (Disc 3)
	else if((strcmp(bootfile, "SLES_223.30") == 0))
	{
		apply_ppf(SLES_223_30_PPF, SLES_223_30_PPF_len, bin);
	}

	// Final Fantasy IX (Disc 3) (Europe)
	else if((strcmp(bootfile, "SLES_229.65") == 0))
	{
		apply_ppf(SLES_229_65_PPF, SLES_229_65_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 3) (France)
	else if((strcmp(bootfile, "SLES_229.66") == 0))
	{
		apply_ppf(SLES_229_66_PPF, SLES_229_66_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 3) (Germany)
	else if((strcmp(bootfile, "SLES_229.67") == 0))
	{
		apply_ppf(SLES_229_67_PPF, SLES_229_67_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 3) (Italy)
	else if((strcmp(bootfile, "SLES_229.68") == 0))
	{
		apply_ppf(SLES_229_68_PPF, SLES_229_68_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 3) (Spain)
	else if((strcmp(bootfile, "SLES_229.69") == 0))
	{
		apply_ppf(SLES_229_69_PPF, SLES_229_69_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 4) (Europe)
	else if((strcmp(bootfile, "SLES_329.65") == 0))
	{
		apply_ppf(SLES_329_65_PPF, SLES_329_65_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 4) (France)
	else if((strcmp(bootfile, "SLES_329.66") == 0))
	{
		apply_ppf(SLES_329_66_PPF, SLES_329_66_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 4) (Germany)
	else if((strcmp(bootfile, "SLES_329.67") == 0))
	{
		apply_ppf(SLES_329_67_PPF, SLES_329_67_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 4) (Italy)
	else if((strcmp(bootfile, "SLES_329.68") == 0))
	{
		apply_ppf(SLES_329_68_PPF, SLES_329_68_PPF_len, bin);
	}
	
	// Final Fantasy IX (Disc 4) (Spain)
	else if((strcmp(bootfile, "SLES_329.69") == 0))
	{
		apply_ppf(SLES_329_69_PPF, SLES_329_69_PPF_len, bin);
	}
	
	// LMA Manager 2001 (Europe) / LMA Manager 2001 (Preproduction) (Europe)
	// TODO: Find Preproduction version
	else if((strcmp(bootfile, "SLES_029.75") == 0))
	{
		apply_ppf(SLES_029_75_PPF, SLES_029_75_PPF_len, bin);
	}
	
	// Roger Lemerre: La Sélection des Champions 2002 (France)
	else if((strcmp(bootfile, "SLES_029.76") == 0))
	{
		apply_ppf(SLES_029_76_PPF, SLES_029_76_PPF_len, bin);
	}
	
	// BDFL Manager 2001 (Germany)
	else if((strcmp(bootfile, "SLES_029.77") == 0))
	{
		apply_ppf(SLES_029_77_PPF, SLES_029_77_PPF_len, bin);
	}
	
	// Football Manager Campionato 2001 (Italy)
	else if((strcmp(bootfile, "SLES_029.78") == 0))
	{
		apply_ppf(SLES_029_78_PPF, SLES_029_78_PPF_len, bin);
	}
	
	// Manager de Liga 2001 (Spain)
	else if((strcmp(bootfile, "SLES_029.79") == 0))
	{
		apply_ppf(SLES_029_79_PPF, SLES_029_79_PPF_len, bin);
	}
	
	// The F.A. Premier League Football Manager 2001 (Europe)
	else if((strcmp(bootfile, "SLES_030.61") == 0))
	{
		apply_ppf(SLES_030_61_PPF, SLES_030_61_PPF_len, bin);
	}
	
	// Fussball Manager 2001 (Germany)
	else if((strcmp(bootfile, "SLES_030.62") == 0))
	{
		apply_ppf(SLES_030_62_PPF, SLES_030_62_PPF_len, bin);
	}
	
	// Disney's 102 Dalmatians: Puppies to the Rescue (Europe) (Fr,De,Es,It,Nl)
	else if((strcmp(bootfile, "SLES_031.89") == 0))
	{
		apply_ppf(SLES_031_89_PPF, SLES_031_89_PPF_len, bin);
	}
	
	// Disney's 102 Dalmatians: Puppies to the Rescue (Scandinavia)
	else if((strcmp(bootfile, "SLES_031.90") == 0))
	{
		apply_ppf(SLES_031_90_PPF, SLES_031_90_PPF_len, bin);
	}
	
	// Disney's 102 Dalmatians: Puppies to the Rescue (Europe) (Fr,De,Es,It,Nl)
	else if((strcmp(bootfile, "SLES_031.91") == 0))
	{
		apply_ppf(SLES_031_91_PPF, SLES_031_91_PPF_len, bin);
	}
	
	// TechnoMage: Return of Eternity (Europe)
	else if((strcmp(bootfile, "SLES_032.41") == 0))
	{
		apply_ppf(SLES_032_41_PPF, SLES_032_41_PPF_len, bin);
	}
	
	// TechnoMage: En Quête de L'Eternité (France)
	else if((strcmp(bootfile, "SLES_032.42") == 0))
	{
		apply_ppf(SLES_032_42_PPF, SLES_032_42_PPF_len, bin);
	}
	
	// TechnoMage: Ritorno all'Eternità (Italy)
	else if((strcmp(bootfile, "SLES_032.43") == 0))
	{
		apply_ppf(SLES_032_43_PPF, SLES_032_43_PPF_len, bin);
	}
	
	// Technomage: El Retorno de la Eternidad (Spain)
	else if((strcmp(bootfile, "SLES_032.44") == 0))
	{
		apply_ppf(SLES_032_44_PPF, SLES_032_44_PPF_len, bin);
	}
	
	// TechnoMage: De Terugkeer der Eeuwigheid (Netherlands)
	else if((strcmp(bootfile, "SLES_032.45") == 0))
	{
		apply_ppf(SLES_032_45_PPF, SLES_032_45_PPF_len, bin);
	}
	
	// Asterix: Mega Madness (Europe) (En,Fr,De,Es,It,Nl)
	else if((strcmp(bootfile, "SLES_033.24") == 0))
	{
		apply_ppf(SLES_033_24_PPF, SLES_033_24_PPF_len, bin);
	}
	
	// Italian Job, The (Europe)
	else if((strcmp(bootfile, "SLES_034.89") == 0))
	{
		apply_ppf(SLES_034_89_PPF, SLES_034_89_PPF_len, bin);
	}
	
	// Men in Black: The Series: Crashdown (Europe)
	else if((strcmp(bootfile, "SLES_035.19") == 0))
	{
		apply_ppf(SLES_035_19_PPF, SLES_035_19_PPF_len, bin);
	}
	
	// Men in Black: The Series: Crashdown (France)
	else if((strcmp(bootfile, "SLES_035.20") == 0))
	{
		apply_ppf(SLES_035_20_PPF, SLES_035_20_PPF_len, bin);
	}
	
	// Men in Black: The Series: Crashdown (Germany)
	else if((strcmp(bootfile, "SLES_035.21") == 0))
	{
		apply_ppf(SLES_035_21_PPF, SLES_035_21_PPF_len, bin);
	}
	
	// Men in Black: The Series: Crashdown (Italy)
	else if((strcmp(bootfile, "SLES_035.22") == 0))
	{
		apply_ppf(SLES_035_22_PPF, SLES_035_22_PPF_len, bin);
	}
	
	// Men in Black: The Series: Crashdown (Spain)
	else if((strcmp(bootfile, "SLES_035.23") == 0))
	{
		apply_ppf(SLES_035_23_PPF, SLES_035_23_PPF_len, bin);
	}
	
	// Lucky Luke: Western Fever (Europe) (En,Fr,De,Es,It,Nl)
	else if((strcmp(bootfile, "SLES_035.30") == 0))
	{
		apply_ppf(SLES_035_30_PPF, SLES_035_30_PPF_len, bin);
	}
	
	// LMA Manager 2002 (Europe)
	else if((strcmp(bootfile, "SLES_036.03") == 0))
	{
		apply_ppf(SLES_036_03_PPF, SLES_036_03_PPF_len, bin);
	}
	
	// Roger Lemerre: La Sélection des Champions 2002 (France)
	else if((strcmp(bootfile, "SLES_036.04") == 0))
	{
		apply_ppf(SLES_036_04_PPF, SLES_036_04_PPF_len, bin);
	}
	
	// BDFL Manager 2002 (Germany)
	else if((strcmp(bootfile, "SLES_036.05") == 0))
	{
		apply_ppf(SLES_036_05_PPF, SLES_036_05_PPF_len, bin);
	}
	
	// Football Manager Campionato 2002 (Italy)
	else if((strcmp(bootfile, "SLES_036.06") == 0))
	{
		apply_ppf(SLES_036_06_PPF, SLES_036_06_PPF_len, bin);
	}
	
	// Manager de Liga 2002 (Spain)
	else if((strcmp(bootfile, "SLES_036.07") == 0))
	{
		apply_ppf(SLES_036_07_PPF, SLES_036_07_PPF_len, bin);
	}
	
	// Italian Job, The (Europe) (Fr,De,Es)
	else if((strcmp(bootfile, "SLES_036.26") == 0))
	{
		apply_ppf(SLES_036_26_PPF, SLES_036_26_PPF_len, bin);
	}
	
	// Italian Job, The (Italy)
	else if((strcmp(bootfile, "SLES_036.48") == 0))
	{
		apply_ppf(SLES_036_48_PPF, SLES_036_48_PPF_len, bin);
	}
	
	// OverBlood 2 (Europe) (Disc 1) (v1.0) / OverBlood 2 (Europe) (Disc 2) (v1.0) /OverBlood 2 (Europe) (Disc 1) (v1.1) / OverBlood 2 (Europe) (Disc 2) (v1.1)
	else if(
	((strcmp(bootfile, "SLES_018.79") == 0)) // Disc 1
	|| 	((strcmp(bootfile, "SLES_118.79") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X18_79_PPF, SLES_X18_79_PPF_len, bin);
	}
	
	// OverBlood 2 (Italy)
	else if(
	((strcmp(bootfile, "SLES_018.80") == 0))
	|| ((strcmp(bootfile, "SLES_118.80") == 0))
	)
	{
		apply_ppf(SLES_X18_80_PPF, SLES_X18_80_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 1) (France)
	else if((strcmp(bootfile, "SLES_020.81") == 0))
	{
		apply_ppf(SLES_020_81_PPF, SLES_020_81_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 1) (Germany) / Final Fantasy VIII (Disc 2) (Germany) / Final Fantasy VIII (Disc 3) (Germany) / Final Fantasy VIII (Disc 4) (Germany)
	else if(
	((strcmp(bootfile, "SLES_020.82") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_120.82") == 0)) // Disc 2
	|| ((strcmp(bootfile, "SLES_220.82") == 0)) // Disc 3
	|| ((strcmp(bootfile, "SLES_320.82") == 0)) // Disc 4
	)
	{
		apply_ppf(SLES_X20_82_PPF, SLES_X20_82_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 1) (Italy) / Final Fantasy VIII (Disc 2) (Italy) / Final Fantasy VIII (Disc 3) (Italy) / Final Fantasy VIII (Disc 4) (Italy)
	else if(
	((strcmp(bootfile, "SLES_020.83") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_120.83") == 0)) // Disc 2
	|| ((strcmp(bootfile, "SLES_220.83") == 0)) // Disc 3
	|| ((strcmp(bootfile, "SLES_320.83") == 0)) // Disc 4
	)
	{
		apply_ppf(SLES_X20_83_PPF, SLES_X20_83_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 1) (Spain)
	else if(((strcmp(bootfile, "SLES_020.84") == 0)))
	{
		apply_ppf(SLES_020_84_PPF, SLES_020_84_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 2) (Spain)
	else if(((strcmp(bootfile, "SLES_120.84") == 0)))
	{
		apply_ppf(SLES_120_84_PPF, SLES_120_84_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 3) (Spain)
	else if(((strcmp(bootfile, "SLES_220.84") == 0)))
	{
		apply_ppf(SLES_220_84_PPF, SLES_220_84_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 4) (Spain)
	else if(((strcmp(bootfile, "SLES_320.84") == 0)))
	{
		apply_ppf(SLES_320_84_PPF, SLES_320_84_PPF_len, bin);
	}
	
	// Parasite Eve II (Europe)
	else if(
	((strcmp(bootfile, "SLES_025.58") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_125.58") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X25_58_PPF, SLES_X25_58_PPF_len, bin);
	}
	
	// Parasite Eve II (France)
	else if(
	((strcmp(bootfile, "SLES_025.59") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_125.59") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X25_59_PPF, SLES_X25_59_PPF_len, bin);
	}
	
	// Parasite Eve II (Germany)
	else if(
	((strcmp(bootfile, "SLES_025.60") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_125.60") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X25_60_PPF, SLES_X25_60_PPF_len, bin);
	}
	
	// Parasite Eve II (Spain)
	else if(
	((strcmp(bootfile, "SLES_025.61") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_125.61") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X25_61_PPF, SLES_X25_61_PPF_len, bin);
	}
	
	// Parasite Eve II (Italy)
	else if(
	((strcmp(bootfile, "SLES_025.62") == 0)) // Disc 1
	|| ((strcmp(bootfile, "SLES_125.62") == 0)) // Disc 2
	)
	{
		apply_ppf(SLES_X25_62_PPF, SLES_X25_62_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 2) (Europe)
	else if((strcmp(bootfile, "SLES_120.80") == 0))
	{
		apply_ppf(SLES_120_80_PPF, SLES_120_80_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 3) (Europe)
	else if((strcmp(bootfile, "SLES_220.80") == 0))
	{
		apply_ppf(SLES_220_80_PPF, SLES_220_80_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 4) (Europe)
	else if((strcmp(bootfile, "SLES_320.80") == 0))
	{
		apply_ppf(SLES_320_80_PPF, SLES_320_80_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 2) (France)
	else if((strcmp(bootfile, "SLES_120.81") == 0))
	{
		apply_ppf(SLES_120_81_PPF, SLES_120_81_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 3) (France)
	else if((strcmp(bootfile, "SLES_220.81") == 0))
	{
		apply_ppf(SLES_220_81_PPF, SLES_220_81_PPF_len, bin);
	}
	
	// Final Fantasy VIII (Disc 4) (France)
	else if((strcmp(bootfile, "SLES_320.81") == 0))
	{
		apply_ppf(SLES_320_81_PPF, SLES_320_81_PPF_len, bin);
	}

	else {
		printf("Unknown game\n");
	    fclose(bin);
		do_exit(1);
	}

    fclose(bin);
	do_exit(0);
}
