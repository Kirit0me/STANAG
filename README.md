# STANAG

Using NATO standard for AEP84, Message manager for UCS and AFCC to communicate with a LOI 5.

Messages Included : Heartbeat(#16002), Acknowledgement(#17000), CUCS(#1), Vehicle_ID(#3).

Validation steps : CRC check(done), SRC_ID and DEST_ID match(to be done), MM_DOC_VER match(to be done).

UDP socket used.

Heartbeat is periodically sent in another thread. 
On KeyboardInterrupt, UCS sends a CUCS which asks information about the AFCC and AFCC replies with Vehicle_ID message.

To be implemented : 

1. Look Up Table(database) for message numbers.

2. Being able to add messages into a database.

3. UI design

# How to Use : 

1. Run ucs_build.bat and afcc_build.bat

2. run ucs.exe and afcc.exe in two different terminals.
