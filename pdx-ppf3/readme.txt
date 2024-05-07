PPF (Playstation Patch File) Version 3.0 Full Distribution
----------------------------------------------------------


About PPF3.0 and the intention for this upgrade
-----------------------------------------------

PPF3.0 is a further development of PPF2.0 which is still used to distribute
(mainly) Playstation patches. Actually, as I stated in the PPF2.0 readme, I
did not want to develop further versions of this tool.  So, to avoid confusion 
let me explain why I did so:

PPF2.0 is restricted compatible with large files (for example DVD-Images >3GB).
Since DVD releases are being spread more and more and since DVD-Burners have
become more affordable I thought it is better to develop a new PPF version
NOW than not have such a tool when it is needed. Also, I have included
several new features which are useful for other platforms than Playstation 1&2. 

For now I think DVD images are not interesting for most of the people in the
scene but I think this will change in the near future and I also think it will
be neccessary to have a running system for patches which covers DVD stuff also.


Whats new ?
-----------

As stated above I added some new features to PPF but also i removed some of the
old ones, or to be exact one feature had to go. So here is the new feature-list:

- Image filesize checking is GONE.
  Over the years it became clear that this feature was simply too inaccurate
  and confused most of the users with warnings which were mostly incorrect.

- Image Identification / Blockcheck was enlarged. (+Imagetype)
  You can either fully DISABLE or ENABLE the blockcheck for a specific patch,
  plus the blockcheck now supports GI (PrimoDVD) images. GI is the common format
  used for distributing DVD-Images.

- Undo data.
  You can optionally choose to include UNDO data in your patch so the users can
  undo your patch anytime.

- Fast
  Its a hell of alot faster than PPF2.0.
  
- Large files support.
  PPF3.0 supports files up to 9,223,372,036,854,775,807 Byte. enough ?

Contents of the distribution
----------------------------

1. If you take a look into the folder "PPFBIN" you will find the commandline
   tools for both Make and ApplyPPF3. The commandline tools are precompiled for
   several platforms which includes: Windows, Linux and MAC OS-X.
   You will find descriptions of these tools in the respective directory.

2. Developers may want to take a look into the folder "PPFDEV". Here you can find
   everything which is needed to start doing your own PPF3.0 tools. ANSI-C source
   for Make aswell as Apply PPF3.0 is included. Furthermore you will find a file
   called "PPF3.txt" which is needed to study the new PPF3.0 file structure.

3. Also for everyone who is not able to use commandline tools for applying I have
   included version 3.0 of the wellknown PPF-O-MATIC program in the folder PPFGUI
   You will find that there is a WINDOWS as well as a "MAC" version included. The
   Windows version is fully compatible with Windows 9x/NT/2K/XP of course.
   The MAC version is for OS-X.


Last notes / thanks.
--------------------

I hope you like the idea which comes along with PPF3.0 as well as the new
features I have included in this version. Not everything was coded by me so
here's a list of tools which were coded by others or were influenced by them:

- PPF-O-MATIC v3.0 MAX OS-X Version was coded by Hu Kares.
- PPF-O-MATIC v3.0 WIindows Version was coded by Zweifeld/Paradox.

I want to say 'thank you' to the following people: Everyone that used PPF tools
so far, this includes mainly big groups like Kalisto, Paradox, Static, Lightforce
and Echelon (Dreamcast). Also, all the independent people who chose the PPF
format to distribute patches. Hu Kares for his fine MAC conversions and fixing
some bugs in the PPF3.0 draft and of course Zweifeld, for PPF-O-MATIC3 Windows.
Last but not least, Scal/Paradox for correcting my miserable
english docs ;-)

Thank you.

Signed,
Icarus of Paradox
