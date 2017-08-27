Build Instruction
=================

VLCQt
-----

You need to have VLCQt librairies installed on your system.
This application needs 2 librairies :
  - libVLCQtCore
  - libVLCQtWidgets

If you don't have VLCQt fallow the instructions in the file qtvlc/vlc-qt-master/BUILDING.md

Foscam CGI Interface
--------------------

In the .pro file, modify the following line to add the path to VLCQt librairies :

LIBS += -L/usr/local/lib/

then type :

qmake
make
