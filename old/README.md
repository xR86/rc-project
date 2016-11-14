# rcproj
Computer Networks Project - MyDownloader

[![GitHub license](http://img.shields.io/badge/license-GPLv3-blue.svg)](https://github.com/xR86/rc-project)
  
-> description from [/~adria/teach/courses/net/](http://profs.info.uaic.ro/~adria/teach/courses/net/ProiecteNet2015.php):
> MyDownloader (B)
>> Sa se implementeze o aplicatie ce descarca toate fisierele existente la un url specificat. Aplicatia va permite mentionarea gradului de adancime pana la care se va realiza descarcarea. De asemenea, daca va fi specificata o pagina web, sa se descarce toate fisierele specificate in link-urile de pe acea pagina.

-> translation:
> MyDownloader (B)
>> Implement an application that will download all files existent at a specified URL. The application will allow to set the depth to which the download will go. Also, if an webpage is specified, download all specified files from the links of that page.

### :squirrel: Updates:
+ commits starting august 2016 will refactor the app
+ aimed functionality is of a server-client combination with secure transfer of downloaded materials (exchange of public keys, and exchange of encrypted messages afterwards - client sends a URL, server downloads it with specified parameteres, encrypts the files and sends them back - prefferably in a .tar.gz or .zip format)

### :exclamation: Notes:
+ issues remaining: client receives bad server public key (through send_PK() & accept_PK()), and sending of encrypted files :x:
+ code is still a bit tangled :spaghetti:
+ the commits on __11.04.2016__ are only for cleanup, no functional change were made
+ application usage is described in the "**functionare**" file :paperclip:
+ `/server/core` was used for **gdb** debugging (core dump file)

### FEATURES:
- **color coding messages** (error - red , additional - turqoise, normal - white) -> useful for debugging (especially during verbose mode) :ledger:
- **openssl rsa encryption** - public key exchange between server and client, encrypted messages afterwards :closed_lock_with_key:
- *to be added* ... :bulb:

![Demo screenshot](/screenshot-rc.png?raw=true "during execution")
