# SFTP-in-C
Simple File Transfer Protocol implementation in C

## TEST OUTPUT
<pre>

-------------DATABASE-------------
 ---id----|---acc----|---pass---- 
 --user1--|--u1acc1--|--pass123-- 
 --user2--|--u2acc1--|--pass123-- 
 --user2--|--u2acc1--|--pass123-- 
 --user3--|---NULL---|---NULL---- 
 --user4--|--u4acc1--|---NULL---- 
 --user5--|---NULL---|--pass123-- 
----------------------------------

<b>Run the server in a different terminal from server directory before this test.</b>
<b>From the main directory -> cd server -> ./server</b>

<b>Each test is ending with the DONE command so it is already being tested. The DONE command closes the connection between the client and server.
It also closes the client application but keeps the server running so new clients can connect and utilise the service.

To close the server, go to the terminal running the server and press ctrl+c. The client can be closed with the DONE command.</b>

<b style="color:green">Full Login</b>
<i style="color:green">Commands from client:</i>
USER user1
ACCT u1acc1
PASS pass123
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
+user1 ok, send account and password
+user1 account verified, send password
!user1 logged in
<b style="color:green">✔ Login Tested</b>

<b style="color:green">Login when PASS and ACCT not required</b>
<i style="color:green">Commands from client:</i>
USER user3
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
<b style="color:green">✔ Login without PASS and ACCT Tested</b>

<b style="color:green">Login when PASS not required</b>
<i style="color:green">Commands from client:</i>
USER user4
ACCT u4acc1
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
+user4 ok, send account, password not required
!user4 logged in
<b style="color:green">✔ Login without PASS Tested</b>

<b style="color:green">Login when ACCT not required</b>
<i style="color:green">Commands from client:</i>
USER user5
PASS pass123
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
+user5 ok, send password, account not required
!user5 logged in
<b style="color:green">✔ Login without ACCT Tested</b>

<b style="color:green">Trying to run a command without logging in</b>
<i style="color:green">Commands from client:</i>
TYPE A
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
-Permission denied, not logged in
<b style="color:green">✔ Cannot access without logging in</b>

<b style="color:green">Changing type to ASCII</b>
<i style="color:green">Commands from client:</i>
USER user3
TYPE A
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+Using Ascii mode
<b style="color:green">✔ Cannot access without logging in</b>

<b style="color:green">Changing type to BINARY</b>
<i style="color:green">Commands from client:</i>
USER user3
TYPE B
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+Using Binary mode
<b style="color:green">✔ Cannot access without logging in</b>

<b style="color:green">Changing type to CONTINUOUS</b>
<i style="color:green">Commands from client:</i>
USER user3
TYPE C
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+Using Continuous mode
<b style="color:green">✔ Cannot access without logging in</b>

<b style="color:green">LISTING all files using LIST with F mode</b>
<i style="color:green">Commands from client:</i>
USER user3
LIST F
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+PS
rick.txt
hell0.txt
_temp1.txt
dir2
renameMe.txt
topdawg.txt
dir1
deleteThis.txt
temp1.txt

<b style="color:green">✔ Files in current directory listed with F mode</b>

<b style="color:green">LISTING all files using LIST with V mode</b>
<i style="color:green">Commands from client:</i>
USER user3
LIST V
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+PS
rick.txt	39 bytes
hell0.txt	47 bytes
_temp1.txt	44 bytes
dir2	64 bytes
renameMe.txt	0 bytes
topdawg.txt	0 bytes
dir1	128 bytes
deleteThis.txt	0 bytes
temp1.txt	512 bytes

<b style="color:green">✔ Files in current directory listed with V mode</b>

<b style="color:green">Changing directory using CDIR and listing</b>
<i style="color:green">Commands from client:</i>
USER user3
CDIR dir1
LIST V
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
!Changed working dir to dir1
+PS
dir2.txt	23 bytes
temp2.txt	23 bytes

<b style="color:green">✔ Files in changed directory listed</b>

<b style="color:green">Trying to KILL while specifying wrong name</b>
<i style="color:green">Commands from client:</i>
USER user3
KILL wrongname.txt
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
-file not deleted
<b style="color:green">✔ File not deleted as it does not exist</b>

<b style="color:green">Trying to KILL while specifying correct name</b>
<i style="color:green">Commands from client:</i>
USER user3
KILL deleteThis.txt
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+deleteThis.txt deleted
<b style="color:green">✔ deleteThis.txt file deleted in the server</b>

<b style="color:green">Trying to rename with NAME while specifying wrong file name</b>
<i style="color:green">Commands from client:</i>
USER user3
NAME wrongRenameMe.txt
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
-Can't find wrongRenameMe.txt
<b style="color:green">✔ Cannot rename using NAME as specified filename is wrong</b>

<b style="color:green">Trying to rename with NAME while specifying correct file name</b>
<i style="color:green">Commands from client:</i>
USER user3
NAME renameMe.txt
TOBE renamed.txt
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+File exists
+renameMe.txt renamed to renamed.txt
✔ File renameMe.txt renamed to renamed.txt.
<b style="color:green">This can be confirmed by checking the recieved_files directory in the sever folder.</b>

<b style="color:green">Trying to use RETR command when no file is specified</b>
<i style="color:green">Commands from client:</i>
USER user3
RETR
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
err: not enough arguments, please specify file name
<b style="color:green">✔ Could not use RETR with incomplete arguments</b>

<b style="color:green">Trying to use RETR when non existing file is specified</b>
<i style="color:green">Commands from client:</i>
USER user3
RETR abc
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
-File doesn't exist
<b style="color:green">✔ RETR does not work when file does not exist in the server</b>

<b style="color:green">Trying to use RETR with correct filename</b>
<i style="color:green">Commands from client:</i>
USER user3
RETR temp1.txt
SEND
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
512
<b style="color:green">✔ File successfully transferred from server to client.
This can be confirmed by observing the transfer_files folder in the tests folder.
It can be seen the file transferred is 512 bytes.</b>

<b style="color:green">Aborting using STOP</b>
<i style="color:green">Commands from client:</i>
USER user3
RETR temp1.txt
STOP
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
512
+ok, RETR aborted
<b style="color:green">✔ Successfully aborted using STOP.</b>

<b style="color:green">Trying to use STOR without complete arguments</b>
<i style="color:green">Commands from client:</i>
USER user3
STOR
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
err: not enough arguments, please specify file name
<b style="color:green">✔ Could not use STOR with incomplete arguments</b>

<b style="color:green">Using STOR with NEW command</b>
<i style="color:green">Commands from client:</i>
USER user3
STOR NEW stor_test.txt
SIZE 100
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+File does not exist, will create new file
+ok, waiting for file
+Saved stor_test.txt
<b style="color:green">✔ File successfully transferred.
To verify check recieved_files folder in server folder.</b>

<b style="color:green">Using STOR with OLD command</b>
<i style="color:green">Commands from client:</i>
USER user3
STOR OLD stor_test.txt
SIZE 100
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+Will write over old file
+ok, waiting for file
+Saved stor_test.txt
<b style="color:green">✔ File successfully transferred.
To verify check recieved_files folder in server folder.</b>

<b style="color:green">Using STOR with APP command</b>
<i style="color:green">Commands from client:</i>
USER user3
STOR APP stor_test.txt
SIZE 100
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+Will append to file
+ok, waiting for file
+Saved stor_test.txt
<b style="color:green">✔ File successfully transferred.
To verify check recieved_files folder in server folder.</b>
</pre>