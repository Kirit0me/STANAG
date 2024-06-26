# STANAG

Using NATO standard for AEP84, Message manager for UCS and AFCC to communicate with a LOI 5.

Messages Included : Heartbeat(#16002), Acknowledgement(#17000).

Validation steps : CRC check, SRC_ID and DEST_ID match, MM_DOC_VER match.

UDP socket used.

To be implemented : 

1. Look Up Table

2. #1 and #3 Messages.