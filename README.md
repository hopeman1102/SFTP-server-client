# SFTP-in-C
This project implements the Simple File Transfer Protocol described in RFC 913 (https://tools.ietf.org/html/rfc913). This is completed in C, using Port 8080 as the default.

**NOTE: When the client and server are running, the DONE command breaks the connection between the client and server. It also shuts down the client. The server keeps running and looks for new connections. As the server keeps running, a new client can connect to the server and utilise it. Upto 5 clients can form a queue and connect based on turn. If one client uses the DONE command, the next client in the queue would connect.** 

**NOTE: Please close the client using the DONE command and not ctrl+c.** 

## File Structure
```
-hsin849
	-client
		-transfer_files
		client
		client.c
		global.g
		Makefile
	-server
		-recieved_files
		sqlite
		database.c
		database.h
		global.h
		Makefile
		server
		server.c
		sftp.c
		sftp.h
		user.db
	-tests
		-transfer_files
		kill_and_name.sh
		list_test.sh
		retr.sh
		stor.sh
		test_output.txt
		test.sh (main testing file)
		type_test.sh
	.gitignore
	Makefile
	README.md
```

## Client Commands
Provided below is the list of commands provided in the RFC913 Protocol.
```
<command> : = <cmd> [<SPACE> <args>] <NULL>

<cmd> : =  USER ! ACCT ! PASS ! TYPE ! LIST ! CDIR ! KILL ! NAME ! DONE ! RETR ! STOR

<response> : = <response-code> [<message>] <NULL>

<response-code> : =  + | - |   | !
```

In this implementation, the commands are NOT case sensitive. Therefore, both 'USER user1' and 'user user1' will yield the same result. To send a command, simply enterthe command on the Client console, followed by the ENTER key. **Types are NOT case sensitive. For example, the command 'stor new temp.txt' will not work. The type NEW has to be in capital like: 'stor NEW temp.txt'**.

If the cmd sent is not listed above, the server will respond with: ``'-Invalid Command'``

All commands except USER, ACCT and PASS require the Client to be authenticated. If a command is entered that required authentication and the client is not logged in, the server will respond with:'Please login first to use this command.'

## User Details
|   id    |  acct |  pass  |
| ------- |:-----:| ------:|
| user1   |u1acc1 |pass123 |
| user2   |u2acc1 |pass123 |
| user2   |u2acc2 |pass123 |
| user3   |       |        |
| user4   |u2acc1 |        |
| user5   |       |pass123 |

#### user3 is the superior user i.e. provides easiest login processUser Details are stored in an SQLite Database, and fetched when required from within the ServerConnection. If further testing is required, simply open the users.db file in the server folder using a Database Browser and add in additional users. 

## Instruction
#### NOTE: Must Run these tests BEFORE manually interacting between the server and client

#### NOTE-2: These tests have been tested for LINUX only. Windows and Linux appear to have different file seperators and C compilers, so it has only been run on LINUX. Test functionality on LINUX!

#### NOTE-3: The tests run all the commands automatically. It makes the client send commands to the server and prints what the server replied with. It formats the output so it's easy to read.

#### NOTE-4: The server has to be started in a different terminal before running the tests as specified below.

#### NOTE-5: The compiled executables for the client and server are already present in the client and server folders. To compile again, simply run make in the root directory. This will run the Makefile.

### To test the project:
1) To test the project, first start the server program -> ``cd server`` -> ``./server``.
2) In a different terminal shell, go to the tests directory -> ``cd tests``.
3) Run the test.sh shell file -> ``./test.sh``.
4) This should run all the specified tests for SFTP.

### To start the project:
1) Start two different terminal sessions.
2) The project runs on port:8080, make sure it is free and available to use.
3) Move to the server directory and run ./server in one of the terminals.
	(cd server && ./server)
4) In the other terminal session, run ./client from the client directory.
	(cd client && ./client)
5) Once the client and server connection is estalised, commands can be entered from the client.

## TEST OUTPUT
<pre>

-------------DATABASE-------------
 ---id----|---acc----|---pass---- 
 --user1--|--u1acc1--|--pass123-- 
 --user2--|--u2acc1--|--pass123-- 
 --user2--|--u2acc2--|--pass123-- 
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

<b style="color:green">Specifying subdir while using LIST with V mode</b>
<i style="color:green">Commands from client:</i>
USER user3
LIST V dir1
DONE
<i style="color:green">Message from Server:</i>
+CS725 SFTP Service
!user3 logged in
+PS
dir2.txt	23 bytes
temp2.txt	23 bytes

✔ Files in directory dir1 listed with V mode

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
