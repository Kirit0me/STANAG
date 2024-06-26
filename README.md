# STANAG

Using NATO standard for AEP84, Message manager for UCS and AFCC to communicate with a LOI 5.

Messages Included : Heartbeat(#16002), Acknowledgement(#17000).

Validation steps : CRC check(done), SRC_ID and DEST_ID match(to be done), MM_DOC_VER match(to be done).

UDP socket used.

To be implemented : 

1. Look Up Table for message numbers.

2. #1 and #3 Messages.

3. Periodically sending #16002 in another thread.

# How to Use : 

1. Run ucs_build.bat and afcc_build.bat

2. run ucs.exe and afcc.exe in two different terminals.